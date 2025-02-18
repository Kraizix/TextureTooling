#pragma once
#include <vector>

class Operators
{
public:

	static std::vector<unsigned char> Perlin(int width, int height, int seed, float frequency, int octaves);

	static std::vector<unsigned char> WhiteNoise(int width, int height, int seed);
};

