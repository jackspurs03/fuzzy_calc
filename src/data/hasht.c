#include "hasht.h"

// Инициализация хеш-таблицы
HashTable* create_table() {
    HashTable* table = (HashTable*)malloc(sizeof(HashTable));
    table->buckets = (HashNode**)calloc(TABLE_SIZE, sizeof(HashNode*));
    table->size = TABLE_SIZE;
    table->count = 0;
    return table;
}

// Хеш-функция (комбинация умножения и деления)
static unsigned int hash(int key) {  // static - видимость только в этом файле
    unsigned long h = key;
    h = h * 2654435761U; // Золотое сечение умножения
    return h % TABLE_SIZE;
}

// Вставка или обновление элемента
void upsert(HashTable* table, int key, float value) {
    unsigned int index = hash(key);
    HashNode* node = table->buckets[index];
    
    // Поиск существующего ключа
    while (node != NULL) {
        if (node->key == key) {
            // Обновление значения
            node->value = value;
            return;
        }
        node = node->next;
    }
    
    // Создание нового узла
    HashNode* new_node = (HashNode*)malloc(sizeof(HashNode));
    new_node->key = key;
    new_node->value = value;
    new_node->next = table->buckets[index];
    table->buckets[index] = new_node;
    table->count++;
}

// Поиск элемента
float find(HashTable* table, int key) {
    unsigned int index = hash(key);
    HashNode* node = table->buckets[index];
    
    while (node != NULL) {
        if (node->key == key) {
            return node->value;
        }
        node = node->next;
    }
    
    return 0.0f; // Возвращаем 0 для отсутствующих ключей
}

// Освобождение памяти таблицы
void free_table(HashTable* table) {
    for (int i = 0; i < table->size; i++) {
        HashNode* node = table->buckets[i];
        while (node != NULL) {
            HashNode* temp = node;
            node = node->next;
            free(temp);
        }
    }
    free(table->buckets);
    free(table);
}