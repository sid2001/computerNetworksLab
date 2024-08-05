#include <sys/socket.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <stdio.h>
#include <string.h>
#include "./validators/ipv4_validator.c"

struct hostent *gethostbyname(const char *name);
struct hostent *gethostbyaddr(const void *addr, socklen_t len, int type);

struct hostent *he;
struct in_addr **addr_list;
struct in_addr addr;

int main() {
	char* hostname;
	char* ip_addr;
	int opt;
	char ip[100];
	char hh[100];

	printf("1. Hostname to IP\n2. IP to Hostname\n");
	scanf("%d", &opt);
	char a;
	switch(opt) {
		case 1:
			scanf("%c%s",&a,hh);
			hostname = hh;	
			he = gethostbyname(hostname);
			if (he == NULL) {
				herror("gethostbyname");
				return 1;
			}
			break;
		case 2:
			scanf("%c%s",&a,ip);
			char* tok = strtok(ip,"--");
			ip_addr = tok;
			tok = strtok(NULL,"--");
			printf("%s",tok);
			if(!strcmp(tok,"1")){
				ipv4_addr ii = toDotNotation(atoll(ip_addr));
				printf("%s",ii);
			}
			ip_addr = ip;
			
			if(!ipv4Validator((ipv4_addr)ip_addr)) {
				printf("Invalid IP address.");
				return 1;
			}
			inet_aton(ip_addr, &addr);
			he = gethostbyaddr(&addr, sizeof(addr), AF_INET);
			if (he == NULL) {
				herror("gethostbyaddr");
				return 1;
			}
			break;
		default:
			printf("Invalid option.");
	}

	

	printf("Official name is: %s\n", he->h_name);
	printf("IP address: %s\n", inet_ntoa(*(struct in_addr*)he->h_addr));
	printf("All addresses: ");
	addr_list = (struct in_addr **)he->h_addr_list;

	for(int i = 0; addr_list[i] != NULL; i++) {
    		printf("%s ", inet_ntoa(*addr_list[i]));
	}
}
