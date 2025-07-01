#include "file_io_manager.h"
#include "txt_io.h"
#include "csv_io.h"
#include <string.h>
#include <stdio.h>

// Определение формата файла по расширению
int detect_file_format(const char* filename) {
    // Находим последнюю точку в имени файла
    const char* dot = strrchr(filename, '.');
        if (!dot) return UNKNOWN_FORMAT;
    
    // Проверяем расширение файла
        if (strcmp(dot, ".txt") == 0) {
        return TXT_FORMAT;
            } else if (strcmp(dot, ".csv") == 0) {
                return CSV_FORMAT;
    }
    
    return UNKNOWN_FORMAT;
}

// Загрузка множества из файла
HashTable* read_set_from_file(const char* filename) {
    int format = detect_file_format(filename);
    
    switch (format) {
        case TXT_FORMAT:
            return txt_read_set(filename);
        case CSV_FORMAT:
            return csv_read_set(filename);
        default:
            fprintf(stderr, "Error: Unsupported file format for %s\n", filename);
            return NULL;
    }
}

// Сохранение множества в файл
void write_set_to_file(HashTable* table, const char* filename) {
    int format = detect_file_format(filename);
    
    switch (format) {
        case TXT_FORMAT:
            txt_write_set(table, filename);
            break;
        case CSV_FORMAT:
            csv_write_set(table, filename);
            break;
        default:
            fprintf(stderr, "Error: Unsupported file format for %s\n", filename);
            break;
    }
}