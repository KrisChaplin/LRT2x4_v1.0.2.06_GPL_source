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

var foldersTree = gFld("","","",1);

aux4 = insFld(foldersTree, gFld("4","sys_diag.htm","Maintenance",0));
insDoc(aux4, gLnk("4","sys_diag.htm","Diagnostics",0));
insDoc(aux4, gLnk("5","sys_factory.htm","Factory Default",0));
insDoc(aux4, gLnk("6","sys_firmware.htm","Firmware Upgrade",0));
insDoc(aux4, gLnk("7","sys_restart.htm","Restart",0));
insDoc(aux4, gLnk("8","sys_setting.htm","Backup and Restore",0));
insDoc(aux4, gLnk("10","Rebooting.htm","Rebooting",1));

