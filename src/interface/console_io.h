#ifndef COMMAND_LINE_H
#define COMMAND_LINE_H

typedef struct {
    char* operation;          // Название операции (AND, OR, NOT и т.д.)
    char* input_files[2];     // Входные файлы (0-2 файла)
    int num_input_files;      // Количество входных файлов
    char* output_file;        // Выходной файл
    char* tnorm;              // Выбранная t-норма
    char* snorm;              // Выбранная s-норма
} CommandLineArgs;

// Разбор аргументов командной строки
CommandLineArgs parse_arguments(int argc, char *argv[]);

// Вывод справки по использованию
void print_usage();

#endif