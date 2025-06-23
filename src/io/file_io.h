#ifndef FILE_IO_H
#define FILE_IO_H

#include <stdio.h>
#include <math.h>
#include "../data/hasht.h"

HashTable* read_set_from_file(const char* filename);
int compare_keys(const void* a, const void* b);
void write_set_to_file(HashTable* table, const char* filename);

#endif