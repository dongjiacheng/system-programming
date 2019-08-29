/**
 * Machine Problem: Vector
 * CS 241 - Spring 2017
 */

#include "vector.h"
#include <assert.h>
#include "callbacks.h"

#define INITIAL_CAPACITY 10
#define GROWTH_FACTOR 2

struct vector {
  /* The function callback for the user to define the way they want to copy
   * elements */
  copy_constructor_type copy_constructor;

  /* The function callback for the user to define the way they want to destroy
   * elements */
  destructor_type destructor;

  /* The function callback for the user to define the way they a default element
   * to be constructed */
  default_constructor_type default_constructor;

  /* Void pointer to the beginning of an array of void pointers to arbitrary
   * data. */
  void **array;

  /**
   * The number of elements in the vector.
   * This is the number of actual objects held in the vector,
   * which is not necessarily equal to its capacity.
   */
  size_t size;

  /**
   * The size of the storage space currently allocated for the vector,
   * expressed in terms of elements.
   */
  size_t capacity;
};

/**
 * IMPLEMENTATION DETAILS
 *
 * The following is documented only in the .c file of vector,
 * since it is implementation specfic and does not concern the user:
 *
 * This vector is defined by the struct above.
 * The struct is complete as is and does not need any modifications.
 *
 * 'INITIAL_CAPACITY' is the macro you should use to set the capacity of the
 * vector in vector_create().
 * 'GROWTH_FACTOR' is how much you should grow your vector by in automatic
 * reallocation.
 *
 * The only conditions of automatic reallocation is that
 * they should happen logarithmically compared to the growth of the size of the
 * vector
 * inorder to achieve amortized constant time complexity for appending to the
 * vector.
 *
 * For our implementation automatic reallocation happens when -and only when-
 * adding to the vector makes its new  size surpass its current vector capacity
 * OR when the user calls on vector_reserve().
 * When this happens the new capacity will be whatever power of the
 * 'GROWTH_FACTOR' greater than or equal
 * to the target capacity.
 * In the case when the new size exceeds the current capacity the target
 * capacity is the new size.
 * In the case when the user calls vector_reserve(n) the target capacity is 'n'
 * itself.
 * We have provided get_new_capacity() to help make this less ambigious.
 */

static size_t get_new_capacity(size_t target) {
  /**
   * This function works according to 'automatic reallocation'.
   * Start at 1 and keep multiplying by the GROWTH_FACTOR untl
   * you have exceeded or met your target capacity.
   */
  size_t new_capacity = 1;
  while (new_capacity < target) {
    new_capacity *= GROWTH_FACTOR;
  }
  return new_capacity;
}

vector *vector_create(copy_constructor_type copy_constructor,
                      destructor_type destructor,
                      default_constructor_type default_constructor) {
  //In Heap

  vector *V = malloc(sizeof(vector));
  
  if(copy_constructor)
  V->copy_constructor = copy_constructor;
  else
    V->copy_constructor = shallow_copy_constructor;
  if(destructor)
  V->destructor = destructor;
  else
    V->destructor = shallow_destructor;
  if(default_constructor)
  V->default_constructor = default_constructor;
  else
    V->default_constructor = shallow_default_constructor;
  
  V->size = 0;
 
  size_t c = INITIAL_CAPACITY;
 
  V->capacity = c; 
  //I can not use calloc here but i do not know why
//  void** temp = vector->array;
  V->array  = malloc(c*sizeof(void*));
 // vector->capacity = c;
//  void** temp = vector->array;
   for(size_t i=0;i< c;i++){
      V->array[i]= NULL; 
  }

  return V;
}

void vector_destroy(vector *this) {
  assert(this);
  for(size_t i = 0; i < this->capacity; i++)
    if(*(this->array+i))
    {
    this->destructor(*(this->array+i));
    //if(this->array[i])
    //free(this->array[i]);
    *(this->array+i) = NULL;
    }
  free(this->array);
  //this->array = NULL;
  free(this);
  // your code here
}

void **vector_begin(vector *this) { return this->array + 0; }

void **vector_end(vector *this) { return this->array + this->size; }

size_t vector_size(vector *this) {
  assert(this);
  return this->size;
  // your code here
 // return 0;
}

void vector_resize(vector *this, size_t n) {
  assert(this);
  size_t oldsize = this->size;
  //void** temp = this->array;
 if(n < oldsize)
  {
    for(size_t i = n ; i < oldsize; i++)
    {
    this->destructor(*(this->array+i));
    *(this->array+i) = NULL;
    this->size = n;
    }
    return;
  } 
 
  //size_t cap = get_new_capacity(n);
/* 
  if(n < this->capacity)
   {
    for(size_t i = oldsize ; i < n; i++)
    *(this->array+i) = this->default_constructor();
    this->size = n;
    return;
  }
*/ 
 //capacity is enough or not
  if(n > this->capacity)
  {
  vector_reserve(this,n);
  }
  //may cause trouble b.c. from n to capacity is 0 or not;
  
    for(size_t i = oldsize ; i < n; i++)
    *(this->array+i) = this->default_constructor();
    this->size = n;
  
  // your code here
}

size_t vector_capacity(vector *this) {
  assert(this);
  return this->capacity;
  // your code here
  //return 0;
}

