#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main(int argc, char *argv[]) {
    system ("chcp 65001>nul");

    //потом вывод строки поменяем на сами операции 
    if (strcmp(argv[1], "and") == 0 || strcmp(argv[1], "AND") == 0) {
        if (argc > 4) {
            printf("Слишком много файлов");
        }
        else {
            for(int i=2; i<argc; i++) {
                FILE *file;
                file = fopen(argv[i], "r");
                if (file == NULL) {
                    printf("Файл %s не найден\n", argv[i]);
                }

                fclose(file);
            }
        }
    }
    else if (strcmp(argv[1], "or") == 0 || strcmp(argv[1], "OR") == 0) {
        if (argc > 4) {
            printf("Слишком много файлов");
        }
        else {
            for(int i=2; i<argc; i++) {
                FILE *file;
                file = fopen(argv[i], "r");
                if (file == NULL) {
                    printf("Файл %s не найден\n", argv[i]);
                }

                fclose(file);
            }
        }
    }
    else if (strcmp(argv[1], "not") == 0 || strcmp(argv[1], "NOT") == 0) {
        if (argc > 3) 
        {
            printf("Слишком большое кол-во файлов\n");
        }
        else {
            FILE *file;
            file = fopen(argv[3], "r");
            if (file == NULL) {
                printf("Файл %s не найден\n", argv[2]);
            }
            
            fclose(file);
        }
    }
    else {
        printf("Введена неверная операция\n");
    }

    return 0;
}