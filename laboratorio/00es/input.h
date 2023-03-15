#ifndef INPUT_H_
#define INPUT_H_

#ifdef __cplusplus
extern "C" {
#endif 

	#include <GL/freeglut.h>

	typedef struct {
		unsigned char keys[3];
		int x, y;
	} Mouse;

	typedef struct {
		unsigned char keys[256];
		unsigned char specialKeys[256];
		Mouse mouse;
	} Input;


	Input input;

	void initInput(int autoInit);
	void keyDown(unsigned char key, int x, int y);
	void keyUp(unsigned char key, int x, int y);
	void specialKeyDown(int key, int x, int y);
	void specialKeyUp(int key, int x, int y);
	void mouseClick(GLint button, GLint state, GLint x, GLint y);
	void moveMouse(GLint x, GLint y);

#ifdef __cplusplus
}
#endif

#endif // !INPUT_H