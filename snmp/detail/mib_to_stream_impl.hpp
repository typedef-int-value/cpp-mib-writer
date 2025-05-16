#pragma once
#include <sstream>
#include <string>

namespace snmp {

inline std::string declare_mib_import() {
  std::stringstream ss;
  ss << R"(IMPORTS
	enterprises, MODULE-IDENTITY, OBJECT-TYPE, NOTIFICATION-TYPE, Integer32,
	TimeTicks, IpAddress, Counter64 FROM SNMPv2-SMI 
    TEXTUAL-CONVENTION, TruthValue, DateAndTime, DisplayString FROM SNMPv2-TC 
    OBJECT-GROUP, MODULE-COMPLIANCE, NOTIFICATION-GROUP FROM SNMPv2-CONF;

)";

  return ss.str();
}

inline std::string declare_module_identity(const std::string &oid,
                                           const std::string &module_name,
                                           const std::string &last_updated,
                                           const std::string &organization,
                                           const std::string &contact_info,
                                           const std::string &description) {
#if 0
  std::string sep_oid[10];
  int loop = 0;
  boost::char_separator<char> sep(".");
  boost::tokenizer<boost::char_separator<char>> tokens(oid, sep);

  for (const auto &t : tokens) {
    sep_oid[loop] = t;
    loop++;
  }

  std::string module_identity;
  module_identity = "\n" + module_name + " MODULE-IDENTITY\n";

  boost::format mib_module_identity(
      "\tLAST-UPDATED \"%1%\"\n\tORGANIZATION "
      "\"%2%\"\n\tCONTACT-INFO\n\t\t\"\n\t\t%3%\n\t\t\"\n\tDESCRIPTION\n\t\t\"%"
      "4%\"\n");
  /*declare_module_identity % "201902220000Z";
  declare_module_identity % "GS Teletech Inc.";
  declare_module_identity % "HoChan Lee\n\t\thclee677@gsinstech.com";
  declare_module_identity %
      "Copyright (C) GS Teletech Inc. (2019). All rights reserved.";*/

  mib_module_identity % last_updated;
  mib_module_identity % organization;
  mib_module_identity % contact_info;
  mib_module_identity % description;

  boost::format mib_module_identity_link(
      "\n::= { enterprises %1% %2% %3% }\n\n");
  mib_module_identity_link % sep_oid[0];
  mib_module_identity_link % sep_oid[1];
  mib_module_identity_link % sep_oid[2];

  module_identity = module_identity + mib_module_identity.str() +
                    mib_module_identity_link.str();
  return module_identity;
#else
  std::stringstream ss;
  ss << module_name << " MODULE-IDENTITY\n";
  ss << "\tLAST-UPDATED \"" << last_updated << "\"\n";
  ss << "\tORGANIZATION \"" << organization << "\"\n";
  ss << "\tCONTACT-INFO\n";
  ss << "\t\t\"\n";
  ss << "\t\t" << contact_info << "\n";
  ss << "\t\t\"\n";
  ss << "\tDESCRIPTION\n";
  ss << "\t\t\""<< description << "\"\n";
  
  return ss.str();
#endif
}

inline std::string declare_object_identifier(const std::string &oid_num,
                                             const std::string &obj_name,
                                             const std::string &ref_obj_name) {
#if 0
  boost::format mib_object_identity_link(
      "%1%\t\tOBJECT IDENTIFIER ::= { %2% %3% }\n");
  mib_object_identity_link % obj_name;
  mib_object_identity_link % ref_obj_name;
  mib_object_identity_link % oid_num;

  return mib_object_identity_link.str();
#else
  std::stringstream ss;

  ss << obj_name << "\t\t";
  ss << "OBJECT IDENTIFIER ::= { ";
  ss << ref_obj_name << " ";
  ss << oid_num << " }\n";

  return ss.str();
#endif
}

inline std::string link(const std::string &parent_name,
                        const std::string &oid) {
#if 0
  boost::format mib_object_type_link("\t::= { %1% %2% }\n");
  mib_object_type_link % parent_name;
  mib_object_type_link % oid;

  return mib_object_type_link.str();
#else
  std::stringstream ss;
  ss << "\t::= { ";
  ss << parent_name << " ";
  ss << oid << " }\n\n";

  return ss.str();
#endif
}

