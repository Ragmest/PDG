#include "Generator.h"
#include "Speaker.h"
#include "AudioFile.h"
#include <thread>
#include <atomic>
#include <iostream>
#include <Windows.h>
#include <mutex>
#include <conio.h>
#include <consoleapi.h>
#include "Keyboard.h"

//void gotoxy(short x, short y)
//{
//	HANDLE hStdout = GetStdHandle(STD_OUTPUT_HANDLE);
//	COORD position = { x, y };
//
//	SetConsoleCursorPosition(hStdout, position);
//}
//
//void drawKey(char c, short offset)
//{
//	gotoxy(offset, 0);
//	std::cout << "  |     |"; gotoxy(offset, 1);
//	std::cout << "| |     |"; gotoxy(offset, 2);
//	std::cout << "| |     |"; gotoxy(offset, 3);
//	std::cout << "| |     |"; gotoxy(offset, 4);
//	std::cout << "| |  " << c << "  |"; gotoxy(offset, 5);
//	std::cout << "| |_____|"; gotoxy(offset, 6);
//	std::cout << "|/_____/"; gotoxy(offset, 7);
//}

struct EnvelopeS;

struct Note
{
	int id;
	double on;
	double off;
	double amplitude;
	bool active;

	Note(int id, double on, double off, double amplitude, bool active)
		: id(id), on(on), off(off), amplitude(amplitude), active(active) {}
};

struct EnvelopeS
{
	double attackTime;
	double decayTime;
	double sustainAmplitude;
	double releaseTime;
	double startAmplitude;

	EnvelopeS() :
		attackTime(0.10),
		decayTime(0.01),
		startAmplitude(1.0),
		sustainAmplitude(0.8),
		releaseTime(0.20)
	{
	}

	void updateAmplitude(Note& note, double time)
	{
		double amplitude = 0.0;
		double releaseAmplitude = 0.0;

		if (note.on > note.off)
		{
			if (amplitude > 0.0)
				note.amplitude / startAmplitude;

			double dTime = time - note.on;
			if (dTime <= attackTime)
			{
				amplitude = (dTime / attackTime) * startAmplitude;
			}

			if (dTime > attackTime&& dTime <= (attackTime + decayTime))
			{
				amplitude = ((dTime - attackTime) / decayTime) * (sustainAmplitude - startAmplitude) + startAmplitude;
			}

			if (dTime > (attackTime + decayTime))
			{
				amplitude = sustainAmplitude;
			}
		}
		else
		{
			double dTime = time - note.off;

			amplitude = ((dTime) / releaseTime) * (0.0 - sustainAmplitude) + sustainAmplitude;
		}

		if (amplitude <= 0.0001)
			amplitude = 0.0;

		if (amplitude == 0.0 && note.off > note.on)
		{
			note.active = false;
		}

		note.amplitude = amplitude;
	}

	void setTimeOnWithOffset(double time, Note& note)
	{
		note.on = time - (note.amplitude / startAmplitude) * attackTime;
	}
};

namespace {
	constexpr float PI(3.14159265f);
	constexpr float OCTAVE_BASE_FREQ(440.0f);
	EnvelopeS envelope;
	std::vector<Note> notes;
	std::mutex noteLock;

	constexpr float inverse(float val) { return 1.0f / 12.0f; }
	constexpr float constFactor() { return OCTAVE_BASE_FREQ * 2.0f * PI; };
	auto exponent = [](int k) { return static_cast<float>(k - 12)* inverse(12.0f); };
	auto freq = [](int k) { return constFactor() * pow(2.0f, exponent(k)); };
	auto Hertz = [](int k) { return OCTAVE_BASE_FREQ * pow(2.0f, exponent(k)); };

	auto SIN =      [](int id, float time) { return std::sin(freq(id) * time); };
	auto SQUERE =   [](int id, float time) { return SIN(id, time) > 0.0f ? 1.0f : -1.0f; };
	auto TRIANGLE = [](int id, float time) { return std::asin(SIN(id, time)) * (2.0f * PI); };
	auto SAW =      [](int id, float time) { return (2.0f / PI) * (Hertz(id) * fmod(time, 1.0f / Hertz(id))) - (PI / 2.0f); };
}

float sound(float time)
{
	float out = 0.0f;
	noteLock.lock();
	for (auto& note : notes)
	{
		if (!note.active)
			continue;

		envelope.updateAmplitude(note, time);
		out += note.amplitude * SQUERE(note.id, time);
		out += note.amplitude * SIN(note.id, time);
	}
	noteLock.unlock();
	return out * 0.1f;
}

int main()
{
	Speaker speaker;

	//std::cout << "\n" <<
	//	"  |   |   |   |   |   | |   |   |   |   | |   | |   |   |   |" << "\n" <<
	//	"| |   | S |   |   | F | | G |   |   | J | | K | | L |   |   |" << "\n" <<
	//	"| |   |___|   |   |___| |___|   |   |___| |___| |___|   |   |__" << "\n" <<
	//	"| |     |     |     |     |     |     |     |     |     |     |" << "\n" <<
	//	"| |  Z  |  X  |  C  |  V  |  B  |  N  |  M  |  ,  |  .  |  /  |" << "\n" <<
	//	"| |_____|_____|_____|_____|_____|_____|_____|_____|_____|_____|" << "\n" <<
	//	"|/_____/_____/_____/_____/_____/_____/_____/_____/_____/_____/" << "\n" << "\n";

	std::vector<bool> keyStates(16, false);
	Keyboard::init(keyStates);
	Keyboard::draw(keyStates);

	std::thread speakerThread([&speaker]() { speaker.play(&sound); });

	auto keyPressed = [](SHORT keyState) { return keyState & 0x8000; };
	bool keyStateChanged = false;
	while (true)
	{
		keyStateChanged = false;
		for (int k = 0; k < 16; k++)
		{
			auto keyState = GetAsyncKeyState((unsigned char)("ZSXCFVGBNJMK\xbcL\xbe\xbf"[k]));
			auto now = speaker.getTime();
			auto noteFound = std::find_if(notes.begin(), notes.end(), [&k](const auto& note) { return note.id == k; });
			noteLock.lock();
			bool isKeyPressed = keyPressed(keyState);
			keyStateChanged = (keyStates[k] != isKeyPressed) or keyStateChanged;
			keyStates[k] = isKeyPressed;
			if (noteFound == notes.end())
			{
				if (isKeyPressed)
				{
					notes.emplace_back(k, now, 0.0, 0.0, true);
				}
			}
			else
			{
				if (isKeyPressed)
				{
					if (noteFound->off >= noteFound->on)
					{
						noteFound->on = now;
						noteFound->active = true;
					}
					else
					{
						envelope.setTimeOnWithOffset(now, *noteFound);
					}
				}
				else
				{
					if (noteFound->off < noteFound->on)
						noteFound->off = now;
				}
			}
			noteLock.unlock();
		}
		if(keyStateChanged)
			Keyboard::draw(keyStates);
	}

	return 0;
}