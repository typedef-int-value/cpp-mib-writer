#pragma once
#include "mib_octet_string.hpp"
#include "mib_integer.hpp"

#include <boost/variant.hpp>
#include <memory>
#include <string>
#include <vector>

namespace snmp {
class tc_t {
 public:
  typedef boost::variant<std::string, octet_str_t, integer_t> value_t;
  tc_t() {}
  tc_t(const std::string &name, const value_t &value)
      : name_(name), value_(value){};

  bool operator==(const tc_t &that) {
    if (display_hint_ != that.display_hint_) return false;
    if (name_ != that.name_) return false;
    if (value_ != that.value_) return false;
    if (description_ != that.description_) return false;
    if (status_ != that.status_) return false;

    return true;
  };
  bool operator!=(const tc_t &that) { return !(*this == that); }

  std::string display_hint_;
  std::string name_;
  std::string status_;
  std::string description_;
  value_t value_;
};

inline std::shared_ptr<tc_t> make_tc(const std::string &name,
                                     const tc_t::value_t &value) {
  return std::shared_ptr<tc_t>(new tc_t(name, value));
}
}  // namespace snmp
