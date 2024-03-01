// Matrix.h

#ifndef MATRIX_H
#define MATRIX_H

#include <ostream>
#include <istream>

/**
 * @struct matrix_dims
 * @brief Matrix dimensions container. Used in MlpNetwork.h and main.cpp
 */
typedef struct matrix_dims
{
    int rows, cols;
} matrix_dims;

// Insert Matrix class here...

class Matrix
{

private:
    struct matrix_dims _dims;
    float *_p_mat = nullptr;





public:

    Matrix(int rows, int cols);

    Matrix();

    Matrix(const Matrix& other);

    ~Matrix();


    int get_rows() const;

    int get_cols() const;

    Matrix& transpose(); 

    Matrix& vectorize(); 

    void plain_print();

    Matrix dot( Matrix& rhs);

    float norm(); 

    friend void  read_binary_file(std::istream &input_stream, Matrix& mat); 

    Matrix operator+(const Matrix& rhs);

    Matrix& operator=(Matrix rhs);

    Matrix operator*(const Matrix& rhs);

    Matrix operator*(float num) const ;



    friend Matrix operator*( float num ,const Matrix& rhs);

    Matrix& operator+=(const Matrix& rhs); 

    float& operator()(int i, int j);
    float operator()(int i, int j) const;

    float& operator[](int i);
    float operator[](int i) const;

    friend std::ostream& operator<<(std::ostream &output_stream,  Matrix &mat);


};
#endif //MATRIX_H




