/*
* Lab 03 - Scene Interactive Navigation with 3D Mesh Models
*
* Solution by Michele Righi (0001025005)
* GitHub: https://github.com/mikyll/Fondamenti-di-Computer-Graphics-M/tree/main/laboratorio/03es
* 
* 
* Description:
* This program implements a scene filled with 3D object, that
* the user can interact with, by using mouse and keyboard controls.
* Objects are characterized by:
*	- a polygonal mesh, that gives them a geometric structure;
*	- a material, that indicates how their surface "behave" when hit by light;
*	- a shader, that do some processing in order to implement the lighting and
*		motion effects (on GPU);
* 
*/


/******************************************************************************************
LAB 03
Gestione interattiva di una scena 3D mediante controllo da mouse e da tastiera.
I modelli geometrici in scena sono mesh poligonali in formato *.obj

TRL+WHEEL = pan orizzontale della telecamera
SHIFT+WHEEL = pan verticale della telecamera
WHEEL = se navigazione  --> ZOOM IN/OUT
		se modify       --> agisce sulla trasformazione dell'oggetto
g r s	per le modalità di lavoro: traslate/rotate/scale
x y z	per l'asse di lavoro
wcs/ocs selezionabili dal menu pop-up

OpenGL Mathematics (GLM) is a header only C++ mathematics library for graphics software
based on the OpenGL Shading Language (GLSL) specifications.
*******************************************************************************************/

#include "commons.h"

// todo
extern int windowWidth, windowHeight;
Application app = {};
extern Camera camera;

extern std::map<MaterialType, Material> materials;
extern std::map<ShadingType, Shader> shaders;
std::vector<Object> objects;
int selectedObj = 0;

static Object axisObject, gridObject;
PointLight light;

extern void initMaterials();
extern void initShaders(PointLight light);
extern void initCamera();
extern std::string getCoordinateSystemName(CoordinateSystem coordinateSystem);
extern std::string getOperationModeName(OperationMode operationMode);
extern std::string getWorkingAxisName(WorkingAxis workingAxis);


void init();
void initApplication();
void initObjects();

void modifyModelMatrix(glm::vec3 translation_vector, glm::vec3 rotation_vector, GLfloat angle, GLfloat scale_factor);
void resize(int w, int h);
void refresh_monitor(int millis);
void drawObject(Object object);
void printSceneInfo();


void init() {
	// Default render settings
	glEnable(GL_DEPTH_TEST);	// Hidden surface removal
	glCullFace(GL_BACK);		// remove faces facing the background
	glEnable(GL_LINE_SMOOTH);

	// Light initialization
	light.position = { 5.0,5.0,-5.0 };
	light.color = { 1.0,1.0,1.0 };
	light.power = 1.f;

	initApplication();
	initCamera();

	initMaterials();
	initShaders(light);
	initObjects();
}

void initApplication()
{
	app.coordinateSystem = WCS;
	app.operationMode = MODE_NAVIGATION;
	app.workingAxis = AXIS_X;
	app.showTransform = false;
	app.showTransform = true;
}

