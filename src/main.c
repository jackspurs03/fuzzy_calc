#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "./data/hasht.h"
#include "./io/file_io.h"
#include "./operations/fuzzy_ops.h"

int main(int argc, char *argv[]) {
    system ("chcp 65001>nul");

    //потом вывод строки поменяем на сами операции 
    if (strcmp(argv[1], "and") == 0 || strcmp(argv[1], "AND") == 0) {
        if (argc > 4 && strcmp(argv[5], "-o") == 0) {
            printf("Слишком много файлов");
        }
        else {
            HashTable* setA = read_set_from_file(argv[2]);
            HashTable* setB = read_set_from_file(argv[3]);
    
            if (!setA || !setB) {
                fprintf(stderr, "Error reading input files\n");
                if (setA) free_table(setA);
                if (setB) free_table(setB);
                return 1;
            }
        
            printf("Sets loaded: A=%d elements, B=%d elements\n", setA->count, setB->count);

            HashTable* result = AND(setA, setB);
               
            char output_path[256];
                if (argc >= 6 && strcmp(argv[4], "-o") == 0) {
                    // Формируем путь с учётом переданного имени файла
                    snprintf(output_path, sizeof(output_path), "%s", argv[5]);
                } 
                else {
                        // Используем имя по умолчанию
                        strcpy(output_path, "result.txt");
                }

            write_set_to_file(result, output_path);
        
            printf("Intersection completed. Result: %d elements\n", result->count);

            // Освобождение памяти
            free_table(setA);
            free_table(setB);
            free_table(result);

        }
    }
    else if (strcmp(argv[1], "or") == 0 || strcmp(argv[1], "OR") == 0) {
         if (argc > 4 && strcmp(argv[5], "-o") == 0) {
            printf("Слишком много файлов");
        }
        else {
             HashTable* setA = read_set_from_file(argv[2]);
            HashTable* setB = read_set_from_file(argv[3]);
    
            if (!setA || !setB) {
                fprintf(stderr, "Error reading input files\n");
                if (setA) free_table(setA);
                if (setB) free_table(setB);
                return 1;
            }
        
            printf("Sets loaded: A=%d elements, B=%d elements\n", setA->count, setB->count);

            HashTable* result = OR(setA, setB);
           
           char output_path[256];
                if (argc >= 6 && strcmp(argv[4], "-o") == 0) {
                    // Формируем путь с учётом переданного имени файла
                    snprintf(output_path, sizeof(output_path), "%s", argv[5]);
                } 
                else {
                        // Используем имя по умолчанию
                        strcpy(output_path, "result.txt");
                }

                write_set_to_file(result, output_path);
        
            printf("Union completed. Result: %d elements\n", result->count);

            // Освобождение памяти
            free_table(setA);
            free_table(setB);
            free_table(result);
        }
    }
    else if (strcmp(argv[1], "not") == 0 || strcmp(argv[1], "NOT") == 0) {
        if (argc > 3 && strcmp(argv[4], "-o") == 0) 
        {
            printf("Слишком большое кол-во файлов\n");
        }
        else {
             HashTable* setA = read_set_from_file(argv[2]);
    
            if (!setA) {
                fprintf(stderr, "Error reading input files\n");
                if (setA) free_table(setA);
                return 1;
            }
        
            printf("Sets loaded: A=%d elements\n", setA->count);

            HashTable* result = NOT(setA);

             char output_path[256];
                if (argc >= 5 && strcmp(argv[3], "-o") == 0) {
                    // Формируем путь с учётом переданного имени файла
                    snprintf(output_path, sizeof(output_path), "%s", argv[4]);
                } 
                else {
                        // Используем имя по умолчанию
                        strcpy(output_path, "result.txt");
                }
               
                write_set_to_file(result, output_path);
        
            printf("Complement completed. Result: %d elements\n", result->count);

            // Освобождение памяти
            free_table(setA);
            free_table(result);
        }
    }
    else {
        printf("Введена неверная операция\n");
    }

    return 0;
}




