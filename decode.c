//
// Created by malaklinux on 4/19/23.
//
#include "stdio.h"
#include <string.h>
#include <dlfcn.h>

int main(int argc, char *argv[]) {
    if (argc < 3) {
        printf("Usage : encode/decode <codec> <message>\n");
        return 1;
    }
    if (strcmp(argv[0], "./encode") != 0 && strcmp(argv[0], "./decode") != 0) {
        printf("Usage : encode/decode <codec> <message>\n");
        return 1;
    }
    if (strcmp(argv[1], "codecA") != 0 && strcmp(argv[1], "codecB") != 0) {
        printf("Usage : encode/decode <codec> <message>\n");
        return 1;
    }

    if (strcmp(argv[1], "codecA") == 0) {
        void *handle = dlopen("./codecA.so", RTLD_LAZY);
        if (!handle) {
            printf("Error: couldn't load library %s\n", argv[1]);
            return 1;
        }
        void (*convertToUpperOrLower)(char *) = dlsym(handle, "convertToUpperOrLower");
        if (!convertToUpperOrLower) {
            printf("Error: couldn't find convertToUpperOrLower function in library %s\n", argv[1]);
            dlclose(handle);
            return 1;
        }
        for (int i = 2; i < argc; i++) {
            (*convertToUpperOrLower)(argv[i]);
            if (i == argc - 1)
                printf("%s\n", argv[i]);
            else
                printf("%s ", argv[i]);
        }
        dlclose(handle);
        return 0;
    } else if (strcmp(argv[1], "codecB") == 0) {
        void *handle = dlopen("./codecB.so", RTLD_LAZY);
        if (!handle) {
            printf("Error: couldn't load library %s\n", argv[1]);
            return 1;
        }
        void (*substractThree)(char *) = dlsym(handle, "substractThree");
        if (!substractThree) {
            printf("Error: couldn't find addThree function in library %s\n", argv[1]);
            dlclose(handle);
            return 1;
        }
        for (int i = 2; i < argc; i++) {
            char str[strlen(argv[i]) + 1];
            strcpy(str, argv[i]);
            (*substractThree)(str);
            printf("%s", str);
            char space[] = " ";
            (*substractThree)(space);
            printf("%s", space);
        }
        printf("\n");
        dlclose(handle);
        return 0;
    }
}