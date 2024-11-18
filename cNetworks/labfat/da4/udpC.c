#include <unistd.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>

int main(){
	struct sockaddr_in servaddr, cli;
	int sockfd,n;
	char* msg = "HEllo";
	bzero(&servaddr, sizeof(servaddr));
	servaddr.sin_family = AF_INET;
	servaddr.sin_addr.s_addr = inet_addr("127.0.0.1");
	servaddr.sin_port = htons(6969);

	sockfd = socket(AF_INET, SOCK_DGRAM,0);


	sendto(sockfd, msg, sizeof(msg),MSG_WAITALL,(struct sockaddr*)&servaddr, sizeof(servaddr));

	int len = sizeof(servaddr);
	n = recvfrom(sockfd, msg, sizeof(msg),MSG_CONFIRM,(struct sockaddr*)&servaddr, &len);
	printf("%s",msg);
	return 0;
}
