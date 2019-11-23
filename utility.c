#include "utility.h"

void print_char(char c) {
    if (c == '\n')
        printf("\\n");
    else if (c == '\t')
        printf("\\t");
    else
        printf("%c", c);
}

/* unsigned char */

int sig_bits(unsigned char c) {
    return (c != 0) ? (int)log2((double)c) + 1 : 0;
}

void printbin_recur(unsigned char c) {
    if (c == 0)
        return;
    else {
        printbin_recur(c / 2);
        printf("%d", c % 2 == 0 ? 0 : 1);
    }
}

void printbin(unsigned char c) {
    int sb = sig_bits(c);
    for (int i = 8 * sizeof(unsigned char); i > sb; i--)
        printf("0");
    printbin_recur(c);
    printf("\n");
}

/* int */

int sig_bits_int(int n) {
    return (n != 0) ? (int)log2((double)n) + 1 : 0;
}

void printbin_int_recur(int n) {
    if (n == 0)
        return;
    else {
        printbin_int_recur(n / 2);
        printf("%d", n % 2 == 0 ? 0 : 1);
    }
}

void printbin_int(int n) {
    int sb = sig_bits_int(n);
    for (int i = 8 * sizeof(int); i > sb; i--)
        printf("0");
    printbin_int_recur(n);
    printf("\n");
}

/* huffman */

void printbin_huffcode(int code, size_t length) {
    int sb = sig_bits_int(code);
    for (int i = length; i > sb; i--)
        printf("0");
    printbin_int_recur(code);
    printf("\n");
}
