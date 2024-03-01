 /*******************************
 ** C/C++ spring semester 2021 **
 ** Ex6 tests                  **
 ** version 1.1                **
 ** Written by Erel Debel      **
 *******************************/

#define BOOST_TEST_MODULE ex6_tests
#include <boost/core/ignore_unused.hpp>
#include <boost/test/included/unit_test.hpp>
#include "vl_vector.h"

/*********************************
* Basic constructors and getters *
*********************************/

BOOST_AUTO_TEST_SUITE(basic_constructors_and_getters)

BOOST_AUTO_TEST_CASE (default_constructor)
{
  vl_vector<int> v1;
  BOOST_TEST (v1.empty ());
  BOOST_TEST (v1.size () == 0UL);
  BOOST_TEST (v1.capacity () == 16UL);
  BOOST_TEST (!v1.contains (0));

  const vl_vector<double, 3> v2;
  BOOST_TEST (v2.empty ());
  BOOST_TEST (v2.size () == 0UL);
  BOOST_TEST (v2.capacity () == 3UL);
  BOOST_TEST (!v2.contains (0));
}

BOOST_AUTO_TEST_CASE (single_value_initialized_constructor)
{
  vl_vector<float> v1 (8, 0.25);
  BOOST_TEST (!v1.empty ());
  BOOST_TEST (v1.size () == 8);
  BOOST_TEST (v1.capacity () == 16);
  for (auto elem : v1)
    BOOST_TEST (elem == 0.25);
  BOOST_TEST (v1.contains (0.25));
  BOOST_TEST (!v1.contains (0.5));

  const vl_vector<float, 4> v2 (8, 0.25);
  BOOST_TEST (!v2.empty ());
  BOOST_TEST (v2.size () == 8);
  BOOST_TEST (v2.capacity () == 3 * 8 / 2);
  BOOST_TEST (v2.data ()[0] == 0.25);
  for (auto elem : v2)
    BOOST_TEST (elem == 0.25);
  BOOST_TEST (v2.contains (0.25));
  BOOST_TEST (!v2.contains (0.5));

  vl_vector<float> v3 (20, 0.25);
  BOOST_TEST (!v3.empty ());
  BOOST_TEST (v3.size () == 20);
  BOOST_TEST (v3.capacity () == 3 * 20 / 2);
  BOOST_TEST (v3.data ()[0] == 0.25);
  for (auto elem : v3)
    BOOST_TEST (elem == 0.25);
}

BOOST_AUTO_TEST_CASE (sequence_based_constructor)
{
  int arr[20];
  for (int i = 0; i < 20; ++i)
    arr[i] = i;
  vl_vector<int> v (arr, arr + 20);
  BOOST_TEST (!v.empty ());
  BOOST_TEST (v.size () == 20);
  BOOST_TEST (v.capacity () == 3 * 20 / 2);
  int i = 0;
  for (auto iter = v.cbegin (); iter < v.cend (); ++iter)
    BOOST_TEST (*iter == i++);
}

BOOST_AUTO_TEST_SUITE_END()


/************
* Iterators *
************/

BOOST_AUTO_TEST_SUITE (iterators)

BOOST_AUTO_TEST_CASE(iterator_static)
{
  int arr_len = 10;
  int arr[10];
  for (int i = 0; i < arr_len; ++i)
    arr[i] = i;
  vl_vector<int> v (arr, arr + arr_len);
  int i = 0;
  for (auto iter = v.cbegin (); iter != v.cend (); ++i, ++iter)
    BOOST_TEST(*iter == i);
  i = 0;
  for (auto iter = v.begin (); iter != v.end (); ++i, ++iter)
    BOOST_TEST(*iter == i);

  const vl_vector<int> const_v (arr, arr + arr_len);
  i = 0;
  for (auto iter = const_v.begin (); iter != const_v.end (); ++i, ++iter)
    BOOST_TEST(*iter == i);
}

