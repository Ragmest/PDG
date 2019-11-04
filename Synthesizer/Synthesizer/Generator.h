#pragma once
#include <vector>

/*
I guess not needed offline generator. 
I won't remove it until I am not 100% sure it's redundant.
*/
namespace Generator
{
	std::vector<float> generateWhiteNoise(float time, uint32_t frequnecy = 44100);
	std::vector<float> generateWhiteNoise(float time, float amplitude, uint32_t frequnecy = 44100);
};

