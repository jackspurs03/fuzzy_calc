#include "../data/hasht.h"
#include "fuzzy_ops.h"
#include <math.h>
#include <stdlib.h>

// Вспомогательная функция для проверки ключей
static int key_exists(const HashTable* table, int key) {
    return find((HashTable*)table, key) != 0.0f;
}

// Отрицание одного множества
HashTable* not(const HashTable* set) {
    HashTable* result = create_table();

    // Обрабатываем первое множество
    for (int i = 0; i < TABLE_SIZE; i++) {
        HashNode* node = set->buckets[i];
        while (node != NULL) {
            float neg_val = 1 - node->value;
            upsert(result, node->key, neg_val);
            node = node->next;
        }
    }

    return result;
}

// Пересечение множеств с поддержкой t-нормы
HashTable* and(const HashTable* a, const HashTable* b, float (*tnorm)(float, float)) {
    if (!a || !b || !tnorm) return NULL;
    
    HashTable* result = create_table();
    if (!result) return NULL;

    // Обрабатываем ключи из первого множества
    for (int i = 0; i < TABLE_SIZE; i++) {
        HashNode* node = a->buckets[i];
        while (node != NULL) {
            int key = node->key;
            float a_val = node->value;
            float b_val = key_exists(b, key) ? find((HashTable*)b, key) : 0.0f;
            float new_val = tnorm(a_val, b_val);
            
            if (new_val != 0.0f) {
                upsert(result, key, new_val);
            }
            node = node->next;
        }
    }
    return result;
}

// Объединение множеств с поддержкой s-нормы
HashTable* or(const HashTable* a, const HashTable* b, float (*snorm)(float, float)) {
    if (!a || !b || !snorm) return NULL;
    
    HashTable* result = create_table();
    if (!result) return NULL;

    // Обрабатываем ключи из первого множества
    for (int i = 0; i < TABLE_SIZE; i++) {
        HashNode* node = a->buckets[i];
        while (node != NULL) {
            int key = node->key;
            float a_val = node->value;
            float b_val = key_exists(b, key) ? find((HashTable*)b, key) : 0.0f;
            float new_val = snorm(a_val, b_val);
            
            if (new_val != 0.0f) {
                upsert(result, key, new_val);
            }
            node = node->next;
        }
    }

    // Добавляем уникальные элементы из второго множества
    for (int i = 0; i < TABLE_SIZE; i++) {
        HashNode* node = b->buckets[i];
        while (node != NULL) {
            int key = node->key;
            if (!key_exists(a, key)) {
                float new_val = snorm(0.0f, node->value);
                if (new_val != 0.0f) {
                    upsert(result, key, new_val);
                }
            }
            node = node->next;
        }
    }
    return result;
}

// Импликация (расширенная операция)
HashTable* implication(const HashTable* a, const HashTable* b) {
    if (!a || !b) return NULL;
    
    HashTable* result = create_table();
    if (!result) return NULL;

    // Обрабатываем общие ключи
    for (int i = 0; i < TABLE_SIZE; i++) {
        HashNode* node = a->buckets[i];
        while (node != NULL) {
            int key = node->key;
            float a_val = node->value;
            float b_val = key_exists(b, key) ? find((HashTable*)b, key) : 1.0f;
            float new_val = fminf(1.0f, 1.0f - a_val + b_val);
            upsert(result, key, new_val);
            node = node->next;
        }
    }

    // Обрабатываем уникальные ключи из b
    for (int i = 0; i < TABLE_SIZE; i++) {
        HashNode* node = b->buckets[i];
        while (node != NULL) {
            int key = node->key;
            if (!key_exists(a, key)) {
                upsert(result, key, 1.0f);
            }
            node = node->next;
        }
    }
    return result;
}

// Декартово произведение (расширенная операция)
HashTable* cartesian_product(const HashTable* a, const HashTable* b) {
    if (!a || !b) return NULL;
    
    HashTable* result = create_table();
    if (!result) return NULL;

    for (int i = 0; i < TABLE_SIZE; i++) {
        HashNode* nodeA = a->buckets[i];
        while (nodeA != NULL) {
            for (int j = 0; j < TABLE_SIZE; j++) {
                HashNode* nodeB = b->buckets[j];
                while (nodeB != NULL) {
                    int new_key = (nodeA->key << 16) | (nodeB->key & 0xFFFF);
                    float new_val = fminf(nodeA->value, nodeB->value);
                    upsert(result, new_key, new_val);
                    nodeB = nodeB->next;
                }
            }
            nodeA = nodeA->next;
        }
    }
    return result;
}