BOOST_AUTO_TEST_CASE(reverse_iterator_static)
{
  int arr_len = 10;
  int arr[10];
  for (int i = 0; i < arr_len; ++i)
    arr[i] = i;
  vl_vector<int> v (arr, arr + arr_len);
  int i = arr_len - 1;
  for (auto iter = v.crbegin (); iter != v.crend (); --i, ++iter)
    BOOST_TEST(*iter == i);
  i = arr_len - 1;
  for (auto iter = v.rbegin (); iter != v.rend (); --i, ++iter)
    BOOST_TEST(*iter == i);

  const vl_vector<int> const_v (arr, arr + 10);
  i = arr_len - 1;
  for (auto iter = const_v.rbegin (); iter != const_v.rend (); --i, ++iter)
    BOOST_TEST(*iter == i);
}

BOOST_AUTO_TEST_CASE (iterators_dynamic)
{
  int arr_len = 20;
  int arr[20];
  for (int i = 0; i < arr_len; ++i)
    arr[i] = i;
  vl_vector<int> v (arr, arr + arr_len);
  int i = 0;
  for (auto iter = v.cbegin (); iter != v.cend (); ++i, ++iter)
    BOOST_TEST(*iter == i);
  i = arr_len - 1;
  for (auto iter = v.crbegin (); iter != v.crend (); --i, ++iter)
    BOOST_TEST(*iter == i);
}

BOOST_AUTO_TEST_SUITE_END()


/************************
* Random access methods *
************************/

BOOST_AUTO_TEST_SUITE (random_access_methods)

BOOST_AUTO_TEST_CASE(square_brackets_operator)
{
  int arr_len = 10;
  int arr1[10];
  for (int i = 0; i < arr_len; ++i)
    arr1[i] = i;
  vl_vector<int> v1 (arr1, arr1 + arr_len);
  for (int i = 0; i < arr_len; ++i)
    BOOST_TEST (v1[i] == i);
  try
    { v1[13]; }
  catch (const std::exception &e)
    { BOOST_TEST (false, "operator[] should throw exceptions.\n"); }

  arr_len = 20;
  int arr2[20];
  for (int i = 0; i < arr_len; ++i)
    arr2[i] = i;
  const vl_vector<int> v2 (arr2, arr2 + arr_len);
  for (int i = 0; i < arr_len; ++i)
    BOOST_TEST (v2[i] == i);
}

BOOST_AUTO_TEST_CASE(at_method)
{
  int arr_len = 10;
  int arr1[10];
  for (int i = 0; i < arr_len; ++i)
    arr1[i] = i;
  vl_vector<int> v1 (arr1, arr1 + arr_len);
  for (int i = 0; i < arr_len; ++i)
    BOOST_TEST (v1.at (i) == i);
  try
    {
      v1.at (13);
      BOOST_TEST (true,
                  "did not throw exception for an out of range index.\n");
    }
  catch (const std::out_of_range &e)
    { BOOST_TEST (true, "out_of_range exception thrown.\n"); }
  catch (const std::exception &e)
    { BOOST_TEST (false, "exception thrown, but not out_of_range.\n"); }

  arr_len = 20;
  int arr2[20];
  for (int i = 0; i < arr_len; ++i)
    arr2[i] = i;
  const vl_vector<int> v2 (arr2, arr2 + arr_len);
  for (int i = 0; i < arr_len; ++i)
    BOOST_TEST (v2.at (i) == i);
}

BOOST_AUTO_TEST_SUITE_END()


/**************************
* Operations on vl_vector *
**************************/

BOOST_AUTO_TEST_SUITE (operations_on_vl_vector)

BOOST_AUTO_TEST_CASE (push_back)
{
  vl_vector<int, 5> v;
  int *static_vec = v.data ();
  BOOST_TEST (!v.contains (0));
  for (int i = 0; i < 10; ++i)
    v.push_back (i);
  for (int i = 0; i < 10; ++i)
    BOOST_TEST (v.contains (0));
  for (int i = 0; i < 10; ++i)
    BOOST_TEST (v[i] == i);
  BOOST_TEST (static_vec != v.data ());
}

