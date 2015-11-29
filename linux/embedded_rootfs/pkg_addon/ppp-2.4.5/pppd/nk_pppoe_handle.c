#include <stdio.h>
#include <ctype.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <signal.h>
#include <errno.h>
#include <fcntl.h>
#include <syslog.h>
#include <netdb.h>
#include <utmp.h>
#include <pwd.h>
#include <setjmp.h>
#include <sys/param.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/time.h>
#include <sys/resource.h>
#include <sys/stat.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <nkutil.h>
#include "pppd.h"
#include "fsm.h"
#include "ipcp.h"
#include <time_zones.h>
#include <time.h>
/* shared memory */
int	ClientStatusIdx = 0;
int ClientConnIdx = 0;
int IPInfoIdx = 0;
int ServerInfoidx = 0;
/* User data point */
int UserIdx = -1;

u_char key_w[512];

userConn *ClientStatus = NULL;
ClientInfo *ClientConn = NULL;
IPInfo *IPInfoTable = NULL;
ServerSetting *ServerInfo = NULL;

void *shared_memory = (void *)0;
void *shared_memory2 = (void *)0;
void *shared_memory3 = (void *)0;
void *shared_memory4 = (void *)0;

/* alloc memory */
void nk_CreateClientStatus(void)
{		
	ClientStatusIdx = shmget((key_t)1234, sizeof(userConn), 0666 );
	
	if(ClientStatusIdx == -1)
	{
		fatal("create memory failed for Client Status\n");
	}
	
}

void nk_CreateClientInfo(void)
{
	ClientConnIdx = shmget((key_t)2234, sizeof(ClientInfo), 0666 );
	
	if(ClientConnIdx == -1)
	{
		fatal("create memory failed for Client Info\n");
	}
	
}

void nk_CreateIPInfo(void)
{
	IPInfoIdx = shmget((key_t)3234, sizeof(IPInfo), 0666 );
	
	if(IPInfoIdx == -1)
	{
		fatal("create memory failed for IP Info\n");	
	}
	
}
void nk_CreateServerSettingInfo(void)
{	
	ServerInfoidx = shmget((key_t)4234, sizeof(ServerSetting), 0666 );
	if(ServerInfoidx == -1)
	{
		fatal("create memory failed for Server Info");
	}
}
/* point to shared memory */
void nk_GetClientStatuslink(int reCreate)
{
ClientStatusRetry:

	shared_memory = shmat(ClientStatusIdx, (void*)0, 0);
	
	if(shared_memory == (void *)-1){
		if(!reCreate){
			fatal("memory mapping failed for Client Status");
		}
		else
		{
			nk_CreateClientStatus();
			reCreate = 0;
			goto ClientStatusRetry;
		}
	}
	
	ClientStatus = (userConn *) shared_memory;
	
	if(!ClientStatus)
	{
		if(!reCreate){
			fatal("memory malloc failed for Client Status");
		}
		else{
			nk_CreateClientStatus();
			reCreate = 0;
			goto ClientStatusRetry;
		}
	}
}

void nk_GetClientInfolink(int reCreate)
{
ClientInfoRetry:

	shared_memory2 = shmat(ClientConnIdx, (void*)0, 0);
	
	if(shared_memory2 == (void *)-1){
		if(!reCreate)
		{
			fatal("memory mapping failed for Client Info");
		}
		else
		{
			nk_CreateClientInfo();
			reCreate = 0;
			goto ClientInfoRetry;
		}
	}
	
	ClientConn = (ClientInfo *) shared_memory2;
	
	if(!ClientConn){
		if(!reCreate)
		{
			fatal("memory malloc failed for Client Info");
		}
		else
		{
			nk_CreateClientInfo();
			reCreate = 0;
			goto ClientInfoRetry;
		}
	}
}

