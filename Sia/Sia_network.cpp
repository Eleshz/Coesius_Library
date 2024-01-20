#include "Sia_network.hpp"

/**
 * This method is used to delete layers in a neural network.
 * The layers are stored in the '_layers' vector. Each layer is a tuple where:
 * - the first element represents the type of the layer,
 * - the second element is the ID of the layer, normally a large random number, I'm using a 0 to mark it for deletion,
 * - the third element is the index of any settings/matricies/whatever for that specific layer.
 *
 * The method iterates over the '_layers' vector. If it finds a layer that is marked for deletion (second element of the tuple is 0),
 * it removes the layer from the '_layers' vector and also deletes its settings from the corresponding settings vector.
*/
void Sia::Layered_network::deleteLayers() {
    if (_layers.empty()) {
        return;
    }

    std::size_t index = 0;
    for (auto &layer : _layers) {
        int layer_type;
        int setting_index;

        if (std::get<1>(layer) == 0) {
            layer_type = std::get<0>(layer);
            setting_index = std::get<2>(layer);

            _layers.erase(_layers.begin() + index);

            if (layer_type != 0) {
                switch(layer_type) {
                    case (1): {
                        _dense_settings.erase(_dense_settings.begin() + setting_index);
                        break;
                    }
                    default:
                        break;
                }
            }
        }
        ++index;
    }
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