BOOST_AUTO_TEST_CASE (insert_element)
{
  vl_vector<int, 5> v;
  int *static_vec = v.data ();
  auto pos = v.begin ();
  for (int i = 5; i >= 0; --i)
    {
      BOOST_TEST (v.data () == static_vec);
      pos = v.insert (pos, i * 2);
    }
  BOOST_TEST (v.data () != static_vec);
  BOOST_TEST (v.capacity () == (size_t) (3 * 6) / 2);

  int *dynamic_vec = v.data ();
  for (int i = 1; i < 10; i += 2)
    v.insert (v.cbegin () + i, i);
  BOOST_TEST (v.size () == (size_t) 11);
  BOOST_TEST (v.capacity () == (size_t) (3 * 10) / 2);
  for (int i = 0; i < 11; ++i)
    BOOST_TEST (v[i] == i);

  BOOST_TEST (static_vec != v.data ());
  BOOST_TEST (dynamic_vec != v.data ());
}

BOOST_AUTO_TEST_CASE (insert_sequence)
{
  const int arr_len = 10;
  int arr[10];
  int j = 0;
  for (; j < arr_len / 2; ++j)
    arr[j] = j;
  for (; j < arr_len; ++j)
    arr[j] = j + 10;

  vl_vector<int, arr_len> v (arr, arr + arr_len);
  int *static_vec = v.data ();

  j = 0;
  for (; j < arr_len / 2; ++j)
    arr[j] = j + 10;
  for (; j < arr_len; ++j)
    arr[j] = j;

  auto pos = v.insert (v.cbegin () + 5, arr, arr + 5);
  BOOST_TEST (static_vec != v.data ());
  BOOST_TEST (v.capacity () == (size_t) (3 * 15) / 2);

  int *dynamic_vec = v.data ();
  v.insert (pos, arr + 5, arr + 10);
  BOOST_TEST (v.size () == (size_t) 20);
  for (int i = 0; i < 20; ++i)
    BOOST_TEST (v[i] == i);

  BOOST_TEST (static_vec != v.data ());
  BOOST_TEST (dynamic_vec == v.data ());
  BOOST_TEST (v.capacity () == (size_t) (3 * 15) / 2);
}

BOOST_AUTO_TEST_CASE (pop_back)
{
  int arr_len = 20;
  int arr[20];
  for (int i = 0; i < arr_len; ++i)
    arr[i] = i;
  vl_vector<int, 10> v (arr, arr + arr_len);
  BOOST_TEST (v.capacity () == (size_t) (3 * arr_len) / 2);
  int *dynamic_vec = v.data ();
  for (int i = 0; i < 10; ++i)
    {
      BOOST_TEST (v.size () == (size_t) (arr_len - i));
      BOOST_TEST (v.capacity () == (size_t) (3 * arr_len) / 2);
      BOOST_TEST (v.data () == dynamic_vec);
      v.pop_back ();
    }
  int *static_vec = v.data ();
  for (int i = 0; i < 10; ++i)
    {
      BOOST_TEST (v.size () == (size_t) (10 - i));
      BOOST_TEST (v.capacity () == (size_t) 10);
      BOOST_TEST (v.data () != dynamic_vec);
      BOOST_TEST (v.data () == static_vec);
      v.pop_back ();
    }
}

