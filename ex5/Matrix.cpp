#include <iostream>
#include <fstream>
#include <math.h>
#include "Matrix.h"


#define REQUIRED_SPACE 0.1
#define SPACES "  "
#define ASTERISKS "**"



/*
 * @brief  construct a new matrix at a given cimensions, 
           init all elements to 0
 * @param  rows - number of rows a the new matrix
 * @param  cols - number of columns a the new matrix
 * @return none
 */
Matrix::Matrix(int rows, int cols) {
    if ((rows <= 0) || (cols <= 0)) {
        std::cerr << "Error: wrong dims" << std::endl;
        exit(EXIT_FAILURE);
    }
    _dims.rows = rows;
    _dims.cols = cols;

    _p_mat = new(std::nothrow) float[rows * cols]{0};
    if (_p_mat == nullptr) {
        std::cerr << "Error: memory allocation failure" << std::endl;
        exit(EXIT_FAILURE);
    }
//    int mat_size = cols * rows;
//    for (int i = 0; i < mat_size; i++)
//    {
//        _p_mat[i] = 0;
//    }

}

/*
 * @brief  construct a new matrix at a given cimensions 1*1, 
           init all elements to 0
 * @param  none
 * @return none
 */
Matrix::Matrix() : Matrix(1, 1) {}

/*
 * @brief  construct a new matrix as a copy of a given matrix
 * @param  other - the other matrix to copy from 
 * @return none
 */
Matrix::Matrix(const Matrix &other) : Matrix(other._dims.rows,
                                             other._dims.cols) {
    int mat_size = _dims.cols * _dims.rows;
    for (int i = 0; i < mat_size; i++) {
//        _p_mat[i] = 0;
        _p_mat[i] = other._p_mat[i];
    }
}




/*
 * @brief  desstruct the matrix object and free the memory
 * @param  none
 * @return none
 */
Matrix::~Matrix() {
    delete[] _p_mat;
}

/*
 * @brief  return the number of the rows of the matrix
 * @param  none
 * @return the number of the rows of the matrix
 */
int Matrix::get_rows() const {
    return _dims.rows;
}

/*
 * @brief  return the number of the cols of the matrix
 * @param  none
 * @return the number of the cols of the matrix
 */
int Matrix::get_cols() const {
    return _dims.cols;
}

/*
 * @brief  Transforms a matrix into its transpose matrix.
 * @param  none
 * @return ref to the matrix
 */





Matrix &Matrix::transpose() {
    int mat_size = _dims.rows * _dims.cols;
    int new_rows = _dims.cols;
    int new_cols = _dims.rows;
    auto *p_new_mat_elements = new(std::nothrow) float[mat_size];
    if (p_new_mat_elements == nullptr) {
        std::cerr << "Error: memory allocation failure" << std::endl;
        exit(EXIT_FAILURE);
    }
    int old_r = 0, old_c = 0, new_index = 0;
    for (int i = 0; i < mat_size; i++) {
        old_r = i / _dims.cols;
        old_c = i % _dims.cols;
        new_index = old_c * _dims.rows + old_r;
        p_new_mat_elements[new_index] = _p_mat[i];
    }

    for (int i = 0; i < mat_size; i++) {
        _p_mat[i] = p_new_mat_elements[i];
    }

    delete[] p_new_mat_elements;
    _dims.cols = new_cols;
    _dims.rows = new_rows;
    return *this;
}


/*
 * @brief  Transforms a matrix into a column vector.
 * @param  none
 * @return ref to the matrix
 */
Matrix &Matrix::vectorize() {
    _dims.rows = _dims.rows * _dims.cols;
    _dims.cols = 1;
    return *this;
}

/*
 * @brief   Prints matrix elements .
 * @param  none
 * @return none
 */
void Matrix::plain_print() {
    for (int i = 0; i < _dims.rows; i++) {
        for (int j = 0; j < _dims.cols; j++) {
            std::cout << _p_mat[(i * _dims.cols) + j] << " ";
        }
        std::cout << std::endl;
    }
}

/*
 * @brief  calculate a dot product of this matrix and another matrix
 * @param  the other matrix
 * @return the new dot matrix
 */
