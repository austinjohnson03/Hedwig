#include "server.h"
#include "queue.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <unistd.h>
#include <pthread.h>

#define BUFFER_SIZE 1024

static int server_fd = -1;
static volatile bool running = false;

int init_server_socket(const char *socket_path) {
    struct sockaddr_un addr;
    int fd;

    if ((fd = socket(AF_UNIX, SOCK_STREAM, 0)) == -1) {
        perror("socket");
        return -1;
    }

    memset(&addr, 0, sizeof(addr));
    addr.sun_family = AF_UNIX;
    strncpy(addr.sun_path, socket_path, sizeof(addr.sun_path) - 1);
    unlink(socket_path);

    if (bind(fd, (struct sockaddr*)&addr, sizeof(addr)) == -1) {
        perror("bind");
        close(fd);
        return -1;
    }

    if (listen(fd, 5) == -1) {
        perror("listen");
        close(fd);
        return -1;
    }

    return fd;
}

void handle_client(int client_fd, Queue *q) {
    char buffer[BUFFER_SIZE];
    ssize_t n = read(client_fd, buffer, BUFFER_SIZE - 1);
    if (n > 0) {
        buffer[n] = '\0';
        printf("Received: %s\n", buffer);
        enqueue(q, buffer, strlen(buffer) + 1);
    }
    close(client_fd);
}

bool start_server(const char *socket_path, Queue *q) {
    if (!q) {
        fprintf(stderr, "Queue cannot be NULL.\n");
        return false;
    }

    socket_path = socket_path ? socket_path : DEFAULT_SOCKET_PATH;
    server_fd = init_server_socket(socket_path);
    if (server_fd == -1) {
        return false;
    }

    running = true;
    printf("Server listening on %s\n", socket_path);

    while (running) {
        int client_fd = accept(server_fd, NULL, NULL);
        if (client_fd == -1) {
            perror("accept");
            continue;
        }
        handle_client(client_fd, q);
    }

    cleanup_socket(socket_path);
    return true;
}

void stop_server(void) {
    running = false;
    if (server_fd != -1) {
        close(server_fd);
    }
}

void cleanup_socket(const char *socket_path) {
    if (socket_path) {
        unlink(socket_path);
    }
}