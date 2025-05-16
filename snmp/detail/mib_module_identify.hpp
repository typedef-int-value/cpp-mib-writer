#pragma once
#include <vector>
#include "mib_object_base.hpp"

namespace snmp {

class module_id_t : public Parent<module_id_t> {
 public:
  name_t name() const { return name_; }
  std::vector<number_t> oid() const { return oid_; }

  void set_last_updated(const std::string &last_updated) {
    last_updated_ = last_updated;
  }
  std::string last_updated() const { return last_updated_; }

  void set_organization(const std::string organization) {
    organization_ = organization;
  }
  std::string organization() const { return organization_; }

  void set_contact_info(const std::string &contact_info) {
    contact_info_ = contact_info;
  }
  std::string contact_info() const { return contact_info_; }

  void set_description(const std::string &description) {
    description_ = description;
  }
  std::string description() const { return description_; }

  void set_parent_name(const name_t &parent_name) {
    parent_name_ = parent_name;
  }
  std::string parent_name() const { return parent_name_; }

  template <typename... Types>
  friend std::shared_ptr<module_id_t> make_module_id(const name_t &name,
                                                     Types... oids);
  template <class Ty1, class Ty2>
  friend void edge(std::shared_ptr<Ty1> &from, std::shared_ptr<Ty2> &to);

  friend class Parent<module_id_t>;
 private:
  /** don't use this. you can use snmp::make_module_id */
  module_id_t() {}
  template <typename... Types>
  module_id_t(const name_t &name, Types... oids) {
    set_name(name);
    set_oid(oids...);
    last_updated_ = __DATE__;
  }

  void set_name(const name_t &name) { name_ = name; }
  template <typename... Types>
  void set_oid(const number_t &oid, Types... args) {
    oid_.push_back(oid);
    set_oid(args...);
  }
  void set_oid(){};

 private:
  name_t name_;
  std::vector<number_t> oid_;

  std::string last_updated_;
  std::string organization_;
  std::string contact_info_;
  std::string description_;

  name_t parent_name_;
  std::vector<child_t> children_;
};

template <typename... Types>
std::shared_ptr<module_id_t> make_module_id(const name_t &name, Types... oids) {
  return std::shared_ptr<module_id_t>(new module_id_t(name, oids...));
}

}  // namespace snmp