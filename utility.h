#ifndef UTILITY_H
#define UTILITY_H

#include "huffman.h"

void print_node(HuffNode *node) {
    if (is_leaf(node)) {
        if (node->c == '\n')
            printf("\\n");
        else if (node->c == '\t')
            printf("\\t");
        else
            printf("%c", node->c);
        printf("/%d\n", node->freq);
    }
    else {
        printf("%s/%d\n", "internal node", node->freq);
    }
}

void print_queue(Queue *q) {
    printf("\n");
    for (int i = 0; i < q->size; i++) {
        if (is_leaf(q->array[i])) {
            if (q->array[i]->c == '\n')
                printf("\\n");
            else if (q->array[i]->c == '\t')
                printf("\\t");
            else
                printf("%c", q->array[i]->c);
            printf("/%d\n", q->array[i]->freq);
        }
        else {
            printf("%s/%d\n", "internal node", q->array[i]->freq);
        }
    }
    printf("\n");
}

#endif