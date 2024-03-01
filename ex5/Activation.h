//Activation.h
#ifndef ACTIVATION_H
#define ACTIVATION_H





#include "Matrix.h"
/**
 * @enum ActivationType
 * @brief Indicator of activation function.
 */


enum ActivationType
{
    RELU,
    SOFTMAX
};

// Insert Activation class here...

//The class Activation
//=========
//Constructor 
//------------



class Activation
{
private:
    ActivationType act_type;


    void relu_function(Matrix& mat, int index);
    void softmax_function(Matrix& mat, int index);



public:

    explicit Activation(ActivationType a_type);


    ActivationType get_activation_type();



    Matrix operator()(const Matrix& input);

    void softmax_fun(Matrix& matrix);
};
#endif //ACTIVATION_H
