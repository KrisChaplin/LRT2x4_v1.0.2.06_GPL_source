/* This file is automatically generated by configure.  Do not modify by hand. */
  if (should_init("proxy")) shutdown_proxy();
  if (should_init("notification_log")) shutdown_notification_log();
  if (should_init("snmpNotifyFilterTable")) shutdown_snmpNotifyFilterTable();
  if (should_init("ifTable")) shutdown_ifTable();
  if (should_init("ipAddressTable")) shutdown_ipAddressTable();
  if (should_init("inetNetToMediaTable")) shutdown_inetNetToMediaTable();
  if (should_init("ipSystemStatsTable")) shutdown_ipSystemStatsTable();
  //if (should_init("ipCidrRouteTable")) shutdown_ipCidrRouteTable();
  //if (should_init("inetCidrRouteTable")) shutdown_inetCidrRouteTable();
  if (should_init("tcpConnectionTable")) shutdown_tcpConnectionTable();
  if (should_init("tcpListenerTable")) shutdown_tcpListenerTable();
  if (should_init("udpEndpointTable")) shutdown_udpEndpointTable();
  if (should_init("cpu")) shutdown_cpu();
