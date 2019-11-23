#ifndef HUFFMAN_H
#define HUFFMAN_H

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include "debugmalloc.h"
#include "debugmalloc-impl.h"
#include "charsampling.h"
#include "utility.h"

/* Node of a Huffman tree */
typedef struct HuffNode {
    char c;
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
    int code;
    size_t length;
    unsigned char character;
} HuffCode;

bool is_leaf(HuffNode *node);
void print_node(HuffNode *node);
void print_queue(Queue *q);
Queue *create_queue(int initsize);
void shift(Queue *q);
void remove_first_two(Queue *q);
HuffNode *new_internal_node(Queue *q);
void insert(Queue *q, HuffNode *node);
void swap(HuffNode **a, HuffNode **b);
void quicksort(Queue *q, int min, int max);
void huffman_reduce(Queue *q);
Queue *sample(char *filename);
void free_queue(Queue *q);
HuffNode *build_huffman_tree(char *filename, size_t *table_size);
void free_tree(HuffNode *node);
int power(int n, int exponent);
void fill_huffman_table(HuffCode *table, size_t *size, HuffNode *node, int code, size_t code_length);
void print_huffman_table(HuffCode *table, size_t size);
HuffCode *build_huffman_table(HuffNode *huffman_tree, size_t table_size);
void swap_codes(HuffCode *a, HuffCode *b);
void quickort_table(HuffCode *table, int min, int max);
void compress(char *filename);

#endif
