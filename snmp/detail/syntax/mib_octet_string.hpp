#pragma once
#include <boost/variant.hpp>
#include <utility>
#include <vector>

namespace snmp {
class octet_str_t {
 public:
  typedef uint8_t fixed_t;
  typedef std::pair<uint8_t, uint8_t> min_max_t;
  typedef std::vector<fixed_t> fixed_var_t;
  typedef boost::variant<min_max_t, fixed_var_t> octet_size_t;

  octet_str_t() { size_ = min_max_t(0, 65535); };
  octet_str_t(const fixed_t &fix_len) { size_ = fixed_var_t{fix_len}; }
  octet_str_t(const min_max_t &min_max) { size_ = min_max; }
  octet_str_t(const fixed_var_t &sizes) { size_ = sizes; }

  friend std::ostream &operator<<(std::ostream &os, const octet_str_t &that);
  friend bool operator==(const octet_str_t &lhs, const octet_str_t &rhs);

 private:
  octet_size_t size_;
};
inline bool operator==(const octet_str_t &lhs, const octet_str_t &rhs) {
  return (lhs.size_ == rhs.size_);
}
inline bool operator!=(const octet_str_t &lhs, const octet_str_t &rhs) {
  return !(lhs == rhs);
}
inline std::ostream &operator<<(std::ostream &os, const octet_str_t &that) {
  class octet_visitor : public boost::static_visitor<> {
   public:
    octet_visitor(std::ostream &os) : stream_(os) {
      stream_.get() << "OCTET STRING";
    }
    void operator()(const octet_str_t::min_max_t &fixed) {
      if (!(fixed.first == 0 && fixed.second == 65535))
        stream_.get() << " (SIZE(" << std::to_string(fixed.first) << ".."
                      << std::to_string(fixed.second) << "))";
    }
    void operator()(const octet_str_t::fixed_var_t &fixed) {
      stream_.get() << " (SIZE(";
      if (fixed.size() == 1)
        stream_.get() << std::to_string(fixed[0]);
      else if (fixed.size() > 1) {
        for (const auto &v : fixed) {
          stream_.get() << std::to_string(v);
          if (&v != &fixed.back()) stream_.get() << " | ";
        }
      }
      stream_.get() << "))";
    }

    std::reference_wrapper<std::ostream> stream_;
  };
  octet_visitor v(os);
  boost::apply_visitor(v, that.size_);

  return os;
}

}  // namespace snmp