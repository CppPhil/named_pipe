#pragma once
#include <stdexcept>

#ifdef _WIN32
#include <Windows.h>
#endif

#include "status.hpp"
#include "string_types.hpp"

namespace np {
class NamedPipe {
public:
  enum class Mode { Create, Connect };

  NamedPipe(String name, Mode mode);

  NamedPipe(const NamedPipe&) = delete;

  NamedPipe& operator=(const NamedPipe&) = delete;

  ~NamedPipe();

  const String& name() const noexcept;

  Mode mode() const noexcept;

  Status write(const void* data, std::size_t byteCount);

  Status read(void* buffer, std::size_t bytesToRead);

private:
  String m_name;
  Mode   m_mode;
#ifdef _WIN32
  HANDLE m_pipe;
#else
  int m_pipe;
#endif
};
} // namespace np
