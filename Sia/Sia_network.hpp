#include <Eigen/Dense>

#include <vector>

#include "Sia_compile_random.hpp"

#pragma once

class Layered_network;

enum layer_types {
    INPUT = 0,
    OUTPUT,
    DENSE
};

namespace Sia {

class Dense_layer {
protected:

    const Layered_network& network;
    long unsigned int _ID = DYC_RAND_NEXT;
    int _layer_type = layer_types::INPUT;

public:
    Dense_layer() {};
    ~Dense_layer() {};
};

class Output_matrix {
protected:

    long unsigned int _ID = DYC_RAND_NEXT;
    int _layer_type = layer_types::OUTPUT;


public:
    Output_matrix() {};
    ~Output_matrix() {};

};

class Input_matrix {
protected:

    long unsigned int _ID = DYC_RAND_NEXT;
    int _layer_type = layer_types::INPUT;


public:
    Input_matrix() {};
    ~Input_matrix() {};

};

class Layered_network
{
private:

    Eigen::VectorXf _output; 

public:
    Layered_network() {};
    ~Layered_network() {};

    
};

} // Namespace end scope

