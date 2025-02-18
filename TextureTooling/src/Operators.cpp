#include "Operators.h"
#include "FastNoise/FastNoiseLite.h"
#include <iostream>

std::vector<unsigned char> Operators::Perlin(int width, int height, int seed, float frequency, int octaves)
{
	FastNoiseLite* FastNoise = new FastNoiseLite;
	FastNoise->SetNoiseType(FastNoiseLite::NoiseType_Perlin);
	FastNoise->SetSeed(seed);
	FastNoise->SetFrequency(frequency);
	FastNoise->SetFractalOctaves(octaves);

	static std::vector<unsigned char> textureData;
	textureData.resize(width * height * 3);

	for (int y = 0; y < height; y++)
	{
		for (int x = 0; x < width; x++)
		{
			// Normalize pixel coordinates to [0,1]
			float nx = x / static_cast<float>(width);
			float ny = y / static_cast<float>(height);
			float noiseValue = FastNoise->GetNoise<float>(x, y);
			unsigned char value = (noiseValue * 0.5f + 0.5f) * 255;
			int index = (y * width + x) * 3;
			// Create a grayscale texture.
			textureData[index] = value;
			textureData[index + 1] = value;
			textureData[index + 2] = value;
		}
	}

	delete FastNoise;

	return textureData;
}

std::vector<unsigned char> Operators::WhiteNoise(int width, int height, int seed)
{
	std::srand(seed);

	static std::vector<unsigned char> textureData;
	textureData.resize(width * height * 3);

	for (int y = 0; y < height; y++)
	{
		for (int x = 0; x < width; x++)
		{
			// Normalize pixel coordinates to [0,1]
			float nx = x / static_cast<float>(width);
			float ny = y / static_cast<float>(height);
			unsigned char value = std::rand() % 255;
			int index = (y * width + x) * 3;
			// Create a grayscale texture.
			textureData[index] = value;
			textureData[index + 1] = value;
			textureData[index + 2] = value;
		}
	}

	return textureData;
}

std::vector<unsigned char> Operators::Colorize(std::vector<unsigned char> inputData, int R, int G, int B)
{
	static std::vector<unsigned char> textureData;
	textureData.resize(inputData.size());
	for (unsigned int i = 0; i < inputData.size(); i += 3)
	{
		int averageValue = (inputData[i] + inputData[i + 1] + inputData[i + 2]) / 3;
		textureData[i]     = static_cast<int>(averageValue * (static_cast<float>(R) / (R + G + B)));
		textureData[i + 1] = static_cast<int>(averageValue * (static_cast<float>(G) / (R + G + B)));
		textureData[i + 2] = static_cast<int>(averageValue * (static_cast<float>(B) / (R + G + B)));
	}
	return textureData;
}