BOOST_AUTO_TEST_CASE(erase_element)
{
  int arr_len = 20;
  int arr[20];
  for (int i = 0; i < arr_len; ++i)
    arr[i] = i - 2;

  vl_vector<int, 10> v (arr, arr + arr_len);
  BOOST_TEST (v.size () == (size_t) arr_len);
  int *dynamic_vec = v.data ();
  v.erase (v.begin ());
  v.erase (v.begin ());
  for (int i = 0; i < 8; ++i)
    {
      BOOST_TEST (v.size () == (size_t) (arr_len - 2 - i));
      BOOST_TEST (v.capacity () == (size_t) (3 * arr_len) / 2);
      BOOST_TEST (v.data () == dynamic_vec);
      v.erase (v.begin () + 10);
    }
  BOOST_TEST (v.size () == 10);
  BOOST_TEST (v.capacity () == 10);
  BOOST_TEST (v.data () != dynamic_vec);
  for (int i = 0; i < 10; ++i)
    BOOST_TEST (v[i] == i);
}

BOOST_AUTO_TEST_CASE (erase_sequence)
{
  int arr_len = 20;
  int arr[20];
  for (int i = 0; i < arr_len; ++i)
    arr[i] = i - 2;

  vl_vector<int, 10> v (arr, arr + arr_len);
  BOOST_TEST (v.size () == (size_t) arr_len);
  int *dynamic_vec = v.data ();
  v.erase (v.begin (), v.begin () + 2);
  BOOST_TEST (v.data () == dynamic_vec);
  v.erase (v.begin () + 10, v.begin () + 18);
  BOOST_TEST (v.size () == (size_t) 10);
  BOOST_TEST (v.capacity () == (size_t) 10);
  BOOST_TEST (v.data () != dynamic_vec);
  for (int i = 0; i < 10; ++i)
    BOOST_TEST (v[i] == i);
}

BOOST_AUTO_TEST_CASE (clear)
{
  int arr_len = 20;
  int arr[20];
  for (int i = 0; i < arr_len; ++i)
    arr[i] = i - 2;

  vl_vector<int, 10> v (arr, arr + arr_len);
  BOOST_TEST (v.size () == (size_t) arr_len);
  BOOST_TEST (v.capacity () == (size_t) ((3 * arr_len) / 2));
  int *dynamic_vec = v.data ();
  v.clear ();
  BOOST_TEST (v.size () == (size_t) 0);
  BOOST_TEST (v.capacity () == (size_t) 10);
  BOOST_TEST (v.data () != dynamic_vec);
  int *static_vec = v.data ();
  v.clear ();
  BOOST_TEST (v.size () == (size_t) 0);
  BOOST_TEST (v.capacity () == (size_t) 10);
  BOOST_TEST (v.data () != dynamic_vec);
  BOOST_TEST (v.data () == static_vec);
}

BOOST_AUTO_TEST_SUITE_END()


/**********************
* Copy and comparison *
**********************/

BOOST_AUTO_TEST_SUITE (copy_and_comparison)

BOOST_AUTO_TEST_CASE (copy_constructor_dynamic)
{
  int arr[20];
  for (int i = 0; i < 20; ++i)
    arr[i] = i;
  const vl_vector<int> v (arr, arr + 20);
  vl_vector<int> v_copy (v);
  BOOST_TEST (v == v_copy);
  BOOST_TEST (!(v != v_copy));
  BOOST_TEST (!v_copy.empty ());
  BOOST_TEST (v_copy.size () == 20);
  BOOST_TEST (v_copy.capacity () == 3 * 20 / 2);
  int i = 0;
  for (auto elem : v_copy)
    BOOST_TEST (elem == i++);
  BOOST_TEST (v.data () != v_copy.data ());
  v_copy.pop_back ();
  try
    {
      v.at (19);
      BOOST_TEST (true, "v.at(19) did not change.\n");
    }
  catch (const std::out_of_range &e)
    {
      BOOST_TEST (false, "v.at(19) changed.\n");
    }
  try
    {
      v_copy.at (19);
      BOOST_TEST (false, "v_copy.at(19) did not change.\n");
    }
  catch (const std::out_of_range &e)
    {
      BOOST_TEST (true, "v_copy.at(19) changed.\n");
    }
  v_copy[0] = 3;
  BOOST_TEST (v[0] != v_copy[0]);
  BOOST_TEST (!(v == v_copy));
  BOOST_TEST (v != v_copy);
  BOOST_TEST (v.size () == 20);
  BOOST_TEST (v_copy.size () == 19);
  BOOST_TEST (v.capacity () == 3 * 20 / 2);
  BOOST_TEST (v_copy.capacity () == 3 * 20 / 2);
}

