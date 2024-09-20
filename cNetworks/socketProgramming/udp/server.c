#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <arpa/inet.h>
#include <unistd.h>

#define PORT 8080
#define BUFFER_SIZE 1024

int main() {
    int server_fd;
    struct sockaddr_in server_addr, client_addr;
    char buffer[BUFFER_SIZE];
    socklen_t addr_len = sizeof(client_addr);
    int received_len;

    server_fd = socket(AF_INET, SOCK_DGRAM, 0);
    if (server_fd < 0) {
        perror("Socket creation failed");
        exit(EXIT_FAILURE);
    }

    memset(&server_addr, 0, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = INADDR_ANY;
    server_addr.sin_port = htons(PORT);

    if (bind(server_fd, (struct sockaddr*)&server_addr, sizeof(server_addr)) < 0) {
        perror("Bind failed");
        close(server_fd);
        exit(EXIT_FAILURE);
    }

    printf("UDP Echo Server is running on port %d...\n", PORT);

    while (1) {
        memset(buffer, 0, BUFFER_SIZE);

        received_len = recvfrom(server_fd, buffer, BUFFER_SIZE, 0, 
                                (struct sockaddr*)&client_addr, &addr_len);
        if (received_len < 0) {
            perror("Receive failed");
            continue;
        }

        printf("Received message: %s\n", buffer);

        if (sendto(server_fd, buffer, received_len, 0, 
                   (struct sockaddr*)&client_addr, addr_len) < 0) {
            perror("Send failed");
        }
    }

    close(server_fd);

    return 0;
}
