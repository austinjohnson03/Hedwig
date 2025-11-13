#ifndef LOGGING_H
#define LOGGING_H

#include <stdio.h>
#include <stdbool.h>

typedef enum {
    LOG_DEBUG = 0,
    LOG_INFO,
    LOG_WARN,
    LOG_ERROR
} LogLevel;

typedef struct Logger {
    FILE *file;
    LogLevel level;
    bool use_stdout; 
} Logger;

void init_logger(Logger *logger, const char *filepath, LogLevel level, bool use_stdout);
void logger_log(Logger *logger, LogLevel level, const char *fmt, ...);
void logger_close(Logger *logger);

extern Logger global_logger;


#endif // LOGGING_H