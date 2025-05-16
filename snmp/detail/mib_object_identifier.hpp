#pragma once

#include <vector>
#include "mib_object_base.hpp"

namespace snmp {

class oid_t : public Parent<oid_t> {
 public:
  ~oid_t() {}
#if 0
  std::vector<child_t>::const_iterator begin() const {
    return children_.begin();
  }
  std::vector<child_t>::const_iterator end() const { return children_.end(); }
#endif
  parent_t parent() const { return parent_; }

  friend std::shared_ptr<oid_t> make_oid(const name_t &name,
                                         const number_t &oid);

  template <class Ty1, class Ty2>
  friend void edge(std::shared_ptr<Ty1> &from, std::shared_ptr<Ty2> &to);


  name_t name() const { return name_; }
  number_t oid() const { return oid_; }

  void set_name(const name_t &name) { name_ = name; }
  void set_oid(const number_t &oid) { oid_ = oid; }

  friend class Parent<oid_t>;
 private:
  /** don't use this. you can use snmp::make_oid */
  oid_t(const name_t &name, const number_t &oid) : name_(name), oid_(oid) {}
  name_t name_;
  number_t oid_;

  parent_t parent_;
  std::vector<child_t> children_;
};

inline std::shared_ptr<oid_t> make_oid(const name_t &name,
                                       const number_t &oid) {
  return std::shared_ptr<oid_t>(new oid_t(name, oid));
}

inline std::vector<std::shared_ptr<oid_t>> make_oids() {
  return std::vector<std::shared_ptr<oid_t>>();
}

template <typename... Types>
std::vector<std::shared_ptr<oid_t>> make_oids(std::pair<name_t, number_t> oid,
                                              Types... args) {
  std::vector<std::shared_ptr<oid_t>> oids;

  oids.push_back(make_oid(oid.first, oid.second));
  auto children = make_oids(args...);
  for (const auto &child : children) oids.push_back(child);

  return oids;
}

}  // namespace snmp
