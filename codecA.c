//
// Created by malaklinux on 4/19/23.
//
#include <ctype.h>
#include "codecA.h"

char *convertToUpperOrLower(char *str) {
    int i = 0;
    while ('\0' != *str) {
        if (str[i] >= 'a' && str[i] <= 'z') {
            // str[i] = toupper(str[i]);
            str[i] = str[i] - ('a' - 'A');
        } else if (str[i] >= 'A' && str[i] <= 'Z') {
            //str[i] = tolower(str[i]);
            str[i] = str[i] + ('a' - 'A');
        }
        str++;
        i++;
    }
    return str;
}





