/*
* Lab 01 - B�zier Curves
* 
* Solution by Michele Righi (0001025005)
* GitHub: https://github.com/mikyll/Fondamenti-di-Computer-Graphics-M/tree/main/laboratorio/01es
* 
* 
* Description:
* This program draws straight lines connecting dots placed with mouse clicks.
*
* Usage:
*	Left click to place a control point.
*		(Maximum number of control points allowed is currently set at 300)
*	Press 'f' to remove the first control point
*	Press 'l' to remove the last control point.
*	Drag and drop the mouse while hovering a control point to move it.
*	Press escape to exit.
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

// Personal Libraries
#include "defs.h"
#include "utils.h"

// Exercises
#include "01_3.h"
#include "01_5a.h"
#include "01_5b.h"
//#include "01_5c.h"

using namespace glm;

#define CURSOR_NORMAL GLUT_CURSOR_RIGHT_ARROW
#define CURSOR_HOVER GLUT_CURSOR_DESTROY
#define CURSOR_DRAG GLUT_CURSOR_CYCLE

// FUNCTION DECLARATIONS ========================
void initShader();
void init();

void addNewPoint(Point2D point);
void addNewPoint(float x, float y);
void removeFirstPoint();
void removeLastPoint();

// Function Pointer for getting the curve
void (*getCurve)(Point2D* ctrlPts, int numCtrl, Point2D* curvePts, int* numCurve);

void update(int value);

void resizeWindow(int w, int h);
void inputKeyboard(unsigned char key, int x, int y);
void inputMouseClick(int button, int state, int x, int y);
void inputMouseDrag(int x, int y);
void inputMousePassiveMove(int x, int y);

void drawScene();

// GLOBAL VARIABLES =============================
static unsigned int programId;

// Window size in pixels
int	width = 500;
int	height = 500;

// Vertex Array Objects & Vertex Buffer Objects
// Control Polygon (Points and Segments)
unsigned int VAO_ControlPoints;
unsigned int VBO_ControlPoints;
// Curves
unsigned int VAO_CurvePoints;
unsigned int VBO_CurvePoints;

Point2D ctrlPointArray[MAX_NUM_PTS];
Point2D curvePointArray[(MAX_NUM_PTS*2) - 1];

// Current number of control polygon
int numCtrlPts = 0;

// Num of points used to render the curve
int numCurvePts = 300;

// Size of the Control Points
float ctrlPointSize = 8.0;

// Index of the Control Point the mouse is hovering
int iHoverCtrlPt = -1;

// Used to detect while the user is dragging the mouse
int dragging = 0;

int incrementCtrlPtSize = 0;

// INIT ===================================================
void initShader()
{
	GLenum ErrorCheckValue = glGetError();

	char* vertexShader = (char*)"vertexShader.glsl";
	char* fragmentShader = (char*)"fragmentShader.glsl";

	programId = ShaderMaker::createProgram(vertexShader, fragmentShader);
	glUseProgram(programId);

}

/*
Init VAOs and VBOs
*/
void init()
{
	// VAO for control polygon
	glGenVertexArrays(1, &VAO_ControlPoints);
	glBindVertexArray(VAO_ControlPoints);
	glGenBuffers(1, &VBO_ControlPoints);
	glBindBuffer(GL_ARRAY_BUFFER, VBO_ControlPoints);

	// VAO for curve
	glGenVertexArrays(1, &VAO_CurvePoints);
	glBindVertexArray(VAO_CurvePoints);
	glGenBuffers(1, &VBO_CurvePoints);
	glBindBuffer(GL_ARRAY_BUFFER, VBO_CurvePoints);

	// Background color
	glClearColor(1.0, 1.0, 1.0, 1.0);
	glViewport(0, 0, 500, 500);
}

// LOGIC ==================================================
/*
Remove the first Control Point, moving the rest
of the array by one position.
*/
void removeFirstPoint()
{
	if (numCtrlPts > 0)
	{
		if (iHoverCtrlPt == 0)
		{
			iHoverCtrlPt = -1;
			glutSetCursor(CURSOR_NORMAL);
		}
		else if(iHoverCtrlPt > 0)
			iHoverCtrlPt--;

		// Remove the first point, slide the rest down
		numCtrlPts--;
		for (int i = 0; i < numCtrlPts; i++)
		{
			ctrlPointArray[i].x = ctrlPointArray[i + 1].x;
			ctrlPointArray[i].y = ctrlPointArray[i + 1].y;
		}
	}
}

