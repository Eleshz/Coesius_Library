#pragma once

#include <Eigen/Dense>

#include <vector>
#include <cstdint>
#include <span>
#include <array>
#include <type_traits>
#include <iostream>
#include <algorithm>

#define NAMESPACE_COESIUS Coesius

#ifndef COESIUS_LIB_H
#define COESIUS_LIB_H

#include <coesius/dev/general.ipp>


namespace NAMESPACE_COESIUS { // Start namespace scope

class Layered_network;

namespace Internal{

    enum layer_types {
        INPUT = 0,
        OUTPUT,
        DENSE
    };

    enum matrix_types {
        ONE_D = 0,
        TWO_D,
        THREE_D,
        type_NAN
    };

// vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv

template <typename T>
concept is_valid_1D_matrix = requires(T t) {
    { t.size() } -> std::same_as<std::ptrdiff_t>;
    { t(0) } -> std::same_as<typename T::Scalar&>;
    { t(0) } -> std::same_as<float&>;
    requires T::SizeAtCompileTime != Eigen::Dynamic;
    requires std::is_base_of_v<Eigen::DenseBase<T>, T> && T::RowsAtCompileTime == 1;
};

template <typename T>
concept is_valid_2D_matrix = requires(T t) {
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
concept is_valid_3D_matrix = requires(T t) {
    requires is_std_array<T>::value;
    requires is_valid_2D_matrix<typename T::value_type>;
};

// ^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

template <typename T>
concept is_valid = is_valid_1D_matrix<T> || is_valid_2D_matrix<T> || is_valid_3D_matrix<T>;

} // INTERNAL end scope

template <Coesius::Internal::is_valid T>
class Input_matrix {

    template <typename U>
    constexpr uint16_t deduce_input_type();

    friend Layered_network;
    
protected:

    const T& _input;

    const Layered_network& _network;
    const uint64_t _ID = RANDOM_UINT64T();
    const uint8_t _layer_type = Internal::layer_types::INPUT;
    const uint16_t _input_type;


public:
    Input_matrix(const T& input, const Coesius::Layered_network& network) : _input(input), _network(network), _input_type(deduce_input_type<T>()) {};

    ~Input_matrix() {};

    uint64_t getID() {return _ID; };
    void debugStatus();
    
};

#include <coesius/dev/input.ipp>

class Dense_layer {
    friend Layered_network;
protected:

// Settings
    const Layered_network& _network;
    const uint64_t _ID = RANDOM_UINT64T();
    const uint8_t _layer_type = Internal::layer_types::DENSE;

    Eigen::ArrayXf* _output = nullptr;
    std::size_t _width;
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
    const uint8_t _layer_type = Internal::layer_types::OUTPUT;


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
    std::vector<std::tuple<Internal::layer_types, uint64_t, uint64_t>> _layers; // (Type / unique ID / index [for the network])
    std::vector<std::pair<uint64_t, uint64_t>> _links; // (Two unique IDs that represent a link, first is head, second is tail)
    std::vector<Eigen::MatrixXf> _matrices; // Stores whatever matrices the network needs, layers-anything
    std::vector<Eigen::ArrayXf> _arrays;  // Stores whatever arrays the network needs, layers-anything

    std::vector<Eigen::MatrixXf> _map_matrices; // Matrices for the final map of the network
    std::vector<Eigen::ArrayXf> _map_arrays; // Array for the final map of the network
    std::vector<void (*)(const Eigen::ArrayXf&)> _map_functions; // Array functions for the final map of the network
// Input stuff ----------------------------------------------------------------------------------------------------------------------------
    std::tuple<uint16_t, Internal::matrix_types> _input_settings; // Just saves whether it's a 1/2/3 dimensional input
// Dense stuff ----------------------------------------------------------------------------------------------------------------------------
    // (Layer width, activation function, activation function derivative, using a bias, using the weights, unique ID)
    std::vector<std::tuple<size_t, void (*)(const Eigen::ArrayXf&), void (*)(const Eigen::ArrayXf&), bool, bool, uint64_t>> _dense_settings;



// ----------------------------------------------------------------------------------------------------------------------------------------
    /* Cleans up the given layer by ID, purging the network of it's very existence */
    void delete_layer(const uint64_t arg_ID);
    /* Deletes the settings of a specific type, and uses the index to potentially update others to reflect that */
    void settings_delete(const uint8_t arg_type, const uint32_t arg_index);
    /* Returns true if the ID is in the network */
    bool existing_ID(const u_int64_t& arg_ID);
    /* Reserves more space in the vectors that need it, adds chunks of 10 at a time */
    void conditional_reserve();

void NewFunction();

u_int64_t _current_working_index_matrix = 0;
u_int64_t _current_working_index_array = 0;
u_int64_t _current_working_index_general = 0;

public:
    explicit Layered_network() {};
    ~Layered_network() {};

    template <Coesius::Internal::is_valid_1D_matrix T>
    void add_layer(const Input_matrix<T>& arg_layer);
    
    template <Coesius::Internal::is_valid_2D_matrix T>
    void add_layer(const Input_matrix<T>& arg_layer);

    template <Coesius::Internal::is_valid_3D_matrix T>
    void add_layer(const Input_matrix<T>& arg_layer);
    
};

#include <coesius/dev/network.ipp>

} // Namespace end scope

#endif // No more Coesius_lib