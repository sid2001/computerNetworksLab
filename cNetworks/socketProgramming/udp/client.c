#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <arpa/inet.h>
#include <unistd.h>

#define SERVER_IP "127.0.0.1"
#define PORT 8080
#define BUFFER_SIZE 1024

int main() {
    int client_fd;
    struct sockaddr_in server_addr;
    char buffer[BUFFER_SIZE];
    socklen_t addr_len = sizeof(server_addr);
    int sent_len, received_len;

    client_fd = socket(AF_INET, SOCK_DGRAM, 0);
    if (client_fd < 0) {
        perror("Socket creation failed");
        exit(1);
    }

    memset(&server_addr, 0, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(PORT);
    server_addr.sin_addr.s_addr = inet_addr(SERVER_IP);

    printf("Enter a message to send to the server: ");
    fgets(buffer, BUFFER_SIZE, stdin);

    sent_len = sendto(client_fd, buffer, strlen(buffer), 0, 
                      (struct sockaddr*)&server_addr, addr_len);
    if (sent_len < 0) {
        perror("Send failed");
        close(client_fd);
        exit(1);
    }

    received_len = recvfrom(client_fd, buffer, BUFFER_SIZE, 0, 
                            (struct sockaddr*)&server_addr, &addr_len);
    if (received_len < 0) {
        perror("Receive failed");
        close(client_fd);
        exit(1);
    }

    buffer[received_len] = '\0';
    printf("Echoed message from server: %s\n", buffer);

    close(client_fd);

    return 0;
}
