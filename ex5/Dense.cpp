#include "Dense.h"
#include "Activation.h"







Dense::Dense( const Matrix&  w,  const Matrix&  bias, ActivationType A_Type):
    weights(w),
    bias(bias),
    activation_obj(Activation(A_Type))
{}

Matrix Dense::get_weights()
{
    return this->weights;
}



Matrix Dense::get_bias()
{
    return this->bias;
}

Activation Dense::get_activation() const
{
    return this->activation_obj;
}


Matrix Dense::operator()( Matrix& input_mat)
{
    Matrix result;

    result = weights * input_mat;
    result += bias;



    result = activation_obj(result);


    return result;
}



