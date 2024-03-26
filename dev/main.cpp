#include <coesius/dev/network.hpp>
#include <Fastor/Fastor.h>
#include <cblas.h>

#include <array>
#include <iostream>

template<typename T, size_t... Dims>
requires std::same_as<T, float>
void processTensor(const Fastor::Tensor<T, Dims...>& tensor) {
    tensor *= 2.0f;
}

using namespace Fastor;

int main() {

    // Create a 3x3 fixed-size tensor
    Tensor<double,3,3> A = {1,2,3,4,5,6,7,8,9};

    // Process the tensor
    processTensor(A);

    print(A);

    return 0;
}