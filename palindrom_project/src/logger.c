#include "logger.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>
#include <time.h>

static FILE* log_file = NULL;
static LogLevel min_level = LOG_INFO;

void log_init(const char* filename, const char* level_str) {
    if (level_str) {
        if (strcmp(level_str, "WARNING") == 0) min_level = LOG_WARNING;
        else if (strcmp(level_str, "ERROR") == 0) min_level = LOG_ERROR;
    }
    if (filename && strlen(filename) > 0) {
        log_file = fopen(filename, "w");
    }
    if (!log_file) log_file = stdout;
}

void log_msg(LogLevel level, const char* format, ...) {
    if (level < min_level) return;
    
    const char* lvl_names[] = {"INFO", "WARNING", "ERROR"};
    time_t rawtime;
    struct tm * timeinfo;
    char time_buffer[80];

    time(&rawtime);
    timeinfo = localtime(&rawtime);
    strftime(time_buffer, sizeof(time_buffer), "%Y-%m-%d %H:%M:%S", timeinfo);

    fprintf(log_file, "[%s] [%s] ", time_buffer, lvl_names[level]);
    
    va_list args;
    va_start(args, format);
    vfprintf(log_file, format, args);
    va_end(args);
    
    fprintf(log_file, "\n");
    fflush(log_file);
}

void log_close(void) {
    if (log_file && log_file != stdout) {
        fclose(log_file);
    }
}
