cmake_minimum_required(VERSION 3.10)
project(EmbeddedTest VERSION 1.0)

# C++17 표준 강제
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# 컴파일 경고 활성화 (시니어는 경고를 에러처럼 다룹니다)
add_compile_options(-Wall -Wextra -Wpedantic)

add_executable(app main.cpp)