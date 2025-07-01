#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#include "./io/file_io_manager.h"
#include "./io/csv_io.h"
#include "./io/txt_io.h"
#include "./data/hasht.h"
#include "./operations/fuzzy_ops.h"
#include "./operations/norms.h"  // Добавляем заголовочный файл норм

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
        if (strncmp(argv[i], flag, strlen(flag)) == 0) {
            return 1;
        }
    }
    return 0;
}

// Получаем значение флага в формате --flag=value
const char* get_flag_value(int argc, char* argv[], const char* flag_prefix) {
    for (int i = 1; i < argc; i++) {
        if (strncmp(argv[i], flag_prefix, strlen(flag_prefix)) == 0) {
            return argv[i] + strlen(flag_prefix);
        }
    }
    return NULL;
}

// Получаем указатель на функцию нормы
float (*get_norm_func(const char* norm_name, int is_tnorm))(float, float) {
    if (norm_name == NULL) {
        // Возвращаем стандартную норму
        return is_tnorm ? t_min : s_max;
    }
    
    if (strcmp(norm_name, "t_min") == 0) return t_min;
    if (strcmp(norm_name, "t_prod") == 0) return t_prod;
    if (strcmp(norm_name, "t_lukas") == 0) return t_lukas;
    
    if (strcmp(norm_name, "s_max") == 0) return s_max;
    if (strcmp(norm_name, "s_asum") == 0) return s_asum;
    if (strcmp(norm_name, "s_lukas") == 0) return s_lukas;
    
    fprintf(stderr, "Warning: Unknown norm '%s'. Using default.\n", norm_name);
    return is_tnorm ? t_min : s_max;
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

    // Получаем параметры норм
    const char* tnorm_name = get_flag_value(argc, argv, "--tnorm=");
    float (*tnorm_func)(float, float) = get_norm_func(tnorm_name, 1);
    
    HashTable* result = and(a, b, tnorm_func);  // Используем новую функцию с нормой

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

    // Получаем параметры норм
    const char* snorm_name = get_flag_value(argc, argv, "--snorm=");
    float (*snorm_func)(float, float) = get_norm_func(snorm_name, 0);
    
    HashTable* result = or(a, b, snorm_func);  // Используем новую функцию с нормой

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

    HashTable* result = not(a);

    const char* out = get_arg_value(argc, argv, "-o");
    if (!out) out = "result.txt";

    write_set_to_file(result, out);

    free_table(a);
    free_table(result);
    return 0;
}

int handle_impl(int argc, char* argv[]) {
    printf("Starting IMPL operation\n");
    printf("File1: %s, File2: %s\n", argv[2], argv[3]);
    
    HashTable* a = read_set_from_file(argv[2]);
    HashTable* b = read_set_from_file(argv[3]);
    
    if (!a) printf("Failed to load %s\n", argv[2]);
    if (!b) printf("Failed to load %s\n", argv[3]);
    
    HashTable* result = implication(a, b);
    
    if (!result) printf("IMPL operation returned NULL\n");

    const char* out = get_arg_value(argc, argv, "-o");
    if (!out) out = "result.txt";

    write_set_to_file(result, out);

    free_table(a);
    free_table(b);
    free_table(result);
    return 0;
}

int handle_cart(int argc, char* argv[]) {
    if (argc < 4) {
        fprintf(stderr, "Недостаточно аргументов для CART\n");
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

    HashTable* result = cartesian_product(a, b);

    const char* out = get_arg_value(argc, argv, "-o");
    if (!out) out = "result.txt";

    write_set_to_file(result, out);

    free_table(a);
    free_table(b);
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
    { "IMPL", handle_impl },    // Добавляем новые операции
    { "CART", handle_cart },
    { NULL, NULL }
};

// ========== Основной main ==========
int main(int argc, char* argv[]) {
    system("chcp 65001>nul");

    if (argc < 2) {
        fprintf(stderr, "Ошибка: не указана команда\n");
        fprintf(stderr, "Доступные команды: AND, OR, NOT, IMPL, CART\n");
        return 1;
    }

    // Приводим команду к верхнему регистру для удобства
    for (char *p = argv[1]; *p; p++) *p = toupper(*p);
    
    for (int i = 0; commands[i].name != NULL; i++) {
        if (strcmp(argv[1], commands[i].name) == 0) {
            return commands[i].handler(argc, argv);
        }
    }

    fprintf(stderr, "Ошибка: неизвестная команда %s\n", argv[1]);
    fprintf(stderr, "Доступные команды: AND, OR, NOT, IMPL, CART\n");
    return 1;
}

