#pragma once
#include <memory>

namespace snmp {

class displaystring_t {
 public:
  displaystring_t() {}
  displaystring_t(size_t min, size_t max) : min_(min), max_(max) {}

  size_t min_ = 0;
  size_t max_ = 0;
};

inline bool operator==(const displaystring_t &lhs, const displaystring_t &rhs) {
  return (lhs.min_ == rhs.min_ && lhs.max_ == rhs.max_);
}

inline bool operator!=(const displaystring_t &lhs, const displaystring_t &rhs) {
  return !(lhs == rhs);
}
inline std::ostream &operator<<(std::ostream &os, const displaystring_t &that) {
  if ((that.max_ == 0) && (that.max_ == that.min_)) {
    os << "DisplayString";
  } else {
    os << "DisplayString (SIZE (" << that.min_ << ".." << that.max_ << "))";
  }

  return os;
}


}  // namespace snmp