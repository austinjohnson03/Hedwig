#ifndef CONFIG_H
#define CONFIG_H

#include <stddef.h>

#define MAX_PATH_LEN 256
#define MAX_LOG_LEVEL_LEN 16

typedef struct ServerConfig {
    char socket_path[MAX_PATH_LEN];
    char log_level[MAX_LOG_LEVEL_LEN];
    int max_clients;
} ServerConfig;

int load_config(const char *filepath, ServerConfig *config);

#endif // CONFIG_H