#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <time.h>

#define PORT 8080

void process_client(int client_sock) {
    char buffer[256] = {0};
    char message[256] = {0};

    srand(time(NULL));  
    int choice = rand() % 2 + 1; // Random 1 or 2

    if (choice == 1) {
        const char *ask_name = "Please enter your Name: ";
        send(client_sock, ask_name, strlen(ask_name), 0);

        read(client_sock, buffer, sizeof(buffer));
        printf("Client says (Name): %s\n", buffer);

        snprintf(message, sizeof(message), "Hello %s! Welcome to the server.\n", buffer);
        send(client_sock, message, strlen(message), 0);

    } else {
        const char *ask_sapid = "Please enter your SAP ID: ";
        send(client_sock, ask_sapid, strlen(ask_sapid), 0);

        read(client_sock, buffer, sizeof(buffer));
        printf("Client says (SAP ID): %s\n", buffer);

        snprintf(message, sizeof(message), "Welcome SAP ID %s! Connection successful.\n", buffer);
        send(client_sock, message, strlen(message), 0);
    }

    close(client_sock);
    printf("Client disconnected.\n");
}

void start_server() {
    int server_fd, client_sock;
    struct sockaddr_in address;
    int addrlen = sizeof(address);

    if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) == 0) {
        perror("Socket failed");
        exit(EXIT_FAILURE);
    }

    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(PORT);

    if (bind(server_fd, (struct sockaddr *)&address, sizeof(address)) < 0) {
        perror("Bind failed");
        close(server_fd);
        exit(EXIT_FAILURE);
    }

    if (listen(server_fd, 5) < 0) {
        perror("Listen failed");
        close(server_fd);
        exit(EXIT_FAILURE);
    }

    printf("Server listening on port %d...\n", PORT);

    client_sock = accept(server_fd, (struct sockaddr *)&address, (socklen_t *)&addrlen);
    if (client_sock < 0) {
        perror("Accept failed");
        close(server_fd);
        exit(EXIT_FAILURE);
    }

    process_client(client_sock);
    close(server_fd);
}

int main() {
    start_server();
    return 0;
}
