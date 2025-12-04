// #include "server.h"
// #include "queue.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>

// TODO: Read in macros from config.

#define BUFFER_SIZE 1024 
#define PORT 5100
#define MAX_BACKLOG 64       // affects is how many incoming connections can queue up 
                             // if your application isn't accept()ing connections as soon as they come in.

int main(void) {
    struct sockaddr_in my_addr; 
    int sock_fd, rv;
    char buffer[BUFFER_SIZE] = { 0 };
    char *hello = "Hello from server\n";

    sock_fd = socket(AF_INET, SOCK_STREAM, 0);

    if (sock_fd == -1) {
        fprintf(stderr, "Error initializing server socket.\n");
        exit(1);
    }
    
    // bzero depricated
    // bzero(&my_addr, sizeof(my_addr));

    memset(&my_addr, 0, sizeof(my_addr));

    my_addr.sin_family = AF_INET;
    my_addr.sin_port = htons(PORT);
    my_addr.sin_addr.s_addr = htonl(INADDR_ANY); //  allows server to accept client on any interface

    socklen_t addrlen = sizeof(my_addr);

    if ((bind(sock_fd, (struct sockaddr *)&my_addr, sizeof(my_addr))) < 0) {
        fprintf(stderr, "Error binding server socket.\n");
        exit(1);
    }

    int lv = listen(sock_fd, MAX_BACKLOG);
    if (lv == -1) {
        fprintf(stderr, "Error listening for connections on server.\n");
        exit(1);
    }

    int new_sock = accept(sock_fd, (struct sockaddr *)&my_addr, &addrlen);

    if (new_sock < 0) {
        fprintf(stderr, "Error accepting connection from client\n");
        exit(1);
    }

    fprintf(stderr, "Reading from buffer.\n");
    rv = read(new_sock, buffer, BUFFER_SIZE - 1);
    if (rv < 0 ) {
        fprintf(stderr, "Error reading from client.\n");
        exit(1);
    }
    printf("%s\n", buffer);

    close(new_sock);
    close(sock_fd);

    return 0;
}
