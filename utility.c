#include "utility.h"
#include "econio.h"

void print_char(char c) {
    if (c == '\n')
        printf("\\n");
    else if (c == '\t')
        printf("\\t");
    else
        printf("%c", c);
}

/* unsigned char */

int sig_bits(unsigned char c) {
    return (c != 0) ? (int)log2((double)c) + 1 : 0;
}

void printbin_recur(unsigned char c) {
    if (c == 0)
        return;
    else {
        printbin_recur(c / 2);
        printf("%d", c % 2 == 0 ? 0 : 1);
    }
}

void printbin(unsigned char c) {
    int sb = sig_bits(c);
    for (int i = 8 * sizeof(unsigned char); i > sb; i--)
        printf("0");
    printbin_recur(c);
    printf("\n");
}

/* int */

int sig_bits_ul(unsigned long n) {
    return (n != 0) ? (int)log2((double)n) + 1 : 0;
}

void printbin_ul_recur(unsigned long n) {
    if (n == 0)
        return;
    else {
        printbin_ul_recur(n / 2);
        printf("%d", n % 2 == 0 ? 0 : 1);
    }
}

void printbin_ul(unsigned long n) {
    int sb = sig_bits_ul(n);
    for (int i = 8 * sizeof(unsigned long); i > sb; i--)
        printf("0");
    printbin_ul_recur(n);
    printf("\n");
}

/* huffman */
void printbin_huffcode(unsigned long code, size_t length) {
    int sb = sig_bits_ul(code);
    for (int i = length; i > sb; i--)
        printf("0");
    printbin_ul_recur(code);
    printf("\n");
}

/* copies the filename without the extension */
void cpy_wo_ext(char *dest, char *filename) {
    strcpy(dest, filename);
    char *p = strchr(dest, '.');
    *p = '\0';
}

/* filename without the "tiny_" prefix */
char *name_wo_pref(char *filename) {
    return strchr(filename, '_') + 1;
}

/* file stream of the compressed file */
FILE *comp_file(char *filename, char *mode) {
    if (strcmp(mode, "wb") == 0) {
        char s[101];
        char name[110];
        cpy_wo_ext(s, filename);
        strcpy(name, "tiny_");
        strcat(name, s);
        strcat(name, ".dat");
        return fopen(name, mode);
    } else {
        return fopen(filename, mode);
    }
}

/* file stream of the helper file */
FILE *helper_file(char *filename, char *mode) {
    char s[101];
    if (strcmp(mode, "wb") == 0)
        cpy_wo_ext(s, filename);
    else
        cpy_wo_ext(s, name_wo_pref(filename));

    char name[112];
    strcpy(name, s);
    strcat(name, "_helper.dat");

    return fopen(name, mode);
}

/* file stream of the restored file */
FILE *restored_file(char *filename) {
    char str[110];
    cpy_wo_ext(str, filename);
    char *s = strchr(str, '_') + 1;

    char name[114];
    strcpy(name, s);
    strcat(name, "_restored.txt");

    return fopen(name, "w");
}

/* computes n to the power of exp, assuming n and exp are positive integers */
int power(int n, int exp) {
    if (exp == 0)
        return 1;
    int result = n;
    for (int i = 1; i < exp; i++)
        result *= n;

    return result;
}

/* gets file size */
long int file_size(FILE *f) {
    fseek(f, 0L, SEEK_END);
    long int size = ftell(f);
    rewind(f);
    return size;
}

/* validates filename (txt) */
bool is_txt(char *filename) {
    char *p = strchr(filename, '.');
    if (p == NULL)
        return false;
    else
        return strcmp(p, ".txt") == 0;
}

/* validates filename (dat) */
bool is_dat(char *filename) {
    char *p = strchr(filename, '.');
    if (p == NULL)
        return false;
    else
        return strcmp(p, ".dat") == 0;
}

/* prints all information about the usage of the program */
void print_usage() {
    printf("Usage:");

    printf("\n\n");

    econio_textcolor(COL_GREEN);
    printf("\t./main compress file.txt\n");
    printf("\t./main restore tiny_file.dat");
    econio_textcolor(COL_RESET);

    printf("\n\n");

    printf("when compressing a file, ");
    econio_textcolor(COL_GREEN);
    printf("file.txt ");
    econio_textcolor(COL_RESET);
    printf("must be a text file (.txt)\n");

    printf("when restoring a file, ");
    econio_textcolor(COL_GREEN);
    printf("tiny_file.dat ");
    econio_textcolor(COL_RESET);
    printf("must be a binary file (.dat)\n");
}