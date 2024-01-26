#pragma once

#include <Eigen/Dense>

#include <vector>
#include <cstdint>
#include <span>
#include <array>
#include <type_traits>
#include <iostream>
#include <algorithm>

#include "Sia_general.ipp"

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

namespace INTERNAL{

template <typename T>
concept eigen_fixed_1D = requires(T t) {
    { t.size() } -> std::same_as<std::ptrdiff_t>;
    { t(0) } -> std::same_as<typename T::Scalar&>;
    { t(0) } -> std::same_as<float&>;
    requires T::SizeAtCompileTime != Eigen::Dynamic;
    requires std::is_base_of_v<Eigen::DenseBase<T>, T> && T::RowsAtCompileTime == 1;
};

template <typename T>
concept eigen_fixed_2D = requires(T t) {
    { t.rows() } -> std::same_as<std::ptrdiff_t>;
    { t.cols() } -> std::same_as<std::ptrdiff_t>;
    { t(0, 0) } -> std::same_as<float&>;
    requires T::RowsAtCompileTime != Eigen::Dynamic;
    requires T::ColsAtCompileTime != Eigen::Dynamic;
    requires T::RowsAtCompileTime != 1;
};

template <typename T>
struct is_std_array : std::false_type {};

template <typename T, std::size_t N>
struct is_std_array<std::array<T, N>> : std::true_type {};

template <typename T>
concept std_array_eigen_fixed_2D = requires(T t) {
    requires is_std_array<T>::value;
    requires eigen_fixed_2D<typename T::value_type>;
};

template <typename T>
concept is_valid = eigen_fixed_1D<T> || eigen_fixed_2D<T> || std_array_eigen_fixed_2D<T>;

} // INTERNAL end scope

template <Sia::INTERNAL::is_valid T>
class Input_matrix {

    template <typename U>
    constexpr uint16_t deduce_input_type();

    friend Layered_network;
    
protected:

    const T& _input;

    const Layered_network& _network;
    const uint64_t _ID = RANDOM_UINT64T();
    const uint16_t _layer_type = layer_types::INPUT;
    const uint16_t _input_type;


public:
    Input_matrix(const T& input, const Sia::Layered_network& network) : _input(input), _network(network), _input_type(deduce_input_type<T>()) {};
    ~Input_matrix() {};

    uint64_t getID() {return _ID; };
    void debugStatus();
    
};

#include "Sia_input.ipp"

class Dense_layer {
    friend Layered_network;
protected:

// Settings
    const Layered_network& _network;
    const uint64_t _ID = RANDOM_UINT64T();
    const uint16_t _layer_type = layer_types::INPUT;

    Eigen::ArrayXf* _output = nullptr;
    const std::size_t _width;
    bool use_bias = true;
    bool use_weights = true;
    void (*_activation)(const Eigen::ArrayXf&) = nullptr;

public:
    explicit Dense_layer(std::size_t width, const Layered_network& network) : _network(network), _width(width) {};
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
    explicit Output_matrix(const Layered_network& network) : _network(network) {};
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
    std::vector<Eigen::MatrixXf> _matrices; // Stores whatever matrices the network needs, layers-anything
    std::vector<Eigen::ArrayXf> _arrays;  // Stores whatever arrays the network needs, layers-anything

    std::vector<Eigen::MatrixXf> _map_matrices; // Matrices for the final map of the network
    std::vector<Eigen::ArrayXf> _map_arrays; // Array for the final map of the network
    std::vector<void (*)(const Eigen::ArrayXf&)> _map_functions; // Array functions for the final map of the network
// Input stuff ----------------------------------------------------------------------------------------------------------------------------
    std::tuple<uint16_t, matrix_types> _input_settings; // Just saves whether it's a 1/2/3 dimensional input
// Dense stuff ----------------------------------------------------------------------------------------------------------------------------
    // (Layer width, activation function, activation function derivative, using a bias, using the weights, unique ID)
    std::vector<std::tuple<size_t, void (*)(const Eigen::ArrayXf&), void (*)(const Eigen::ArrayXf&), bool, bool, uint64_t>> _dense_settings;

    /* Cleans up the given layer by ID, removing all settings and purging the network of it's very existence */
    void deleteLayer(const uint64_t ID);
    bool existing_ID(const u_int64_t& id);

public:
    explicit Layered_network() {

    };
    ~Layered_network() {};

    template <Sia::INTERNAL::is_valid T>
    void addLayer(const Input_matrix<T>& layer);
    
};

#include "Sia_network.ipp"

} // Namespace end scope