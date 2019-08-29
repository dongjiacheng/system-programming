/**
 * Machine Problem: Vector
 * CS 241 - Spring 2017
 */
 #include <stdio.h>
#include "vector.h"
int main(int argc, char *argv[]) {
  // Write your test cases here
  vector *new_vector = int_vector_create();
  printf("vector's capacity is %zu\n", vector_capacity(new_vector));
  printf("vector's size is %zu\n", vector_size(new_vector));
  bool bo  = vector_empty(new_vector);
  printf("bo%d\n",bo);
  //0
  int *a = malloc(sizeof(int));
  *a = 1;
  vector_push_back(new_vector, a);
  void * h = vector_get(new_vector,0);
  if(h!=NULL)
  printf("%d\n",*(int*)h);
  
 int *b = malloc(sizeof(int));
  *b = 2;
  vector_push_back(new_vector, b);
  h = vector_get(new_vector,1);

if(h!=NULL)
  printf("%d\n",*(int*)h);
 
  //2
 int *c = malloc(sizeof(int));
  *c = 3;
 vector_push_back(new_vector, c);
  //3
  
int *d = malloc(sizeof(int));
  *d = 4;
  vector_push_back(new_vector, d);
  //4
  int *e = malloc(sizeof(int));
  *e = 5;
  vector_push_back(new_vector, e);
  //5
 int *f = malloc(sizeof(int));
  *f = 6;
  vector_push_back(new_vector, f);
  //6
  int *g = malloc(sizeof(int));
  *g = 7;
  vector_push_back(new_vector, g);
  //7
  int *s = malloc(sizeof(int));
  *s = 8;
  vector_push_back(new_vector, s);
  //8
  int *i = malloc(sizeof(int));
  *i = 9;
  vector_push_back(new_vector, i);
  //9
  int *j = malloc(sizeof(int));
  *j = 10;
  vector_push_back(new_vector, j);

  size_t p = 0;
 while(p < vector_size(new_vector))
  { 
   printf("%d\n", *(int*)vector_get(new_vector, p));
   p++;
  }
 printf("vector's new capacity is %zu\n", vector_capacity(new_vector));
  printf("vector's new size is %zu\n", vector_size(new_vector));
int *n = malloc(sizeof(int));
  *n = 11;
  vector_push_back(new_vector, n);

  int *o = malloc(sizeof(int));
  *o = 12;
  vector_push_back(new_vector, o);

  int *p_ = malloc(sizeof(int));
  *p_ = 13;
  vector_push_back(new_vector, p_);

  int *q = malloc(sizeof(int));
  *q = 14;
   vector_push_back(new_vector, q);

  int *r = malloc(sizeof(int));
  *r = 15;
  vector_push_back(new_vector, r);
  p = 0;
 while(p < vector_size(new_vector))
  {
   printf("%d\n", *(int*)vector_get(new_vector, p));
   p++;
  }

  vector_insert(new_vector,0,r);

  vector_insert(new_vector,10,r);

  printf("vector's new capacity is %zu\n", vector_capacity(new_vector));
  printf("vector's new size is %zu\n", vector_size(new_vector));
 p = 0;
 while(p < vector_size(new_vector))
  {
   printf("%d\n", *(int*)vector_get(new_vector, p));
   p++;
  }


 vector_pop_back(new_vector);
 vector_pop_back(new_vector);
 puts("poped");
 printf("vector's new capacity is %zu\n", vector_capacity(new_vector));
  printf("vector's new size is %zu\n", vector_size(new_vector));
  p = 0;
  while(p < vector_size(new_vector))
  {
   printf("%d\n", *(int*)vector_get(new_vector, p));
   p++;
  }

  printf("front of the vector is: %d\n",  *((int*)*vector_front(new_vector)));
  printf("back of the vector is: %d\n",  *((int*)*vector_back(new_vector)));
 
  //vector_resize(new_vector,3);
  puts("resized");
  printf("front of the vector is: %d\n",  *((int*)*vector_front(new_vector)));
  printf("back of the vector is: %d\n",  *((int*)*vector_back(new_vector)));
  printf("vector's new capacity is %zu\n", vector_capacity(new_vector));
  printf("vector's new size is %zu\n", vector_size(new_vector));
  
  p = 0;
  while(p < vector_size(new_vector))
  {
   printf("%d\n", *(int*)vector_get(new_vector, p));
   p++;
 }

  vector_erase(new_vector,2);
  printf("front of the vector is: %d\n",  *((int*)*vector_front(new_vector)));
  printf("vector's new size is %zu\n", vector_size(new_vector));
  

  p = 0;
  while(p < vector_size(new_vector))
  {
   printf("%d\n", *(int*)vector_get(new_vector, p));
   p++;
 }

  vector_clear(new_vector);
  printf("vector's new size is %zu\n", vector_size(new_vector));
  printf("vector's new capacity is %zu\n", vector_capacity(new_vector));
  vector_destroy(new_vector);


free(a);
free(b);
free(c);
free(d);
free(e);
free(f);
free(g);
free(s);
free(i);
free(j);
free(n);
free(o);
free(p_);
free(q);
free(r);






}