inline std::string declare_object_type(
    const std::string &oid_num, const std::string &obj_name,
    const std::string &syntax, const std::string &units,
    const std::string &access, const std::string &desc,
    const std::vector<std::string> &indexes) {
#if 0
  std::string object_type;
  std::string parse_units;
  object_type = "\n" + obj_name + "\tOBJECT-TYPE\n";
  if (units != "") {
    parse_units = "\tUNITS\t\t\"" + units + "\"\n";
  }

  boost::format mib_object_type(
      "\tSYNTAX\t\t%1%\n%2%\tMAX-ACCESS\t%3%"
      "\n\tSTATUS\t\tcurrent\n\tDESCRIPTION\n\t\t\"%4%\"\n");
  mib_object_type % syntax;
  mib_object_type % parse_units;
  mib_object_type % access;
  mib_object_type % desc;

  object_type += mib_object_type.str();

  if (!indexes.empty()) {
    object_type += "\tINDEX\t{ ";

    for (const auto &index : indexes) {
      object_type += index;
      if (&index != &indexes.back()) {
        object_type += ", ";
      }
    }
    object_type += " }\n";
  }

  return object_type;
#else
  std::stringstream ss;
  ss << obj_name << "\tOBJECT-TYPE\n";
  ss << "\tSYNTAX\t\t" << syntax << "\n";
  if (!units.empty())  ss << "\tUNITS\t\t" << units << "\n";
  ss << "\tMAX-ACCESS\t" << access << "\n";
  ss << "\tSTATUS\t\tcurrent\n";
  ss << "\tDESCRIPTION\n";
  ss << "\t\t\"" << desc << "\"\n";

  if (!indexes.empty()) {
    ss << "\tINDEX\t{ ";
    for (const auto &index : indexes) {
      ss << index;
      if (&index != &indexes.back()) {
        ss << ", ";
      }
    }
    ss << " }\n";
  }

  return ss.str();
#endif
}

inline std::string declare_object_type(
    const std::string &oid_num, const std::string &obj_name,
    const std::string &syntax, const std::string &units,
    const std::string &access, const std::string &desc,
    const std::vector<std::string> &indexes, const std::string &ref_obj_name) {
  auto object_type = declare_object_type(oid_num, obj_name, syntax, units,
                                         access, desc, indexes);

  object_type += link(ref_obj_name, oid_num);

  return object_type;
}

inline std::string declare_textual_convention(const std::string &obj_name,
                                              const std::string &display_hint,
                                              const std::string &desc,
                                              const std::string &syntex) {
#if 0
  std::string textual_convenction;
  textual_convenction = "\n" + obj_name + " ::= TEXTUAL-CONVENTION\n";
  if (display_hint != "") {
    boost::format mib_display_hint("\tDISPLAY-HINT\t\"%1%\"\n");
    mib_display_hint % display_hint;
    textual_convenction += mib_display_hint.str();
  }

  boost::format mib_textual_convenction(
      "\tSTATUS\t\tcurrent\n\tDESCRIPTION\n\t\t\"%1%\"\n\tSYNTAX\t\t%2%"
      "\n\t\t\n");
  mib_textual_convenction % desc;
  mib_textual_convenction % syntex;

  textual_convenction = textual_convenction + mib_textual_convenction.str();

  return textual_convenction;
#else
  std::stringstream ss;
  ss << obj_name << " ::= TEXTUAL-CONVENTION\n";
  if (!display_hint.empty())
    ss << "\tDISPLAY-HINT\t\"" << display_hint << "\"\n";

  ss << "\tSTATUS\t\tcurrent\n";
  ss << "\tDESCRIPTION\n";
  ss << "\t\t\"" << desc << "\"\n";
  ss << "\tSYNTAX\t\t" << syntex << "\n\n";

  return ss.str();
#endif
}

inline std::string declare_notification_type(
    const std::string &name, const std::string &oid, const std::string &desc,
    const std::string &ref_name, const std::vector<std::string> &objects) {
  std::stringstream ntype_stream;
  ntype_stream << name << "\n\tNOTIFICATION-TYPE\n";
  ntype_stream << "\tOBJECTS\n";
  ntype_stream << "\t{\n";
  ntype_stream << "\t\t";
  for (const auto &o : objects) {
    ntype_stream << o;
    if (&o != &objects.back()) ntype_stream << ", ";
  }
  ntype_stream << "\n\t}\n";

  ntype_stream << "\tSTATUS current\n";
  ntype_stream << "\tDESCRIPTION\n";
  ntype_stream << "\t\t\"" << desc << "\"\n";

  ntype_stream << link(ref_name, oid);

  return ntype_stream.str();
}

}  // namespace snmp