#include "Sia_network.hpp"

/**
 * This method is used to delete specific layers in a neural network.
 * The layers are stored in the '_layers' vector. Each layer is a tuple where:
 * - the first element represents the type of the layer,
 * - the second element is the ID of the layer. A layer is deleted if its ID matches the input ID,
 * - the third element is the index of the settings/matrices for that specific layer in their respective storage.
 *
 * The method iterates over the '_layers' vector. If it finds a valid layer ID (second element of the tuple matches the input ID),
 * it removes the layer from the '_layers' vector and also deletes its settings from the corresponding settings vector based on the layer type.
 */
void Sia::Layered_network::deleteLayers(const uint64_t ID) {
    if (_layers.empty()) {
        return;
    }
    std::size_t index = 0;
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

/**
 * This method checks if a layer with a given ID exists in the neural network.
 * The layers are stored in the '_layers' vector. Each layer is a tuple where:
 * - the first element represents the type of the layer,
 * - the second element is the ID of the layer,
 * - the third element is the index of any settings/matricies/whatever for that specific layer.
 *
 * The method iterates over the '_layers' vector. If it finds a layer with the same ID as the one passed as an argument,
 * it returns true, indicating that a layer with this ID exists in the network.
 * If no such layer is found after checking all layers, the method returns false.
 */
const bool Sia::Layered_network::existing_ID(const u_int64_t &id) {
    for (auto layer : _layers){
        if(std::get<1>(layer) == id)
            return true;
    }
    return false;
}
