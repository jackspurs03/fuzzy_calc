#ifndef CSV_IO_H
#define CSV_IO_H

#include "../data/hasht.h"

// Функции для работы с CSV форматом
HashTable* csv_read_set(const char* filename);
void csv_write_set(HashTable* table, const char* filename);

#endif // CSV_IO_H