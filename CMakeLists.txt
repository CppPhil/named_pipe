cmake_minimum_required(VERSION 3.15)

project(named_pipe_project)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS ON)

add_subdirectory(lib)
add_subdirectory(server)
add_subdirectory(client)

