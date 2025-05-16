#include <iostream>
#include <vector>

#include "snmp/mib_conversion.hpp"
#include "snmp/mib_writer.hpp"

struct object_types {
  std::string name;
  snmp::access_type access;
  snmp::syntax_type syntax;
  std::string desc = "";
};

void make_connectivity(std::shared_ptr<snmp::oid_t> gst_acs) {
  std::vector<object_types> children = {
      {"connectivityEMS", snmp::access_type::kReadOnly, snmp::displaystring_t()},
      {"connectivityDomain", snmp::access_type::kReadOnly, snmp::displaystring_t()},
      {"connectivityVendor", snmp::access_type::kReadOnly, snmp::displaystring_t()},
      {"connectivityVLANVRF", snmp::access_type::kReadOnly, snmp::displaystring_t()},
      {"connectivityLBIP", snmp::access_type::kReadOnly, snmp::syntax::kIpAddress},
      {"connectivityFQDN", snmp::access_type::kReadOnly, snmp::displaystring_t()},
      {"connectivityEMSIP", snmp::access_type::kReadOnly, snmp::syntax::kIpAddress},
      {"connectivityEMSSegment", snmp::access_type::kReadOnly, snmp::displaystring_t()},
      {"connectivityProtocol", snmp::access_type::kReadOnly, snmp::displaystring_t()},
      {"connectivityCommunityString", snmp::access_type::kReadOnly, snmp::displaystring_t()},
      {"connectivityExtraDetailForSNMPv3", snmp::access_type::kReadOnly, snmp::displaystring_t()},
      {"connectivityConnectivityStatus", snmp::access_type::kReadOnly, snmp::displaystring_t()},
      {"connectivityRemark", snmp::access_type::kReadWrite, snmp::displaystring_t()},
  };

  auto connectivity = snmp::make_oid("connectivity", 1);
  for (auto i = 0; i < children.size(); ++i) {
    auto& child = children[i];
    auto otype = snmp::make_otype(child.name, i+1);
    otype->set_access(child.access);
    otype->set_syntax(child.syntax);
    snmp::edge(connectivity, otype);
  }

  snmp::edge(gst_acs, connectivity);
}

void make_alarm_list(std::shared_ptr<snmp::oid_t> gst_acs) {
  /* create the entry type: mib has no array. */
  auto alarm_table_entry = snmp::make_seq("alarmEntryType");
  std::vector<object_types> children_of_alarm_table = {
    {"alarmNEHostName", snmp::access_type::kReadOnly, snmp::displaystring_t()},
    {"alarmAlarmIdentifier", snmp::access_type::kReadOnly, snmp::displaystring_t()},
    {"alarmSeverity", snmp::access_type::kReadOnly, snmp::displaystring_t()},
    {"alarmStatus", snmp::access_type::kReadOnly, snmp::displaystring_t()},
    {"alarmEventOccurenceTime", snmp::access_type::kReadOnly, snmp::displaystring_t()},
    {"alarmInstanceId", snmp::access_type::kReadOnly, snmp::displaystring_t()},
    {"alarmSequenceNumber", snmp::access_type::kReadOnly, snmp::syntax::kUnsigned32},
    {"alarmDescription", snmp::access_type::kReadOnly, snmp::displaystring_t()},
    {"alarmPortDetail", snmp::access_type::kReadOnly, snmp::displaystring_t()},
  };
    
  auto alarm_index = snmp::make_otype("alarmIndex", 1);
  alarm_index->set_syntax(snmp::syntax::kInteger32);
  alarm_index->set_access(snmp::access_type::kReadOnly);
  alarm_index->set_description("index of alarm in alarm table.");

  snmp::edge(alarm_table_entry, alarm_index);
  for (auto i = 0; i < children_of_alarm_table.size(); ++i) {
    auto child = snmp::make_otype(children_of_alarm_table[i].name, i + 2);
    child->set_syntax(children_of_alarm_table[i].syntax);
    child->set_access(children_of_alarm_table[i].access);
    
    snmp::edge(alarm_table_entry, child);
  }
  
  auto alarm_entry_type = snmp::make_otype("alarmEntryType", 1);
  alarm_entry_type->set_syntax(alarm_table_entry);
  alarm_entry_type->set_access(snmp::access_type::kReadOnly);
  alarm_entry_type->set_index(1, 1);

  auto alarm_table = snmp::make_otype("alarmTable", 2);
  alarm_table->set_syntax(alarm_entry_type);
  alarm_table->set_access(snmp::access_type::kReadOnly);

  /* make alarm oid: 10000.2. */
  auto alarm = snmp::make_oid("alarm", 2);
  snmp::edge(gst_acs, alarm);

  /* make total count of table: 10000.2.1. */
  auto alarm_total_count = snmp::make_otype("alarmTableNumber", 1);
  alarm_total_count->set_syntax(snmp::syntax::kUnsigned32);
  alarm_total_count->set_access(snmp::access_type::kReadOnly);
  snmp::edge(alarm, alarm_total_count);

  /* alarm sequence number: 10000.2.3. */
#if 0
  auto alarm_sequence_number = snmp::make_otype("alarmSequenceNumber", 3);
  alarm_sequence_number->set_access(snmp::access_type::kReadOnly);
  alarm_sequence_number->set_syntax(snmp::syntax::kUnsigned32);
  snmp::edge(alarm, alarm_sequence_number);
#endif
  /* oss alarm sequence number: 10000.2.4. (for resync the alarm) */
  auto alarm_oss_sequence_number = snmp::make_otype("alarmOSSSequenceNumber", 4);
  alarm_oss_sequence_number->set_access(snmp::access_type::kReadWrite);
  alarm_oss_sequence_number->set_syntax(snmp::syntax::kUnsigned32);
  snmp::edge(alarm, alarm_oss_sequence_number);

  /* make table: 10000.2.2. */
  snmp::edge(alarm, alarm_table);
}

