#include <stdio.h>
#include <pthread.h>
#include <unistd.h>
#include <stdlib.h>

// ЗАДАНИЕ 1 (Параллельная штука)
void* child1(void* arg) {
    for (int i = 1; i <= 5; i++) {
        printf("Дочерний: %d\n", i);
    }
    return NULL;
}

int main_task1() {
    pthread_t thread;
    pthread_create(&thread, NULL, child1, NULL);
    for (int i = 1; i <= 5; i++) {
        printf("Родитель: %d\n", i);
    }
    pthread_join(thread, NULL);
    return 0;
}

//ЗАДАНИЕ 2 (Последовательная штука)
void* child2(void* arg) {
    for (int i = 1; i <= 5; i++) {
        printf("Дочерний: %d\n", i);
    }
    return NULL;
}

int main_task2() {
    pthread_t thread;
    pthread_create(&thread, NULL, child2, NULL);
    pthread_join(thread, NULL);
    for (int i = 1; i <= 5; i++) {
        printf("Родитель: %d\n", i);
    }
    return 0;
}

//ЗАДАНИЕ 3 
void* print_strings(void* arg) {
    char** strings = (char**)arg;
    
    for (int i = 0; strings[i] != NULL; i++) {
        printf("%s\n", strings[i]);
    }
    return NULL;
}

int main_task3() {
    pthread_t threads[4];
    
    char* strings1[] = {"Поток 1: Привет", "Поток 1: Как дела?", "Поток 1: Пока", NULL};
    char* strings2[] = {"Поток 2: Раз", "Поток 2: Два", "Поток 2: Три", NULL};
    char* strings3[] = {"Поток 3: A", "Поток 3: B", "Поток 3: C", "Поток 3: D", NULL};
    char* strings4[] = {"Поток 4: Первая строка", "Поток 4: Последняя строка", NULL};
    
    pthread_create(&threads[0], NULL, print_strings, strings1);
    pthread_create(&threads[1], NULL, print_strings, strings2);
    pthread_create(&threads[2], NULL, print_strings, strings3);
    pthread_create(&threads[3], NULL, print_strings, strings4);
    
    for (int i = 0; i < 4; i++) {
        pthread_join(threads[i], NULL);
    }
    return 0;
}

//ЗАДАНИЕ 4
void* child4(void* arg){
    int* ptr = arg;
    int id = *ptr;
    
    for (int i = 1; i <= 10; i++){
        printf("Поток %d: строка %d\n", id, i);
        sleep(1);
    }
    return NULL;
}
int main_task4(){
    pthread_t threads[4];
    int ids[4] = {1, 2, 3, 4};

    for (int i=0; i < 4; i++){
        pthread_create(&threads[i], NULL, child4, &ids[i]);   
    }
    for (int i = 0; i < 4; i++) {
        pthread_join(threads[i], NULL);
    }
    return 0;
}

//ЗАДАНИЕ 5
void cleanup_handler(void* arg){
    int* id = (int*)arg;
    printf("Поток %d: завершается\n", *id);
}

void* child5(void* arg){
    int* ptr = (int*)arg;
    int id = *ptr;

    pthread_cleanup_push(cleanup_handler, ptr);

    for (int i=1; i<=10; i++){
        printf("Поток %d: строка %d\n", id, i);
        sleep(1);
    }
    pthread_cleanup_pop(1);
    return NULL;
}
int main_task5(){
    pthread_t threads[4];
    int ids[4] = {1, 2, 3, 4};

    for (int i = 0; i < 4; i++){
        pthread_create(&threads[i], NULL, child5, &ids[i]);
    }
    for (int i = 0; i < 4; i++){
        pthread_join(threads[i], NULL);
    }
    return 0;
}

//ЗАДАНИЕ 6
typedef struct{
    int value;
    int index;
}sleepsort_data_t;

void* sleep_sort_thread(void* arg){
    sleepsort_data_t* data = (sleepsort_data_t*)arg;
    int value = data->value;

    sleep(value);

    printf("%d", value);

    free(data);
    return NULL;
}
void sleepsort(int arr[], int n){
    pthread_t threads[n];

    printf("Исходный массив: ");
    for (int i = 0; i < n; i++){
        printf("%d", arr[i]);
    }
    printf("\n");

    printf("Отсортированный массив:");

    for (int i = 0; i < n; i++){
        sleepsort_data_t* data = (sleepsort_data_t*)malloc(sizeof(sleepsort_data_t));
        data->value = arr[i];
        data->index = i;
        pthread_create(&threads[i], NULL, sleep_sort_thread, data);
    }
    for (int i = 0; i < n; i++){
        pthread_join(threads[i], NULL);
    }
    printf("\n");
}

int main_task6(){
    int arr1[] = {5, 2, 8, 1, 9, 3, 7, 4, 6};
    int n1 = sizeof(arr1) / sizeof(arr1[0]);

    printf("Пример 1:\n");
    sleepsort(arr1, n1);

    printf("\n");

    int arr2[] = {3, 1, 4, 1, 5, 9, 2, 6, 5, 3};
    int n2 = sizeof(arr2) / sizeof(arr2[0]);

    printf("Пример 2:\n");
    sleepsort(arr2, n2);

    printf("\n");

    int arr3[50];
    for (int i = 0; i <50; i++){
        arr3[i] = rand() % 20 + 1;
    }
    int n3 = sizeof(arr3) / sizeof(arr3[0]);

    printf("Пример 3:\n");
    sleepsort(arr3, n3);

    return 0;
}

//ЗАДАНИЕ 7 
pthread_mutex_t mutex;
int turn = 0; 

void* child7(void* arg) {
    for (int i = 1; i <= 5; i++) {
        while (1) {
            pthread_mutex_lock(&mutex);
            
            if (turn == 1) {
                printf("Дочерний: %d\n", i);
                turn = 0;
                pthread_mutex_unlock(&mutex);
                break;
            }
            
            pthread_mutex_unlock(&mutex);
            usleep(1000);
        }
    }
    return NULL;
}

int main_task7() {
    pthread_t thread;
     pthread_mutex_init(&mutex, NULL);
    
    pthread_create(&thread, NULL, child7, NULL);
    
    for (int i = 1; i <= 5; i++) {
        while (1) {
            pthread_mutex_lock(&mutex);
            
            if (turn == 0) {
                printf("Родитель: %d\n", i);
                turn = 1;
                pthread_mutex_unlock(&mutex);
                break;
            }
            
            pthread_mutex_unlock(&mutex);
            usleep(1000);
        }
    }
    
    pthread_join(thread, NULL);
    pthread_mutex_destroy(&mutex);
    
    return 0;
}


int main() {
    // printf("Задание 1\n");
    // main_task1();
    
    // printf("\nЗадание 2\n");
    // main_task2();
    
    // printf("\nЗадание 3\n");
    // main_task3();

    // printf("\nЗадание 4\n");
    // main_task4();

    // printf("\nЗадание 5\n");
    // main_task5();
    
    // printf("\nЗадание 6\n");
    // main_task6();

    printf("\nЗадание 7\n");
    main_task7();

    return 0;
}
