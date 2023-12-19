#pragma once
 
#include <memory>
#include <vector>
#include <variant>
#include <Eigen/Dense>

class Sia_layered_network;
class Base_layer {
    public:
    Base_layer(){};
    ~Base_layer(){};
};

class Sia_input_matrix : public Base_layer {
protected:
enum type_matrix {
        ONE_D,
        TWO_D,
        THREE_D,
    };
//Knows what type the input is, 1/2/3 dimensional matrices
    type_matrix matrix_type;

// Saves pointers to the passed matrices
    const Eigen::VectorXf* OneD_input_matrix;
    const Eigen::MatrixXf* TwoD_input_matrix;
    const std::vector<Eigen::MatrixXf*>* ThreeD_input_matrix;
public:

//Contructors for each type of matrix
    Sia_input_matrix(const Eigen::VectorXf& matrix);
    Sia_input_matrix(const Eigen::MatrixXf& matrix);
    Sia_input_matrix(const std::vector<Eigen::MatrixXf*>& matrix);
    Sia_input_matrix();

//Auto destructor (for now I guess)
    ~Sia_input_matrix();

//Prints all the values in the internally saved input matrix (Should just be a reference lol)
    void print_values();
};

class Sia_output_matrix : public Base_layer {
protected:
    const Eigen::VectorXf* OneD_output_matrix;
    const Eigen::MatrixXf* TwoD_output_matrix;
    const std::vector<Eigen::MatrixXf*>* ThreeD_output_matrix;
public:
    Sia_output_matrix();
    ~Sia_output_matrix();

};

class Sia_dense_layer : public Base_layer {
protected:
public:
    Sia_dense_layer();
    ~Sia_dense_layer();
};

class Sia_linear_layer :public  Base_layer {
protected:
public:
    Sia_linear_layer();
    ~Sia_linear_layer();
};

class Sia_convolution_layer : public Base_layer {
protected:
public:
    Sia_convolution_layer();
    ~Sia_convolution_layer();
};

class Sia_activation_layer : public Base_layer {
protected:
public:
    Sia_activation_layer();
    ~Sia_activation_layer();
};

class Sia_pooling_layer : public Base_layer {
protected:
public:
    Sia_pooling_layer();
    ~Sia_pooling_layer();
};

class Sia_normalization_layer : public Base_layer {
protected:
public:
    Sia_normalization_layer();
    ~Sia_normalization_layer();
};

class Sia_dropout_layer : public Base_layer {
protected:
public:
    Sia_dropout_layer();
    ~Sia_dropout_layer();
};
class Sia_layered_network
{

private:
    std::vector<std::unique_ptr<Base_layer>> layers;
public:

    ~Sia_layered_network();
    Sia_layered_network();

//Adds a layer from the type specified, and passes the arguments to the layer being constructed
    template<typename LayerType, typename... Args>
    LayerType& add_layer(Args&&... args){
//Makes a unique pointer for the specified type of layer, and fowards the arguments to the constructor
        std::unique_ptr<LayerType> temp_layer = std::make_unique<LayerType>(std::forward<Args>(args)...);
//Creates a reference to the layer just made
        LayerType& layer_ref = *temp_layer;
//"Moves" the layer into the vector of layers and tells the compiler to get rid of the pointer
        layers.push_back(std::move(temp_layer));
//Returns a reference to be saved by the user...
        return layer_ref;
    }
};