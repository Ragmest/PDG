#define _CRT_SECURE_NO_WARNINGS
#include "Speaker.h"
#include <stdio.h>
#include <math.h>
#include <thread>
#include <iostream>
#include <random>
#include "portaudio.h"

namespace {
	void terminate(const PaError& err)
	{
		Pa_Terminate();
		fprintf(stderr, "An error occured while using the portaudio stream\n");
		fprintf(stderr, "Error number: %d\n", err);
		fprintf(stderr, "Error message: %s\n", Pa_GetErrorText(err));
	}
}


#define NUM_SECONDS   (5)
#define SAMPLE_RATE   (44100)
#define FRAMES_PER_BUFFER  (64)

#ifndef M_PI
#define M_PI  (3.14159265)
#endif
#define TABLE_SIZE   (64)
typedef struct
{
	std::mt19937 gen;
	std::uniform_real_distribution<float> dis;
}
paTestData;

static int patestCallback(const void*, void* outputBuffer,
	unsigned long framesPerBuffer,
	const PaStreamCallbackTimeInfo*,
	PaStreamCallbackFlags,
	void* userData)
{
	paTestData* data = (paTestData*)userData;
	float* out = (float*)outputBuffer;
	unsigned long i;

	for (i = 0; i < framesPerBuffer; i++)
	{
		float sample = data->dis(data->gen);
		*out++ = sample;  /* left */
		*out++ = sample;  /* right */
	}

	return paContinue;
}

static void StreamFinished(void*)
{
}

void Speaker::play()
{
	PaStreamParameters outputParameters;
	PaStream* stream;
	PaError err;
	int i;
	std::random_device rd;
	float half_amplitude = 1.0f / 2.0f;

	paTestData data{
		std::mt19937(rd()),
		std::uniform_real_distribution<float>(-half_amplitude, half_amplitude)
	};

	printf("PortAudio Test: output sine wave. SR = %d, BufSize = %d\n", SAMPLE_RATE, FRAMES_PER_BUFFER);

	err = Pa_Initialize();
	if (err != paNoError) terminate(err);

	outputParameters.device = Pa_GetDefaultOutputDevice(); /* default output device */
	if (outputParameters.device == paNoDevice) {
		fprintf(stderr, "Error: No default output device.\n");
		terminate(err);
	}
	outputParameters.channelCount = 2;       /* stereo output */
	outputParameters.sampleFormat = paFloat32; /* 32 bit floating point output */
	outputParameters.suggestedLatency = Pa_GetDeviceInfo(outputParameters.device)->defaultLowOutputLatency;
	outputParameters.hostApiSpecificStreamInfo = NULL;

	err = Pa_OpenStream(
		&stream,
		NULL, /* no input */
		&outputParameters,
		SAMPLE_RATE,
		FRAMES_PER_BUFFER,
		paClipOff,      /* we won't output out of range samples so don't bother clipping them */
		patestCallback,
		&data);
	if (err != paNoError) terminate(err);

	err = Pa_SetStreamFinishedCallback(stream, &StreamFinished);
	if (err != paNoError) terminate(err);

	err = Pa_StartStream(stream);
	if (err != paNoError) terminate(err);

	std::thread stoppingThread([]() { std::cin.get(); });
	stoppingThread.join();

	err = Pa_StopStream(stream);
	if (err != paNoError) terminate(err);

	err = Pa_CloseStream(stream);
	if (err != paNoError) terminate(err);

	Pa_Terminate();
	printf("Test finished.\n");

	return;
}
