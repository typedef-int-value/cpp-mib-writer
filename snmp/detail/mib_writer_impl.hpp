#pragma once
#include <fstream>
#include <iostream>
#include <map>
#include <sstream>

#include <boost/variant.hpp>

#include "mib_to_stream.hpp"

namespace snmp {
class otype_parent_writer : public boost::static_visitor<> {
 public:
  otype_parent_writer(std::ostream &os, std::shared_ptr<otype_t> c)
      : os_(os), c_(c) {}

  void operator()(const std::weak_ptr<seq_t> &p) {
    if (auto parent = p.lock()) {
      if (auto p_of_p = parent->parent_.lock()) {
        os_.get() << link(p_of_p->name(), std::to_string(c_->oid()));
      }
    }
  }

  template <typename Ty>
  void operator()(const std::weak_ptr<Ty> &p) {
    if (auto parent = p.lock()) {
      os_.get() << link(parent->name(), std::to_string(c_->oid()));
    }
  }
  std::reference_wrapper<std::ostream> os_;
  std::shared_ptr<otype_t> c_;
};

class child_visitor : public boost::static_visitor<> {
 public:
  child_visitor(std::map<std::string, tc_t> &tc_map, std::ostream &body_os)
      : tc_map_(tc_map), os_(body_os) {}
  void operator()(const std::shared_ptr<otype_t> &obj) {
    class otype_syntax_writer : public boost::static_visitor<> {
     public:
      otype_syntax_writer(std::map<std::string, tc_t> &tc_map,
                          std::ostream &body_os)
          : tc_map_(tc_map), os_(body_os) {}
      void operator()(const std::string &that) {}
      void operator()(const octet_str_t &that) {}
      void operator()(const integer_t &that) {}
      void operator()(const displaystring_t &that) {}
      void operator()(const std::shared_ptr<tc_t> &tc) {
        auto find_from = tc_map_.get().find(tc->name_);

        if (find_from == tc_map_.get().end()) {
          tc_map_.get().insert(std::make_pair(tc->name_, *tc));
        } else {
          if (*tc != find_from->second) {
            throw std::invalid_argument("already inserted same name");
          }
        }
      }
      void operator()(const std::shared_ptr<seq_t> &that) {
        /** children will be written by MibToStream */
        os_ << *that;
        for (const auto &child : that->children_) {
          child_visitor v(tc_map_, os_);
          v(child);
        }
      }
      void operator()(const std::shared_ptr<otype_t> &that) {
        child_visitor v(tc_map_, os_);
        v(that);
      }

      std::reference_wrapper<std::map<std::string, tc_t>> tc_map_;
      std::reference_wrapper<std::ostream> os_;
    };

    os_ << *obj;

    otype_parent_writer p(os_, obj);
    boost::apply_visitor(p, obj->parent());

    otype_syntax_writer v(tc_map_, os_);
    boost::apply_visitor(v, obj->syntax());
  }
  void operator()(const std::shared_ptr<oid_t> &obj) {
    os_ << *obj;

    for (const auto &child : *obj) {
      child_visitor v(tc_map_, os_);
      boost::apply_visitor(v, child);
    }
  }
  void operator()(const std::shared_ptr<ntype_t> &obj) { os_ << *obj; }
  void operator()(const std::shared_ptr<seq_t> &obj) {
    std::cout << "Not yet implemented";
  }

 private:
  std::reference_wrapper<std::map<std::string, tc_t>> tc_map_;
  std::reference_wrapper<std::ostream> os_;
};

class mib_writer {
 public:
  mib_writer(std::ostream &os, const std::shared_ptr<module_id_t> &mod_id)
      : os_(os) {
    os_.get() << mod_id->name() + " DEFINITIONS ::= BEGIN\n\n";
    os_.get() << declare_mib_import();

    write_module(mod_id);
    write_body(mod_id);
  }
  ~mib_writer() { os_.get() << "\nEND\n"; }

 private:
  void write_module(const std::shared_ptr<module_id_t> &mod_id) {
    os_.get() << *mod_id;
  }
  void write_body(const std::shared_ptr<module_id_t> &mod_id) {
    std::map<std::string, tc_t> tc_map;
    std::stringstream body_stream;

    body_stream << "-- Object Types\n";
    for (const auto &child : *mod_id) {
      child_visitor v(tc_map, body_stream);
      boost::apply_visitor(v, child);
    }

    /* tc_map to stream */
    std::stringstream tc_stream;
    tc_stream << "-- Textual Convention\n";
    for (const auto &tc : tc_map) {
      std::stringstream syntax_value;
      syntax_value << tc.second.value_;
      tc_stream << declare_textual_convention(
          tc.second.name_, tc.second.display_hint_, tc.second.description_,
          syntax_value.str());
    }

    /** tc and body to mib */
    std::ostreambuf_iterator<char> os_iter(os_);

    /* write tc */
    std::istreambuf_iterator<char> tc_iter(tc_stream.rdbuf());
    std::istreambuf_iterator<char> tc_eos;
    std::copy(tc_iter, tc_eos, os_iter);

    /* write body */
    std::istreambuf_iterator<char> body_iter(body_stream.rdbuf());
    std::istreambuf_iterator<char> body_eos;
    std::copy(body_iter, body_eos, os_iter);
  }

 private:
  std::reference_wrapper<std::ostream> os_;
};

}