bool vector_empty(vector *this) {
  assert(this);
  // your code here
  if(!(this->size))
  return true;
  else
    return false;
}

void vector_reserve(vector *this, size_t n) {
  assert(this);
  int nC;
  if(n <= this-> capacity)
    return;
  else{

  nC = get_new_capacity(n);
  
  this->capacity = nC;
  this->array = realloc(this->array,nC*sizeof(void*));

  for(int i = this->size; i < nC; i++)
    *(this->array+i) = NULL;
      }   // your code here
}

void **vector_at(vector *this, size_t position) {
  assert(this);
  assert(position < this->size && position >= 0);
  return (this->array+position);
  // your code here
  //return NULL;
}

void vector_set(vector *this, size_t position, void *element) {
  assert(this);
  assert( position >= 0);
  
  //space limited check
  if(position >= this->size)
    vector_resize(this,position + 1);

  if(*(this->array + position) != NULL)
  this->destructor(*(this->array + position));
  //if(element)
  *(this->array + position) = this->copy_constructor(element);
  //else
  //this->array[position] = NULL;
  // your code here
}

void *vector_get(vector *this, size_t position) {
  assert(this);
  assert(position < this->size && position >= 0);
  // your code here222
  //use Should I use copy constuct???
    if(*(this->array + position) == NULL)
    return NULL;

  return *(this->array + position);
}

void **vector_front(vector *this) {
  assert(this);
  // your code here
  return this->array;
}

void **vector_back(vector *this) {
  assert(this);
  assert(this->size > 0);
  // your code here
  return (this->array + this->size - 1);
}

void vector_push_back(vector *this, void *element) {
  assert(this);
  size_t newsize = this->size + 1;
  if(newsize > this->capacity)
    vector_resize(this,newsize);
  
  *(this->array+(newsize-1)) = this->copy_constructor(element);
  this->size = newsize;
//  vector_set(this,newsize - 1,element);
  // your code here
}

void vector_pop_back(vector *this) {
  assert(this);
  assert(this->size > 0);

  if(!*(this->array+this->size -1))
    {
    this->size--;
    return;
    }
  int size = this->size;
  this->destructor(*(this->array+(size-1)));
  *(this->array+(size-1)) = NULL;
  this->size--;
  // your code here
}

void vector_insert(vector *this, size_t position, void *element) {
  assert(this);
  assert(position>=0);
  if(position <= this->capacity && position >= this->size )
{
        vector_resize(this,position + 1);	
	*(this->array+position) =this->copy_constructor(element);
	return;
}
  else if(position >= this->capacity)
	{
	vector_resize(this,position + 1);
	*(this->array+position) = this->copy_constructor(element);
	return;
	}

  vector_resize(this,this->size + 1);

  void* replace = this->copy_constructor(element);
  for(size_t i = position; i < this->size; i++)
  {
    void* tempA = vector_get(this,i);
    *(this->array+i) = replace;
    replace = tempA;
  }

//  vector_set(this,this->size,replace);

  // your code here
}

void vector_erase(vector *this, size_t position) {
  assert(this);
  assert(position>=0&&position<this->size);
  int size = this->size;
  if(size < 2)
  {
    vector_pop_back(this);
    return;
  }

  void* tp = this->array[size-1];
  void* tp2;
  for(size_t i = (size-2); i > position ;i-- )
    {
    tp2 = this->array[i];
    this->array[i] = tp;
    tp = tp2;
    }
  this->destructor(this->array[position]);
  this->array[position] = tp;
  *(this->array+(size-1)) = NULL;
  this->size--;
  // your code here
}

void vector_clear(vector *this) {
  // your code here
  assert(this);
  for(size_t i = 0; i < this->size; i++)
  {
    this->destructor(*(this->array+i));
    *(this->array+i) = NULL;
  }
  this->size = 0;
}

// The following is code generated:

vector *char_vector_create() {
  return vector_create(char_copy_constructor, char_destructor,
                       char_default_constructor);
}
vector *double_vector_create() {
  return vector_create(double_copy_constructor, double_destructor,
                       double_default_constructor);
}
vector *float_vector_create() {
  return vector_create(float_copy_constructor, float_destructor,
                       float_default_constructor);
}
vector *int_vector_create() {
  return vector_create(int_copy_constructor, int_destructor,
                       int_default_constructor);
}
vector *long_vector_create() {
  return vector_create(long_copy_constructor, long_destructor,
                       long_default_constructor);
}
vector *short_vector_create() {
  return vector_create(short_copy_constructor, short_destructor,
                       short_default_constructor);
}
vector *unsigned_char_vector_create() {
  return vector_create(unsigned_char_copy_constructor, unsigned_char_destructor,
                       unsigned_char_default_constructor);
}
vector *unsigned_int_vector_create() {
  return vector_create(unsigned_int_copy_constructor, unsigned_int_destructor,
                       unsigned_int_default_constructor);
}
vector *unsigned_long_vector_create() {
  return vector_create(unsigned_long_copy_constructor, unsigned_long_destructor,
                       unsigned_long_default_constructor);
}
vector *unsigned_short_vector_create() {
  return vector_create(unsigned_short_copy_constructor,
                       unsigned_short_destructor,
                       unsigned_short_default_constructor);
}
