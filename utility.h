#ifndef UTILITY_H
#define UTILITY_H

#include <stdio.h>
#include <math.h>
#include <string.h>
#include <stdbool.h>
#include "debugmalloc.h"
#include "econio.h"

/* file handling */
FILE *comp_file(char *filename, char *mode);
FILE *helper_file(char *filename, char *mode);
FILE *restored_file(char *filename, char *mode);
FILE *orig_file(char *filename);

long int file_size(FILE *f);
bool is_txt(char *filename);
bool is_dat(char *filename);

/* other utility functions */
int power(int n, int exp);
void print_usage();
void pretty_print(long int n);
void fprintbin(FILE *f, unsigned long code, size_t length);
void fprint_ascii_name(FILE *f, unsigned char c);
void fprint_char(FILE *f, unsigned char c);

#endif
