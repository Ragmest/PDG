#include "Keyboard.h"
#include <iostream>
#include <Windows.h>
#include <conio.h>
#include <consoleapi.h>
#include <map>
#include <functional>

namespace
{
	const unsigned char keyTable[] = "ZSXCFVGBNJMK,L./";
	constexpr int STANDARD(15);
	constexpr int WHITE(240);
}

std::vector<bool> Keyboard::_previousKeyState;

void Keyboard::init(const std::vector<bool>& keys)
{
	_previousKeyState = keys;
}

void Keyboard::draw(const std::vector<bool>& keys)
{
	system("cls");
	drawKey(keyTable[15], 60, keys[15]);
	drawKey(keyTable[14], 54, keys[14]);
	drawKey(keyTable[12], 48, keys[12]);
	drawKey(keyTable[10], 42, keys[10]);
	drawKey(keyTable[8], 36, keys[8]);
	drawKey(keyTable[7], 30, keys[7]);
	drawKey(keyTable[5], 24, keys[5]);
	drawKey(keyTable[3], 18, keys[3]);
	drawKey(keyTable[2], 12, keys[2]);
	drawKey(keyTable[0], 6, keys[0]);

	drawHalfTone(keyTable[13], 54, keys[13]);
	drawHalfTone(keyTable[11], 48, keys[11]);
	drawHalfTone(keyTable[9], 42, keys[9]);
	drawHalfTone(keyTable[6], 30, keys[6]);
	drawHalfTone(keyTable[4], 24, keys[4]);
	drawHalfTone(keyTable[1], 12, keys[1]);
}

void Keyboard::checkStyles()
{
	HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
	for (int k = 1; k < 255; k++)
	{
		SetConsoleTextAttribute(hConsole, k);
		std::cout << k << " Style " << std::endl;
	}
}

void Keyboard::drawKey(char c, short offsetX, bool pushed)
{
	HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
	int offsetY = pushed ? 1 : 0;
	if (pushed)
		offsetX -= 1;
	gotoxy(offsetX, offsetY + 0);
	std::cout << "  |     |"; gotoxy(offsetX, offsetY + 1);
	std::cout << "| |     |"; gotoxy(offsetX, offsetY + 2);
	std::cout << "| |     |"; gotoxy(offsetX, offsetY + 3);
	std::cout << "| |     |"; gotoxy(offsetX, offsetY + 4);
	std::cout << "| |  " << c << "  |"; gotoxy(offsetX, offsetY + 5);
	std::cout << "| |_____|"; gotoxy(offsetX, offsetY + 6);
	std::cout << "|/_____/"; gotoxy(offsetX, offsetY + 7);
}

void Keyboard::drawHalfTone(char c, short offsetX, bool pushed)
{
	HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
	gotoxy(offsetX, 0);
	SetConsoleTextAttribute(hConsole, pushed ? STANDARD : WHITE);
	std::cout << "|   |"; gotoxy(offsetX, 1);
	std::cout << "| " << c << " |"; gotoxy(offsetX, 2);
	std::cout << "|___|"; gotoxy(offsetX, 3);
	SetConsoleTextAttribute(hConsole, STANDARD);
}

void Keyboard::gotoxy(short x, short y)
{
	HANDLE hStdout = GetStdHandle(STD_OUTPUT_HANDLE);
	COORD position = { x, y };

	SetConsoleCursorPosition(hStdout, position);
}