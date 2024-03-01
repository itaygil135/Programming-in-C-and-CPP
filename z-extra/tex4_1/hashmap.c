#include <stdio.h>
#include <string.h>
#include "hashmap.h"
#include "vector.h"

#define     TRUE                              1
#define     FALSE                             0
#define     PASS                              1
#define     FAIL                              0
#define     INVALID_LOAD_FACTOR              -1

static size_t int_to_capacity(size_t num, size_t capacity);
static hashmap *allocate_resized_map(hashmap *hash_map, void *p_deleted_pair);
static hashmap *create_hashmap(hash_func func, size_t capacity);
static void free_hashmap_buckets(hashmap *hash_map);
void print_hashmap(hashmap *hash_map);
/**
 * Allocates dynamically new hash map element.
 * @param func a function which "hashes" keys.
 * @return pointer to dynamically allocated hashmap.
 * @if_fail return NULL.
 */

hashmap *hashmap_alloc (hash_func func)
{
    return create_hashmap(func,HASH_MAP_INITIAL_CAP);
}



/**
 * Frees a hash map and the elements the hash map itself allocated.
 * @param p_hash_map pointer to dynamically allocated pointer to hash_map.
 */
void hashmap_free (hashmap **p_hash_map)
{
    if ((p_hash_map != NULL) && (*p_hash_map != NULL))
    {
        free_hashmap_buckets(*p_hash_map);
        free(*p_hash_map);
        *p_hash_map = NULL;
    }
}



/**
 * Inserts a new in_pair to the hash map.
 * The function inserts *new*, *copied*, *dynamically allocated* in_pair,
 * NOT the in_pair it receives as a parameter.
 * @param hash_map the hash map to be inserted with new element.
 * @param in_pair a in_pair the hash map would contain.
 * @return returns 1 for successful insertion, 0 otherwise.
 */
int hashmap_insert (hashmap *hash_map, const pair *in_pair)
{
    int rc = FAIL;
    do 
    {
        if (hash_map == NULL)
        {
            break;
        }
        if (hashmap_at(hash_map,in_pair->key) != NULL)
        {
            break;
        }

        /* assuming insert new element will success,  increase the map size, 
              and increase the map capacity if needed */
        (hash_map->size)++;

        if (hashmap_get_load_factor(hash_map) > HASH_MAP_MAX_LOAD_FACTOR)
        {
            // create a new map with larger capacity
            hashmap *new_hash_map = allocate_resized_map(hash_map, NULL);
            if (new_hash_map == NULL)
            {
                (hash_map->size)--;
                 break;
            }
            /* if new map was created - try to push the new element into it
               in case of failure - free the new map */
            if (hashmap_insert(new_hash_map, in_pair) == FAIL)
            {
                hashmap_free(&new_hash_map);
                (hash_map->size)--;
                break;
            }
            /* if new element was pushed into the new map-free the old buckets
                and link the new map's buckets to the hash map */

                free_hashmap_buckets(hash_map);
                hash_map->buckets = new_hash_map->buckets;
                hash_map->capacity = new_hash_map->capacity;
                hash_map->size = new_hash_map->size;
                free(new_hash_map);
        }
        else
        {
            size_t ind = int_to_capacity(hash_map->hash_func(in_pair->key),
                hash_map->capacity);
            /* if there is a vector for this index at the map
                - push the new element to the vector  */
            if (hash_map->buckets[ind] != NULL)
            {
                if (vector_push_back(hash_map->buckets[ind], in_pair) == FAIL)
                {
                    (hash_map->size)--;
                    break;
                }
            }
            /* if there is no vector for this index at the map
              -  allocate new vector and push the element to the new vector*/
            else
            {
                hash_map->buckets[ind] = vector_alloc(pair_copy,
                                                      pair_cmp,
                                                      pair_free);
                if (hash_map->buckets[ind] == NULL)
                {
                    (hash_map->size)--;
                    break;
                }

                if (vector_push_back(hash_map->buckets[ind], in_pair) == FAIL)
                {
                    vector_free(&(hash_map->buckets[ind]));
                    (hash_map->size)--;
                    break;
                }
            }
        }
        rc = PASS;
    } while (0);
    return rc;
}

/**
 * The function returns the value associated with the given key.
 * @param hash_map a hash map.
 * @param key the key to be checked.
 * @return the value associated with key if exists, NULL otherwise 
    (the value itself, not a copy of it).
 */
