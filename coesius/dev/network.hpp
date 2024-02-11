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

    enum class layer_types {
        INPUT,
        OUTPUT,
        DENSE
    };

    enum class matrix_types {
        ONE_D,
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
    friend Layered_network;
    uint16_t deduce_input_type();
protected:
    T& _input;
public:

};

#include <coesius/dev/input.ipp>

class Dense_layer {
    friend Layered_network;
protected:

};

class Output_matrix {
    friend Layered_network;
protected:

};

class Layered_network {
private:
// Output
    Eigen::ArrayXf _output;
// General network stuff ------------------------------------------------------------------------------------------------------------------

    struct m_layer {
        Internal::layer_types layer_type;
        uint64_t layer_id;
        uint64_t layer_network_id;
    };  // (Type / unique ID / index [for the network])

    std::vector<m_layer> m_layers;

    struct m_link {
        uint64_t id_head;
        uint64_t id_tail;
    };  // (Two unique IDs that represent a link, first is head, second is tail)

    std::vector<m_link> m_links;

    std::vector<Eigen::MatrixXf> m_matrices; // Stores whatever matrices the network needs, layers-anything
    std::vector<Eigen::ArrayXf> m_arrays; // Stores whatever arrays the network needs, layers-anything

    std::vector<Eigen::MatrixXf> m_map_matrices; // Matrices for the final map of the network
    std::vector<Eigen::ArrayXf> m_map_arrays; // Array for the final map of the network

    std::vector<void (*)(const Eigen::ArrayXf&)> m_map_functions_array; // Array functions for the final map of the network
    std::vector<void (*)(const Eigen::MatrixXf&)> m_map_functions_matrices; // Array functions for the final map of the network
// Input stuff ----------------------------------------------------------------------------------------------------------------------------
    Internal::matrix_types m_input_setting; // Just saves whether it's a 1/2/3 dimensional input
// Dense stuff ----------------------------------------------------------------------------------------------------------------------------

    struct m_dense_setting {
        size_t layer_width;
        void (*act_func)(const Eigen::ArrayXf&);
        void (*act_func_deriv)(const Eigen::ArrayXf&);
        bool use_bias;
        bool use_weights;
        uint64_t id;
    }; // (Layer width, activation function, activation function derivative, using a bias, using the weights, unique ID)

    std::vector<m_dense_setting> m_dense_settings;

public:
    explicit Layered_network()  = default;
    ~Layered_network() = default; 
};

#include <coesius/dev/network.ipp>

} // Namespace end scope

#endif // No more Coesius_lib