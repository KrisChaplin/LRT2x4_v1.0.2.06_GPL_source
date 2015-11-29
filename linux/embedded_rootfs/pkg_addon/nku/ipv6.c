#include <features.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/ioctl.h>
#include <netinet/in.h>
#include <net/if.h>
#include <net/if_arp.h>
#include <stdio.h>
#include <errno.h>
#include <fcntl.h>
#include <ctype.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <netdb.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include "ipv6.h"
#include "nkutil.h"
#include <stdarg.h>
#include <sys/stat.h>

char *IPv6_to_regular(char *ip_addr)
{
	char tmp_addr[8][4],ip_addr_temp[128];
	char *p,*pp,*pp1;
	int count=0,i,j,k,calue=0;
	char *delim = ":";
	char * pch;

	strcpy(ip_addr_temp,ip_addr);

	pch = strtok(ip_addr_temp,delim);
	while (pch != NULL)
	{
		strcpy(tmp_addr[count],pch);

		if (strlen(pch) <4)
		{
			switch (strlen(pch))
			{
				case 1:
    				 tmp_addr[count][3]=tmp_addr[count][0];
    				 tmp_addr[count][2]='0';
    				 tmp_addr[count][1]='0';
    				 tmp_addr[count][0]='0';
    				 break;
				case 2:
    				 tmp_addr[count][3]=tmp_addr[count][1];
    				 tmp_addr[count][2]=tmp_addr[count][0];
    				 tmp_addr[count][1]='0';
    				 tmp_addr[count][0]='0';
    				 break;
				case 3:
    				 tmp_addr[count][3]=tmp_addr[count][2];
    				 tmp_addr[count][2]=tmp_addr[count][1];
    				 tmp_addr[count][1]=tmp_addr[count][0];
    				 tmp_addr[count][0]='0';
    				 break;
			}
		}
		pch = strtok (NULL, delim);
		count++;
	}

	for ( i=0; i < 39; i++)
	{
		if (ip_addr[i]==':' && i > 0)
		{
			calue++;
		}

		if (ip_addr[i]==':' && ip_addr[i+1]==':')
		{
			for (j=7; j > 7-((count)-calue) ; j--)
			{
				 tmp_addr[j][3]=tmp_addr[(count-1)-(7-j)][3];
				 tmp_addr[j][2]=tmp_addr[(count-1)-(7-j)][2];
				 tmp_addr[j][1]=tmp_addr[(count-1)-(7-j)][1];
				 tmp_addr[j][0]=tmp_addr[(count-1)-(7-j)][0];
			}
			for (k=j ; k >=calue ; k--) 
			{
				 tmp_addr[k][3]='0';
				 tmp_addr[k][2]='0';
 				 tmp_addr[k][1]='0';
				 tmp_addr[k][0]='0';
			}
			i=40;
		}
	}

	for (j=0; j < 39; j++)
	{
		if (j%5==4)
		{
			ip_addr[j]=':';
		}
		else
		{
			ip_addr[j]=tmp_addr[j/5][j%5];
		}
	}
	ip_addr[j]='\0';
	return ip_addr;
}

