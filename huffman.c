#include "huffman.h"
#include "utility.h"

int main() {
    Queue *q = sample("bible.txt");
    print_queue(q);

    while (q->size != 1)
        huffman_reduce(q);

    print_queue(q);
    printf("tree height = %d\n", height(q->array[0]));
    
    free_mem(q);
    return 0;
}