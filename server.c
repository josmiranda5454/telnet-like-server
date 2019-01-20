#include <stdio.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>

#define PORT_NUMBER 23
#define MAX_ATTEMPTS 3

#define USER "test"
#define PASS "test1"
#define LOGOUT "LOGOUT"

int loginPrompt(int socket_fd);
int main(int argc, char *argv[]) {

    int port = -1;
    int socket_fd = 0;
    int client_socket_fd = 0;
    struct sockaddr_in server_address;
    struct sockaddr_in client_address;
    int rc = 0;
    int client_address_len = 0;
    char reply[256];
    char sendbuf[256];
    unsigned char logged = 0;

    // Check the arguments before proceeding
    if (argc < 2 || argc >= 3) {
        fprintf(stderr, "Usage: ./server <port_number>\n");
        return 1;
    }

    // Convert the string to an argument
    port = atoi(argv[1]);

    if (port != PORT_NUMBER) {
        fprintf(stderr, "Usage: ./server <port_number>, port_number = 23\n");
        exit(1);
    }

    // Let's create a socket
    socket_fd = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if (socket_fd < 1) {
        perror("Error opening our socket");
        exit(1);
    }

    // Let's setup the server_address
    server_address.sin_family = AF_INET;
    server_address.sin_port = htons(port);
    server_address.sin_addr.s_addr = htonl(INADDR_ANY);

    // Let's bind it
    if (bind(socket_fd, (struct sockaddr*) &server_address, sizeof(server_address)) != 0) {
        perror("Error in binding");
        close(socket_fd);
        exit(1);
    }

    // Let's listen now for only one connection for this purposes
    rc = listen(socket_fd, 1);
    if (rc != 0 ) {
        perror("Error listening");
        close(socket_fd);
        exit(1);
    }

    client_address_len = sizeof(client_address);
    client_socket_fd = accept(socket_fd, (struct sockaddr *) &client_address, &client_address_len);
    if (client_socket_fd < 0) {
        perror("Error on accept");
        close(socket_fd);
        exit(1);
    }
    
    while(1) {
        if (!logged) { 
            if (loginPrompt(client_socket_fd) == 0) {
                send(client_socket_fd, "login failed", 12, 0);
                fprintf(stderr, "Login failed\n");
                break;
            } else {
                logged = 1;
            }
        }
        memset(reply, 0, sizeof(reply));
        rc = recv(client_socket_fd, reply, 256, 0);

        if (rc < 0) {
            break;
        }

        if (strcmp(reply, LOGOUT) == 0) {
            send(client_socket_fd, "Session Terminated, you can leave", 33, 0);
            break;
        }

        snprintf(sendbuf, sizeof(sendbuf),  "%s\n", reply);
        sendbuf[255] = '\0'; 
        rc = send(client_socket_fd, sendbuf, strlen(sendbuf), 0);
        if (rc < 0 ) {
            perror("Write to socket failed");
        }
    }
    close(socket_fd);
    close(client_socket_fd);
    printf("Disconnected\n");
    return 0;
}

int loginPrompt(int socket_fd) {
    const char* username = "Username: ";
    const char* password = "Password: ";
    const char* welcome = "Welcome, I can echo whatever\n";
    unsigned char logged = 0;
    int attempts = 0;
    char user[100];
    char pass[100];
    int rc = 0;

    while (attempts < MAX_ATTEMPTS) {
        memset(user, 0, sizeof(user));
        send(socket_fd, username, strlen(username), 0);
        rc = recv(socket_fd, user, 100, 0);
        if (rc < 0) {
            break;
        }
        memset(pass, 0, sizeof(pass));
        send(socket_fd, password, strlen(password), 0);
 
        rc = recv(socket_fd, pass, 100, 0);
        if (rc < 0) {
            break;
        }

        if (strcmp(user, USER) == 0) {
            if (strcmp(pass, PASS) == 0 ) {
                send(socket_fd, welcome, strlen(welcome), 0);
                logged = 1;
                break;
            }
        }
        attempts++;
    }
    return logged;
}
