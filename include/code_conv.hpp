#ifndef INCG_NP_CODE_CONV_HPP
#define INCG_NP_CODE_CONV_HPP
#include <stdexcept>
#include <string>
#include <string_view>

namespace np {
#ifdef _WIN32
std::wstring utf8ToUtf16(std::string_view utf8);

std::string utf16ToUtf8(std::wstring_view utf16);
#endif // _WIN32
} // namespace np
#endif // INCG_NP_CODE_CONV_HPP