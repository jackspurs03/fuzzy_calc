#include "txt_io.h"
#include <stdio.h>
#include <stdlib.h>

// Вспомогательная функция для сравнения ключей
static int compare_keys(const void* a, const void* b) {
    int key1 = *(const int*)a;
    int key2 = *(const int*)b;
    return (key1 > key2) - (key1 < key2);
}

// Чтение данных из TXT файла
HashTable* txt_read_set(const char* filename) {
    FILE* file = fopen(filename, "r");
    if (!file) {
        perror("Error opening TXT file");
        return NULL;
    }

    HashTable* table = create_table();
    int key;
    float value;
    int line = 0;

    while (fscanf(file, "%d %f", &key, &value) == 2) {
        line++;
        
        // Проверка корректности значения
        if (value < 0.0f || value > 1.0f) {
            fprintf(stderr, "Warning: Value %.2f clamped to [0,1] at line %d\n", value, line);
            value = (value < 0.0f) ? 0.0f : (value > 1.0f) ? 1.0f : value;
        }
        
        upsert(table, key, value);
    }

    if (!feof(file)) {
        fprintf(stderr, "Error reading TXT file at line %d\n", line + 1);
    }

    fclose(file);
    return table;
}

// Запись данных в TXT файл
void txt_write_set(HashTable* table, const char* filename) {
    if (!table || table->count == 0) {
        fprintf(stderr, "Error: Empty set for TXT export\n");
        return;
    }

    FILE* file = fopen(filename, "w");
    if (!file) {
        perror("Error creating TXT file");
        return;
    }

    // Сбор и сортировка ключей
    int* keys = (int*)malloc(table->count * sizeof(int));
    int index = 0;
    
    for (int i = 0; i < table->size; i++) {
        HashNode* node = table->buckets[i];
        while (node) {
            keys[index++] = node->key;
            node = node->next;
        }
    }
    
    qsort(keys, table->count, sizeof(int), compare_keys);

    // Запись данных с форматированием
    for (int i = 0; i < table->count; i++) {
        float value = find(table, keys[i]);
        // Удаление лишних нулей
        if (value == (int)value) {
            fprintf(file, "%d %d\n", keys[i], (int)value);
        } else {
            fprintf(file, "%d %.6f\n", keys[i], value);
        }
    }

    free(keys);
    fclose(file);
}