valueT hashmap_at (const hashmap *hash_map, const_keyT key)
{
    valueT found_value = NULL;
    do 
    {
        if (hash_map == NULL)
        {
            break;
        }

        size_t ind = int_to_capacity(hash_map->hash_func(key),
                                     hash_map->capacity);
        if(hash_map->buckets[ind] == NULL)
        {
            break;
        }
        pair *p_pair = NULL;
        size_t i = 0;
        for(i = 0; i < hash_map->buckets[ind]->size; i++)
        {
            p_pair = (pair*)vector_at(hash_map->buckets[ind],i);
            if(p_pair->key_cmp(p_pair->key,key) == PASS)
            {
                found_value = p_pair->value;
                break;
            }
        }

    } while (0);

    return found_value;
}

/**
 * The function erases the pair associated with key.
 * @param hash_map a hash map.
 * @param key a key of the pair to be erased.
 * @return 1 if the erasing was done successfully, 0 otherwise.
     (if key not in map, considered fail).
 */
int hashmap_erase (hashmap *hash_map, const_keyT key)
{
    int rc = FAIL;
    hashmap *new_hash_map = NULL;
    do
    {
        if((hash_map == NULL) || (key == NULL))
        {
            break;
        }

        if (hashmap_at(hash_map,key) == NULL)
        {
            break;
        }


        size_t ind = int_to_capacity(hash_map->hash_func(key),
                                     hash_map->capacity);

        /* since the key was found at the table, for sure its vector is not
         * NULL, so no need to verify it */

        pair *p_pair = NULL;
        //pair *keep_pair = NULL;
        size_t i = 0;
        /* scan all pairs at the relevant vector. At the end of the loop 
              p_pair point to the pair, i is the index at vector data */
        for(i = 0; i < hash_map->buckets[ind]->size; i++)
        {
            p_pair = (pair *) vector_at(hash_map->buckets[ind], i);
            if (p_pair->key_cmp(p_pair->key, key) == PASS)
            {
                break; // pair was found, exit the for loop
            }
        }

        // assuming the erase will work - resize the map if needed.
        (hash_map->size)--;

        if (hashmap_get_load_factor(hash_map) < HASH_MAP_MIN_LOAD_FACTOR)
        {
            // try to allocate new resized map, without the pair
            new_hash_map = allocate_resized_map(hash_map, (void *)p_pair);
            if (new_hash_map == NULL)
            {
                (hash_map->size)++;
                break;
            }
            free_hashmap_buckets(hash_map);
            hash_map->buckets = new_hash_map->buckets;
            hash_map->capacity = new_hash_map->capacity;
            hash_map->size = new_hash_map->size;
            free(new_hash_map);
        }
        else
        {
            // handle the case  the pair is the only one at the vector
            if (hash_map->buckets[ind]->size == 1)
            {
               // keep_pair = pair_copy(p_pair);
                if (vector_erase(hash_map->buckets[ind], i) == FAIL)
                {
                   // pair_free((void **)&keep_pair);
                    (hash_map->size)++;
                    break;
                }
                vector_free(&(hash_map->buckets[ind]));
            }
            else  // handle the case there are few pairs at the vector
            {
                //keep_pair = pair_copy(p_pair);
                if (vector_erase(hash_map->buckets[ind], i) == FAIL)
                {
                    //pair_free((void **)&keep_pair);
                    break;
                }
            }
        }
        rc = PASS;
    } while (0);
    return rc;
}

/**
 * This function returns the load factor of the hash map.
 * @param hash_map a hash map.
 * @return the hash map's load factor, -1 if the function failed.
 */
double hashmap_get_load_factor (const hashmap *hash_map)
{
    double load = INVALID_LOAD_FACTOR;
    do
    {
        if (hash_map == NULL)
        {
            break;
        }

        if (hash_map->capacity > 0)
        {
            load = ((double)(hash_map->size) / (double)(hash_map->capacity));
        }

    } while (0);

    return load;
}

/**
 * This function receives a hashmap and 2 functions, the first checks a condition on the keys,
 * and the seconds apply some modification on the values. The function should apply the modification
 * only on the values that are associated with keys that meet the condition.
 *
 * Example: if the hashmap maps char->int, keyT_func checks if the char is a capital letter (A-Z),
 * and val_t_func multiples the number by 2, hashmap_apply_if will change the map:
 * {('C',2),('#',3),('X',5)}, to: {('C',4),('#',3),('X',10)}, and the return value will be 2.
 * @param hash_map a hashmap
 * @param keyT_func a function that checks a condition on keyT and return 1 if true, 0 else
 * @param valT_func a function that modifies valueT, in-place
 * @return number of changed values
 */
