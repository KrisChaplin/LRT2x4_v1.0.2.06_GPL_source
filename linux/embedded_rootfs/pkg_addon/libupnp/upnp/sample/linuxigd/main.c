#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <syslog.h>
#include <signal.h>
#include <sys/types.h>
#include <unistd.h>
#include <sys/stat.h>
#include <sys/resource.h>
#include <time.h>
#include <net/if.h>
#include <upnp/upnp.h>
#include "globals.h"
#include "config.h"
#include "gatedevice.h"
#include "util.h"
#include "pmlist.h"

// Global variables
struct GLOBALS g_vars;

/* purpose : 0017099 author: incifer date : 2013-04-25      */
/* description : fix upnp gatedesc.xml                      */
/*               config gatedesc_model.xml                  */
/*               sync ca4                                   */
void config_desc ( void )
{
    int switch_type = Get_Switch_Type();
    char filename[PATH_LEN], cmd[128];
    char manufacturer[PATH_LEN], manufacturerurl[PATH_LEN],
         modelname[PATH_LEN], modelnumber[PATH_LEN],
         serialnumber[PATH_LEN], devicewebpage[PATH_LEN];
    FILE *fd;

    if ( !switch_type )
    {
        return;
    }

    sprintf ( filename, "/etc/linuxigd/gatedesc_%d.xml", switch_type );
    sprintf ( g_vars.descDocName, "gatedesc_%d.xml", switch_type );

    switch ( switch_type )
    {
        case 214:
            strcpy ( manufacturer, "Linksys, LLC." );
            strcpy ( manufacturerurl, "www.linksys.com" );
            strcpy ( modelname, "LRT214 Gigabit VPN Router" );
            strcpy ( modelnumber, "LRT214" );
            break;
        case 224:
            strcpy ( manufacturer, "Linksys, LLC." );
            strcpy ( manufacturerurl, "www.linksys.com" );
            strcpy ( modelname, "RT224 Dual WAN Gigabit VPN Router" );
            strcpy ( modelnumber, "LRT224" );
            break;
    }
    kd_doCommand ( "VERSION SERIALNO", 3, 0, serialnumber );
    kd_doCommand ( "SYSTEM LAN", 3, 0, devicewebpage );

    sprintf ( cmd, "cp -f /etc/linuxigd/gatedesc.xml %s", filename );
    system ( cmd );

    sprintf ( cmd, "sed -i 's/Linux Internet Gateway Device/%s/g' %s", modelnumber, filename );
    system ( cmd ); /* friendly name */

    sprintf ( cmd, "sed -i 's/Linux UPnP IGD Project/%s/g' %s", manufacturer, filename );
    system ( cmd ); /* manufacturer */

    sprintf ( cmd, "sed -i 's/linux-igd.sourceforge.net/%s/g' %s", manufacturerurl, filename );
    system ( cmd ); /* manufacturerURL */

    sprintf ( cmd, "sed -i 's/IGD Version 1.00/%s/g' %s", modelname, filename );
    system ( cmd ); /* modelName */

    sprintf ( cmd, "sed -i 's/model number/%s/g' %s", modelnumber, filename );
    system ( cmd ); /* modelNumber */

    sprintf ( cmd, "sed -i 's/serial number/%s/g' %s", serialnumber, filename );
    system ( cmd ); /* serial number */

    sprintf ( cmd, "sed -i 's/device webpage/%s/g' %s", devicewebpage, filename );
    system ( cmd ); /* device webpage */
}

