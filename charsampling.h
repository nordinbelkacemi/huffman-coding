#include <stdio.h>

void feed(int *char_freq, char *filename) {
    FILE *f = fopen(filename, "r");
    if (f != NULL) {
        char c;
        while ((c = fgetc(f)) != EOF) {
            char_freq[c] += 1;
        }
    } else {
        perror("File not found");
    }
    fclose(f);
}

// determines how many distinct characters the text file contains.
int character_set_size(int *char_freq) {
    int size = 0;
    for (int i = 0; i < 128; i++)
        if (char_freq[i] != 0)
            size += 1;
    return size;
}