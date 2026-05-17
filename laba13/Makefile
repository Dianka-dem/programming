CC = gcc
#включает все предупреждения и указывает путь к заголовочным файлам
CFLAGS = -Wall -I./include
#Имя исполняемого файла
TARGET = main
#Путь к папке с исходниками
SOURCE_DIR = src/

#Выполняем команду make и сначала создаст библиотеку, потом исполняемый файл
all: libsmarthome.a $(TARGET)

#Создать статическую библиотеку
libsmarthome.a: smarthome.o
	ar rcs libsmarthome.a smarthome.o

#Создать исполняемый файл main
$(TARGET): main.o libsmarthome.a
	gcc -o $(TARGET) main.o -L. -lsmarthome

#Создать объектный файл main.o
main.o: $(SOURCE_DIR)main.c
	gcc -c $(SOURCE_DIR)main.c -o main.o -I./include

#Создать объектный файл smarthome.o
smarthome.o: $(SOURCE_DIR)smarthome.c
	gcc -c $(SOURCE_DIR)smarthome.c -o smarthome.o -I./include

clean:
	rm -rf *.o *.a $(TARGET)

run: $(TARGET)
	./main