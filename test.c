#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "huffman.h"

HuffCode *binsearch_huffcode(HuffCode *table, size_t n, char c) {
    int l = 0;
    int r = n - 1;
    while (l <= r) {
        int m = (l + r) / 2;
        if (table[m].character < c)
            l = m + 1;
        else if (table[m].character > c)
            r = m - 1;
        else
            return &table[m];
    }
    return NULL;
}

/* appends a code to the buffer */
void add_to_buffer(unsigned long *buffer, size_t *curr_buffsize, HuffCode code) {
    code.code = code.code << (8 * sizeof(unsigned long) - code.length - *curr_buffsize);
    *buffer = *buffer | code.code;
    *curr_buffsize += code.length;
}

int main() {
    HuffCode h1 = {67784, 17, 't'};
    HuffCode h2 = {17, 5, 't'};
    HuffCode h3 = {7 << 1, 4, 'y'};
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
    unsigned long buffer = 0;
    size_t curr_buffsize = 0;

    printf("code:\t");
    printbin_ul(h1.code);
    add_to_buffer(&buffer, &curr_buffsize, h1);
    printf("\t");
    printbin_ul(buffer);

    printf("code:\t");
    printbin_ul(h2.code);
    add_to_buffer(&buffer, &curr_buffsize, h2);
    printf("\t");
    printbin_ul(buffer);

    printf("code:\t");
    printbin_ul(h3.code);
    add_to_buffer(&buffer, &curr_buffsize, h3);
    printf("\t");
    printbin_ul(buffer);
    return 0;
}
