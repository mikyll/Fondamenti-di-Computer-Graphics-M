/*
 * Lab-01_students.c
 *
 *     This program draws straight lines connecting dots placed with mouse clicks.
 *
 * Usage:
 *   Left click to place a control point.
 *		Maximum number of control points allowed is currently set at 64.
 *	 Press "f" to remove the first control point
 *	 Press "l" to remove the last control point.
 *	 Press escape to exit.
 */


#include <iostream>
#include "ShaderMaker.h"
#include <GL/glew.h>
#include <GL/freeglut.h>
#include <vector>

 // Include GLM
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/transform.hpp>

using namespace glm;

#define MAX_NUM_PTS 300

typedef struct {
	float x, y;
} Point2D;

// FUNCTION DECLARATIONS ========================
void resizeWindow(int w, int h);
void myKeyboardFunc(unsigned char key, int x, int y);
void myMouseFunc(int button, int state, int x, int y);

void initShader();
void init();

void addNewPoint(float x, float y);
void removeFirstPoint();
void removeLastPoint();

Point2D lerp(Point2D a, Point2D b, float t);
void lerp2(float a[2], float b[2], float t, float res[2]);
Point2D deCasteljau(float points[MAX_NUM_PTS][2], int numPoints, float t);

void drawScene();

// GLOBAL VARIABLES =============================
static unsigned int programId;

// Vertex Array Objects & Vertex Buffer Objects
// Control Polygon (Points and Segments)
unsigned int VAO;
unsigned int VBO;
// Curves
unsigned int VAO_2;
unsigned int VBO_2;

float ctrlPointArray[MAX_NUM_PTS][2];
float curvePointArray[MAX_NUM_PTS][2];

// current number of control polygon
int numPts = 0;

// Num of points used to render the curve
int numPtsCurve = 100;

// Window size in pixels
int		width = 500;
int		height = 500;

// FUNCTION DEFINITIONS =========================
void resizeWindow(int w, int h)
{
	height = (h > 1) ? h : 2;
	width = (w > 1) ? w : 2;
	gluOrtho2D(-1.0f, 1.0f, -1.0f, 1.0f);
	glViewport(0, 0, (GLsizei)w, (GLsizei)h);
}

/*
* Keyboard Input
*/
void myKeyboardFunc(unsigned char key, int x, int y)
{
	switch (key) {
	case 'f':
		removeFirstPoint();
		glutPostRedisplay();
		break;
	case 'l':
		removeLastPoint();
		glutPostRedisplay();
		break;
	case 27:			// Escape key
		exit(0);
		break;
	}
}

/*
* Mouse Input
*/
void myMouseFunc(int button, int state, int x, int y)
{
	if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN)
	{
		// (x,y) viewport(0,width)x(0,height)   -->   (xPos,yPos) window(-1,1)x(-1,1)
		float xPos = -1.0f + ((float)x) * 2 / ((float)(width));
		float yPos = -1.0f + ((float)(height - y)) * 2 / ((float)(height));

		addNewPoint(xPos, yPos);
		glutPostRedisplay();
	}
}

void initShader()
{
	GLenum ErrorCheckValue = glGetError();

	char* vertexShader = (char*)"vertexShader.glsl";
	char* fragmentShader = (char*)"fragmentShader.glsl";

	programId = ShaderMaker::createProgram(vertexShader, fragmentShader);
	glUseProgram(programId);

}

/*
* Init VAOs and VBOs
*/
void init()
{
	// VAO for control polygon
	glGenVertexArrays(1, &VAO);
	glBindVertexArray(VAO);
	glGenBuffers(1, &VBO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);

	// VAO for curve
	glGenVertexArrays(1, &VAO_2);
	glBindVertexArray(VAO_2);
	glGenBuffers(1, &VBO_2);
	glBindBuffer(GL_ARRAY_BUFFER, VBO_2);

	// Background color
	glClearColor(1.0, 1.0, 1.0, 1.0);
	glViewport(0, 0, 500, 500);
}

/*
* Remove the first Control Point, moving the rest
* of the array by one position.
*/
void removeFirstPoint()
{
	if (numPts > 0)
	{
		// Remove the first point, slide the rest down
		numPts--;
		for (int i = 0; i < numPts; i++)
		{
			ctrlPointArray[i][0] = ctrlPointArray[i + 1][0];
			ctrlPointArray[i][1] = ctrlPointArray[i + 1][1];
		}
	}
}

