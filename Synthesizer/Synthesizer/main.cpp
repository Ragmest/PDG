#include "Generator.h"
#include "AudioFile.h"

int main()
{
	AudioFile<double> audioFile;
	std::vector<std::vector<double>> audioBuffer;
	audioBuffer.emplace_back(Generator::generateWhiteNoise(1.0f, 1.0f));
	audioFile.setAudioBuffer(audioBuffer);
	audioFile.setNumChannels(1);
	audioFile.setSampleRate(44100);

	audioFile.save("./WhiteNoise.wav");

	return 0;
}