#pragma once
#include <memory>
#include <string>
#include <vector>
namespace snmp {
class otype_t;
class seq_t {
 public:
  seq_t(const std::string &name) : name_(name) {}
  std::string name_;
  
  std::weak_ptr<otype_t> parent_;
  std::vector<std::shared_ptr<otype_t>> children_;

  std::string name() const { return name_; }
};

inline std::shared_ptr<seq_t> make_seq(const std::string &name) {
  return std::shared_ptr<seq_t>(new seq_t(name));
}

}  // namespace snmp
