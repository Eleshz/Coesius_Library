template <Sia::INTERNAL::is_valid S>
template <typename U>
constexpr uint16_t Sia::Input_matrix<S>::deduce_input_type() {
    if constexpr (std::is_base_of<Eigen::DenseBase<std::decay_t<U>>, std::decay_t<U>>::value) {
        return 0;
    } else if constexpr (is_std_array<std::decay_t<U>>::value) {
        return 1;
    }
    return 2; 
}

template <Sia::INTERNAL::is_valid S>
const void Sia::Input_matrix<S>::debugStatus(){
    bool was_synced = std::ios::sync_with_stdio();
    std::ios::sync_with_stdio(false);
    std::cout << "The ID of this layer is: " << _ID << "\n";
    switch (_input_type) {
    case (0):
        std::cout << "The type of this layer's input is a Eigen compatable type\n";
        break;
    case (1):
        std::cout << "The type of this layer's input is an std::array type, will be useable\n";
        break;
    case (2):
        std::cout << "The type of this layer's input is an unknown type, consider UB\n";
        break;
    default:
        break;
    }
    std::ios::sync_with_stdio(was_synced);
}