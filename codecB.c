//
// Created by malaklinux on 4/19/23.
//

#include "codecB.h"

char* addThree(char* str){
    while ('\0' != *str) {
        *str = (*str + 3) % 128;
        str++;
    }
    return str;
}

char* substractThree(char* str){
    while ('\0' != *str) {
        *str = (*str - 3) % 128;
        str++;
    }
    return str;
}