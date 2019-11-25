#ifndef HUFFMAN_H
#define HUFFMAN_H

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
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
    unsigned long code;
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
void make_helper(Queue *q, char *filename);
Queue *get_queue(char *filename);
HuffNode *build_huffman_tree(char *filename, size_t *table_size, char *mode);
void free_tree(HuffNode *node);
void fill_huffman_table(HuffCode *table, size_t *size, HuffNode *node, unsigned long code, size_t code_length);
void print_huffcode(HuffCode code);
void print_huffman_table(HuffCode *table, size_t size);
HuffCode *build_huffman_table(HuffNode *huffman_tree, size_t table_size);
void swap_codes(HuffCode *a, HuffCode *b);
void quickort_table(HuffCode *table, int min, int max);
HuffCode *binsearch(HuffCode *table, size_t n, unsigned char c);
void add_to_buffer(unsigned long *buffer, size_t *curr_buffsize, HuffCode code);
void write_buffer(FILE *f, unsigned long *buffer, size_t *curr_buffsize);
void write_binary(char *filename, HuffCode *huffman_table, size_t table_size);
void read_compressed(char *filename);
void compress(char *filename);
void write_text(char *filename, HuffNode *huffman_tree);
void decompress(char *filename);

#endif
