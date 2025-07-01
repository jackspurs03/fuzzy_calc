#ifndef FILE_IO_MANAGER_H
#define FILE_IO_MANAGER_H

#include "../data/hasht.h"

// Коды форматов файлов
#define TXT_FORMAT 0
#define CSV_FORMAT 1
#define UNKNOWN_FORMAT -1

// Функции для работы с файлами
int detect_file_format(const char* filename);
HashTable* read_set_from_file(const char* filename);
void write_set_to_file(HashTable* table, const char* filename);

#endif // FILE_IO_MANAGER_H