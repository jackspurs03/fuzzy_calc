#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#include "./data/hasht.h"
#include "./io/file_io.h"
#include "./operations/fuzzy_ops.h"

// ========== Утилиты для аргументов ==========
const char* get_arg_value(int argc, char* argv[], const char* flag) {
    for (int i = 1; i < argc - 1; i++) {
        if (strcmp(argv[i], flag) == 0) {
            return argv[i + 1];
        }
    }
    return NULL;
}

int has_flag(int argc, char* argv[], const char* flag) {
    for (int i = 1; i < argc; i++) {
        if (strcmp(argv[i], flag) == 0) return 1;
    }
    return 0;
}

// ========== Обработчики операций ==========
int handle_and(int argc, char* argv[]) {
    if (argc < 4) {
        fprintf(stderr, "Недостаточно аргументов для AND\n");
        return 1;
    }

    HashTable* a = read_set_from_file(argv[2]);
    HashTable* b = read_set_from_file(argv[3]);
    if (!a || !b) {
        fprintf(stderr, "Ошибка чтения входных файлов\n");
        if (a) free_table(a);
        if (b) free_table(b);
        return 1;
    }

    HashTable* result = AND(a, b);

    const char* out = get_arg_value(argc, argv, "-o");
    if (!out) out = "result.txt";

    write_set_to_file(result, out);

    free_table(a);
    free_table(b);
    free_table(result);
    return 0;
}

int handle_or(int argc, char* argv[]) {
    if (argc < 4) {
        fprintf(stderr, "Недостаточно аргументов для OR\n");
        return 1;
    }

    HashTable* a = read_set_from_file(argv[2]);
    HashTable* b = read_set_from_file(argv[3]);
    if (!a || !b) {
        fprintf(stderr, "Ошибка чтения входных файлов\n");
        if (a) free_table(a);
        if (b) free_table(b);
        return 1;
    }

    HashTable* result = OR(a, b);

    const char* out = get_arg_value(argc, argv, "-o");
    if (!out) out = "result.txt";

    write_set_to_file(result, out);

    free_table(a);
    free_table(b);
    free_table(result);
    return 0;
}

int handle_not(int argc, char* argv[]) {
    if (argc < 3) {
        fprintf(stderr, "Недостаточно аргументов для NOT\n");
        return 1;
    }

    HashTable* a = read_set_from_file(argv[2]);
    if (!a) {
        fprintf(stderr, "Ошибка чтения входного файла\n");
        return 1;
    }

    HashTable* result = NOT(a);

    const char* out = get_arg_value(argc, argv, "-o");
    if (!out) out = "result.txt";

    write_set_to_file(result, out);

    free_table(a);
    free_table(result);
    return 0;
}

// ========== Таблица команд ==========
typedef struct {
    const char* name;
    int (*handler)(int argc, char* argv[]);
} Command;

Command commands[] = {
    { "AND", handle_and },
    { "OR", handle_or },
    { "NOT", handle_not },
    { NULL, NULL }
};

// ========== Основной main ==========
int main(int argc, char* argv[]) {
    system("chcp 65001>nul");

    if (argc < 2) {
        fprintf(stderr, "Ошибка: не указана команда\n");
        return 1;
    }

    for (int i = 0; commands[i].name != NULL; i++) {
        if (strcasecmp(argv[1], commands[i].name) == 0) {
            return commands[i].handler(argc, argv);
        }
    }

    fprintf(stderr, "Ошибка: неизвестная команда %s\n", argv[1]);
    return 1;
}
