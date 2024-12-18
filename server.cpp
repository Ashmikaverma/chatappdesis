#include <iostream>
#include <cstring>
#include <unistd.h>
#include <arpa/inet.h>

#define PORT 8080
#define BUFFER_SIZE 1024

int main() {
    int server_fd, new_socket;
    struct sockaddr_in address;
    int addrlen = sizeof(address);
    char buffer[BUFFER_SIZE] = {0};

    // Create socket file descriptor
    if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) == 0) {
        perror("Socket failed");
        exit(EXIT_FAILURE);
    }

    // Bind the socket to the port
    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(PORT);

    if (bind(server_fd, (struct sockaddr *)&address, sizeof(address)) < 0) {
        perror("Bind failed");
        close(server_fd);
        exit(EXIT_FAILURE);
    }

    // Listen for incoming connections
    if (listen(server_fd, 3) < 0) {
        perror("Listen failed");
        close(server_fd);
        exit(EXIT_FAILURE);
    }
    std::cout << "Server listening on port " << PORT << "...\n";

    // Accept a connection
    if ((new_socket = accept(server_fd, (struct sockaddr *)&address, 
                             (socklen_t *)&addrlen)) < 0) {
        perror("Accept failed");
        close(server_fd);
        exit(EXIT_FAILURE);
    }

    // Ask for username
    char username[BUFFER_SIZE] = {0};
    std::cout << "Enter your username: ";
    std::cin.getline(username, BUFFER_SIZE);

    // Send server username to the client
    send(new_socket, username, strlen(username), 0);

    // Receive client username
    recv(new_socket, buffer, BUFFER_SIZE, 0);
    std::string client_username(buffer);
    std::cout << "Connected to client: " << client_username << "\n";

    while (true) {
        memset(buffer, 0, BUFFER_SIZE);
        // Receive message from client
        int bytes_read = recv(new_socket, buffer, BUFFER_SIZE, 0);
        if (bytes_read <= 0) {
            std::cout << "Client disconnected.\n";
            break;
        }
        std::cout << client_username << ": " << buffer << "\n";

        // Send message to client
        std::cout << username << ": ";
        std::cin.getline(buffer, BUFFER_SIZE);
        send(new_socket, buffer, strlen(buffer), 0);
    }

    close(new_socket);
    close(server_fd);
    return 0;
}
