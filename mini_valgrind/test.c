/**
 * Mini Valgrind Lab
 * CS 241 - Spring 2017
 */

#include <stdio.h>
#include <stdlib.h>

int main() {
  // your tests here using malloc and free
 	void *p1 = malloc(30);
    void *p2 = malloc(40);
    char *p3 = malloc(50);
    free(p2);
    free(p3+60);
    return 0;
}
