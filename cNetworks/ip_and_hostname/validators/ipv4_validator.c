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

unsigned int binaryToInt(ipv4_addr addr) {
	unsigned int ans = 0;
	while(*addr) {
		ans = (ans << 1) + (*addr++ - '0');
	}
	return ans;
}

ipv4_addr toDotNotation(ipv4_addr addr) {
	unsigned int addr_value = binaryToInt(addr);
	ipv4_addr ip = (ipv4_addr)malloc(15);
	unsigned int shift = 8;
	ipv4_addr buffer = (ipv4_addr)malloc(15);
	for(unsigned int i = 0; i < (unsigned int)4; i++) {
		unsigned int octet = (addr_value >> (i*shift)) & 0xFF;
	//	printf("octet = %d",octet);
		sprintf(buffer,"%s", ip);
		sprintf(ip, "%d", octet);
		if(i > 0) {
			strcat(ip,".");
		}
		sprintf(ip+strlen(ip),"%s",buffer);
		//printf("i = %d %s\n",i,ip);
		
	}
	//printf("ipp: %s",ip);
	return ip;
}
