#pragma once
#include <boost/variant.hpp>
#include <memory>

namespace snmp {
class module_id_t;
class oid_t;
class otype_t;
class ntype_t;
class seq_t;

typedef boost::variant<std::weak_ptr<module_id_t>, std::weak_ptr<oid_t>,
                       std::weak_ptr<seq_t>, std::weak_ptr<otype_t>,
                       std::weak_ptr<ntype_t>>
    parent_t;

}  // namespace snmp
