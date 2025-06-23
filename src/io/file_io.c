#include "file_io.h"

// Чтение множества из файла
HashTable* read_set_from_file(const char* filename) {
    FILE* file = fopen(filename, "r");
    if (!file) {
        perror("Error opening file");
        return NULL;
    }

    HashTable* table = create_table();
    int key;
    float value;
    int line = 0;

    while (fscanf(file, "%d %f", &key, &value) == 2) {
        line++;
        if (value < 0 || value > 1) {
            fprintf(stderr, "Warning: Value %.2f clamped to [0,1] at line %d\n", value, line);
            value = fmaxf(0.0f, fminf(1.0f, value));
        }
        upsert(table, key, value);
    }

    if (!feof(file)) {
        fprintf(stderr, "Error reading file at line %d\n", line + 1);
    }

    fclose(file);
    return table;
}

// Функция сравнения для qsort (должна быть отдельной)
int compare_keys(const void* a, const void* b) {
    int key1 = *(const int*)a;
    int key2 = *(const int*)b;
    return (key1 > key2) - (key1 < key2);
}

// Обновленная функция записи в файл
void write_set_to_file(HashTable* table, const char* filename) {
    FILE* file = fopen(filename, "w");
    if (!file) {
        perror("Error creating output file");
        return;
    }

    // Временный массив для сортировки
    int* keys = (int*)malloc(table->count * sizeof(int));
    int index = 0;

    // Сбор всех ключей
    for (int i = 0; i < TABLE_SIZE; i++) {
        HashNode* node = table->buckets[i];
        while (node != NULL) {
            keys[index++] = node->key;
            node = node->next;
        }
    }

    // Сортировка ключей с использованием нашей функции сравнения
    qsort(keys, table->count, sizeof(int), compare_keys);

    // Запись отсортированных данных
    for (int i = 0; i < table->count; i++) {
        float value = find(table, keys[i]);
        fprintf(file, "%d %.6f\n", keys[i], value);
    }

    free(keys);
    fclose(file);
}