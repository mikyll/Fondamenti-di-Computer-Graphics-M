#include <GL/glew.h>
#include <GL/freeglut.h>
#include <stdio.h>

#include "input.h"

int mode = GL_POINTS;

int enableLeft = 1;
int enableRight = 1;

void drawScene()
{
	glClear(GL_COLOR_BUFFER_BIT);

	GL_TRIANGLE_FAN;
	glBegin(mode); //starts drawing of points
	glColor3f(1.0, 0.0, 0.0);
	glVertex3f(0.0f, 0.0f, 0.0f);//upper-right corner
	glVertex3f(-1.0f, -1.0f, 0.0f);//lower-left corner

	glColor3f(0.0, 0.0, 1.0);
	glVertex3f(1.0f, 0.0f, 0.0f);//upper-right corner
	glVertex3f(0.0f, -1.0f, 0.0f);//lower-left corner

	glEnd(); //end drawing of points

	glutSwapBuffers();
}

void doInput()
{
	if (input.specialKeys[GLUT_KEY_LEFT])
	{
		if (mode > 0 && enableLeft)
		{
			mode--;
			enableLeft = 0;
		}
	}
	else enableLeft = 1;


	if (input.specialKeys[GLUT_KEY_RIGHT])
	{
		if (mode < GL_TRIANGLE_FAN && enableRight)
		{
			mode++;
			enableRight = 0;
		}
	}
	else enableRight = 1;
}

void update(int value)
{
	doInput();
	glutPostRedisplay();
	glutTimerFunc(20, update, 0);
}

int main(int argc, char** argv)
{
	glutInit(&argc, argv);
	glutInitContextProfile(GLUT_CORE_PROFILE);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA);
	glutInitWindowSize(500, 500);
	glutInitWindowPosition(0, 0);
	glutCreateWindow("simple");



	// Input
	initInput();
	glutSetKeyRepeat(GLUT_KEY_REPEAT_OFF);
	glutKeyboardFunc(keyDown);
	glutKeyboardUpFunc(keyUp);
	glutSpecialFunc(specialKeyDown);
	glutSpecialUpFunc(specialKeyUp);
	glutMouseFunc(mouseClick);
	glutPassiveMotionFunc(moveMouse);

	// update
	glutTimerFunc(20, update, 0);


	//set background color
	glClearColor(1.0, 0.5, 0.0, 1.0);
	// Draw
	glutDisplayFunc(drawScene);

	// Shaders
	/*initShader()
		init();*/


	glutMainLoop();
}