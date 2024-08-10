#include <sys/socket.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <stdio.h>
#include <string.h>
#include "./validators/ipv6_validator.c"
#include "./validators/ipv4_validator.c"

struct hostent *gethostbyname(const char *name);
struct hostent *gethostbyaddr(const void *addr, socklen_t len, int type);

struct hostent *he;
struct in_addr **addr_list;
struct in_addr addr;
struct in6_addr addr6;

int main() {
	char* hostname;
	char* ip_addr;
	int opt;
	char ip[100];
	char hh[100];

	printf("1. Hostname to IP\n2. IPv4 to Hostname\n3. IPv6 to Hostname\n");
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
			//printf("%s",tok);
			if(tok && !strcmp(tok,"1")){
				ip_addr = toDotNotation((ipv4_addr)ip_addr);
			}
			char* ip_addr_copy = (char*)malloc(15);
			sprintf(ip_addr_copy,"%s",ip_addr);
			if(!ipv4Validator((ipv4_addr)ip_addr_copy)) {
				printf("Invalid IP address.");
				return 1;
			}
			printf("\nip: %s\n",ip_addr);
			inet_aton((char *)ip_addr, &addr);
			he = gethostbyaddr(&addr, sizeof(addr), AF_INET);
			if (he == NULL) {
				herror("gethostbyaddr");
				return 1;
			}
			break;
		case 3:
			scanf("%c%s", &a, ip);
			char* ip6_addr_copy = (char*)malloc(24);
			sprintf(ip6_addr_copy, "%s", ip);
			if(!ipv6_validator((ipv6_addr)ip6_addr_copy)) {
				printf("Invalid IP address.");
			} else {
				inet_pton(AF_INET6, (char *)ip, &addr6);
				he = gethostbyaddr(&addr6, sizeof(addr6), AF_INET6);
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
