var LimitMultiSubnet=5;
var LimitQosRate = 100;	
var LimitQosPrio = 50;
var LimitForwarding = 30;
var LimitTriggering=30;
var LimitNATNumber = 10;
var LimitProtocolBinding = 100;
var LimitStaticIP = 253;
var LimitTrustedDomain = 50;
var LimitForbiddenDomains = 50;
var LimitKeywords = 50;
var LimitService = 100;
var LimitDhcpLocalDB = 100;
var LimitRouting=100;
var SupportNOIPDDNS = 1;
var SupportCHANGEIPDDNS = 1;
var SupportYIDDNS = 0;

var foldersTree = gFld("","","",1);
aux2 = insFld(foldersTree, gFld("2","network.htm","Setup",0));
insDoc(aux2, gLnk("1","network.htm","Network",0));
insDoc(aux2, gLnk("2","password.htm","Password",0));
insDoc(aux2, gLnk("3","time.htm","Time",0));
insDoc(aux2, gLnk("4","adv_dmz.htm","DMZ Host",0));
insDoc(aux2, gLnk("5","adv_forwarding.htm","Forwarding",0));
insDoc(aux2, gLnk("6","adv_upnp.htm","Port Address Translation",0));
insDoc(aux2, gLnk("7","adv_nat.htm","One-to-One NAT",0));
insDoc(aux2, gLnk("8","adv_mac.htm","MAC Address Clone",0));
insDoc(aux2, gLnk("9","adv_ddns.htm","Dynamic DNS",0));
insDoc(aux2, gLnk("10","adv_routing.htm","Advanced Routing",0));
insDoc(aux2, gLnk("12","smtp_config.htm","Outgoing Mail Server",0));
insDoc(aux2, gLnk("11","transition.htm","IPv6 Transition",0));
aux3 = insFld(foldersTree, gFld("3","dhcp_setup.htm","DHCP",0));
insDoc(aux3, gLnk("1","dhcp_setup.htm","DHCP Setup",0));
insDoc(aux3, gLnk("2","dhcp_status.htm","DHCP Status",0));
insDoc(aux3, gLnk("3","dhcp_ra.htm","Router Advertisement",0));
insDoc(aux3, gLnk("4","dhcp_static.htm","IP & MAC binding",0));
insDoc(aux3, gLnk("5","dhcp_dns.htm","DNS Local Database",0));
if(ModelName=="LRT214")
{
	aux4 = insFld(foldersTree, gFld("4","edit_sys_dualwan2.htm","System Management",0));
	insDoc(aux4, gLnk("1","edit_sys_dualwan2.htm","Network Service Detection",0));
}
else if(ModelName=="LRT224")
{
	aux4 = insFld(foldersTree, gFld("4","edit_sys_dualwan3.htm","System Management",0));
	insDoc(aux4, gLnk("1","edit_sys_dualwan3.htm","Multi-WAN",0));
}
else
{
	aux4 = insFld(foldersTree, gFld("4","sys_dualwan2.htm","System Management",0));
	insDoc(aux4, gLnk("1","sys_dualwan2.htm","Multi-WAN",0));
}
insDoc(aux4, gLnk("2","qos.htm","Bandwidth Management",0));
insDoc(aux4, gLnk("4","session_control.htm","Session Control",0));
insDoc(aux4, gLnk("3","sys_snmp.htm","SNMP",0));
//insDoc(aux4, gLnk("4","sys_diag.htm","Diagnostics",0));
//insDoc(aux4, gLnk("5","sys_factory.htm","Factory Default",0));
//insDoc(aux4, gLnk("6","sys_firmware.htm","Firmware Upgrade",0));
//insDoc(aux4, gLnk("7","sys_restart.htm","Restart",0));
//insDoc(aux4, gLnk("8","sys_setting.htm","Backup and Restore",0));
//insDoc(aux4, gLnk("10","Rebooting.htm","Rebooting",1));
insDoc(aux4, gLnk("11","ssl_cert.htm","SSL Certificate",0));
aux5 = insFld(foldersTree, gFld("5","lan_setting.htm","Port Management",0));
insDoc(aux5, gLnk("1","lan_setting.htm","Port Setup",0));
insDoc(aux5, gLnk("2","lan_status.htm","Port Status",0));
insDoc(aux5, gLnk("3","8021q_simple.htm","802.1Q",0));
aux6 = insFld(foldersTree, gFld("6","f_general.htm","Firewall",0));
insDoc(aux6, gLnk("1","f_general.htm","General",0));
insDoc(aux6, gLnk("2","access_rules.htm","Access Rules",0));
insDoc(aux6, gLnk("3","content_filter.htm","Content Filter",0));
aux7 = insFld(foldersTree, gFld("7","vpn_summary.htm","VPN",0));
insDoc(aux7, gLnk("1","vpn_summary.htm","Summary",0));
insDoc(aux7, gLnk("2","gateway_to_gateway.htm","Gateway To Gateway",0));
insDoc(aux7, gLnk("3","client_to_gateway_t.htm","Client To Gateway",0));
insDoc(aux7, gLnk("5","adv_through.htm","VPN Passthrough",0));
insDoc(aux7, gLnk("6","pptp.htm","PPTP Server",0));
aux10 = insFld(foldersTree, gFld("10","ezlinkvpn_summary.htm","EasyLink VPN",0));
insDoc(aux10, gLnk("1","ezlinkvpn_summary.htm","Summary",0));
insDoc(aux10, gLnk("2","ezlinkvpn_responder.htm","Inbound EasyLink VPN",0));
insDoc(aux10, gLnk("3","ezlinkvpn_initiator.htm","Outbound EasyLink VPN",0));
aux9 = insFld(foldersTree, gFld("9","openvpn_summary.htm","OpenVPN",0));
insDoc(aux9, gLnk("1","openvpn_summary.htm","Summary",0));
insDoc(aux9, gLnk("2","openvpn_server.htm","OpenVPN Server",0));
insDoc(aux9, gLnk("3","openvpn_client.htm","OpenVPN Client",0));
aux8 = insFld(foldersTree, gFld("8","log_setting.htm","Log",0));
insDoc(aux8, gLnk("1","log_setting.htm","System Log",0));
insDoc(aux8, gLnk("2","log_report.htm","System Statistics",0));
