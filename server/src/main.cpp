#include <cstdlib>

#include <iostream>

#include "code_conv.hpp"
#include "constants.hpp"
#include "named_pipe.hpp"
#include "pipe_name.hpp"

static void toUpper(std::string& str)
{
  for (char& c : str) {
    c &= ~0x20;
  }
}

int main()
{
  try {
    np::NamedPipe namedPipe{np::pipeName, np::NamedPipe::Mode::Create};
    NP_COUT << NP_TEXT("Server: Created pipe with name: ") << namedPipe.name()
            << NP_TEXT("\n");

    std::string buffer(np::bufferSize, ' ');

    const np::Status status{namedPipe.read(buffer.data(), buffer.size())};

    if (status.isOk()) {
      toUpper(buffer);

      std::cout << "Server: result is \"" << buffer << "\"\n";
    }
    else {
      NP_CERR << NP_TEXT("Server: reading failed. ") << status.message()
              << NP_TEXT("\n");

      return EXIT_FAILURE;
    }
  }
  catch (const std::runtime_error& ex) {
#if _WIN32
    NP_CERR << NP_TEXT("Server caught runtime_error: ")
            << np::utf8ToUtf16(ex.what()) << NP_TEXT("\n");

    return EXIT_FAILURE;
#else
    NP_CERR << NP_TEXT("Server caught runtime_error: ") << ex.what()
            << NP_TEXT("\n");

    return EXIT_FAILURE;
#endif
  }

  return EXIT_SUCCESS;
}