char *IPv6_to_zip(char *ip_addr)
{
	char ip_addr_temp_front[40], ip_addr_temp_end[40];
	int idx, parse_more, double_colon_idx;
	char *double_colon_addr;
	int i;

	ip_addr_temp_front[0] = '\0';
	ip_addr_temp_end[0] = '\0';
	double_colon_addr = strstr(ip_addr, "::");
	if (double_colon_addr)
	{
		double_colon_idx = double_colon_addr - ip_addr;
		// section after "::"
		for (i = double_colon_idx+2, idx = 0; i < strlen(ip_addr); i++)
		{
			if (idx == 0)
			{
				if (ip_addr[i] == '0' || ip_addr[i] == ':')
					continue;
			}
			else if (ip_addr[i] == '0')
			{
				if (ip_addr_temp_end[idx-1] == ':' && ip_addr[i+1] != ':' && ip_addr[i+1] != '\0')
					continue;
			}
			ip_addr_temp_end[idx++] = ip_addr[i];
		}
		ip_addr_temp_end[idx] = '\0';
		
		// section before "::"
		parse_more = 0;
		for (i =  double_colon_idx - 1; i >= 0; i--)
		{
			if (ip_addr[i] == ':')
				double_colon_idx = i;
			else if (ip_addr[i] != '0')
			{
				parse_more = 1;
				break;
			}
		}
		if (parse_more == 1)
		{
			for (i = 0, idx = 0; i < double_colon_idx; i++)
			{
				if (ip_addr[i] == '0')
				{
					if (ip_addr[i+1] != ':' && (idx == 0 || ip_addr_temp_front[idx-1] == ':'))
						continue;
				}
				ip_addr_temp_front[idx++] = ip_addr[i];
			}
			ip_addr_temp_front[idx] = '\0';
			sprintf(ip_addr, "%s::%s", ip_addr_temp_front, ip_addr_temp_end);
		}
		else
			sprintf(ip_addr, "::%s", ip_addr_temp_end);
	}
	else
	{
		//strcpy(ip_addr_temp_front, ip_addr);
		parse_more = 0;
		for (i = 0, idx = 0; i < strlen(ip_addr); i++)
		{
			if (ip_addr[i] == '0')
			{
				if ((idx == 0) || (ip_addr_temp_front[idx-1] == ':'))
				{
					if (ip_addr[i+1] == ':' || ip_addr[i+1] == '\0')
					{
						double_colon_idx = i;
						parse_more = 1;
						if (idx > 0) /* remove last ':' */
							idx--;
						break;
					}
					else
						continue;
				}
			}
			ip_addr_temp_front[idx++] = ip_addr[i];
		}
		ip_addr_temp_front[idx] = '\0';
		
		if (parse_more == 1)
		{
			for (i = double_colon_idx+2, idx = 0; i < strlen(ip_addr); i++)
			{
				if (idx == 0)
				{
					if (ip_addr[i] == '0' || ip_addr[i] == ':')
						continue;
				}
				else if (ip_addr[i] == '0')
				{
					if (ip_addr_temp_end[idx-1] == ':' && ip_addr[i+1] != ':' && ip_addr[i+1] != '\0')
						continue;
				}
				ip_addr_temp_end[idx++] = ip_addr[i];
			}
			ip_addr_temp_end[idx] = '\0';
			sprintf(ip_addr, "%s::%s", ip_addr_temp_front, ip_addr_temp_end);
		}
		else
			sprintf(ip_addr, "%s", ip_addr_temp_front);
	}
	
	return ip_addr;
}

void ipv6_console_printf(char *str)
{
 FILE *fp;
 fp = fopen("/dev/console", "w");
 if (fp == NULL) {
   return;
 }
 fprintf(fp, "ipv6: %s\r\n", str);
 fflush(fp);
 fclose(fp);
}

int nk_ipv6_get_ip_type(void)
{
	char temp[10];
	
	kd_doCommand("SYSTEM IPTYPE", CMD_PRINT, ASH_DO_NOTHING, temp);
	return atoi(temp);
}

int nk_ipv6_get_device_ip_total(char *dev)
{
	char cmdBuf[256],buf[100],file_name[64];
	FILE *fp=NULL;
	int i=0, number=0;
    struct stat sb;
	
	sprintf(file_name,"/tmp/ifconfig.%s",dev);
	if (stat( file_name, &sb ) < 0 ) /* get password from web but incorrect*/
	{
		sprintf(cmdBuf,"/sbin/ifconfig %s 2>/dev/null | /bin/grep -c inet6 > /tmp/ifconfig.%s ; /sbin/ifconfig %s 2>/dev/null | /bin/grep inet6 >> /tmp/ifconfig.%s",dev,dev,dev,dev);
		system(cmdBuf);
	}

	fp=fopen(file_name,"r");
	if (fp == NULL) {
		sprintf(cmdBuf,"/bin/rm -rf %s > /dev/null 2>&1",file_name);
		system(cmdBuf);
		return -1;
	}
	
	fgets(buf, sizeof(buf), fp);/* get the number in firest line */
	number=atoi(buf);

	if(fp)
	{
		fclose(fp);
	}
	
	sprintf(cmdBuf,"/bin/rm -rf %s > /dev/null 2>&1",file_name);
	system(cmdBuf);
	
	return number;
}

