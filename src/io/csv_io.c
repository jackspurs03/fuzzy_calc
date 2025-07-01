#include "csv_io.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

// Вспомогательная функция для сравнения ключей
static int compare_keys(const void* a, const void* b) {
    int key1 = *(const int*)a;
    int key2 = *(const int*)b;
    return (key1 > key2) - (key1 < key2);
}

// Функция для удаления пробелов в строке
static void trim_whitespace(char* str) {
    char* end;
    // Удаляем пробелы в начале
    while (isspace((unsigned char)*str)) str++;
    
    // Удаляем пробелы в конце
    end = str + strlen(str) - 1;
    while (end > str && isspace((unsigned char)*end)) end--;
    
    // Записываем новый конец строки
    *(end + 1) = '\0';
}

// Чтение данных из CSV файла
HashTable* csv_read_set(const char* filename) {
    FILE* file = fopen(filename, "r");
    if (!file) {
        perror("Error opening CSV file");
        return NULL;
    }

    HashTable* table = create_table();
    char buffer[256];
    int line = 0;

    while (fgets(buffer, sizeof(buffer), file)) {
        line++;
        
        // Удаляем символы перевода строки
        buffer[strcspn(buffer, "\r\n")] = '\0';
        
        // Пропускаем пустые строки
        if (strlen(buffer) == 0) continue;
        
        // Ищем разделитель
        char* sep = strchr(buffer, ';');
        if (!sep) {
            fprintf(stderr, "Error: Missing ';' separator at line %d\n", line);
            continue;
        }
        
        // Разделяем ключ и значение
        *sep = '\0';
        char* key_str = buffer;
        char* value_str = sep + 1;
        
        // Удаляем пробелы
        trim_whitespace(key_str);
        trim_whitespace(value_str);
        
        // Пропускаем строки без данных
        if (strlen(key_str) == 0 || strlen(value_str) == 0) {
            continue;
        }
        
        // Преобразуем в числа
        int key = atoi(key_str);
        float value = atof(value_str);
        
        // Проверка корректности значения
        if (value < 0.0f || value > 1.0f) {
            fprintf(stderr, "Warning: Value %.2f clamped to [0,1] at line %d\n", value, line);
            value = (value < 0.0f) ? 0.0f : (value > 1.0f) ? 1.0f : value;
        }
        
        upsert(table, key, value);
    }

    if (ferror(file)) {
        perror("Error reading CSV file");
    }

    fclose(file);
    
    if (table->count == 0) {
        fprintf(stderr, "Warning: No valid data found in CSV file\n");
    }
    
    return table;
}

// Запись данных в CSV файл
void csv_write_set(HashTable* table, const char* filename) {
    if (!table || table->count == 0) {
        fprintf(stderr, "Error: Empty set for CSV export\n");
        return;
    }

    FILE* file = fopen(filename, "w");
    if (!file) {
        perror("Error creating CSV file");
        return;
    }

    // Сбор всех ключей
    int* keys = (int*)malloc(table->count * sizeof(int));
    int index = 0;
    
    for (int i = 0; i < table->size; i++) {
        HashNode* node = table->buckets[i];
        while (node) {
            keys[index++] = node->key;
            node = node->next;
        }
    }
    
    // Сортировка ключей
    qsort(keys, table->count, sizeof(int), compare_keys);

    // Запись данных
    for (int i = 0; i < table->count; i++) {
        float value = find(table, keys[i]);
        // Записываем в формате int;float
        fprintf(file, "%d;%g\n", keys[i], value);
    }

    free(keys);
    fclose(file);
}