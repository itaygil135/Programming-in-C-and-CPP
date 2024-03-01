typedef void(*FreeFunc)(const void *);
typedef int(*CopyFunc)(void *dest, const void *src);

/**
* The vector holds these structs
*
*/
typedef struct Element {
size_t elem_size;
void *data;
FreeFunc free_func;
CopyFunc copy_func;
} Element;

typedef void(*ForEachFunc)(Element *element, void *extra_data);

typedef struct Vector {
size_t size;
size_t capacity;
Element *arr;
} Vector;

/*******************************************************/
/*******************************************************/
/**             Implementation from here              **/
/*******************************************************/
/*******************************************************/

/**
* returns a dynamically allocated vetor, with initial capacity of 2
*/
Vector *NewVector()
{
Vector* vec;
vec = (Vector*)malloc(Vector);
if(vec != NULL)
{
vec->capacity = 2;
vec->size = 0;
vec-arr = (Element*)malloc(2*sizeof(Element));
if(vec-arr == NULL){return NULL;}
}
return vec;
}
/**
* PushBack - gets a pointer to a vector, generic data, pointers to free and copy
* functions and the element size. Adds a copy of the given element at the back of
* the vector (the first available spot). Should return 1 on success, 0 on fail.
* Runtime - O(1) amortized.
*
* Adds an element with a copy of the given data, the given free_func and copy_func and elem size
* to the end of the given vector. Runtime should be O(1) amortized.
*/
int PushBack(Vector *v, void *data, FreeFunc free_func, CopyFunc copy_func, size_t elem_size)
{
if((v == NULL)&&(free_func == NULL)&&(copy_func == NULL)&&(elem_size == NULL))
{return NULL;}
v->arr[v->size].data = malloc(elem_size);
if(v->arr[v->size].data == NULL){return NULL;}
copy_func(v->arr[v->size].data,data);
v->arr[v->size].elem_size = elem_size;
v->arr[v->size]->free_func = free_func;
v->arr[v->size]->copy_func = copy_func;
v->size++;
if(v->size == v->capacity)
{
Element* temp;
temp = realloc(v->arr,((v->capacity)*2)*sizeof(Element));
if(temp == NULL){return NULL;}
v->arr = temp;
v->capacity = v->capacity*2;
}
return 1;
}

/**
* Erases an element from the vector. Releases the element's allocated resources and moves
* the elements of the vector such that there are no "gaps" in the sequence of alement.
* Does not change the capacity of the vector.
* For example, if the vector has capacity of 4, with 3 elements: A->B->C->NIL, then Erase(1) will result
* in the vector A->C->NIL->NIL
*/
void Erase(Vector *v, size_t pos)
{
void* mil;
mil = realloc(vec-arr[pos].data,vec-arr[pos+1].elem_size);
if(mil != NULL)
{
vec->arr[pos].data = mil;
vec-arr[pos].copy_func(vec-arr[pos].data,vec->arr[pos+1].data);

}
}

/**
* Removes the last element from the vector.
*/
void PopBack(Vector *v) {

}

/**
* Applies the given function on every element in the vector.
*/
void ForEach(Vector *v, ForEachFunc f, void *extra_data) {

}

/**
* Free's all resources of the vector
*/
void FreeVector(Vector **p_vec) {

}