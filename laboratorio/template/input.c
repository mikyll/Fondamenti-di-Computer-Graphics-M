#include "input.h"

void initInput()
{
	memset(&input, 0, sizeof(Input));
}

void keyDown(unsigned char key, int x, int y)
{
	input.keyboard.keys[key] = 1;
}

void keyUp(unsigned char key, int x, int y)
{
	input.keyboard.keys[key] = 0;
}

void specialKeyDown(int key, int x, int y)
{
	input.keyboard.specialKeys[key] = 1;
}

void specialKeyUp(int key, int x, int y)
{
	input.keyboard.specialKeys[key] = 0;
}

void mouseClick(GLint button, GLint state, GLint x, GLint y)
{
	input.mouse.keys[button] = !state;
	input.mouse.x = x;
	input.mouse.y = y;
}

void moveMouse(GLint x, GLint y)
{
	input.mouse.x = x;
	input.mouse.y = y;
}