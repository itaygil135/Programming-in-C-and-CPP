#include "Matrix.h"
#include <fstream>
#include <cmath>
#include <cassert>
#include "Activation.h"

void chek_0 ()
{
  int rows = 3, cols = 5;
  Matrix m (rows, cols);
  assert(m.get_cols () == cols);
  assert(m.get_rows () == rows);
  for (int i = 0; i < rows; ++i)
    for (int j = 0; j < cols; ++j)
      {
        assert (&m[i * cols + j] == &m (i, j));
        assert(m[i * cols + j] == 0);
      }
}

void chek_1 ()
{
  Matrix m;
  assert(m.get_cols () == 1);
  assert(m.get_rows () == 1);
  assert(m[0] == 0);
}

void chek_2 ()
{
  int rows = 10, cols = 10;
  Matrix m (rows, cols);
  for (int i = 0; i < rows * cols; ++i)
    m[i] = (float) i;
  for (int i = 0; i < rows * cols; ++i)
    assert(m[i] == i);
  for (int i = 0; i < rows; ++i)
    for (int j = 0; j < cols; ++j)
      assert(m (i, j) == i * cols + j);
}

void chek_3 ()
{
  int rows = 4, cols = 7;
  Matrix to_copy (rows, cols);
  for (int i = 0; i < rows * cols; ++i)
    to_copy[i] = (float) 1 / (float) (i + 1);
  Matrix copied (to_copy);
  for (int i = 0; i < rows * cols; ++i)
    assert(copied[i] == (float) 1 / (i + 1));
  for (int i = 0; i < rows * cols; ++i)
    to_copy[i] += 1;
  for (int i = 0; i < rows * cols; ++i)
    assert(copied[i] == (float) 1 / (i + 1));
}

Matrix &randomly_fill_matrix (Matrix &m)
{
  for (int i = 0; i < m.get_rows () * m.get_cols (); ++i)
    m[i] = (float) rand () / (float) (rand () + 1);
  return m;
}

void chek_transopse ()
{
  int rows = 39, cols = 15;
  Matrix m (rows, cols);
  randomly_fill_matrix (m);
  Matrix m_transposed (m);
  m_transposed.transpose ();
  for (int i = 0; i < rows; ++i)
    for (int j = 0; j < cols; ++j)
      assert(m (i, j) == m_transposed (j, i));
  m_transposed.transpose ().transpose ().transpose ();
  for (int i = 0; i < rows; ++i)
    for (int j = 0; j < cols; ++j)
      assert(m (i, j) == m_transposed (i, j));
}

void chek_vectorize ()
{
  int rows = 39, cols = 15;
  Matrix m (rows, cols);
  randomly_fill_matrix (m);
  Matrix m_vectorized (m);
  m_vectorized.vectorize ();
  assert(m_vectorized.get_cols () == 1);
  assert(m_vectorized.get_rows () == rows * cols);
  for (int i = 0; i < rows * cols; ++i)
    assert(m[i] == m_vectorized[i]);
}

void chek_norm ()
{
  Matrix easy (1, 2);
  easy[0] = 3;
  easy[1] = 4;
  assert(easy.norm () == 5);
  int rows = 22, cols = 58;
  Matrix m (rows, cols);
  randomly_fill_matrix (m);
  float squared_norm = 0;
  for (int i = 0; i < rows * cols; ++i)
    squared_norm += m[i] * m[i];

  double norm = m.norm ();

  assert(m.norm () == sqrtf (squared_norm));
}
#define LIM 50

void chek_add ()
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
      assert(m_sum[i] == 1 / (i + 1));
      if (i % 2)
        {
          assert(m1[i] == 1 / (i + 1));
          assert(m2[i] == 0);
        }
      else
        {
          assert(m1[i] == 0);
          assert(m2[i] == 1 / (i + 1));
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
    assert(d[i] == (a[i] + b[i]) + c[i]);
}

void chek_another_add ()
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
      assert(m1[i] == 1 / (i + 1));
      if (i % 2)
        assert(m2[i] == 0);
      else
        assert(m2[i] == 1 / (i + 1));
    }
}

void chek_scalar ()
{
  int row = rand () % LIM, col = rand () % LIM;
  float a = 35.9306, b = -0.6235;
  Matrix m (row, col);
  randomly_fill_matrix (m);
  Matrix p = (a * m) * b;
  for (int i = 0; i < row * col; ++i)
    assert(p[i] == (a * m[i]) * b);
}

void chek_dot ()
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
      assert(a[i] == a_copy[i]);
      assert(b[i] == b_copy[i]);
      assert(c[i] == c_copy[i]);
      assert(product[i] == ((a_copy[i] * b_copy[i])) * c_copy[i]);
    }
}

