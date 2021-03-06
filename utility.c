#include "utility.h"
#include "econio.h"
#include <math.h>

/* copies the filename without the extension */
void cpy_wo_ext(char *dest, char *filename) {
    strcpy(dest, filename);
    char *p = strchr(dest, '.');
    if (p != NULL)
        *p = '\0';
}

/* filename without the "tiny_" prefix */
char *name_wo_pref(char *filename) {
    char *p = strchr(filename, '_');
    if (p != NULL)
        return p + 1;
    else
        return filename;
}

/* file stream of the compressed file */
FILE *comp_file(char *filename, char *mode) {
    FILE *f;

    if (strcmp(mode, "wb") == 0) {
        char s[101];
        char name[110];
        cpy_wo_ext(s, filename);
        strcpy(name, "tiny_");
        strcat(name, s);
        strcat(name, ".dat");
        f = fopen(name, mode);
    } else
        f = fopen(filename, mode);
    
    return f;
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

    FILE *f = fopen(name, mode);
    return f;
}

/* file stream of the restored file */
FILE *restored_file(char *filename, char *mode) {
    char str[110];
    cpy_wo_ext(str, filename);
    char *s = strchr(str, '_') + 1;

    char name[114];
    strcpy(name, s);
    strcat(name, "_restored.txt");

    FILE *f = fopen(name, mode);
    return f;
}

/* file stream of the original file (provided the filename string passed in 
starts with "tiny_" and ends in ".dat") */
FILE *orig_file(char *filename) {
    char name[105];
    char *p = strchr(filename, '_') + 1;
    cpy_wo_ext(name, p);
    strcat(name, ".txt");
    FILE *f = fopen(name, "r");
    return f;
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

/* computes n to the power of exp, assuming n and exp are positive integers */
int power(int n, int exp) {
    if (exp == 0)
        return 1;
    int result = n;
    for (int i = 1; i < exp; i++)
        result *= n;

    return result;
}

void pretty_print(long int n) {
    if (n < 1000) {
        printf("%ld", n);
        return;
    } else {
        pretty_print(n / 1000);
        printf(",%03ld", n % 1000);
    }
}

int sigbits(unsigned long n) {
    if (n == 0)
        return 1;
    else
        return (int)log2(n) + 1;
}

void fprintbin_recur(FILE *f, unsigned long code) {
    if (code == 0)
        return;
    else
        fprintbin_recur(f, code / 2);
    fprintf(f, "%lu", code % 2);
}


void fprintbin(FILE *f, unsigned long code, size_t length) {
    if (length == 0) {
        fprintf(f, "0");
        return;
    }
    else {
        for (int i = 0; i < length - sigbits(code); i++)
            fprintf(f, "0");
    }
    if (code == 0)
        fprintf(f, "0");
    else
        fprintbin_recur(f, code);
}

void fprint_ascii_name(FILE *f, unsigned char c) {
    fprintf(f, "ASCII%d", c);
}

void fprint_char(FILE *f, unsigned char c) {
    if (c > 127) {
        fputc(c, f);
        return;
    }
    switch(c) {
        case 9:     fprintf(f, "tab"); break;
        case 10:    fprintf(f, "%c%cn", 92, 92); break;
        case 13:    fprintf(f, "%c%cr", 92, 92); break;
        case 39:    fprintf(f, "%c%c", 92, c); break;
        case 34:    fprintf(f, "%c%c", 92, c); break;
        case 60:    fprintf(f, "\\<"); break;
        case 62:    fprintf(f, "\\>"); break;
        case 123:   fprintf(f, "%c%c", 92, 123); break;
        case 125:   fprintf(f, "%c%c", 92, 125); break;
        default:    fprintf(f, "%c", c); break;
    }
}