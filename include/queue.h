#ifndef QUEUE_H
#define QUEUE_H

#include <stdlib.h>
#include <stdbool.h>
#include <pthread.h>

typedef struct Node {
    void *data;
    size_t size;
    struct Node *next;
} Node;

typedef struct Queue {
    struct Node *front;
    struct Node *back;
    size_t count;

    pthread_mutex_t lock;
    pthread_cond_t not_empty;
} Queue;

void queue_init(Queue *q);
static Node* create_node(const void *data, size_t size);
static void free_node(Node *node);
void enqueue(Queue *q, const void *data, size_t size);
Node* dequeue(Queue *q);
Node* peek(Queue *q);
bool is_empty(Queue *q);
size_t queue_size(Queue *q);

void destroy_queue(Queue *q);


#endif // QUEUE_H
