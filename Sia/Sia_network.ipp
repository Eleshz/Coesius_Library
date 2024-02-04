#include <algorithm>

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