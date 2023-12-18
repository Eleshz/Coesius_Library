#include "Sia_network.hpp"
#include <iostream>

int main(){
    Eigen::MatrixXf sheet_one(2,2);
    sheet_one << 1, 2, 3, 4;
    Eigen::MatrixXf sheet_two(2,2);
    sheet_two << 1, 2, 3, 4;
    Eigen::MatrixXf sheet_three(2,2);
    sheet_three << 1, 2, 3, 4;
    std::vector<Eigen::MatrixXf*> input;
    input.push_back(&sheet_one);
    input.push_back(&sheet_two);
    input.push_back(&sheet_three);
    Sia_layered_network network;
    auto& input_layer = network.add_layer<Sia_input_matrix>(input);
    input_layer.print_values();

    // Clear the matrices
    sheet_one.setZero();
    sheet_two.setZero();
    sheet_three.setZero();

    // Add new values
    sheet_one << 5, 6, 7, 8;
    sheet_two << 9, 10, 11, 12;
    sheet_three << 13, 14, 15, 16;

    std::cout << std::endl;

    // Print the matrices again
    input_layer.print_values();

    return 0;
}