void make_trap(std::shared_ptr<snmp::oid_t> gst_acs) { 
  std::vector<object_types> v = {
    {"trapNEHostName", snmp::access_type::kReadOnly, snmp::displaystring_t()},
    {"trapAlarmIdentifier", snmp::access_type::kReadOnly, snmp::displaystring_t()},
    {"trapSeverity", snmp::access_type::kReadOnly, snmp::displaystring_t()},
    {"trapStatus", snmp::access_type::kReadOnly, snmp::displaystring_t()},
    {"trapEventOccurenceTime", snmp::access_type::kReadOnly, snmp::displaystring_t()},
    {"trapInstanceId", snmp::access_type::kReadOnly, snmp::displaystring_t()},
    {"trapSequenceNumber", snmp::access_type::kReadOnly, snmp::syntax::kUnsigned32},
    {"trapDescription", snmp::access_type::kReadOnly, snmp::displaystring_t()},
    {"trapPortDetail", snmp::access_type::kReadOnly, snmp::displaystring_t()},
  };

  auto trap = snmp::make_oid("trap", 3);
  snmp::edge(gst_acs, trap);
  auto trap_alarm = snmp::make_ntype("trapAlarm", v.size() + 1);
  auto trap_alarm_cleared = snmp::make_ntype("trapAlarmCleared", v.size() + 2);

  for (auto i = 0; i < v.size(); ++i) {
    auto otype = snmp::make_otype(v[i].name, i + 1);
    otype->set_syntax(v[i].syntax);
    snmp::edge(trap, otype);
    trap_alarm->objects_.push_back(v[i].name);
    trap_alarm_cleared->objects_.push_back(v[i].name);
  }
  snmp::edge(trap, trap_alarm);
  snmp::edge(trap, trap_alarm_cleared);
}

void make_heartbeat(std::shared_ptr<snmp::oid_t> gst_acs) {
  auto heartbeat = snmp::make_oid("heartbeat", 4);
  snmp::edge(gst_acs, heartbeat);

  std::vector<object_types> v = {
    {"heartbeatDestIP1", snmp::access_type::kReadWrite, snmp::syntax::kIpAddress},
    {"heartbeatDestIP2", snmp::access_type::kReadWrite, snmp::syntax::kIpAddress},
    {"heartbeatEnable", snmp::access_type::kReadWrite, snmp::syntax::kInteger32},
    {"heartbeatInterval", snmp::access_type::kReadWrite, snmp::syntax::kUnsigned32},
  };

  
  auto n_type = snmp::make_ntype("heartbeatNotification", 5);
  snmp::edge(heartbeat, n_type);
  for (auto i = 0; i < v.size(); ++i) {
    auto o_type = snmp::make_otype(v[i].name, i + 1);
    o_type->set_access(v[i].access);
    o_type->set_syntax(v[i].syntax);
    n_type->objects_.push_back(v[i].name);
    snmp::edge(heartbeat, o_type);
  }
}

int main() {
  /* create root node */
  auto mod_id = snmp::make_module_id("mod", 24793);
  mod_id->set_contact_info("sb.kang@outlook.com");
  mod_id->set_organization("myorg");
  //mod_id->set_last_updated("20210215");
  mod_id->set_parent_name("enterprises");
  
  auto myoid = snmp::make_oid("myoid", 10000);
  snmp::edge(mod_id, myoid);

  /* create oid for connectivity */
  make_connectivity(myoid);

  make_alarm_list(myoid);

  /* create trap */
  make_trap(myoid);

  /* heartbeat */
  make_heartbeat(myoid);

  /* export */
  snmp::write_mib("example2.txt", mod_id);
  return 0;
}
