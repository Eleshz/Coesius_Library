

template <Sia::INTERNAL::is_valid S>
template <typename U>
constexpr uint16_t Sia::Input_matrix<S>::deduce_input_type() {
    if constexpr (Sia::INTERNAL::eigen_fixed_1D<U>) {
        return 1;
    } else if constexpr (Sia::INTERNAL::eigen_fixed_2D<U>) {
        return 2;
    } else if constexpr (Sia::INTERNAL::std_array_eigen_fixed_2D<U>) {
        return 3;
    }
    return 4; 
}

template <Sia::INTERNAL::is_valid S>
const void Sia::Input_matrix<S>::debugStatus(){
    bool was_synced = std::ios::sync_with_stdio();
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
    std::ios::sync_with_stdio(was_synced);
}