int nk_ipv6_get_device_ip(char *dev, struct NK_IPV6_DATA *IPV6_list, int index)
{
	char cmdBuf[256],buf[100],file_name[64];
	FILE *fp=NULL;
	int i=0, number=0;
	char ipv6[INET6_ADDRSTRLEN],prefix[100],scope[10];
    struct stat sb;
	
	sprintf(file_name,"/tmp/ifconfig.%s",dev);
	if (stat( file_name, &sb ) < 0 )
	{
		sprintf(cmdBuf,"/sbin/ifconfig %s 2>/dev/null | /bin/grep -c inet6 > /tmp/ifconfig.%s ; /sbin/ifconfig %s 2>/dev/null | /bin/grep inet6 >> /tmp/ifconfig.%s",dev,dev,dev,dev);
		system(cmdBuf);
	}

	fp=fopen(file_name,"r");
	if (fp == NULL) {
		return -1;
	}
	
	fgets(buf, sizeof(buf), fp);/* get the number in firest line */
	number=atoi(buf);
    
	if( number == 0 || number <= index ) goto error;
	
	for(i=0;i<index;i++)
	{
		fgets(buf, sizeof(buf), fp);
	}

	fgets(buf, sizeof(buf), fp);
	
	if ((strstr(buf,"inet6 addr: ") != NULL) && 
		sscanf(strstr(buf,"inet6 addr: ")+strlen("inet6 addr: ") , "%[^/]/%[^ ] Scope:%s", ipv6,prefix,scope) != 3)
	{
		goto error;
	}
	inet_pton (AF_INET6, &ipv6[0], &IPV6_list->ipv6_address);
	IPV6_list->prefix_len=atoi(prefix);
	
	if(!strncmp(scope,"Link",strlen("Link")))
	{
		IPV6_list->scope_type=SCOPE_LINK;
	}
	else if(!strncmp(scope,"Global",strlen("Global")))
	{
		IPV6_list->scope_type=SCOPE_GLOBAL;
	}
	else if(!strncmp(scope,"Compat",strlen("Compat")))
	{
		IPV6_list->scope_type=SCOPE_COMPAT;
	}
out:
	if(fp)
	{
		fclose(fp);
	}
	return 1;
	
error:
	if(fp)
	{
		fclose(fp);
	}
	sprintf(cmdBuf,"/bin/rm -rf %s > /dev/null 2>&1",file_name);
	system(cmdBuf);
	return -1;
}

int nk_ipv6_get_device_mac(char *dev, struct NK_IPV6_DATA *IPV6_list)
{
	char cmdBuf[256],buf[100],file_name[64];
	FILE *fp=NULL;
	int i=0;
    struct stat sb;
	int str_mac[6];
	
	sprintf(file_name,"/tmp/ifconfig.mac.%s",dev);
	sprintf(cmdBuf,"/sbin/ifconfig %s 2>/dev/null | /bin/grep  HWaddr > %s",dev,file_name);
	system(cmdBuf);

	if (stat( file_name, &sb ) < 0 )
	{
		return -1;
	}

	fp=fopen(file_name,"r");
	if (fp == NULL) {
		return -1;
	}
	
	fgets(buf, sizeof(buf), fp);
	
	if(strlen(buf) && strstr(buf,"HWaddr"))
	{
		if( sscanf(strstr(buf,"HWaddr") , "%*s %02x:%02x:%02x:%02x:%02x:%02x",
			&str_mac[0],&str_mac[1],&str_mac[2],&str_mac[3],&str_mac[4],&str_mac[5]) == 6)
		{
			for(i=0;i<6;i++)
			{
				IPV6_list->mac[i]=(char *)str_mac[i];
			}
		}
	}
	
	if(fp)
	{
		fclose(fp);
	}
	sprintf(cmdBuf,"/bin/rm -rf %s > /dev/null 2>&1",file_name);
	system(cmdBuf);
	return 1;
}

