#include <GL/glew.h>
#include <GL/freeglut.h>
#include <stdio.h>

#include "input.h"
#include "text.h"

#define MIN(a,b) (((a)<(b))?(a):(b))
#define MAX(a,b) (((a)>(b))?(a):(b))

int help = 0;
int mode = GL_POINTS;
GLfloat pointSize = 1.0;
GLfloat lineWidth = 1.0;

int enableLeft = 1;
int enableRight = 1;

char modes[10][32] = {
	"GL_POINTS",
	"GL_LINES",
	"GL_LINE_LOOP",
	"GL_LINE_STRIP",
	"GL_TRIANGLES",
	"GL_TRIANGLE_STRIP",
	"GL_TRIANGLE_FAN",
	"GL_QUADS",
	"GL_QUAD_STRIP",
	"GL_POLYGON",
};

typedef struct {
	GLfloat x, y, z;
} Point3D;
typedef struct {
	GLfloat r, g, b;
} ColorRGB;

Point3D points[5] = {
	{0.2f, 0.0f, 0.0f},
	{-0.5f, 0.1f, 0.0f},
	{0.0f, 0.2f, 0.0f},
	{-0.7f, 0.5f, 0.0f},
	{-0.5, -0.2f},
};
ColorRGB colors[5] = {
	{1.0f, 0.0f, 0.0f},
	{0.0f, 1.0f, 0.0f},
	{0.0f, 0.0f, 1.0f},
	{1.0f, 1.0f, 0.0f},
	{1.0f, 0.0f, 1.0f},
};

void drawFigure()
{
	glPointSize(pointSize);
	glLineWidth(lineWidth);

	// Start Drawing (Immediate Mode)
	glBegin(mode);
	for (int i = 0; i < 5; i++)
	{
		glColor3f(colors[i].r, colors[i].g, colors[i].b);
		glVertex3f(points[i].x, points[i].y, points[i].z);
	}
	// End Drawing (Immediate Mode)
	glEnd();

	// Draw points names
	for (int i = 0; i < 5; i++)
	{
		char out[30];
		sprintf_s(out, 30, "P%d", i);
		RenderString(points[i].x + 0.01, points[i].y + 0.01, GLUT_BITMAP_HELVETICA_12, out, 1.0f, 1.0f, 1.0f);
	}
}

void drawModeInfo()
{
	// Mode rectangle
	glBegin(GL_TRIANGLE_STRIP);
	glColor3f(1.0, 1.0, 1.0);
	glVertex3f(-1.0f, -1.0f, 0.0f);
	glVertex3f(0.0f, -1.0f, 0.0f);
	glVertex3f(0.0f, -0.9f, 0.0f);

	glVertex3f(0.0f, -0.9f, 0.0f);
	glVertex3f(-1.0f, -0.9f, 0.0f);
	glVertex3f(-1.0f, -1.0f, 0.0f);
	glEnd();

	// Mode text
	char out[30];
	sprintf_s(out, 30, "Mode: %s", modes[mode]);
	RenderString(-0.99, -0.99, GLUT_BITMAP_HELVETICA_18, out, 0.0f, 0.0f, 0.0f);

	sprintf_s(out, 30, "Point size: %1.3f", pointSize);
	RenderString(-0.95, 0.92, GLUT_BITMAP_HELVETICA_18, out, 0.0f, 0.0f, 0.0f);
	
	sprintf_s(out, 30, "Line width: %1.3f", lineWidth);
	RenderString(-0.95, 0.85, GLUT_BITMAP_HELVETICA_18, out, 0.0f, 0.0f, 0.0f);
}

void drawInfo()
{
	// Info rectangle
	glBegin(GL_TRIANGLE_STRIP);
	glColor3f(1.0f, 1.0f, 1.0f);
	glVertex3f(-0.8f, -0.8f, 0.0f);
	glVertex3f(0.8f, -0.8f, 0.0f);
	glVertex3f(0.8f, 0.8f, 0.0f);

	glVertex3f(0.8f, 0.8f, 0.0f);
	glVertex3f(-0.8f, 0.8f, 0.0f);
	glVertex3f(-0.8f, -0.8f, 0.0f);
	glEnd();

	RenderString(-0.1, 0.7, GLUT_BITMAP_HELVETICA_18, "Help", 0.0f, 0.0f, 0.0f);
	RenderString(-0.75, 0.5, GLUT_BITMAP_HELVETICA_12, "Left/Right arrow: change mode", 0.0f, 0.0f, 0.0f);
	RenderString(-0.75, 0.4, GLUT_BITMAP_HELVETICA_12, "Up/Down arrow: change point size/line width", 0.0f, 0.0f, 0.0f);
}

void drawHelp()
{
	RenderString(0.3, -0.96, GLUT_BITMAP_HELVETICA_12, "Press 'h' to show help", 0.0f, 0.0f, 0.0f);
}

void drawScene()
{
	glClear(GL_COLOR_BUFFER_BIT);

	drawFigure();
	drawModeInfo();
	if (!help)
		drawHelp();
	else drawInfo();

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
		if (mode < GL_POLYGON && enableRight)
		{
			mode++;
			enableRight = 0;
		}
	}
	else enableRight = 1;

	if (input.specialKeys[GLUT_KEY_UP])
	{
		if (mode == GL_POINTS)
		{
			pointSize += 0.1;
			pointSize = MIN(pointSize, 10.0);
		}
		else if (mode == GL_LINES || 
			mode == GL_LINE_LOOP || mode == GL_LINE_STRIP)
		{
			lineWidth += 0.1;
			lineWidth = MIN(lineWidth, 10.0);
		}
	}
	if (input.specialKeys[GLUT_KEY_DOWN])
	{
		if (mode == GL_POINTS)
		{
			pointSize -= 0.1;
			pointSize = MAX(pointSize, 1.0);
		}
		else if (mode == GL_LINES ||
			mode == GL_LINE_LOOP || mode == GL_LINE_STRIP)
		{
			lineWidth -= 0.1;
			lineWidth = MAX(lineWidth, 1.0);
		}
	}

	help = input.keys['h'];
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
	glutCreateWindow("Template");

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