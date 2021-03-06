#include "compression.h"

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

    econio_textcolor(COL_RED);
    pretty_print(file_size(fin));
    printf(" bytes ");
    econio_textcolor(COL_RESET);
    printf("-> ");

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

    econio_textcolor(COL_GREEN);
    pretty_print(file_size(fout));
    printf(" bytes\n");
    econio_textcolor(COL_RESET);

    fclose(fout);
    fclose(fin);
}

/* takes in a file's name and compresses it into a binary file */
void compress(char *filename) {
    if (fopen(filename, "r") == NULL){
        perror("File not found");
        exit(1);
    }

    size_t table_size;
    HuffNode *huffman_tree = build_huffman_tree(filename, &table_size, "compression");
    HuffCode *huffman_table = build_huffman_table(huffman_tree, table_size);
    quicksort_table(huffman_table, 0, table_size - 1);

    draw_tree(huffman_tree, huffman_table, table_size);

    printf("%s:\t", filename);
    write_binary(filename, huffman_table, table_size);

    free(huffman_table);
    free_tree(huffman_tree);
}

/* writes characters into the restored text file */
void write_text(char *filename, HuffNode *huffman_tree) {
    FILE *fout = restored_file(filename, "w");
    FILE *fin = comp_file(filename, "rb");

    HuffNode *root = huffman_tree;
    HuffNode *p = root;
    int chars_to_write = root->freq;
    int chars_written = 0;
    int curr_buffsize;

    unsigned char buffer;

    while (fread(&buffer, sizeof(unsigned char), 1, fin) == 1) {
        curr_buffsize = 8;
        while (curr_buffsize != 0 && chars_written != chars_to_write) {
            int step = buffer / power(2, sizeof(unsigned char) * 8 - 1);
            buffer = buffer << 1;
            curr_buffsize -= 1;
            if (!is_leaf(p)) {
                if (step == 0)
                    p = p->left;
                else
                    p = p->right;
            }
            if (is_leaf(p)) {
                fputc(p->c, fout);
                chars_written += 1;
                p = root;
            }
        }
    }

    fclose(fin);
    fclose(fout);
}

/* restores the compressed file back to its original state */
void restore(char *filename) {
    if (fopen(filename, "rb") == NULL) {
        perror("File not found");
        exit(1);
    }

    size_t table_size;
    HuffNode *huffman_tree = build_huffman_tree(filename, &table_size, "restore");
    HuffCode *huffman_table = build_huffman_table(huffman_tree, table_size);

    write_text(filename, huffman_tree);

    free(huffman_table);
    free_tree(huffman_tree);
}

/* compares the contents of two file streams */
void compare(FILE *a, FILE *b) {
    int c1, c2;
    while ((c1 = fgetc(a)) != EOF && (c2 = fgetc(b)) != EOF) {
        if (c1 != c2) {
            econio_textcolor(COL_RED);
            printf("Failed: The restored file does not match the original file!\n");
            econio_textcolor(COL_RESET);
            return;
        }
    }
    econio_textcolor(COL_GREEN);
    printf("Success: The restored file matches the original file!\n");
    econio_textcolor(COL_RESET);
}

/* verifies if the original and the restored files match */
void verify(char *filename) {
    FILE *original = orig_file(filename);
    FILE *restored = restored_file(filename, "r");

    compare(original, restored);
}
