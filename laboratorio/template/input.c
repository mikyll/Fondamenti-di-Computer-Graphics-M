#include "input.h"

#include <stdio.h>

void initInput()
{
	memset(&input, 0, sizeof(Input));
}

void keyDown(unsigned char key, int x, int y)
{
	input.keys[key] = 1;
}

void keyUp(unsigned char key, int x, int y)
{
	input.keys[key] = 0;
}

void specialKeyDown(unsigned char key, int x, int y)
{
	input.specialKeys[key] = 1;
}

void specialKeyUp(unsigned char key, int x, int y)
{
	input.specialKeys[key] = 0;
}

void mouse(GLint button, GLint state, GLint x, GLint y)
{
	input.mouseKeys[button] = state;
	input.mouse.x = x;
	input.mouse.y = y;
}