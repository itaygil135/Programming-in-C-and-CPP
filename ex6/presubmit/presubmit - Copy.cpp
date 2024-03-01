#include <iostream>
#include <vector>
#include <algorithm>
#include "vl_vector.h"
#include "helpers.h"

#define BONUS_IMPLEMENTED 1

#ifdef BONUS_IMPLEMENTED
# include "vl_string.h"
#endif

#define PRESUBMISSION_ASSERT(testCase) __ASSERT_TEST("Presubmit", testCase)

//-------------------------------------------------------
// Tests
//-------------------------------------------------------

int TestCreateVector() {
  vl_vector<int, STATIC_CAP> int_vec{};
  vl_vector<std::string, STATIC_CAP> string_vec{};
  vl_vector<double, 4> double_vec{};

  ASSERT_VLA_PROPERTIES(int_vec, STATIC_CAP, 0)
  ASSERT_VLA_PROPERTIES(string_vec, STATIC_CAP, 0)
  ASSERT_VLA_PROPERTIES(double_vec, 4, 0)

  auto dynamic_allocated_vec = new vl_vector<int, STATIC_CAP>();
  delete dynamic_allocated_vec;

  return 1;
}

int TestPushBack() {
  vl_vector<int, STATIC_CAP> vec;
  ASSERT_VLA_PROPERTIES(vec, STATIC_CAP, 0)
  vec.push_back (1);
  ASSERT_VLA_PROPERTIES(vec, STATIC_CAP, 1)

  return 1;
}

int TestSize() {
  vl_vector<int, STATIC_CAP> vec;
  ASSERT_SIZE(vec, 0)

  for (int i = 0; i < 5; ++i) {
    vec.push_back (i);
  }

  ASSERT_SIZE(vec, 5)

  for (int i = 0; i < 15; ++i) {
    vec.push_back (i);
  }

  ASSERT_SIZE(vec, 20)
  return 1;
}

int TestCapacity() {
  vl_vector<int, STATIC_CAP> vec;
  ASSERT_CAPACITY(vec, STATIC_CAP)

  // Inserting [0, 15) (15 items)
  for (int i = 0; i < 15; ++i) {
    vec.push_back (i);
  }

  ASSERT_CAPACITY(vec, STATIC_CAP)

  // Inserting [15, 30) (15 items)
  for (int i = 15; i < 30; ++i) {
    vec.push_back (i);
  }

  ASSERT_CAPACITY(vec, 39)

  // Inserting [30, 39) (15 items)
  for (int i = 30; i < 39; ++i) {
    vec.push_back (i);
  }

  ASSERT_CAPACITY(vec, 39)
  return 1;
}

int TestEmpty() {
  vl_vector<int, STATIC_CAP> vec;
  ASSERT_TRUE(vec.empty())
  vec.push_back (1);
  RETURN_ASSERT_TRUE(!vec.empty())
}

int TestClear() {
  vl_vector<int, STATIC_CAP> vec;

  vec.push_back (1);
  vec.clear();

  ASSERT_SIZE(vec, 0)
  ASSERT_CAPACITY(vec, STATIC_CAP)

  return 1;
}

int TestPopBack() {
  vl_vector<int, STATIC_CAP> vec;
  if (!vec.empty()) {
    return 0;
  }

  vec.push_back (1);
  ASSERT_SIZE(vec, 1)
  ASSERT_CAPACITY(vec, STATIC_CAP)

  vec.pop_back();
  ASSERT_SIZE(vec, 0)
  ASSERT_CAPACITY(vec, STATIC_CAP)

  return 1;
}

int TestGetElement() {
  vl_vector<int, STATIC_CAP> vec;
  vec.push_back (1);

  ASSERT_TRUE(vec.at(0) == 1)  // using vl_vector::At
  RETURN_ASSERT_TRUE(vec[0] == 1) // using vl_vector::operator[]
}

