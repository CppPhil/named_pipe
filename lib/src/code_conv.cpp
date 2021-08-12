#include <cstddef>

#ifdef _WIN32
#include <Windows.h>
#endif // _WIN32

#include "code_conv.hpp"

namespace np {
#ifdef _WIN32
namespace {
[[nodiscard]] std::size_t utf8ToUtf16RequiredChars(std::string_view utf8)
{
  return MultiByteToWideChar(
    /* CodePage */ CP_UTF8,
    /* dwFlags */ 0,
    /* lpMultiByteStr */ utf8.data(),
    /* cbMultiByte */ -1,
    /* lpWideCharStr */ nullptr,
    /* cchWideChar */ 0);
}

[[nodiscard]] std::size_t utf16ToUtf8RequiredBytes(std::wstring_view utf16)
{
  return WideCharToMultiByte(
    /* CodePage */ CP_UTF8,
    /* dwFlags */ 0,
    /* lpWideCharStr */ utf16.data(),
    /* cchWideChar */ -1,
    /* lpMultiByteStr */ nullptr,
    /* cbMultiByte */ 0,
    /* lpDefaultChar */ nullptr,
    /* lpUsedDefaultChar */ nullptr);
}
} // anonymous namespace

std::wstring utf8ToUtf16(std::string_view utf8)
{
  const std::size_t charsNeeded{utf8ToUtf16RequiredChars(utf8)};
  std::wstring      buffer(charsNeeded, L' ');

  const int statusCode{MultiByteToWideChar(
    /* CodePage */ CP_UTF8,
    /* dwFlags */ 0,
    /* lpMultiByteStr */ utf8.data(),
    /* cbMultiByte */ -1,
    /* lpWideCharStr */ buffer.data(),
    /* cchWideChar */ static_cast<int>(buffer.size()))};

  if (statusCode == 0) {
    throw std::runtime_error{
      "Could not convert \"" + std::string{utf8} + "\" to UTF-16."};
  }

  buffer.pop_back();

  return buffer;
}

std::string utf16ToUtf8(std::wstring_view utf16)
{
  const std::size_t bytesNeeded{utf16ToUtf8RequiredBytes(utf16)};
  std::string       buffer(bytesNeeded, ' ');

  const int statusCode{WideCharToMultiByte(
    /* CodePage */ CP_UTF8,
    /* dwFlags */ 0,
    /* lpWideCharStr */ utf16.data(),
    /* cchWideChar */ -1,
    /* lpMultiByteStr */ buffer.data(),
    /* cbMultiByte */ static_cast<int>(buffer.size()),
    /* lpDefaultChar */ nullptr,
    /* lpUsedDefaultChar */ nullptr)};

  if (statusCode == 0) {
    throw std::runtime_error{"Could not convert UTF-16 data to UTF-8."};
  }

  buffer.pop_back();

  return buffer;
}
#endif
} // namespace np