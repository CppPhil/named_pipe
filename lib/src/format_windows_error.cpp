#include "format_windows_error.hpp"

namespace np {
#ifdef _WIN32
std::wstring formatWindowsError(DWORD errorCode)
{
  LPWSTR      buffer{nullptr};
  const DWORD statusCode{FormatMessageW(
    /* dwFlags */ FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_ALLOCATE_BUFFER
      | FORMAT_MESSAGE_IGNORE_INSERTS,
    /* lpSource */ nullptr,
    /* dwMessageId */ errorCode,
    /* dwLanguageId */ MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
    /* lpBuffer */ reinterpret_cast<LPWSTR>(&buffer),
    /* nSize */ 0,
    /* Arguments */ nullptr)};

  if (statusCode == 0) {
    throw std::runtime_error{"FormatMessageW failed!"};
  }

  std::wstring result{};

  if (buffer != nullptr) {
    result = buffer;
    const HLOCAL hLocal{LocalFree(buffer)};

    if (hLocal != nullptr) {
      throw std::runtime_error{"LocalFree failed!"};
    }

    return result;
  }

  throw std::runtime_error{"Couldn't allocate memory for FormatMessageW!"};
}
#endif // _WIN32
} // namespace np
