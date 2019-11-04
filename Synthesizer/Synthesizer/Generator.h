#pragma once
#include <vector>

struct Generator
{
	static std::vector<double> generateWhiteNoise(float time, uint32_t frequnecy = 44100);
	static std::vector<double> generateWhiteNoise(float time, float amplitude, uint32_t frequnecy = 44100);
};