int TestData() {
  vl_vector<int, STATIC_CAP> vec;
  UNUSED(vec.data());
  return 1;
}

int TestComparison() {
  vl_vector<int, STATIC_CAP> vec1{};
  vl_vector<int, STATIC_CAP> vec2{};

  RETURN_ASSERT_TRUE(vec1 == vec2)
}

int TestAssignment() {
  vl_vector<int, STATIC_CAP> vec1{};
  vec1.push_back (1);

  vl_vector<int, STATIC_CAP> vec2 = vec1;
  UNUSED(vec2);
  return 1;
}

int TestInsertSingleValue() {
  vl_vector<int, STATIC_CAP> vec;
  vec.push_back (0);
  vec.push_back (2);
  auto it = vec.begin();
  ++it;
  vec.insert(it, 1);

  RETURN_ASSERT_TRUE(vec[1] == 1)
}

int TestInsertIteratorInStaticCapacity() {
  vl_vector<int, STATIC_CAP> vec;
  vec.push_back (0);

  std::vector<int> values{1, 2, 3, 4};
  auto it = vec.begin();
  ++it;
  vec.insert(it, values.begin(), values.end());
  RETURN_ASSERT_TRUE(vec.size () == 5)
}

int TestInsertIteratorInDynamicCapacity() {
  vl_vector<int, STATIC_CAP> vec;
  std::vector<int> values(30UL);
  auto it = vec.begin();
  vec.insert(it, values.begin(), values.end());
  RETURN_ASSERT_TRUE(vec.size () == 30)
}

int TestResize() {
  vl_vector<int> vec;
  ASSERT_VLA_PROPERTIES(vec, STATIC_CAP, 0)

  // Add 1 item
  vec.push_back (1);
  ASSERT_VLA_PROPERTIES(vec, STATIC_CAP, 1)

  // Add 16 item
  for (int i = 2; i <= 17; ++i) {
    vec.push_back (i);
  }
  ASSERT_VLA_PROPERTIES(vec, /* capacity: */ 25, /* size: */17)

  // Add another 12 items
  std::vector<int> entries(13, 1);
  vec.insert(vec.begin(), entries.cbegin(), entries.cend());
  ASSERT_VLA_PROPERTIES(vec, /* capacity: */ 45, /* size: */30)

  // Remove those 12 items (note that the capacity won't be decreased).
  for (int i = 18; i <= 30; ++i) {
    vec.erase(vec.end() - 1);
  }
  ASSERT_VLA_PROPERTIES(vec, /* capacity: */ 45, /* size: */17)

  // Clear
  vec.clear();
  ASSERT_VLA_PROPERTIES(vec, /* capacity: */ STATIC_CAP, /* size: */0)

  // Adding 17 items (note that we have a capacity of 25 instead of 39, as we did Clear, which returned
  // us to static memory and free'd the dynamic memory).
  for (int i = 1; i <= 17; ++i) {
    vec.push_back (i);
  }
  ASSERT_VLA_PROPERTIES(vec, /* capacity: */ 25, /* size: */17)

  return 1;
}

int TestCheckIterator() {
  vl_vector<int>::iterator it;
  vl_vector<int>::const_iterator it2;

  vl_vector<int> vec1;
  vl_vector<int> const vec2;

  it = vec1.begin();
  it2 = vec2.begin();

  it = vec1.end();
  it2 = vec2.end();

  UNUSED(it);
  UNUSED(it2);

  return 1;
}

int TestCheckReverseIterator() {
  vl_vector<int>::reverse_iterator it;
  vl_vector<int>::const_reverse_iterator it2;

  vl_vector<int> vec1;
  vl_vector<int> const vec2;

  it = vec1.rbegin();
  it2 = vec2.crbegin();

  it = vec1.rend();
  it2 = vec2.crend();

  UNUSED(it);
  UNUSED(it2);

  return 1;
}

