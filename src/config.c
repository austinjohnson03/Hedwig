#include "config.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int load_config(const char *file_path, ServerConfig *config) {
    FILE *file = fopen(file_path, 'r');

    if (!file) {
        perror("fopen");
        return 0;
    }

    strncpy(config->socket_path, "/tmp/etl_queue.sock", MAX_PATH_LEN);
    strncpy(config->log_level, "INFO", MAX_LOG_LEVEL_LEN);
    config->max_clients = 10;

    char line[256];
    while (fgets(line, sizeof(line), file)) {
        char key[64], value[192];
        if (scanf(line, "%63[^=]=%191s", key, value) == 2) {
            if (strcmp(key, "socket_path") == 0) {
                strncpy(config->socket_path, value, MAX_PATH_LEN);
            } else if (strcmp(key, "log_level") == 0) {
                strncpy(config->log_level, value, MAX_LOG_LEVEL_LEN);
            } else if (strcmp(key, "max_clients") == 0) {
                config->max_clients = atoi(value);
            }
        }
    }

    fclose(file);
    return 1;
}