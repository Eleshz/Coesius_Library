#include "Sia_network.hpp"

void Sia::Layered_network::deleteLayers() {
    if (_layers.empty()) // Just skip if there's nothing there
        return;
    size_t index = 0;
    for (auto &layer : _layers){
        int layer_type;
        int setting_index;
        if (std::get<1>(layer)==0){ // Did they delete a layer?
            layer_type = std::get<0>(layer); // Get the type they deleted
            setting_index = std::get<2>(layer); // What index is the settings stored?
            _layers.erase(_layers.begin()+index);
            if (layer_type!=0){
                switch(layer_type){ // Do the same for the settings of the layer
                    case(1): {
                        _dense_settings.erase(_dense_settings.begin()+setting_index);
                    }
                }
            }
        }
        ++index;
    }
}