/*
Remove the last Control Point
*/
void removeLastPoint()
{
	if (numCtrlPts > 0)
	{
		if (iHoverCtrlPt == numCtrlPts - 1)
		{
			iHoverCtrlPt = -1;
			glutSetCursor(CURSOR_NORMAL);
		}
		numCtrlPts--;
	}
}

/*
Add a new control point (to the end of the array).
If there are too many (if MAX is reached) we remove the
first of the array.
*/
void addNewPoint(Point2D point)
{
	if (numCtrlPts >= MAX_NUM_PTS)
	{
		removeFirstPoint();
	}
	ctrlPointArray[numCtrlPts].x = point.x;
	ctrlPointArray[numCtrlPts].y = point.y;
	numCtrlPts++;
}
void addNewPoint(float x, float y)
{
	addNewPoint({x, y});
}

// INPUT ==================================================
void resizeWindow(int w, int h)
{
	height = (h > 1) ? h : 2;
	width = (w > 1) ? w : 2;
	gluOrtho2D(-1.0f, 1.0f, -1.0f, 1.0f);
	glViewport(0, 0, (GLsizei)w, (GLsizei)h);
}

/*
Keyboard Input: normal keys
*/
void inputKeyboard(unsigned char key, int x, int y)
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
	case 27: // Escape key
		exit(0);
		break;
	}
}

/*
Keyboard Input: special keys (down)
*/
void specialKeyDown(int key, int x, int y)
{
	switch (key) {
	case GLUT_KEY_UP:
		incrementCtrlPtSize = 1;
		break;
	case GLUT_KEY_DOWN:
		incrementCtrlPtSize = -1;
		break;
	// Algorithm Selection
	case GLUT_KEY_F1:
		getCurve = getCurve_deCasteljau;
		break;
	case GLUT_KEY_F2:
		getCurve = getCurve_adaptiveSubdivision;
		break;
	}
}
/*
Keyboard Input: special keys (up)
*/
void specialKeyUp(int key, int x, int y)
{
	if(key == GLUT_KEY_UP || key == GLUT_KEY_DOWN)
		incrementCtrlPtSize = 0;
}

/*
Mouse Input: click
*/
void inputMouseClick(int button, int state, int x, int y)
{
	if (button == GLUT_LEFT_BUTTON)
	{
		if (state == GLUT_DOWN)
		{
			// If there is already a Control Point in (x,y) => drag
			if (iHoverCtrlPt != -1)
			{
				dragging = 1;
				glutSetCursor(CURSOR_DRAG);
			}
			// Otherwise => add a new Control Point
			else
			{
				// (x,y) viewport(0,width)x(0,height)   -->   (xPos,yPos) window(-1,1)x(-1,1)
				Point2D point = {
					-1.0f + ((float)x) * 2 / ((float)(width)),
					-1.0f + ((float)(height - y)) * 2 / ((float)(height)),
				};
				
				addNewPoint(point);
				glutPostRedisplay();
			}
		}
		if (state == GLUT_UP)
		{
			dragging = 0;
			glutSetCursor(CURSOR_NORMAL);
		}
	}
}

/*
Mouse Input: move
*/
void inputMousePassiveMove(int x, int y)
{
	float xMousePos = -1.0f + ((float)x) * 2 / ((float)(width));
	float yMousePos = -1.0f + ((float)(height - y)) * 2 / ((float)(height));

	float sizeOffsetX = ctrlPointSize / (width);
	float sizeOffsetY = ctrlPointSize / (height);

	/*
	Check if the mouse is hovering a Control Point.
	If it is: store in iHoverCtrlPt the index of the Control Point
	that (xMousePos,yMousePos) are inside.
	Otherwise: set iHoverCtrlPt to -1 (it's not hovering
	a control point).
	*/
	for (int i = 0; i < numCtrlPts; i++)
	{
		float minX, maxX, minY, maxY;
		minX = ctrlPointArray[i].x - sizeOffsetX;
		maxX = ctrlPointArray[i].x + sizeOffsetX;
		minY = ctrlPointArray[i].y - sizeOffsetY;
		maxY = ctrlPointArray[i].y + sizeOffsetY;

		// Mouse coords inside the point
		if (minX <= xMousePos && xMousePos <= maxX &&
			minY <= yMousePos && yMousePos <= maxY)
		{
			iHoverCtrlPt = i;
			glutSetCursor(CURSOR_HOVER);
			break;
		}
		else
		{
			iHoverCtrlPt = -1;
			glutSetCursor(CURSOR_NORMAL);
		}
	}
}

