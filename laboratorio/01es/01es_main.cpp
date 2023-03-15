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

#include "text.h"

static unsigned int programId;

unsigned int VAO;
unsigned int VBO;

unsigned int VAO_2;
unsigned int VBO_2;


#define MaxNumPts 300
float PointArray[MaxNumPts][2];
float CurveArray[MaxNumPts][2];

int NumPts = 0;

// Window size in pixels
int		width = 500;
int		height = 500;

/* Prototypes */
void addNewPoint(float x, float y);
int main(int argc, char** argv);
void removeFirstPoint();
void removeLastPoint();

typedef struct {
	float x, y;
} Point2D;

typedef struct {
	float x, y, z;
} Point3D;

int changed = 0;

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

void resizeWindow(int w, int h)
{
	height = (h > 1) ? h : 2;
	width = (w > 1) ? w : 2;
	gluOrtho2D(-1.0f, 1.0f, -1.0f, 1.0f);
	glViewport(0, 0, (GLsizei)w, (GLsizei)h);
}

// Left button presses place a new control point.
void myMouseFunc(int button, int state, int x, int y) {
	if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN)
	{
		// (x,y) viewport(0,width)x(0,height)   -->   (xPos,yPos) window(-1,1)x(-1,1)
		float xPos = -1.0f + ((float)x) * 2 / ((float)(width));
		float yPos = -1.0f + ((float)(height - y)) * 2 / ((float)(height));

		addNewPoint(xPos, yPos);
		glutPostRedisplay();

		changed = 1;
	}
}

void removeFirstPoint() {
	int i;
	if (NumPts > 0) {
		// Remove the first point, slide the rest down
		NumPts--;
		for (i = 0; i < NumPts; i++) {
			PointArray[i][0] = PointArray[i + 1][0];
			PointArray[i][1] = PointArray[i + 1][1];
		}
	}
}

// Add a new point to the end of the list.  
// Remove the first point in the list if too many points.
void removeLastPoint() {
	if (NumPts > 0) {
		NumPts--;
	}
}

// Add a new point to the end of the list.  
// Remove the first point in the list if too many points.
void addNewPoint(float x, float y) {
	if (NumPts >= MaxNumPts) {
		removeFirstPoint();
	}
	PointArray[NumPts][0] = x;
	PointArray[NumPts][1] = y;
	NumPts++;
}
void initShader(void)
{
	GLenum ErrorCheckValue = glGetError();

	char* vertexShader = (char*)"vertexShader.glsl";
	char* fragmentShader = (char*)"fragmentShader.glsl";

	programId = ShaderMaker::createProgram(vertexShader, fragmentShader);
	glUseProgram(programId);

}


void init(void)
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

Point2D deCasteljau(float points[MaxNumPts][2], int numPoints, float t)
{
	// Auxiliary array (since we edit it in each iteration, we don't want to lose the initial array)
	Point2D pointArrayAux[MaxNumPts];
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

void drawScene(void)
{
	glClear(GL_COLOR_BUFFER_BIT);

	// If there are at least 2 points
	if (NumPts > 1) {
		int numPtsCurve = 100;

		// Draw curve
		if (changed)
		{
			for (int i = 0; i < numPtsCurve; i++)
			{
				Point2D res = deCasteljau(PointArray, NumPts, (float)i / numPtsCurve);
				CurveArray[i][0] = res.x;
				CurveArray[i][1] = res.y;
			}

			changed = 0;
		}

		// Draw curve
		glBindVertexArray(VAO_2);
		glBindBuffer(GL_ARRAY_BUFFER, VBO_2);
		glBufferData(GL_ARRAY_BUFFER, sizeof(CurveArray), &CurveArray[0], GL_STATIC_DRAW);
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
	glBufferData(GL_ARRAY_BUFFER, sizeof(PointArray), &PointArray[0], GL_STATIC_DRAW);
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	// Draw the control points CP
	glPointSize(6.0);
	glDrawArrays(GL_POINTS, 0, NumPts);
	// Draw the line segments between CP
	glLineWidth(2.0);
	glDrawArrays(GL_LINE_STRIP, 0, NumPts);
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

	glutDisplayFunc(drawScene);
	glutReshapeFunc(resizeWindow);
	glutKeyboardFunc(myKeyboardFunc);
	glutMouseFunc(myMouseFunc);

	glewExperimental = GL_TRUE;
	glewInit();

	initShader();
	init();


	//test
	/*printf("\n\n");
	float arr[4][2] = { { 1.0, 1.0 }, { 1.0, 2.0 }, { 5.0, 5.0 }, { 6.0, 6.0 } };
	float t = 0.5;

	Point2D p1 = { 1.0, 1.0 }, p2 = { 1.0, 2.0 }, p3 = { 5.0, 5.0 }, p4 = { 6.0, 6.0 };
	Point2D p12 = lerp(p1, p2, t), p23 = lerp(p2, p3, t), p34 = lerp(p2, p3, t);
	Point2D res = deCasteljau(arr, 4, t);

	printf("lerp_P1-P2: (%1.1f,%1.1f), lerp_P2-P3: (%1.1f,%1.1f)\nres: (%1.1f,%1.1f)\n", p12.x, p12.y, p23.x, p23.y, res.x, res.y);

	return 0;*/

	glutMainLoop();
}
