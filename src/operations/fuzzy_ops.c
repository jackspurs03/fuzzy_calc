#include "../data/hasht.h"

// Объединение двух множеств
HashTable* AND(HashTable* tableA, HashTable* tableB) {
    HashTable* result = create_table();

    // Обрабатываем первое множество
    for (int i = 0; i < TABLE_SIZE; i++) {
        HashNode* node = tableA->buckets[i];
        while (node != NULL) {
            float b_val = find(tableB, node->key);
            float max_val = (node->value > b_val) ? node->value : b_val;
            upsert(result, node->key, max_val);
            node = node->next;
        }
    }

    // Добавляем уникальные элементы из второго множества
    for (int i = 0; i < TABLE_SIZE; i++) {
        HashNode* node = tableB->buckets[i];
        while (node != NULL) {
            // Если элемент уже обработан в первом цикле, пропускаем
            if (find(result, node->key) == 0.0f) {
                upsert(result, node->key, node->value);
            }
            node = node->next;
        }
    }

    return result;
}

// Пересечение двух множеств
HashTable* OR(HashTable* tableA, HashTable* tableB) {
    HashTable* result = create_table();

    // Обрабатываем первое множество
    for (int i = 0; i < TABLE_SIZE; i++) {
        HashNode* node = tableA->buckets[i];
        while (node != NULL) {
            float b_val = find(tableB, node->key);
            float min_val = (node->value < b_val) ? node->value : b_val;
            upsert(result, node->key, min_val);
            node = node->next;
        }
    }

    // Добавляем уникальные элементы из второго множества
    for (int i = 0; i < TABLE_SIZE; i++) {
        HashNode* node = tableB->buckets[i];
        while (node != NULL) {
            // Если элемент уже обработан в первом цикле, пропускаем
            if (find(result, node->key) == 0.0f) {
                upsert(result, node->key, node->value);
            }
            node = node->next;
        }
    }

    return result;
}