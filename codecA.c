//
// Created by malaklinux on 4/19/23.
//
#include <ctype.h>
#include "codecA.h"

char *convertToUpperOrLower(char *str) {
    while ('\0' != *str) {
        if (*str >= 'a' && *str <= 'z') {
            *str = toupper(*str);
        } else if (*str >= 'A' && *str <= 'Z') {
            *str = tolower(*str);
        }
        str++;
    }
    return str;
}






