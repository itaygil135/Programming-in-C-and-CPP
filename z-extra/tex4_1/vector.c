#include <stdlib.h>
#include <stdio.h>
#include "vector.h"
#define     TRUE                              1
#define     FALSE                             0
#define     PASS                              1
#define     FAIL                              0
#define     NOT_FOUND                        -1
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
      vector* v = (vector*)calloc(1, sizeof(*v));
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
      v->data = calloc(v->capacity, sizeof(void*));
      if (v->data == NULL)
      {
          free(v);
          v = NULL;
          return NULL;
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
    if ((p_vector != NULL) && (*p_vector != NULL))
    {
        vector_clear(*p_vector);
        free((*p_vector)->data);
        (*p_vector)->data = NULL;
        free(*p_vector);
        *p_vector = NULL;
    }

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
    void * rc = NULL;
    do
    {
        if(vector == NULL)
        {
            break;
        }
        if (ind >  vector->size)
        {
            break;
        }
        rc = vector->data[ind];

    } while (0);
    return rc;
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
    int rc = NOT_FOUND;
    size_t ind = 0;
    if((vector != NULL)&&(value != NULL))
    {
        while (ind < vector->size)
        {
            if(vector->elem_cmp_func(vector->data[ind],value) != FALSE)
            {
               rc = ind;
               break;
            }
            ind++;
        }
    }
    return rc;
}

/**
 * Adds a new value to the back (index vector_size) of the vector.
 * @param vector a pointer to vector.
 * @param value the value to be added to the vector.
 * @return 1 if the adding has been done successfully, 0 otherwise.
 */
int vector_push_back(vector *vector, const void *value)
{
    int rc = FAIL;
    do
    {
        if ((vector == NULL)||(value == NULL))
        {break;}
        void* value_to_add =  vector->elem_copy_func(value);
        if (value_to_add == NULL)
        {
            break;
        }
        vector->data[vector->size] = value_to_add;
        (vector->size)++;
        if (vector_get_load_factor(vector) >= VECTOR_MAX_LOAD_FACTOR)
        {
            void** temp = NULL;
            temp = (void**)realloc(vector->data,sizeof(void*)*(vector->capacity * VECTOR_GROWTH_FACTOR));
            if (temp == NULL)
            {
                (vector->size)--;
                free(vector->data[vector->size]);
                vector->data[vector->size] = NULL;
                break;
            }
            vector->data = temp;
            (vector->capacity) *=VECTOR_GROWTH_FACTOR;
        }
        rc = PASS;
    } while (0);
    return rc;
}

/**
 * This function returns the load factor of the vector.
 * @param vector a vector.
 * @return the vector's load factor, -1 if the function failed.
 */
double vector_get_load_factor(const vector *vector)
{
    if (vector == NULL){return -1;}
    double load = -1;
    if (vector->capacity > 0)
    {
        load = ((double)(vector->size)/(double)(vector->capacity));
    }
    return load;
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
    int rc = FAIL;
    do
    {
        if(vector == NULL)
        {
            break;
        }
        if (ind > vector->size)
        {
            break;
        }
        void* keep_value = vector->data[ind];
        size_t i = ind;
        while (keep_value != vector->data[(vector->size)-1])
        {
            vector->data[i] = vector->data[i+1];
            vector->data[i+1] = keep_value;
            i++;
        }
        (vector->size)--;
        if (vector_get_load_factor(vector) <= VECTOR_MIN_LOAD_FACTOR)
        {
            void** temp = NULL;
            temp = (void**)realloc(vector->data,sizeof(void*)*(vector->capacity / VECTOR_GROWTH_FACTOR));
            if (temp == NULL)
            {
                (vector->size)++;
                while (keep_value != vector->data[ind])
                {
                    vector->data[i] = vector->data[i-1];
                    vector->data[i-1] = keep_value;
                    i--;
                }
                break;
            }
            vector->data = temp;
            (vector->capacity)/= VECTOR_GROWTH_FACTOR;
        }

        vector->elem_free_func(&keep_value);

        rc = PASS;
    } while (0);
    return rc;
}

/**
 * Deletes all the elements in the vector.
 * @param vector vector a pointer to vector.
 */
void vector_clear(vector *vector)
{
    if (vector != NULL)
    {
        while (vector->size != 0) {
            (void) vector_erase(vector, vector->size - 1);
        }
    }
}


