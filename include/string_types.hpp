#ifndef INCG_NP_STRING_TYPES_HPP
#define INCG_NP_STRING_TYPES_HPP
#include <iostream>
#include <string>
#include <string_view>

namespace np {
#ifdef _WIN32
using Character = wchar_t;
#define NP_TEXT(text) L##text
#define NP_COUT ::std::wcout
#define NP_CERR ::std::wcerr
#else
using Character = char;
#define NP_TEXT(text) text
#define NP_COUT ::std::cout
#define NP_CERR ::std::cerr
#endif

using String     = std::basic_string<Character>;
using StringView = std::basic_string_view<Character>;
} // namespace np
#endif // INCG_NP_STRING_TYPES_HPP