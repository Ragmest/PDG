#pragma once
#include <vector>
#include "portaudio.h"

class Speaker
{
private:
	PaStream* _stream;

public:
	Speaker();
	~Speaker();

	void play();

private:
	void Terminate(const PaError& err);
};