void chek_mal_id ()
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
      assert(m[i] == m_copy[i]);
      assert(m_copy[i] == product[i]);
    }
}
void chek_mat_scalar_mult ()
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
    assert(lhs[i] == lhs_copy[i]);
  for (int i = 0; i < col * col; ++i)
    assert(rhs[i] == rhs_copy[i]);
  for (int i = 0; i < row * col; ++i)
    {
      assert(m[i] == m_copy[i]);
      assert((m_copy[i] * 2) * 0.5f == product[i]);
    }
}

void chek_general_mult ()
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
  assert(product.get_cols () == m2_cols);
  assert(product.get_rows () == m1_rows);
  for (int i = 0; i < m1_rows * m2_cols; ++i)
    assert(product[i] == expected_product_arr[i]);
}

void act_contr_chek ()
{
  Activation relu (RELU);
  assert(relu.get_activation_type () == RELU);
  Activation softmax (SOFTMAX);
  assert(softmax.get_activation_type () == SOFTMAX);
}

void act_relu_chek ()
{
  Activation relu (RELU);
  assert(relu.get_activation_type () == RELU);
  int row = 3, col = 5;
  Matrix a (row, col), b (row, col);
  for (int i = 0; i < row * col; ++i)
    b[i] = -1;
  Matrix relued_b = relu (b);
  for (int i = 0; i < row * col; ++i)
    assert(relued_b[i] == 0);
  for (int i = 0; i < row * col; ++i)
    assert(b[i] == -1);
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
        assert(relued[i] == 0);
      else
        assert(relued[i] == i + 3);
    }
}

void act_softmax_chek ()
{
  Activation softmax (SOFTMAX);
  assert(softmax.get_activation_type () == SOFTMAX);
  int row = 2, col = 2;
  Matrix a (row, col);
  for (int i = 0; i < row * col; ++i)
    a[i] = 1;
  Matrix easy = softmax (a);
  for (int i = 0; i < row * col; ++i)
    assert(easy[i] != a[i]);
  float sum = 0;
  for (int i = 0; i < row * col; ++i)
    {
      assert(easy[i] == 0.25);
      sum += easy[i];
    }
  assert(sum == 1);
  Matrix easy_again = softmax (easy);
  sum = 0;
  for (int i = 0; i < row * col; ++i)
    {
      assert(easy[i] == 0.25);
      sum += easy[i];
    }
  assert(sum == 1);
  row = 3;
  col = 2;
  Matrix b (row, col);
  for (int i = 0; i < row * col; ++i)
    b[i] = i - 3;
  Matrix softmaxed = softmax (b);
  sum = 0;
  for (int i = 0; i < row * col; ++i)
    sum += softmaxed[i];
  assert(sum == 1);
  sum = 0;
  for (int i = 0; i < row * col; ++i)
    sum += (float) std::exp ((float) (i - 3));
  for (int i = 0; i < row * col; ++i)
    {
      if (softmaxed[i] == ((float) std::exp (i - 3) * (1 / sum)))
        assert(softmaxed[i] == ((float) std::exp (i - 3) * (1 / sum)));
      else
        assert(softmaxed[i] == ((float) std::exp (i - 3) / sum));
    }
}

int main ()
{
  chek_0 ();
  chek_1 ();
  chek_2 ();
  chek_3 ();
  chek_transopse ();
  chek_vectorize ();
  chek_norm ();
  chek_add ();
  chek_another_add ();
  chek_scalar ();
  chek_dot ();
  chek_mal_id ();
  chek_mat_scalar_mult ();
  chek_general_mult ();

  act_contr_chek();
  act_relu_chek();
  act_softmax_chek();


  float m1[] = {10, 11, 12, 13, 14, 15, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9,
                9, 9, 9};

  float m0[] = {0, 1, 2, 3, 4, 5, 9, 9, 9,
                1, 2, 3, 4, 5, 6, 7, 8, 9,
                3, 4, 5, 6, 7, 8, 9, 10, 11};

  float m3[] = {1, 10,
                1, 132,
                1, 132,
                1, 1234,
                1, 123,
                7, 3,
                1, 13,
                1, 9,
                1, 10,};

//  Matrix matrix0 = Matrix (m0, 9, 3);
//
//  Matrix matrix3 = Matrix (m3, 2, 9);
//
//  Matrix mat = matrix0 * matrix3;
//
//  mat.plain_print ();
//
//  std::cout << std::endl;
//
//  mat.transpose ();
//
//  mat.plain_print ();

  return 0;
}