#include <stdlib.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <stdio.h>

#define CLIENT_LIMIT 10
#define PORT 6980
#define MESSAGE_LIMIT 50
#define MESSAGE_SIZE 1000
#define HOST "127.0.0.1"


struct Client {
	char *message[MESSAGE_LIMIT];
	int ptr;
	int sockfd;
	int cli_id;
	struct sockaddr_in addr;
};

struct TcpAgent {
	struct sockaddr_in serv_addr;
	int sockfd;
	int ptr;
	struct Client *clients[CLIENT_LIMIT];
};

int startServer(struct TcpAgent* serv) {
	char *ip = HOST;
	serv->serv_addr.sin_family = AF_INET;
	serv->serv_addr.sin_port = htons(PORT);
	serv->serv_addr.sin_addr.s_addr = inet_addr(ip); 
	
	//memset(&(serv->serv_addr.sin_zero),'\0',8);
	//printf("Inside\n");
	// 0 lets it choose the protocol itself work 99.99% of the time
	if((serv->sockfd = socket(PF_INET, SOCK_STREAM, 0)) == -1) {
		printf("\n------Error creating socket!!------\n");
		return -1;
	}
	//printf("a\n");
	if(bind(serv->sockfd, (struct sockaddr*)&(serv->serv_addr), sizeof(struct sockaddr)) == -1) {
		printf("\n------Error binding to port!!-----\n");
		return -1;
	}
	//printf("sd\n");
	if(listen(serv->sockfd, 10) == -1) {
		printf("\n------Error listening------\n");
		return -1;
	} else {
		printf("\nServer listening: %s:%d\n",HOST,PORT);
	}
	//printf("sd\n");
	return 0;
}

void fileHandler(char *data, int cli_id){
	FILE *fptr;
	fptr = fopen("file_s_client.txt","w");
	if(fptr == NULL) {
		printf("Error opening file\n");
	}else{
		printf("\nStoring file from client%d...\n",cli_id);
		fprintf(fptr,"%s",data);
		printf("File received\n");
	}
	fclose(fptr);
	return;
}

void messageHandler(char* msg, int cli_id) {
	char *data = (char *)malloc(sizeof(char)*100);
	switch (msg[0]) {
		case '0':
			sprintf(data,"%s",msg+2);
			fileHandler(data, cli_id);
			free(data);
			break;
		case '1':
			sprintf(data,"%s",msg+2);
			printf("\nclient%d: %s\n",cli_id, data);
			break;
		default:
			printf("\nInvalid message format!\n");
	}
	free(data);
	return;
}	

void receive(int sockfd, struct Client* cli) {
	int p = cli->ptr;
	while(1) {
		
		cli->message[p%MESSAGE_LIMIT] = (char*)malloc(sizeof(char)*MESSAGE_SIZE);
		int r_bits = recv(sockfd, cli->message[p%MESSAGE_LIMIT],MESSAGE_SIZE,0);
		if(r_bits==0) {
			printf("\nClient%d disconnect\n",cli->cli_id);
			close(sockfd);
			return;
		}else if(r_bits<0){
			perror("Error occurred!");
			close(sockfd);
			return;
		}
		messageHandler(cli->message[p%MESSAGE_LIMIT],cli->cli_id);
		p = p+1;
	}
}

int main() {
	struct TcpAgent serv;
	serv.ptr = 0;
	
	if(startServer(&serv)==-1) {
		exit(1);
	}
	int sin_size;
	while(1) {
		struct sockaddr_in cli_addr;
		int fd;
		if((fd = accept(serv.sockfd, (struct sockaddr *)&(cli_addr), &sin_size))==-1){
			printf("\nError accepting connection\n");
		} else {
			int ptr = serv.ptr;
			struct Client* cli = (struct Client*)malloc(sizeof(struct Client));
			cli->sockfd = fd;
			cli->addr = cli_addr;
			cli->cli_id = ptr;
			serv.clients[ptr] = (struct Client*) malloc(sizeof(struct Client));
			serv.clients[ptr] = cli;
			//char* addr = inet_ntoa(cli->addr);
			char client_ip[100];
			inet_ntop(AF_INET, &cli_addr.sin_addr, client_ip, sizeof(client_ip));
			printf("\n\rConnection accepted from %s\n",client_ip);
			receive(serv.clients[ptr]->sockfd, serv.clients[ptr]);
			serv.ptr++;
			fd = -1;
		}
	
	}
}
