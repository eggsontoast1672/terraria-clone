#pragma once

#include <random>

float get_random_float(std::ranlux24_base &rng, float min, float max);
int get_random_int(std::ranlux24_base &rng, int min, int max);
bool get_random_chance(std::ranlux24_base &rng, float chance);
