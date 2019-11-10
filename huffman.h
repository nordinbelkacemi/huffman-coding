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

/* checks if a given node is a leaf node */
bool is_leaf(HuffNode *node) {
    return node->left == NULL && node->right == NULL;
}

/* utility function for debugging purposes */
void print_node(HuffNode *node) {
    if (is_leaf(node)) {
        print_char(node->c);
        printf("/%d\n", node->freq);
    }
    else {
        printf("%s/%d\n", "internal node", node->freq);
    }
}

/* utility function for debugging purposes */
void print_queue(Queue *q) {
    printf("\n");
    for (int i = 0; i < q->size; i++) {
        if (is_leaf(q->array[i])) {
            print_char(q->array[i]->c);
            printf("/%d\n", q->array[i]->freq);
        }
        else {
            printf("%s/%d\n", "internal node", q->array[i]->freq);
        }
    }
    printf("\n");
}


/* allocates memory for the queue and sets its size to 0 */
Queue *create_queue(int initsize) {
    Queue *q = (Queue *)malloc(sizeof(Queue));

    q->size = 0;
    q->initsize = initsize;
    q->array = (HuffNode **)malloc(initsize * sizeof(HuffNode *));

    return q;
}

/* shifts all nodes in the queue to the left by two starting with index 2 */
void shift(Queue *q) {
    for (int i = 2; i < q->size; i++)
        q->array[i - 2] = q->array[i];
}

/* removes first two elements from the queue */
void remove_first_two(Queue *q) {
    shift(q);
    q->size -= 2;
}

/* extracts the two nodes with the smallest frequency (minimum nodes) and makes a new parent
   node with a frequency equal to the sum of its two children (the minimum nodes) */
HuffNode *new_internal_node(Queue *q) {
    HuffNode *parent = (HuffNode *)malloc(sizeof(HuffNode));

    /* queue is always sorted, so the minimum nodes will be the first two */
    HuffNode *left = q->array[0];
    HuffNode *right = q->array[1];
    remove_first_two(q);

    parent->freq = left->freq + right->freq;
    parent->left = left;
    parent->right = right;

    return parent;
}


/* inserts a newly made Huffman node into the queue, keeping it sorted. */
void insert(Queue *q, HuffNode *node) {
    if (q->size == q->initsize) {
        fprintf(stderr, "Error: Can't add node because the queue is full.");
        exit(1);
    }

    int insertion_idx = q->size;
    for (int i = 0; i < q->size; i++) {
        if (node->freq <= q->array[i]->freq) {
            insertion_idx = i;
            break;
        }
    }

    for (int i = q->size - 1; i >= insertion_idx; i--)
        q->array[i + 1] = q->array[i];

    q->array[insertion_idx] = node;
    q->size += 1;
}

/* swaps two Huffman nodes */
void swap(HuffNode **a, HuffNode **b) {
    HuffNode *tmp = *a;
    *a = *b;
    *b = tmp;
}

/* sorts the queue by node frequency */
void quicksort(Queue *q, int min, int max) {
    HuffNode *pivot = q->array[(min + max) / 2];
    int i = min, j = max;
    while (i <= j) {
        while (q->array[i]->freq < pivot->freq)
            i += 1;
        while (q->array[j]->freq > pivot->freq)
            j -= 1;
        if (i <= j) {
            swap(&q->array[i], &q->array[j]);
            i += 1;
            j -= 1;
        }
    }

    if (min < j)
        quicksort(q, min, j);
    if (i < max)
        quicksort(q, i, max);
}

/* performs Huffman's compression algorithm (see documentation) */
void huffman_reduce(Queue *q) {
    HuffNode *internal_node = new_internal_node(q);
    insert(q, internal_node);
}


/* fills the queue with nodes, each of which contain a character and its frequency in a given text file */
Queue *sample(char *filename) {
    int char_freq[128] = {0};
    feed(char_freq, filename);
    int size = character_set_size(char_freq);

    Queue *q = create_queue(character_set_size(char_freq));

    for (int i = 0; i < 128; i++) {
        if (char_freq[i] != 0) {
            HuffNode *node = (HuffNode *)malloc(sizeof(HuffNode));
            node->c = i;
            node->freq = char_freq[i];
            node->left = NULL;
            node->right = NULL;
            q->array[q->size] = node;
            q->size += 1;
        }
    }
    quicksort(q, 0, q->size - 1);
    return q;
}

/* frees the rest of the memory */
void free_queue(Queue *q) {
    free(q->array);
    free(q);
}

HuffNode *build_huffman_tree(char *filename) {
    Queue *q = sample(filename);

    while (q->size != 1)
        huffman_reduce(q);

    /* the first and only element left in the queue is the root of the huffman tree */
    HuffNode *tree = q->array[0];

    /* free memory */
    free_queue(q);

    return tree;
}

/* frees the memory allocated for each node in the huffman tree */
void free_tree(HuffNode *node) {
    if (node == NULL)
        return;
    free_tree(node->left);
    free_tree(node->right);
    free(node);
}

/* prints the encoding of each character node */
void print_encodings(HuffNode *node, StackNode *stack) {
    if (is_leaf(node)) {
        print_char(node->c);
        printf(":\t");
        print_bits(stack);
        return;
    }
    stack = push(stack, 0);
    print_encodings(node->left, stack);
    stack = pop(stack);

    stack = push(stack, 1);
    print_encodings(node->right, stack);
    stack = pop(stack);
}

/* creates a stack then prints all encodings using the print_encodings function */
void show_huffman_table(HuffNode *node) {
    StackNode *stack = init_stack();
    print_encodings(node, stack);
    free(stack);
}

#endif