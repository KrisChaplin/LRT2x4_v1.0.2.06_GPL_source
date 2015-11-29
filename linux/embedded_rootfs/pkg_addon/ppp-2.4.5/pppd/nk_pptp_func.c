#include <nkutil.h>
#if (defined(CONFIG_SUPPORT_PPTPD) ||  defined(CONFIG_NK_L2TP_SERVER))
#include "pppd.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <signal.h>

void nk_PPTPClearUserDate()
{
	char ip[16], ip_tmp[20], uname[20], tmp[128], cmd[100];
	int type, i;
	FILE *fp1;
	
	nk_PPTP_read_file_fun();
	
	// Added by Gavin Lin, 2010/01/28
	memset(ip, 0, sizeof(ip));
	kd_doCommand("PPTP_SERVER TYPE", CMD_PRINT, ASH_DO_NOTHING, tmp);
	type = atoi(tmp);
	if (!type)
	{
		sprintf(cmd,"iptables -t nat -D PREROUTING -s %s -j ACCEPT",nk_PPTP_write_t.remote_ip);
		system(cmd);
		warn("%s: %s", __func__, cmd);
		// sprintf(cmd,"iptables -t nat -D POSTROUTING -s %s/255.255.255.255 -j ACCEPT",nk_PPTP_write_t.remote_ip);
		// system(cmd);
		// warn("%s: %s", __func__, cmd);
	}
	else
	{
		fp1 = fopen("/etc/ppp/chap-secrets", "r");
    	if(fp1 == NULL)
		{
			return;
		}
		memset(ip, 0, sizeof(ip));
		memset(ip_tmp, 0, sizeof(ip));
		memset(tmp, 0, sizeof(tmp));	
		while (fgets(tmp, sizeof(tmp), fp1) != NULL) {
			sscanf(tmp,"%s %*s %*s %s",uname, ip_tmp);
			sprintf(tmp, "\"%s\"", nk_PPTP_write_t.username);
			if (!strcmp(tmp, uname))
			{
    			strncpy(ip, ip_tmp+1, strlen(ip_tmp)-2);
				break;
			}
			memset(ip_tmp, 0, sizeof(ip));
			memset(tmp, 0, sizeof(tmp));
		}
		fclose(fp1);
		sprintf(cmd,"iptables -t nat -D PREROUTING -s %s -j ACCEPT", ip);
		warn("%s: %s", __func__, cmd);
		warn(cmd);
		// sprintf(cmd,"iptables -t nat -D POSTROUTING -s %s/255.255.255.255 -j ACCEPT",ip);
		// system(cmd);
		// warn("%s: %s", __func__, cmd);
	}


	for(i=0; i < PPTP_FILE_SIZE;i++)
	{
		if(!strcmp(nk_PPTP_write_file[i].username,nk_PPTP_write_t.username))
		{
			FILE *fp = fopen("/tmp/.pptp.disconnect", "w");
			if(fp)
			{
				fprintf(fp, "%d\n", nk_PPTP_write_file[i].Pid);
				fflush(fp); fclose(fp);
			}
			nk_PPTP_write_file[i].connected = 0;			
			break;
		}
	}
	nk_PPTP_read_file_fun_filiter();
	nk_PPTP_write_file_fun();
	system("killall -SIGUSR2 pptpd");
}

