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

# macos libs
find_library(APPLE_FWK_FOUNDATION Foundation REQUIRED)
find_library(APPLE_FWK_APPKIT AppKit REQUIRED)
find_library(APPLE_FWK_QUARTZ_CORE QuartzCore REQUIRED)
find_library(APPLE_FWK_METAL Metal REQUIRED)

add_library(cgxm STATIC)

# set_target_properties(cgxm PROPERTIES
#   MACOSX_BUNDLE_INFO_PLIST "${CMAKE_CURRENT_SOURCE_DIR}/Info.plist"
# )

set_source_files_properties(
        # ${CMAKE_CURRENT_SOURCE_DIR}/src/render/metal/renderer.mm
        ${CMAKE_CURRENT_SOURCE_DIR}/src/platform/darwin/app.mm
        ${CMAKE_CURRENT_SOURCE_DIR}/src/platform/darwin/window.mm
        PROPERTIES COMPILE_FLAGS "-fobjc-arc")

target_sources(cgxm
    PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/src/core/engine.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/src/core/log.cpp

        # ${CMAKE_CURRENT_SOURCE_DIR}/src/render/metal/renderer.mm

        ${CMAKE_CURRENT_SOURCE_DIR}/src/platform/window.hpp
        ${CMAKE_CURRENT_SOURCE_DIR}/src/platform/app.hpp
        ${CMAKE_CURRENT_SOURCE_DIR}/src/platform/darwin/app.mm
        ${CMAKE_CURRENT_SOURCE_DIR}/src/platform/darwin/window.mm

        ${CMAKE_CURRENT_SOURCE_DIR}/src/container/ts_deque.hpp
    PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}/include/cgxm/core/engine.hpp
        ${CMAKE_CURRENT_SOURCE_DIR}/include/cgxm/core/log.hpp

        # ${CMAKE_CURRENT_SOURCE_DIR}/include/cgxm/render/renderer.hpp
)

target_include_directories(cgxm
  PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/include
  PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/src
)

target_link_libraries(cgxm
        PRIVATE
        quill::quill
        ${APPLE_FWK_FOUNDATION}
        ${APPLE_FWK_QUARTZ_CORE}
        ${APPLE_FWK_METAL}
        ${APPLE_FWK_APPKIT}
)

# ?? add_executable(cgxm-editor MACOSX_BUNDLE src/editor/main.cpp)

enable_testing()

add_executable(ts_deque_test ${CMAKE_CURRENT_SOURCE_DIR}/tests/ts_deque_test.cpp)
target_include_directories(ts_deque_test PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/src)
target_link_libraries(ts_deque_test PRIVATE cgxm GTest::gtest_main)

include (GoogleTest)
gtest_discover_tests(ts_deque_test)
add_subdirectory(examples)
