#pragma once

#include <Eigen/Dense>

#include <vector>
#include <cstdint>
#include <span>
#include <array>
#include <type_traits>
#include <iostream>
#include <algorithm>

#include "Sia_common.hpp"

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

    template<typename>
    struct is_std_array : std::false_type {};

    template<typename U, std::size_t N>
    struct is_std_array<std::array<U, N>> : std::true_type {};

    template <typename U>
    constexpr uint16_t deduce_input_type() {
        if constexpr (std::is_base_of<Eigen::DenseBase<std::decay_t<U>>, std::decay_t<U>>::value) {
            return 0;
        } else if constexpr (is_std_array<std::decay_t<U>>::value) {
            return 1;
        }
        return 2; 
    }

    friend Layered_network;
protected:

    const T& _input;

    const Layered_network& _network;
    const uint64_t _ID = RANDOM_UINT64T();
    const uint16_t _layer_type = layer_types::INPUT;
    const uint16_t _input_type; //Please don't change, will fix soon


public:
    Input_matrix(const T& input, const Sia::Layered_network& network) : _input(input), _network(network), _input_type(deduce_input_type<T>()) {};
    ~Input_matrix() {};

    uint64_t getID() {return _ID; };
    const void debugStatus(){
        bool was_synced = std::ios::sync_with_stdio();
        std::ios::sync_with_stdio(false);
        std::cout << "The ID of this layer is: " << _ID << "\n";
        switch (_input_type)
        {
        case (0):
            std::cout << "The type of this layer's input is a Eigen compatable type\n";
            break;
        case (1):
            std::cout << "The type of this layer's input is an std::array type, will be useable\n";
            break;
        case (2):
            std::cout << "The type of this layer's input is an unknown type, consider UB\n";
            break;
        default:
            break;
        }
        std::ios::sync_with_stdio(was_synced);
    }
};

class Dense_layer {
    friend Layered_network;
protected:

// Settings
    const Layered_network& _network;
    const uint64_t _ID = RANDOM_UINT64T();
    const uint16_t _layer_type = layer_types::INPUT;

    Eigen::ArrayXf* _output;
    const std::size_t _width;
    bool use_bias = true;
    bool use_weights = true;
    void (*_activation)(const Eigen::ArrayXf&);

public:
    Dense_layer(std::size_t width, Layered_network& network) : _width(width), _network(network) {};
    ~Dense_layer() {};

    uint64_t getID() {return _ID; };
};

class Output_matrix {
    friend Layered_network;
protected:

    const Layered_network& _network;
    const uint64_t _ID = RANDOM_UINT64T();
    const uint16_t _layer_type = layer_types::OUTPUT;


public:
    Output_matrix(Layered_network& network) : _network(network) {};
    ~Output_matrix() {};

    uint64_t getID() {return _ID; };

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
    // (Layer width, activation function, activation function derivative, using a bias, using the weights, unique ID)
    std::vector<std::tuple<size_t, void (*)(const Eigen::ArrayXf&), void (*)(const Eigen::ArrayXf&), bool, bool, uint64_t>> _dense_settings; // 

    /* Cleans up all removed layers and settings, used
    before many other operations to make sure a clean
    slate and no weirdness, right now it just removes
    'empty' layers */
    void deleteLayers();
    const bool existing_ID(const u_int64_t& id);

public:
    Layered_network() {

    };
    ~Layered_network() {};

    template <is_valid T>
    void addLayer(const Input_matrix<T>& layer) {
        deleteLayers();
        for (auto layer_i : _layers){
            if(existing_ID(layer._ID)) {
                std::cerr << "Only one input layer permitted... this has done nothing!\n";
                return;
            }
        }
        _layers.emplace_back(INPUT, (layer._ID), _layers.size()-1);
        _matricies[_layers.size()-1] = layer._input;
    }
    
};

} // Namespace end scope

