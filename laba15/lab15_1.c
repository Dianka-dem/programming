#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct {
    int id;
    char name[100];
    int salary;
    char level[20];
} Employee;

void Binary(Employee* employees, int count,const char* filename){
    FILE* file = fopen(filename, "wb");
    if (file == NULL) {
        printf("Ошибка: не могу открыть файл %s\n", filename);
        return 1;
    }
}  
fwrite(&count, sizeof(int), 1, file);
fwrite(employess, sizeof(Employee), count, file);
fclose(file);
printf("Сохранено %d сотрудников в файл %s\n", count, filename);


    
int main(int argc, char* argv[]){
    if (argc < 2){
        printf("Использование: %s staff.cvs\n", argv[0]);
        return 1;
    }
    FILE* file = fopen(argv[1], "r");
        if (file == NULL) {
            printf("Ошибка: не могу открыть файл %s\n", argv[1]);
            return 1;
    }
    Employee employees[100];
    char line[256];
    int count = 0;

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
    
    if (count == 0) {
        printf("Не удалось прочитать данные из файла\n");
        printf("Проверьте формат файла. Должно быть:\n");
        printf("Имя;Зарплата;Уровень\n");
        printf("Пример: Ivan Ivanov;5000;Level_A\n");
        return 1;
    }
    
    printf("\n");
    printf("+------+------------------------+----------+----------+\n");
    printf("|  ID  | Имя                    | Зарплата | Уровень  |\n");
    printf("+------+------------------------+----------+----------+\n");
    
    for (int i = 0; i < count; i++) {
        printf("| %4d | %-22s | %8d | %-8s |\n",
               employees[i].id,
               employees[i].name,
               employees[i].salary,
               employees[i].level);
    }
    
    printf("+------+------------------------+----------+----------+\n");
    printf("Всего сотрудников: %d\n", count);
    
    return 0;
}
