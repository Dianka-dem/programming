#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct {
    int id;
    char name[100];
    int salary;
    char level[20];
} Employee;

// На оценку 5
void compress__png(char *input, char *output) {
    FILE* in = fopen(input, "rb");
    FILE* out = fopen(output, "wb"); 
   
    
    if (!in || !out) {
        printf("Ошибка\n");
        if (in) fclose(in);
        if (out) fclose(out);
        return;
    }
    
    fseek(in, 0, SEEK_END);
    long size_before = ftell(in);
    fseek(in, 0, SEEK_SET);

    printf("\n=== СЖАТИЕ PNG КАРТИНКИ ===\n");
    printf("Исходный файл: image.png\n");
    printf("Размер до: %.2f Кб (%ld байт)\n", size_before / 1024.0, size_before);
    
    unsigned char byte, next;
    unsigned char count;
    
    while (ftell(in) < size_before) {
        fread(&byte, 1, 1, in);
        count = 1;
        
        while (ftell(in) < size_before && count < 255) {
            fread(&next, 1, 1, in);
            if (next == byte) {
                count++;
            } else {
                fseek(in, -1, SEEK_CUR);
                break;
            }
        }
        
        fwrite(&count, 1, 1, out);
        fwrite(&byte, 1, 1, out);
    }
    
    fseek(out, 0, SEEK_END);
    long size_after = ftell(out);
    
    fclose(in); 
    fclose(out);

    printf("Размер после: %.2f Кб (%ld байт)\n", size_after / 1024.0, size_after);
    printf("Сжатие: %.1f%%\n", (1.0 - (float)size_after / size_before) * 100);
    printf("Сэкономлено: %.2f Кб\n", (size_before - size_after) / 1024.0);
    printf("===========================\n");
}

void decompress_png(char *input, char *output) {
    FILE* in = fopen(input, "rb");
    FILE* out = fopen(output, "wb");
    
    if (!in || !out) {
        printf("Ошибка открытия файлов для восстановления\n");
        if (in) fclose(in);
        if (out) fclose(out);
        return;
    }
    
    unsigned char count, value;
    long restored_size = 0;
    
    while (fread(&count, 1, 1, in) == 1) {
        if (fread(&value, 1, 1, in) == 1) {
            for (int i = 0; i < count; i++) {
                fwrite(&value, 1, 1, out);
                restored_size++;
            }
        }
    }
    
    fflush(out);
    fclose(in);
    fclose(out);

    printf("\n=== ВОССТАНОВЛЕНИЕ PNG ===\n");
    printf("Восстановлен: image_restored.png\n");
    printf("Размер: %.2f Кб (%ld байт)\n", restored_size / 1024.0, restored_size);
    printf("==========================\n");
}


void ID() {
    FILE* db_file = fopen("database.dat", "rb");
    if (db_file == NULL) {
        printf("Ошибка\n");
        return;
    }
    
    Employee emp;
    int search_id;
    
    printf("\nВведите ID сотрудника для поиска: ");
    scanf("%d", &search_id);
    
    int found = 0;
    
    while (fread(&emp, sizeof(Employee), 1, db_file) == 1) {
        if (emp.id == search_id) {
            printf("\n+--------------------------------------+\n");
            printf("|Сотрудник найден:                     |\n"); 
            printf("+--------------------------------------+\n");
            printf("|ID:       %d                           |\n", emp.id);
            printf("|Имя:      %s               |\n", emp.name);
            printf("|Зарплата: %d                        |\n", emp.salary);
            printf("|Уровень:  %s                     |\n", emp.level);
            printf("+--------------------------------------+\n");
            found = 1;
            break;
        }
    }
    if (!found) {
        printf("\nСотрудник с ID %d не найден\n", search_id);
    }
    fclose(db_file);
}


int main(int argc, char* argv[]) {
    if (argc < 2) {
        printf("Использование: %s staff.csv\n", argv[0]);
        return 1;
    }
    
    FILE* file = fopen(argv[1], "r");
    if (file == NULL) {
        printf("Ошибка %s\n", argv[1]);
        return 1;
    }
    
    Employee employees[100];
    char line[256];
    int count = 0;
    
    fgets(line, sizeof(line), file);
    
    while (fgets(line, sizeof(line), file) != NULL && count < 100) {
        line[strcspn(line, "\n")] = '\0';
        
        char* name = strtok(line, ";");
        char* salary_str = strtok(NULL, ";");
        char* level = strtok(NULL, ";");
        
        if (name != NULL && salary_str != NULL && level != NULL) {
            employees[count].id = count + 1;
            strcpy(employees[count].name, name);
            employees[count].salary = atoi(salary_str);
            strcpy(employees[count].level, level);
            count++;
        }
    }
    
    fclose(file);
    
    // printf("\n");
    // printf("+------+------------------------+----------+----------+\n");
    // printf("|  ID  | Имя                    | Зарплата | Уровень  |\n");
    // printf("+------+------------------------+----------+----------+\n");
    
    // for (int i = 0; i < count; i++) {
    //     printf("| %4d | %-22s | %8d | %-8s |\n",
    //            employees[i].id,
    //            employees[i].name,
    //            employees[i].salary,
    //            employees[i].level);
    // }
    
    // printf("+------+------------------------+----------+----------+\n");
    // printf("Всего сотрудников: %d\n", count);
    
    // FILE* db_file = fopen("database.dat", "wb");
    // if (db_file != NULL) {
    //     size_t written = fwrite(employees, sizeof(Employee), count, db_file);
    //     fclose(db_file);
        
    //     if (written == count) {
    //         printf("\nДанные сохранены\n");
    //     } else {
    //         printf("Ошибка при сохранении\n");
    //         return 1;
    //     }
    // }
    
    compress__png("39dc0d67814f328bb4f4f5451f3d075b.png", "img.rle");
    
    decompress_png("img.rle", "restored.png");

    compress__png("database.dat", "database.rle");
    
    decompress_png("database.rle", "database_restored.dat");
    
    // ID();
    
    // return 0;
}