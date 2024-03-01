#ifndef C___PROJECT_DENSE_H
#define C___PROJECT_DENSE_H



#include "Activation.h"




// implement class Dense here...

class Dense
{
private:
    Matrix weights;
    Matrix bias;
    Activation activation_obj;


public:

    //Constructor 
    Dense( const Matrix&  w,  const Matrix&  bias, ActivationType A_Type);


    Matrix get_weights();


    Matrix get_bias();

    Activation get_activation() const;

    Matrix operator()( Matrix& input_mat);


};

#endif //C___PROJECT_DENSE_H




