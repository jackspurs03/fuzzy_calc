#include "command_line.h"
#include <string.h>
#include <stdio.h>
#include <stdlib.h>

CommandLineArgs parse_arguments(int argc, char *argv[]) {
    CommandLineArgs args = {0};
    args.output_file = "result.txt"; // Значение по умолчанию
    args.tnorm = "t_min";            // Значение по умолчанию
    args.snorm = "s_max";            // Значение по умолчанию
    
    // Минимальное количество аргументов: название программы + операция
    if (argc < 2) {
        print_usage();
        exit(1);
    }
    
    // Первый аргумент - операция
    args.operation = argv[1];
    
    // Обработка остальных аргументов
    for (int i = 2; i < argc; i++) {
        if (strcmp(argv[i], "-o") == 0) {
            // Обработка флага выходного файла
            if (i + 1 < argc) {
                args.output_file = argv[i + 1];
                i++; // Пропускаем следующий аргумент (имя файла)
            } else {
                fprintf(stderr, "Error: Missing filename for -o option\n");
                exit(1);
            }
        } else if (strncmp(argv[i], "--tnorm=", 8) == 0) {
            // Обработка t-нормы
            args.tnorm = argv[i] + 8;
        } else if (strncmp(argv[i], "--snorm=", 8) == 0) {
            // Обработка s-нормы
            args.snorm = argv[i] + 8;
        } else {
            // Обработка входных файлов
            if (args.num_input_files < 2) {
                args.input_files[args.num_input_files++] = argv[i];
            } else {
                fprintf(stderr, "Error: Too many input files\n");
                print_usage();
                exit(1);
            }
        }
    }
    
    // Проверка корректности количества файлов
    if (strcmp(args.operation, "NOT") == 0) {
        if (args.num_input_files != 1) {
            fprintf(stderr, "Error: NOT operation requires exactly 1 input file\n");
            exit(1);
        }
    } else {
        if (args.num_input_files != 2) {
            fprintf(stderr, "Error: %s operation requires exactly 2 input files\n", args.operation);
            exit(1);
        }
    }
    
    return args;
}

void print_usage() {
    printf("Usage: calc.exe [operation] [file1] [file2] [options]\n");
    printf("Operations:\n");
    printf("  AND    - Intersection of fuzzy sets\n");
    printf("  OR     - Union of fuzzy sets\n");
    printf("  NOT    - Complement of a fuzzy set\n");
    printf("  IMPL   - Implication\n");
    printf("  CART   - Cartesian product\n\n");
    printf("Options:\n");
    printf("  -o [filename]     - Output file (default: result.txt)\n");
    printf("  --tnorm=[norm]    - t-norm for AND (t_min, t_prod, t_lukas)\n");
    printf("  --snorm=[norm]    - s-norm for OR (s_max, s_asum, s_lukas)\n\n");
    printf("Examples:\n");
    printf("  calc.exe AND setA.txt setB.txt -o result.txt\n");
    printf("  calc.exe OR setA.txt setB.txt --tnorm=t_prod --snorm=s_asum\n");
    printf("  calc.exe NOT setA.txt -o complement.txt\n");
}