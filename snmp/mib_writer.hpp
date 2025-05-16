#pragma once
#include <fstream>
#include <memory>
#include <string>

#include "detail/mib_writer_impl.hpp"

namespace snmp {
class module_id_t;

inline void write_mib(const std::string &filename,
                      const std::shared_ptr<module_id_t> &mod_id) {
  std::ofstream of(filename);
  mib_writer mw(of, mod_id);
}

inline void write_mib(std::ostream &os,
                      const std::shared_ptr<module_id_t> &mod_id) {
  mib_writer mw(os, mod_id);
}

};  // namespace snmp
