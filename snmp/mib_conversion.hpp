#pragma once
#include <utility>
#include "detail/mib_module_identify.hpp"
#include "detail/mib_notification_type.hpp"
#include "detail/mib_object_identifier.hpp"
#include "detail/mib_object_type.hpp"
#include "detail/mib_syntax.hpp"

namespace snmp {

template <class Ty1, class Ty2>
void edge(std::shared_ptr<Ty1> &from, std::shared_ptr<Ty2> &to) {
  from->children_.push_back(to);
  to->parent_ = from;
}

}  // namespace snmp