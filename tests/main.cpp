#include <sia/sia_lib.hpp>

#include <array>
#include <iostream>

int main(){

    Eigen::Array<float, 1, 3> eigenMatrix1D;
    eigenMatrix1D << 1.0, 2.0, 3.0; // cppcheck-suppress constStatement // TESTING

    Eigen::Array<float, 3, 3> eigenMatrix2D;
    eigenMatrix2D << 1.0, 2.0, 3.0, 4.0, 5.0, 6.0, 7.0, 8.0, 9.0; // cppcheck-suppress constStatement // TESTING

    std::array<Eigen::Matrix<float, 3, 3>, 3> eigenMatrix3D;
    eigenMatrix3D[0] << 1.0, 2.0, 3.0, 4.0, 5.0, 6.0, 7.0, 8.0, 9.0; // cppcheck-suppress constStatement // TESTING
    eigenMatrix3D[1] << 1.0, 2.0, 3.0, 4.0, 5.0, 6.0, 7.0, 8.0, 9.0; // cppcheck-suppress constStatement // TESTING
    eigenMatrix3D[2] << 1.0, 2.0, 3.0, 4.0, 5.0, 6.0, 7.0, 8.0, 9.0; // cppcheck-suppress constStatement // TESTING

    Sia::Layered_network network;
    Sia::Input_matrix input(eigenMatrix3D, network);

    network.add_layer(input);

    input.debugStatus();
    
    return 0;
}