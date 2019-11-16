#include "stack.h"

/* creates an empty stack (an empty stack is one containing a single node whose bit is -1) */
StackNode *init_stack() {
    StackNode *top = (StackNode *)malloc(sizeof(StackNode));
    top->bit = -1;
    top->prev = NULL;
    top->next = NULL;

    return top;
}

/* checks if the stack is empty */
bool is_empty(StackNode *top) {
    return top->bit == -1;
}

/* pushes a bit onto the stack */
StackNode *push(StackNode *top, int bit) {
    if (is_empty(top)) {
        top->bit = bit;
        return top;
    }
    else {
        StackNode *new_node = (StackNode *)malloc(sizeof(StackNode));
        new_node->bit = bit;
        new_node->next = NULL;
        new_node->prev = top;
        top->next = new_node;
        return new_node;
    }
}

/* pops a bit from the top of the stack */
StackNode *pop(StackNode *top) {
    if (top->prev == NULL) {
        top->bit = -1;
        return top;
    }
    else {
        StackNode *new_top = top->prev;
        free(top);
        new_top->next = NULL;
        return new_top;
    }
}

/* returns the bottom of the stack (ie the first element pushed in) */
StackNode *bottom(StackNode *top) {
    while (top->prev != NULL)
        top = top->prev;

    return top;
}

/* print the bits on the stack */
void print_bits(StackNode *stack) {
    StackNode *node = bottom(stack);
    while (node != NULL) {
        printf("%d", node->bit);
        node = node->next;
    }
    printf("\n");
}
