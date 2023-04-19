//
// Created by malaklinux on 4/19/23.
//
#include "stdio.h"
#include <string.h>
#include "codecA.h"

int main(int argc, char *argv[]) {
    printf("%s %s %s", argv[0],argv[1],argv[2]);
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

        char *str = "";

        for (int i = 2; i < argc; i++) {

            str++;
        }
        printf("%s\n", *str);
        return 0;
    }
}
