cmake_minimum_required(VERSION 3.10)
project(BlockBreaker C)

set(CMAKE_C_STANDARD 11)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

add_subdirectory(external/raylib)

add_executable(block-breaker 
  main.c
  defs.c
  utils.c
  home.c
)
target_link_libraries(block-breaker raylib)
target_include_directories(block-breaker PRIVATE
  ./external/raylib/src/
)

if(UNIX AND NOT APPLE)
    target_link_libraries(pong m dl pthread)
endif()
