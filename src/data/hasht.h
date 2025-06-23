#ifndef HASH_TABLE_H
#define HASH_TABLE_H

#include <stdlib.h>

// Константы хеш-таблицы (можно настраивать)
#define TABLE_SIZE 1000003  // Простое число для уменьшения коллизий
#define LOAD_FACTOR 0.7

// Структура элемента хеш-таблицы
typedef struct HashNode {
    int key;
    float value;
    struct HashNode* next;
} HashNode;

// Структура хеш-таблицы
typedef struct {
    HashNode** buckets;
    int size;
    int count;
} HashTable;

// Прототипы функций
HashTable* create_table(void);
void upsert(HashTable* table, int key, float value);
float find(HashTable* table, int key);
void free_table(HashTable* table);

#endif // HASH_TABLE_H