cmake_minimum_required(VERSION 3.29)
project(ProjetING3 C)

set(CMAKE_C_STANDARD 99)

add_executable(ProjetING3 main.c)

target_link_libraries(ProjetING3 -lmingw32 -lalleg44)