typedef char* ipv6_addr;

int isxdigit(int c) {
	return (c >= '0' && c <= '9') || (c >= 'A' && c <= 'F') || (c >= 'a' && c <= 'f');
}

int ipv6_validator(ipv6_addr addr) {
    int i, j, k;
    printf("sd");
    int num = 0;
    int colons = 0;
    int len = strlen(addr);
    if (len < 2 || len > 39) {
	return 0;
    }
    for (i = 0; i < len; i++) {
	if (addr[i] == ':') {
	    colons++;
	}
    }
    if (colons < 2) {
	printf("dsd");
	return 0;
    }
    char* token = strtok(addr, ":");
    while (token != NULL) {
	if (strlen(token) > 4) {
	    return 0;
	}
	for (j = 0; j < strlen(token); j++) {
	    if (!isxdigit(token[j])) {
		return 0;
	    }
	}
	token = strtok(NULL, ":");
    }
    return 1;
}