void nk_GetIPInfolink(int reCreate)
{
IPInfoRetry:

	shared_memory3 = shmat(IPInfoIdx, (void*)0, 0);
	
	if(shared_memory3 == (void *)-1){
		if(!reCreate)
		{
			fatal("memory mapping failed for IP Info");
		}
		else
		{
			nk_CreateIPInfo();
			reCreate = 0;
			goto IPInfoRetry;
		}
	}
	
	IPInfoTable = (IPInfo *) shared_memory3;
	
	if(!IPInfoTable)
	{
		if(!reCreate)
			fatal("memory malloc failed for IP Info");
		else
		{
			nk_CreateIPInfo();
			reCreate = 0;
			goto IPInfoRetry;
		}
		
	}
	

	
}

void nk_GetServerInfolink(int reCreate)
{

ServerInfoRetry:

	shared_memory4 = shmat(ServerInfoidx, (void*)0, 0);

	if(shared_memory4 == (void *)-1){
		if(!reCreate)
		{
			fatal("memory mapping failed for Server Info\n");
		}
		else
		{
			nk_CreateServerSettingInfo();
			reCreate = 0;
			goto ServerInfoRetry;
		}
	}
	
	ServerInfo = (ServerSetting *) shared_memory4;
	
	if(!ServerInfo)
	{
		if(!reCreate)
			fatal("memory malloc failed for Server Info");
		else
		{
			nk_CreateServerSettingInfo();
			reCreate = 0;
			goto ServerInfoRetry;
		}
	}
}
void nk_DisconnClientStatuslink(void)
{
	if(shmdt(shared_memory) == -1){
		warn("remove memory link failed for Client Status Info\n");
	}
}

void nk_DisconnClientInfolink(void)
{
    if(shmdt(shared_memory2) == -1){
		warn("remove memory link failed for IP Client Info\n");
	}
}

void nk_DisconnIPInfolink(void)
{
    if(shmdt(shared_memory3) == -1){
		warn("remove memory link failed for IP Info\n");
	}
}
void nk_DisconnServerInfolink(void)
{
    if(shmdt(shared_memory4) == -1){
		warn("remove memory link failed for Server Info Info\n");
	}
}
/* show Memory Data */
void nk_displaySharedMemData(void)
{
	
	int k=0;

	nk_GetClientStatuslink(1);
	nk_GetClientInfolink(1);
	nk_GetIPInfolink(1);
	nk_GetServerInfolink(1);
	
	notice("ServerInfo.StartIp %s", ServerInfo->StartIp);
	notice("ServerInfo.MaxConnClient %d", ServerInfo->MaxConnClient);
		
	for(k=0;ClientStatus[k].SaveData == 1;k++)
	{
		notice("ClientStatus[%d].username[%s]", k, ClientStatus[k].username);
		notice("ClientStatus[%d].MaxConn[%d]", k, ClientStatus[k].MaxConn);
		notice("ClientStatus[%d].Conn[%d]", k, ClientStatus[k].Conn);
		notice("ClientStatus[%d].IsValid[%d]", k, ClientStatus[k].IsValid);
		notice("ClientStatus[%d].validtime[%ld]", k, ClientStatus[k].validtime);
	}

	for(k=0;ClientConn[k].SaveData == 1;k++)
	{
		notice("ClientConn[%d].username[%s]", k, ClientConn[k].username);
		notice("ClientConn[%d].eth[%s]", k, ClientConn[k].eth);
		notice("ClientConn[%d].ifname[%s]", k, ClientConn[k].ifname);
		notice("ClientConn[%d].pid[%d]", k, ClientConn[k].pid);
	}

	for(k=0; k<(ServerInfo->MaxConnClient + skipMultiSubnet); k++)
	{
		notice("IPInfoTable[%d].peerip [%s] ", k, IPInfoTable[k].peerip);
		notice("IPInfoTable[%d].IsAssign [%d] ", k, IPInfoTable[k].IsAssign);
		notice("IPInfoTable[%d].IsUsed [%d] ", k, IPInfoTable[k].IsUsed);
		notice("IPInfoTable[%d].IsLock [%d] ", k, IPInfoTable[k].IsLock);
		notice("IPInfoTable[%d].IsInvalid [%d] ", k, IPInfoTable[k].IsInvalid);
		notice("IPInfoTable[%d].LockTTL [%ld] ", k, IPInfoTable[k].LockTTL);
	}
	
	nk_DisconnServerInfolink();
	nk_DisconnClientStatuslink();
    nk_DisconnClientInfolink();
    nk_DisconnIPInfolink();

}
/* Get Client status table index By username */
int nk_getClientStatusIdx(char *name)
{
	int k=0;
	u_char decode_name[50];
	
	nk_str_to_ascii(name, &decode_name);
	for(k=0;ClientStatus[k].SaveData == 1;k++)
	{
		if(!strcmp(ClientStatus[k].username, decode_name))
		{
			/* User data index */
			UserIdx = k;
			return 1;
		}
	}
	return 0;
}

