#ifndef COMPRESSION_H
#define COMPRESSION_H

#include "huffman.h"
#include "file-compare.h"

void compress(char *filename);
void restore(char *filename);
void verify(char *filename);

#endif
