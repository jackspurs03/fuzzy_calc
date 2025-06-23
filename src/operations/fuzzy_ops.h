#ifndef FUZZY_OPS_H
#define FUZZY_OPS_H

#include "../data/hasht.h"

HashTable* AND(HashTable* tableA, HashTable* tableB);
HashTable* OR(HashTable* tableA, HashTable* tableB);
HashTable* NOT(HashTable* table);

#endif