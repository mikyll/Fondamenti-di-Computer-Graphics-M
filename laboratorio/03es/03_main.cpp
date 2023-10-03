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
*	- a polygonal mesh (loaded from a .obj file), that gives them a geometric structure;
*	- a material, that indicates how their surface "behave" when hit by light;
*	- a shader (vertex + fragment), that does some processing in order to implement the lighting and
*		motion effects (on GPU);
* 
* 
* Materials:
* - None;
* - Red Plastic;
* - Emerald;
* - Brass;
* - Slate;
* - Black Plastic (New);
* - Gold (New);
* - Silver (New);
* - Bronze (New);
* - Copper (New);
* - Turquoise (New);
* 
* Shaders:
* - Pass Through;
* - Gouraud;
* - Phong;
* - Blinn;
* - Wave (New);
* - Wave Color (New);
* - Wave Light (New);
* - Toon (New);
* - Toon V2 (New);
* 
* 
* Controls:
* - Scene setup:
*	- 'F1' reset the scene to defaults;
*	- 'F2', 'F3', 'F4', 'F5' moves the light in different points;
* 
* - Navigation:
*	- Mouse drag & drop: trackball that moves the camera;
*	- 'CTRL' + Mouse wheel: horizontal camera pan;
*	- 'SHIFT' + Mouse wheel: vertical camera pan;
* 
* - Transform Operations:
*	When in modify mode, the user can scroll the Mouse wheel
*	to perform transform operations.
*	To select the transformation:
*		- 'g' set the operation mode to "Translation";
*		- 'r' set the operation mode to "Rotation";
*		- 's' set the operation mode to "Scaling";
*	To select the transform axis:
*		- 'x' set the X-axis;
*		- 'y' set the Y-axis;
*		- 'z' set the Z-axis;
* 
* - Select scene objects:
*	- Left/Right arrow to scroll through scene objects:
* 
* - Scene Menu:
*	- Left Mouse click opens a context menu, that allows the user to
*		select the Reference Coordinate System (WCS/OCS) and
*/

#include "commons.h"

extern int windowWidth, windowHeight;
Application app;
extern Camera camera;

extern std::map<MaterialType, Material> materials;
extern std::map<ShadingType, Shader> shaders;
std::vector<Object> objects;
int selectedObj = 0;

static Object axisObject, gridObject;
extern PointLight light;

extern void initMaterials();
extern void initShaders(PointLight light);
extern void initLight();
extern void initLightObject();
extern void initCamera();
extern glm::vec3 getPosition(glm::mat4 modelMatrix);
extern glm::vec3 getRotation(glm::mat4 modelMatrix, bool degrees);
extern float getScalingFactor(glm::mat4 modelMatrix);
extern std::string getCoordinateSystemName(ReferenceSystem coordinateSystem);
extern std::string getOperationModeName(OperationMode operationMode);
extern std::string getWorkingAxisName(WorkingAxis workingAxis);

void printControls();
void init();
void initApplication();
void initGridAxis();
void initObjects();

void modifyModelMatrix(glm::vec3 translation_vector, glm::vec3 rotation_vector, GLfloat angle, GLfloat scale_factor);
void resize(int w, int h);
void refresh_monitor(int millis);
void drawObject(Object object);
void printSceneInfo();

