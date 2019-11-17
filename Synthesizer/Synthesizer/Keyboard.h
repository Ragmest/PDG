#pragma once
#include <vector>
#include <map>

class Keyboard
{
public:
	static void init(const std::vector<bool>& keys);
	static void draw(const std::vector<bool>& keys);
	static void checkStyles();

private:
	static void gotoxy(short x, short y);
	static void drawKey(char c, short offsetX, bool pushed);
	static void drawHalfTone(char c, short offsetX, bool pushed);

	static std::vector<bool> _previousKeyState;
};

