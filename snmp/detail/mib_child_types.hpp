#pragma once
#include <boost/variant.hpp>
#include <memory>

namespace snmp {
class oid_t;
class otype_t;
class ntype_t;
class seq_t;
typedef boost::variant<std::shared_ptr<oid_t>, std::shared_ptr<otype_t>,
                       std::shared_ptr<ntype_t>, std::shared_ptr<seq_t>>
    child_t;

}  // namespace snmp
