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


static struct RandomUInt64T {
    std::mt19937_64 _gen;
    std::uniform_int_distribution<uint64_t> _dis;

    RandomUInt64T() : _gen(std::random_device{}()), _dis(std::numeric_limits<uint64_t>::min(), std::numeric_limits<uint64_t>::max()) {}

    uint64_t operator()() {
        return _dis(_gen);
    }
} RANDOM_UINT64T;

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

namespace Sia { // Start namespace scope

class Layered_network;

namespace INTERNAL{

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

template <Sia::INTERNAL::is_valid T>
class Input_matrix {

    template <typename U>
    constexpr uint16_t deduce_input_type();

    friend Layered_network;
    
protected:

    const T& _input;

    const Layered_network& _network;
    const uint64_t _ID = RANDOM_UINT64T();
    const uint8_t _layer_type = layer_types::INPUT;
    const uint16_t _input_type;

public:
    Input_matrix(const T& input, const Sia::Layered_network& network) : _input(input), _network(network), _input_type(deduce_input_type<T>()) {};

    ~Input_matrix() {};

    uint64_t getID() {return _ID; };
    void debugStatus();
    
};

template <Sia::INTERNAL::is_valid S>
template <typename U>
constexpr uint16_t Sia::Input_matrix<S>::deduce_input_type() {
    if constexpr (Sia::INTERNAL::is_valid_1D_matrix<U>) {
        return 1;
    } else if constexpr (Sia::INTERNAL::is_valid_2D_matrix<U>) {
        return 2;
    } else if constexpr (Sia::INTERNAL::is_valid_3D_matrix<U>) {
        return 3;
    }
    return 4; 
}

template <Sia::INTERNAL::is_valid S>
void Sia::Input_matrix<S>::debugStatus(){
    bool sync_status = std::ios::sync_with_stdio();
    std::ios::sync_with_stdio(false);
    std::cout << "The ID of this layer is: " << _ID << "\n";
    switch (_input_type) {
    case (1):
        std::cout << "One dimensional Eigen array/matrix used\n";
        break;
    case (2):
        std::cout << "Two dimensional Eigen array/matrix used:\n";
        break;
    case (3):
        std::cout << "Three dimensional Eigen array/matrix used\n";

        break;
    default:
        break;
    }
    std::ios::sync_with_stdio(sync_status);
}

class Dense_layer {
    friend Layered_network;
protected:

// Settings
    const Layered_network& _network;
    const uint64_t _ID = RANDOM_UINT64T();
    const uint8_t _layer_type = layer_types::DENSE;

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
    const uint8_t _layer_type = layer_types::OUTPUT;

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

    template <Sia::INTERNAL::is_valid_1D_matrix T>
    void add_layer(const Input_matrix<T>& arg_layer);
    
    template <Sia::INTERNAL::is_valid_2D_matrix T>
    void add_layer(const Input_matrix<T>& arg_layer);

    template <Sia::INTERNAL::is_valid_3D_matrix T>
    void add_layer(const Input_matrix<T>& arg_layer);
    
};

void Sia::Layered_network::delete_layer(const uint64_t arg_ID) {
    if (_layers.empty() || !existing_ID(arg_ID)) {
        return;
    }
    std::ptrdiff_t index = 0;
    for (auto &layer : _layers) {
        if (std::get<1>(layer) == arg_ID) {
            u_int16_t layer_type = std::get<0>(layer);
            u_int32_t setting_index = std::get<2>(layer);
            _layers.erase(_layers.begin() + index);
            if(layer_type == 1) {
                settings_delete(layer_types::INPUT, setting_index);
            }
            std::for_each(_layers.begin() + index, _layers.end(), [&](auto& lambda_layer){--std::get<2>(lambda_layer);});
            break;
        }
        ++index;
    }
}

void Sia::Layered_network::settings_delete(const uint8_t arg_type, const uint32_t arg_index) {
    switch (arg_type)
    {
    case layer_types::INPUT:
        std::get<0>(_input_settings) = 0;
        std::get<1>(_input_settings) = matrix_types::type_NAN;
        _dense_settings.erase(_dense_settings.begin() + arg_index);
        break;
    case layer_types::DENSE:

    default:
        break;
    }
}

bool Sia::Layered_network::existing_ID(const u_int64_t& arg_ID) {
    for (auto layer : _layers){
        if(std::get<1>(layer) == arg_ID)
            return true;
    }
    return false;
}

void Sia::Layered_network::conditional_reserve(){

    // Both 3 and 10 are 'randomly' chosen numbers, 3 seems like a
    // fair ~buffer~ before reallocating more memory, and 10 seems
    // like a good chunk to reserve

    if ((_layers.capacity()-_current_working_index_general)<=3) {
        _layers.reserve(_layers.capacity()+10);
        _dense_settings.reserve(_dense_settings.capacity()+10);
    }
    if ((_arrays.capacity()-_current_working_index_array)<=3) {
        _arrays.reserve(_arrays.capacity()+10);
    }
    if ((_matrices.capacity()-_current_working_index_matrix)<=3) {
        _matrices.reserve(_matrices.capacity()+10);
    }
}

template <Sia::INTERNAL::is_valid_1D_matrix T>
void Sia::Layered_network::add_layer(const Sia::Input_matrix<T>& arg_layer) {

    conditional_reserve();

    for (auto layer_i : _layers){
        if(existing_ID(arg_layer._ID)) {
            std::cerr << "Only one input layer permitted... this has done nothing!\n";
            return;
        }
    }
    _layers.emplace_back(INPUT, (arg_layer._ID), _current_working_index_general);
    _arrays[_current_working_index_array] = arg_layer._input;
    ++_current_working_index_array;
    ++_current_working_index_general;
}

template <Sia::INTERNAL::is_valid_2D_matrix T>
void Sia::Layered_network::add_layer(const Sia::Input_matrix<T>& arg_layer) {
    
    conditional_reserve();

    for (auto layer_i : _layers){
        if(existing_ID(arg_layer._ID)) {
            std::cerr << "Only one input layer permitted... this has done nothing!\n";
            return;
        }
    }
    _layers.emplace_back(INPUT, (arg_layer._ID), _current_working_index_general);
    _matrices[_current_working_index_matrix] = arg_layer._input;
    ++_current_working_index_matrix;
    ++_current_working_index_general;
}

template <Sia::INTERNAL::is_valid_3D_matrix T>
void Sia::Layered_network::add_layer(const Sia::Input_matrix<T>& arg_layer) {
    
    conditional_reserve();

    const std::size_t first_layer = 0;
    const std::size_t second_layer = 1;
    const std::size_t third_layer = 2;

    for (std::tuple<layer_types, uint64_t, uint64_t> layer_i : _layers){
        if(existing_ID(std::get<2>(layer_i))) {
            std::cerr << "Only one input layer permitted... this has done nothing!\n";
            return;
        }
    }
    _layers.emplace_back(INPUT, (arg_layer._ID), _current_working_index_general);
    _matrices[_current_working_index_matrix] = arg_layer._input[first_layer];
    _matrices[_current_working_index_matrix+1] = arg_layer._input[second_layer];
    _matrices[_current_working_index_matrix+2] = arg_layer._input[third_layer];
    _current_working_index_matrix += 3;
    ++_current_working_index_general;
}

} // Namespace end scope
