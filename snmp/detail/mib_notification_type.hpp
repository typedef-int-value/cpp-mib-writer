#pragma once
#include "mib_object_type.hpp"
#include <vector>

namespace snmp {

class ntype_t {
 public:
  name_t name_;
  number_t oid_;
  std::vector<name_t> objects_;
  std::string status_;
  std::string description_;

  name_t name() const { return name_; }
  number_t oid() const { return oid_; }

  void set_name(const name_t &name) { name_ = name; }
  void set_oid(const number_t &oid) { oid_ = oid; }

  parent_t parent() const { return parent_; }

  friend std::shared_ptr<ntype_t> make_ntype(const name_t &name,
                                             const number_t &oid); 

  template <class Ty1, class Ty2>
  friend void edge(std::shared_ptr<Ty1> &from, std::shared_ptr<Ty2> &to);

 private:
  ntype_t(const name_t &name, const number_t &oid) : name_(name), oid_(oid) {};

 private:
  parent_t parent_;
};

inline std::shared_ptr<ntype_t> make_ntype(const name_t &name, const number_t &oid) {
  return std::shared_ptr<ntype_t>(new ntype_t(name, oid));
}
}  // namespace snmp
