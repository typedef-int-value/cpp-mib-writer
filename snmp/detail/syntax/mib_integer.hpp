#pragma once
#include <map>
#include <iterator>

namespace snmp {
class integer_t {
 public:
  integer_t() {}

  template <typename ...Types>
  integer_t(const std::pair<int, std::string> &v, Types... args) {
    enum_.insert(v);
    insert(args...);
  }

  template <typename ...Types>
  void insert(const std::pair<int, std::string> &v, Types... args) {
    enum_.insert(v);
    insert(args...);
  }

  void insert(){};

  void insert(const int &num, const std::string &name) {
    enum_.insert(std::make_pair(num, name));
  }

  friend std::ostream &operator<<(std::ostream &os, const integer_t &integer);
  friend inline bool operator==(const integer_t &l, const integer_t &r);
 private:
  std::map<int, std::string> enum_;
};

inline std::ostream &operator<<(std::ostream &os, const integer_t &integer) {
  os << "INTEGER\n";
  os << "\t\t{\n";
  for (auto iter = integer.enum_.begin(); iter != integer.enum_.end(); ++iter) {
    os << "\t\t\t";
    os << iter->second << "(" << iter->first << ")";
    if (std::next(iter) != integer.enum_.end()) {
      os << ",";
    }
    os << "\n";
  }
  os << "\t\t}\n";
  
  return os;
}

inline bool operator==(const integer_t &l, const integer_t &r) { 
  return (l.enum_ == r.enum_);
}
}  // namespace snmp