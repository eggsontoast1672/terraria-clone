#include "random.hpp"

#include <random>

float get_random_float(std::ranlux24_base &rng, float min, float max)
{
    return std::uniform_real_distribution<float>(min, max)(rng);
}

int get_random_int(std::ranlux24_base &rng, int min, int max)
{
    return std::uniform_int_distribution<int>(min, max)(rng);
}

bool get_random_chance(std::ranlux24_base &rng, float chance)
{
    return get_random_float(rng, 0.0f, 1.0f) <= chance;
}
