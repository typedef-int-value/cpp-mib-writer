#pragma once
#include <iostream>
#include "mib_module_identify.hpp"
#include "mib_notification_type.hpp"
#include "mib_object_base.hpp"
#include "mib_object_identifier.hpp"
#include "mib_object_type.hpp"
#include "syntax/mib_sequence.hpp"
#include "syntax/mib_textual_convention.hpp"

#include "mib_to_stream_impl.hpp"

namespace snmp {

class parent_name_visitor : public boost::static_visitor<> {
 public:
  template <class Ty>
  void operator()(const std::weak_ptr<Ty> &that) {
    if (auto parent = that.lock()) {
      name_ = parent->name();
    }
  }

  std::string name_;
};

inline std::string get_name(const parent_t &p) {
  parent_name_visitor parent_name;
  boost::apply_visitor(parent_name, p);

  return parent_name.name_;
}

inline std::ostream &operator<<(std::ostream &os, const module_id_t &obj) {
  std::string oid_str;
  auto oid = obj.oid();

  if (!oid.empty()) {
    for (const auto &child : oid) {
      oid_str += std::to_string(child);

      if (&child != &oid.back()) oid_str += ".";
    }
  }
  os << declare_module_identity(oid_str, obj.name(), obj.last_updated(),
                                obj.organization(), obj.contact_info(),
                                obj.description());

  os << "::= { enterprises ";
  for (const auto &child : oid) {
    os << std::to_string(child) << " ";
  }
  os << "}\n\n";

  return os;
}
inline std::ostream &operator<<(std::ostream &os, const oid_t &obj) {
  os << declare_object_identifier(std::to_string(obj.oid()), obj.name(),
                                  get_name(obj.parent()));

  return os;
}
inline std::ostream &operator<<(std::ostream &os, const syntax_type &syntax) {
  class syntax_name_visitor : public boost::static_visitor<> {
   public:
    syntax_name_visitor(std::ostream &os) : stream_(os){};
    void operator()(const std::string &value) { stream_.get() << value; }
    void operator()(const std::shared_ptr<tc_t> &textual) {
      stream_.get() << textual->name_;
    }
    void operator()(const std::shared_ptr<seq_t> &seq) {
      stream_.get() << seq->name_;
    }
    void operator()(const octet_str_t &that) { stream_.get() << that; }
    void operator()(const std::shared_ptr<otype_t> &that) {
      stream_.get() << "SEQUENCE OF " << that->name();
    }
    void operator()(const integer_t &that){ stream_.get() << that; }
    void operator()(const displaystring_t &that) { stream_.get() << that; }
    std::reference_wrapper<std::ostream> stream_;
  };

  syntax_name_visitor value(os);
  boost::apply_visitor(value, syntax);

  return os;
}
inline std::ostream &operator<<(std::ostream &os, const otype_t &obj) {
  std::stringstream access_ss;
  std::stringstream syntax_ss;
  access_ss << obj.access();
  syntax_ss << obj.syntax();

  std::vector<std::string> indexes;
  auto index = obj.index();
  if (index.first || index.second) {
    try {
      auto seq = boost::get<std::shared_ptr<seq_t>>(obj.syntax());
      for (const auto &child : seq->children_) {
        if (child->oid() >= index.first && child->oid() <= index.second)
          indexes.push_back(child->name());
      }
    } catch (std::exception) {
    }
  }
  os << declare_object_type(std::to_string(obj.oid()), obj.name(),
                            syntax_ss.str(), "", access_ss.str(),
                            obj.description(), indexes);
    
  return os;
}
inline std::ostream &operator<<(std::ostream &os, const ntype_t &obj) {
  os << declare_notification_type(obj.name(), std::to_string(obj.oid()),
                                  obj.description_, get_name(obj.parent()),
                                  obj.objects_);

  return os;
}
inline std::ostream &operator<<(std::ostream &os, const seq_t &seq) {
  os << "\n";
  os << seq.name_ << "::=" << std::endl;
  os << "\tSEQUENCE {\n";
  for (const auto &child : seq.children_) {
    os << "\t\t";
    os << child->name() << "\t" << child->syntax();
    if (&child != &seq.children_.back()) os << ",\n";
  }
  os << "\n\t}\n";

  return os;
}
}  // namespace snmp