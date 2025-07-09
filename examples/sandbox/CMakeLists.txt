# examples/sandbox/CMakeLists.txt

add_executable(sandbox src/main.cpp) 

target_sources(sandbox PRIVATE
  ${CMAKE_CURRENT_SOURCE_DIR}/src/sandbox.cpp
)

target_include_directories(sandbox 
  PUBLIC 
    ${CMAKE_CURRENT_SOURCE_DIR}/include
  PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/src
)

target_link_libraries(sandbox PRIVATE cgxm)

add_dependencies(sandbox cgxm)
