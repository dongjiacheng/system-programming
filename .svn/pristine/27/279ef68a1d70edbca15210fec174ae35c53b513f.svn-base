/**
 * Machine Problem: Vector
 * CS 241 - Spring 2017
 */

#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "document.h"
#include "vector.h"

struct document {
  vector *vector;
};

// This is the constructor function for string element.
// Use this as copy_constructor callback in vector.
void *string_copy_constructor(void *elem) {
  char *str = elem;
  assert(str);
  return strdup(str);
}

// This is the destructor function for string element.
// Use this as destructor callback in vector.
void string_destructor(void *elem) { free(elem); }

// This is the default constructor function for string element.
// Use this as a default constructor callback in vector.
void *string_default_constructor(void) {
  // A single null byte
  return calloc(1, sizeof(char));
}

document *document_create() {
  document * Doc = malloc(sizeof(document));// you code here!
  Doc->vector = vector_create(string_copy_constructor,string_destructor,string_default_constructor);
  return Doc;
}

void document_write_to_file(document *this, const char *path_to_file) {
  assert(this);
  assert(path_to_file);

  FILE* file_I = fopen(path_to_file,"w+");
  size_t ssize = document_size(this);
  for(size_t i = 0; i < ssize; i++)
  {
    if(document_get_line(this,i+1))
    fprintf(file_I, "%s\n", document_get_line(this,i+1));
    else
    fprintf(file_I, "\n");
  }
  fclose(file_I);

  // see the comment in the header file for a description of how to do this!
  // your code here!
}

document *document_create_from_file(const char *path_to_file) {
  assert(path_to_file);
  FILE* file_I =  fopen(path_to_file,"r");
  document* result = document_create();
  if(!file_I)
    return result;

  char *file_buf = NULL;
  size_t n;
  ssize_t read = 0;
  size_t lines = 1;
  while ((read = getline(&file_buf, &n, file_I)) != -1)
  {
    size_t len = strlen(file_buf);
    char *temp = strdup(file_buf);
    if(temp[len-1] == '\n')
    temp[len-1] = '\0';
    document_set_line(result,lines,temp);
    lines++;
    free(temp);
  }
  free(file_buf);
  fclose(file_I);
  // this function will read a file which is created by document_write_to_file
  // your code here!
  return result;
 }

void document_destroy(document *this) {
  assert(this);
  vector_destroy(this->vector);
  free(this);
}

size_t document_size(document *this) {
  assert(this);
  return vector_size(this->vector);
}

void document_set_line(document *this, size_t line_number, const char *str) {
  assert(this);
  assert(str);
  size_t index = line_number - 1;
  vector_set(this->vector, index, (void *)str);
}

const char *document_get_line(document *this, size_t line_number) {
  assert(this);
  assert(line_number > 0);
  size_t index = line_number - 1;
  return (const char *)vector_get(this->vector, index);
}

void document_insert_line(document *this, size_t line_number, const char *str) {
  assert(this);
  assert(str);
  assert(line_number > 0);
  size_t position = line_number - 1;
  vector_insert(this->vector,position,(void*)str);
  // your code here!
  // How are you going to handle the case when the user wants to
  // insert a line past the end of the document?
}

void document_delete_line(document *this, size_t line_number) {
  assert(this);
  assert(line_number > 0);
  size_t index = line_number - 1;
  vector_erase(this->vector, index);
}
