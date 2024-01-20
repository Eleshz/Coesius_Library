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