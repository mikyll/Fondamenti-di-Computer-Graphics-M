/*
* Lab 00 - OpenGL Introduction
*
* Solution by Michele Righi (0001025005)
* GitHub: https://github.com/mikyll/Fondamenti-di-Computer-Graphics-M/tree/main/laboratorio/00es
*
*
* Description:
* This program draws a single 3D cube with faces of different flat colors
*	(red, green, blue, yellow, magenta, cyan) and allows the user to
*	interact with it.
*
Usage:
*	- Press 'w' to move the camera forwards.
*	- Press 'a' to move the camera to the left.
*	- Press 's' to move the camera backwards.
*	- Press 'd' to move the camera to the right.
*	- Press SPACE to move the camera upwards.
*	- Press MAIUSC to move the camera downwards.
*	- Press 't' to reset the camera position.
* 
*	- Press up arrow to rotate the cube on x-axis clockwise.
*	- Press down arrow to rotate the cube on x-axis counter-clockwise.
*	- Press left arrow to rotate the cube on y-axis clockwise.
*	- Press right arrow to rotate the cube on y-axis counter-clockwise.
*	- Press page-up to rotate the cube on z-axis clockwise.
*	- Press page-down to rotate the cube on z-axis counter-clockwise.
*	- Press 'r' to reset the cube rotation.
* 
*	- Press left mouse button to scale the cube on x-axis.
*	- Press middle mouse button (wheel) to scale the cube on y-axis.
*	- Press right mouse button to scale the cube on z-axis.
*	- Scroll mouse wheel to change FOV (Field Of View).
* 
*	- Press ESC to exit.
*/

#include <iostream>

#include "ShaderMaker.h"
#include <GL/glew.h>
#include <GL/freeglut.h>

// Include GLM; libreria matematica per le opengl
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "input.h"
#include "text.h"


#define MIN(a,b) (((a)<(b))?(a):(b))
#define MAX(a,b) (((a)>(b))?(a):(b))

typedef glm::vec4 point4;
typedef glm::vec4 colorRGBA;

static unsigned int programId, MatProj, MatModel, MatView;

const colorRGBA BLACK = colorRGBA(1.0, 0.0, 0.0, 1.0);
const colorRGBA RED = colorRGBA(1.0, 0.0, 0.0, 1.0);
const colorRGBA YELLOW = colorRGBA(1.0, 1.0, 0.0, 1.0);
const colorRGBA GREEN = colorRGBA(0.0, 1.0, 0.0, 1.0);
const colorRGBA BLUE = colorRGBA(0.0, 0.0, 1.0, 1.0);
const colorRGBA MAGENTA = colorRGBA(1.0, 0.0, 1.0, 1.0);
const colorRGBA WHITE = colorRGBA(1.0, 1.0, 1.0, 1.0);
const colorRGBA CYAN = colorRGBA(0.0, 1.0, 1.0, 1.0);

const int width = 500;
const int height = 500;
const float zNear = 0.01;
const float zFar = 10000.0;
int fov = 45;

glm::vec3 cameraPos = glm::vec3(0.0, 0.0, 10.0);
glm::vec3 cameraFront = glm::vec3(0.0, 0.0, -1.0);
glm::vec3 cameraUp = glm::vec3(0.0, 1.0, 0.0);

float deltaTime = 0.0; // tempo fra il frame corrente e l'ultimo
float last_frame = 0.0; // tempo dell'ultimo frame

float rotateX = 15, rotateY = -15, rotateZ = 0;  // rotation amounts about axes, controlled by keyboard
float scaleX = 1.0, scaleY = 1.0, scaleZ = 1.0;

//////////////////////////////////////////////////////////////////////////
// Cube data
unsigned int vao;
const int NumVertices = 36;
point4  vPositions[NumVertices];
colorRGBA  vColors[NumVertices];

point4 positions[8] = {
	point4(-0.5, -0.5,  0.5, 1.0),
	point4(-0.5,  0.5,  0.5, 1.0),
	point4(0.5,  0.5,  0.5, 1.0),
	point4(0.5, -0.5,  0.5, 1.0),
	point4(-0.5, -0.5, -0.5, 1.0),
	point4(-0.5,  0.5, -0.5, 1.0),
	point4(0.5,  0.5, -0.5, 1.0),
	point4(0.5, -0.5, -0.5, 1.0)
};

int Index = 0;  // global variable indexing into VBO arrays

void polygon(int a, int b, int c, int d, colorRGBA color)
{
	vPositions[Index] = positions[a]; vColors[Index] = color; Index++;
	vPositions[Index] = positions[b]; vColors[Index] = color; Index++;
	vPositions[Index] = positions[c]; vColors[Index] = color; Index++;
	vPositions[Index] = positions[a]; vColors[Index] = color; Index++;
	vPositions[Index] = positions[c]; vColors[Index] = color; Index++;
	vPositions[Index] = positions[d]; vColors[Index] = color; Index++;
}

