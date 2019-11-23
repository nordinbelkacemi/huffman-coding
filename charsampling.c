#include <stdlib.h>
#include "charsampling.h"

/* fills the char_freq array with the right character frequencies */
void feed(int *char_freq, char *filename) {
    FILE *f = fopen(filename, "r");
    if (f != NULL) {
        int c;
        while ((c = fgetc(f)) != EOF) {
            char_freq[c] += 1;
        }
    }
    else {
        perror("File not found");
        exit(1);
    }
    fclose(f);
}

/* determines how many distinct characters the text file contains. */
int character_set_size(int *char_freq) {
    int size = 0;
    for (int i = 0; i < 256; i++)
        if (char_freq[i] != 0)
            size += 1;
    return size;
}
