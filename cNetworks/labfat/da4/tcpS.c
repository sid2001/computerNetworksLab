#include <stdlib.h>
#include <arpa/inet.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <string.h>
#include <sys/types.h>
#include <netdb.h>

#define MAX 20
#define SA struct sockaddr
int main(){
	struct sockaddr_in servaddr, cli;
	int connfd, sockfd;

	sockfd = socket(AF_INET, SOCK_STREAM,0);

	if(sockfd == -1){
		printf("Failed socket\n");
	}
	printf("Socket created!!\n");

	servaddr.sin_family = AF_INET;
	servaddr.sin_addr.s_addr = htonl(INADDR_ANY);
	servaddr.sin_port = htons(6969);

	if(bind(sockfd, (SA*)&servaddr, sizeof(servaddr))!=0){
		printf("Socket bind failed\n");
		return 1;
	}

	if(listen(sockfd,5) != 0){
		printf("Listen failed");
		return 1;
	}
	int len = sizeof(cli);
	while(1){
		connfd = accept(sockfd, (SA*)&cli, &len);
		if(connfd< 0) {
			printf("Server accept failed\n");
			return 0;
		}
		char* clientip;
		int cliport;
		inet_ntop(AF_INET, &cli.sin_addr,clientip,INET_ADDRSTRLEN);
		cliport = ntohs(cli.sin_port);
		int n = 0;
		char* buff = (char*)malloc(sizeof(char)*MAX);
		bzero(buff,MAX);
		
		while(1){
			read(connfd,buff, sizeof(buff));
			printf("%s\n",buff);
			n = 0;
			while((buff[n++] = getchar())!='\n');
			write(connfd, buff,sizeof(buff));
		}
	}
	return 0;

}
