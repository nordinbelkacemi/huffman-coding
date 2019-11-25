#include "huffman.h"

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

/* reads in data from binary file to make a queue that will be used to build the huffman tree */
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
        printf("queue sampled from text file before compression:\n");
        make_helper(q, filename);
    } else {
        q = get_queue(filename);
        printf("queue sampled from helper binary before decompression:\n");
    }
    *table_size = q->size;
    print_queue(q);

    // print_queue(q);

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

/* prints huffcode with corresponding character */
void print_huffcode(HuffCode code) {
    print_char(code.character);
    printf(":\t");
    printbin_huffcode(code.code, code.length);
}

/* prints the encoding of each character */
void print_huffman_table(HuffCode *table, size_t size) {
    for (int i = 0; i < size; i++)
        print_huffcode(table[i]);
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

/* appends a code to the buffer */
void add_to_buffer(unsigned long *buffer, size_t *curr_buffsize, HuffCode code) {
    code.code = code.code << (8 * sizeof(unsigned long) - code.length - *curr_buffsize);
    *buffer = *buffer | code.code;
    *curr_buffsize += code.length;
}

/* writes the first byte of the buffer into a file */
void write_buffer(FILE *f, unsigned long *buffer, size_t *curr_buffsize) {
    unsigned char data = *buffer >> (sizeof(unsigned long) * 8 - sizeof(unsigned char) * 8);
    fwrite(&data, sizeof(unsigned char), 1, f);
    *buffer = *buffer << 8;
    *curr_buffsize -= 8;
}

/* writes the binary file */
void write_binary(char *filename, HuffCode *huffman_table, size_t table_size) {
    FILE *fout = comp_file(filename, "wb");
    FILE *fin = fopen(filename, "r");

    unsigned long buffer = 0;
    size_t curr_buffsize = 0;
    
    int c;
    while ((c = fgetc(fin)) != EOF) {
        unsigned char chr = c;
        HuffCode *code = binsearch(huffman_table, table_size, chr);
        add_to_buffer(&buffer, &curr_buffsize, *code);
        while (curr_buffsize >= 8)
            write_buffer(fout, &buffer, &curr_buffsize);
    }

    curr_buffsize = 8;
    write_buffer(fout, &buffer, &curr_buffsize);
    fclose(fout);
    fclose(fin);
}

/* reads the contents of binary file (for checking purposes) */
void read_compressed(char *filename) {
    FILE *f = comp_file(filename, "rb");
    unsigned char bfr;
    while (fread(&bfr, sizeof(unsigned char), 1, f) == 1)
        printbin(bfr);
    fclose(f);
}

/* only shows the huffman table for now */
void compress(char *filename) {
    size_t table_size;
    HuffNode *huffman_tree = build_huffman_tree(filename, &table_size, "compression");
    HuffCode *huffman_table = build_huffman_table(huffman_tree, table_size);
    quicksort_table(huffman_table, 0, table_size - 1);

    // print_huffman_table(huffman_table, table_size);
    write_binary(filename, huffman_table, table_size);

    // read_compressed(filename);

    free(huffman_table);
    free_tree(huffman_tree);
}

void write_text(char *filename, HuffNode *huffman_tree) {
    FILE *fout = restored_file(filename);
    FILE *fin = comp_file(filename, "rb");

    HuffNode *root = huffman_tree;
    HuffNode *p = root;
    int chars_to_write = root->freq;
    int chars_written = 0;
    int buffer_size;

    unsigned char buffer;

    while (fread(&buffer, sizeof(unsigned char), 1, fin) == 1) {
        buffer_size = 8;
        while (buffer_size != 0 && chars_written != chars_to_write) {
            // printbin(buffer);
            int step = buffer / power(2, sizeof(unsigned char) * 8 - 1);
            buffer = buffer << 1;
            buffer_size -= 1;
            if (step == 0)
                p = p->left;
            else
                p = p->right;
            if (is_leaf(p)) {
                fputc(p->c, fout);
                // printf("%c\n", p->c);
                chars_written += 1;
                p = root;
            }
        }
    }
}

void decompress(char *filename) {
    size_t table_size;
    HuffNode *huffman_tree = build_huffman_tree(filename, &table_size, "decompression");
    HuffCode *huffman_table = build_huffman_table(huffman_tree, table_size);
    print_huffman_table(huffman_table, table_size);
    // print_node(huffman_tree);
    write_text(filename, huffman_tree);
    free(huffman_table);
    free_tree(huffman_tree);
}
