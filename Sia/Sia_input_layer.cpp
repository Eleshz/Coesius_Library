#include "Sia_network.hpp"

#include <vector>
#include <iostream>
#include <Eigen/Dense>

Sia_input_matrix::Sia_input_matrix(const Eigen::VectorXf& matrix){
//Saves a reference to the matrix
    OneD_input_matrix = &matrix;
//Sets the type of input to single dimension
    matrix_type = ONE_D;
};
Sia_input_matrix::Sia_input_matrix(const Eigen::MatrixXf& matrix){
//Saves a reference to the matrix
    TwoD_input_matrix = &matrix;
//Sets the type of input to two dimensional
    matrix_type = TWO_D;
};
Sia_input_matrix::Sia_input_matrix(const std::vector<Eigen::MatrixXf*> &matrix){
//Saves a reference to the matrix
    ThreeD_input_matrix = &matrix;
//Sets the type of input to three dimensional
    matrix_type = THREE_D;
};
Sia_input_matrix::~Sia_input_matrix(){

};
void Sia_input_matrix::print_values(){
//What type am I printing??
    switch(matrix_type){
        case(ONE_D): {
//Prints one dimensional matrix
        const int size = OneD_input_matrix->size();
        for (int i = 0; i < size; ++i) {
        std::cout << *(OneD_input_matrix->data() + i) << " ";
            if ((i + 1) % 20 == 0) {
                std::cout << "continue..." << std::endl;
            };
        }

        }
        break;
        case(TWO_D): {
//Prints two dimensional matrix
        const int size = TwoD_input_matrix->size();
        const int columns = TwoD_input_matrix->cols();
        for (int i = 0; i < size; ++i) {
        std::cout << *(TwoD_input_matrix->data() + i) << " ";
            if ((i + 1) % columns == 0) {
                std::cout << std::endl;
            };
        }

        }
        break;
        case(THREE_D): {
//Prints three dimensional matrix
        const int size = ((ThreeD_input_matrix->at(0)->size())*(ThreeD_input_matrix->size()));
        const int sheets = ThreeD_input_matrix->size();
        const int sheet_size = ThreeD_input_matrix->at(0)->size();
        const int columns = ThreeD_input_matrix->at(0)->cols();
        for (int i = 0; i < sheets; ++i) {
            for (int j = 0; j < sheet_size; ++j) {
                std::cout << *(ThreeD_input_matrix->at(i)->data() + j) << " ";
                if ((j + 1) % columns == 0) {
                    std::cout << std::endl;
                };
            }
//If the "sheet" is done, prints this little message lol idk
            if (i<sheets-1){
                std::cout << "----------" << std::endl;
                std::cout << "NEXT SHEET" << std::endl;
                std::cout << "----------" << std::endl;
            }
        }

        }
        break;
    }
};