// ES 4: Drag & drop control points
/*
Mouse Input: drag (move while pressing)
*/
void inputMouseDrag(int x, int y)
{
	if (dragging)
	{
		float xPos = -1.0f + ((float)x) * 2 / ((float)(width));
		float yPos = -1.0f + ((float)(height - y)) * 2 / ((float)(height));
		
		// MIN/MAX to limit the dragging to end up inside the window
		ctrlPointArray[iHoverCtrlPt].x = MIN(1.0, MAX(-1.0, xPos));
		ctrlPointArray[iHoverCtrlPt].y = MIN(1.0, MAX(-1.0, yPos));
	}
}

// test
/*void test()
{
	Point2D arr1[3] = { {-0.596,0.332}, {-0.088,0.556}, {0.324, 0.34} };
	Point2D arr2[3] = { {-0.2,-0.2}, {-0.3,-0.3}, {-0.4,-0.4} };

	glBindVertexArray(VAO_CurvePoints);
	glBindBuffer(GL_ARRAY_BUFFER, VBO_CurvePoints);
	glBufferData(GL_ARRAY_BUFFER, sizeof(arr1), &arr1[0], GL_STATIC_DRAW);
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	// Draw the segments that form the line
	glLineWidth(0.5);
	glDrawArrays(GL_LINE_STRIP, 0, 2);
	glBindVertexArray(0);

	glBindVertexArray(VAO_CurvePoints);
	glBindBuffer(GL_ARRAY_BUFFER, VBO_CurvePoints);
	glBufferData(GL_ARRAY_BUFFER, sizeof(arr2), &arr2[0], GL_STATIC_DRAW);
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	// Draw the segments that form the line
	glLineWidth(0.5);
	glDrawArrays(GL_LINE_STRIP, 0, 3);
	glBindVertexArray(0);
}*/

// UPDATE =================================================
void update(int value)
{
	ctrlPointSize = MAX(MIN_CTRL_PTS_SIZE, MIN(MAX_CTRL_PTS_SIZE, ctrlPointSize + 0.1 * incrementCtrlPtSize));

	// Update curve: draw the curve only if there are at least 2 control points
	if (numCtrlPts > 1)
	{
		if(getCurve)
			getCurve(ctrlPointArray, numCtrlPts, curvePointArray, &numCurvePts);
	}

	glutPostRedisplay();
	glutTimerFunc(20, update, 0);
}

// DRAW ===================================================
void drawScene()
{
	glClear(GL_COLOR_BUFFER_BIT);

	// Draw control polygon
	glBindVertexArray(VAO_ControlPoints);
	glBindBuffer(GL_ARRAY_BUFFER, VBO_ControlPoints);
	glBufferData(GL_ARRAY_BUFFER, sizeof(ctrlPointArray), &ctrlPointArray[0], GL_STATIC_DRAW);
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	// Draw the control points CP
	glPointSize(ctrlPointSize);
	glDrawArrays(GL_POINTS, 0, numCtrlPts);
	// Draw the line segments between CP
	glLineWidth(2.0);
	glDrawArrays(GL_LINE_STRIP, 0, numCtrlPts);
	glBindVertexArray(0);

	if (numCtrlPts > 1)
	{
		// Draw curve
		glBindVertexArray(VAO_CurvePoints);
		glBindBuffer(GL_ARRAY_BUFFER, VBO_CurvePoints);
		glBufferData(GL_ARRAY_BUFFER, sizeof(curvePointArray), &curvePointArray[0], GL_STATIC_DRAW);
		glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), (void*)0);
		glEnableVertexAttribArray(0);
		// Draw the segments that form the line
		glLineWidth(0.5);
		glDrawArrays(GL_LINE_STRIP, 0, numCurvePts);
		glBindVertexArray(0);
	}

	glutSwapBuffers();
}

// MAIN ===================================================
int main(int argc, char** argv)
{
	glutInit(&argc, argv);

	glutInitContextVersion(4, 0);
	glutInitContextProfile(GLUT_CORE_PROFILE);

	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA);

	glutInitWindowSize(width, height);
	glutInitWindowPosition(100, 100);
	glutCreateWindow("Draw curves 2D");

	// Update callback
	glutTimerFunc(20, update, 0);

	// Input callbacks
	glutSetKeyRepeat(GLUT_KEY_REPEAT_OFF);
	glutKeyboardFunc(inputKeyboard);
	glutSpecialFunc(specialKeyDown);
	glutSpecialUpFunc(specialKeyUp);
	glutMouseFunc(inputMouseClick);
	glutPassiveMotionFunc(inputMousePassiveMove);
	glutMotionFunc(inputMouseDrag);
	
	// Draw callback
	glutDisplayFunc(drawScene);

	glewExperimental = GL_TRUE;
	glewInit();

	initShader();
	init();

	glutMainLoop();

	return 0;
}