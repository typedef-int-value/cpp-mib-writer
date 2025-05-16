#pragma once
#include <string>
#include <vector>

#include "mib_child_types.hpp"
#include "mib_parent_type.hpp"

namespace snmp {
typedef unsigned int number_t;
typedef std::string name_t;

template <typename Ty>
class Parent {
 public:
  std::vector<child_t>::const_iterator begin() const {
    return static_cast<Ty const*>(this)->children_.begin();
  }
  std::vector<child_t>::const_iterator end() const {
    return static_cast<Ty const*>(this)->children_.end();
  }
};
}  // namespace snmp
