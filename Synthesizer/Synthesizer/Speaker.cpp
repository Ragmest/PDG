#define _CRT_SECURE_NO_WARNINGS
#include "Speaker.h"
#include "Generator.h"

#include <stdio.h>
#include <math.h>

namespace {
	constexpr uint32_t SAMPLE_RATE(44100);
	constexpr uint32_t FRAMES_PER_BUFFER(1024);
	constexpr float M_PI(3.14159265f);
	constexpr uint32_t TABLE_SIZE(200u);
}

Speaker::Speaker()
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

void Speaker::play()
{
	PaError err;
	float buffer[FRAMES_PER_BUFFER][2]; /* stereo output buffer */

	err = Pa_StartStream(_stream);
	if (err != paNoError) Terminate(err);

	while (true)
	{
		auto arr = Generator::generateWhiteNoise<1024u>(0.5f);
		for (uint32_t j = 0; j < FRAMES_PER_BUFFER; ++j)
		{
			buffer[j][0] = arr[j];  /* left */
			buffer[j][1] = arr[j];  /* right */
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

