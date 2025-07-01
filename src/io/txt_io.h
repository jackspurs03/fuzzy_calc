#ifndef TXT_IO_H
#define TXT_IO_H

#include "../data/hasht.h"

// Функции для работы с TXT форматом
HashTable* txt_read_set(const char* filename);
void txt_write_set(HashTable* table, const char* filename);

#endif // TXT_IO_H