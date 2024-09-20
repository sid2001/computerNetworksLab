#include <arpa/inet.h> 
#include <netdb.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <strings.h> // bzero()
#include <sys/socket.h>
#include <unistd.h>
#define MAX 80
#define PORT 6980

int main() {
	char *ip = "127.0.0.1";
	
	int sock;
	struct sockaddr_in addr;
	socklen_t addr_size;
	char buffer[1024];
	int n;

	sock = socket(PF_INET, SOCK_STREAM, 0);
	if(sock<0) {
		perror("Socket error");
		exit(1);
	}
	printf("TCP server socket created.\n");

	addr.sin_family = AF_INET;
	addr.sin_port = htons(PORT);
	addr.sin_addr.s_addr = inet_addr(ip);
	memset(&addr.sin_zero, '\0', sizeof(addr));

	if(connect(sock, (struct sockaddr*)&addr, sizeof(struct sockaddr))==-1){
		printf("Cannot connect to the server.\n");
		exit(1);
	}
	printf("Connected to the server.\n");
	
	printf("1.Send file\n2.Chat\n");
	char* x = (char *)malloc(sizeof(char)*2);
	scanf("%s",x);
	if(x[0]=='2'){
		while(1){
			bzero(buffer, 1024);
			printf(">>");
			scanf("%*c%[^\n]",buffer);
			//printf("%s\n",buffer);
			//char *msg = "hello";
			char *msg = (char *)malloc(sizeof(char)*100);
			sprintf(msg,"1 %s",buffer);
			int s_bits = send(sock, msg, strlen(msg),0);
			if(s_bits==-1) {
				printf("Socket disconnected");
			}
		}
	}else if(x[0]){
		FILE *fptr;
		fptr = fopen("client_file.txt","r");
		if(!fptr) printf("Can't open file.\n");
		else {
			char *msg = (char *)malloc(sizeof(char)*100);
			char st[98];

			fgets(st,98,fptr);
			sprintf(msg,"0 %s",st);
			printf("\nSending file...\n");
			int s_bits = send(sock,msg,strlen(msg),0);
			if(s_bits==-1) {
				printf("Socket disconnected");
			}
		}

	}
	close(sock);
	printf("Disconnected form the server. \n");

	return 0;
}
