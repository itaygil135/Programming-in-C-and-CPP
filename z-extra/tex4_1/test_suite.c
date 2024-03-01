#include <stdio.h>

#include "hashmap.h"
#include "pair.h"
#include "test_pairs.h"
#include "hash_funcs.h"
#include "test_suite.h"

#define   NUM_OF_PAIRS                      50
#define   PASS                              1
#define   FAIL                              0

#define  HASH_MAP_INITIAL_CAP_MUL_4         ( HASH_MAP_INITIAL_CAP * 4)
#define  HASH_MAP_INITIAL_CAP_MUL_2         ( HASH_MAP_INITIAL_CAP * 2)
#define  HASH_MAP_INITIAL_CAP_DIV_2         ( HASH_MAP_INITIAL_CAP / 2)
#define  HASH_MAP_INITIAL_CAP_DIV_4         ( HASH_MAP_INITIAL_CAP / 4)
#define  HASH_MAP_INITIAL_CAP_DIV_8         ( HASH_MAP_INITIAL_CAP / 8)


#define IND0                                 0
#define IND1                                 1
#define IND2                                 2
#define IND3                                 3
#define IND4                                 4
#define IND7                                 7
#define IND8                                 8
#define IND11                                11
#define IND12                                12
#define IND13                                13
#define IND15                                15
#define IND16                                16
#define IND24                                24
#define IND25                                25



void print_hashmap( hashmap *hash_map);
static pair **create_global_pairs()
{
    pair **pairs = malloc (sizeof (pair*)*NUM_OF_PAIRS);
    for (int j = 0; j < NUM_OF_PAIRS; ++j)
    {
        char key = (char) (j + 48);
        //even keys are capital letters, odd keys are digits
//        if (key % 2)
//        {
//            key += 17;
//        }
        int value = j+200;
        //size_t hash =  hash_char((void *)&key);
       // printf("j = %d key=%c  value=%d  hash=%lu\n",j, key, value, hash);
        pairs[j] = pair_alloc (&key, &value, char_key_cpy, int_value_cpy, char_key_cmp,
                               int_value_cmp, char_key_free, int_value_free);
    }
    return pairs;
}

static void free_global_pairs(pair **pairs)
{
    // Free the pairs.
    for (int k_i = 0; k_i < NUM_OF_PAIRS; ++k_i)
    {
        pair_free ((void **) &pairs[k_i]);
    }
}

/**
 * This function checks the hashmap_insert function of the hashmap library.
 * If hashmap_insert fails at some points, the functions exits with exit code 1.
 */
void test_hash_map_insert(void)
{
    unsigned int num_of_pairs = 0;
    hashmap *map = NULL;
    pair **pairs = create_global_pairs();
  

    // test #1:  insert pairs and verify the map resize
    //           each pair will be inserted to a different vector
    assert((map = hashmap_alloc(hash_char)) != NULL);

    // insert first pair and verify the load factor
    num_of_pairs++;
    assert(hashmap_insert(map, pairs[IND0]) == 1);
    assert(map->capacity == HASH_MAP_INITIAL_CAP);
    assert(map->size == num_of_pairs);
    assert(hashmap_get_load_factor(map) == 
           1.0/(float)HASH_MAP_INITIAL_CAP);
    
    // insert 11 more pairs , and verify the load factor
    for (int k = IND1; k < IND12; ++k)
    {
        num_of_pairs++;
        assert(hashmap_insert (map, pairs[k]) == PASS);
        //printf("insert key = %c\n",*(char *)(pairs[k]->key));
    }
    assert(hashmap_get_load_factor(map) == 
           12.0/(float)HASH_MAP_INITIAL_CAP); //12

    // insert 1 more pair and verify the load factor. map should be resized
    num_of_pairs++;
    assert(hashmap_insert(map, pairs[IND12]) == PASS);
    assert(hashmap_get_load_factor(map) == 
           (float)(13) / (float)(HASH_MAP_INITIAL_CAP_MUL_2));

    // insert 12 more pairs , and verify the load factor
    for (int k = IND13; k < IND25; ++k)
    {
        num_of_pairs++;
        assert(hashmap_insert(map, pairs[k]) == PASS);
        //printf("insert key = %c\n", *(char *)(pairs[k]->key));
    }

    assert(hashmap_get_load_factor(map) ==
           (float)(25) / (float)(HASH_MAP_INITIAL_CAP_MUL_4));

    // try to insert the same pair and expect a failure
    assert(hashmap_insert(map, pairs[24]) == FAIL);

    assert(hashmap_get_load_factor(map) == 
           (float)(25) / (float)(HASH_MAP_INITIAL_CAP_MUL_4));

    // clean up - remove all pairs from the map
    for (int k = IND0; k < IND25; ++k)
    {
         assert(hashmap_erase (map, pairs[k]->key) == PASS);
    }
    assert(hashmap_get_load_factor(map) == 
           (float)(0) / (float)HASH_MAP_INITIAL_CAP);

    // Free the hash-map.
    hashmap_free (&map);
    assert(map == NULL); // once hashmap was freed, the pointer should be null

    //  test #2 - load the vectors 
    assert((map = hashmap_alloc(hash_char_same_index)) != NULL);
    num_of_pairs = 0;
    // insert 25  pairs , all to the same vector, and verify the load factor
    for (int k = IND0; k < IND25; ++k)
    {
        num_of_pairs++;
        assert(hashmap_insert(map, pairs[k]) == PASS);
        //printf("insert key = %c\n", *(char *)(pairs[k]->key));
    }

    assert(hashmap_get_load_factor(map) == 
           (float)(25) / (float)(HASH_MAP_INITIAL_CAP_MUL_4));

    // try to insert the same pair and expect a failure
    assert(hashmap_insert(map, pairs[24]) == FAIL);

    assert(hashmap_get_load_factor(map) ==
           (float)(25) / (float)(HASH_MAP_INITIAL_CAP_MUL_4));
       
    // clean up - remove all pairs from the map
    for (int k = IND0; k < IND25; ++k)
    {
        assert(hashmap_erase(map, pairs[k]->key) == PASS);
    }
    assert(hashmap_get_load_factor(map) == 
           (float)(0) / (float)HASH_MAP_INITIAL_CAP);



    // Free the hash-map.
    hashmap_free(&map);
    assert(map == NULL); // once hashmap was freed, the pointer should be null




    // Free the pairs.
    free_global_pairs(pairs);

}

