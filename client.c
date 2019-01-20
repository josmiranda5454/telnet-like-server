#include <stdio.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <arpa/inet.h>

#define PORT_NUMBER 23

int main (int argc, char *argv[]) {
    int socket_fd = 0;
    struct sockaddr_in server_address;
    char buf[256];
    char rep[256];
    int rc = 0;
 
    // Let's get a socket
    socket_fd = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if (socket_fd < 1) {
        perror("Error opening our socket");
        exit(1);
    }

    // Let's setup the server_address to connect to
    server_address.sin_family = AF_INET;
    server_address.sin_port = htons(PORT_NUMBER);
    server_address.sin_addr.s_addr = inet_addr("127.0.0.1");

    // Let's connect to the server
    if (connect(socket_fd, (struct sockaddr*) &server_address, sizeof(server_address)) != 0) {
        perror("Error in binding");
        close(socket_fd);
        exit(1);
    }

    while (1) {
        memset(rep, 0, sizeof(rep));
        if (recv(socket_fd, rep, 256, 0) < 0) {
            perror("Receive failed");
            break;
        }
        printf("%s", rep);

        scanf("%s", buf);
        if (send(socket_fd, buf, strlen(buf), 0) < 0) {
            perror("Send failed");
            break;
        }
    }

    close(socket_fd);
    return 0;
}
