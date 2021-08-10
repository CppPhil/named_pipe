add_executable(
  client
  src/main.cpp)

target_link_libraries(
  client
  PRIVATE
  named_pipe_lib)

