#ifndef SERVER_H
#define SERVER_H

#include <stdbool.h>
#include <stddef.h>
#include "queue.h"

#define DEFAULT_SOCKET_PATH "/tmp/etl_queue.sock"

typedef struct Queue Queue;

bool start_server(const char *socket_path, Queue *q);
void stop_server(void);
void handle_client(int client_fd, Queue *q);
int init_server_socket(const char *socket_path);
void cleanup_socket(const char *socket_path);


#endif // SERVER_H