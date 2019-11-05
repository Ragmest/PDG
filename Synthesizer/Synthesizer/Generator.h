#pragma once
#include <vector>
#include <array>
#include <random>

namespace Generator
{
	static std::random_device rd;
	static std::mt19937 gen(rd());

	template<size_t _Size>
	auto generateWhiteNoise(float amplitude)
	{
		float half_amplitude = amplitude / 2.0f;
		std::uniform_real_distribution<float> dis(-half_amplitude, half_amplitude);
		std::array<float, _Size> result;
		std::generate(result.begin(), result.end(), [&dis]() {return dis(gen); });
		return result;
	}

};

