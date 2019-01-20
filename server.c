#include <stdio.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <unistd.h>
#include <stdlib.h>

#define PORT_NUMBER 23

int main(int argc, char *argv[]) {

    int port = -1;
    int socket_fd = 0;
    int client_socket_fd = 0;
    struct sockaddr_in server_address;
    struct sockaddr_in client_address;
    int rc = 0;
    int client_address_len = 0;
    char reply[256];

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
    socket_fd = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP);
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

    // Let's run forever
    while(1) {
        // Let's block until we get a client
        client_address_len = sizeof(client_address);
        client_socket_fd = accept(socket_fd, (struct sockaddr *) &client_address, &client_address_len);
        if (client_socket_fd < 0) {
            perror("Error on accept");
            close(socket_fd);
            exit(1);
        }

        rc = read(client_socket_fd, reply, 256);

        if (rc < 0) {
            close(socket_fd);
            close(client_socket_fd);
            printf("Disconnected");
            exit(0);
        }

       rc = write(client_socket_fd, "hi from sever", 13);
       if (rc < 0 ) {
           perror("Write to socket failed");
       }
       close(client_socket_fd);
    }
    close(socket_fd);
    return 0;
}


