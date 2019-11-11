#pragma once
#include <vector>
#include <array>
#include <random>

namespace Generator
{
	static std::random_device rd;
	static std::mt19937 gen(rd());
	static float PI{ 3.14159265358979323846f };

	template<size_t _Size>
	auto whiteNoise(float amplitude)
	{
		float half_amplitude = amplitude / 2.0f;
		std::uniform_real_distribution<float> dis(-half_amplitude, half_amplitude);
		std::array<float, _Size> result;
		std::generate(result.begin(), result.end(), [&dis]() {return dis(gen); });
		return result;
	}

	template<size_t _Size>
	auto sineWave(const uint32_t sampleRate, const float frequency)
	{
		std::array<float, _Size> result;
		float factor = (frequency * 2.0f * PI) /  (static_cast<float>(sampleRate));
		float phaseShift = /*time() %*/ frequency * 2.0f * PI;
		int index = -1;
		std::generate(result.begin(), result.end(),
			[&index, &factor, &phaseShift]() { 
				index++; 
				return std::sin((static_cast<float>(index)* factor) + phaseShift); });
		return result;
	}
};

