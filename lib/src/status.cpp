#include <utility>

#include "status.hpp"

namespace np {
Status Status::ok()
{
  return Status{Status::Ok, NP_TEXT("The operation completed successfully.")};
}

Status::Status(StatusEnum enumerator, String message)
  : m_enumerator{enumerator}, m_message{std::move(message)}
{
}

Status::StatusEnum Status::enumerator() const noexcept
{
  return m_enumerator;
}

const String& Status::message() const noexcept
{
  return m_message;
}

bool Status::isOk() const noexcept
{
  return m_enumerator == Ok;
}

bool Status::isError() const noexcept
{
  return !isOk();
}
} // namespace np
