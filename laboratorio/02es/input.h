#pragma once

#include "commons.h"

typedef struct {
	unsigned char keys[256];
	unsigned char specialKeys[256];
} Keyboard;

typedef struct {
	unsigned char keys[3];
	int x, y;
} Mouse;

typedef struct {
	Keyboard keyboard;
	Mouse mouse;
} Input;

void keyDown(unsigned char key, int x, int y);
void keyUp(unsigned char key, int x, int y);
void specialKeyDown(int key, int x, int y);
void specialKeyUp(int key, int x, int y);
void mouseClick(GLint button, GLint state, GLint x, GLint y);
void moveMouse(GLint x, GLint y);