BOOST_AUTO_TEST_CASE (assignment_operator_dynamic)
{
  int arr[20];
  for (int i = 0; i < 20; ++i)
    arr[i] = i;
  const vl_vector<int> v (arr, arr + 20);
  vl_vector<int> v_copy;
  v_copy = v;
  BOOST_TEST (v == v_copy);
  BOOST_TEST (!v_copy.empty ());
  BOOST_TEST (v_copy.size () == 20);
  BOOST_TEST (v_copy.capacity () == 3 * 20 / 2);
  int i = 0;
  for (auto elem : v_copy)
    BOOST_TEST (elem == i++);
  BOOST_TEST (v.data () != v_copy.data ());
  v_copy.pop_back ();
  try
    {
      v.at (19);
      BOOST_TEST (true, "v.at(19) did not change.\n");
    }
  catch (const std::out_of_range &e)
    {
      BOOST_TEST (false, "v.at(19) changed.\n");
    }
  try
    {
      v_copy.at (19);
      BOOST_TEST (false, "v_copy.at(19) did not change.\n");
    }
  catch (const std::out_of_range &e)
    {
      BOOST_TEST (true, "v_copy.at(19) changed.\n");
    }
  v_copy[0] = 3;
  BOOST_TEST (v[0] != v_copy[0]);
  BOOST_TEST (!(v == v_copy));
  BOOST_TEST (v != v_copy);
  BOOST_TEST (v.size () == 20);
  BOOST_TEST (v_copy.size () == 19);
  BOOST_TEST (v.capacity () == 3 * 20 / 2);
  BOOST_TEST (v_copy.capacity () == 3 * 20 / 2);
}

BOOST_AUTO_TEST_CASE (copy_constructor_static)
{
  int arr[20];
  for (int i = 0; i < 20; ++i)
    arr[i] = i;
  const vl_vector<int, 20> v (arr, arr + 20);
  vl_vector<int, 20> v_copy (v);
  BOOST_TEST (v == v_copy);
  BOOST_TEST (!(v != v_copy));
  BOOST_TEST (!v_copy.empty ());
  BOOST_TEST (v_copy.size () == 20);
  BOOST_TEST (v_copy.capacity () == 20);
  int i = 0;
  for (auto elem : v_copy)
    BOOST_TEST (elem == i++);
  BOOST_TEST (v.data () != v_copy.data ());
  v_copy.pop_back ();
  try
    {
      v.at (19);
      BOOST_TEST (true, "v.at(19) did not change.\n");
    }
  catch (const std::out_of_range &e)
    {
      BOOST_TEST (false, "v.at(19) changed.\n");
    }
  try
    {
      v_copy.at (19);
      BOOST_TEST (false, "v_copy.at(19) did not change.\n");
    }
  catch (const std::out_of_range &e)
    {
      BOOST_TEST (true, "v_copy.at(19) changed.\n");
    }
  v_copy[0] = 3;
  BOOST_TEST (v[0] != v_copy[0]);
  BOOST_TEST (!(v == v_copy));
  BOOST_TEST (v != v_copy);
  BOOST_TEST (v.size () == 20);
  BOOST_TEST (v_copy.size () == 19);
  BOOST_TEST (v.capacity () == 20);
  BOOST_TEST (v_copy.capacity () == 20);
}