void printControls()
{
	std::cout << "Controls:" << std::endl;
	std::cout << "- Scene setup:" << std::endl;
	std::cout << "  - 'F1' reset the scene to defaults;" << std::endl;
	std::cout << "  - 'F2', 'F3', 'F4', 'F5' moves the light in different points;" << std::endl << std::endl;
	std::cout << "- Navigation:" << std::endl;
	std::cout << "  - Mouse drag & drop: trackball that moves the camera;" << std::endl;
	std::cout << "  - 'CTRL' + Mouse wheel: horizontal camera pan;" << std::endl;
	std::cout << "  - 'SHIFT' + Mouse wheel: vertical camera pan;" << std::endl;
	std::cout << "- Transform Operations:" << std::endl;
	std::cout << "  When in modify mode, the user can scroll the " << std::endl;
	std::cout << "  Mouse wheel to perform transform operations." << std::endl;
	std::cout << "  To select the transformation:" << std::endl;
	std::cout << "  - 'g' set the operation mode to \"Translation\";" << std::endl;
	std::cout << "  - 'r' set the operation mode to \"Rotation\";" << std::endl;
	std::cout << "  - 's' set the operation mode to \"Scaling\";" << std::endl;
	std::cout << "  To select the transform axis:" << std::endl;
	std::cout << "  - 'x' set the X-axis;" << std::endl;
	std::cout << "  - 'y' set the Y-axis;" << std::endl;
	std::cout << "  - 'z' set the Z-axis;" << std::endl << std::endl;
	std::cout << "- Select scene objects:" << std::endl;
	std::cout << "  - Left/Right arrow to scroll through scene objects." << std::endl;
	std::cout << "- Scene Menu:" << std::endl;
	std::cout << "  - Left Mouse click opens a context menu, that allows the user" << std::endl;
	std::cout << "    to custom the settings and select shaders / materials." << std::endl << std::endl;
}

void init()
{
	// Default render settings
	glEnable(GL_DEPTH_TEST);	// Hidden surface removal
	glCullFace(GL_BACK);		// remove faces facing the background
	glEnable(GL_LINE_SMOOTH);

	initApplication();
	initCamera();
	initLight();

	initMaterials();
	initShaders(light);
	initObjects();
}

void initApplication()
{
	app = {};
	app.coordinateSystem = WCS;
	app.operationMode = MODE_NAVIGATION;
	app.workingAxis = AXIS_X;
	app.showTransformInfo = true;
	app.showGridAxis = true;
}

void initGridAxis()
{
	Object obj;
	Mesh mesh;

	// Axis for reference
	if (loadMesh("axis.obj", VERTEX_NORMALS, &mesh) == 0)
	{
		obj = createObject("axis_", mesh, materials.at(COPPER), shaders.at(BLINN), glm::vec3(), glm::vec3(), glm::vec3(2., 2., 2.));
		axisObject = obj;
	}
	else exit(EXIT_FAILURE);

	// White Grid Plane for reference
	if (loadMesh("reference_grid.obj", VERTEX_NORMALS, &mesh) == 0)
	{
		obj = createObject("grid_", mesh, materials.at(NONE), shaders.at(GOURAUD), glm::vec3(), glm::vec3(), glm::vec3(1., 1., 1.));
		gridObject = obj;
	}
	else exit(EXIT_FAILURE);
}