int main (int argc, char** argv)
{
	char descDocUrl[7+15+1+5+1+sizeof(g_vars.descDocName)+1]; // http://ipaddr:port/docName<null>
	char intIpAddress[16];     // Server internal ip address
	sigset_t sigsToCatch;
	int ret, signum, arg = 1, foreground = 0;

	if (argc < 3 || argc > 4) {
	  printf("Usage: upnpd [-f] <external ifname> <internal ifname>\n");
	  printf("  -f\tdon't daemonize\n");
	  printf("Example: upnpd ppp0 eth0\n");
	  exit(0);
	}

	parseConfigFile(&g_vars);
/* purpose : 0017099 author: incifer date : 2013-04-25      */
/* description : fix upnp gatedesc.xml                      */
/*               sync ca4                                   */
	config_desc();

	// check for '-f' option
	if (strcmp(argv[arg], "-f") == 0) {
		foreground = 1;
		arg++;
	}

	// Save interface names for later use
	strncpy(g_vars.extInterfaceName, argv[arg++], IFNAMSIZ);
	strncpy(g_vars.intInterfaceName, argv[arg++], IFNAMSIZ);

	// Get the internal ip address to start the daemon on
	if (GetIpAddressStr(intIpAddress, g_vars.intInterfaceName) == 0) {
		fprintf(stderr, "Invalid internal interface name '%s'\n", g_vars.intInterfaceName);
		exit(EXIT_FAILURE);
	}

	if (!foreground) {
		struct rlimit resourceLimit = { 0, 0 };
		pid_t pid, sid;
		unsigned int i;

		// Put igd in the background as a daemon process.
		pid = fork();
		if (pid < 0)
		{
			perror("Error forking a new process.");
			exit(EXIT_FAILURE);
		}
		if (pid > 0)
			exit(EXIT_SUCCESS);

		// become session leader
		if ((sid = setsid()) < 0)
		{
			perror("Error running setsid");
			exit(EXIT_FAILURE);
		}

		// close all file handles
		resourceLimit.rlim_max = 0;
		ret = getrlimit(RLIMIT_NOFILE, &resourceLimit);
		if (ret == -1) /* shouldn't happen */
		{
		    perror("error in getrlimit()");
		    exit(EXIT_FAILURE);
		}
		if (0 == resourceLimit.rlim_max)
		{
		    fprintf(stderr, "Max number of open file descriptors is 0!!\n");
		    exit(EXIT_FAILURE);
		}	
		for (i = 0; i < resourceLimit.rlim_max; i++)
		    close(i);
	
		// fork again so child can never acquire a controlling terminal
		pid = fork();
		if (pid < 0)
		{
			perror("Error forking a new process.");
			exit(EXIT_FAILURE);
		}
		if (pid > 0)
			exit(EXIT_SUCCESS);
	
		if ((chdir("/")) < 0)
		{
			perror("Error setting root directory");
			exit(EXIT_FAILURE);
		}
	}

	umask(0);

// End Daemon initialization

	openlog("upnpd", LOG_CONS | LOG_NDELAY | LOG_PID | (foreground ? LOG_PERROR : 0), LOG_LOCAL6);

	// Initialize UPnP SDK on the internal Interface
	trace(3, "Initializing UPnP SDK ... ");
	if ( (ret = UpnpInit(intIpAddress,0) ) != UPNP_E_SUCCESS)
	{
		syslog (LOG_ERR, "Error Initializing UPnP SDK on IP %s ",intIpAddress);
		syslog (LOG_ERR, "  UpnpInit returned %d", ret);
		UpnpFinish();
		exit(1);
	}
	trace(2, "UPnP SDK Successfully Initialized.");

	// Set the Device Web Server Base Directory
	trace(3, "Setting the Web Server Root Directory to %s",g_vars.xmlPath);
	if ( (ret = UpnpSetWebServerRootDir(g_vars.xmlPath)) != UPNP_E_SUCCESS )
	{
		syslog (LOG_ERR, "Error Setting Web Server Root Directory to: %s", g_vars.xmlPath);
		syslog (LOG_ERR, "  UpnpSetWebServerRootDir returned %d", ret); 
		UpnpFinish();
		exit(1);
	}
	trace(2, "Succesfully set the Web Server Root Directory.");

	//initialize the timer thread for expiration of mappings
	if (ExpirationTimerThreadInit()!=0) {
	  syslog(LOG_ERR,"ExpirationTimerInit failed");
	  UpnpFinish();
	  exit(1);
	}

	// Form the Description Doc URL to pass to RegisterRootDevice
	sprintf(descDocUrl, "http://%s:%d/%s", UpnpGetServerIpAddress(),
				UpnpGetServerPort(), g_vars.descDocName);

	// Register our IGD as a valid UPnP Root device
	trace(3, "Registering the root device with descDocUrl %s", descDocUrl);
	if ( (ret = UpnpRegisterRootDevice(descDocUrl, EventHandler, &deviceHandle,
					   &deviceHandle)) != UPNP_E_SUCCESS )
	{
		syslog(LOG_ERR, "Error registering the root device with descDocUrl: %s", descDocUrl);
		syslog(LOG_ERR, "  UpnpRegisterRootDevice returned %d", ret);
		UpnpFinish();
		exit(1);
	}

	trace(2, "IGD root device successfully registered.");
	
	// Initialize the state variable table.
	StateTableInit(descDocUrl);
	
	// Record the startup time, for uptime
	startup_time = time(NULL);
	
	// Send out initial advertisements of our device's services with timeouts of 30 minutes
	if ( (ret = UpnpSendAdvertisement(deviceHandle, 1800) != UPNP_E_SUCCESS ))
	{
		syslog(LOG_ERR, "Error Sending Advertisements.  Exiting ...");
		UpnpFinish();
		exit(1);
	}
	trace(2, "Advertisements Sent.  Listening for requests ... ");
	
	// Loop until program exit signals received
	do {
	  sigemptyset(&sigsToCatch);
	  sigaddset(&sigsToCatch, SIGINT);
	  sigaddset(&sigsToCatch, SIGTERM);
	  sigaddset(&sigsToCatch, SIGUSR1);
	  pthread_sigmask(SIG_SETMASK, &sigsToCatch, NULL);
	  sigwait(&sigsToCatch, &signum);
	  trace(3, "Caught signal %d...\n", signum);
	  switch (signum) {
	  case SIGUSR1:
	    DeleteAllPortMappings();
	    break;
	  default:
	    break;
	  }
	} while (signum!=SIGTERM && signum!=SIGINT);

	trace(2, "Shutting down on signal %d...\n", signum);

	// Cleanup UPnP SDK and free memory
	DeleteAllPortMappings();
	ExpirationTimerThreadShutdown();

	UpnpUnRegisterRootDevice(deviceHandle);
	UpnpFinish();

	// Exit normally
	return (0);
}
