#include "compression.h"

int main(int argc, char *argv[]) {
    if (argc - 1 != 2) {
        print_usage();
        return 1;
    } else {
        char *mode = argv[1];
        char *filename = argv[2];

        if (strcmp(mode, "compress") == 0) {
            if (is_txt(filename))
                compress(filename);
            else {
                print_usage();
                return 3;
            }
        } else if (strcmp(mode, "restore") == 0) {
            if (is_dat(filename))
                restore(filename);
            else {
                print_usage();
                return 3;
            }
        } else {
            print_usage();
            return 2;
        }
    }
    return 0;
}