int hashmap_apply_if (const hashmap *hash_map, keyT_func keyT_func, valueT_func valT_func)//const
{
    int num_of_changed_pairs = 0;
    pair *p_pair = NULL;
    for (size_t ind_map = 0; ind_map < hash_map->capacity; ind_map++)
    {
        if (hash_map->buckets[ind_map] != NULL)
        {
            for (size_t ind_vec = 0; ind_vec < hash_map->buckets[ind_map]->size; ind_vec++)
            {
                p_pair = (pair*)vector_at(hash_map->buckets[ind_map], ind_vec);
                if (keyT_func(p_pair->key) == PASS)
                {
                    valT_func(p_pair->value);
                    num_of_changed_pairs++;
                }
            }
        }
    }
    return num_of_changed_pairs;
}


void print_hashmap( hashmap *hash_map)
{
    printf("---start--------------------------------------------------------\n");
    printf ("cap=%lu, size =%lu , load=%f \n",hash_map->capacity, hash_map->size,
            hashmap_get_load_factor((hash_map)));
    for (size_t i = 0 ; i < hash_map->capacity; i++)
    {
        if (hash_map->buckets[i] != NULL)
        {
            printf ("bucket %lu    size of bucket=%lu\n",i, hash_map->buckets[i]->size);
            for (size_t j = 0 ; j < hash_map->buckets[i]->size; j++)
            {

            }
        }
    }
    printf("---end--------------------------------------------------------\n");

}


static hashmap *allocate_resized_map(hashmap *hash_map, void *p_deleted_pair)
{
    hashmap * new_hashmap = NULL;
    size_t new_capacity = 0;
    pair *p_pair = NULL;
    void *p_new_pair = NULL;
    int b_error_found = FALSE;
    do
    {
        if (hashmap_get_load_factor(hash_map) < HASH_MAP_MIN_LOAD_FACTOR)
        {
            new_capacity = hash_map->capacity / HASH_MAP_GROWTH_FACTOR;
        }
        else if (hashmap_get_load_factor(hash_map) > HASH_MAP_MAX_LOAD_FACTOR)
        {
            new_capacity = hash_map->capacity * HASH_MAP_GROWTH_FACTOR;
        }
        else
        {
            break;  // unexpected
        }

        new_hashmap = create_hashmap(hash_map->hash_func, new_capacity);
        if (new_hashmap == NULL)
        {
            break;
        }

        /* copy all pairs from existing map to the new one, 
            excluding the deleted one  */
        for (size_t hash_ind = 0; hash_ind < hash_map->capacity; hash_ind++)
        {
            if (hash_map->buckets[hash_ind] != NULL)
            {
                for (size_t vec_ind = 0;
                    vec_ind < hash_map->buckets[hash_ind]->size;
                    vec_ind++)
                {
                    p_pair = (pair *)vector_at(hash_map->buckets[hash_ind],
                        vec_ind);
                    if ((p_deleted_pair == NULL) // increasing the map
                        ||
                        ((p_deleted_pair != NULL)  // decreasing the map
                            && (pair_cmp(p_deleted_pair, p_pair) == 0))) 
                    {
                        p_new_pair = pair_copy(p_pair);
                        if (hashmap_insert(new_hashmap, p_new_pair) == FAIL)
                        {
                            pair_free((void **)&p_new_pair);
                            b_error_found = TRUE;
                            break;
                        }
                    }
                }
            }
            if (b_error_found == TRUE)
            {
                break;
            }
        }
        if (b_error_found == TRUE)
        {
            hashmap_free(&new_hashmap);
        }

    } while (0);

    return new_hashmap;
}

/**
 * Allocates dynamically new hash map element, at a given size
 * @param func a function which "hashes" keys.
 * @param capacity the initial capacity of the map
 * @return pointer to dynamically allocated hashmap.
 * @if_fail return NULL.
 */
static hashmap *create_hashmap(hash_func func, size_t capacity)
{
    hashmap * my_hashmap = NULL;
    my_hashmap = (hashmap*)calloc(1, sizeof(hashmap));

    if (my_hashmap == NULL)
    {
        return NULL;
    }
    my_hashmap->buckets = (vector**)calloc(capacity, sizeof(vector*));
    if (my_hashmap->buckets == NULL)
    {
        free(my_hashmap);
        return NULL;
    }
    memset(my_hashmap->buckets, 0, sizeof(vector*)*capacity);
    my_hashmap->size = 0;
    my_hashmap->capacity = capacity;
    my_hashmap->hash_func = func;
    return my_hashmap;

}
static void free_hashmap_buckets(hashmap *hash_map)
{
    size_t index = 0;
    for (index = 0; index < (hash_map)->capacity; index++)
    {
        vector_free(&((hash_map)->buckets[index]));
    }
    free((hash_map)->buckets);
    (hash_map)->buckets = NULL;

}

static size_t int_to_capacity(size_t num, size_t capacity)
{
    return (num & (capacity - 1));
}

