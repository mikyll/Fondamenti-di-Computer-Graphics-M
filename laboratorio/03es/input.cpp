#include "input.h"

static bool moving_trackball = 0;
static int last_mouse_pos_Y;
static int last_mouse_pos_X;


static void mouseInput(int button, int state, int x, int y);
static void specialKeyDown(int key, int x, int y);
static void mouseActiveMotion(int x, int y);
static void keyboardDown(unsigned char key, int x, int y);
static glm::vec3 getTrackBallPoint(float x, float y);


void initInput()
{
	glutKeyboardFunc(keyboardDown);
	glutSpecialFunc(specialKeyDown);
	glutMouseFunc(mouseInput);
	glutMotionFunc(mouseActiveMotion);
}

static void keyboardDown(unsigned char key, int x, int y)
{
	switch (key) {
	case 27:
		glutLeaveMainLoop();
		break;
		// Transform operation selection
	case 'g':
		app.operationMode = MODE_TRASLATING;
		break;
	case 'r':
		app.operationMode = MODE_ROTATING;
		break;
	case 's':
		app.operationMode = MODE_SCALING;
		break;
		// Transform axis selection
	case 'x':
		app.workingAxis = AXIS_X;
		break;
	case 'y':
		app.workingAxis = AXIS_Y;
		break;
	case 'z':
		app.workingAxis = AXIS_Z;
		break;
	case 'c':
		initCamera();
		break;
	default:
		break;
	}
	glutPostRedisplay();
}

static void specialKeyDown(int key, int x, int y)
{
	switch (key)
	{
	case GLUT_KEY_LEFT:
		selectedObj = selectedObj > 0 ? selectedObj - 1 : objects.size() - 1;
		break;
	case GLUT_KEY_RIGHT:
		selectedObj = (selectedObj + 1) < objects.size() ? selectedObj + 1 : 0;
		break;
	// Reset Scene
	case GLUT_KEY_F1:
		initLight();
		initShaders(light);
		initObjects();
		break;
	case GLUT_KEY_F2:
		updateLight(glm::vec3(5., 5., 5.), glm::vec3(1.0f, 1.0f, 1.0f), 1.0f);
		break;
	case GLUT_KEY_F3:
		updateLight(glm::vec3(-5., 5., 5.), glm::vec3(1.0f, 1.0f, 1.0f), 1.0f);
		break;
	case GLUT_KEY_F4:
		updateLight(glm::vec3(-5., 5., -5.), glm::vec3(1.0f, 1.0f, 1.0f), 1.0f);
		break;
	case GLUT_KEY_F5:
		updateLight(glm::vec3(80., 100., 50.), glm::vec3(1., .89, .52), 1.0f);
		break;
	default:
		break;
	}
	glutPostRedisplay();
}

static void mouseInput(int button, int state, int x, int y)
{
	glutPostRedisplay();
	int modifiers = glutGetModifiers();
	if (modifiers == GLUT_ACTIVE_SHIFT)
	{
		switch (button)
		{
		case SHIFT_WHEEL_UP:
			moveCameraUp();
			break;
		case SHIFT_WHEEL_DOWN:
			moveCameraDown();
			break;
		}
		return;
	}
	else if (modifiers == GLUT_ACTIVE_CTRL)
	{
		switch (button)
		{
		case CTRL_WHEEL_UP:
			moveCameraRight();
			break;
		case CTRL_WHEEL_DOWN:
			moveCameraLeft();
			break;
		}
		return;
	}

	glm::vec4 axis;
	float amount = 0.1f;
	// Imposto il valore della trasformazione
	switch (button)
	{
		// scroll wheel up
	case 3:
		amount *= 1;
		break;

		// scroll wheel down
	case 4:
		amount *= -1;
		break;

	case GLUT_LEFT_BUTTON:
		// If the user is pressing the left mouse button, we are moving the trackball
		moving_trackball = state == GLUT_DOWN;

		app.operationMode = MODE_NAVIGATION;
		last_mouse_pos_X = x;
		last_mouse_pos_Y = y;
		break;

	default:
		break;
	}
	// Selezione dell'asse per le trasformazioni
	switch (app.workingAxis)
	{
	case AXIS_X:	axis = glm::vec4(1.0, 0.0, 0.0, 0.0);
		break;

	case AXIS_Y: axis = glm::vec4(0.0, 1.0, 0.0, 0.0);
		break;

	case AXIS_Z: axis = glm::vec4(0.0, 0.0, 1.0, 0.0);
		break;

	default:
		break;
	}

	switch (app.operationMode)
	{
	case MODE_NAVIGATION:
		// Wheel reports as button 3(scroll up) and button 4(scroll down)
		if (button == 3) {
			moveCameraBack();
		}
		else if (button == 4) {
			moveCameraForeward();
		}
		break;

	case MODE_TRASLATING:
		modifyModelMatrix(axis * amount, axis, 0.0f, 1.0f);
		break;

	case MODE_ROTATING:
		modifyModelMatrix(glm::vec3(0), axis, amount * 20.0f, 1.0f);
		break;

	case MODE_SCALING:
		modifyModelMatrix(glm::vec3(0), axis, 0.0f, 1.0f + amount);
		break;

	default:
		break;
	}
	if (app.operationMode == MODE_TRASLATING ||
		app.operationMode == MODE_ROTATING ||
		app.operationMode == MODE_SCALING)
	{
		if (objects.at(selectedObj).name == "light")
		{
			light.position = getPosition(objects.at(selectedObj).M);
			light.power = getScalingFactor(objects.at(selectedObj).M) * LIGHT_SCALE_FACTOR;
		}
	}
}

static void mouseActiveMotion(int x, int y)
{
	// Spostamento su trackball del vettore posizione Camera 
	if (!moving_trackball) {
		return;
	}
	glm::vec3 destination = getTrackBallPoint(x, y);
	glm::vec3 origin = getTrackBallPoint(last_mouse_pos_X, last_mouse_pos_Y);
	float dx, dy, dz;
	dx = destination.x - origin.x;
	dy = destination.y - origin.y;
	dz = destination.z - origin.z;
	if (dx || dy || dz) {
		// rotation angle = acos( (v dot w) / (len(v) * len(w)) ) o approssimato da ||dest-orig||;
		float pi = glm::pi<float>();
		float angle = sqrt(dx * dx + dy * dy + dz * dz) * (180.0 / pi);
		// rotation axis = (dest vec orig) / (len(dest vec orig))
		glm::vec3 rotation_vec = glm::cross(origin, destination);
		// calcolo del vettore direzione w = C - A
		glm::vec4 direction = camera.viewSetup.position - camera.viewSetup.target;
		// rotazione del vettore direzione w 
		// determinazione della nuova posizione della camera 
		camera.viewSetup.position = camera.viewSetup.target + glm::rotate(glm::mat4(1.0f), glm::radians(-angle), rotation_vec) * direction;
	}
	last_mouse_pos_X = x; last_mouse_pos_Y = y;
	glutPostRedisplay();
}

/*
* Trackball: Converts a 2D-point on screen in a 3D-point on the trackball
*/
static glm::vec3 getTrackBallPoint(float x, float y)
{
	float zTemp;
	glm::vec3 point;
	//map to [-1;1]
	point.x = (2.0f * x - windowWidth) / windowWidth;
	point.y = (windowHeight - 2.0f * y) / windowHeight;

	zTemp = 1.0f - pow(point.x, 2.0) - pow(point.y, 2.0);
	point.z = (zTemp > 0.0f) ? sqrt(zTemp) : 0.0;
	return point;
}