BOOST_AUTO_TEST_CASE (assignment_operator_static)
{
  int arr[20];
  for (int i = 0; i < 20; ++i)
    arr[i] = i;
  const vl_vector<int, 20> v (arr, arr + 20);
  vl_vector<int, 20> v_copy;
  v_copy = v;
  BOOST_TEST (v == v_copy);
  BOOST_TEST (!v_copy.empty ());
  BOOST_TEST (v_copy.size () == 20);
  BOOST_TEST (v_copy.capacity () == 20);
  int i = 0;
  for (auto elem : v_copy)
    BOOST_TEST (elem == i++);
  BOOST_TEST (v.data () != v_copy.data ());
  v_copy.pop_back ();
  try
    {
      v.at (19);
      BOOST_TEST (true, "v.at(19) did not change.\n");
    }
  catch (const std::out_of_range &e)
    {
      BOOST_TEST (false, "v.at(19) changed.\n");
    }
  try
    {
      v_copy.at (19);
      BOOST_TEST (false, "v_copy.at(19) did not change.\n");
    }
  catch (const std::out_of_range &e)
    {
      BOOST_TEST (true, "v_copy.at(19) changed.\n");
    }
  v_copy[0] = 3;
  BOOST_TEST (v[0] != v_copy[0]);
  BOOST_TEST (!(v == v_copy));
  BOOST_TEST (v != v_copy);
  BOOST_TEST (v.size () == 20);
  BOOST_TEST (v_copy.size () == 19);
  BOOST_TEST (v.capacity () == 20);
  BOOST_TEST (v_copy.capacity () == 20);
}

BOOST_AUTO_TEST_SUITE_END()

BOOST_AUTO_TEST_SUITE (mixed_test)

BOOST_AUTO_TEST_CASE (element_invariation)
{
  vl_vector<long, 5> v (4UL, 6L);
  BOOST_TEST (v.size () == 4);
  BOOST_TEST (v.capacity () == 5);
  long *static_vec = v.data ();
  long arr[5];
  for (int i = 0; i < 5; ++i)
    arr[i] = i;
  v.insert (v.begin (), arr, arr + 5);
  auto iter = v.begin ();
  for (int i = 0; iter < v.begin () + 5; ++iter, ++i)
    BOOST_TEST (*iter == i);
  for (; iter < v.end (); ++iter)
    BOOST_TEST (*iter == 6);
  BOOST_TEST (v.size () == 9);
  BOOST_TEST (v.capacity () == 13);

  auto pos = v.end ();
  for (int j = 0; j < 6; ++j)
    pos = v.insert (pos, 1000);
  iter = v.begin ();
  for (int i = 0; iter < v.begin () + 5; ++iter, ++i)
    BOOST_TEST (*iter == i);
  for (; iter < v.begin () + 9; ++iter)
    BOOST_TEST (*iter == 6);
  for (; iter < v.end (); ++iter)
    BOOST_TEST (*iter == 1000);
  BOOST_TEST (v.size () == 15);
  BOOST_TEST (v.capacity () == 21);
  long *dynamic_vec = v.data ();
  pos = v.begin () + 1;
  for (int j = 0; j < 9; ++j)
    {
      BOOST_TEST (v.size () == 15 - j);
      BOOST_TEST (v.capacity () == 21);
      pos = v.erase (pos);
    }
  BOOST_TEST (v.size () == 6);
  BOOST_TEST (v.capacity () == 21);
  BOOST_TEST (v.data () == dynamic_vec);

  BOOST_TEST (*v.begin () == 0);
  iter = v.begin () + 1;
  for (; iter < v.end (); ++iter)
    BOOST_TEST (*iter == 1000);

  v.pop_back ();
  BOOST_TEST (v.size () == 5);
  BOOST_TEST (v.capacity () == 5);
  BOOST_TEST (v.data () != dynamic_vec);
  BOOST_TEST (v.data () == static_vec);

  BOOST_TEST (*v.begin () == 0);
  iter = v.begin () + 1;
  for (; iter < v.end (); ++iter)
    BOOST_TEST (*iter == 1000);
}

BOOST_AUTO_TEST_SUITE_END()