int show_device_ipv6(char *dev)
{
    int i=0;
	struct NK_IPV6_DATA IPV6_list;
	char temp[50],line[256];
	struct in6_addr ipv6_address;
	
	sprintf(line,"oolong total=%d\n",nk_ipv6_get_device_ip_total(dev));
	ipv6_console_printf(line);
	
	memset(&IPV6_list,0,sizeof(struct NK_IPV6_DATA));
	memset(&ipv6_address,0,sizeof(ipv6_address));
	while(nk_ipv6_get_device_ip(dev,&IPV6_list,i) != -1)
	{
		inet_ntop(AF_INET6, &(IPV6_list.ipv6_address), temp, 50);
		sprintf(line,"oolong end[%d]: ipv6=%s prefix_len = %d scope=%d \n",i,temp,IPV6_list.prefix_len,IPV6_list.scope_type);
		ipv6_console_printf(line);
		i++;
	}
}

void init_dhclient_data_ipv6(int iFace)
{
	char cmdBuf[256];
	
	sprintf(cmdBuf, "ISP%d DNS1_V6=%s", iFace+1,"::");
	kd_doCommand(cmdBuf, CMD_WRITE, ASH_DO_NOTHING, NULL);
	sprintf(cmdBuf, "ISP%d DNS2_V6=%s", iFace+1,"::");
	kd_doCommand(cmdBuf, CMD_WRITE, ASH_DO_NOTHING, NULL);
	
	sprintf(cmdBuf, "ISP%d GATEWAY_V6=%s", iFace+1,"::");
	kd_doCommand(cmdBuf, CMD_WRITE, ASH_DO_NOTHING, NULL);
	
	sprintf(cmdBuf, "ISP%d WAN_V6=%s", iFace+1,"::");
	kd_doCommand(cmdBuf, CMD_WRITE, ASH_DO_NOTHING, NULL);
}

