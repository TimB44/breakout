cmake_minimum_required(VERSION 3.10)
project(breakout C)

set(CMAKE_C_STANDARD 11)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

add_subdirectory(external/raylib)

add_executable(breakout 
  main.c
  defs.c
  utils.c
  home.c
  game.c
)
target_link_libraries(breakout raylib)
target_include_directories(breakout PRIVATE
  ./external/raylib/src/
)

if(UNIX AND NOT APPLE)
    target_link_libraries(breakout m dl pthread)
endif()
