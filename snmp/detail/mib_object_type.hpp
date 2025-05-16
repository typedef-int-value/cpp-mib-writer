#pragma once
#include <memory>

#include "mib_access.hpp"
#include "mib_object_base.hpp"
#include "mib_syntax.hpp"

namespace snmp {
class otype_t : public std::enable_shared_from_this<otype_t> {
 public:
  ~otype_t() {}

  void set_syntax(const syntax_type &syntax) { syntax_ = syntax; }
  void set_syntax(std::shared_ptr<otype_t> &seq_of_otype) {
    seq_of_otype->parent_ = shared_from_this();
    syntax_ = seq_of_otype;
  }
  void set_syntax(std::shared_ptr<seq_t> &seq_entry) {
    seq_entry->parent_ = shared_from_this();
    syntax_ = seq_entry;
  }

  syntax_type syntax() const { return syntax_; }

  void set_access(const access_type &access) { access_ = access; }
  access_type access() const { return access_; }

  void set_description(const std::string &desc) { description_ = desc; }
  std::string description() const { return description_; }

  void set_units(const std::string &units) { units_ = units; }
  std::string units() const { return units_; }

  void set_status(const std::string &status) { status_ = status; }
  std::string status() const { return status_; }

  name_t name() const { return name_; }
  number_t oid() const { return oid_; }

  void set_name(const name_t &name) { name_ = name; }
  void set_oid(const number_t &oid) { oid_ = oid; }

  parent_t parent() const { return parent_; }

  void set_index(const number_t &min, const number_t &max) {
    index_ = std::make_pair(min, max);
  }
  std::pair<number_t, number_t> index() const { return index_; 
  }
 public:
  friend std::shared_ptr<otype_t> make_otype(const name_t &name,
                                             const number_t &oid);
  template <class Ty1, class Ty2>
  friend void edge(std::shared_ptr<Ty1> &from, std::shared_ptr<Ty2> &to);

 private:
  /** don't use this. you can use snmp::make_otype */
  otype_t(const name_t &name, const number_t &oid) : name_(name), oid_(oid) {
    status_ = "current";
    access_ = access_type::kNotAccessible;
  }

 private:
  name_t name_;
  number_t oid_;

  syntax_type syntax_;
  access_type access_;

  std::string description_;  // optional
  std::string units_;        // optional
  std::string status_;       // optional
  std::pair<number_t, number_t> index_;

  parent_t parent_;
};

inline std::shared_ptr<otype_t> make_otype(const name_t &name,
                                           const number_t &oid) {
  return std::shared_ptr<otype_t>(new otype_t(name, oid));
}

inline std::shared_ptr<otype_t> make_copy(
    const std::shared_ptr<otype_t> &that) {
  std::shared_ptr<otype_t> make = make_otype(that->name(), that->oid());

  make->set_access(that->access());
  make->set_description(that->description());
  make->set_status(that->status());
  make->set_syntax(that->syntax());
  make->set_units(that->units());

  return make;
}

}  // namespace snmp
