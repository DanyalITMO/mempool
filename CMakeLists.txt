cmake_minimum_required(VERSION 3.17)
project(hw2)

set(CMAKE_CXX_STANDARD 17)

add_executable(hw2 main.cpp)
target_compile_options(${PROJECT_NAME} PRIVATE -fno-omit-frame-pointer -fsanitize=address)
target_link_options(${PROJECT_NAME} PRIVATE -fno-omit-frame-pointer -fsanitize=address)
