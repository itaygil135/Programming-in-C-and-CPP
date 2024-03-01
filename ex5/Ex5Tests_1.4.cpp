/*
 * C/C++ spring semester 2021
 * Ex5 tests
 * Written by Erel Debel
 */

#define BOOST_TEST_MODULE ex5_test_module
#include <boost/test/included/unit_test.hpp>
#include "Matrix.h"
#include "Activation.h"
#include <cmath>

#define LIM 50

/***************
* MATRIX TESTS *
***************/

BOOST_AUTO_TEST_SUITE (matrix_test_suite)

BOOST_AUTO_TEST_CASE (matrix_constructor)
{
  int rows = 3, cols = 5;
  Matrix m (rows, cols);
  BOOST_TEST (m.get_cols () == cols);
  BOOST_TEST (m.get_rows () == rows);
  for (int i = 0; i < rows; ++i)
    for (int j = 0; j < cols; ++j)
      {
        BOOST_TEST (&m[i * cols + j] == &m (i, j));
        BOOST_TEST (m[i * cols + j] == 0);
      }
}

BOOST_AUTO_TEST_CASE (matrix_default_constructor)
{
  Matrix m;
  BOOST_TEST (m.get_cols () == 1);
  BOOST_TEST (m.get_rows () == 1);
  BOOST_TEST (m[0] == 0);
}

BOOST_AUTO_TEST_CASE (matrix_indexing)
{
  int rows = 10, cols = 10;
  Matrix m (rows, cols);
  for (int i = 0; i < rows * cols; ++i)
    m[i] = (float) i;
  for (int i = 0; i < rows * cols; ++i)
    BOOST_TEST (m[i] == i);
  for (int i = 0; i < rows; ++i)
    for (int j = 0; j < cols; ++j)
      BOOST_TEST (m (i, j) == i * cols + j);
}

BOOST_AUTO_TEST_CASE (matrix_copy_constructor)
{
  int rows = 4, cols = 7;
  Matrix to_copy (rows, cols);
  for (int i = 0; i < rows * cols; ++i)
    to_copy[i] = (float) 1 / (float) (i + 1);
  Matrix copied (to_copy);
  for (int i = 0; i < rows * cols; ++i)
    BOOST_TEST(copied[i] == (float) 1 / (i + 1));
  for (int i = 0; i < rows * cols; ++i)
    to_copy[i] += 1;
  for (int i = 0; i < rows * cols; ++i)
    BOOST_TEST(copied[i] == (float) 1 / (i + 1));
}

BOOST_AUTO_TEST_CASE (matrix_assignment_operator)
{
  int rows = 4, cols = 7;
  Matrix to_copy (rows, cols);
  for (int i = 0; i < rows * cols; ++i)
    to_copy[i] = (float) 1 / (float) (i + 1);
  Matrix copied;
  copied = to_copy;
  for (int i = 0; i < rows * cols; ++i)
    BOOST_TEST(copied[i] == (float) 1 / (i + 1));
  for (int i = 0; i < rows * cols; ++i)
    to_copy[i] += 1;
  for (int i = 0; i < rows * cols; ++i)
    BOOST_TEST(copied[i] == (float) 1 / (i + 1));
}

Matrix &randomly_fill_matrix (Matrix &m)
{
  for (int i = 0; i < m.get_rows () * m.get_cols (); ++i)
    m[i] = (float) rand () / (float) (rand () + 1);
  return m;
}

BOOST_AUTO_TEST_CASE (matrix_transpose)
{
  int rows = 39, cols = 15;
  Matrix m (rows, cols);
  randomly_fill_matrix (m);
  Matrix m_transposed (m);
  m_transposed.transpose ();
  for (int i = 0; i < rows; ++i)
    for (int j = 0; j < cols; ++j)
      BOOST_TEST (m (i, j) == m_transposed (j, i));
  m_transposed.transpose ().transpose ().transpose ();
  for (int i = 0; i < rows; ++i)
    for (int j = 0; j < cols; ++j)
      BOOST_TEST (m (i, j) == m_transposed (i, j));
}

BOOST_AUTO_TEST_CASE (matrix_vectorize)
{
  int rows = 39, cols = 15;
  Matrix m (rows, cols);
  randomly_fill_matrix (m);
  Matrix m_vectorized (m);
  m_vectorized.vectorize ();
  BOOST_TEST (m_vectorized.get_cols () == 1);
  BOOST_TEST (m_vectorized.get_rows () == rows * cols);
  for (int i = 0; i < rows * cols; ++i)
    BOOST_TEST (m[i] == m_vectorized[i]);
}

BOOST_AUTO_TEST_CASE (matrix_norm)
{
  Matrix easy (1, 2);
  easy[0] = 3;
  easy[1] = 4;
  BOOST_TEST (easy.norm () == 5);
  int rows = 22, cols = 58;
  Matrix m (rows, cols);
  randomly_fill_matrix (m);
  float squared_norm = 0;
  for (int i = 0; i < rows * cols; ++i)
    squared_norm += m[i] * m[i];
  BOOST_TEST (m.norm () == sqrtf (squared_norm));
}

