#ifndef FUZZY_OPS_H
#define FUZZY_OPS_H

#include "../data/hasht.h"

// Базовые операции
HashTable* not(const HashTable* set);
HashTable* and(const HashTable* a, const HashTable* b, float (*tnorm)(float, float));
HashTable* or(const HashTable* a, const HashTable* b, float (*snorm)(float, float));

// Расширенные операции
HashTable* implication(const HashTable* a, const HashTable* b);
HashTable* cartesian_product(const HashTable* a, const HashTable* b);
#endif