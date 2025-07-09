cmake_minimum_required(VERSION 3.20)
project(cgxm)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra -Werror")

enable_language(OBJCXX)

# find_library(APPLE_FWK_FOUNDATION Foundation REQUIRED)
# find_library(APPLE_FWK_APPKIT AppKit REQUIRED)
# find_library(APPLE_FWK_QUARTZ_CORE QuartzCore REQUIRED)
# find_library(APPLE_FWK_METAL Metal REQUIRED)

add_library(cgxm STATIC)

target_sources(cgxm PRIVATE 
    ${CMAKE_CURRENT_SOURCE_DIR}/src/core/engine.cpp
)

target_include_directories(cgxm
  PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/include
  PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/src
)

add_subdirectory(examples)
