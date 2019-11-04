#include "Generator.h"
#include <random>
#include <algorithm>

std::vector<double> Generator::generateWhiteNoise(float time, uint32_t frequnecy)
{
	return generateWhiteNoise(time, 2.0f, frequnecy);
}

std::vector<double> Generator::generateWhiteNoise(float time, float amplitude, uint32_t frequnecy)
{
	std::random_device rd;
	std::mt19937 gen(rd());
	float half_amplitude = amplitude / 2.0f;
	std::uniform_real_distribution<double> dis(-half_amplitude, half_amplitude);
	uint32_t size = static_cast<uint32_t>(static_cast<float>(frequnecy)* time);
	std::vector<double> result(size);
	std::generate(result.begin(), result.end(), [&dis, &gen]() {return dis(gen); });
	return result;
}
