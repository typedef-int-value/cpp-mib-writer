#pragma once
#include <boost/variant.hpp>
#include <memory>
#include <string>

#include "syntax/mib_integer.hpp"
#include "syntax/mib_octet_string.hpp"
#include "syntax/mib_sequence.hpp"
#include "syntax/mib_textual_convention.hpp"
#include "syntax/mib_displaystring.hpp"

namespace snmp {
// class tc_t;
// class seq_t;
class otype_t;
typedef boost::variant<std::string, octet_str_t, integer_t, std::shared_ptr<tc_t>,
                       std::shared_ptr<seq_t>, std::shared_ptr<otype_t>, displaystring_t>
    syntax_type;

namespace syntax {
constexpr auto kIpAddress= "IpAddress";
constexpr auto kInteger32 = "Integer32";
constexpr auto kUnsigned32 = "Unsigned32";
constexpr auto kCounter64 = "Counter64";
constexpr auto kDateAndTime = "DateAndTime";
}

}  // namespace snmp