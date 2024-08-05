#include <stdlib.h>
typedef char* ipv4_addr ;
typedef unsigned int ipv4_addr_bin;

int atoi(const char *str);
int isdigit(int c);

int ipv4Validator(ipv4_addr addr) {
	int i, j, k;
	    int num = 0;
	    int dots = 0;
	    int len = strlen(addr);
	    if (len < 7 || len > 15) {
		return 0;
	    }
	    for (i = 0; i < len; i++) {
		if (addr[i] == '.') {
	dots++;
		}
	    }
	    if (dots != 3) {
		return 0;
	    }
	    char* token = strtok(addr, ".");
	    while (token != NULL) {
		if (strlen(token) > 3) {
	return 0;
		}
		if (token[0] == '0' && strlen(token) > 1) {
	return 0;
		}
		for (j = 0; j < strlen(token); j++) {
	if (!isdigit(token[j])) {
			return 0;
	}
		}
		num = atoi(token);
		if (num < 0 || num > 255) {
	return 0;
		}
		token = strtok(NULL, ".");
	    }
	return 1;
}

ipv4_addr toDotNotation(unsigned long long addr) {
	ipv4_addr ip = (ipv4_addr)malloc(15);
	for(int i = 0; i < 4; i++) {
		int octet = (addr >> (i==0?0:i*10*sizeof(1)*8)) & 0xFF;
		printf("octet = %d",octet);
		sprintf(ip + strlen(ip), "%d", octet);
		printf("i = %d %s\n",i,ip);
		if(i < 3) {
			strcat(ip, ".");
		}
	}
	return ip;
}