Matrix Matrix::dot(Matrix &rhs) {
    if (_dims.rows != rhs._dims.rows || _dims.cols != rhs._dims.cols) {
        std::cerr << "Error: different dims for the two matrixes" << std::endl;
        exit(EXIT_FAILURE);
    }

    Matrix new_mat = Matrix(_dims.rows, _dims.cols);
    int mat_size = _dims.rows * _dims.cols;
    for (int i = 0; i < mat_size; i++) {
        new_mat._p_mat[i] = _p_mat[i] * rhs._p_mat[i];
    }

    return new_mat;
}

/*
 * @brief  calculate robenius norm of the given matrix
 * @param  none
 * @return the calculated norm
 */
float Matrix::norm() {
    float result = 0;
    int mat_size = _dims.rows * _dims.cols;
    for (int i = 0; i < mat_size; i++) {
        result += _p_mat[i] * _p_mat[i];
    }
    result = std::sqrt(result);
    return result;
}

/*
 * @brief  Read binary file an fills matrix elements 
 * @param  input_stream - the binary file (istream) to read from 
 * @param  mat - the matrix to fill in.
 * @return the calculated norm
 */
void read_binary_file(std::istream &input_stream, Matrix &mat) {
    int bytes_to_read = mat._dims.rows * mat._dims.cols * sizeof(float);
    input_stream.read((char *) mat._p_mat, bytes_to_read);

    if (!input_stream.good()) {
        std::cerr << "Error: read error" << std::endl;
        exit(EXIT_FAILURE);
    }

}


/*
 * @brief  calculate an addition matrix of this matrix and another matrix
 * @param  the other matrix
 * @return the new addition matrix
 */
Matrix Matrix::operator+(const Matrix &rhs) {
    if (_dims.rows != rhs._dims.rows || _dims.cols != rhs._dims.cols) {
        std::cerr << "Error: different dims for the two matrixes" << std::endl;
        exit(EXIT_FAILURE);
    }

    Matrix new_mat = Matrix(_dims.rows, _dims.cols);
    int mat_size = _dims.rows * _dims.cols;
    for (int i = 0; i < mat_size; i++) {
        new_mat._p_mat[i] = _p_mat[i] + rhs._p_mat[i];
    }

    return new_mat;
}

/*
 * @brief  Assign the rhs matrix to this matrix 
 * @param  the rhs matrix
 * @return the new assigned matrix
 */
Matrix &Matrix::operator=(Matrix rhs)  /* Assignment Matrix a, b;
                                ...
                             a = b; */
{
    if (this != &rhs) {
        delete[] _p_mat;
        _dims.rows = rhs.get_rows();
        _dims.cols = rhs.get_cols();
        int mat_size = _dims.rows * _dims.cols;
        _p_mat = new(std::nothrow) float[mat_size];
        if (_p_mat == nullptr) {
            std::cerr << "Error: memory allocation failure" << std::endl;
            exit(EXIT_FAILURE);
        }
        for (int i = 0; i < mat_size; i++) {
            _p_mat[i] = rhs._p_mat[i];
        }
    }
    return *this;
}

/*
 * @brief  Mulltiple the rhs matrix to this matrix
 * @param  the rhs matrix
 * @return the new assigned matrix
 */
Matrix Matrix::operator*(const Matrix &rhs)   /*    multiplication
                        Matrix a, b; -> a * b  */
{
    if (_dims.cols != rhs.get_rows()) {
        std::cerr << "Error: wrong dims" << std::endl;
        exit(EXIT_FAILURE);
    }


    int new_rows = _dims.rows;
    int new_cols = rhs.get_cols(); // rows from l cols from r
    int common_val = _dims.cols;

    Matrix multiplication_mat(new_rows, new_cols);

    for (int i = 0; i < new_rows; i++) {
        for (int j = 0; j < new_cols; j++) {
            for (int k = 0; k < common_val; k++) {
                multiplication_mat[i * new_cols + j] +=
                        _p_mat[i * common_val + k] *
                        rhs._p_mat[j + k * new_cols];
            }
        }
    }
    return multiplication_mat;

}

/*
 * @brief  multiplication a scalar on the right Matrix
 * @param  the scalr
 * @return the new matrix
 */
