#include <iostream>
#include <cstring>
#include <unistd.h>
#include <arpa/inet.h>

#define PORT 8080
#define BUFFER_SIZE 1024

int main() {
    int sock = 0;
    struct sockaddr_in serv_addr;
    char buffer[BUFFER_SIZE] = {0};

    // Create socket
    if ((sock = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
        perror("Socket creation error");
        return -1;
    }

    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(PORT);

    // Convert IPv4 and IPv6 addresses from text to binary form
    if (inet_pton(AF_INET, "127.0.0.1", &serv_addr.sin_addr) <= 0) {
        perror("Invalid address/ Address not supported");
        return -1;
    }

    // Connect to server
    if (connect(sock, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0) {
        perror("Connection failed");
        return -1;
    }

    // Ask for username
    char username[BUFFER_SIZE] = {0};
    std::cout << "Enter your username: ";
    std::cin.getline(username, BUFFER_SIZE);

    // Receive server username
    recv(sock, buffer, BUFFER_SIZE, 0);
    std::string server_username(buffer);
    std::cout << "Connected to server: " << server_username << "\n";

    // Send client username to server
    send(sock, username, strlen(username), 0);

    while (true) {
        // Send message to server
        std::cout << username << ": ";
        std::cin.getline(buffer, BUFFER_SIZE);
        send(sock, buffer, strlen(buffer), 0);

        // Receive message from server
        memset(buffer, 0, BUFFER_SIZE);
        int bytes_read = recv(sock, buffer, BUFFER_SIZE, 0);
        if (bytes_read <= 0) {
            std::cout << "Server disconnected.\n";
            break;
        }
        std::cout << server_username << ": " << buffer << "\n";
    }

    close(sock);
    return 0;
}
