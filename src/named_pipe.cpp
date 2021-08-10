#include <utility>

#include "named_pipe.hpp"

namespace np {
namespace {
#ifdef _WIN32
void addPrefixToPipeName(String& name)
{
  static const String prefix{TEXT("\\\\.\\pipe\\")};

  name = prefix + name;
}

[[nodiscard]] HANDLE createNamedPipe(StringView name)
{
  const HANDLE handle{CreateNamedPipeW(
    /* lpName */ name.data(),
    /* dwOpenMode */ PIPE_ACCESS_INBOUND,
    /* dwPipeMode */ PIPE_TYPE_BYTE | PIPE_WAIT,
    /* nMaxInstances */ PIPE_UNLIMITED_INSTANCES,
    /* nOutBufferSize */ 100,
    /* nInBufferSize */ 100,
    /* nDefaultTimeOut */ 0,
    /* lpSecurityAttributes */ nullptr)};

  return handle;
}

[[nodiscard]] HANDLE openNamedPipe(StringView name)
{
  const HANDLE handle{CreateFileW(
    /* lpFileName */ name.data(),
    /* dwDesiredAccess */ GENERIC_WRITE,
    /* dwShareMode */ FILE_SHARE_READ,
    /* lpSecurityAttributes */ nullptr,
    /* dwCreationDisposition */ OPEN_EXISTING,
    /* dwFlagsAndAttributes */ FILE_ATTRIBUTE_NORMAL,
    /* hTemplateFile */ nullptr)};

  return handle;
}
#endif
} // anonymous namespace

NamedPipe::NamedPipe(String name, Mode mode) : m_name
{
  std::move(name)
}
#ifdef _WIN32
, m_pipe
{
  INVALID_HANDLE_VALUE
}
#endif
{
#ifdef _WIN32
  addPrefixToPipeName(m_name);

  switch (mode) {
  case Mode::Create: {
    m_pipe = createNamedPipe(m_name);

    if (m_pipe == INVALID_HANDLE_VALUE) {
      // TODO: Handle error.
    }
    break;
  }
  case Mode::Connect: {
    m_pipe = openNamedPipe(m_name);

    if (m_pipe == INVALID_HANDLE_VALUE) {
      // TODO: HANDLE ERROR
    }

    if (!ConnectNamedPipe(
          /* hNamedPipe */ m_pipe, /* lpOverlapped */ nullptr)) {
      // TODO: HANDLE ERROR
      // Deallocate the named pipe.
    }

    break;
  }
  }
#endif
}

NamedPipe::~NamedPipe()
{
  // TODO: HERE
}

const String& NamedPipe::name() const noexcept
{
  return m_name;
}

NamedPipe::Mode NamedPipe::mode() const noexcept
{
  return m_mode;
}

Status NamedPipe::write(const void* data, std::size_t byteCount)
{
  // TODO: HERE
}

Status NamedPipe::read(void* buffer, std::size_t bytesToRead)
{
  // TODO: HERE
}
} // namespace np