int nk_PPTPcheckUserLogin(char *name)
{
	int i, type;
	char cmd[200];
	char ip[16], ip_tmp[20], uname[20];
	char tmp[128];
	FILE *fp1;
	
	nk_PPTP_write_t.Pid=getpid();
	nk_PPTP_read_file_fun();
	for(i=0;i<PPTP_FILE_SIZE;i++)
	{
		if(!strcmp(nk_PPTP_write_file[i].username,name)&&nk_PPTP_write_file[i].connected == 1)
		{
			kill(nk_PPTP_write_file[i].Pid, SIGTERM);
            return 0;
		}
	}
	
	strcpy(nk_PPTP_write_t.username,name);
	strcpy(nk_PPTP_write_t.ipparam_local_ip,ipparam);
	nk_PPTP_write_t.Pid=getpid();
	
	// Added by Gavin Lin, 2010/01/28
	memset(ip, 0, sizeof(ip));
	kd_doCommand("PPTP_SERVER TYPE", CMD_PRINT, ASH_DO_NOTHING, tmp);
	type = atoi(tmp);
	
	if (!type)
	{	
		/* pptp server assign ip type  not assign ip */
		sprintf(cmd,"iptables -t nat -A PREROUTING -s %s -j ACCEPT",nk_PPTP_write_t.remote_ip);
	}
	else
	{
		/* pptp server assign ip type is assign ip */
		fp1 = fopen("/etc/ppp/chap-secrets", "r");
		if(fp1 == NULL)
		{
			return 0;
		}
		memset(ip, 0, sizeof(ip));
		memset(tmp, 0, sizeof(tmp));	
		while (fgets(tmp, sizeof(tmp), fp1) != NULL) {
			
			memset(ip_tmp, 0, sizeof(ip_tmp));
			
			sscanf(tmp,"%s %*s %*s %s",uname, ip_tmp);
			sprintf(tmp, "\"%s\"", nk_PPTP_write_t.username);
			if (!strcmp(tmp, uname))
			{
				strncpy(ip, ip_tmp+1, strlen(ip_tmp)-2);
				break;
			}
			memset(tmp, 0, sizeof(tmp));
		}
		fclose(fp1);
		sprintf(cmd,"iptables -t nat -A PREROUTING -s %s -j ACCEPT", ip);
	}
	system(cmd);
	warn("%s: %s", __func__, cmd);
	warn("pptpd: iptables set %s\n", cmd);


	for(i=0; i < PPTP_FILE_SIZE;i++)
	{
		if(nk_PPTP_write_file[i].connected != 1)
		{
			strcpy(nk_PPTP_write_file[i].username,nk_PPTP_write_t.username);
#ifdef CONFIG_SUPPORT_PPTPD_READ_CONfIGFILE_BY_SIGNAL
			strcpy(nk_PPTP_write_file[i].password,nk_PPTP_write_t.password);
#endif						
			strcpy(nk_PPTP_write_file[i].ipparam_local_ip,nk_PPTP_write_t.ipparam_local_ip);
			strcpy(nk_PPTP_write_file[i].remote_ip,nk_PPTP_write_t.remote_ip);
			nk_PPTP_write_file[i].Pid = nk_PPTP_write_t.Pid;
			nk_PPTP_write_file[i].connected = 1;
			break;
		}
	}
	nk_PPTP_read_file_fun_filiter();
	nk_PPTP_write_file_fun();
	
	return 1;
}

