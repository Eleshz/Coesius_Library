#include "Sia_network.hpp"

Sia_output_matrix::Sia_output_matrix(const Eigen::VectorXf& matrix){
//Saves a reference to the matrix
    OneD_output_matrix = &matrix;
//Sets the type of input to single dimension
    matrix_type = ONE_D;
};
Sia_output_matrix::Sia_output_matrix(const Eigen::MatrixXf& matrix){
//Saves a reference to the matrix
    TwoD_output_matrix = &matrix;
//Sets the type of input to two dimensional
    matrix_type = TWO_D;
};
Sia_output_matrix::Sia_output_matrix(const std::vector<Eigen::MatrixXf*> &matrix){
//Saves a reference to the matrix
    ThreeD_output_matrix = &matrix;
//Sets the type of input to three dimensional
    matrix_type = THREE_D;
};