int nk_getConnClientIdx()
{
	int i=0;
	
	nk_GetClientInfolink(1);
	nk_GetServerInfolink(1);
	
	for(i=0; i< ServerInfo->MaxConnClient; i++)
	{
		if(ClientConn[i].SaveData == 1)
		{
			if(strcmp(ClientConn[i].ifname, ifname))
				continue;
				
			nk_PeerConnClientIdx = i;
			nk_DisconnClientInfolink();
			return 1;
		}
	}
	
	nk_DisconnServerInfolink();
	nk_DisconnClientInfolink();
	return 0;
}

int nk_killOldClientConn(char *name)
{
	char cmdbuf[128];
	int  k=0, retval=0;
	u_char decode_name[50];

	for(k=0;ClientConn[k].SaveData == 1;k++)
	{
		nk_str_to_ascii(ClientConn[k].username, &decode_name);
		if(!strcmp(decode_name, name))
		{
			notice("ClientConn name [%s] and user name [%s] match", ClientConn[k].username, ClientConn[k].username);
			notice("this user [%s] Max Connection [%d] ", ClientConn[k].username, ClientStatus[UserIdx].MaxConn);
			
			/* MaxConn > 1 , kill the same MAC of connection */
			if(ClientStatus[UserIdx].MaxConn > 1)
			{
				if(strcmp(ClientConn[k].eth, remote_number))
					continue;
			}
			
			notice("this user [%s] mac[%s] ifname [%s] pid[%d]", ClientConn[k].username, ClientConn[k].eth, ClientConn[k].ifname, ClientConn[k].pid);
			
			/* Check the record is valid*/
			sprintf(cmdbuf,"/var/run/%s.pid",ClientConn[k].ifname);
			if(access(cmdbuf,F_OK) == 0)
			{
				/* valid , kill daemon */
				kill(ClientConn[k].pid, SIGTERM);
				retval = 1;
				goto freelink;
			}
			else
			{
				/* not valid, remove record */
				sprintf(cmdbuf, "echo \"%s\" > %s ", ClientConn[k].eth, NKPPPOE_FIFO1_IPC);
				system(cmdbuf);
				retval = 1;
				goto freelink;
			}
		}
	}
	retval = 0;

freelink:
    return retval;
}
int nk_CountConn_check(char *name)
{
	int k = 0, retval =0;
	u_char decode_name[50];
	
	nk_str_to_ascii(name, &decode_name);
	/* Point to shared memory */
	nk_GetClientStatuslink(1);

	/* Get index from ClientStatus table */
	if(UserIdx == -1)
	{
		if(!nk_getClientStatusIdx(name))
		{
			nk_DisconnClientStatuslink();
			fatal("Not found user data for Connection check\n");
        }
	}
	else if(strcmp(ClientStatus[UserIdx].username, decode_name))
	{
		if(!nk_getClientStatusIdx(name))
		{
			nk_DisconnClientStatuslink();
			fatal("Not found user data for Connection check\n");
		}
	}

	/* Point to shared memory */
	nk_GetClientInfolink(1);

	/* Get index from ClientStatus table */
	if(ClientStatus[UserIdx].MaxConn == 0)
	{		
		retval = 1;
		goto freemem;
	}
	notice("this user %s Max connection %d online connection %d", name, ClientStatus[UserIdx].MaxConn, ClientStatus[UserIdx].Conn);
    if(ClientStatus[UserIdx].Conn >= ClientStatus[UserIdx].MaxConn)
	{
		
		/* Connection total count > Connection Max Number */
		if(ClientStatus[UserIdx].MaxConn > 1)
		{
			/* Max Connection > 1 */
			
			for(k=0;ClientConn[k].SaveData == 1;k++)
			{
				if(!strcmp(ClientConn[k].eth, remote_number))
				{
					/* kill the Last(Same Mac) Connection*/
					nk_killOldClientConn(ClientStatus[k].username);
					retval = 0;
					goto freemem;
				}
			}
			/* no Last(Same Mac) Connection*/
			/* NK_LOG_PPPoE(LOG_WARNING,"(PPPoE Server) User %s connection is already online",name); */
			info("connection is already online, User = [%s]", name);
			retval = 0;
			goto freemem;
		}
		else
		{
			/* Max Connection = 1 */
			nk_killOldClientConn(ClientStatus[UserIdx].username);
			retval = 0;
			goto freemem;
		}
	}
	
	retval = 1;
		
freemem:
	nk_DisconnClientStatuslink();
	nk_DisconnClientInfolink();
	return retval;
}
int nk_schedule_check(char *name)
{
	struct	timeval tv;
	int retval;
	u_char decode_name[50];
	
	nk_str_to_ascii(name, &decode_name);
	/* Point to shared memory */
	nk_GetClientStatuslink(1);

	/* Get index from ClientStatus table */
	if(UserIdx == -1)
	{
		if(!nk_getClientStatusIdx(name))
		{
			nk_DisconnClientStatuslink();
			fatal("Not found User data for schedule check\n");
        }
	}
	else if(strcmp(ClientStatus[UserIdx].username, name))
	{
		if(!nk_getClientStatusIdx(name))
		{
			nk_DisconnClientStatuslink();
			fatal("Not found User data for schedule check\n");
		}
	}
	
	if(!ClientStatus[UserIdx].IsValid)
	{
		retval = 1;
		goto disconn;
	}
	
	gettimeofday(&tv,NULL);
	
	if(tv.tv_sec > ClientStatus[UserIdx].validtime)
	{
		/* NK_LOG_PPPoE(LOG_WARNING,"(PPPoE Server) User %s connection fail, cause:This user Connection time up",name); */
		info("connection fail, cause:This User %s Connection time up",name);
		kd_doCommand(NULL,CMD_WRITE,ASH_PAGE_PPPOE_USERSETTING,NULL);
		retval = 0;
		goto disconn;
	}
	retval = 1;
disconn:
	nk_DisconnClientStatuslink();
	return retval;
}

