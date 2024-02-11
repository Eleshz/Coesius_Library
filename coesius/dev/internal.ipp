#include <type_traits>

enum class layer_types {
    INPUT,
    OUTPUT,
    DENSE
};

enum class matrix_types {
    ONE_D,
    TWO_D,
    THREE_D,
    type_NAN // Gotta have some error type
};

template <typename matrix_type>
concept matrix_1D = requires(matrix_type arg_input) {
    { arg_input.size() } -> std::convertible_to<std::ptrdiff_t>; // I think this is the standard thing to do here
    { arg_input(0) } -> std::same_as<float&>;
    requires matrix_type::SizeAtCompileTime != Eigen::Dynamic; // SFINAE moment
    requires std::is_base_of_v<Eigen::DenseBase<matrix_type>, matrix_type>;
    matrix_type::RowsAtCompileTime == 1; // It's one dimensional, so there should be only one row
};

template <typename matrix_type>
concept matrix_2D = requires(matrix_type arg_input) {
    { arg_input.rows() } -> std::convertible_to<std::ptrdiff_t>; // standard?
    { arg_input.cols() } -> std::convertible_to<std::ptrdiff_t>; // standard?
    { arg_input(0, 0) } -> std::same_as<float&>; // Also you couldn't use the "(0, 0)" syntax if this were 
    requires matrix_type::RowsAtCompileTime != Eigen::Dynamic;
    requires matrix_type::ColsAtCompileTime != Eigen::Dynamic;
    requires matrix_type::RowsAtCompileTime != 1; // If it's got only one row, then it's not two dimensional, it's one dimensional duh
    requires matrix_type::ColsAtCompileTime != 1; // Same with the columns, but the one dimensional doesn't work this way either
};

template <typename matrix_type>
struct is_std_array : std::false_type {};
template <typename matrix_type, std::size_t N>
struct is_std_array<std::array<matrix_type, N>> : std::true_type {};
// Idk why this isn't in the std somewhere but whatever

template <typename matrix_type>
concept matrix_3D = requires(matrix_type arg_input) {
    requires is_std_array<matrix_type>::value; // value is from the std::true/false type
    requires matrix_2D<typename matrix_type::value_type>; // if the last line is true (it is an std::array), then get the type the array holds with ::value_type
};

template <typename matrix_type>
concept matrix = matrix_1D<matrix_type> || matrix_2D<matrix_type> || matrix_3D<matrix_type>;