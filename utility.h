#ifndef UTILITY_H
#define UTILITY_H

#include <stdio.h>

void print_char(char c) {
    if (c == '\n')
        printf("\\n");
    else if (c == '\t')
        printf("\\t");
    else
        printf("%c", c);
}

#endif