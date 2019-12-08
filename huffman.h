#ifndef HUFFMAN_H
#define HUFFMAN_H

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include "debugmalloc.h"
#include "charsampling.h"
#include "utility.h"

/* Node of a Huffman tree */
typedef struct HuffNode {
    unsigned char c;
    int freq;
    struct HuffNode *left, *right;
} HuffNode;

/* Element of Priority Queue used to build Huffman Tree */
typedef struct Queue {
    int initsize, size;
    HuffNode **array;
} Queue;

/* Huffman code used to write into and read from binary files */
typedef struct HuffCode {
    unsigned long code;
    size_t length;
    unsigned char character;
} HuffCode;

/* methods used in compression.c */
bool is_leaf(HuffNode *node);
HuffNode *build_huffman_tree(char *filename, size_t *table_size, char *mode);
void free_tree(HuffNode *node);
HuffCode *build_huffman_table(HuffNode *huffman_tree, size_t table_size);
void quicksort_table(HuffCode *table, int min, int max);
HuffCode *binsearch(HuffCode *table, size_t n, unsigned char c);
void draw_tree(HuffNode *root, HuffCode *table, size_t size);


#endif
