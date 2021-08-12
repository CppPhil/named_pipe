#include <cstddef>
#include <cstdlib>

#include <array>
#include <string>

#include "code_conv.hpp"
#include "constants.hpp"
#include "named_pipe.hpp"
#include "pipe_name.hpp"
#include "string_types.hpp"

int main()
{
  try {
    np::NamedPipe namedPipe{np::pipeName, np::NamedPipe::Mode::Connect};

    NP_COUT << NP_TEXT("Client connected to pipe: ") << namedPipe.name()
            << NP_TEXT("\n");

    static const std::string         alphabet{"abcdefghijklmnopqrstuvwxyz"};
    std::array<char, np::bufferSize> buffer{};

    for (std::size_t i{0}; i < buffer.size(); ++i) {
      buffer[i] = alphabet[i % alphabet.size()];
    }

    const np::Status status{namedPipe.write(buffer.data(), buffer.size())};

    if (status.isOk()) {
      std::cout << "Client wrote ";

      for (char c : buffer) {
        std::cout << c;
      }

      std::cout << '\n';
    }
    else {
      NP_CERR << NP_TEXT("Client failed to write: ") << status.message()
              << NP_TEXT("\n");

      return EXIT_FAILURE;
    }
  }
  catch (const std::runtime_error& ex) {
#ifdef _WIN32
    NP_CERR << NP_TEXT("Client caught runtime_error: ")
            << np::utf8ToUtf16(ex.what()) << NP_TEXT("\n");

    return EXIT_FAILURE;
#else
    NP_CERR << NP_TEXT("Client caught runtime_error: ") << ex.what()
            << NP_TEXT("\n");
#endif
  }

  return EXIT_SUCCESS;
}
