#include <math.h>
#include "Activation.h"










void exp_sum_and_change_vec(Matrix& mat,  int vec_col,  int vec_len,
                             int mat_cols);

Activation::Activation(ActivationType a_type): act_type(a_type)
{

}

ActivationType Activation::get_activation_type()
{
    return this->act_type;
}


Matrix Activation::operator()(const Matrix& input)
{

    int cols = input.get_cols();
    Matrix new_mat(input);
    if (act_type == RELU)
    {
        for (int j = 0; j < cols; j++)
        {
            relu_function(new_mat, j);
        }
    }
    else if (act_type == SOFTMAX)
    {
        softmax_fun(new_mat);
//        for (int j = 0; j < cols; j++)
//        {
//            softmax_function(new_mat, j);
//        }
    }
    return new_mat;
}


void Activation::relu_function(Matrix &mat, int index)
{
    int rows = mat.get_rows(); int cols = mat.get_cols();
    for (int i = 0; i < rows; i++)
    {
        if (mat(i, index) < 0)
        {
            mat[i * cols + index] = 0;
        }
    }
}
void Activation::softmax_function(Matrix& mat, int index)
{
    int rows = mat.get_rows(); int cols = mat.get_cols();
    exp_sum_and_change_vec(mat, index, rows, cols);
}

void Activation::softmax_fun(Matrix &matrix) {

    float sum =0;
    int mat_dims = matrix.get_rows()*matrix.get_cols();
    for(int i = 0; i < mat_dims; i++)
    {
        sum += (float)std::exp (matrix[i]);
    }
    for (int j = 0; j < mat_dims; ++j) {
        matrix[j] = (float)std::exp (matrix[j]);
        matrix[j] /= sum;
    }
}

/**
 * @brief Calculates the exp sum for each coordinate and change each col by
 * the solution
 * @param mat
 * @param vecCol
 * @param vecLen
 * @param matCols
 */
void exp_sum_and_change_vec(Matrix& mat, const int vec_col, const int vec_len,
                        const int mat_cols)
{
    float sum = 0;
    for (int i = 0; i < vec_len; i++)
    {
        mat[i * mat_cols + vec_col] = std::exp(mat[i * mat_cols + vec_col]);
        sum += mat[i * mat_cols + vec_col];
    }
    float exp_sum = ((float)1) / sum;
    for (int i = 0; i < vec_len; i++)
    {
        mat[i * mat_cols + vec_col] = mat[i * mat_cols + vec_col] * exp_sum;
    }
}
