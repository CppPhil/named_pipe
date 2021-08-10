#ifndef INCG_NP_STATUS_HPP
#define INCG_NP_STATUS_HPP
#include "string_types.hpp"

namespace np {
class [[nodiscard]] Status {
public:
  enum StatusEnum { Ok, WriteFailure, ReadFailure };

  static Status ok();

  Status(StatusEnum enumerator, String message);

  StatusEnum enumerator() const noexcept;

  const String& message() const noexcept;

  [[nodiscard]] bool isOk() const noexcept;

  [[nodiscard]] bool isError() const noexcept;

private:
  StatusEnum m_enumerator;
  String     m_message;
};
} // namespace np
#endif // INCG_NP_STATUS_HPP
