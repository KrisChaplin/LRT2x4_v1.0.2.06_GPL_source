#define OPENVPN_SERVER_MANAGE_PORT 7505
#define OPENVPN_SERVER_MANAGE_ip "127.0.0.1"

#define OPENVPN_SOCKET_BUFFER_SIZE 512

struct openvpn_connect_entry{
	struct openvpn_connect_enry *next;
	char name[64];  //username
	char vip[25];   //virutal ip
	char rip[25];   //real ip
	unsigned int rport;  //real port
};

int nk_openvpn_get_status(struct openvpn_connect_entry **head);
int nk_openvpn_free_entry(struct openvpn_connect_entry *head);
/*purpose     : openvpn author : trenchen date : 2011-08-31 */
/*description :  kill user                                  */
int nk_openvpn_discon(char *name);