void colorcube()
{
	polygon(1, 0, 3, 2, RED);
	polygon(2, 3, 7, 6, BLUE);
	polygon(3, 0, 4, 7, MAGENTA);
	polygon(6, 5, 1, 2, GREEN);
	polygon(4, 5, 6, 7, CYAN);
	polygon(5, 4, 0, 1, YELLOW);
}
///////////////////////////////////////////////////////////////////////////////////

glm::mat4 Projection, Model, View;

glm::vec3 cubePosition = glm::vec3(0.0f, 0.0f, 0.0f);

void initShader(void)
{
		GLenum ErrorCheckValue = glGetError();

		char* vertexShader = (char*)"resources/shaders/vertexShader_C.glsl";
		char* fragmentShader = (char*)"resources/shaders/fragmentShader_C.glsl";

		programId = ShaderMaker::createProgram(vertexShader, fragmentShader);
		glUseProgram(programId);
}

void init(void)
{
	colorcube();

	// Create a vertex array object
	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);

	// 1st attribute VBO : positions 
	unsigned int vpositionID;
	glGenBuffers(1, &vpositionID);
	glBindBuffer(GL_ARRAY_BUFFER, vpositionID);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vPositions), vPositions, GL_STATIC_DRAW);
	glEnableVertexAttribArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, vpositionID);
	glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 0, (void*)0);

	// 2nd attribute VBO : colors 
	unsigned int vcolorID;
	glGenBuffers(1, &vcolorID);
	glBindBuffer(GL_ARRAY_BUFFER, vcolorID);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vColors), vColors, GL_STATIC_DRAW);
	glEnableVertexAttribArray(1);
	glBindBuffer(GL_ARRAY_BUFFER, vcolorID);
	glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, 0, (void*)0);
	
	// Definisco il colore che verrà assegnato allo schermo
	glClearColor(0.0, 0.0, 0.0, 0.0);

	// Ottieni l'identificativo della variabile uniform mat4 Projection (in vertex shader).
    // Questo identificativo sarà poi utilizzato per il trasferimento della matrice Projection al Vertex Shader
	MatProj = glGetUniformLocation(programId, "Projection");
	// Ottieni l'identificativo della variabile uniform mat4 Model (in vertex shader)
	// Questo identificativo sarà poi utilizzato per il trasferimento della matrice Model al Vertex Shader
	MatModel = glGetUniformLocation(programId, "Model");
	// Ottieni l'identificativo della variabile uniform mat4 View (in vertex shader)
	// Questo identificativo sarà poi utilizzato per il trasferimento della matrice View al Vertex Shader
	MatView = glGetUniformLocation(programId, "View");

}

void mousewheel(int button, int dir, int x, int y)
{
	if (dir > 0)
		fov += 1;
	else
		fov -= 1;
	glutPostRedisplay();
}
void resize(int w, int h)
{
	glViewport(0, 0, w, h);
}

void doInput()
{
	float alfa = 0.025 * deltaTime;

	// Quit
	if (input.keys[033])
	{
		exit(EXIT_SUCCESS);
	}

	// Move camera upwards
	if (input.keys[' ']) 
	{
		cameraPos -= alfa * cameraUp;
	}
	// Move camera downwards
	if (input.specialKeys[0160])
	{
		cameraPos += alfa * cameraUp;
	}
	// Move camera to the left
	if (input.keys['a']) 
	{
		glm::vec3 direction = normalize(cross(cameraFront, cameraUp));
		cameraPos -= alfa * direction;
	}
	// Move camera to the right
	if (input.keys['d'])
	{
		glm::vec3 direction = normalize(cross(cameraFront, cameraUp));
		cameraPos += alfa * direction;
	}
	// Move camera close (same direction)
	if (input.keys['w'])
	{
		cameraPos += alfa * cameraFront;
	}
	// move camera away (same direction)
	if (input.keys['s'])
	{ 
		cameraPos -= alfa * cameraFront;
	}

	// Reset camera position
	if (input.keys['t'])
	{
		cameraPos = glm::vec3(0.0, 0.0, 10.0);
		cameraFront = glm::vec3(0.0, 0.0, -1.0);
		cameraUp = glm::vec3(0.0, 1.0, 0.0);
	}

	// Rotate along X axis (clockwise)
	if (input.specialKeys[GLUT_KEY_UP])
	{
		rotateX -= 15;
		if (rotateX < 0)
			rotateX += 360;
	}
	// Rotate along X axis (counter-clockwise)
	if (input.specialKeys[GLUT_KEY_DOWN])
	{
		rotateX += 15;
		if (rotateX >= 360)
			rotateX -= 360;
	}
	// Rotate along Y axis (clockwise)
	if (input.specialKeys[GLUT_KEY_LEFT])
	{
		rotateY -= 15;
		if (rotateY < 0)
			rotateY += 360;
	}
	// Rotate along Y axis (counter-clockwise)
	if (input.specialKeys[GLUT_KEY_RIGHT])
	{
		rotateY += 15;
		if (rotateY >= 360)
			rotateY -= 360;
	}
	// Rotate along Z axis (clockwise)
	if (input.specialKeys[GLUT_KEY_PAGE_UP])
	{
		rotateZ -= 15;
		if (rotateZ < 0)
			rotateZ += 360;
	}
	// Rotate along Z axis (counter-clockwise)
	if (input.specialKeys[GLUT_KEY_PAGE_DOWN])
	{
		rotateZ += 15;
		if (rotateZ >= 360)
			rotateZ -= 360;
	}

	// Reset rotation
	if (input.keys['r'])
	{
		rotateX = 15;
		rotateY = -15;
		rotateZ = 0;
	}

	// Scale along X axis
	if (input.mouse.keys[GLUT_LEFT_BUTTON])
	{
		scaleX = MIN(10.0, scaleX * 1.1);
	}
	else scaleX = MAX(1.0, scaleX * 0.9);
	
	// Scale along Y axis
	if (input.mouse.keys[GLUT_MIDDLE_BUTTON])
	{
		scaleY = MIN(10.0, scaleY * 1.1);
	}
	else scaleY = MAX(1.0, scaleY * 0.9);
	
	// Scale along Z axis
	if (input.mouse.keys[GLUT_RIGHT_BUTTON])
	{
		scaleZ = MIN(10.0, scaleZ * 1.1);
	}
	else scaleZ = MAX(1.0, scaleZ * 0.9);
	
}

