#include <Eigen/Dense>

#include <vector>
#include <cstdint>
#include <span>
#include <iostream>
#include <algorithm>

#include "Sia_common.hpp"

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
    uint64_t _ID = RANDOM_UINT64T();
    uint16_t _layer_type = layer_types::INPUT;

    Eigen::ArrayXf* _output;
    bool use_bias = true;
    bool use_weights = true;
    void (*_activation)(const Eigen::ArrayXf&);

public:
    Dense_layer(Layered_network& network) : _network(network) {};
    ~Dense_layer() {};
};

class Output_matrix {
protected:

    const Layered_network& _network;
    uint64_t _ID = RANDOM_UINT64T();
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
    uint64_t _ID = RANDOM_UINT64T();
    uint16_t _layer_type = layer_types::INPUT;

    const T& _input;

public:
    Input_matrix(const T& input, const Sia::Layered_network& network) : _input(input), _network(network) {};
    ~Input_matrix() {};

};

class Layered_network {
private:
// Output
    Eigen::ArrayXf _output;
// General network stuff ------------------------------------------------------------------------------------------------------------------
    std::vector<std::tuple<layer_types, uint64_t, uint64_t>> _layers; // (Type / unique ID / index [for the network])
    std::vector<std::pair<uint64_t, uint64_t>> _links; // (Two unique IDs that represent a link, first is head, second is tail)
    std::vector<Eigen::MatrixXf> _matricies; // Stores whatever matricies the network needs, layers-anything
    std::vector<Eigen::ArrayXf> _arrays;  // Stores whatever arrays the network needs, layers-anything

    std::vector<Eigen::MatrixXf> _map_matricies; // Matricies for the final map of the network
    std::vector<Eigen::ArrayXf> _map_arrays; // Array for the final map of the network
    std::vector<void (*)(const Eigen::ArrayXf&)> _map_functions; // Array functions for the final map of the network
// Input stuff ----------------------------------------------------------------------------------------------------------------------------
    matrix_types _input_settings; // Just saves whether it's a 1/2/3 dimensional input
// Dense stuff ----------------------------------------------------------------------------------------------------------------------------
    // (Layer width, activation function, activation function derivative, weights in relation to the layer above them, unique ID)
    std::vector<std::tuple<size_t, void (*)(const Eigen::ArrayXf&), void (*)(const Eigen::ArrayXf&), Eigen::MatrixXf, bool, bool, uint64_t>> _dense_settings; // 

    /* Cleans up all removed layers and settings, used
    before many other operations to make sure a clean
    slate and no weirdness, right now it just removes
    'empty' layers */
    void deleteLayers();


public:
    Layered_network() {
        _layers.reserve(10);
        _dense_settings.reserve(10);
    };
    ~Layered_network() {};

    template <is_valid T>
    void addLayer(const Input_matrix<T>& layer) {
        deleteLayers();
        for (auto layer_i : _layers){
            if(std::get<0>(layer_i) == INPUT) {
                std::cerr << "Only one input layer permitted... this has done nothing!\n";
                return;
            }
        }
        _layers.emplace_back(INPUT, (layer._ID), _layers.size());
    }
    
};

} // Namespace end scope

