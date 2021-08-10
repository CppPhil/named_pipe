add_executable(
  client
  src/main.cpp)

target_link_libraries(
  server
  PRIVATE
  named_pipe_lib)

