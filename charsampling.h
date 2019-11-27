#ifndef CHARSAMPLING_H
#define CHARSAMPLING_H

#include <stdio.h>
#include "debugmalloc.h"

void feed(int *char_freq, char *filename);
int character_set_size(int *char_freq);

#endif
