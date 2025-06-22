#include <stdio.h>
#include <stdlib.h>
#include <string.h>
int main(int argc, char *argv[]) {
    system ("chcp 65001>null");
//потом вывод строки поменяем на сами операции 
if (strcmp(argv[1], "and") == 0 || strcmp(argv[1], "AND") == 0) {
        printf("Будет выполняться операция И\n");
    }
    else if (strcmp(argv[1], "or") == 0 || strcmp(argv[1], "OR") == 0) {
        printf("Будет выполняться операция ИЛИ\n");
    }
    else if (strcmp(argv[1], "not") == 0 || strcmp(argv[1], "NOT") == 0) {
        printf("Будет выполняться операция НЕ\n");
    }
    else {
        printf("Введена неверная операция\n");
    }
for(int i=2;i<argc;i++){
  FILE *file;
  file=fopen(argv[i],"r");
  if (file == NULL) {
        printf("Файл %s не найден\n",argv[i]); // Выводим системное сообщение об ошибке
        continue;
    }
  fclose(file);
}
    return 0;
}