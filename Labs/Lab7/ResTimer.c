#include <Windows.h>
#include <stdlib.h>
#include <stdio.h>

int main() {

	LARGE_INTEGER lpFrequency, lpPerformanceCountBefore, lpPerformanceCountAfter;
	int i, empty;
	double period;

	QueryPerformanceFrequency(&lpFrequency);
	period = (double)(1000.0/lpFrequency.QuadPart);
	printf("Frequency of High-Resolution Counter: %ld ticks_per_seconds\n", lpFrequency.QuadPart);
	printf("Period of High-Resolution Counter: %lf milliseconds\n", period);

	QueryPerformanceCounter(&lpPerformanceCountBefore);
	for(i = 0; i <= 1000000; i++){
		empty = i;
	}
	QueryPerformanceCounter(&lpPerformanceCountAfter);
	printf("Duration of loop: %lf miliseconds\n", period*(lpPerformanceCountAfter.QuadPart - lpPerformanceCountBefore.QuadPart));

	QueryPerformanceCounter(&lpPerformanceCountBefore);
	QueryPerformanceCounter(&lpPerformanceCountAfter);
	printf("Overhead: %lf miliseconds\n", period*(lpPerformanceCountAfter.QuadPart - lpPerformanceCountBefore.QuadPart));

	getchar();
}