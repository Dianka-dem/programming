#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <locale.h>
#include "palindrom.h"
#include "logger.h"

void parse_config(const char* filename, char* log_file, size_t log_file_len, char* log_level, size_t log_level_len, int* num_threads) {
    FILE* file = fopen(filename, "r");
    if (!file) return;

    char line[256];
    while (fgets(line, sizeof(line), file)) {
        line[strcspn(line, "\r\n")] = 0;
        
        char* key = strtok(line, "=");
        char* value = strtok(NULL, "");
        if (key && value) {
            if (strcmp(key, "LOG_FILE") == 0) {
                strncpy(log_file, value, log_file_len - 1);
                log_file[log_file_len - 1] = '\0';
            }
            if (strcmp(key, "LOG_LEVEL") == 0) {
                strncpy(log_level, value, log_level_len - 1);
                log_level[log_level_len - 1] = '\0';
            }
            if (strcmp(key, "NUM_THREADS") == 0) {
                *num_threads = atoi(value);
            }
        }
    }
    fclose(file);
}

int main(int argc, char* argv[]) {
    setlocale(LC_ALL, "");

    char log_file[256] = "default.log";
    char log_level[32] = "INFO";
    int num_threads = 4;
    
    FILE* cfg_check = fopen("config.cfg", "r");
    if (cfg_check) {
        fclose(cfg_check);
        parse_config("config.cfg", log_file, sizeof(log_file), log_level, sizeof(log_level), &num_threads);
    } else {
        parse_config("../config.cfg", log_file, sizeof(log_file), log_level, sizeof(log_level), &num_threads);
    }

    log_init(log_file, log_level);
    log_msg(LOG_INFO, "Program started with %d threads", num_threads);

    if (argc < 2) {
        log_msg(LOG_ERROR, "No input file provided via CLI arguments.");
        fprintf(stderr, "Ошибка: Укажите имя файла. Пример: %s text.txt\n", argv[0]);
        log_close();
        return 1;
    }

    log_msg(LOG_INFO, "Trying to open file: %s", argv[1]);
    FILE* infile = fopen(argv[1], "r");
    if (!infile) {
        log_msg(LOG_ERROR, "Cannot open target file: %s", argv[1]);
        fprintf(stderr, "Критическая ошибка: Не удалось найти или открыть файл '%s'\n", argv[1]);
        perror("Системное сообщение");
        log_close();
        return 1;
    }

    fseek(infile, 0, SEEK_END);
    long size = ftell(infile);
    if (size < 0) {
        log_msg(LOG_ERROR, "Failed to determine file size.");
        fclose(infile);
        log_close();
        return 1;
    }
    fseek(infile, 0, SEEK_SET);

    char* buffer = malloc(size + 1);
    if (!buffer) {
        log_msg(LOG_ERROR, "Memory allocation failed for file buffer.");
        fclose(infile);
        log_close();
        return 1;
    }

    size_t read_bytes = fread(buffer, 1, size, infile);
    buffer[read_bytes] = '\0';
    fclose(infile);

    TextData data;
    preprocess_text(buffer, &data, num_threads);
    find_palindromes_manacher_mt(&data);
    
    free_text_data(&data);
    free(buffer);
    
    log_msg(LOG_INFO, "Program finished successfully");
    log_close();
    return 0;
}
