#pragma once
#include <Eigen/Dense>
#include <algorithm>
#include <array>
#include <cstdint>
#include <iostream>
#include <random>
#include <span>
#include <type_traits>
#include <vector>


#ifndef COESIUS_LIB_H
#define COESIUS_LIB_H

#define NAMESPACE_COESIUS Coesius

class RandomUInt64T {
    std::mt19937_64 m_gen;
    std::uniform_int_distribution<uint64_t> m_dis;

public:
    RandomUInt64T() : m_gen(std::random_device{}()), m_dis(std::numeric_limits<uint64_t>::min(), std::numeric_limits<uint64_t>::max()) {}

    uint64_t operator()() {
        return m_dis(m_gen);
    }
}; // Super stupid but it works for now and fairly easily replaceable 

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
    type_NAN // Gotta have some error type
};

template <typename matrix_type>
concept matrix_1D = requires(matrix_type arg_input) {
    { arg_input.size() } -> std::convertible_to<std::ptrdiff_t>; // I think this is the standard thing to do here
    { arg_input(0) } -> std::same_as<float&>;
    requires matrix_type::SizeAtCompileTime != Eigen::Dynamic; // SFINAE moment
    requires std::is_base_of_v<Eigen::DenseBase<matrix_type>, matrix_type>;
    matrix_type::RowsAtCompileTime == 1; // It's one dimensional, so there should be only one row
};

template <typename matrix_type>
concept matrix_2D = requires(matrix_type arg_input) {
    { arg_input.rows() } -> std::convertible_to<std::ptrdiff_t>; // standard?
    { arg_input.cols() } -> std::convertible_to<std::ptrdiff_t>; // standard?
    { arg_input(0, 0) } -> std::same_as<float&>; // Also you couldn't use the "(0, 0)" syntax if this were 
    requires matrix_type::RowsAtCompileTime != Eigen::Dynamic;
    requires matrix_type::ColsAtCompileTime != Eigen::Dynamic;
    requires matrix_type::RowsAtCompileTime != 1; // If it's got only one row, then it's not two dimensional, it's one dimensional duh
    requires matrix_type::ColsAtCompileTime != 1; // Same with the columns
};

template <typename matrix_type>
struct is_std_array : std::false_type {};
template <typename matrix_type, std::size_t N>
struct is_std_array<std::array<matrix_type, N>> : std::true_type {};
// Idk why this isn't in the std somewhere but whatever

template <typename matrix_type>
concept matrix_3D = requires(matrix_type arg_input) {
    requires is_std_array<matrix_type>::value; // value is from the std::true/false type
    requires matrix_2D<typename matrix_type::value_type>; // if the last line is true (it is an std::array), then get the type the array holds with ::value_type
};

template <typename matrix_type>
concept matrix = matrix_1D<matrix_type> || matrix_2D<matrix_type> || matrix_3D<matrix_type>;

} // INTERNAL end scope

template <Coesius::Internal::matrix matrix_type>
class Input_matrix {
    friend Layered_network;
    Internal::matrix_types m_input_type;
protected:
    matrix_type& m_ref_to_input;
public:

};

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
    Eigen::ArrayXf m_network_output;
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

} // Namespace end scope

#endif // No more Coesius_lib