//Matrix Matrix::operator* (float scalar) const
Matrix Matrix::operator*(float num) const {
    int mat_size = 0;


    mat_size = _dims.rows * _dims.cols;
//    Matrix new_mat(*this);
    Matrix new_mat(_dims.rows,_dims.cols);
    for (int i = 0; i < mat_size; i++) {
        new_mat._p_mat[i] = _p_mat[i] * (float) num;
    }
    return new_mat;
}

/*
 * @brief  multiplication a scalar on the lef Matrix 
 * @param  the
 * @return the new matrix
 */


//Matrix operator* (float left_scalar, const Matrix &right_mut)
Matrix operator*(float num, const Matrix &rhs) {
    Matrix new_mat(rhs.get_rows(), rhs.get_cols());
    int mat_size = rhs.get_rows() * rhs.get_cols();
    for (int i = 0; i < mat_size; i++) {
        new_mat._p_mat[i] = rhs._p_mat[i] * (float) num;
    }
    return new_mat;

//    Matrix new_mat(this->get_rows(),this->get_cols());
//    int mat_size = this->get_rows() * this->get_cols();
//    for (int i = 0 ; i < mat_size; i++)
//    {
//        new_mat(i) = *this(i) * num;
//    }
//    return new_mat;

}

/*
 * @brief  addition of another matrix to this Matrix
 * @param  the other
 * @return this matirx, after the addition
 */
Matrix &Matrix::operator+=(const Matrix &rhs) {
    if (_dims.rows != rhs._dims.rows || _dims.cols != rhs._dims.cols) {
        std::cerr << "Error: different dims for the two matrixes" << std::endl;
        exit(EXIT_FAILURE);
    }

    int mat_size = _dims.rows * _dims.cols;
    for (int i = 0; i < mat_size; i++) {
        _p_mat[i] = _p_mat[i] + rhs._p_mat[i];
    }

    return *this;
}



/*
 * @brief  Parenthesis indexing For i, j indices
 * @param  none
 * @return the i, j element in the matrix 
 */
float &Matrix::operator()(int i, int j) {
    if ((i >= _dims.rows) || (j > _dims.cols) || (i < 0) || (j < 0)) {
        std::cerr << "Error: wrong index" << std::endl;
        exit(EXIT_FAILURE);
    }
    return _p_mat[i * _dims.cols + j];
}

float Matrix::operator()(int i, int j) const {
    if ((i >= _dims.rows) || (j > _dims.cols) || (i < 0) || (j < 0)) {
        std::cerr << "Error: wrong index" << std::endl;
        exit(EXIT_FAILURE);
    }
    return _p_mat[i * _dims.cols + j];
}


/*
 * @brief  Brackets indexing For i index,
 * @param  none
 * @return the i'th element in the matrix
 */
float &Matrix::operator[](int i) {
    if ((i >= (_dims.rows * _dims.cols)) || (i < 0)) {
        std::cerr << "Error: wrong index" << std::endl;
        exit(EXIT_FAILURE);
    }
    return _p_mat[i];
}

float Matrix::operator[](int i) const {
    if ((i >= (_dims.rows * _dims.cols)) || (i < 0)) {
        std::cerr << "Error: wrong index" << std::endl;
        exit(EXIT_FAILURE);
    }
    return _p_mat[i];
}

/*
 * @brief  Brackets indexing For i index,
 * @param  none
 * @return the i'th element in the matrix
 */
std::ostream &operator<<(std::ostream &output_stream, Matrix &mat) {
    for (int i = 1; i < mat.get_rows(); ++i) {
        for (int j = 1; j < mat.get_cols(); ++j) {
            if (mat(i, j) >= REQUIRED_SPACE) {
                output_stream << SPACES;
            } else {
                output_stream << ASTERISKS;
            }
        }
        output_stream << std::endl;
    }

    return output_stream;
}

Matrix &randomly_fill_matrix (Matrix &m)
{
    for (int i = 0; i < m.get_rows () * m.get_cols (); ++i)
        m[i] = (float) rand () / (float) (rand () + 1);
    return m;
}



//
//void func()
//{
//    Matrix my_matrix(3, 4);
//    std::ifstream is;
//    read_binary_file(is, my_matrix);
//    my_matrix.plain_print();
//    my_matrix.transpose();
//    my_matrix.plain_print();
//    my_matrix.vectorize();
//    my_matrix.plain_print();
//
//}



