#include "Sia_network.hpp"

Sia_output_matrix::Sia_output_matrix(Eigen::VectorXf& matrix) : OneD_output_matrix(matrix), TwoD_output_matrix(dummyTwoD), ThreeD_output_matrix(dummyThreeD) {
//Sets the type of input to single dimension
    matrix_type = ONE_D;
};
Sia_output_matrix::Sia_output_matrix(Eigen::MatrixXf& matrix) : TwoD_output_matrix(matrix), OneD_output_matrix(dummyOneD), ThreeD_output_matrix(dummyThreeD) {
//Sets the type of input to two dimensional
    matrix_type = TWO_D;
};
Sia_output_matrix::Sia_output_matrix(std::vector<Eigen::MatrixXf*> &matrix) : ThreeD_output_matrix(matrix), OneD_output_matrix(dummyOneD), TwoD_output_matrix(dummyTwoD) {
//Sets the type of input to three dimensional
    matrix_type = THREE_D;
};