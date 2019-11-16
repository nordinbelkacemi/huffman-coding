#ifndef STACK_H
#define STACK_H

#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include "debugmalloc.h"
#include "debugmalloc-impl.h"

/* Stack as a doubly linked list */
typedef struct StackNode {
    int bit;
    struct StackNode *next, *prev;
} StackNode;

StackNode *init_stack();
bool is_empty(StackNode *top);
StackNode *push(StackNode *top, int bit);
StackNode *pop(StackNode *top);
StackNode *bottom(StackNode *top);
void print_bits(StackNode *stack);

#endif
