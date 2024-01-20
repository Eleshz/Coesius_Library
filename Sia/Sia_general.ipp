
#pragma once

#include <random>

static struct RandomUInt64T {
    std::mt19937_64 gen;
    std::uniform_int_distribution<uint64_t> dis;

    RandomUInt64T() : gen(std::random_device{}()), dis(std::numeric_limits<uint64_t>::min(), std::numeric_limits<uint64_t>::max()) {}

    uint64_t operator()() {
        return dis(gen);
    }
} RANDOM_UINT64T;