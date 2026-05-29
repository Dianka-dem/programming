#ifndef LOGGER_H
#define LOGGER_H

typedef enum {
    LOG_INFO,
    LOG_WARNING,
    LOG_ERROR
} LogLevel;

void log_init(const char* filename, const char* level_str);
void log_msg(LogLevel level, const char* format, ...);
void log_close(void);

#endif
