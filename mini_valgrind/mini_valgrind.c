/**
 * Mini Valgrind Lab
 * CS 241 - Spring 2017
 */

#include "mini_valgrind.h"
#include <stdio.h>
#include <string.h>

void *mini_malloc(size_t request_size, const char *filename,
                  void *instruction) {
  //0 size requested
  if(request_size == 0)
    return NULL;

  //memory for metadata and size, what should i returned? updated globle
  meta_data* memory = malloc(request_size + sizeof(meta_data));
  total_memory_requested += request_size;
  memory->request_size = request_size;
  memory->instruction = instruction;
  memory->filename = filename;
  //memory->next = NULL;
 
    meta_data* temp = head;
    head = memory;
    memory->next = temp;
  
  
  // your code here

  return (void*)(&memory[1]);
}

void *mini_calloc(size_t num_elements, size_t element_size,
                  const char *filename, void *instruction) {
  // if(num_elements == 0 || element_size == 0)
  //  return NULL;
  void* ptr =  mini_malloc(num_elements*element_size,filename,instruction);
  memset(ptr,0,num_elements*element_size);
  // your code here
  
  return ptr;
}

void *mini_realloc(void *payload, size_t request_size, const char *filename,
                   void *instruction) {
  // your code here
  if(payload == NULL && request_size == 0)
    return NULL;
  if(payload == NULL)
    return mini_malloc(request_size,filename,instruction);
  
  //check valid of payload
  if(head == NULL)
  {
    invalid_addresses++;
    return NULL;
  }
  
  meta_data* memory = head;
  meta_data* beforeM = NULL;
  while((void*)(&memory[1]) != payload)
  {
    beforeM = memory;
    memory = memory->next;
    if(memory == NULL)
    {
      invalid_addresses++;
      return NULL;
    }
  }

  if(request_size == 0)
  {
    mini_free(payload);
    return NULL;
  }

size_t previousSize = memory->request_size;
meta_data* temp = memory->next;

if(previousSize > request_size)
  total_memory_freed += (previousSize - request_size);
if(previousSize < request_size)
  total_memory_requested += (request_size - previousSize);

memory = realloc(memory,request_size + sizeof(meta_data));

if(memory == NULL)
{
  puts("realloc failed");
  mini_free((void*)(&memory[1]));
  return NULL;
}

memory->request_size = request_size;
memory->instruction = instruction;
memory->filename = filename;
memory->next = temp;
if(beforeM == NULL)
  head = memory;
else
  beforeM->next = memory;


return (void*)(&memory[1]);
  


 
}

void mini_free(void *payload) {
  // your code here
  if(payload == NULL)
    return;
  
  if(head == NULL)
  {
    invalid_addresses++;
    return;
  }
  
  meta_data* memory = head;
  meta_data* beforeM = NULL;
  while((void*)(&memory[1]) != payload)
  {
    beforeM = memory;
    memory = memory->next;
    if(memory == NULL)
    {
      invalid_addresses++;
      return;
    }
  }

  total_memory_freed += memory->request_size;

  if(beforeM == NULL)
  {
    head = memory->next;
    free(memory);
    return;
  }

  beforeM->next = memory->next;
  
  free(memory);
}
