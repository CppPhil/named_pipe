cmake_minimum_required(VERSION 3.15)

project(named_pipe_project)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS ON)

set(APP_HEADERS
    include/code_conv.hpp
    include/format_windows_error.hpp
    include/named_pipe.hpp
    include/status.hpp
    include/string_types.hpp)

set(APP_SOURCES
    src/code_conv.cpp
    src/format_windows_error.cpp
    src/main.cpp
    src/named_pipe.cpp
    src/status.cpp)

set(APP_NAME named_pipe)

add_executable(
  ${APP_NAME}
  ${APP_HEADERS}
  ${APP_SOURCES})

target_include_directories(
  ${APP_NAME}
  PRIVATE
  ${CMAKE_CURRENT_SOURCE_DIR}/include)

if(WIN32)
  target_link_libraries(
    ${APP_NAME}
    PRIVATE
    Kernel32.lib)   
endif()