void nk_PPTP_read_file_fun(void)
{
	char buf[255],*pp,*p;
	FILE *fp;
	int i;
	i = 0;
	for( i = 0 ; i < PPTP_FILE_SIZE ; i++)
	{
		strcpy(nk_PPTP_write_file[i].username,"");
		strcpy(nk_PPTP_write_file[i].ipparam_local_ip,"");
		strcpy(nk_PPTP_write_file[i].remote_ip,"");
		nk_PPTP_write_file[i].Pid = 0;
		nk_PPTP_write_file[i].connected = 0;
	}

	if (set_pptp_cfg_file_status()) //Add by Robert 2008/04/18
	{
		if (!(fp = fopen(nk_PPTP_write_path, "r"))) {
			warn("could not open input file=%s",nk_PPTP_write_path);
			clear_pptp_cfg_file_status(); //Add by Robert 2008/04/18
			return;
		}
		for(i=0;fgets(buf,400,fp)!=NULL;i++){
			pp = buf;
			p =  strchr(pp, ' ');
			*p = '\0';
			strcpy(nk_PPTP_write_file[i].username,pp);
#ifdef CONFIG_SUPPORT_PPTPD_READ_CONfIGFILE_BY_SIGNAL
			pp = p + 1;
			p =  strchr(pp, ' ');
			*p = '\0';
			strcpy(nk_PPTP_write_file[i].password,pp);
#endif

			pp = p + 1;
			p =  strchr(pp, ' ');
			*p = '\0';
			strcpy(nk_PPTP_write_file[i].ipparam_local_ip,pp);
			
			pp = p + 1;
			p =  strchr(pp, ' ');
			*p = '\0';
			strcpy(nk_PPTP_write_file[i].remote_ip,pp);
			
			pp = p + 1;
			p =  strchr(pp, ' ');
			*p = '\0';
			sscanf(pp, "%d", &nk_PPTP_write_file[i].Pid);
			
			sscanf(p+1, "%d", &nk_PPTP_write_file[i].connected);
		}
		fclose(fp);
		clear_pptp_cfg_file_status(); //Add by Robert 2008/04/18
	}
}
void nk_PPTP_write_file_fun(void)
{

	FILE *fp;
	int i;
	i = 0;
	char ip[16], ip_tmp[20], uname[20];
	char tmp[128];
	int type;
	FILE *fp1;
	
	// Added by Gavin Lin, 2010/01/28
	memset(ip, 0, sizeof(ip));
	kd_doCommand("PPTP_SERVER TYPE", CMD_PRINT, ASH_DO_NOTHING, tmp);
	type = atoi(tmp);
	
	if (set_pptp_cfg_file_status()) //Add by Robert 2008/04/18
	{
		if (!(fp = fopen(nk_PPTP_write_path, "w"))) {
			warn("could not open input file=%s",nk_PPTP_write_path);

			clear_pptp_cfg_file_status(); //Add by Robert 2008/04/18
			return;
		}
		for( i = 0 ; i < PPTP_FILE_SIZE ; i++)
		{
			if(nk_PPTP_write_file[i].connected == 1)
			{
				// Added by Gavin Lin, 2010/01/28
				if (!type)
				{
					strcpy(ip, nk_PPTP_write_file[i].remote_ip);
				}
				else
				{
					fp1 = fopen("/etc/ppp/chap-secrets", "r");
					if(fp1 == NULL)
					{
						fclose(fp); // not fp1
						clear_pptp_cfg_file_status();
						return;
					}
					memset(ip, 0, sizeof(ip));
					memset(ip_tmp, 0, sizeof(ip));
					memset(tmp, 0, sizeof(tmp));	
					while (fgets(tmp, sizeof(tmp), fp1) != NULL) {
						sscanf(tmp,"%s %*s %*s %s",uname, ip_tmp);
						sprintf(tmp, "\"%s\"", nk_PPTP_write_file[i].username);
						if (!strcmp(tmp, uname))
						{
							strncpy(ip, ip_tmp+1, strlen(ip_tmp)-2);
							break;
						}
						memset(ip_tmp, 0, sizeof(ip));
						memset(tmp, 0, sizeof(tmp));
					}
					fclose(fp1);
				}
#ifdef CONFIG_SUPPORT_PPTPD_READ_CONfIGFILE_BY_SIGNAL
				fprintf(fp, "%s %s %s %s %d %d\n",
#else
				fprintf(fp, "%s %s %s %d %d\n",
#endif
				nk_PPTP_write_file[i].username,
#ifdef CONFIG_SUPPORT_PPTPD_READ_CONfIGFILE_BY_SIGNAL
				nk_PPTP_write_file[i].password,
#endif
				nk_PPTP_write_file[i].ipparam_local_ip,
				// Modified by Gavin Lin, 2010/01/28
				ip,
				nk_PPTP_write_file[i].Pid,
				nk_PPTP_write_file[i].connected);
			}
		}
		fclose(fp);
		clear_pptp_cfg_file_status(); //Add by Robert 2008/04/18
	}
}
void nk_PPTP_read_file_fun_filiter(void)
{
	int i,j;
	for(i=PPTP_FILE_SIZE-1;i>=0;i--)
	{
		for(j=i-1;j>=0;j--)
		{
			if(nk_PPTP_write_file[i].connected == 1)
			{
				if(!strcmp(nk_PPTP_write_file[i].remote_ip,nk_PPTP_write_file[j].remote_ip))
				{
					nk_PPTP_write_file[j].connected = 0;
				}
			}
		}
	}
}
void dconsole_printf(char *str)
{
	FILE *fp;
	fp = fopen("/dev/console", "w");
	if (fp == NULL) {
		return;
	}
	fprintf(fp, "pptpd: %s\r\n", str);
	fflush(fp);
	fclose(fp);
}

//Add by Robert 2008/04/17
int set_pptp_cfg_file_status()
{
	FILE *fp;
	int i,n=1;
	int iRetry=0,iCounts=1000;
	short iflag;	

	if (!(fp = fopen(nk_PPTP_FILE_STATUS, "r"))) 
	{
		fp=fopen(nk_PPTP_FILE_STATUS,"a");
		fprintf(fp,"0");		
	};
	fclose(fp);
	
	while(1) 
	{
		srandom(n);
		n=0;
		while(n<10 || n>60000) n=random();
		for(i=0;i<n;i++);

		fp=fopen(nk_PPTP_FILE_STATUS,"r+");
		fscanf(fp,"%ld",&iflag);
		if (!iflag)
		{
			fprintf(fp,"1");
			fclose(fp);
			break;
		}
		fclose(fp);
		iRetry++;
		if (iRetry > iCounts) break;
	}

	system("sync");
	return !iflag;
}

void clear_pptp_cfg_file_status()
{
	FILE *fp;
	
	fp=fopen(nk_PPTP_FILE_STATUS,"w");
	fprintf(fp,"0");
	fclose(fp);
	system("sync");
}
#endif

