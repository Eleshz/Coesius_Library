#include <Eigen/Dense>

#include <vector>

#include "Sia_compile_random.hpp"

#pragma once

enum layer_types {
    INPUT,
    DENSE,
    LINEAR,
    CONVOLUTION,
    ACTIVATION,
    POOLING,
    NORMALIZATION,
    DROPOUT,
    OUTPUT
};

namespace Sia {

class Dense_layer {
protected:

    long unsigned int _ID = DYC_RAND_NEXT;
    int _layer_type = layer_types::INPUT;

public:
    Dense_layer() {};
    ~Dense_layer() {};
};

class Linear_layer {
protected:

    long unsigned int _ID = DYC_RAND_NEXT;
    int _layer_type = layer_types::LINEAR;

public:
    Linear_layer() {};
    ~Linear_layer() {};
};

class Convolution_layer {
protected:

    long unsigned int _ID = DYC_RAND_NEXT;
    int _layer_type = layer_types::CONVOLUTION;

public:
    Convolution_layer() {};
    ~Convolution_layer() {};
};

class Activation_layer {
protected:

    long unsigned int _ID = DYC_RAND_NEXT;
    int _layer_type = layer_types::ACTIVATION;

public:
    Activation_layer() {};
    ~Activation_layer() {};
};

class Pooling_layer {
protected:

    long unsigned int _ID = DYC_RAND_NEXT;
    int _layer_type = layer_types::POOLING;

public:
    Pooling_layer() {};
    ~Pooling_layer() {};
};

class Normalization_layer {
protected:

    long unsigned int _ID = DYC_RAND_NEXT;
    int _layer_type = layer_types::NORMALIZATION;

public:
    Normalization_layer() {};
    ~Normalization_layer() {};
};

class Dropout_layer {
protected:

    long unsigned int _ID = DYC_RAND_NEXT;
    int _layer_type = layer_types::DROPOUT;

public:
    Dropout_layer() {};
    ~Dropout_layer() {};
};

class Output_matrix {
protected:

    long unsigned int _ID = DYC_RAND_NEXT;
    int _layer_type = layer_types::OUTPUT;

public:
    Output_matrix() {};
    ~Output_matrix() {};

};

template<typename T>
concept input_output_restrict = std::is_same_v<T, int> || std::is_same_v<T, double>;

class Layered_network
{
private:


public:
    Layered_network() {};
    ~Layered_network() {};

    
};

} // Namespace end scope

