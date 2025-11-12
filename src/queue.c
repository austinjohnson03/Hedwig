#include "queue.h"

void queue_init(Queue *q) {
    q->front = NULL;
    q->back = NULL;
    q->count = 0;
    pthread_mutex_init(&q->lock, NULL);
    pthread_cond_init(&q->not_empty, NULL);
}

static Node* create_node(const void *data, size_t size) {
    Node *node = malloc(sizeof(Node));
    if (!node) {
        fprintf(stderr, "Unable to allocate memory for new Node.\n");
        return NULL;
    }

    node->data = malloc(size);
    if (!node->data) {
        free(node);
        fprintf(stderr, "Unable to allocate memory for Node data.\n");
        return NULL;
    }

    memcpy(node->data, data, size);
    node->size = size;
    node->next = NULL;
    return node;
}

static void free_node(Node *node) {
    if (!node) return;

    if (node->data) {
        free(node->data);
        node->data = NULL;
    }

    node->next = NULL;
    free(node);
}

void enqueue(Queue *q, const void *data, size_t size) {
    Node *node = create_node(data, size);
    if (!node) return;

    pthread_mutex_lock(&q->lock);
    if (q->back) {
        q->back->next = node;
    } else {
        q->front = node;
    }

    q->back = node;
    q->count++;
    pthread_cond_signal(&q->not_empty);
    pthread_mutex_unlock(&q->lock);
}

Node* dequeue(Queue *q) {
    pthread_mutex_lock(&q->lock);
    while (q->count == 0) {
        pthread_cond_wait(&q->not_empty, &q->lock);
    }

    Node *node = q->front;
    q->front = node->next;

    if (!q->front) {
        q->back = NULL;
    }

    q->count--;
    pthread_mutex_unlock(&q->lock);

    node->next = NULL;
    return node;
}

Node* peek(Queue *q) {
    pthread_mutex_lock(&q->lock);
    Node *node = q->front;
    pthread_mutex_unlock(&q->lock);
    return node;
}

bool is_empty(Queue *q) {
    pthread_mutex_lock(&q->lock);
    bool empty = (q->count == 0);
    pthread_mutex_unlock(&q->lock);
    return empty;
}

size_t queue_size(Queue *q) {
    pthread_mutex_lock(&q->lock);
    size_t size = q->count;
    pthread_mutex_unlock(&q->lock);
    return size;
}

void destroy_queue(Queue *q) {
    pthread_mutex_lock(&q->lock);

    Node *current = q->front;
    while (current) {
        Node *next = current->next;
        free_node(current);
        current = next;
    }

    q->front = NULL;
    q->back = NULL;
    q->count = 0;

    pthread_mutex_unlock(&q->lock);

    pthread_mutex_destroy(&q->lock);
    pthread_cond_destroy(&q->not_empty);
}
