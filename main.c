#include "huffman.h"

int main(int argc, char *argv[]) {
    if (argc != 2) {
        printf("Given a text file called filename.txt, the command you need to run is the following:\n\n./main filename.txt\n\n");
        return 1;
    } else {
        char *filename = argv[1];
        compress(filename);
    }
    return 0;
}