//-------------------------------------------------------
//  Bonus
//-------------------------------------------------------
#ifdef BONUS_IMPLEMENTED

template<size_t StaticCapacity = 16ul>
int VLStringEqual(const vl_string<StaticCapacity>& str, const char* expected_str, size_t expected_capacity) {
  // Check the VLA properties
  size_t expected_str_len = strlen(expected_str);
  ASSERT_VLA_PROPERTIES(str, expected_capacity, expected_str_len);

  // Make sure that the actual values corresponds to the string values
  for (size_t i = 0; i < expected_str_len; ++i) {
	ASSERT_TRUE(str[i] == expected_str[i])
  }

  return 1;
}

int TestVLStringCreateString() {
  vl_string<> str1;
  ASSERT_TRUE(str1.size() == 0 && str1.capacity () == STATIC_CAP)

  vl_string<> str2("Hello");
  ASSERT_TRUE(str2.size() == 5 && str2.capacity () == STATIC_CAP)

  vl_string<> str2_copy(str2);
  ASSERT_TRUE(str2_copy.size () == 5 && str2_copy.capacity () == STATIC_CAP)

  return 1;
}

int TestVLStringConcat() {
  vl_string<> str;
  ASSERT_TRUE(str.size () == 0 && str.capacity () == STATIC_CAP)

  str += "Hello";
  ASSERT_TRUE(str.size () == 5 && str.capacity () == STATIC_CAP)

  str += ", World from C++";
  ASSERT_TRUE(str.size () == 21 && str.capacity () == 33)

  str += '!';
  ASSERT_TRUE(str.size () == 22 && str.capacity () == 33)

  return 1;
}

int TestVLStringContentStack() {
  const vl_string<> str("Hello");
  const char* expected_str = "Hello";

  ASSERT_TRUE(VLStringEqual(str, expected_str, /* capacity: */ 16));

  return 1;
}

int TestVLStringContentHeap() {
  const vl_string<> str("Hello my name is ...");
  const char* expected_str = "Hello my name is ...";

  ASSERT_TRUE(VLStringEqual(str, expected_str, /* capacity: */ 31));

  return 1;
}

#endif

//-------------------------------------------------------
//  The main entry point
//-------------------------------------------------------
int RunPresubmitChecks() {
  PRESUBMISSION_ASSERT(TestCreateVector)
  PRESUBMISSION_ASSERT(TestPushBack)
  PRESUBMISSION_ASSERT(TestSize)
  PRESUBMISSION_ASSERT(TestCapacity)
  PRESUBMISSION_ASSERT(TestEmpty)
  PRESUBMISSION_ASSERT(TestClear)
  PRESUBMISSION_ASSERT(TestPopBack)
  PRESUBMISSION_ASSERT(TestGetElement)
  PRESUBMISSION_ASSERT(TestData)
  PRESUBMISSION_ASSERT(TestComparison)
  PRESUBMISSION_ASSERT(TestAssignment)
  PRESUBMISSION_ASSERT(TestInsertSingleValue)
  PRESUBMISSION_ASSERT(TestInsertIteratorInStaticCapacity)
  PRESUBMISSION_ASSERT(TestInsertIteratorInDynamicCapacity)
  PRESUBMISSION_ASSERT(TestResize)
  PRESUBMISSION_ASSERT(TestCheckIterator)
  PRESUBMISSION_ASSERT(TestCheckReverseIterator)

#ifdef BONUS_IMPLEMENTED
  PRESUBMISSION_ASSERT(TestVLStringCreateString)
  PRESUBMISSION_ASSERT(TestVLStringConcat)
  PRESUBMISSION_ASSERT(TestVLStringContentStack)
  PRESUBMISSION_ASSERT(TestVLStringContentHeap)
#endif

  return 1;
}

int main() {
  return RunPresubmitChecks() ? EXIT_SUCCESS : EXIT_FAILURE;
}