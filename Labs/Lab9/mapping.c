#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/sem.h>

double big_one[500];
double small_one[50];
int y;

int main() {
    char *arr;
    bool test;
    int x = 0;
    char *ptr = 'c';
    double arr1[1024];
    double arr2[16];
    struct sembuf swait,ssig;

    //Before malloc
    printf (". Before malloc is at %X\n", arr);

    arr = (char*)malloc(20);
    strcpy(arr, "Hello World");

    printf ("This main function begins at %X\n", main);
    printf ("The two arrays are allocated at %X and %X\n", big_one,
            small_one);
    printf ("int y is at %X\n", &y);
    printf ("Test is at %X\n", &test);
    printf ("swait is at %X\n", &swait);
    printf ("int x is at %X\n", &x);
    printf ("ptr is at %X\n", &ptr);
    printf ("arr1 is at %X\n", arr1);
    printf ("arr2 is at %X\n", arr2);

    //During malloc and after free
    printf ("Malloc arr is at %X\n", arr);
    free(arr);
    printf ("Free arr is at %X\n", arr);

    return 0;
}
