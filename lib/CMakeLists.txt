set(LIB_HEADERS
    include/code_conv.hpp
    include/format_windows_error.hpp
    include/named_pipe.hpp
    include/pipe_name.hpp
    include/status.hpp
    include/string_types.hpp)

set(LIB_SOURCES
    src/code_conv.cpp
    src/format_windows_error.cpp
    src/named_pipe.cpp
    src/pipe_name.cpp
    src/status.cpp)

set(LIB_NAME named_pipe_lib)

add_library(
  ${LIB_NAME}
  STATIC
  ${LIB_HEADERS}
  ${LIB_SOURCES})

target_include_directories(
  ${LIB_NAME}
  PUBLIC
  ${CMAKE_CURRENT_SOURCE_DIR}/include)

if(WIN32)
  target_link_libraries(
    ${LIB_NAME}
    PUBLIC
    Kernel32.lib)

  target_compile_definitions(
    ${LIB_NAME}
    PUBLIC
    NOMINMAX)
endif()

