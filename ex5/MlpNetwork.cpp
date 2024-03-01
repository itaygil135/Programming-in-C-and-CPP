#include "MlpNetwork.h"



MlpNetwork::MlpNetwork(Matrix weights[], Matrix biases[])  /*
                                    Accepts 2 arrays, size 4 each.
                                    one for weights and one for biases.
                                    constructs the network described (sec. 2.2)
                                  */
        :
        input_layer(weights[0], biases[0], RELU),
        layer1(weights[1], biases[1], RELU),
        layer2(weights[2], biases[2], RELU),
        output_layer(weights[3], biases[3], SOFTMAX) {

}

digit MlpNetwork::operator()(Matrix &mat)  /* Parenthesis Applies the entire
   network on input
                        returns digit struct
                        MlpNetwork m(...);
                        Digit output = m(img);
                        */

{

    digit output;


    Matrix local_mat(mat);
    int index = 0;

    local_mat = input_layer(local_mat);
    local_mat = layer1(local_mat);
    local_mat = layer2(local_mat);
    local_mat = output_layer(local_mat);

    index = find_max(local_mat);
    output.value = index;
    output.probability = local_mat[index];

    return output;

}









int MlpNetwork::find_max(Matrix local_mat) {
    int i;
    int max_index = 0;
    int mat_size = local_mat.get_rows();
    for (i = 0; i < mat_size; i++) {
        if (local_mat[i] > local_mat[max_index]) {
            max_index = i;
        }
    }

    return max_index;
}


