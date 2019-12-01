#ifndef COMPRESSION_H
#define COMPRESSION_H

#include "debugmalloc.h"
#include "huffman.h"
#include "econio.h"

void compress(char *filename);
void restore(char *filename);
void verify(char *filename);

#endif
