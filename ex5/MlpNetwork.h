//MlpNetwork.h


#ifndef MLPNETWORK_H
#define MLPNETWORK_H

#include "Matrix.h"
#include "Digit.h"
#include "Dense.h"

#define MLP_SIZE 4

//
const matrix_dims img_dims = {28, 28};
const matrix_dims weights_dims[] = {{128, 784},
                                    {64, 128},
                                    {20, 64},
                                    {10, 20}};
const matrix_dims bias_dims[]    = {{128, 1},
                                    {64, 1},
                                    {20, 1},
                                    {10, 1}};




// Insert MlpNetwork class here...
class MlpNetwork
{
private:
    Dense input_layer;
    Dense layer1;
    Dense layer2;
    Dense output_layer;

    int find_max(Matrix local_mat);
public:
    MlpNetwork(Matrix  weights[], Matrix bias[]);
    digit operator()( Matrix& mat);

};


#endif // MLPNETWORK_H



