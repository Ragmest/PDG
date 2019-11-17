#pragma once
#include <vector>
#include "portaudio.h"
#include <functional>
#include <chrono>

class Speaker
{
private:
	PaStream* _stream;
	double _globalTime;
	const double _timeStep;

public:
	Speaker();
	~Speaker();

	void play(std::function<float(float)> sampleGenerator);
	void playFiltredHelicopter(std::function<float()> sampleGenerator, float cutoffFrequency, float resonanceFrequency);
	inline double getTime() { return _globalTime; };

private:
	void Terminate(const PaError& err);
};

