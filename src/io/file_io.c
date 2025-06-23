#include <stdio.h>
#include <stdlib.h>

int main() {
    system("chcp 65001>nul");
    
    char filename[100];
    printf("Введите имя файла для чтения: ");
    scanf("%99s", filename);

    // 1. Открываем файл для чтения
    FILE *file = fopen(filename, "r");
    if (file == NULL) {
        printf("Ошибка: не удалось открыть файл %s\n", filename);
        return 1;
    }

    // 2. Выводим заголовок
    printf("\nСодержимое файла %s:\n", filename);
    printf("\n");
    
    char line[256]; // Буфер для хранения строки

    // 3. Сначала читаем и выводим файл
    while (fgets(line, sizeof(line), file)) {
        printf("%s", line);    // Вывод в консоль
    }

    // 4. Теперь запрашиваем имя выходного файла
    printf("\nВведите имя файла для записи: ");
    char outputFilename[100];
    scanf("%99s", outputFilename);

    // 5. Переоткрываем файл для чтения (или используем rewind)
    rewind(file); // Возвращаем указатель в начало файла

    // 6. Открываем файл для записи
    FILE *outputFile = fopen(outputFilename, "w");
    if (outputFile == NULL) {
        printf("Ошибка: не удалось создать/открыть файл %s\n", outputFilename);
        fclose(file);
        return 1;
    }

    // 7. Снова читаем файл и записываем в новый
    while (fgets(line, sizeof(line), file)) {
        fprintf(outputFile, "%s", line); // Запись в файл
    }

    printf("\nДанные успешно записаны в файл %s\n", outputFilename);

    // 8. Закрываем оба файла
    fclose(file);
    fclose(outputFile);

    return 0;
}