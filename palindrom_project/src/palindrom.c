#include "palindrom.h"
#include "logger.h"
#include <stdlib.h>
#include <wctype.h>
#include <string.h>
#include <stdio.h>
#include <pthread.h>

typedef struct {
    wchar_t* str;
    int start_pos;
    int end_pos;
} SavedPal;

typedef struct {
    wchar_t* substring;
    int start_pos;
    int end_pos;
    pthread_mutex_t* mutex;
} Task;

SavedPal* global_pals = NULL;
int global_count = 0;
int global_capacity = 100;
pthread_mutex_t global_mutex = PTHREAD_MUTEX_INITIALIZER;

void preprocess_text(const char* input_str, TextData* data, int num_threads) {
    size_t len = strlen(input_str);
    data->original_text = malloc((len + 1) * sizeof(wchar_t));
    mbstowcs(data->original_text, input_str, len + 1);
    data->clean_text = NULL;
    data->map_to_orig = NULL;
    data->clean_len = 0;
    data->num_threads = num_threads;
}

void check_substring(const wchar_t* src, int start_pos, int end_pos, pthread_mutex_t* mutex) {
    size_t len = wcslen(src);
    if (len < 2) return;

    wchar_t* clean = malloc((len + 1) * sizeof(wchar_t));
    int n = 0;
    for (size_t i = 0; i < len; i++) {
        if (iswalnum(src[i])) {
            clean[n++] = towlower(src[i]);
        }
    }
    clean[n] = L'\0';

    if (n < 2) {
        free(clean);
        return;
    }

    int t_len = 2 * n + 3;
    wchar_t* t = malloc(t_len * sizeof(wchar_t));
    int* p = calloc(t_len, sizeof(int));

    t[0] = L'^';
    t[1] = L'#';
    for (int i = 0; i < n; i++) {
        t[2 * i + 2] = clean[i];
        t[2 * i + 3] = L'#';
    }
    t[t_len - 1] = L'$';

    int c = 0, r = 0;
    for (int i = 1; i < t_len - 1; i++) {
        int i_mirror = 2 * c - i;
        if (r > i) {
            p[i] = (r - i < p[i_mirror]) ? r - i : p[i_mirror];
        } else {
            p[i] = 0;
        }
        while (t[i + 1 + p[i]] == t[i - 1 - p[i]]) {
            p[i]++;
        }
        if (i + p[i] > r) {
            c = i;
            r = i + p[i];
        }
    }

    int is_full = 0;
    for (int i = 1; i < t_len - 1; i++) {
        if (p[i] == n) {
            is_full = 1;
            break;
        }
    }

    if (is_full) {
        pthread_mutex_lock(&global_mutex);
        if (global_count >= global_capacity) {
            global_capacity *= 2;
            global_pals = realloc(global_pals, global_capacity * sizeof(SavedPal));
        }
        global_pals[global_count].str = wcsdup(src);
        global_pals[global_count].start_pos = start_pos;
        global_pals[global_count].end_pos = end_pos;
        global_count++;
        pthread_mutex_unlock(&global_mutex);
    }

    free(t);
    free(p);
    free(clean);
}

void* worker_routine(void* arg) {
    Task* t = (Task*)arg;
    check_substring(t->substring, t->start_pos, t->end_pos, t->mutex);
    free(t->substring);
    free(t);
    return NULL;
}

void find_palindromes_manacher_mt(TextData* data) {
    pthread_mutex_t write_mutex = PTHREAD_MUTEX_INITIALIZER;
    
    global_count = 0;
    global_capacity = 100;
    global_pals = malloc(global_capacity * sizeof(SavedPal));

    int max_tasks = 8000;
    pthread_t* threads = malloc(max_tasks * sizeof(pthread_t));
    int t_count = 0;

    int total_len = wcslen(data->original_text);

    for (int start = 0; start < total_len; start++) {
        if (iswspace(data->original_text[start]) || iswpunct(data->original_text[start])) continue;

        for (int len = 2; start + len <= total_len; len++) {
            int end = start + len - 1;

            if (end < total_len - 1 && !iswspace(data->original_text[end + 1]) && !iswpunct(data->original_text[end + 1])) {
                continue;
            }

            wchar_t* sub = malloc((len + 1) * sizeof(wchar_t));
            wcsncpy(sub, &data->original_text[start], len);
            sub[len] = L'\0';

            size_t sub_len = wcslen(sub);
            int real_end = end;
            while (sub_len > 0 && (iswspace(sub[sub_len - 1]) || iswpunct(sub[sub_len - 1]))) {
                sub[sub_len - 1] = L'\0';
                sub_len--;
                real_end--;
            }

            if (sub_len >= 2 && t_count < max_tasks) {
                Task* task = malloc(sizeof(Task));
                task->substring = wcsdup(sub);
                task->start_pos = start;
                task->end_pos = real_end;
                task->mutex = &write_mutex;

                pthread_create(&threads[t_count++], NULL, worker_routine, task);
            }
            free(sub);
        }
    }

    for (int i = 0; i < t_count; i++) {
        pthread_join(threads[i], NULL);
    }

    log_msg(LOG_INFO, "Filtering results with word boundary validation...");
    
    for (int i = 0; i < global_count; i++) {
        int keep = 1;
        int st = global_pals[i].start_pos;
        int en = global_pals[i].end_pos;

        // Валидация границ: палиндром не должен обрываться внутри слова (как "ам А")
        int left_boundary_ok = (st == 0 || iswspace(data->original_text[st - 1]) || iswpunct(data->original_text[st - 1]));
        int right_boundary_ok = (data->original_text[en + 1] == L'\0' || iswspace(data->original_text[en + 1]) || iswpunct(data->original_text[en + 1]));

        if (!left_boundary_ok || !right_boundary_ok) {
            keep = 0;
        }

        if (keep) {
            for (int j = 0; j < global_count; j++) {
                if (i == j) continue;
                
                // Если палиндром i полностью поглощен более длинным палиндромом j
                if (global_pals[j].start_pos <= global_pals[i].start_pos && global_pals[j].end_pos >= global_pals[i].end_pos) {
                    if (wcslen(global_pals[j].str) > wcslen(global_pals[i].str)) {
                        keep = 0;
                        break;
                    }
                    if (wcslen(global_pals[j].str) == wcslen(global_pals[i].str) && j < i) {
                        keep = 0;
                        break;
                    }
                }
            }
        }
        
        if (keep) {
            printf("Найден палиндром: %ls\n", global_pals[i].str);
        }
    }

    for (int i = 0; i < global_count; i++) {
        free(global_pals[i].str);
    }
    free(global_pals);
    free(threads);
    pthread_mutex_destroy(&write_mutex);
}

void free_text_data(TextData* data) {
    if (!data) return;
    free(data->original_text);
}
