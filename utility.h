#ifndef UTILITY_H
#define UTILITY_H

#include <stdio.h>
#include <math.h>

void print_char(char c);

/* unsigned char */

int sig_bits(unsigned char c);
void printbin_recur(unsigned char c);
void printbin(unsigned char c);

/* int */

int sig_bits_int(int n);
void printbin_int_recur(int n);
void printbin_int(int n);

/* huffman */

void printbin_huffcode(int code, size_t length);

#endif
