#include "huffman.h"

int main() {
    HuffNode *huffman_tree = build_huffman_tree("bible.txt");
    show_huffman_table(huffman_tree);
    free_tree(huffman_tree);
    return 0;
}