#ifndef INPUT_H_
#define INPUT_H_

#include <GL/freeglut.h>

typedef struct {
	int x, y;
} Mouse;

typedef struct {
	unsigned char keys[256];
	unsigned char specialKeys[256];
	unsigned char mouseKeys[3];
	Mouse mouse;
} Input;


Input input;

void initInput();
void keyDown(unsigned char key, int x, int y);
void keyUp(unsigned char key, int x, int y);
void specialKeyDown(unsigned char key, int x, int y);
void specialKeyUp(unsigned char key, int x, int y);
void mouseClick(GLint button, GLint state, GLint x, GLint y);
void moveMouse(GLint x, GLint y);

#endif