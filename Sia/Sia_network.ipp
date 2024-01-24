void Sia::Layered_network::deleteLayers(const uint64_t ID) {
    if (_layers.empty()) {
        return;
    }
    std::ptrdiff_t index = 0;
    for (auto &layer : _layers) {

        if (std::get<1>(layer) == ID) {
            u_int16_t layer_type = std::get<0>(layer);
            u_int32_t setting_index = std::get<2>(layer);
            _layers.erase(_layers.begin() + index);
            switch(layer_type) {
                case (1): {
                    _dense_settings.erase(_dense_settings.begin() + setting_index);
                    break;
                }
                default:
                    break;
            }
            goto END;
        }
        ++index;
    }
    END:
    return;
}

bool Sia::Layered_network::existing_ID(const u_int64_t &id) {
    for (auto layer : _layers){
        if(std::get<1>(layer) == id)
            return true;
    }
    return false;
}

template <Sia::INTERNAL::is_valid T>
void Sia::Layered_network::addLayer(const Sia::Input_matrix<T>& layer) {
    for (auto layer_i : _layers){
        if(existing_ID(layer._ID)) {
            std::cerr << "Only one input layer permitted... this has done nothing!\n";
            return;
        }
    }
    _layers.emplace_back(INPUT, (layer._ID), _layers.size()-1);
    _matricies[_layers.size()-1] = layer._input;
}
