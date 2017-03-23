#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <mcheck.h>

#define SIZE 16

int main()
{
    mtrace();
    char *data1, *data2;
    int k;
    do {
        data1 = malloc(SIZE);
        printf ("Please input your EOS username: ");
        scanf ("%s", data1);
        if (! strcmp (data1, "quit"))
            break;
        data2 = malloc(SIZE);
        for (k = 0; k < SIZE; k++)
            data2[k] = data1[k];
        free (data1);
        printf ("data2 :%s:\n", data2);
	//Added freeing data2
	free(data2);
	muntrace();
    } while(1);
    return 0;
}
