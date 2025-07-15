cmake_minimum_required(VERSION 3.20)
project(cgxm)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra -Werror")

enable_language(OBJCXX)

# quill
add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/external/quill)
add_compile_definitions(-DQUILL_DISABLE_NON_PREFIXED_MACROS)

# google test
include(FetchContent)
FetchContent_Declare(
    googletest
    GIT_REPOSITORY https://github.com/google/googletest.git
    GIT_TAG "v1.17.0"
)
FetchContent_MakeAvailable(googletest)

# find_library(APPLE_FWK_FOUNDATION Foundation REQUIRED)
# find_library(APPLE_FWK_APPKIT AppKit REQUIRED)
# find_library(APPLE_FWK_QUARTZ_CORE QuartzCore REQUIRED)
# find_library(APPLE_FWK_METAL Metal REQUIRED)

add_library(cgxm STATIC)

# set_target_properties(cgxm PROPERTIES 
#   MACOSX_BUNDLE_INFO_PLIST "${CMAKE_CURRENT_SOURCE_DIR}/Info.plist"
# )

target_sources(cgxm PRIVATE 
    ${CMAKE_CURRENT_SOURCE_DIR}/src/core/engine.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/core/log.cpp
)

target_include_directories(cgxm
  PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/include
  PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/src
)

target_link_libraries(cgxm PRIVATE quill::quill)

# target_link_libraries(cgxm
#   ${APPLE_FWK_FOUNDATION}
#   ${APPLE_FWK_QUARTZ_CORE}
#   ${APPLE_FWK_METAL}
#   ${APPLE_FWK_APPKIT}
# )

# ?? add_executable(cgxm-editor MACOSX_BUNDLE src/editor/main.cpp)

enable_testing()

add_executable(deque_test ${CMAKE_CURRENT_SOURCE_DIR}/tests/deque_test.cpp)
target_include_directories(deque_test PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/src)
target_link_libraries(deque_test PRIVATE cgxm GTest::gtest_main)

include (GoogleTest)
gtest_discover_tests(deque_test)
add_subdirectory(examples)
