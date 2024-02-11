#pragma once

#include <random>
#define EIGEN_USE_BLAS

class RandomUInt64T {
    std::mt19937_64 m_gen;
    std::uniform_int_distribution<uint64_t> m_dis;

public:
    RandomUInt64T() : m_gen(std::random_device{}()), m_dis(std::numeric_limits<uint64_t>::min(), std::numeric_limits<uint64_t>::max()) {}

    uint64_t operator()() {
        return m_dis(m_gen);
    }
}; // Super stupid but it works for now and fairly easily replaceable 