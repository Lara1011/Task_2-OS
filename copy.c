#include "stdio.h"
#include <string.h>
#include <ctype.h>

void copy(FILE *f1, FILE *output_file){
    char ch1 = fgetc(f1);
    while (ch1 != EOF){
        fputc(ch1, output_file);
        ch1 = fgetc(f1);
    }
}

int main(int argc, char *argv[]){
    if(argc < 3 || argc > 5){
        printf("Usage: copy <file1> <file2> [-v] [-f]\n");
        return 1;
    }
    FILE *f1 = fopen(argv[1], "r");
    FILE *f2 = fopen(argv[2], "r");
    if(f1 == NULL){
        printf("Cannot open '%s'.\n", argv[1]);
        return 1;
    }
    int force = 0, f2_exists = 0, verbose = 0;
    if(f2 != NULL){
        f2_exists = 1;
        fclose(f2);
    }
    else{

    }
    if(argc > 3){
        if(strcmp(argv[3], "-f") == 0)
            force = 1;
        else if(strcmp(argv[3], "-v") == 0)
            verbose = 1;
        else{
            printf("Usage: copy <file1> <file2> [-v] [-f]\n");
            return 1;
        }
    }
    if(argc == 5){
        if(strcmp(argv[4], "-f") == 0)
            force = 1;
        else if(strcmp(argv[4], "-v") == 0)
            verbose = 1;
        else{
            printf("Usage: copy <file1> <file2> [-v] [-f]\n");
            return 1;
        }
    }
    if(force == 0 && f2_exists){
        if(verbose == 1)
            printf("Error: target file exist\n");
        return 1;
    }
    FILE *output_file = fopen(argv[2], "w");
    if(output_file == NULL){
        printf("Error: general failure\n");
        return 1;
    }
    copy(f1, output_file);
    fclose(f1);
    fclose(output_file);
    if(verbose == 1)
        printf("success\n");
    return 0;
}