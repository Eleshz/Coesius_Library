#pragma once

#include <Eigen/Dense>

#include <vector>
#include <cstdint>
#include <span>
#include <array>
#include <type_traits>
#include <iostream>
#include <algorithm>

#include <Fastor/Fastor.h>
#include <cblas.h>

#ifndef COESIUS_LIB_H
#define COESIUS_LIB_H

#define NAMESPACE_COESIUS Coesius

#include <coesius/dev/general.ipp>

namespace NAMESPACE_COESIUS { // Start namespace scope

class Layered_network;

namespace Internal{

#include <coesius/dev/internal.ipp> // There's so much I'm not stuffing it all into this header

} // INTERNAL end scope

template <Coesius::Internal::matrix matrix_type>
class Input_matrix {
    friend Layered_network;
    Internal::matrix_types m_input_type;

    struct m_links {
        std::vector<uint64_t> link;
    };

protected:
    matrix_type& m_ref_to_input;
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

#include <coesius/dev/network.ipp>

} // Namespace end scope

#endif // No more Coesius_lib