#define _CRT_SECURE_NO_WARNINGS
#include "Speaker.h"

#include <thread>
#include <stdio.h>
#include <math.h>
#include "portaudio.h"

namespace {
	constexpr uint32_t SAMPLE_RATE(44100);
	constexpr uint32_t FRAMES_PER_BUFFER(1024);
	constexpr float M_PI(3.14159265f);
	constexpr uint32_t TABLE_SIZE(200u);
}

void Terminate(const PaError& err)
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

void Speaker::play()
{
	PaStreamParameters outputParameters;
	PaStream* stream;
	PaError err;
	float buffer[FRAMES_PER_BUFFER][2]; /* stereo output buffer */
	float sine[TABLE_SIZE]; /* sine wavetable */
	int left_phase = 0;
	int right_phase = 0;
	int left_inc = 1;
	int right_inc = 3; /* higher pitch so we can distinguish left and right. */
	int i, j, k;
	int bufferCount;

	printf("PortAudio Test: output sine wave. SR = %d, BufSize = %d\n", SAMPLE_RATE, FRAMES_PER_BUFFER);

	/* initialise sinusoidal wavetable */
	for (i = 0; i < TABLE_SIZE; i++)
	{
		sine[i] = (float)sin(((double)i / (double)TABLE_SIZE) * M_PI * 2.);
	}


	err = Pa_Initialize();
	if (err != paNoError) Terminate(err);

	outputParameters.device = Pa_GetDefaultOutputDevice(); /* default output device */
	if (outputParameters.device == paNoDevice) {
		fprintf(stderr, "Error: No default output device.\n");
		Terminate(err);
	}
	outputParameters.channelCount = 2;       /* stereo output */
	outputParameters.sampleFormat = paFloat32; /* 32 bit floating point output */
	outputParameters.suggestedLatency = 0.050; // Pa_GetDeviceInfo( outputParameters.device )->defaultLowOutputLatency;
	outputParameters.hostApiSpecificStreamInfo = NULL;

	err = Pa_OpenStream(
		&stream,
		NULL, /* no input */
		&outputParameters,
		SAMPLE_RATE,
		FRAMES_PER_BUFFER,
		paClipOff,      /* we won't output out of range samples so don't bother clipping them */
		NULL, /* no callback, use blocking API */
		NULL); /* no callback, so no callback userData */
	if (err != paNoError) Terminate(err);

	printf("Play 3 times, higher each time.\n");

	err = Pa_StartStream(stream);
	if (err != paNoError) Terminate(err);

	while (true)
	{
		for (j = 0; j < FRAMES_PER_BUFFER; j++)
		{
			buffer[j][0] = sine[left_phase];  /* left */
			buffer[j][1] = sine[right_phase];  /* right */
			left_phase += 3;
			if (left_phase >= TABLE_SIZE) left_phase -= TABLE_SIZE;
			right_phase += 3;
			if (right_phase >= TABLE_SIZE) right_phase -= TABLE_SIZE;
		}

		err = Pa_WriteStream(stream, buffer, FRAMES_PER_BUFFER);
		if (err != paNoError) Terminate(err);
	}

	err = Pa_StopStream(stream);
	if (err != paNoError) Terminate(err);

	Pa_Sleep(1000);

	err = Pa_CloseStream(stream);
	if (err != paNoError) Terminate(err);

	Pa_Terminate();
	printf("Test finished.\n");

	return;
}