void drawText()
{
	glDisable(GL_DEPTH_TEST);
	glEnable(GL_BLEND);

	char out[64];
	sprintf_s(out, 64, "Rotation: [%3.0f, %3.0f, %3.0f]", rotateX, rotateY, rotateZ);
	RenderString(-0.99, -0.99, 1.0, GLUT_BITMAP_HELVETICA_18, out, 1.0f, 0.0f, 0.0f);
	printf("%s", out);
	sprintf_s(out, 64, "Size: [%2.3f, %2.3f, %2.3f]", scaleX, scaleY, scaleZ);
	RenderString(0.0, 0.0, 1.0, GLUT_BITMAP_HELVETICA_18, out, 1.0f, 1.0f, 1.0f);
	printf(" - %s\n", out);

	glEnable(GL_DEPTH_TEST);
	glDisable(GL_BLEND);
}

void drawScene()
{
	float timevalue = glutGet(GLUT_ELAPSED_TIME) * 0.0001;

	// Passo al Vertex Shader il puntatore alla matrice Projection, che sarà associata alla variabile Uniform mat4 Projection
	// all'interno del Vertex shader. Uso l'identificatio MatProj
	Projection = glm::perspective(glm::radians((float)fov), (float)(width) / float(height), zNear, zFar);
	glUniformMatrix4fv(MatProj, 1, GL_FALSE, value_ptr(Projection));
	// Costruisco la matrice di Vista che applicata ai vertici in coordinate mondo WCS
	// li trasforma nel sistema di riferimento della camera VCS.
	// usage: lookAt(eye,at,up);
	View = lookAt(cameraPos, cameraPos + cameraFront, cameraUp);
	glUniformMatrix4fv(MatView, 1, GL_FALSE, value_ptr(View));

	float currentFrame = glutGet(GLUT_ELAPSED_TIME);
	deltaTime = currentFrame - last_frame;
	last_frame = currentFrame;

	glClearColor(0.0, 0.0, 0.0, 0.0);

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);


	Model = glm::mat4(1.0);
	Model = translate(Model, cubePosition);
	Model = glm::rotate(Model, glm::radians(rotateX), glm::vec3(1.0f, 0.0f, 0.0f));
	Model = glm::rotate(Model, glm::radians(rotateY), glm::vec3(0.0f, 1.0f, 0.0f));
	Model = glm::rotate(Model, glm::radians(rotateZ), glm::vec3(0.0f, 0.0f, 1.0f));
	Model = glm::scale(Model, glm::vec3(scaleX, scaleY, scaleZ));

	// Passo al Vertex Shader il puntatore alla matrice Model, che sarà associata alla variabile Uniform mat4 Projection
	// all'interno del Vertex shader. Uso l'identificatio MatModel
	glUniformMatrix4fv(MatModel, 1, GL_FALSE, value_ptr(Model));
	
	glBindVertexArray(vao);
	// glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	glDrawArrays(GL_TRIANGLES, 0, NumVertices);
	
	// drawText();

	glutSwapBuffers();
}

void update(int a)
{
	doInput();
	glutPostRedisplay();
	glutTimerFunc(20, update, 0);
}

int main(int argc, char* argv[])
{
	glutInit(&argc, argv);

	glutInitContextVersion(4, 0);
	glutInitContextProfile(GLUT_CORE_PROFILE);

	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA);

	glutInitWindowSize(width, height);
	glutInitWindowPosition(0, 0);
	glutCreateWindow("LAB 00 Es");

	glewExperimental = GL_TRUE;
	glewInit();
	
	// Input
	glutMouseWheelFunc(mousewheel);
	initInput(1);

	// Update
	glutTimerFunc(20, update, 0);

	// Draw
	glutDisplayFunc(drawScene);
	glutReshapeFunc(resize);

	initShader();
	init();

	glEnable(GL_DEPTH_TEST);
	glEnable(GL_BLEND);
	glEnable(GL_ALPHA_TEST);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	
	glutMainLoop();
}