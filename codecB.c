//
// Created by malaklinux on 4/19/23.
//
#include "stdio.h"
#include <string.h>
#include <ctype.h>

int main(int argc, char *argv[]) {
    if (argc < 3) {
        printf("Usage : encode/decode <codec> <message>\n");
        return 1;
    }
    if (strcmp(argv[0], "encode") != 0 && strcmp(argv[0], "decode") != 0){
        printf("Usage : encode/decode <codec> <message>\n");
        return 1;
    }
    if(strcmp(argv[1], "codecB") != 0){
        printf("Usage : encode/decode <codec> <message>\n");
        return 1;
    }

}