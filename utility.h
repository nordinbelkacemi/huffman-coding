#ifndef UTILITY_H
#define UTILITY_H

#include <stdio.h>
#include <math.h>
#include <string.h>
#include <stdbool.h>
#include "debugmalloc.h"
#include "econio.h"

/* for debugging */
void print_char(char c);
void printbin(unsigned char c);
void printbin_huffcode(unsigned long code, size_t length);

/* for opening files */
FILE *comp_file(char *filename, char *mode);
FILE *helper_file(char *filename, char *mode);
FILE *restored_file(char *filename, char *mode);
FILE *orig_file(char *filename);

/* other utility functions */
long int file_size(FILE *f);
bool is_txt(char *filename);
bool is_dat(char *filename);
int power(int n, int exp);
void print_usage();

#endif