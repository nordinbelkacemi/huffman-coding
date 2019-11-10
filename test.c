#include <stdio.h>
#include <stdbool.h>

void print_binary(int n) {
    if (n == 0)
        return;
    else
        print_binary(n / 2);
    printf("%d", n % 2);
}

int main() {
    bool a = 202;
    print_binary(a);
    printf("\n");
    a = a << 2;
    print_binary(a);
    printf("\n");
    a = a << 2;
    print_binary(a);
    printf("\n");
    return 0;
}