void initObjects()
{
	Object obj;
	Mesh mesh;

	objects.clear();

	initLightObject();
	initGridAxis();

	// FLAT Sphere (face normals)
	if (loadMesh("sphere_n_t_flat.obj", FACE_NORMALS, &mesh) == 0)
	{
		obj = createObject("Sphere FLAT", mesh, materials.at(EMERALD), shaders.at(PHONG), glm::vec3(3., 1., -6.), glm::vec3(), glm::vec3(1., 1., 1.));
		objects.push_back(obj);
	}

	// SMOOTH Sphere (vertex normals)
	if (loadMesh("sphere_n_t_smooth.obj", FACE_NORMALS, &mesh) == 0)
	{
		obj = createObject("Sphere SMOOTH", mesh, materials.at(SILVER), shaders.at(BLINN), glm::vec3(6., 1., -3.), glm::vec3(), glm::vec3(1., 1., 1.));
		objects.push_back(obj);
	}

	// Waving plane
	if (loadMesh("GridPlane.obj", FACE_NORMALS, &mesh) == 0)
	{
		obj = createObject("Waves", mesh, materials.at(TURQUOISE), shaders.at(WAVE_LIGHT), glm::vec3(0., -2., 0.), glm::vec3(), glm::vec3(8., 8., 8.));
		objects.push_back(obj);
	}

	// Bunny model
	if (loadMesh("bunny.obj", VERTEX_NORMALS, &mesh) == 0)
	{
		obj = createObject("Bunny", mesh, materials.at(RED_PLASTIC), shaders.at(TOON), glm::vec3(0., 0., -2.), glm::vec3(), glm::vec3(2., 2., 2.));
		objects.push_back(obj);
	}

	// Airplane model
	if (loadMesh("airplane.obj", VERTEX_NORMALS, &mesh) == 0)
	{
		obj = createObject("Airplane", mesh, materials.at(RED_PLASTIC), shaders.at(PHONG), glm::vec3(-10., 5., 0.), glm::vec3(0.0, 0.0, -40.0), glm::vec3(5., 5., 5.));
		objects.push_back(obj);
	}

	// Horse model 1
	if (loadMesh("horse.obj", VERTEX_NORMALS, &mesh) == 0)
	{
		obj = createObject("Horse FLAT", mesh, materials.at(GOLD), shaders.at(PHONG), glm::vec3(-3., 3, 5.), glm::vec3(0.0, 225.0f, 0.0), glm::vec3(0.5, 0.5, 0.5));
		objects.push_back(obj);
	}

	// Horse model 2
	if (loadMesh("horse.obj", FACE_NORMALS, &mesh) == 0)
	{
		obj = createObject("Horse SMOOTH", mesh, materials.at(SLATE), shaders.at(PHONG), glm::vec3(-5., 3, 7.), glm::vec3(0.0, 225.0f, 0.0), glm::vec3(0.5, 0.5, 0.5));
		objects.push_back(obj);
	}

	// Teapot model
	if (loadMesh("teapot.obj", VERTEX_NORMALS, &mesh) == 0)
	{
		obj = createObject("Teapot", mesh, materials.at(RED_PLASTIC), shaders.at(TOON_V2), glm::vec3(5., 0., 7.), glm::vec3(0.0, 45.0f, 0.0), glm::vec3(0.5, 0.5, 0.5));
		objects.push_back(obj);
	}

	// Broken loading of a mesh
	/*if (loadBrokenMesh("horse.obj", FACE_NORMALS, &mesh) == 0)
	{
		obj = createObject("Broken Horse", mesh, materials.at(GOLD), shaders.at(PHONG), glm::vec3(3., 0., 5.), glm::vec3(0.0, 225.0f, 0.0), glm::vec3(0.5, 0.5, 0.5));
		objects.push_back(obj);
	}*/
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
		glm::vec3 objectPosition = getPosition(newModelMatrix);
		glm::vec3 origin = glm::vec3(0, 0, 0);

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

void drawScene()
{

	glClearColor(0.4, 0.4, 0.4, 1);
	glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);

	if (app.showGridAxis)
	{
		drawObject(axisObject);
		drawObject(gridObject);
	}

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
	glm::mat4 modelM = objects.at(selectedObj).M;
	glm::vec3 pos = getPosition(modelM);
	std::stringstream posStream;
	posStream << std::fixed << std::setprecision(1) << pos.x << ", " << pos.y << ", " << pos.z;
	
	glm::vec3 rot = getRotation(modelM, true);
	std::stringstream rotationStream;
	rotationStream << std::fixed << std::setprecision(1) << rot.x << ", " << rot.y << ", " << rot.z;

	float scalingFactor = getScalingFactor(modelM);
	std::stringstream scaleStream;
	scaleStream << std::fixed << std::setprecision(5) << scalingFactor;

	std::string refSystem = "Ref. System: " + getCoordinateSystemName(app.coordinateSystem);
	std::string mode = "Operation: " + getOperationModeName(app.operationMode);
	std::string axis = "Axis: " + getWorkingAxisName(app.workingAxis);
	std::string object = "Object: " + objects.at(selectedObj).name;
	std::string position = " Pos.: (" + posStream.str() + ")";
	std::string rotation = " Rot.: (" + rotationStream.str() + ")";
	std::string scale = " Scale: " + scaleStream.str();
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
	if (app.showTransformInfo)
	{
		lines.push_back(scale);
		lines.push_back(rotation);
		lines.push_back(position);
	}
	lines.push_back(object);
	lines.push_back(axis);
	lines.push_back(mode);
	lines.push_back(refSystem);
	glDisable(GL_DEPTH_TEST);
	HUD_Logger::get()->printInfo(lines);
	glEnable(GL_DEPTH_TEST);

	resize(windowWidth, windowHeight);
}

int main(int argc, char** argv)
{
	printControls();

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
