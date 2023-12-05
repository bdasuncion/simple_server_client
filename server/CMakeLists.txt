cmake_minimum_required(VERSION 3.10)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED True)

set(THREADS_PREFER_PTHREAD_FLAG ON)

project(Server VERSION 1.0 LANGUAGES C CXX)

add_executable(Server main.cpp)
find_package(Threads REQUIRED)
target_link_libraries(Server   Threads::Threads)
target_include_directories(Server PUBLIC "${PROJECT_BINARY_DIR}")