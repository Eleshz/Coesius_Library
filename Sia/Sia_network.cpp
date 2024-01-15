#include "Sia_network.hpp"

void Sia::Layered_network::deleteLayers() {
    if (_layers.empty()) // Just skip if there's nothing there
        return;
    size_t index = 0;
    for (auto &layer : _layers){
        if (index >= _layers.size() - 1) break; // prevent overflow
        int layer_type;
        int setting_index;
        if (std::get<1>(layer)==0){ // Did they delete a layer?
            layer_type = std::get<0>(layer); // Get the type they deleted
            setting_index = std::get<2>(layer); // What index is the settings stored?
            std::rotate(_layers.begin()+index, _layers.begin()+(1+index), _layers.end()); // Move all the layers to the left, pushing the "empty" layer to the back
            _layers.pop_back(); // Delete it
            if (layer_type!=0){
                switch(layer_type){ // Do the same for the settings of the layer
                    case(1): {
                        std::rotate(_dense_settings.begin()+setting_index, _dense_settings.begin()+(1+setting_index), _dense_settings.end());
                    }
                }
            }
        }
        ++index;
    }
}