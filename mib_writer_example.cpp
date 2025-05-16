#include <iostream>

#include "snmp/mib_conversion.hpp"
#include "snmp/mib_writer.hpp"

int main() {
  auto evalIndex = snmp::make_otype("evalIndex", 1);
  evalIndex->set_syntax(snmp::syntax::kInteger32);

  auto evalString = snmp::make_otype("evalString", 2);
  evalString->set_syntax(snmp::displaystring_t());

  auto evalValue = snmp::make_otype("evalStatus", 3);
  auto RowStatus = snmp::make_tc("RowStatus", snmp::octet_str_t(8));
  evalValue->set_syntax(RowStatus);

  auto seq = snmp::make_seq("EvalEntry");
  snmp::edge(seq, evalIndex);
  snmp::edge(seq, evalString);
  snmp::edge(seq, evalValue);

  auto evalEntry = snmp::make_otype("evalEntry", 1);
  evalEntry->set_access(snmp::access_type::kNotAccessible);
  evalEntry->set_syntax(seq);
  evalEntry->set_index(1, 1);

  auto evalTable = snmp::make_otype("evalTable", 1);
  evalTable->set_syntax(evalEntry);

  auto evalOid = snmp::make_oid("eval", 1);
  snmp::edge(evalOid, evalTable);

  auto eval_mod = snmp::make_module_id("evalModule", 1);
  eval_mod->set_contact_info("sb.kang@outlook.com");
  snmp::edge(eval_mod, evalOid);

  write_mib("eval.txt", eval_mod);

  return 0;
}