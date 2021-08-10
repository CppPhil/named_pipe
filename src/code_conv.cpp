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
} // namespace

std::wstring utf8ToUtf16(std::string_view utf8)
{
  static_assert(sizeof(char) == 1, "char isn't 1 byte wide");

  const std::size_t charsNeeded{utf8ToUtf16RequiredChars(utf8)};
  std::wstring      buffer(charsNeeded, L' ');

  const int statusCode{MultiByteToWideChar(
    /* CodePage */ CP_UTF8,
    /* dwFlags */ 0,
    /* lpMultiByteStr */ utf8.data(),
    /* cbMultiByte */ -1,
    /* lpWideCharStr */ buffer.data(),
    /* cchWideChar */ buffer.size())};

  if (statusCode == 0) {
    throw std::runtime_error{
      "Could not convert \"" + std::string{utf8} + "\" to UTF-16."};
  }

  buffer.pop_back(); // Remove extraneous L'\0' character written by
                     // MultiByteToWideChar, std::wstring is already
                     // null-terminated.
  return buffer;
}

std::string utf16ToUtf8(std::wstring_view utf16)
{
  static_assert(sizeof(wchar_t) == 2, "wchar_t isn't 2 bytes wide");

  const std::size_t bytesNeeded{utf16ToUtf8RequiredBytes(utf16)};
  std::string       buffer(bytesNeeded, ' ');

  const int statusCode{WideCharToMultiByte(
    /* CodePage */ CP_UTF8,
    /* dwFlags */ 0,
    /* lpWideCharStr */ utf16.data(),
    /* cchWideChar */ -1,
    /* lpMultiByteStr */ buffer.data(),
    /* cbMultiByte */ buffer.size(),
    /* lpDefaultChar */ nullptr,
    /* lpUsedDefaultChar */ nullptr)};

  if (statusCode == 0) {
    throw std::runtime_error{"Could not convert UTF-16 data to UTF-8."};
  }

  buffer
    .pop_back(); // Remove extraneous 0x00 byte written by WideCharToMultiByte,
                 // std::string is already null-terminated.
  return buffer;
}
#endif // _WIN32
} // namespace np
