#include <stdio.h>
#include <stdlib.h>

int main() {

    int randomnumber;
    int i = 0;
    srand(time(NULL));

    while(i < 10) {
         randomnumber = rand();
	 printf("%d\n", randomnumber);
         i++;
    }

    return 0;
}
