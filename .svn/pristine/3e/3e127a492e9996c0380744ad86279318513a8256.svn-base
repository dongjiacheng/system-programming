/**
 * Pointers Gone Wild
 * CS 241 - Spring 2017
 */

#include "part2-functions.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/**
 * (Edit this function to print out the "Illinois" lines in
 * part2-functions.c in order.)
 */
int main() {
  first_step(81);// your code here
  int a = 132;
  second_step(&a);
  int *b;
  b = malloc(sizeof(int));
  b[0] = 8942;
  double_step(&b);
  free(b);
  b = NULL;
  
  char *value = malloc(9);
  value[5] = 15;
  strange_step(value);//???
  free(value);
  char *c = malloc(4);
  c[3] = 0;
  empty_step(c);//???
  free (c);

  char* s = "uuuuu";
  void* s2;
  s2 = s;
  two_step(s2,s);

  char* first = malloc(6);
 
  
  three_step(&first[0],&2[first],&first[4]);
  free(first);
 // free(second);
 // free(third);

  char tt[4];
  tt[3] = 24;
  char ts[3];
  ts[2] = 16;
  char tf[3];
  tf[1] = 8;
  step_step_step(tf,ts,tt);

  int ii = 49;
  char* ic = "1";
  it_may_be_odd(ic,ii);

  char* tok = malloc(15);
  strcpy(tok , "Fword,CS241,!!");
  tok_step(tok);
  free (tok);
  
  char* orange = malloc(5);
  char* blue;
  blue = orange;
  orange[0] = 1;
  orange[2] = 20;
  
  the_end(orange,blue);
  free(orange);
  orange = NULL;

  return 0;
}
