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
#ifdef _WIN32
  DWORD numberOfBytesWritten{};

  if (!WriteFile(
        /* hFile */ m_pipe,
        /* lpBuffer */ data,
        /* nNumberOfBytesToWrite */ byteCount,
        /* lpNumberOfBytesWritten */ &numberOfBytesWritten,
        /* lpOverlapped */ nullptr)) {
    return Status{
      Status::WriteFailure,
      NP_TEXT("") /* TODO: get error message from windows */};
  }

  if (numberOfBytesWritten != byteCount) {
    return Status{
      Status::WriteFailure,
      NP_TEXT("Number of bytes written was not equal to the number of bytes it "
              "should've written.")};
  }

  return Status::ok();
#endif
}

Status NamedPipe::read(void* buffer, std::size_t bytesToRead)
{
#ifdef _WIN32
  DWORD numberOfBytesRead{};

  if (!ReadFile(
        /* hFile */ m_pipe,
        /* lpBuffer */ buffer,
        /* nNumberOfBytesToRead */ bytesToRead,
        /* lpNumberOfBytesRead */ &numberOfBytesRead,
        /* lpOverlapped */ nullptr)) {
    return Status{
      Status::ReadFailure,
      NP_TEXT("") /* TODO: get error message from windows */};
  }

  if (numberOfBytesRead != bytesToRead) {
    return Status{
      Status::ReadFailure,
      NP_TEXT("Number of bytes actually read was not equal to the number of "
              "bytes that should've been read.")};
  }

  return Status::ok();
#endif
}
} // namespace np
