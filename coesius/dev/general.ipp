#pragma once

#include <random>
class RandomUInt64T {
    std::mt19937_64 _gen;
    std::uniform_int_distribution<uint64_t> _dis;

public:
    RandomUInt64T() : _gen(std::random_device{}()), _dis(std::numeric_limits<uint64_t>::min(), std::numeric_limits<uint64_t>::max()) {}

    uint64_t operator()() {
        return _dis(_gen);
    }
};