BOOST_AUTO_TEST_CASE (matrix_addition)
{
  int row = 7, col = 10;
  Matrix m1 (row, col);
  Matrix m2 (row, col);
  for (int i = 0; i < row * col; ++i)
    {
      if (i % 2)
        m1[i] = 1 / (i + 1);
      else
        m2[i] = 1 / (i + 1);
    }
  Matrix m_sum = m1 + m2;
  for (int i = 0; i < row * col; ++i)
    {
      BOOST_TEST (m_sum[i] == 1 / (i + 1));
      if (i % 2)
        {
          BOOST_TEST (m1[i] == 1 / (i + 1));
          BOOST_TEST (m2[i] == 0);
        }
      else
        {
          BOOST_TEST (m1[i] == 0);
          BOOST_TEST (m2[i] == 1 / (i + 1));
        }
    }
  row = rand () % LIM;
  col = rand () % LIM;
  Matrix a (row, col), b (row, col), c (row, col);
  randomly_fill_matrix (a);
  randomly_fill_matrix (b);
  randomly_fill_matrix (c);
  Matrix d = (a + b) + c;
  for (int i = 0; i < row * col; ++i)
    BOOST_TEST (d[i] == (a[i] + b[i]) + c[i]);
}

BOOST_AUTO_TEST_CASE (matrix_addition_accumulation)
{
  int row = 7, col = 10;
  Matrix m1 (row, col);
  Matrix m2 (row, col);
  for (int i = 0; i < row * col; ++i)
    {
      if (i % 2)
        m1[i] = 1 / (i + 1);
      else
        m2[i] = 1 / (i + 1);
    }
  m1 += m2;
  for (int i = 0; i < row * col; ++i)
    {
      BOOST_TEST (m1[i] == 1 / (i + 1));
      if (i % 2)
        BOOST_TEST (m2[i] == 0);
      else
        BOOST_TEST (m2[i] == 1 / (i + 1));
    }
}

BOOST_AUTO_TEST_CASE (matrix_scalar_multiplication)
{
  int row = rand () % LIM, col = rand () % LIM;
  float a = 35.9306, b = -0.6235;
  Matrix m (row, col);
  randomly_fill_matrix (m);
  Matrix p = (a * m) * b;
  for (int i = 0; i < row * col; ++i)
    BOOST_TEST (p[i] == (a * m[i]) * b);
}

BOOST_AUTO_TEST_CASE (matrix_dot_product)
{
  int row = rand () % LIM, col = rand () % LIM;
  Matrix a (row, col), b (row, col), c (row, col);
  randomly_fill_matrix (a);
  randomly_fill_matrix (b);
  randomly_fill_matrix (c);
  Matrix a_copy (a);
  Matrix b_copy (b);
  Matrix c_copy (c);
  Matrix product = a.dot (b).dot (c);
  for (int i = 0; i < row * col; ++i)
    {
      BOOST_TEST (a[i] == a_copy[i]);
      BOOST_TEST (b[i] == b_copy[i]);
      BOOST_TEST (c[i] == c_copy[i]);
      BOOST_TEST (product[i] == ((a_copy[i] * b_copy[i])) * c_copy[i]);
    }
}

BOOST_AUTO_TEST_CASE (matrix_identity_multiplication)
{
  int row = 5, col = 7;
  Matrix lhs (row, row);
  Matrix m (row, col);
  Matrix rhs (col, col);
  for (int i = 0; i < row; ++i)
    lhs (i, i) = 1;
  for (int i = 0; i < col; ++i)
    rhs (i, i) = 1;
  for (int i = 0; i < row * col; ++i)
    m[i] = (7 * i) / 3;
  Matrix lhs_copy (lhs);
  Matrix rhs_copy (rhs);
  Matrix m_copy (m);
  Matrix product = (lhs * m) * rhs;
  for (int i = 0; i < row * col; ++i)
    {
      BOOST_TEST (m[i] == m_copy[i]);
      BOOST_TEST (m_copy[i] == product[i]);
    }
}

BOOST_AUTO_TEST_CASE (matrix_scalar_matrix_multiplication)
{
  int row = 5, col = 7;
  Matrix lhs (row, row);
  Matrix m (row, col);
  Matrix rhs (col, col);
  for (int i = 0; i < row; ++i)
    lhs (i, i) = 2;
  for (int i = 0; i < col; ++i)
    rhs (i, i) = 0.5f;
  for (int i = 0; i < row * col; ++i)
    m[i] = (float) (7 * i) / 3;
  Matrix lhs_copy (lhs);
  Matrix rhs_copy (rhs);
  Matrix m_copy (m);
  Matrix product = (lhs * m) * rhs;
  for (int i = 0; i < row * row; ++i)
    BOOST_TEST (lhs[i] == lhs_copy[i]);
  for (int i = 0; i < col * col; ++i)
    BOOST_TEST (rhs[i] == rhs_copy[i]);
  for (int i = 0; i < row * col; ++i)
    {
      BOOST_TEST (m[i] == m_copy[i]);
      BOOST_TEST ((m_copy[i] * 2) * 0.5f == product[i]);
    }
}

