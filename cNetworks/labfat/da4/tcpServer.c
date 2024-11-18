#include <stdio.h> 
#include <netdb.h> 
#include <netinet/in.h> 
#include <stdlib.h> 
#include <arpa/inet.h>
#include <string.h> 
#include <sys/socket.h> 
#include <sys/types.h> 
#include <unistd.h> // read(), write(), close()

#define MAX 100
#define SA struct sockaddr
#define PORT 6969


int main() {
	struct sockaddr_in servaddr, cli;
	int sockfd, connfd, len;

	sockfd = socket(AF_INET, SOCK_STREAM, 0);

	if(sockfd == -1) {
		printf("Failed creating socket!!\n");
		exit(1);
	}else{
	printf("Socket created!!\n");
	}
	bzero(&servaddr, sizeof(servaddr));
	servaddr.sin_family = AF_INET;
	servaddr.sin_addr.s_addr = htonl(INADDR_ANY);
	servaddr.sin_port = htons(PORT);

	if(bind(sockfd, (SA*)&servaddr,sizeof(servaddr)) != 0) {
		printf("socket bind failed!!\n");
		exit(1);
	}else{
		printf("socket bind success!!\n");
	}

	if(listen(sockfd, 5) != 0) {
		printf("Listen failed\n");
		exit(1);
	}else{
		printf("Sever listening!!\n");
	}
	
	len = sizeof(cli);
	while(1) {
		
		connfd = accept(sockfd,(SA*)&cli, &len);
		if(connfd < 0){
			printf("Server accept failed!!\n");
			exit(1);
		}else{
			printf("Server accept the client\n");
		}
		char buff[MAX];
		int n;
			char clientip[INET_ADDRSTRLEN];
			inet_ntop(AF_INET, &cli.sin_addr, clientip, INET_ADDRSTRLEN);
			int clientport = ntohs(cli.sin_port);
			printf("From client: %s:%d %s\n",clientip,clientport,buff);
		while(1){
			bzero(buff, MAX);
			read(connfd,buff,sizeof(buff));
			bzero(buff,MAX);
			while((buff[n++] = getchar()) != '@');
			write(connfd, buff,sizeof(buff));
			if(strncmp("exit",buff,4) == 0) {
				printf("Server Exit...\n");
				break;
			}
		}
	}
}
