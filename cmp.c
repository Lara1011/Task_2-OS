#include "stdio.h"
#include <string.h>
#include <ctype.h>

int compare(FILE *f1, FILE *f2, int ignore_case){
    char ch1 = getc(f1);
    char ch2 = getc(f2);
    if(ch1 == EOF && ch2 == EOF)
        return 0;
    while(ch1 != EOF && ch2 != EOF){
        if(ignore_case == 1) {
            if (tolower(ch1) != tolower(ch2)) {
                return 1;
            }
        } else{
            if(ch1 != ch2)
                return 1;
        }
        ch1 = getc(f1);
        ch2 = getc(f2);
    }
    if(ch1 != ch2)
        return 1;
    return 0;
}

int main(int argc, char *argv[]){
    if(argc < 3 || argc > 5){
        printf("Usage: cmp <file1> <file2> [-v] [-i]\n");
        return 1;
    }
    int verbose = 0;
    int ignore_case = 0;
    if(argc > 3){
        if(strcmp(argv[3], "-v") == 0){
            verbose = 1;
        }else if(strcmp(argv[3], "-i") == 0){
            ignore_case = 1;
        }else{
            printf("Usage: cmp <file1> <file2> [-v] [-i]\n");
            return 1;
        }
    }
    if(argc == 5){
        if(strcmp(argv[4], "-v") == 0){
            verbose = 1;
        }else if(strcmp(argv[4], "-i") == 0){
            ignore_case = 1;
        }else{
            printf("Usage: cmp <file1> <file2> [-v] [-i]\n");
            return 1;
        }
    }
    FILE *f1 = fopen(argv[1], "r");
    FILE *f2 = fopen(argv[2], "r");
    if(f1 == NULL){
        printf("Cannot open '%s'.\n", argv[1]);
        return 1;
    }
    if(f2 == NULL){
        printf("Cannot open '%s'.\n", argv[2]);
        return 1;
    }
    int result = compare(f1, f2, ignore_case);
    fclose(f1);
    fclose(f2);
    if(verbose == 1){
        if(result == 0)
            printf("equal\n");
        else
            printf("distinct\n");
    }
    return result;
}