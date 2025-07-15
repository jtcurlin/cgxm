// tests/deque_tests.cpp

#include <gtest/gtest.h>
#include "container/deque.hpp"

class DequeTest : public ::testing::Test
{
protected:
    void SetUp() override
    {
        deque = new cgxm::Deque<int*>(3);
    }

    void TearDown() override
    {
        delete deque;
    }

    cgxm::Deque<int*>* deque;
};

TEST_F(DequeTest, IsInitiallyEmpty)
{
    ASSERT_TRUE(deque->empty());
    ASSERT_EQ(deque->size(), 0);
}

TEST_F(DequeTest, PushIncreasesSize)
{
    int x = 10;
    deque->push(&x);
    ASSERT_FALSE(deque->empty());
    ASSERT_EQ(deque->size(), 1);
}

TEST_F(DequeTest, PopDecreasesSize)
{
    int x = 10;
    deque->push(&x);

    int* val = deque->pop();

    ASSERT_EQ(val, &x);
    ASSERT_TRUE(deque->empty());
    ASSERT_EQ(deque->size(), 0);
}

TEST_F(DequeTest, PopFollowsLIFO)
{
    int x = 10, y = 20, z = 30;
    deque->push(&x);
    deque->push(&y);
    deque->push(&z);

    ASSERT_EQ(deque->pop(), &z);
    ASSERT_EQ(deque->pop(), &y);
    ASSERT_EQ(deque->pop(), &x);
    ASSERT_TRUE(deque->empty());
}

TEST_F(DequeTest, PopFromEmptyReturnsNull)
{
    ASSERT_TRUE(deque->empty());
    int* val = deque->pop();
    ASSERT_EQ(val, nullptr);
}

TEST_F(DequeTest, StealFromEmptyReturnsNull)
{
    ASSERT_TRUE(deque->empty());
    int* val = deque->steal();
    ASSERT_EQ(val, nullptr);
}

TEST_F(DequeTest, StealFollowsFIFO) {
    int x = 10, y = 20, z = 30;
    deque->push(&x);
    deque->push(&y);
    deque->push(&z);

    ASSERT_EQ(deque->size(), 3);
    ASSERT_EQ(deque->steal(), &x);
    ASSERT_EQ(deque->size(), 2);
    ASSERT_EQ(deque->steal(), &y);
    ASSERT_EQ(deque->size(), 1);
}

TEST_F(DequeTest, ResizeOnOverflow)
{
    ASSERT_EQ(deque->capacity(), 8);
    int values[9];
    for (int i=0; i<9; ++i) {
        values[i] = i;
        deque->push(&values[i]);
    }

    // resize should have happened during 9th push

    ASSERT_EQ(deque->size(), 9);
    ASSERT_EQ(deque->capacity(), 16); // capacity should have doubled

    for (int i=8; i>=0; --i) {
        ASSERT_EQ(deque->pop(), &values[i]);
    }
    ASSERT_TRUE(deque->empty());
}