int nk_checkip(u_int32_t remote, char *ipRange)
{
	char *tmpBuf = NULL, *tmp = NULL, buf[128];
	u_int32_t StartIPValue = 0, EndIPValue = 0, tmpValue =0;
	int i=0, MaxConnection = 0, retval =0;
	/* Point to shared memory */
	nk_GetClientStatuslink(1);

	/* Get index from ClientStatus table */
	if(UserIdx == -1)
	{
		if(!nk_getClientStatusIdx(peer_authname))
		{
			nk_DisconnClientStatuslink();
			fatal("Not found User data for ip check\n");
        }
	}
	else if(strcmp(ClientStatus[UserIdx].username, peer_authname))
	{
		if(!nk_getClientStatusIdx(peer_authname))
		{
			nk_DisconnClientStatuslink();
			fatal("Not found User data for ip check\n");
		}
	}
	
	/* Point to shared memory */
	nk_GetIPInfolink(1);	
	
	/* Check Server IP Range*/
		
	sprintf(buf,"%s",ipRange);
	if(strchr(buf, ':'))
	{
		tmpBuf=buf;
		if((tmp=strchr(buf, ':')))
		{
			*tmp='\0';
			tmp++;
		}
		EndIPValue = inet_addr(tmp);
		StartIPValue = inet_addr(tmpBuf);
	}
	else
	{
		fatal("No valid ip addresses found in variables for ip check\n");
	}
	
	/* Check user IP Type . no check if type is "assign ip" */
	if(ClientStatus[UserIdx].IsAssign)
	{
		
		nk_PeerIPInfoIdx = remote - StartIPValue;
		notice("get IP by administrator setting");
		if(IPInfoTable[nk_PeerIPInfoIdx].IsAssign != 1)
		{
			retval = 0;
			goto check_free;
		}
		
		if(!IPInfoTable[nk_PeerIPInfoIdx].IsUsed){
			retval = 1;
			goto check_free;
		}
				
		/* NK_LOG_PPPoE(LOG_WARNING,"(PPPoE Server) User %s connection fail, cause: This IP has used",peer_authname); */
		info("connection fail, cause: User %s IP has used", peer_authname);
		retval = 0;
		goto check_free;
	}
	
	if((remote < StartIPValue) || (remote > EndIPValue))
	{
		/* NK_LOG_PPPoE(LOG_WARNING,"(PPPoE Server) User %s connection fail, cause:IP %s is out of Server Assign IP range",peer_authname,ip_ntoa(remote)); */
		info("connection fail, cause:User %s IP %s is out of Server Assign IP range", peer_authname, ip_ntoa(remote));
		retval = 0;
		goto check_free;
	}
		
	sprintf(tmpBuf,"PPPOE_SERVER_SETTING TotalNum");
	kd_doCommand(tmpBuf, CMD_PRINT, ASH_DO_NOTHING, tmp);
	
	if (sscanf(tmp, "%d", &MaxConnection) != 1) {
		nk_DisconnClientStatuslink();
		nk_DisconnIPInfolink();
		fatal("No valid MaxConnection found in variables for ip check\n");
	}
	
	tmpValue = inet_addr(IPInfoTable[nk_PeerIPInfoIdx].peerip);

	if(tmpValue == remote)
	{
        notice("Get IP by PPPoE Server");
		/* Client get IP by PPPoE Server */
        if(IPInfoTable[nk_PeerIPInfoIdx].IsAssign == 1)
		{
            retval = 0;
			goto check_free;
		}
		
		if(IPInfoTable[nk_PeerIPInfoIdx].IsInvalid)
		{
			/* NK_LOG_PPPoE(LOG_WARNING,"(PPPoE Server) User %s connection fail, cause: Not valid ip addresses.",peer_authname); */
			info("connection fail, cause:User %s  Not valid ip addresses.", peer_authname);
			retval = 0;
			goto check_free;
		}
		
		if( IPInfoTable[nk_PeerIPInfoIdx].IsUsed )
		{
			/* NK_LOG_PPPoE(LOG_WARNING,"(PPPoE Server) User %s connection fail, cause: This IP has used",peer_authname); */
			info("connection fail, cause:User %s IP has used", peer_authname);
			retval = 0;
			goto check_free;
		}
		else
		{
			if(IPInfoTable[nk_PeerIPInfoIdx].IsLock){
				retval = 1;
				goto check_free;
			}
			retval = 0;
			goto check_free;
		}
	}
	else
	{
		/* Client get IP by self */
		notice("get IP by User setting");
		for(i=0; i< (MaxConnection + skipMultiSubnet); i++)
		{
			tmpValue = inet_addr( IPInfoTable[i].peerip);
			if(tmpValue == remote)
			{
				if(((IPInfoTable[i].IsAssign == 1) && (ClientStatus[UserIdx].IsAssign !=1)))
				{
					retval = 0;
					goto check_free;
				}
				
				if(!IPInfoTable[i].IsLock){
					nk_PeerIPInfoIdx = i;
					retval = 1;
					goto check_free;
				}
				
				/* NK_LOG_PPPoE(LOG_WARNING,"(PPPoE Server) User %s connection fail, cause: This IP has used",peer_authname); */
				info("connection fail, cause: User %s IP has used", peer_authname);
				retval = 0;
				goto check_free;
			}
		}
	}
	
	/* NK_LOG_PPPoE(LOG_WARNING,"(PPPoE Server) User %s connection fail, cause: No valid ip addresses. ",peer_authname); */
	info("connection fail, cause: User %s No valid ip addresses. ", peer_authname);
check_free:
	nk_DisconnClientStatuslink();
	nk_DisconnIPInfolink();
	return retval;
}
void nk_pppoe_gettime(char *time)
{
    time_t t;
    char buf[64];
    struct timeval	tv;
    struct timezone	tz;
    struct tm		*s,tm;
    int i=0;
    gettimeofday(&tv, &tz);
	
	//timezone~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

	kd_doCommand("SYSTEM TIMEZONE", CMD_PRINT, ASH_DO_NOTHING, buf);
	for (i=0; time_zones[i].name; i++)
	{
		if ( strlen(buf) && !strncmp(buf, time_zones[i].name, strlen(time_zones[i].name)) )
			break;
	} // for loop

	tv.tv_sec = tv.tv_sec + time_zones[i].gmt_offset * 3600;

	kd_doCommand("SYSTEM NTPSTATUS", CMD_PRINT, ASH_DO_NOTHING, buf);
	if(!strcmp(buf,"YES"))
	{
		// daylight saving
		kd_doCommand("SYSTEM DAYLIGHTSAVING", CMD_PRINT, ASH_DO_NOTHING, buf);
		if(!strcmp(buf,"YES"))
		{
			int	smonth,sday,emonth,eday;
			int sDateValue=0,eDateValue=0,nowDateValue=0;  // formate=%2d%2d(month,day)

			kd_doCommand("SYSTEM DAYLIGHTSTARTDATE", CMD_PRINT, ASH_DO_NOTHING, buf);
			sscanf(buf,"%d:%d",&smonth,&sday);
			sprintf(buf,"(Verify) %d;%d",smonth,sday);
			kd_doCommand("SYSTEM DAYLIGHTENDDATE", CMD_PRINT, ASH_DO_NOTHING, buf);
			sscanf(buf,"%d:%d",&emonth,&eday);

			memcpy(&tm, localtime(&tv.tv_sec), sizeof(struct tm));

			sDateValue=smonth*100+sday;
			eDateValue=emonth*100+eday;
			nowDateValue=(tm.tm_mon+1)*100+tm.tm_mday;

			if( ((sDateValue <= eDateValue) && ((sDateValue<=nowDateValue)&&(nowDateValue<=eDateValue))) ||
				( (sDateValue > eDateValue) && ((nowDateValue > sDateValue) ||(nowDateValue<eDateValue))) )
			{
				tv.tv_sec = tv.tv_sec + 3600;
			}
		}
	}
	t=tv.tv_sec;
	s=gmtime(&t);
	sprintf(time,"%d/%02d/%02d %02d:%02d",(1900+s->tm_year),(1+s->tm_mon),s->tm_mday,s->tm_hour,s->tm_min);
}

