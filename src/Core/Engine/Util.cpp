//
// Created by Carpov Pavel on 26.09.2025.
//

#include "Util.hpp"

#include <random>

uint32_t Util::randomInRange(uint32_t max, uint32_t min)
{
	static std::random_device rd;  // Will be used to obtain a seed for the random number engine
	static std::mt19937 gen(rd()); // Standard mersenne_twister_engine seeded with rd()
	std::uniform_int_distribution<> distrib(min, max);
	return distrib(gen);
}