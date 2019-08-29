#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "debug_example.h"

#define SIZE_OF_SAMPLES 3


int main(void) {
    int* arr = malloc(sizeof(*arr)*SIZE_OF_SAMPLES);
  
    srand(time(NULL));

    uniq_rand_int(SIZE_OF_SAMPLES, arr);
    for(int j = 0; j < SIZE_OF_SAMPLES; j++) {
        printf("%d ", arr[i]);
    }
    printf("\n");
    free(arr);
    return 0;
}

void uniq_rand_int(int n, int *arr) {
    while(i < n) {
        int rand_int = rand();
        if (member(arr, i, rand_int)) {
            arr[i] =  rand_int;
            i++;
        }
    }
}

int member(int *array, int n, int x) {
    for (int i = 0; i < n; i++) {
        if (array[i] == x) {
            return 1;
        }
    }
    return 0;
}
