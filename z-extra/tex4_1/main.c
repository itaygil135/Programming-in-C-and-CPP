#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include "hashmap.h"
#include "test_suite.h"

#define     PASS                              1
#define     FAIL                              0
#define     NOT_FOUND                        -1

void test_demo(void);
void test_int_vector(void);


void *elem_cpy(const void * elem)
{
    int *a = malloc(sizeof (int));
    *a = *((int*)elem);
    return a;
}
int elem_cmp(const void *elem_1, const void *elem_2)
{
    return *((const int*)elem_1) == *((const int *)elem_2);
}

void elem_free(void **elem)
{
    free(*elem);
    *elem =NULL;
}

int main()
{

    /*test_demo();
     *
     */
    test_hash_map_insert();
    test_hash_map_erase();
    test_hash_map_at();
    test_hash_map_get_load_factor();
    test_hash_map_apply_if();
    //test_int_vector();

    printf("all tests passed\n");
    return EXIT_SUCCESS;

}

void test_int_vector(void)
{
    int num = 5;
    int num8 = 8;
    int num9 = 9;
    int num3 = 3;
    int* p = &num;
    vector* v = vector_alloc(elem_cpy, elem_cmp, elem_free);
    assert(vector_get_load_factor(v) == 0.0);
    assert(vector_push_back(v,p) == PASS);
    assert(vector_get_load_factor(v) == (1.0/16.0));
    assert(vector_find(v,p) == 0);
    assert(vector_find(v,&num8) == NOT_FOUND);
    assert(*((int*)vector_at(v,0)) == 5);

    assert(vector_push_back(v,&num8) == PASS);
    assert(vector_get_load_factor(v) == (2.0/16.0));
    assert(vector_find(v,&num8) == 1);
    assert(vector_find(v,&num9) == NOT_FOUND);
    assert(*((int*)vector_at(v,1)) == num8);

    assert(vector_push_back(v,&num9) == PASS);

    assert(vector_get_load_factor(v) == (3.0/16.0));
    assert(vector_find(v,&num9) == 2);
    assert(vector_find(v,&num3) == NOT_FOUND);
    assert(*((int*)vector_at(v,2)) == num9);
    assert(vector_push_back(v,&num3) == PASS);

    assert(vector_get_load_factor(v) == (0.25));
    assert(vector_find(v,&num3) == 3);
    assert(*((int*)vector_at(v,3)) == num3);
    assert(vector_erase(v,1) == 1);

    assert(vector_get_load_factor(v) == (3.0/8.0));
    assert(vector_find(v,&num9) == 1);
    assert(vector_find(v,&num8) == NOT_FOUND);
   assert(*((int*)vector_at(v,1)) == num9);
   vector_clear(v);
   assert((v->size) == 0);
   vector_free(&v);
   assert(v == NULL);


}