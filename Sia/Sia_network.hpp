#include <Eigen/Dense>

#include <vector>
#include <cstdint>
#include <span>

#include "Sia_compile_random.hpp"

#pragma once

enum layer_types {
    INPUT = 0,
    OUTPUT,
    DENSE
};

enum matrix_types {
    ONE_D = 0,
    TWO_D,
    THREE_D
};

namespace Sia { // Start namespace scope

class Layered_network;

template <std::size_t N>
class Dense_layer {
protected:

    const Layered_network& _network;
    uint64_t _ID = DYC_RAND_NEXT;
    uint16_t _layer_type = layer_types::INPUT;

    Eigen::ArrayXf* _output;
    bool use_bias = true;
    void (*)() _activation;

public:
    Dense_layer(Layered_network& network) : _network(network) {};
    ~Dense_layer() {};
};

class Output_matrix {
protected:

    const Layered_network& _network;
    uint64_t _ID = DYC_RAND_NEXT;
    uint16_t _layer_type = layer_types::OUTPUT;


public:
    Output_matrix(Layered_network& network) : _network(network) {};
    ~Output_matrix() {};

};

template <typename T>
concept is_fixed_size_eigen_matrix_or_vector = requires(T a) {
    { a.rows() } -> std::same_as<std::ptrdiff_t>;
    { a.cols() } -> std::same_as<std::ptrdiff_t>;
    { a(0, 0) } -> std::same_as<float&>;
    requires T::RowsAtCompileTime != Eigen::Dynamic;
    requires T::ColsAtCompileTime != Eigen::Dynamic;
};

template <typename T>
concept is_valid = requires(T a) {
    { a.size() } -> std::same_as<std::size_t>;
    { a[0] } -> std::same_as<float&>;
} || is_fixed_size_eigen_matrix_or_vector<T>;


template <is_valid T>
class Input_matrix {
    friend Layered_network;
protected:

    const Layered_network& _network;
    uint64_t _ID = DYC_RAND_NEXT;
    uint16_t _layer_type = layer_types::INPUT;

    const T& _input;

public:
    Input_matrix(const T& input, const Sia::Layered_network& network) : _input(input), _network(network) {};
    ~Input_matrix() {};

};

class Layered_network {
private:
// Output
    Eigen::VectorXf _output;
// General network stuff ------------------------------------------------------------------------------------------------------------------
    std::vector<std::tuple<layer_types, uint64_t, uint64_t>> _layers; // (Type / unique ID / index [for the network])
    std::vector<std::pair<uint64_t, uint64_t>> _links; // (Two unique IDs that represent a link, first is head, second is tail)
    std::vector<Eigen::MatrixXf> _matricies; // Stores whatever matricies the network needs, layers-anything
    std::vector<Eigen::ArrayXf> _arrays;  // Stores whatever arrays the network needs, layers-anything
// Input stuff ----------------------------------------------------------------------------------------------------------------------------
    matrix_types _input_settings; // Just saves whether it's a 1/2/3 dimensional input
// Dense stuff ----------------------------------------------------------------------------------------------------------------------------
    std::vector<std::tuple<size_t, void (*)(), uint64_t>> _dense_settings; // (Layer width, activation function, unique ID)
    std::vector<Eigen::MatrixXf> _dense_weights; // Saves the weights in relation to the layer above them, as you should lol
    std::vector<void (*)()> _activation_derivatives; // Self explanitory...


public:
    Layered_network() {};
    ~Layered_network() {};

    
};

} // Namespace end scope

