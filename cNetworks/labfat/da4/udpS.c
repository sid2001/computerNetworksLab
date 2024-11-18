// server program for udp connection 
#include <stdio.h> 
#include <strings.h> 
#include <sys/types.h> 
#include <arpa/inet.h> 
#include <sys/socket.h> 
#include<netinet/in.h> 
#define PORT 6969 
#define MAXLINE 1000 

// Driver code 
int main() 
{ 
	char buffer[100]; 
	char *message = "Hello Client"; 
	int listenfd, len; 
	struct sockaddr_in servaddr, cliaddr; 
	bzero(&servaddr, sizeof(servaddr)); 

	// Create a UDP Socket 
	listenfd = socket(AF_INET, SOCK_DGRAM, 0);		 
	servaddr.sin_addr.s_addr = htonl(INADDR_ANY); 
	servaddr.sin_port = htons(PORT); 
	servaddr.sin_family = AF_INET; 

	// bind server address to socket descriptor 
	bind(listenfd, (struct sockaddr*)&servaddr, sizeof(servaddr)); 
	
	//receive the datagram 
	len = sizeof(cliaddr); 
	int n = recvfrom(listenfd, buffer, sizeof(buffer), MSG_WAITALL, (struct sockaddr*)&cliaddr,&len);
	buffer[n] = '\0'; 
	printf("%s",buffer);
		
	// send the response 
	sendto(listenfd, buffer, sizeof(buffer), MSG_CONFIRM, (struct sockaddr*)&cliaddr, len);
	
} 