/**
 * This function checks the hashmap_at function of the hashmap library.
 * If hashmap_at fails at some points, the functions exits with exit code 1.
 */
void test_hash_map_at(void)
{


}

/**
 * This function checks the hashmap_erase function of the hashmap library.
 * If hashmap_erase fails at some points, the functions exits with exit code 1.
 */
void test_hash_map_erase(void)
{
    unsigned int num_of_pairs = 0;
    hashmap *map = NULL;
    pair **pairs = create_global_pairs();


    /* test #1:   insert and erase pairs when each one locate at a different bucket
       test #1.0  insert and remove one pair and verify the load factor
    */
    assert((map = hashmap_alloc(hash_char)) != NULL);
    num_of_pairs = 0;

    // insert first pair and verify the load factor
    num_of_pairs++;
    assert(hashmap_insert(map, pairs[IND0]) == PASS);
    assert(map->size == num_of_pairs);
    assert(map->capacity == HASH_MAP_INITIAL_CAP);
    assert(hashmap_get_load_factor(map) == 
              1.0 / (float)(HASH_MAP_INITIAL_CAP));

    // erase first pair and verify the load factor
    assert(hashmap_erase(map, pairs[IND0]->key) == PASS);
    num_of_pairs--;
    assert(hashmap_get_load_factor(map) == 
            0.0 / (float)(HASH_MAP_INITIAL_CAP));
    assert(map->capacity == (HASH_MAP_INITIAL_CAP_DIV_2));
    assert(map->size == num_of_pairs);


    /* test #1.1:   remove  pair from empty map, expect to fail
                    and verify the load factor */
    assert(hashmap_erase(map, pairs[0]->key) == FAIL);
    assert(map->capacity == (HASH_MAP_INITIAL_CAP_DIV_2));
    assert(map->size == num_of_pairs);
    assert(hashmap_get_load_factor(map) ==
        0.0 / (float)(HASH_MAP_INITIAL_CAP_DIV_2));


    /* test 1.1: start from capacity*4, erase all and verify */
    // insert 25 more pairs , and verify the load factor
    for (int k = IND0; k < IND25; ++k)
    {
        num_of_pairs++;
        assert(hashmap_insert(map, pairs[k]) == PASS);
       // printf("insert key = %c\n", *(char *)(pairs[k]->key));
    }
    assert(map->capacity == HASH_MAP_INITIAL_CAP_MUL_4);
    assert(map->size == num_of_pairs);
    assert(hashmap_get_load_factor(map) == 
           25.0 / (float)(HASH_MAP_INITIAL_CAP_MUL_4));

    /* remove pairs until capacity is HASH_MAP_MIN_LOAD_FACTOR*/
    for (int k = IND16; k < IND25; ++k)
    {
        assert(hashmap_erase(map, pairs[k]->key) == PASS);
        num_of_pairs--;
    }
    assert(map->capacity == HASH_MAP_INITIAL_CAP_MUL_4);
    assert(map->size == num_of_pairs);
    assert(hashmap_get_load_factor(map) ==
        16.0 / (float)(HASH_MAP_INITIAL_CAP_MUL_4));

    /* remove one pair and verify capacity (resize) */
    assert(hashmap_erase(map, pairs[15]->key) == PASS);
    num_of_pairs--;
    assert(map->capacity == HASH_MAP_INITIAL_CAP_MUL_2);
    assert(map->size == num_of_pairs);

    assert(hashmap_get_load_factor(map) ==
        15.0 / (float)(HASH_MAP_INITIAL_CAP_MUL_2));

  
    /* remove pairs until capacity is resized from 32 to 16*/
    for (int k = IND8; k < IND15; ++k)
    {
        assert(hashmap_erase(map, pairs[k]->key) == PASS);
        num_of_pairs--;
    }
    assert(map->capacity == HASH_MAP_INITIAL_CAP_MUL_2);
    assert(map->size == num_of_pairs);
    assert(hashmap_get_load_factor(map) ==
        8.0 / (float)(HASH_MAP_INITIAL_CAP_MUL_2));

    /* remove one pair and verify capacity (resize) */
    assert(hashmap_erase(map, pairs[7]->key) == PASS);
    num_of_pairs--;
    assert(map->capacity == HASH_MAP_INITIAL_CAP);
    assert(map->size == num_of_pairs);
    assert(hashmap_get_load_factor(map) ==
        7.0 / (float)(HASH_MAP_INITIAL_CAP ));


    /* remove pairs until capacity is resized from 16 to 8*/
    for (int k = IND4; k < IND7; ++k)
    {
        assert(hashmap_erase(map, pairs[k]->key) == PASS);
        num_of_pairs--;
    }
    assert(map->capacity == HASH_MAP_INITIAL_CAP);
    assert(map->size == num_of_pairs);
    assert(hashmap_get_load_factor(map) ==
        4.0 / (float)(HASH_MAP_INITIAL_CAP));

    /* remove one pair and verify capacity (resize) */
    assert(hashmap_erase(map, pairs[IND3]->key) == PASS);
    num_of_pairs--;
    assert(map->capacity == HASH_MAP_INITIAL_CAP_DIV_2);
    assert(map->size == num_of_pairs);
    assert(hashmap_get_load_factor(map) ==
        3.0 / (float)(HASH_MAP_INITIAL_CAP_DIV_2));



    /* remove pairs until capacity is resized from 8 to 4*/
    for (int k = IND2; k < IND3; ++k)
    {
        assert(hashmap_erase(map, pairs[k]->key) == PASS);
        num_of_pairs--;
    }
    assert(map->capacity == HASH_MAP_INITIAL_CAP_DIV_2);
    assert(map->size == num_of_pairs);
    assert(hashmap_get_load_factor(map) ==
        2.0 / (float)(HASH_MAP_INITIAL_CAP_DIV_2));

    /* remove one pair and verify capacity (resize) */
    assert(hashmap_erase(map, pairs[IND1]->key) == PASS);
    num_of_pairs--;
    assert(map->capacity == HASH_MAP_INITIAL_CAP_DIV_4);
    assert(map->size == num_of_pairs);
    assert(hashmap_get_load_factor(map) ==
        1.0 / (float)(HASH_MAP_INITIAL_CAP_DIV_4));

    /* remove last pair and verify capacity (resize) */
    assert(hashmap_erase(map, pairs[0]->key) == PASS);
    num_of_pairs--;
    assert(map->capacity == HASH_MAP_INITIAL_CAP_DIV_8);
    assert(map->size == num_of_pairs);
    assert(hashmap_get_load_factor(map) ==
        0.0 / (float)(HASH_MAP_INITIAL_CAP_DIV_8));
    assert(map->capacity == (HASH_MAP_INITIAL_CAP_DIV_8));
    assert(map->size == 0);


    /* test #3  try to remove the same pair and expect a failure
    */


    // clean up - remove all pairs from the map


    // Free the hash-map.
    hashmap_free(&map);
    assert(map == NULL); // once hashmap was freed, the pointer should be null

    //  test #2 - load the vectors 
    assert((map = hashmap_alloc(hash_char_same_index)) != NULL);
    num_of_pairs = 0;
    // insert 25  pairs , all to the same vector, and verify the load factor
    for (int k = IND0; k < IND25; ++k)
    {
        num_of_pairs++;
        assert(hashmap_insert(map, pairs[k]) == PASS);
       // printf("insert key = %c\n", *(char *)(pairs[k]->key));
    }

    assert(map->capacity == (HASH_MAP_INITIAL_CAP_MUL_4));
    assert(map->size == num_of_pairs);
    assert(hashmap_get_load_factor(map) == 
           25.0 / (float)(HASH_MAP_INITIAL_CAP_MUL_4));

    // try to insert the same pair and expect a failure
    assert(hashmap_insert(map, pairs[IND24]) == FAIL);

    assert(map->capacity == HASH_MAP_INITIAL_CAP_MUL_4);
    assert(map->size == num_of_pairs);
    assert(hashmap_get_load_factor(map) == 
           25.0 / (float)(HASH_MAP_INITIAL_CAP_MUL_4));

    // clean up - remove all pairs from the map
    for (int k = IND0; k < IND25; ++k)
    {
        assert(hashmap_erase(map, pairs[k]->key) == PASS);
        num_of_pairs--;
    }
   
    assert(map->capacity == (HASH_MAP_INITIAL_CAP_DIV_8));
    assert(map->size == num_of_pairs);
    assert(hashmap_get_load_factor(map) ==
           0.0 / (float)(HASH_MAP_INITIAL_CAP));



    // Free the hash-map.
    hashmap_free(&map);
    assert(map == NULL); // once hashmap was freed, the pointer should be null

    // Free the pairs.
    free_global_pairs(pairs);

}

