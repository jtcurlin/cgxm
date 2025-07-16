// src/containers/deque.cpp
// thread safe deque

#pragma once
#include <cstdint>
#include <atomic>

#define DEFAULT_DEQUE_LOG_SIZE 8

namespace cgxm
{
template<typename T>
class ts_deque
{
    struct Array
    {
        int64_t         m_capacity;
        int64_t         m_mask;
        std::atomic<T>* m_data;

        explicit Array(int64_t capacity)
            : m_capacity{capacity}
            , m_mask{capacity - 1}
            , m_data{new std::atomic<T>[static_cast<size_t>(m_capacity)]} {}

        ~Array()
        {
            delete[] m_data;
        }

        int64_t capacity() const noexcept
        {
            return m_capacity;
        }

        void push(int64_t i, T o) noexcept
        {
            m_data[i & m_mask].store(o, std::memory_order_relaxed);
        }

        T pop(int64_t i) noexcept
        {
            return m_data[i & m_mask].load(std::memory_order_relaxed);
        }

        Array* resize(int64_t btm, int64_t top)
        {
            Array* ptr = new Array{2 * m_capacity};
            for (int64_t i = top ; i != btm ; ++i) {
                ptr->push(i, pop(i));
            }
            return ptr;
        }
    };

public:
    explicit ts_deque(int64_t log_size = DEFAULT_DEQUE_LOG_SIZE);
    ~ts_deque();

    bool    empty() const noexcept;
    size_t  size() const noexcept;
    int64_t capacity() const noexcept;

    void push(T o);
    T    pop();
    T    steal();

private:
    Array* resize_array(Array* array, int64_t btm, int64_t top);

    std::atomic<int64_t> m_top;
    std::atomic<int64_t> m_btm;
    std::atomic<Array*>  m_array;
    std::vector<Array*>  m_garbage;
};

template<typename T>
ts_deque<T>::ts_deque(int64_t log_size)
{
    m_top.store(0, std::memory_order_relaxed);
    m_btm.store(0, std::memory_order_relaxed);
    m_array.store(new Array{(int64_t{1} << log_size)}, std::memory_order_relaxed);
    m_garbage.reserve(32);
}

template<typename T>
ts_deque<T>::~ts_deque()
{
    for (auto a : m_garbage) {
        delete a;
    }
    delete m_array.load();
}

template<typename T>
bool ts_deque<T>::empty() const noexcept
{
    int64_t top = m_top.load(std::memory_order_relaxed);
    int64_t btm = m_btm.load(std::memory_order_relaxed);
    return (btm <= top);
}

template <typename T>
size_t ts_deque<T>::size() const noexcept
{
    int64_t top = m_top.load(std::memory_order_relaxed);
    int64_t btm = m_btm.load(std::memory_order_relaxed);
    return static_cast<size_t>(btm >= top ? btm - top : 0);
}

template <typename T>
void ts_deque<T>::push(T o)
{
    int64_t btm   = m_btm.load(std::memory_order_relaxed);
    int64_t top   = m_top.load(std::memory_order_acquire);
    Array*  array = m_array.load(std::memory_order_relaxed);

    if (array->capacity() - 1 < (btm - top)) {
        array = resize_array(array, btm, top);
    }

    array->push(btm, o);
    std::atomic_thread_fence(std::memory_order_release);

    m_btm.store(btm + 1, std::memory_order_release);
}

template <typename T>
T ts_deque<T>::pop()
{
    int64_t btm   = m_btm.load(std::memory_order_relaxed) - 1;
    Array*  array = m_array.load(std::memory_order_relaxed);
    m_btm.store(btm, std::memory_order_relaxed);
    std::atomic_thread_fence(std::memory_order_seq_cst);
    int64_t top = m_top.load(std::memory_order_relaxed);

    T item{nullptr};

    if (top <= btm) {
        item = array->pop(btm);
        if (top == btm) {
            // last item just got stolen
            if (!m_top.compare_exchange_strong(top, top + 1,
                std::memory_order_seq_cst, std::memory_order_relaxed)) {
                item = nullptr;
            }
            m_btm.store(btm+1, std::memory_order_relaxed);
        }
    }
    else {
        m_btm.store(btm+1, std::memory_order_relaxed);
    }

    return item;
}

template <typename T>
T ts_deque<T>::steal()
{
    int64_t top = m_top.load(std::memory_order_acquire);
    std::atomic_thread_fence(std::memory_order_seq_cst);
    int64_t btm = m_btm.load(std::memory_order_acquire);

    T item {nullptr};

    if (top < btm) {
        Array* array = m_array.load(std::memory_order_consume);
        item = array->pop(top);
        if (!m_top.compare_exchange_strong(top, top+1, std::memory_order_seq_cst, std::memory_order_relaxed)) {
            return nullptr;
        }
    }

    return item;
}

template <typename T>
int64_t ts_deque<T>::capacity() const noexcept
{
    return m_array.load(std::memory_order_relaxed)->capacity();
}

template <typename T>
ts_deque<T>::Array* ts_deque<T>::resize_array(Array* array, int64_t btm, int64_t top)
{
    Array* tmp = array->resize(btm, top);
    m_garbage.push_back(array);
    m_array.store(tmp, std::memory_order_release);
    return tmp;
}

} // namespace cgxm
