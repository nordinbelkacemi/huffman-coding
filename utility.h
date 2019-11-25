#ifndef UTILITY_H
#define UTILITY_H

#include <stdio.h>
#include <math.h>
#include <string.h>
#include <stdbool.h>

void print_char(char c);

/* unsigned char */

int sig_bits(unsigned char c);
void printbin_recur(unsigned char c);
void printbin(unsigned char c);

/* int */

int sig_bits_ul(unsigned long n);
void printbin_ul_recur(unsigned long n);
void printbin_ul(unsigned long n);

/* huffman */

void printbin_huffcode(unsigned long code, size_t length);

/* filename */

void cpy_without_extension(char *dest, char *filename);
char *name_wo_pref(char *filename);
FILE *comp_file(char *filename, char *mode);
FILE *helper_file(char *filename, char *mode);
FILE *restored_file(char *filename);

/* for checking the first bit of the buffer */

int power(int n, int exp);

#endif
