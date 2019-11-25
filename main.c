#include "huffman.h"

int main(int argc, char *argv[]) {
    if (argc != 3) {
        printf("Given a text file called filename.txt, the command you need to run is the following:\n\n./main filename.txt mode\n\nwhere the mode is either \"compress\" (for compression) or \"decompress\" (for decompression)");
        return 1;
    } else {
        char *filename = argv[1];
        char *mode = argv[2];
        if (strcmp(mode, "compress") == 0)
            compress(filename);
        else if (strcmp(mode, "decompress") == 0)
            decompress(filename);
        else {
            printf("the mode is either compress or decompress!\n");
            return 2;
        }
    }
    return 0;
}
