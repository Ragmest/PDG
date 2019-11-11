#include "Generator.h"
#include "Speaker.h"
#include "AudioFile.h"
#include <thread>
#include <atomic>
#include <iostream>
#include <Windows.h>

struct EnvelopeS
{
	double attackTime;
	double decayTime;
	double sustainAmplitude;
	double releaseTime;
	double startAmplitude;
	double triggerOffTime;
	double triggerOnTime;
	bool noteOn;

	EnvelopeS() :
		attackTime(0.10),
		decayTime(0.01),
		startAmplitude(1.0),
		sustainAmplitude(0.8),
		releaseTime(0.20),
		noteOn(false),
		triggerOffTime(0.0),
		triggerOnTime(0.0)
	{
	}

	// Call when key is pressed
	void NoteOn(double dTimeOn)
	{
		triggerOnTime = dTimeOn;
		noteOn = true;
	}

	// Call when key is released
	void NoteOff(double dTimeOff)
	{
		triggerOffTime = dTimeOff;
		noteOn = false;
	}

	// Get the correct amplitude at the requested point in time
	double GetAmplitude(double dTime)
	{
		double dAmplitude = 0.0;
		double dLifeTime = dTime - triggerOnTime;

		if (noteOn)
		{
			if (dLifeTime <= attackTime)
			{
				// In attack Phase - approach max amplitude
				dAmplitude = (dLifeTime / attackTime) * startAmplitude;
			}

			if (dLifeTime > attackTime&& dLifeTime <= (attackTime + decayTime))
			{
				// In decay phase - reduce to sustained amplitude
				dAmplitude = ((dLifeTime - attackTime) / decayTime) * (sustainAmplitude - startAmplitude) + startAmplitude;
			}

			if (dLifeTime > (attackTime + decayTime))
			{
				// In sustain phase - dont change until note released
				dAmplitude = sustainAmplitude;
			}
		}
		else
		{
			// Note has been released, so in release phase
			dAmplitude = ((dTime - triggerOffTime) / releaseTime) * (0.0 - sustainAmplitude) + sustainAmplitude;
		}

		// Amplitude should not be negative
		if (dAmplitude <= 0.0001)
			dAmplitude = 0.0;

		return dAmplitude;
	}
};

namespace {
	std::atomic<float> dFrequencyOutput = 0.0f;
	const float dOctaveBaseFrequency = 440.0f;
	EnvelopeS envelope;
}

constexpr float inverse(float val)
{
	return 1.0f / 12.0f;
}


float sineGen(float time)
{
	return envelope.GetAmplitude(time) * std::sin(dFrequencyOutput * 2.0f * 3.14159f * time);
}

int main()
{
	Speaker speaker;

	std::thread speakerThread([&speaker]() { speaker.play(&sineGen); });
	int currentKey = -1;
	bool keyPressed = false;
	auto exponent = [](int k) { return static_cast<float>(k - 12) * inverse(12.0f); };
	while (true)
	{
		keyPressed = false;
		for (int k = 0; k < 16; k++)
		{
			if (GetAsyncKeyState((unsigned char)("ZSXCFVGBNJMK\xbcL\xbe\xbf"[k])) & 0x8000)
			{
				if (currentKey != k)
				{
					dFrequencyOutput = dOctaveBaseFrequency * pow(2.0f, exponent(k));
					envelope.NoteOn(speaker.getTime());
					currentKey = k;
				}

				keyPressed = true;
			}
		}

		if (!keyPressed)
		{
			if (currentKey != -1)
			{
				envelope.NoteOff(speaker.getTime());
				currentKey = -1;
			}

		}
	}

	return 0;
}