void initObjects()
{
	Object obj;
	Mesh mesh;

	// Axis for reference
	mesh = loadMesh("axis.obj", VERTEX_NORMALS);
	obj = createObject("axis_", mesh, materials.at(COPPER), shaders.at(BLINN), glm::vec3(), glm::vec3(), glm::vec3(2., 2., 2.));
	axisObject = obj;

	// White Grid Plane for reference
	mesh = loadMesh("reference_grid.obj", VERTEX_NORMALS);
	obj = createObject("grid_", mesh, materials.at(NONE), shaders.at(GOURAUD), glm::vec3(), glm::vec3(), glm::vec3(1., 1., 1.));
	gridObject = obj;

	// Point Light
	mesh = loadMesh("sphere_n_t_smooth.obj", FACE_NORMALS);
	obj = createObject("light", mesh, materials.at(NONE), shaders.at(PASS_THROUGH), light.position, glm::vec3(), glm::vec3(0.2, 0.2, 0.2));
	objects.push_back(obj);

	// FLAT Sphere (face normals)
	mesh = loadMesh("sphere_n_t_flat.obj", FACE_NORMALS);
	obj = createObject("Sphere FLAT", mesh, materials.at(EMERALD), shaders.at(PHONG), glm::vec3(3., 1., -6.), glm::vec3(), glm::vec3(1., 1., 1.));
	objects.push_back(obj);

	// SMOOTH Sphere (vertex normals)
	mesh = loadMesh("sphere_n_t_smooth.obj", FACE_NORMALS);
	obj = createObject("Sphere SMOOTH", mesh, materials.at(SILVER), shaders.at(BLINN), glm::vec3(6., 1., -3.), glm::vec3(), glm::vec3(1., 1., 1.));
	objects.push_back(obj);

	// Waving plane
	mesh = loadMesh("GridPlane.obj", FACE_NORMALS);
	obj = createObject("Waves", mesh, materials.at(TURQUOISE), shaders.at(WAVE_LIGHT), glm::vec3(0., -2., 0.), glm::vec3(), glm::vec3(8., 8., 8.));
	objects.push_back(obj);

	// Bunny model
	mesh = loadMesh("bunny.obj", VERTEX_NORMALS);
	obj = createObject("Bunny", mesh, materials.at(RED_PLASTIC), shaders.at(TOON), glm::vec3(0., 0., -2.), glm::vec3(), glm::vec3(2., 2., 2.));
	objects.push_back(obj);

	// Airplane model
	mesh = loadMesh("airplane.obj", VERTEX_NORMALS);
	obj = createObject("Airplane", mesh, materials.at(RED_PLASTIC), shaders.at(PHONG), glm::vec3(-10., 5., 0.), glm::vec3(0.0, 0.0, -40.0), glm::vec3(5., 5., 5.));
	objects.push_back(obj);

	// Horse model
	mesh = loadMesh("horse.obj", VERTEX_NORMALS);
	obj = createObject("Horse FLAT", mesh, materials.at(GOLD), shaders.at(PHONG), glm::vec3(-3., 2., 5.), glm::vec3(0.0, 225.0f, 0.0), glm::vec3(0.5, 0.5, 0.5));
	objects.push_back(obj);

	// Horse model
	mesh = loadMesh("horse.obj", FACE_NORMALS);
	obj = createObject("Horse SMOOTH", mesh, materials.at(SLATE), shaders.at(PHONG), glm::vec3(-5., 2., 7.), glm::vec3(0.0, 225.0f, 0.0), glm::vec3(0.5, 0.5, 0.5));
	objects.push_back(obj);

	// Test
	//initBrokenMesh("horse.obj", "Horse", FACE_NORMALS, glm::vec3(-5., 2., -5), glm::vec3(0.0, 225.0f, 0.0), glm::vec3(0.5, 0.5, 0.5), materials.at(GOLD), shaders.at(PHONG));
}

/*
* Crea ed applica la matrice di trasformazione alla matrice dell'oggetto discriminando
* tra WCS e OCS. La funzione è gia invocata con un input corretto, è sufficiente
* concludere la sua implementazione.
*/
void modifyModelMatrix(glm::vec3 translation_vector, glm::vec3 rotation_vector, GLfloat angle, GLfloat scale_factor)
{
	// TO-DO: OK
	glm::mat4 newModelMatrix = objects.at(selectedObj).M;
	// Translation
	newModelMatrix = glm::translate(newModelMatrix, translation_vector);
	// Rotation
	if (app.coordinateSystem == OCS)
	{
		// Rotation with pivot on the object's center
		newModelMatrix = glm::rotate(newModelMatrix, glm::radians(angle), rotation_vector);
	}
	else if (app.coordinateSystem == WCS)
	{
		// Rotation with pivot on the scene center
		glm::vec3 objectPosition = newModelMatrix[3];
		glm::vec3 origin = glm::vec3(0, 0, 0); // [3] gets the 4th column of the matrix, which is a vector4 and contains the position (x, y, z, 0/1)

		newModelMatrix = glm::translate(newModelMatrix, origin - objectPosition);
		newModelMatrix = glm::rotate(newModelMatrix, glm::radians(angle), rotation_vector);
		newModelMatrix = glm::translate(newModelMatrix, objectPosition - origin);
	}
	// Scaling
	newModelMatrix = glm::scale(newModelMatrix, glm::vec3(scale_factor, scale_factor, scale_factor));
	objects.at(selectedObj).M = newModelMatrix;
}