int nk_pppoe_conn_up(u_int32_t hisaddr, char *nk_iprange)
{
	char buf[512],cmdBuf[512],times[32];
	int i=0;
		
    if(!nk_checkip(hisaddr, nk_iprange))
    {
		return 1;
	}

	nk_GetClientStatuslink(1);
	nk_GetClientInfolink(1);
	nk_GetIPInfolink(1);
	nk_GetServerInfolink(1);
	
	ClientStatus[UserIdx].Conn = ClientStatus[UserIdx].Conn + 1;
		
	for(i=0; i< ServerInfo->MaxConnClient; i++)
	{
		if(ClientConn[i].SaveData == 0)
		{
			ClientConn[i].SaveData = 1;
			nk_PeerConnClientIdx = i;
			break;
		}
	}
	
	sprintf(ClientConn[nk_PeerConnClientIdx].username, "%s", peer_authname);		
	sprintf(ClientConn[nk_PeerConnClientIdx].eth, "%s", remote_number);
	sprintf(ClientConn[nk_PeerConnClientIdx].ifname, "%s", ifname);
	ClientConn[nk_PeerConnClientIdx].pid = getpid();
	
	IPInfoTable[nk_PeerIPInfoIdx].IsUsed = 1;
	
	nk_DisconnServerInfolink();
	nk_DisconnClientStatuslink();
	nk_DisconnClientInfolink();
	nk_DisconnIPInfolink();
	
	nk_displaySharedMemData();
	
	nk_pppoe_gettime(times);

	sprintf(cmdBuf,"echo \"ID=%d&MAC=%s&NAME=%s&IP=%s&PID=%d&STARTTIME=%s&IFNAME=%s&SESS=%d&\" > %s",
			getpid(), remote_number, peer_authname, ip_ntoa(hisaddr), getpid(), times, ifname, req_unit, NKPPPOE_FIFO1_IPC);
	system(cmdBuf);
	
	/* sprintf(buf,"(PPPoE Server) User %s connection success, ip=[%s], MAC=[%s]",peer_authname,ip_ntoa(hisaddr),remote_number);
	 NK_LOG_PPPoE(LOG_WARNING,buf); */
	info("connection success, User=[%s], ip=[%s], MAC=[%s]", peer_authname, ip_ntoa(hisaddr), remote_number);
	return 0;
}
void nk_pppoe_conn_down(u_int32_t hisaddr)
{
	char cmdBuf[128], tmpbuf[128];
	int k=0;
	u_int32_t StartIPValue = 0;
	
	sprintf(cmdBuf,"%s","PPPOE_SERVER_SETTING Enabled");
 	kd_doCommand(cmdBuf, CMD_PRINT, ASH_DO_NOTHING, cmdBuf);
 	if(!strcmp(cmdBuf,"0")) return ;	
	
	if(access("/etc/ppp/pppoeserver.lock", F_OK) == 0)
	{
		nk_GetIPInfolink(1);	
		nk_GetServerInfolink(1);
		
		sprintf(tmpbuf, "%s", ServerInfo->StartIp);
		
		StartIPValue=inet_addr(tmpbuf);
		
		nk_PeerIPInfoIdx = hisaddr - StartIPValue;
		
		if(nk_PeerIPInfoIdx < ServerInfo->MaxConnClient )
		{
			IPInfoTable[nk_PeerIPInfoIdx].IsUsed = 0;
			IPInfoTable[nk_PeerIPInfoIdx].LockTTL = 0;
			IPInfoTable[nk_PeerIPInfoIdx].IsLock = 0;
		}
		
		nk_DisconnServerInfolink();
		nk_DisconnIPInfolink();
		
		if(nk_getConnClientIdx())
		{
			nk_GetClientInfolink(1);
			ClientConn[nk_PeerConnClientIdx].SaveData = 0;
			nk_DisconnClientInfolink();
		}
		
		/* Get index from ClientStatus table */
		nk_GetClientStatuslink(1);
		k = nk_getClientStatusIdx(peer_authname);
		if(k)
		{
			if(ClientStatus[UserIdx].Conn>0)
				ClientStatus[UserIdx].Conn = ClientStatus[UserIdx].Conn - 1;
		}
		
		nk_DisconnClientStatuslink();
		
		nk_displaySharedMemData();
	}
	sprintf(cmdBuf,"echo \"%s\" > %s ",remote_number,NKPPPOE_FIFO1_IPC);
	system(cmdBuf);

	
	/* NK_LOG_PPPoE(LOG_WARNING,"(PPPoE Server) User %s Connection terminated",peer_authname); */
	notice("Connection terminated, User = [%s]", peer_authname);
}
