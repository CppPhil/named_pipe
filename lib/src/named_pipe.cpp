#include <utility>

#include "code_conv.hpp"
#include "constants.hpp"
#include "format_windows_error.hpp"
#include "named_pipe.hpp"

namespace np {
namespace {
#ifdef _WIN32
void addPrefixToPipeName(String& name)
{
  static const String prefix{NP_TEXT("\\\\.\\pipe\\")};

  name = prefix + name;
}

[[nodiscard]] HANDLE createNamedPipe(StringView name)
{
  const HANDLE handle{CreateNamedPipeW(
    /* lpName */ name.data(),
    /* dwOpenMode */ PIPE_ACCESS_INBOUND,
    /* dwPipeMode */ PIPE_TYPE_MESSAGE | PIPE_READMODE_MESSAGE | PIPE_WAIT,
    /* nMaxInstances */ PIPE_UNLIMITED_INSTANCES,
    /* nOutBufferSize */ bufferSize,
    /* nInBufferSize */ bufferSize,
    /* nDefaultTimeOut */ waitTimeMilliseconds,
    /* lpSecurityAttributes */ nullptr)};

  return handle;
}

[[nodiscard]] HANDLE openNamedPipe(StringView name)
{
  const HANDLE handle{CreateFileW(
    /* lpFileName */ name.data(),
    /* dwDesiredAccess */ GENERIC_WRITE,
    /* dwShareMode */ 0,
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
      throw std::runtime_error{
        "Server: Could not create pipe with name: " + utf16ToUtf8(m_name)};
    }

    if (!ConnectNamedPipe(m_pipe, nullptr)) {
      throw std::runtime_error{
        "Server: ConnectNamedPipe failed: "
        + utf16ToUtf8(formatWindowsError(GetLastError()))};
    }

    break;
  }
  case Mode::Connect: {
    if (!WaitNamedPipeW(
          /* lpNamedPipeName */ m_name.data(),
          /* nTimeOut */ waitTimeMilliseconds)) {
      CloseHandle(/* hObject */ m_pipe);
      throw std::runtime_error{
        "Client: (WaitNamedPipeW) waited too long for pipe "
        + utf16ToUtf8(m_name)};
    }

    m_pipe = openNamedPipe(m_name);

    if (m_pipe == INVALID_HANDLE_VALUE) {
      throw std::runtime_error{
        "Client: (openNamedPipe) Could not open pipe with name: "
        + utf16ToUtf8(m_name)};
    }

    break;
  }
  }
#endif
}

NamedPipe::~NamedPipe()
{
#ifdef _WIN32
  if (m_pipe != INVALID_HANDLE_VALUE) {
    if (!CloseHandle(/* hObject */ m_pipe)) {
      NP_CERR << NP_TEXT("Could not close pipe in destructor!\n");
    }
  }
#endif
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
    return Status{Status::WriteFailure, formatWindowsError(GetLastError())};
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
    return Status{Status::ReadFailure, formatWindowsError(GetLastError())};
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
