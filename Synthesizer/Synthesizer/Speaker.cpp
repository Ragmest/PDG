#define _CRT_SECURE_NO_WARNINGS
#include "Speaker.h"
#include "Generator.h"

#include <stdio.h>
#include <math.h>

namespace {
	constexpr uint32_t SAMPLE_RATE(44100);
	constexpr uint32_t FRAMES_PER_BUFFER(1024);
	constexpr float PI(3.14159265f);
	constexpr float OCTAVE_BASE_FREQ(440.0f);
	constexpr float inverse(float val) { return 1.0f / 12.0f; }
	auto exponent = [](int k) { return static_cast<float>(k - 12)* inverse(12.0f); };
	auto Hertz = [](int k) { return OCTAVE_BASE_FREQ * pow(2.0f, exponent(k)); };
}

Speaker::Speaker() :
	_stream(nullptr),
	_globalTime(0.0),
	_timeStep(1.0 / SAMPLE_RATE)
{
	PaStreamParameters outputParameters;
	PaError err;

	err = Pa_Initialize();
	if (err != paNoError) Terminate(err);

	outputParameters.device = Pa_GetDefaultOutputDevice(); /* default output device */
	if (outputParameters.device == paNoDevice) {
		fprintf(stderr, "Error: No default output device.\n");
		Terminate(err);
	}
	outputParameters.channelCount = 2;       /* stereo output */
	outputParameters.sampleFormat = paFloat32; /* 32 bit floating point output */
	outputParameters.suggestedLatency = Pa_GetDeviceInfo(outputParameters.device)->defaultLowOutputLatency;
	outputParameters.hostApiSpecificStreamInfo = NULL;

	err = Pa_OpenStream(
		&_stream,
		NULL, /* no input */
		&outputParameters,
		SAMPLE_RATE,
		FRAMES_PER_BUFFER,
		paClipOff,      /* we won't output out of range samples so don't bother clipping them */
		NULL, /* no callback, use blocking API */
		NULL); /* no callback, so no callback userData */
	if (err != paNoError) Terminate(err);
}

Speaker::~Speaker()
{
	Pa_Sleep(1000);

	auto err = Pa_CloseStream(_stream);
	if (err != paNoError) Terminate(err);

	Pa_Terminate();
	printf("Test finished.\n");
}

#include <iostream>

void Speaker::play(std::function<float(float)> sampleGenerator)
{
	PaError err;
	float buffer[FRAMES_PER_BUFFER][2]; /* stereo output buffer */

	err = Pa_StartStream(_stream);
	if (err != paNoError) Terminate(err);

	while (true)
	{
		for (int i = 0; i < FRAMES_PER_BUFFER; i++)
		{
			_globalTime += _timeStep;
			auto generatedSample = sampleGenerator(_globalTime);
			buffer[i][0] = generatedSample;  /* left */
			buffer[i][1] = generatedSample;  /* right */
		}

		err = Pa_WriteStream(_stream, buffer, FRAMES_PER_BUFFER);
		if (err != paNoError) Terminate(err);
	}

	err = Pa_StopStream(_stream);
	if (err != paNoError) Terminate(err);
}

void Speaker::playFiltredHelicopter(std::function<float()> sampleGenerator, float cutoffFrequency, float resonanceFrequency)
{
	PaError err;
	float buffer[FRAMES_PER_BUFFER][2]; /* stereo output buffer */

	err = Pa_StartStream(_stream);
	if (err != paNoError) Terminate(err);

	float s = std::sin(2 * PI * cutoffFrequency / SAMPLE_RATE);
	float c = std::cos(2 * PI * cutoffFrequency / SAMPLE_RATE);
	float alf = s / (2 * resonanceFrequency);
	float r = 1 / (1 + alf);

	float a0 = 0.5f * (1 - c) * r;
	float a1 = (1 - c) * r;
	float a2 = a0;
	float b1 = -2 * c * r;
	float b2 = (1 - alf) * r;

	float x1 = sampleGenerator();
	float x2 = sampleGenerator();
	float x3 = sampleGenerator();
	float y1 = 0, y2 = 0;

	while (true)
	{
		for (int i = 0; i < FRAMES_PER_BUFFER; i++)
		{
			_globalTime += _timeStep;
			auto generatedSample = a0 * x1 + a1 * x2 + a2 * x3 - b1 * y1 - b2 * y2;
			x3 = x2;
			x2 = x1;
			x1 = sampleGenerator();
			y2 = y1;
			y1 = generatedSample;
			//sin
			buffer[i][0] = generatedSample * std::sin(_globalTime * 34);  /* left */
			buffer[i][1] = generatedSample * std::sin((1 - _globalTime) * 34);   /* right */
			//triangle 
			//buffer[i][0] = generatedSample * std::asin(std::sin(_globalTime * 34));  /* left */
			//buffer[i][1] = generatedSample * std::asin(std::sin((1 - _globalTime) * 34));   /* right */
			//saw
			//buffer[i][0] = generatedSample * (-std::atan(1/std::tan(_globalTime / 16)));  /* left */
			//buffer[i][1] = generatedSample * (-std::atan(1 / std::tan((1 - _globalTime) * 16)));   /* right */
			//sin(tan(x)) 
			//buffer[i][0] = generatedSample * std::sin(std::tan(_globalTime * 20));  /* left */
			//buffer[i][1] = generatedSample * std::sin(std::tan((1 - _globalTime) * 20));   /* right */
		}

		err = Pa_WriteStream(_stream, buffer, FRAMES_PER_BUFFER);
		if (err != paNoError) Terminate(err);
	}

	err = Pa_StopStream(_stream);
	if (err != paNoError) Terminate(err);
}

void Speaker::Terminate(const PaError& err)
{
	fprintf(stderr, "An error occured while using the portaudio stream\n");
	fprintf(stderr, "Error number: %d\n", err);
	fprintf(stderr, "Error message: %s\n", Pa_GetErrorText(err));
	// Print more information about the error.
	if (err == paUnanticipatedHostError)
	{
		const PaHostErrorInfo* hostErrorInfo = Pa_GetLastHostErrorInfo();
		fprintf(stderr, "Host API error = #%ld, hostApiType = %d\n", hostErrorInfo->errorCode, hostErrorInfo->hostApiType);
		fprintf(stderr, "Host API error = %s\n", hostErrorInfo->errorText);
	}
	Pa_Terminate();
}