BOOST_AUTO_TEST_CASE (matrix_general_multiplication)
{
  int m1_rows = 4, shared = 7, m2_cols = 2;
  float m1_arr[4 * 7] = {1231, 421, 42, 3242, 524, 36, 998,
                         1, 657, 6345, 322, 754, 324, 54,
                         6, 8, 3, 1, 0, 2, 42,
                         1, 55, 23, 729, 84, 12, 9};
  float m2_arr[7 * 2] = {425, 1,
                         33, 65,
                         7, 68,
                         23, 342,
                         34, 2,
                         25, 422,
                         2, 765};
  Matrix m1 (m1_rows, shared);
  Matrix m2 (shared, m2_cols);
  for (int i = 0; i < m1_rows * shared; ++i)
    m1[i] = m1_arr[i];
  for (int i = 0; i < shared * m2_cols; ++i)
    m2[i] = m2_arr[i];
  Matrix product = m1 * m2;
  float expected_product_arr[2 * 4] = {632640, 1919926,
                                       107771, 763836,
                                       2992, 34046,
                                       22342, 266575};
  BOOST_TEST (product.get_cols () == m2_cols);
  BOOST_TEST (product.get_rows () == m1_rows);
  for (int i = 0; i < m1_rows * m2_cols; ++i)
    BOOST_TEST (product[i] == expected_product_arr[i]);
}

BOOST_AUTO_TEST_SUITE_END ()


/*******************
* ACTIVATION TESTS *
*******************/

BOOST_AUTO_TEST_SUITE (activation_test_suite)

BOOST_AUTO_TEST_CASE (activation_constructors_and_type)
{
  Activation relu (RELU);
  BOOST_TEST (relu.get_activation_type () == RELU);
  Activation softmax (SOFTMAX);
  BOOST_TEST (softmax.get_activation_type () == SOFTMAX);
}

BOOST_AUTO_TEST_CASE (activation_relu)
{
  Activation relu (RELU);
  BOOST_TEST (relu.get_activation_type () == RELU);
  int row = 3, col = 5;
  Matrix a (row, col), b (row, col);
  for (int i = 0; i < row * col; ++i)
    b[i] = -1;
  Matrix relued_b = relu (b);
  for (int i = 0; i < row * col; ++i)
    BOOST_TEST (relued_b[i] == 0);
  for (int i = 0; i < row * col; ++i)
    BOOST_TEST (b[i] == -1);
  for (int i = 2; i < row * col; ++i)
    {
      a[i] = i + 3;
      if (i % 2 == 0 && i % 5 != 0)
        a[i] *= -1;
    }
  Matrix relued = relu (a);
  for (int i = 0; i < row * col; ++i)
    {
      if (i < 2 || (i % 2 == 0 && i % 5 != 0))
        BOOST_TEST (relued[i] == 0);
      else
        BOOST_TEST (relued[i] == i + 3);
    }
}

BOOST_AUTO_TEST_CASE (activation_softmax)
{
  Activation softmax (SOFTMAX);
  BOOST_TEST (softmax.get_activation_type () == SOFTMAX);
  int row = 2, col = 2;
  Matrix a (row, col);
  for (int i = 0; i < row * col; ++i)
    a[i] = 1;
  Matrix easy = softmax (a);
  for (int i = 0; i < row * col; ++i)
    BOOST_TEST (easy[i] != a[i]);
  float sum = 0;
  for (int i = 0; i < row * col; ++i)
    {
      BOOST_TEST (easy[i] == 0.25);
      sum += easy[i];
    }
  BOOST_TEST (sum == 1);
  Matrix easy_again = softmax (easy);
  sum = 0;
  for (int i = 0; i < row * col; ++i)
    {
      BOOST_TEST (easy[i] == 0.25);
      sum += easy[i];
    }
  BOOST_TEST (sum == 1);
  row = 3;
  col = 2;
  Matrix b (row, col);
  for (int i = 0; i < row * col; ++i)
    b[i] = i - 3;
  Matrix softmaxed = softmax (b);
  sum = 0;
  for (int i = 0; i < row * col; ++i)
    sum += softmaxed[i];
  BOOST_TEST (sum == 1);
  sum = 0;
  for (int i = 0; i < row * col; ++i)
    sum += (float) std::exp ((float) (i - 3));
  for (int i = 0; i < row * col; ++i)
    {
      if (softmaxed[i] == ((float) std::exp (i - 3) * (1 / sum)))
        BOOST_TEST (softmaxed[i] == ((float) std::exp (i - 3) * (1 / sum)));
      else
        BOOST_TEST (softmaxed[i] == ((float) std::exp (i - 3) / sum));
    }
}

BOOST_AUTO_TEST_SUITE_END ()
