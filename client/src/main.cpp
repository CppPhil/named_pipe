#include <cstddef>
#include <cstdlib>

#include <array>
#include <limits>

#include "named_pipe.hpp"
#include "pipe_name.hpp"
#include "string_types.hpp"
#include "code_conv.hpp"

int main()
{
  try {
    np::NamedPipe namedPipe{np::pipeName, np::NamedPipe::Mode::Connect};

    NP_COUT << NP_TEXT("Client connected to pipe: ") << namedPipe.name() << NP_TEXT("\n");
    NP_COUT << NP_TEXT("Enter 20 characters: ");

    std::array<char, 20> buffer{};

    for (std::size_t i{0}; (i < buffer.size()) && std::cin.get(buffer[i]); ++i) { }

    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

    const np::Status status{
      namedPipe.write(buffer.data(), buffer.size())
    };

    if (status.isOk()) {
      std::cout << "Client wrote ";

      for (char c : buffer) {
        std::cout << c;
      }

      std::cout << '\n';
    } else {
      NP_CERR << NP_TEXT("Client failed to write: ") << status.message() << NP_TEXT("\n");

      return EXIT_FAILURE;
    }
  } catch (const std::runtime_error& ex) {
#ifdef _WIN32
  NP_CERR << NP_TEXT("Client caught runtime_error: ") <<
             np::utf8ToUtf16(ex.what()) << NP_TEXT("\n");

  return EXIT_FAILURE;
#endif
  }

  return EXIT_SUCCESS;
}
