#include <arpa/inet.h> 
#include <netdb.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <strings.h>
#include <sys/socket.h>
#include <unistd.h>
#define MAX 80
#define PORT 6969 
#define SA struct sockaddr

int main() {
	struct sockaddr_in servaddr, cli;
	int sockfd, connfd;

	sockfd = socket(AF_INET,SOCK_STREAM,0);
	if(sockfd == -1){
		printf("socket creation failed!!\n");
		exit(1);
	}else{
		printf("Socket created!!\n");
	}

	bzero(&servaddr,sizeof(servaddr));

	servaddr.sin_family = AF_INET;
	servaddr.sin_addr.s_addr = inet_addr("127.0.0.1");
	servaddr.sin_port = htons(PORT);

	if(connect(sockfd, (SA*)&servaddr, sizeof(servaddr))!=0){
		printf("Connection failed!!\n");
		exit(1);
	}else{
		printf("Connected to server!!\n");
	}

	char buff[MAX];
	int n;
	while(1) {
		bzero(buff, sizeof(buff));
		printf("Message: ");
		while((buff[n++] = getchar()) != '\n');
		n = 0;
		write(sockfd,buff,sizeof(buff));
		bzero(buff, sizeof(buff));
		read(sockfd,buff, sizeof(buff));
		printf("From server: %s\n",buff);
		if(strncmp(buff, "exit",4)==0){
			printf("Client exit");
			break;
		}
	}
	close(sockfd);
}
