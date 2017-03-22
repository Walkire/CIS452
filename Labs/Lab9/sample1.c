#include <stdio.h>
#include <stdbool.h>

double big_one[500];
double small_one[50];

int main() {
    bool test;

    printf ("This main function begins at %X\n", main);
    printf ("The two arrays are allocated at %X and %X\n", big_one,
            small_one);
    printf ("Test is at %X\n", &test);
    return 0;
}
