#include <Eigen/Dense>

#include <vector>

#pragma once

enum layer_types {
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
public:
    Dense_layer() {};
    ~Dense_layer() {};
};

class Linear_layer {
protected:
public:
    Linear_layer() {};
    ~Linear_layer() {};
};

class Convolution_layer {
protected:
public:
    Convolution_layer() {};
    ~Convolution_layer() {};
};

class Activation_layer {
protected:
public:
    Activation_layer() {};
    ~Activation_layer() {};
};

class Pooling_layer {
protected:
public:
    Pooling_layer() {};
    ~Pooling_layer() {};
};

class Normalization_layer {
protected:
public:
    Normalization_layer() {};
    ~Normalization_layer() {};
};

class Dropout_layer {
protected:
public:
    Dropout_layer() {};
    ~Dropout_layer() {};
};

class Output_matrix {
protected:
public:
    Output_matrix() {};
    ~Output_matrix() {};

};

class Layered_network
{
private:

    std::vector<layer_types> _Layer_types;

public:
    Layered_network() {};
    ~Layered_network() {};

    
};

} // Namespace

