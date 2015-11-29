#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#include "openvpn_api.h"


static int openvpn_manage_creat(void)
{
	int sd=0;
	struct sockaddr_in server;
	char buffer[OPENVPN_SOCKET_BUFFER_SIZE];

	//creat socket
	if( (sd = socket(AF_INET, SOCK_STREAM, 0)) == -1 ){
		return -1;
	}
	//connect
	memset( &server, 0, sizeof(server));
	server.sin_family = AF_INET;
	server.sin_port = htons(OPENVPN_SERVER_MANAGE_PORT);
	server.sin_addr.s_addr = inet_addr(OPENVPN_SERVER_MANAGE_ip);
	if( connect( sd,(struct sockaddr*)&server, sizeof(server) ) == -1 ){
		close(sd);
		return -2;
	}
	//read init message
	if( read( sd, buffer, OPENVPN_SOCKET_BUFFER_SIZE) <= 0 ){
		close(sd);
		return -3;
	}

	if( strstr(buffer,"OpenVPN") == 0 ){
		close(sd);
		return -4;
	}

	return sd;

}

static int get_oneline(int sd, char *linebuffer, unsigned int size)
{
	static char buffer[OPENVPN_SOCKET_BUFFER_SIZE]={0};
	static unsigned int len=0;
	static char *srcp = buffer;
	char *dstp = linebuffer;
	unsigned int getlen=0;
	unsigned int reread = 0;
	unsigned int flag = 0;

	while( len || (len=read(sd,buffer,OPENVPN_SOCKET_BUFFER_SIZE)) ){
		reread = 0;
		while( getlen < (size-1) ){
			if(len == 0){
				srcp = buffer;
				reread = 1;
				break;
			}else{
				if( flag && *srcp == '\xa' ){
					dstp--;
					getlen--;

					srcp++;
					len--;

					break;
				}else
					flag = 0;

				if(*srcp == '\xd')
					flag = 1;

				*dstp = *srcp;
				dstp++;
				srcp++;
				getlen++;
				len--;
				
				
			}
		}
		
		//add line end
		if(reread == 0){
			*dstp = '\0';
			break;
		}
	}

	return getlen;
	
}

static int openvpn_manage_status(int sd, struct openvpn_connect_entry **head)
{
	char buffer[OPENVPN_SOCKET_BUFFER_SIZE];
	struct openvpn_connect_entry *temp;

	*head = 0;

	if( sd<=0 )
		return -1;

	write( sd, "status\xd\xa", 9);

	while( get_oneline( sd, buffer, OPENVPN_SOCKET_BUFFER_SIZE) ){
		if( strncmp(buffer, "END", 3) ){
			if( strncmp(buffer, "STATUS", 6) )
				continue;
			//make a list
			if( ( temp = malloc(sizeof(struct openvpn_connect_entry)) ) == NULL )
				return -1;

			sscanf(buffer,"STATUS %s %s %[^:]:%u",temp->name,temp->vip,temp->rip,&(temp->rport));
			//insert list head
			temp->next = *head;
			*head = temp;
		}else
			break;
	}

	return 0;
}

static int openvpn_manage_close(int sd)
{
	write( sd, "exit\xd\xa", 9);
	//close connection
	if( sd > 0 )
		close(sd);

	return 0;
}

int nk_openvpn_get_status(struct openvpn_connect_entry **head)
{
	int sockfd = 0;

	//creat connection to openvpn
	if( (sockfd = openvpn_manage_creat()) < 0 )
		return -1;

	//send message
	openvpn_manage_status(sockfd, head);	
 
	openvpn_manage_close(sockfd);

	return 0;
}


int nk_openvpn_free_entry(struct openvpn_connect_entry *head)
{
	struct openvpn_connect_entry *temp=0;

	while( temp=head ){
		head = temp->next;
		free(temp);	
	}

	return 0;
}


/*purpose     : openvpn author : trenchen date : 2011-08-31 */
/*description : kill user                                   */
static int openvpn_manage_kill(int sd, char *name)
{
	char buffer[50];

	if( sd<=0 )
		return -1;

	sprintf(buffer,"kill %s\xd\xa",name);

	write( sd, buffer, strlen(buffer)+1);
}

int nk_openvpn_discon(char *name)
{
	int sockfd = 0;

	if(!name)
		return 0;

	//creat connection to openvpn
	if( (sockfd = openvpn_manage_creat()) < 0 )
		return -1;

	openvpn_manage_kill(sockfd,name);

	openvpn_manage_close(sockfd);
}




