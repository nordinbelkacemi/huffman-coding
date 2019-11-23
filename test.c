#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "huffman.h"

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

int main() {
    // HuffCode h1 = {67784, 20, 't'};
    // HuffCode h2 = {67782, 20, 't'};
    // printbin_int(h1.code);

    // FILE *fout = fopen("binary.dat", "wb");
    // fwrite(&h1.code, sizeof(int), 1, fout);
    // fwrite(&h2.code, sizeof(int), 1, fout);
    // fclose(fout);

    // FILE *fin = fopen("binary.dat", "rb");
    // char buffer[8];
    // fread(buffer, sizeof(char), 8, fin);

    // for (int i = 0; i < 8; i++)
    //     printbin(buffer[i]);

    // fclose(fin);
    int a = 0;
    a = a << 1;
    a = a << 1;
    a = a << 1;
    printf("%d\n", a);
    return 0;
}