/*
* Remove the last Control Point
*/
void removeLastPoint()
{
	if (numPts > 0)
	{
		numPts--;
	}
}

/*
* Add a new control point (to the end of the array).
* If there are too many (if MAX is reached) we remove the
* first of the array.
*/
void addNewPoint(float x, float y)
{
	if (numPts >= MAX_NUM_PTS)
	{
		removeFirstPoint();
	}
	ctrlPointArray[numPts][0] = x;
	ctrlPointArray[numPts][1] = y;
	numPts++;
}

/*
* Return the linear iterpolation of two points a and b.
*/
Point2D lerp(Point2D a, Point2D b, float t)
{
	Point2D res;
	res.x = (1 - t) * a.x + t * b.x;
	res.y = (1 - t) * a.y + t * b.y;

	return res;
}

void lerp2(float a[2], float b[2], float t, float res[2])
{
	res[0] = (1 - t) * a[0] + t * b[0];
	res[1] = (1 - t) * a[1] + t * b[1];
}

/*
* Given an array of points, evaluate a point of the Bézier curve in t.
*/
Point2D deCasteljau(float points[MAX_NUM_PTS][2], int numPoints, float t)
{
	// Auxiliary array (since we edit it in each iteration, we don't want to lose the initial array)
	Point2D pointArrayAux[MAX_NUM_PTS];
	pointArrayAux[0] = { 0,0 };

	// Copy
	for (int i = 0; i < numPoints; i++)
	{
		pointArrayAux[i].x = points[i][0];
		pointArrayAux[i].y = points[i][1];
	}
	
	// "Depth" loop
	for (int i = 1; i < numPoints; i++)
	{
		// Points loop
		for (int j = 0; j < numPoints - 1; j++)
		{
			pointArrayAux[j] = lerp(pointArrayAux[j], pointArrayAux[j + 1], t);
		}
	}

	return pointArrayAux[0];
}

void drawScene()
{
	glClear(GL_COLOR_BUFFER_BIT);

	// Draw the curve if there's at least 2 points
	if (numPts > 1)
	{
		for (int i = 0; i < numPtsCurve; i++)
		{
			Point2D res = deCasteljau(ctrlPointArray, numPts, (float)i / numPtsCurve);
			curvePointArray[i][0] = res.x;
			curvePointArray[i][1] = res.y;
		}

		// Draw curve
		glBindVertexArray(VAO_2);
		glBindBuffer(GL_ARRAY_BUFFER, VBO_2);
		glBufferData(GL_ARRAY_BUFFER, sizeof(curvePointArray), &curvePointArray[0], GL_STATIC_DRAW);
		glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), (void*)0);
		glEnableVertexAttribArray(0);
		glPointSize(1.0);
		glLineWidth(0.5);
		glDrawArrays(GL_LINE_STRIP, 0, numPtsCurve);
		glBindVertexArray(0);
	}

	// Draw control polygon
	glBindVertexArray(VAO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(ctrlPointArray), &ctrlPointArray[0], GL_STATIC_DRAW);
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	// Draw the control points CP
	glPointSize(6.0);
	glDrawArrays(GL_POINTS, 0, numPts);
	// Draw the line segments between CP
	glLineWidth(2.0);
	glDrawArrays(GL_LINE_STRIP, 0, numPts);
	glBindVertexArray(0);

	glutSwapBuffers();
}

int main(int argc, char** argv)
{
	glutInit(&argc, argv);

	glutInitContextVersion(4, 0);
	glutInitContextProfile(GLUT_CORE_PROFILE);

	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA);

	glutInitWindowSize(width, height);
	glutInitWindowPosition(100, 100);
	glutCreateWindow("Draw curves 2D");

	glutReshapeFunc(resizeWindow);
	glutKeyboardFunc(myKeyboardFunc);
	glutMouseFunc(myMouseFunc);
	glutDisplayFunc(drawScene);

	glewExperimental = GL_TRUE;
	glewInit();

	initShader();
	init();

	glutMainLoop();

	return 0;
}
