#include <stdlib.h>
#include <stdio.h>
#include "vector.h"

/**
 * Dynamically allocates a new vector.
 * @param elem_copy_func func which copies the element stored in the vector (returns
 * dynamically allocated copy).
 * @param elem_cmp_func func which is used to compare elements stored in the vector.
 * @param elem_free_func func which frees elements stored in the vector.
 * @return pointer to dynamically allocated vector.
 * @if_fail return NULL.
 */
vector *vector_alloc(vector_elem_cpy elem_copy_func, vector_elem_cmp elem_cmp_func,
                     vector_elem_free elem_free_func)
  {
      vector* v = (vector*)malloc(sizeof(*v));
      if(v == NULL)
      {
          return NULL;
      }
      if ((elem_copy_func==NULL)||(elem_cmp_func == NULL)||(elem_free_func == NULL))
      {
          return NULL;
      }
      v->capacity = VECTOR_INITIAL_CAP;
      v->size =0;
      v->data = malloc(sizeof(void*)* v->capacity);
      if (v->data == NULL)
      {
          free(v);
          return NULL
      }
      v->elem_copy_func = elem_copy_func;
      v->elem_cmp_func = elem_cmp_func;
      v->elem_free_func = elem_free_func;
      return v;
  }

/**
 * Frees a vector and the elements the vector itself allocated.
 * @param p_vector pointer to dynamically allocated pointer to vector.
 */
void vector_free(vector **p_vector)
{

}

/**
 * Returns the element at the given index.
 * @param vector pointer to a vector.
 * @param ind the index of the element we want to get.
 * @return the element at the given index if exists (the element itself, not a copy of it),
 * NULL otherwise.
 */
void *vector_at(const vector *vector, size_t ind)
{
    return NULL;
}

/**
 * Gets a value and checks if the value is in the vector.
 * @param vector a pointer to vector.
 * @param value the value to look for.
 * @return the index of the given value if it is in the vector ([0, vector_size - 1]).
 * Returns -1 if no such value in the vector.
 */
int vector_find(const vector *vector, const void *value)
{
    return 0;
}

/**
 * Adds a new value to the back (index vector_size) of the vector.
 * @param vector a pointer to vector.
 * @param value the value to be added to the vector.
 * @return 1 if the adding has been done successfully, 0 otherwise.
 */
int vector_push_back(vector *vector, const void *value)
{
    return 0;
}

/**
 * This function returns the load factor of the vector.
 * @param vector a vector.
 * @return the vector's load factor, -1 if the function failed.
 */
double vector_get_load_factor(const vector *vector)
{
    return 0.0;
}

/**
 * Removes the element at the given index from the vector. alters the indices of the remaining
 * elements so that there are no empty indices in the range [0, size-1] (inclusive).
 * @param vector a pointer to vector.
 * @param ind the index of the element to be removed.
 * @return 1 if the removing has been done successfully, 0 otherwise.
 */
int vector_erase(vector *vector, size_t ind)
{
    return 0;
}

/**
 * Deletes all the elements in the vector.
 * @param vector vector a pointer to vector.
 */
void vector_clear(vector *vector)
{

}