/*
* Updates the camera view port, when the windows is resized.
*/
void resize(int w, int h)
{
	if (h == 0)	// Window is minimized
		return;
	int width = h * ASPECT_RATIO; // width is adjusted for aspect ratio
	int left = (w - width) / 2;
	// Set Viewport to window dimensions
	glViewport(left, 0, width, h);
	windowWidth = w;
	windowHeight = h;

	// Fixed Pipeline matrices for retro compatibility
	glUseProgram(0); // Embedded openGL shader
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(camera.perspectiveSetup.fovY, camera.perspectiveSetup.aspect, camera.perspectiveSetup.near_plane, camera.perspectiveSetup.far_plane);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	gluLookAt(camera.viewSetup.position.x, camera.viewSetup.position.y, camera.viewSetup.position.z,
		camera.viewSetup.target.x, camera.viewSetup.target.y, camera.viewSetup.target.z,
		camera.viewSetup.upVector.x, camera.viewSetup.upVector.y, camera.viewSetup.upVector.z);

	// Programmable Pipeline matrices for object rendering
	glm::mat4 P = glm::perspective(camera.perspectiveSetup.fovY, camera.perspectiveSetup.aspect, camera.perspectiveSetup.near_plane, camera.perspectiveSetup.far_plane);
	glm::mat4 V = glm::lookAt(glm::vec3(camera.viewSetup.position), glm::vec3(camera.viewSetup.target), glm::vec3(camera.viewSetup.upVector));

	for (int i = 0; i < shaders.size(); i++) {
		glUseProgram(shaders.at(i).id);
		glUniformMatrix4fv(shaders.at(i).baseUniform.P_Matrix_pointer, 1, GL_FALSE, value_ptr(P));
		glUniformMatrix4fv(shaders.at(i).baseUniform.V_Matrix_pointer, 1, GL_FALSE, value_ptr(V));
	}
}

/*
* Triggers the display redrawing each 'millis' milliseconds.
*/
void refresh_monitor(int millis)
{
	glutPostRedisplay();
	glutTimerFunc(millis, refresh_monitor, millis);
}

void drawObject(Object object)
{
	// Select Shader program
	glUseProgram(object.shader.id);

	updateUniforms(object, light);

	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);
	glBindVertexArray(object.mesh.vertexArrayObjID);
	glDrawArrays(GL_TRIANGLES, 0, object.mesh.vertices.size());

	glDisableVertexAttribArray(0);
	glDisableVertexAttribArray(1);
}

void drawScene() {

	glClearColor(0.4, 0.4, 0.4, 1);
	glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);

	drawObject(axisObject);
	drawObject(gridObject);

	for (int i = 0; i < objects.size(); i++)
	{
		drawObject(objects.at(i));
	}

	// OLD fixed pipeline for simple graphics and symbols
	glUseProgram(0);
	printSceneInfo();

	glutSwapBuffers();
}

/*
* 2D fixed pipeline Font rendering on screen
*/
void printSceneInfo()
{
	std::string refSystem = "Ref. System: " + getCoordinateSystemName(app.coordinateSystem);
	std::string mode = "Operation: " + getOperationModeName(app.operationMode);
	std::string axis = "Axis: " + getWorkingAxisName(app.workingAxis);
	std::string object = "Object: " + objects[selectedObj].name;
	std::string position = " Pos.: " + objects[selectedObj].name;
	std::string rotation = " Rot.: " + objects[selectedObj].name;
	std::string scale = " Scale: " + objects[selectedObj].name;
	std::string material = " Material: " + objects.at(selectedObj).material.name;
	std::string shader = " Shading: " + objects.at(selectedObj).shader.name;

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();

	// Handle output info layout 
	gluOrtho2D(0, windowHeight * ASPECT_RATIO, 0, windowHeight);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	std::vector<std::string> lines;
	lines.push_back(shader);
	lines.push_back(material);
	lines.push_back(object);
	lines.push_back(axis);
	lines.push_back(mode);
	lines.push_back(refSystem);
	glDisable(GL_DEPTH_TEST);
	HUD_Logger::get()->printInfo(lines);
	glEnable(GL_DEPTH_TEST);

	resize(windowWidth, windowHeight);
}

int main(int argc, char** argv) {

	GLboolean GlewInitResult;
	glutInit(&argc, argv);
	glutSetOption(GLUT_MULTISAMPLE, 4);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA | GLUT_DEPTH | GLUT_MULTISAMPLE);
	glutInitWindowSize(windowWidth, windowHeight);
	glutInitWindowPosition(100, 100);
	glutCreateWindow("Model Viewer ");

	glutDisplayFunc(drawScene);
	glutReshapeFunc(resize);
	initInput();

	glewExperimental = GL_TRUE;
	GlewInitResult = glewInit();
	fprintf(
		stdout,
		"INFO: OpenGL Version: %s\n",
		glGetString(GL_VERSION)
	);

	init();

	buildOpenGLMenu();
	refresh_monitor(16);
	glutMainLoop();
}
