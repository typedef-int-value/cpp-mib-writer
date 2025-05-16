#pragma once
#include <iostream>
namespace snmp {
enum class access_type {
  kNotAccessible,       // "not-accessible"
  kAccssibleForNotify,  // "accessible-for-notify"
  kReadOnly,            // "read-only"
  kReadWrite,           // "read-write"
  kReadCreate,          // "read-create"
};

inline std::ostream &operator<<(std::ostream &os, const access_type &access) {
  switch (access) {
    case access_type::kNotAccessible:
      os << "not-accessible";
      break;
    case access_type::kAccssibleForNotify:
      os << "accessible-for-notify";
      break;
    case access_type::kReadOnly:
      os << "read-only";
      break;
    case access_type::kReadWrite:
      os << "read-write";
      break;
    case access_type::kReadCreate:
      os << "read-create";
      break;
  }
  return os;
}
}  // namespace snmp