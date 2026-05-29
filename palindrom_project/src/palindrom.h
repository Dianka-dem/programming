#ifndef PALINDROM_H
#define PALINDROM_H

#include <wchar.h>

typedef struct {
    wchar_t* original_text;
    wchar_t* clean_text;
    int* map_to_orig;
    int clean_len;
    int num_threads;
} TextData;

void preprocess_text(const char* input_str, TextData* data, int num_threads);
void find_palindromes_manacher_mt(TextData* data);
void free_text_data(TextData* data);

#endif
