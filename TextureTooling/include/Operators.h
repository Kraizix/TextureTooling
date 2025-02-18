#pragma once
#include <vector>

class Operators
{
public:

	static std::vector<unsigned char> Perlin(int width, int height, int seed, float frequency, int octaves);

	static std::vector<unsigned char> WhiteNoise(int width, int height, int seed);

	static std::vector<unsigned char> Colorize(std::vector<unsigned char> inputData, int R, int G, int B);

	static std::vector<unsigned char> Mean(std::vector<unsigned char> val, std::vector<unsigned char> val2);
};

