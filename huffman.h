#ifndef HUFFMAN_H
#define HUFFMAN_H

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include "debugmalloc.h"
#include "debugmalloc-impl.h"
#include "charsampling.h"
#include "stack.h"
#include "utility.h"

/* Node of a Huffman tree */
typedef struct HuffNode {
    char c;
    int freq;
    struct HuffNode *left, *right;
} HuffNode;

/* Element of Queue used to build Huffman Tree */
typedef struct Queue {
    int initsize, size;
    HuffNode **array;
} Queue;

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
HuffNode *build_huffman_tree(char *filename);
void free_tree(HuffNode *node);
void print_encodings(HuffNode *node, StackNode *stack);
void show_huffman_table(HuffNode *node);

#endif