int dhclient_write_GW_DNS_to_data_ipv6(int get_ipv6_now, char *NK_interface, char *name_type,char *ipv6)
{
	char cmdBuf[256],buf[128],*temp,gateway_ipv6[100],dns_ipv6_1[100],dns_ipv6_2[100],tmpbuf[128];
	int i=0,j=0,mac[6],flag=0,device=0;
	
	if(get_ipv6_now)
	{
		sscanf(NK_interface,"eth%d",&device);
	
		if( !strncmp(name_type, "server-id", strlen("server-id")) )
		{
			for(i=strlen(ipv6);i>=0;i--)
			{
				if(ipv6[i] == ':')
					flag++;
				if(flag==6)
				{
					char ra_gateway[100];
					sscanf(ipv6+i+1,"%02x:%02x:%02x:%02x:%02x:%02x",&mac[0],&mac[1],&mac[2],&mac[3],&mac[4],&mac[5] );
					sprintf(gateway_ipv6,"fe80::%02x%02x:%02xff:fe%02x:%02x%02x",mac[0]&0xFF ^ 0x2,mac[1],mac[2],mac[3],mac[4],mac[5]);
					
					sprintf(cmdBuf, "ISP%d DHCP_GATEWAY_V6=%s", device,gateway_ipv6);
					kd_doCommand(cmdBuf, CMD_WRITE, ASH_DO_NOTHING, NULL);
					
					sprintf(cmdBuf, "ISP%d RA_GATEWAY_V6", device);
					kd_doCommand(cmdBuf, CMD_PRINT, ASH_DO_NOTHING, ra_gateway);
					
					if((strlen(ra_gateway) > 0) && (strcmp(ra_gateway, "::") != 0))
					{
						strcpy(gateway_ipv6, ra_gateway);
					}
					sprintf(cmdBuf,"/usr/sbin/ip -6 route add default via %s dev %s metric %d", gateway_ipv6,NK_interface,NK_DEFAULT_METRIC );
					system(cmdBuf);
						
					sprintf(cmdBuf, "ISP%d GATEWAY_V6=%s", device,gateway_ipv6);
					kd_doCommand(cmdBuf, CMD_WRITE, ASH_DO_NOTHING, NULL);
					break;
				}
			}
		}
		else if( !strncmp(name_type, "name-servers", strlen("name-servers")) )
		{
			if(strchr(ipv6,','))
			{
				sscanf(ipv6,"%[^,]",dns_ipv6_1);
				sprintf(cmdBuf, "ISP%d DNS1_V6=%s", device,dns_ipv6_1);
				kd_doCommand(cmdBuf, CMD_WRITE, ASH_DO_NOTHING, NULL);
				sscanf(strchr(ipv6,',')+1,"%[^,]",dns_ipv6_2);
				sprintf(cmdBuf, "ISP%d DNS2_V6=%s", device,dns_ipv6_2);
				kd_doCommand(cmdBuf, CMD_WRITE, ASH_DO_NOTHING, NULL);
			}
			else
			{
				sprintf(cmdBuf, "ISP%d DNS1_V6=%s", device,ipv6);
				kd_doCommand(cmdBuf, CMD_WRITE, ASH_DO_NOTHING, NULL);
			}
		}
		else if( !strncmp(name_type, "iaaddr", strlen("iaaddr")) )
		{
			sprintf(cmdBuf, "ISP%d WAN_V6=%s", device,ipv6);
			kd_doCommand(cmdBuf, CMD_WRITE, ASH_DO_NOTHING, NULL);

			
		}
	}
}


char *IPv6_Address_GetPrefix(char *buf, int v6prefixlength)
{
	char v6prefix[128],prefix[128];
	int i, count=0;
	unsigned char tmp;

	strcpy(prefix,buf);

	IPv6_to_regular(prefix);

	v6prefix[0] = '\0';
	for (i=0;count<(v6prefixlength/4);i++)
	{
		if (prefix[i]!=':')
		{
			count++;
		}
		v6prefix[i]=prefix[i];
	}

	if (prefix[i]==':')
	{
		v6prefix[i]=prefix[i];
		i=i+1;
	}

	if ((v6prefixlength%4)!=0)
	{
		if ((unsigned char)prefix[i] >= 0x61) /* a~f */
			tmp = (unsigned char)prefix[i] - 0x61 + 0xa;
		else if ((unsigned char)prefix[i] >= 0x41) /* A~F*/
			tmp = (unsigned char)prefix[i] - 0x61 + 0xa;
		else
			tmp = (unsigned char)prefix[i] - 0x30;
		
		switch(v6prefixlength%4)
		{
			case 1:
				tmp &= 0x8;
				break;
			case 2:
				tmp &= 0xc;
				break;
			case 3:
				tmp &= 0xe;
				break;
			default:
				break;
		}

		if (tmp >= 0xa)
			v6prefix[i] = tmp + 0x61 - 0xa;
		else
			v6prefix[i] = tmp + 0x30;
		i++;
	}
	v6prefix[i]='\0';
	
	switch((strlen(v6prefix))%5)
	{
		case 3:
			strcat(v6prefix,"0");
			break;
		case 2:
			strcat(v6prefix,"00");
			break;
		case 1:
			strcat(v6prefix,"000");
			break;
		default:
			;
	}

	if (v6prefixlength<=112)
	{
		if ((!v6prefixlength) || (v6prefixlength%16))
		{
			strcat(v6prefix,"::");
		}
		else
		{
			strcat(v6prefix,":");
		}
	}

	IPv6_to_zip(v6prefix);
	strcpy(buf,v6prefix);
	return buf;
}