/**
 * This function checks the hashmap_get_load_factor function of the hashmap library.
 * If hashmap_get_load_factor fails at some points, the functions exits with exit code 1.
 */
void test_hash_map_get_load_factor(void)
{
    test_hash_map_insert();
    test_hash_map_erase();

}

/**
 * This function checks the HashMapGetApplyIf function of the hashmap library.
 * If HashMapGetApplyIf fails at some points, the functions exits with exit code 1.
 */
void test_hash_map_apply_if()
{
    hashmap *map = NULL;
    pair **pairs = create_global_pairs();


    // test #1:  apply function for keys that are digits
    assert((map = hashmap_alloc(hash_char)) != NULL);

    /* push two pairs where the key is digits and two pairs 
         where the key is not a digit */
    assert(hashmap_insert(map, pairs[IND1]) == PASS);
    assert(hashmap_insert(map, pairs[IND2]) == PASS);
    assert(hashmap_insert(map, pairs[IND15]) == PASS);
    assert(hashmap_insert(map, pairs[IND16]) == PASS);

    assert(*((int *)hashmap_at(map, (pairs[IND1]->key))) == 201);
    assert(*((int *)hashmap_at(map, (pairs[IND2]->key))) == 202);
    assert(*((int *)hashmap_at(map, (pairs[IND15]->key))) == 215);
    assert(*((int *)hashmap_at(map, (pairs[IND16]->key))) == 216);

    //apply double_value on values where key is a digit
    assert(hashmap_apply_if(map, is_digit, double_value) == 2);

    assert(*((int *)hashmap_at(map, (pairs[IND1]->key))) == 402);
    assert(*((int *)hashmap_at(map, (pairs[IND2]->key))) == 404);
    assert(*((int *)hashmap_at(map, (pairs[IND15]->key))) == 215);
    assert(*((int *)hashmap_at(map, (pairs[IND16]->key))) == 216);


    // Free the hash-map.
    hashmap_free(&map);
    assert(map == NULL); // once hashmap was freed, the pointer should be null


  // test #1:  apply function for keys that are digits
    assert((map = hashmap_alloc(hash_char_same_index)) != NULL);

    /* push two pairs where the key is digits and two pairs
         where the key is not a digit */
    assert(hashmap_insert(map, pairs[IND1]) == PASS);
    assert(hashmap_insert(map, pairs[IND2]) == PASS);
    assert(hashmap_insert(map, pairs[IND15]) == PASS);
    assert(hashmap_insert(map, pairs[IND16]) == PASS);

    assert(*((int *)hashmap_at(map, (pairs[IND1]->key))) == 201);
    assert(*((int *)hashmap_at(map, (pairs[IND2]->key))) == 202);
    assert(*((int *)hashmap_at(map, (pairs[IND15]->key))) == 215);
    assert(*((int *)hashmap_at(map, (pairs[IND16]->key))) == 216);

    //apply double_value on values where key is a digit
    assert(hashmap_apply_if(map, is_digit, double_value) == 2);

    assert(*((int *)hashmap_at(map, (pairs[IND1]->key))) == 402);
    assert(*((int *)hashmap_at(map, (pairs[IND2]->key))) == 404);
    assert(*((int *)hashmap_at(map, (pairs[IND15]->key))) == 215);
    assert(*((int *)hashmap_at(map, (pairs[IND16]->key))) == 216);


    // Free the hash-map.
    hashmap_free(&map);
    assert(map == NULL); // once hashmap was freed, the pointer should be null

    // Free the pairs.
    free_global_pairs(pairs);
}

