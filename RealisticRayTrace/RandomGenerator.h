#pragma once
class RandomGenerator
{
	unsigned long long seed, mult;
public:
	RandomGenerator(unsigned long long s = 7564231ULL)
	{
		seed = s; mult = 6208911ULL;
	}
	inline double rand()
	{
		seed = mult * seed;
		return double(seed%ULLONG_MAX) / ULLONG_MAX;
	}
};