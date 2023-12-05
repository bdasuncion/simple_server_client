cmake_minimum_required(VERSION 3.10)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED True)

set(THREADS_PREFER_PTHREAD_FLAG ON)

project(Client VERSION 1.0 LANGUAGES C CXX)
find_package(Threads REQUIRED)
add_executable(Client main.cpp client.cpp)
target_link_libraries(Client   Threads::Threads)
target_include_directories(Client PUBLIC "${PROJECT_BINARY_DIR}")
