#include "huffman.h"

/* checks if a given node is a leaf node */
bool is_leaf(HuffNode *node) {
    return node->left == NULL && node->right == NULL;
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
    int char_freq[256] = {0};
    feed(char_freq, filename);
    int size = character_set_size(char_freq);

    Queue *q = create_queue(size);

    for (int i = 0; i < 256; i++) {
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

/* makes a helper file, which contains all the characters and their frequencies */
void make_helper(Queue *q, char *filename) {
    FILE *f = helper_file(filename, "wb");
    fwrite(&(q->size), sizeof(int), 1, f);
    for (int i = 0; i < q->size; i++) {
        fwrite(&(q->array[i]->c), sizeof(char), 1, f);
        fwrite(&(q->array[i]->freq), sizeof(int), 1, f);
    }
    fclose(f);
}

/* reads in data from binary file and makes a queue that will be used to build the huffman tree */
Queue *get_queue(char *filename) {
    FILE *f = helper_file(filename, "rb");
    int size;
    fread(&size, sizeof(int), 1, f);
    Queue *q = create_queue(size);

    char c;
    int freq;
    while (fread(&c, sizeof(char), 1, f) == 1 && fread(&freq, sizeof(int), 1, f) == 1) {
        HuffNode *node = (HuffNode *)malloc(sizeof(HuffNode));
        node->c = c;
        node->freq = freq;
        node->left = NULL;
        node->right = NULL;
        q->array[q->size] = node;
        q->size += 1;
    }
    return q;
}

/* builds a huffman tree and copies the size of the huffman table into the table_size variable */
HuffNode *build_huffman_tree(char *filename, size_t *table_size, char *mode) {
    Queue *q;
    if (strcmp(mode, "compression") == 0) {
        q = sample(filename);
        make_helper(q, filename);
    } else
        q = get_queue(filename);

    *table_size = q->size;
    
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

/* fills huffman table with huffman codes, which are encoded by the path to a leaf node (left = 0, right = 1) */
void fill_huffman_table(HuffCode *table, size_t *size, HuffNode *node, unsigned long code, size_t code_length) {
    if (is_leaf(node)) {
        table[*size].character = node->c;
        table[*size].code = code;
        table[*size].length = code_length;
        *size += 1;
        return;
    }

    code = code << 1;
    code_length += 1;
    fill_huffman_table(table, size, node->left, code, code_length);
    code = code >> 1;
    code_length -= 1;

    code = code << 1;
    code += 1;
    code_length += 1;
    fill_huffman_table(table, size, node->right, code, code_length);
    code = code >> 1;
    code -= 1;
    code_length -= 1;
}

/* allocated memory for the huffman table and fills it */
HuffCode *build_huffman_table(HuffNode *huffman_tree, size_t table_size) {
    HuffCode *huffman_table = (HuffCode *)malloc(table_size * sizeof(HuffCode));
    size_t size = 0;
    fill_huffman_table(huffman_table, &size, huffman_tree, 0, 0);
    return huffman_table;
}

/* swaps two elements of a huffman table */
void swap_codes(HuffCode *a, HuffCode *b) {
    HuffCode tmp = *a;
    *a = *b;
    *b = tmp;
}

/* sorts huffman table alphabetically */
void quicksort_table(HuffCode *table, int min, int max) {
    HuffCode pivot = table[(min + max) / 2];
    int i = min, j = max;
    while (i <= j) {
        while (table[i].character < pivot.character)
            i += 1;
        while (table[j].character > pivot.character)
            j -= 1;
        if (i <= j) {
            swap_codes(&table[i], &table[j]);
            i += 1;
            j -= 1;
        }
    }

    if (min < j)
        quicksort_table(table, min, j);
    if (i < max)
        quicksort_table(table, i, max);
}

/* searches the  huffman code of a given character in a huffman table */
HuffCode *binsearch(HuffCode *table, size_t n, unsigned char c) {
    int l = 0;
    int r = n - 1;
    while (l <= r) {
        int m = (l + r) / 2;
        if (table[m].character < c)
            l = m + 1;
        else if (table[m].character > c)
            r = m - 1;
        else
            return &table[m];
    }
    return NULL;
}

void print_links(FILE *f, HuffNode *root) {
    if (is_leaf(root)){
        return;
    } else {
        if (is_leaf(root->left)) {
            fprintf(f, "node%p->", root);
            fprint_ascii_name(f, root->left->c);
            fprintf(f, " [label=%d];\n\t", 0);
        } else {
            fprintf(f, "node%p->node%p [label=%d]\n\t", root, root->left, 0);
            print_links(f, root->left);
        }

        if (is_leaf(root->right)) {
            fprintf(f, "node%p->", root);
            fprint_ascii_name(f, root->right->c);
            fprintf(f, " [label=%d];\n\t", 1);
        } else {
            fprintf(f, "node%p->node%p [label=%d]\n\t", root, root->right, 1);
            print_links(f, root->right);
        }
    }
}

void init_gv_vars(FILE *f, HuffNode *root, HuffCode *table, size_t size) {
    if (is_leaf(root)) {
        fprintf(f, "\t");
        fprint_ascii_name(f, root->c);
        fprintf(f, "[shape = record, style = filled, fillcolor = lightcyan, label = \"{{'");
        fprint_char(f, root->c);
        fprintf(f, "'|%d}|", root->freq);
        HuffCode *code = binsearch(table, size, root->c);
        fprintbin(f, code->code, code->length);
        fprintf(f, "}\"];\n");
        return;
    }
    fprintf(f, "\tnode%p [label = %d];\n", root, root->freq);
    init_gv_vars(f, root->left, table, size);
    init_gv_vars(f, root->right, table, size);
}

void draw_tree(HuffNode *root, HuffCode *table, size_t size) {
    FILE *f = fopen("htree.gv", "w");
    fprintf(f, "digraph G {\n");
    init_gv_vars(f, root, table, size);
    fprintf(f, "\t");
    
    if (is_leaf(root))
        fprint_ascii_name(f, root->c);
    else
        print_links(f, root);
    fprintf(f, "}");
}
