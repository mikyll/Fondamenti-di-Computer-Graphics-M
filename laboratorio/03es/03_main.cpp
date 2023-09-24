/*
* Lab 03 - Scene Interactive Navigation with 3D Mesh Models
*
* Solution by Michele Righi (0001025005)
* GitHub: https://github.com/mikyll/Fondamenti-di-Computer-Graphics-M/tree/main/laboratorio/03es
* 
* 
* Description:
* 
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
/*Application app = {};

void initApplication()
{
	app.windowWidth = DEFAULT_WINDOW_WIDTH;
	app.windowHeight = DEFAULT_WINDOW_HEIGHT;
	app.coordinateSystem = WCS;
	app.operationMode = MODE_NAVIGATION;
	app.workingAxis = AXIS_X;
}*/

extern int windowWidth, windowHeight;
extern Camera camera;

extern std::map<MaterialType, Material> materials;
extern std::map<ShadingType, Shader> shaders;
std::vector<Object> objects;
static Object Axis, Grid;

//extern void initShaders(PointLight light);
extern void initMaterials();
extern std::string getCoordinateSystemName(CoordinateSystem coordinateSystem);
extern std::string getOperationModeName(OperationMode operationMode);
extern std::string getWorkingAxisName(ShadingType shadingType);

int selectedObj = 0;
PointLight light;

CoordinateSystem coordinateSystem = WCS;
OperationMode operationMode = MODE_NAVIGATION;
WorkingAxis workingAxis = AXIS_X;

// Shaders Uniforms 
static std::vector<LightShaderUniform> light_uniforms; // for shaders with light
static std::vector<BaseShaderUniform> base_uniforms; // for ALL shaders
static std::vector<GLuint> shaders_IDs; //Pointers to the shader programs


void initObjects();


void initObjects()
{
	Object obj;
	Mesh mesh;

	// Axis for reference
	mesh = loadMesh("axis.obj", VERTEX_NORMALS);
	//obj = createObject("axis_", mesh, materials.at(COPPER), shaders.at(BLINN), glm::vec3(), glm::vec3(), glm::vec3(2., 2., 2.));
	obj = createObject("axis_", mesh, materials.at(COPPER), BLINN, glm::vec3(), glm::vec3(), glm::vec3(2., 2., 2.));
	Axis = obj;

	// White Grid Plane for reference
	mesh = loadMesh("reference_grid.obj", VERTEX_NORMALS);
	//obj = createObject("grid_", mesh, materials.at(NO_MATERIAL), shaders.at(PASS_THROUGH), glm::vec3(), glm::vec3(), glm::vec3(1., 1., 1.));
	obj = createObject("grid_", mesh, materials.at(NO_MATERIAL), PASS_THROUGH, glm::vec3(), glm::vec3(), glm::vec3(1., 1., 1.));
	Grid = obj;

	// Point Light
	mesh = loadMesh("sphere_n_t_smooth.obj", FACE_NORMALS);
	//obj = createObject("light", mesh, materials.at(NO_MATERIAL), shaders.at(GOURAUD), light.position, glm::vec3(), glm::vec3(0.2, 0.2, 0.2));
	obj = createObject("light", mesh, materials.at(NO_MATERIAL), GOURAUD, light.position, glm::vec3(), glm::vec3(0.2, 0.2, 0.2));
	objects.push_back(obj);

	// FLAT Sphere (face normals)
	mesh = loadMesh("sphere_n_t_flat.obj", FACE_NORMALS);
	//obj = createObject("Sphere FLAT", mesh, materials.at(EMERALD), shaders.at(PHONG), glm::vec3(3., 1., -6.), glm::vec3(), glm::vec3(1., 1., 1.));
	obj = createObject("Sphere FLAT", mesh, materials.at(EMERALD), PHONG, glm::vec3(3., 1., -6.), glm::vec3(), glm::vec3(1., 1., 1.));
	objects.push_back(obj);
	
	// SMOOTH Sphere (vertex normals)
	mesh = loadMesh("sphere_n_t_smooth.obj", FACE_NORMALS);
	//obj = createObject("Sphere SMOOTH", mesh, materials.at(SILVER), shaders.at(BLINN), glm::vec3(6., 1., -3.), glm::vec3(), glm::vec3(1., 1., 1.));
	obj = createObject("Sphere SMOOTH", mesh, materials.at(SILVER), BLINN, glm::vec3(6., 1., -3.), glm::vec3(), glm::vec3(1., 1., 1.));
	objects.push_back(obj);
	
	// Waving plane
	mesh = loadMesh("GridPlane.obj", FACE_NORMALS);
	//obj = createObject("Waves", mesh, materials.at(TURQUOISE), shaders.at(WAVE_LIGHT), glm::vec3(0., -2., 0.), glm::vec3(), glm::vec3(8., 8., 8.));
	obj = createObject("Waves", mesh, materials.at(TURQUOISE), WAVE_LIGHT, glm::vec3(0., -2., 0.), glm::vec3(), glm::vec3(8., 8., 8.));
	objects.push_back(obj);
	
	// Bunny model
	mesh = loadMesh("bunny.obj", VERTEX_NORMALS);
	//obj = createObject("Bunny", mesh, materials.at(RED_PLASTIC), shaders.at(TOON), glm::vec3(0., 0., -2.), glm::vec3(), glm::vec3(2., 2., 2.));
	obj = createObject("Bunny", mesh, materials.at(RED_PLASTIC), TOON, glm::vec3(0., 0., -2.), glm::vec3(), glm::vec3(2., 2., 2.));
	objects.push_back(obj);
	
	// Airplane model
	mesh = loadMesh("airplane.obj", VERTEX_NORMALS);
	//obj = createObject("Airplane", mesh, materials.at(RED_PLASTIC), shaders.at(PHONG), glm::vec3(-10., 5., 0.), glm::vec3(0.0, 0.0, -40.0), glm::vec3(5., 5., 5.));
	obj = createObject("Airplane", mesh, materials.at(RED_PLASTIC), PHONG, glm::vec3(-10., 5., 0.), glm::vec3(0.0, 0.0, -40.0), glm::vec3(5., 5., 5.));
	objects.push_back(obj);
	
	// Horse model
	mesh = loadMesh("horse.obj", VERTEX_NORMALS);
	//obj = createObject("Horse FLAT", mesh, materials.at(GOLD), shaders.at(PHONG), glm::vec3(-3., 2., 5.), glm::vec3(0.0, 225.0f, 0.0), glm::vec3(0.5, 0.5, 0.5));
	obj = createObject("Horse FLAT", mesh, materials.at(GOLD), PHONG, glm::vec3(-3., 2., 5.), glm::vec3(0.0, 225.0f, 0.0), glm::vec3(0.5, 0.5, 0.5));
	objects.push_back(obj);
	
	// Horse model
	mesh = loadMesh("horse.obj", FACE_NORMALS);
	//obj = createObject("Horse SMOOTH", mesh, materials.at(SLATE), shaders.at(PHONG), glm::vec3(-5., 2., 7.), glm::vec3(0.0, 225.0f, 0.0), glm::vec3(0.5, 0.5, 0.5));
	obj = createObject("Horse SMOOTH", mesh, materials.at(SLATE), PHONG, glm::vec3(-5., 2., 7.), glm::vec3(0.0, 225.0f, 0.0), glm::vec3(0.5, 0.5, 0.5));
	objects.push_back(obj);
	
	// Test
	//initBrokenMesh("horse.obj", "Horse", FACE_NORMALS, glm::vec3(-5., 2., -5), glm::vec3(0.0, 225.0f, 0.0), glm::vec3(0.5, 0.5, 0.5), materials.at(GOLD), shaders.at(PHONG));
}

// Main initialization funtion
void init();
// Reshape Function
void resize(int w, int h);
// Calls glutPostRedisplay each millis milliseconds
void refresh_monitor(int millis);
// Trackball: Effettua la rotazione del vettore posizione sulla trackball
void mouseActiveMotion(int x, int y);



//	Crea ed applica la matrice di trasformazione alla matrice dell'oggetto discriminando tra WCS e OCS.
//	La funzione è gia invocata con un input corretto, è sufficiente concludere la sua implementazione.
void modifyModelMatrix(glm::vec3 translation_vector, glm::vec3 rotation_vector, GLfloat angle, GLfloat scale_factor);

// disegna l'origine del assi
void drawAxisAndGrid();
// 2D fixed pipeline Font rendering on screen
void printSceneInfo();



void initShaders()
{
	// SHADERS configuration section
	shaders_IDs.resize(NUM_SHADERS);
	light_uniforms.resize(NUM_SHADERS); // allocate space for uniforms of PHONG, BLINN and GOURAND + TOON
	base_uniforms.resize(NUM_SHADERS); // allocate space for uniforms of PHONG,BLINN,GOURAND,TOON,WAVE

	BaseShaderUniform base_unif = {};
	LightShaderUniform light_unif = {};

	// Pass-Through Shader loading
	shaders_IDs[PASS_THROUGH] = createProgram(SHADERS_DIR + "v_passthrough.glsl", SHADERS_DIR + "f_passthrough.glsl");
	// Otteniamo i puntatori alle variabili uniform per poterle utilizzare in seguito
	base_unif.P_Matrix_pointer = glGetUniformLocation(shaders_IDs[PASS_THROUGH], "P");
	base_unif.V_Matrix_pointer = glGetUniformLocation(shaders_IDs[PASS_THROUGH], "V");
	base_unif.M_Matrix_pointer = glGetUniformLocation(shaders_IDs[PASS_THROUGH], "M");
	base_uniforms[PASS_THROUGH] = base_unif;
	glUseProgram(shaders_IDs[PASS_THROUGH]);
	glUniform4fv(glGetUniformLocation(shaders_IDs[PASS_THROUGH], "Color"), 1, value_ptr(glm::vec4(1.0, 1.0, 1.0, 1.0)));

	// Gourand Shader loading
	shaders_IDs[GOURAUD] = createProgram(SHADERS_DIR + "v_gouraud.glsl", SHADERS_DIR + "f_gouraud.glsl");
	// Otteniamo i puntatori alle variabili uniform per poterle utilizzare in seguito
	base_unif.P_Matrix_pointer = glGetUniformLocation(shaders_IDs[GOURAUD], "P");
	base_unif.V_Matrix_pointer = glGetUniformLocation(shaders_IDs[GOURAUD], "V");
	base_unif.M_Matrix_pointer = glGetUniformLocation(shaders_IDs[GOURAUD], "M");
	base_uniforms[GOURAUD] = base_unif;
	light_unif.material_ambient = glGetUniformLocation(shaders_IDs[GOURAUD], "material.ambient");
	light_unif.material_diffuse = glGetUniformLocation(shaders_IDs[GOURAUD], "material.diffuse");
	light_unif.material_specular = glGetUniformLocation(shaders_IDs[GOURAUD], "material.specular");
	light_unif.material_shininess = glGetUniformLocation(shaders_IDs[GOURAUD], "material.shininess");
	light_unif.light_position_pointer = glGetUniformLocation(shaders_IDs[GOURAUD], "light.position");
	light_unif.light_color_pointer = glGetUniformLocation(shaders_IDs[GOURAUD], "light.color");
	light_unif.light_power_pointer = glGetUniformLocation(shaders_IDs[GOURAUD], "light.power");
	light_uniforms[GOURAUD] = light_unif;
	// Rendiamo attivo lo shader
	glUseProgram(shaders_IDs[GOURAUD]);
	// Shader uniforms initialization
	glUniform3f(light_uniforms[GOURAUD].light_position_pointer, light.position.x, light.position.y, light.position.z);
	glUniform3f(light_uniforms[GOURAUD].light_color_pointer, light.color.r, light.color.g, light.color.b);
	glUniform1f(light_uniforms[GOURAUD].light_power_pointer, light.power);

	// Phong Shader loading
	shaders_IDs[PHONG] = createProgram(SHADERS_DIR + "v_phong.glsl", SHADERS_DIR + "f_phong.glsl");
	// Otteniamo i puntatori alle variabili uniform per poterle utilizzare in seguito
	base_unif.P_Matrix_pointer = glGetUniformLocation(shaders_IDs[PHONG], "P");
	base_unif.V_Matrix_pointer = glGetUniformLocation(shaders_IDs[PHONG], "V");
	base_unif.M_Matrix_pointer = glGetUniformLocation(shaders_IDs[PHONG], "M");
	base_uniforms[PHONG] = base_unif;
	light_unif.material_ambient = glGetUniformLocation(shaders_IDs[PHONG], "material.ambient");
	light_unif.material_diffuse = glGetUniformLocation(shaders_IDs[PHONG], "material.diffuse");
	light_unif.material_specular = glGetUniformLocation(shaders_IDs[PHONG], "material.specular");
	light_unif.material_shininess = glGetUniformLocation(shaders_IDs[PHONG], "material.shininess");
	light_unif.light_position_pointer = glGetUniformLocation(shaders_IDs[PHONG], "light.position");
	light_unif.light_color_pointer = glGetUniformLocation(shaders_IDs[PHONG], "light.color");
	light_unif.light_power_pointer = glGetUniformLocation(shaders_IDs[PHONG], "light.power");
	light_uniforms[PHONG] = light_unif;
	// Rendiamo attivo lo shader
	glUseProgram(shaders_IDs[PHONG]);
	// Shader uniforms initialization
	glUniform3f(light_uniforms[PHONG].light_position_pointer, light.position.x, light.position.y, light.position.z);
	glUniform3f(light_uniforms[PHONG].light_color_pointer, light.color.r, light.color.g, light.color.b);
	glUniform1f(light_uniforms[PHONG].light_power_pointer, light.power);

	// Blinn Shader loading
	shaders_IDs[BLINN] = createProgram(SHADERS_DIR + "v_blinn.glsl", SHADERS_DIR + "f_blinn.glsl");
	base_unif.P_Matrix_pointer = glGetUniformLocation(shaders_IDs[BLINN], "P");
	base_unif.V_Matrix_pointer = glGetUniformLocation(shaders_IDs[BLINN], "V");
	base_unif.M_Matrix_pointer = glGetUniformLocation(shaders_IDs[BLINN], "M");
	base_uniforms[BLINN] = base_unif;
	light_unif.material_ambient = glGetUniformLocation(shaders_IDs[BLINN], "material.ambient");
	light_unif.material_diffuse = glGetUniformLocation(shaders_IDs[BLINN], "material.diffuse");
	light_unif.material_specular = glGetUniformLocation(shaders_IDs[BLINN], "material.specular");
	light_unif.material_shininess = glGetUniformLocation(shaders_IDs[BLINN], "material.shininess");
	light_unif.light_position_pointer = glGetUniformLocation(shaders_IDs[BLINN], "light.position");
	light_unif.light_color_pointer = glGetUniformLocation(shaders_IDs[BLINN], "light.color");
	light_unif.light_power_pointer = glGetUniformLocation(shaders_IDs[BLINN], "light.power");
	light_uniforms[BLINN] = light_unif;
	// Rendiamo attivo lo shader
	glUseProgram(shaders_IDs[BLINN]);
	// Shader uniforms initialization
	glUniform3f(light_uniforms[BLINN].light_position_pointer, light.position.x, light.position.y, light.position.z);
	glUniform3f(light_uniforms[BLINN].light_color_pointer, light.color.r, light.color.g, light.color.b);
	glUniform1f(light_uniforms[BLINN].light_power_pointer, light.power);

	// Wave Shader Loading
	// TO-DO: OK
	shaders_IDs[WAVE] = createProgram(SHADERS_DIR + "v_wave.glsl", SHADERS_DIR + "f_wave.glsl");
	base_unif.P_Matrix_pointer = glGetUniformLocation(shaders_IDs[WAVE], "P");
	base_unif.V_Matrix_pointer = glGetUniformLocation(shaders_IDs[WAVE], "V");
	base_unif.M_Matrix_pointer = glGetUniformLocation(shaders_IDs[WAVE], "M");
	base_unif.time_delta_pointer = glGetUniformLocation(shaders_IDs[WAVE], "time");
	base_uniforms[WAVE] = base_unif;
	// Rendiamo attivo lo shader
	glUseProgram(shaders_IDs[WAVE]);

	// Wave Shader Loading
	// TO-DO Extra: OK
	shaders_IDs[WAVE_COLOR] = createProgram(SHADERS_DIR + "v_wave_color.glsl", SHADERS_DIR + "f_wave_color.glsl");
	base_unif.P_Matrix_pointer = glGetUniformLocation(shaders_IDs[WAVE_COLOR], "P");
	base_unif.V_Matrix_pointer = glGetUniformLocation(shaders_IDs[WAVE_COLOR], "V");
	base_unif.M_Matrix_pointer = glGetUniformLocation(shaders_IDs[WAVE_COLOR], "M");
	base_unif.time_delta_pointer = glGetUniformLocation(shaders_IDs[WAVE_COLOR], "time");
	base_uniforms[WAVE_COLOR] = base_unif;
	// Rendiamo attivo lo shader
	glUseProgram(shaders_IDs[WAVE_COLOR]);

	// Wave Light Shader Loading
	// TO-DO Extra: OK
	shaders_IDs[WAVE_LIGHT] = createProgram(SHADERS_DIR + "v_wave_light.glsl", SHADERS_DIR + "f_wave_light.glsl");
	base_unif.P_Matrix_pointer = glGetUniformLocation(shaders_IDs[WAVE_LIGHT], "P");
	base_unif.V_Matrix_pointer = glGetUniformLocation(shaders_IDs[WAVE_LIGHT], "V");
	base_unif.M_Matrix_pointer = glGetUniformLocation(shaders_IDs[WAVE_LIGHT], "M");
	base_unif.time_delta_pointer = glGetUniformLocation(shaders_IDs[WAVE_LIGHT], "time");
	base_uniforms[WAVE_LIGHT] = base_unif;
	// Rendiamo attivo lo shader
	glUseProgram(shaders_IDs[WAVE_LIGHT]);
	light_unif.light_position_pointer = glGetUniformLocation(shaders_IDs[WAVE_LIGHT], "light.position");
	light_unif.light_color_pointer = glGetUniformLocation(shaders_IDs[WAVE_LIGHT], "light.color");
	light_unif.light_power_pointer = glGetUniformLocation(shaders_IDs[WAVE_LIGHT], "light.power");
	light_unif.material_ambient = glGetUniformLocation(shaders_IDs[WAVE_LIGHT], "material.ambient");
	light_unif.material_diffuse = glGetUniformLocation(shaders_IDs[WAVE_LIGHT], "material.diffuse");
	light_unif.material_specular = glGetUniformLocation(shaders_IDs[WAVE_LIGHT], "material.specular");
	light_unif.material_shininess = glGetUniformLocation(shaders_IDs[WAVE_LIGHT], "material.shininess");
	light_uniforms[WAVE_LIGHT] = light_unif;
	// Shader uniforms initialization
	glUniform3f(light_uniforms[WAVE_LIGHT].light_position_pointer, light.position.x, light.position.y, light.position.z);
	glUniform3f(light_uniforms[WAVE_LIGHT].light_color_pointer, light.color.r, light.color.g, light.color.b);
	glUniform1f(light_uniforms[WAVE_LIGHT].light_power_pointer, light.power);

	// Toon Shader Loading
	// TO-DO: OK
	shaders_IDs[TOON] = createProgram(SHADERS_DIR + "v_toon.glsl", SHADERS_DIR + "f_toon.glsl");
	base_unif.P_Matrix_pointer = glGetUniformLocation(shaders_IDs[TOON], "P");
	base_unif.V_Matrix_pointer = glGetUniformLocation(shaders_IDs[TOON], "V");
	base_unif.M_Matrix_pointer = glGetUniformLocation(shaders_IDs[TOON], "M");
	base_uniforms[TOON] = base_unif;
	light_unif.light_position_pointer = glGetUniformLocation(shaders_IDs[TOON], "light.position");
	light_uniforms[TOON] = light_unif;
	// Rendiamo attivo lo shader
	glUseProgram(shaders_IDs[TOON]);
	// Shader uniforms initialization
	glUniform3f(light_uniforms[TOON].light_position_pointer, light.position.x, light.position.y, light.position.z);
	// Set light color
	glUniform4fv(glGetUniformLocation(shaders_IDs[TOON], "color"), 1, value_ptr(glm::vec4(1.0, 1.0, 1.0, 1.0)));

	// Toon Shader Loading
	// TO-DO Extra: OK
	shaders_IDs[TOON_V2] = createProgram(SHADERS_DIR + "v_toon_v2.glsl", SHADERS_DIR + "f_toon_v2.glsl");
	base_unif.P_Matrix_pointer = glGetUniformLocation(shaders_IDs[TOON_V2], "P");
	base_unif.V_Matrix_pointer = glGetUniformLocation(shaders_IDs[TOON_V2], "V");
	base_unif.M_Matrix_pointer = glGetUniformLocation(shaders_IDs[TOON_V2], "M");
	base_uniforms[TOON_V2] = base_unif;
	// Rendiamo attivo lo shader
	glUseProgram(shaders_IDs[TOON_V2]);
	light_unif.light_position_pointer = glGetUniformLocation(shaders_IDs[TOON_V2], "light.position");
	light_unif.light_color_pointer = glGetUniformLocation(shaders_IDs[TOON_V2], "light.color");
	light_unif.light_power_pointer = glGetUniformLocation(shaders_IDs[TOON_V2], "light.power");
	light_unif.material_ambient = glGetUniformLocation(shaders_IDs[TOON_V2], "material.ambient");
	light_unif.material_diffuse = glGetUniformLocation(shaders_IDs[TOON_V2], "material.diffuse");
	light_unif.material_specular = glGetUniformLocation(shaders_IDs[TOON_V2], "material.specular");
	light_unif.material_shininess = glGetUniformLocation(shaders_IDs[TOON_V2], "material.shininess");
	light_uniforms[TOON_V2] = light_unif;
	// Shader uniforms initialization
	glUniform3f(light_uniforms[TOON_V2].light_position_pointer, light.position.x, light.position.y, light.position.z);
	glUniform3f(light_uniforms[TOON_V2].light_color_pointer, light.color.r, light.color.g, light.color.b);
	glUniform1f(light_uniforms[TOON_V2].light_power_pointer, light.power);
}

void init() {
	// Default render settings
	glEnable(GL_DEPTH_TEST);	// Hidden surface removal
	glCullFace(GL_BACK);	// remove faces facing the background
	glEnable(GL_LINE_SMOOTH);

	// Light initialization
	light.position = { 5.0,5.0,-5.0 };
	light.color = { 1.0,1.0,1.0 };
	light.power = 1.f;

	initMaterials();
	//initShaders(light);
	initShaders();
	initObjects();

	initCamera();
}

void drawScene() {

	glClearColor(0.4, 0.4, 0.4, 1);
	glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);

	
	drawAxisAndGrid(); // The central Axis point of reference

	for (int i = 0; i < objects.size(); i++) {
		// Shader selection
		switch (objects.at(i).shader)
		{
		case PASS_THROUGH:
			glUseProgram(shaders_IDs[PASS_THROUGH]);
			// Caricamento matrice trasformazione del modello
			glUniformMatrix4fv(base_uniforms[PASS_THROUGH].M_Matrix_pointer, 1, GL_FALSE, value_ptr(objects.at(i).M));
			break;

		case GOURAUD:
			glUseProgram(shaders_IDs[GOURAUD]);
			// Caricamento matrice trasformazione del modello
			glUniformMatrix4fv(base_uniforms[GOURAUD].M_Matrix_pointer, 1, GL_FALSE, value_ptr(objects.at(i).M));
			// Material loading
			glUniform3fv(light_uniforms[GOURAUD].material_ambient, 1, glm::value_ptr(objects.at(i).material.ambient));
			glUniform3fv(light_uniforms[GOURAUD].material_diffuse, 1, glm::value_ptr(objects.at(i).material.diffuse));
			glUniform3fv(light_uniforms[GOURAUD].material_specular, 1, glm::value_ptr(objects.at(i).material.specular));
			glUniform1f(light_uniforms[GOURAUD].material_shininess, objects.at(i).material.shininess);
			// Update light properties
			glUniform3f(light_uniforms[GOURAUD].light_position_pointer, light.position.x, light.position.y, light.position.z);
			glUniform1f(light_uniforms[GOURAUD].light_power_pointer, light.power);
			break;

		case PHONG:
			glUseProgram(shaders_IDs[PHONG]);
			// Caricamento matrice trasformazione del modello
			glUniformMatrix4fv(base_uniforms[PHONG].M_Matrix_pointer, 1, GL_FALSE, value_ptr(objects.at(i).M));
			// Material loading
			glUniform3fv(light_uniforms[PHONG].material_ambient, 1, glm::value_ptr(objects.at(i).material.ambient));
			glUniform3fv(light_uniforms[PHONG].material_diffuse, 1, glm::value_ptr(objects.at(i).material.diffuse));
			glUniform3fv(light_uniforms[PHONG].material_specular, 1, glm::value_ptr(objects.at(i).material.specular));
			glUniform1f(light_uniforms[PHONG].material_shininess, objects.at(i).material.shininess);
			// Update light properties
			glUniform3f(light_uniforms[PHONG].light_position_pointer, light.position.x, light.position.y, light.position.z);
			glUniform1f(light_uniforms[PHONG].light_power_pointer, light.power);
			break;

		case BLINN:
			glUseProgram(shaders_IDs[BLINN]);
			// Caricamento matrice trasformazione del modello
			glUniformMatrix4fv(base_uniforms[BLINN].M_Matrix_pointer, 1, GL_FALSE, value_ptr(objects.at(i).M));
			// Material loading
			glUniform3fv(light_uniforms[BLINN].material_ambient, 1, glm::value_ptr(objects.at(i).material.ambient));
			glUniform3fv(light_uniforms[BLINN].material_diffuse, 1, glm::value_ptr(objects.at(i).material.diffuse));
			glUniform3fv(light_uniforms[BLINN].material_specular, 1, glm::value_ptr(objects.at(i).material.specular));
			glUniform1f(light_uniforms[BLINN].material_shininess, objects.at(i).material.shininess);
			// Update light properties
			glUniform3f(light_uniforms[BLINN].light_position_pointer, light.position.x, light.position.y, light.position.z);
			glUniform1f(light_uniforms[BLINN].light_power_pointer, light.power);
			break;

		case WAVE:
			glUseProgram(shaders_IDs[WAVE]);
			// Caricamento matrice trasformazione del modello
			glUniformMatrix4fv(base_uniforms[WAVE].M_Matrix_pointer, 1, GL_FALSE, value_ptr(objects.at(i).M));
			// Time setting: clock() returns the amount of milliseconds from the application start
			glUniform1f(base_uniforms[WAVE].time_delta_pointer, clock());
			break;

		case WAVE_COLOR:
			glUseProgram(shaders_IDs[WAVE_COLOR]);
			// Caricamento matrice trasformazione del modello
			glUniformMatrix4fv(base_uniforms[WAVE_COLOR].M_Matrix_pointer, 1, GL_FALSE, value_ptr(objects.at(i).M));
			// Time setting: clock() returns the amount of milliseconds from the application start
			glUniform1f(base_uniforms[WAVE_COLOR].time_delta_pointer, clock());
			break;

		case WAVE_LIGHT:
			glUseProgram(shaders_IDs[WAVE_LIGHT]);
			// Caricamento matrice trasformazione del modello
			glUniformMatrix4fv(base_uniforms[WAVE_LIGHT].M_Matrix_pointer, 1, GL_FALSE, value_ptr(objects.at(i).M));
			// Time setting: clock() returns the amount of milliseconds from the application start
			glUniform1f(base_uniforms[WAVE_LIGHT].time_delta_pointer, clock());
			// Material loading
			glUniform3fv(light_uniforms[WAVE_LIGHT].material_ambient, 1, glm::value_ptr(objects.at(i).material.ambient));
			glUniform3fv(light_uniforms[WAVE_LIGHT].material_diffuse, 1, glm::value_ptr(objects.at(i).material.diffuse));
			glUniform3fv(light_uniforms[WAVE_LIGHT].material_specular, 1, glm::value_ptr(objects.at(i).material.specular));
			glUniform1f(light_uniforms[WAVE_LIGHT].material_shininess, objects.at(i).material.shininess);
			// Update light properties
			glUniform3f(light_uniforms[WAVE_LIGHT].light_position_pointer, light.position.x, light.position.y, light.position.z);
			glUniform1f(light_uniforms[WAVE_LIGHT].light_power_pointer, light.power);
			break;

		case TOON:
			glUseProgram(shaders_IDs[TOON]);
			// Caricamento matrice trasformazione del modello
			glUniformMatrix4fv(base_uniforms[TOON].M_Matrix_pointer, 1, GL_FALSE, value_ptr(objects.at(i).M));
			glUniform3f(light_uniforms[TOON].light_position_pointer, light.position.x, light.position.y, light.position.z);
			glUniform1f(light_uniforms[TOON].light_power_pointer, light.power);
			break;

		case TOON_V2:
			glUseProgram(shaders_IDs[TOON_V2]);
			// Caricamento matrice trasformazione del modello
			glUniformMatrix4fv(base_uniforms[TOON_V2].M_Matrix_pointer, 1, GL_FALSE, value_ptr(objects.at(i).M));
			// Material loading
			glUniform3fv(light_uniforms[TOON_V2].material_ambient, 1, glm::value_ptr(objects.at(i).material.ambient));
			glUniform3fv(light_uniforms[TOON_V2].material_diffuse, 1, glm::value_ptr(objects.at(i).material.diffuse));
			glUniform3fv(light_uniforms[TOON_V2].material_specular, 1, glm::value_ptr(objects.at(i).material.specular));
			glUniform1f(light_uniforms[TOON_V2].material_shininess, objects.at(i).material.shininess);
			// Update light properties
			glUniform3f(light_uniforms[TOON_V2].light_position_pointer, light.position.x, light.position.y, light.position.z);
			glUniform1f(light_uniforms[TOON_V2].light_power_pointer, light.power);
			break;

		default:
			break;
		}

		glEnableVertexAttribArray(0);
		glEnableVertexAttribArray(1);
		glBindVertexArray(objects.at(i).mesh.vertexArrayObjID);
		glDrawArrays(GL_TRIANGLES, 0, objects.at(i).mesh.vertices.size());

		glDisableVertexAttribArray(0);
		glDisableVertexAttribArray(1);
	}

	// OLD fixed pipeline for simple graphics and symbols
	glUseProgram(0);
	printSceneInfo();

	glutSwapBuffers();
}

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

	for (int i = 0; i < shaders_IDs.size(); i++) {
		glUseProgram(shaders_IDs[i]);
		glUniformMatrix4fv(base_uniforms[i].P_Matrix_pointer, 1, GL_FALSE, value_ptr(P));
		glUniformMatrix4fv(base_uniforms[i].V_Matrix_pointer, 1, GL_FALSE, value_ptr(V));
	}
}

void refresh_monitor(int millis)
{
	glutPostRedisplay();
	glutTimerFunc(millis, refresh_monitor, millis);
}

void modifyModelMatrix(glm::vec3 translation_vector, glm::vec3 rotation_vector, GLfloat angle, GLfloat scale_factor)
{
	// TO-DO: OK
	glm::mat4 newModelMatrix = objects.at(selectedObj).M;
	// Translation
	newModelMatrix = glm::translate(newModelMatrix, translation_vector);
	// Rotation
	if (coordinateSystem == OCS)
	{
		// Rotation with pivot on the object's center
		newModelMatrix = glm::rotate(newModelMatrix, glm::radians(angle), rotation_vector);
	}
	else if (coordinateSystem == WCS)
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

void drawAxisAndGrid()
{
	glUseProgram(shaders_IDs[Grid.shader]);
	glUniformMatrix4fv(base_uniforms[Grid.shader].M_Matrix_pointer, 1, GL_FALSE, value_ptr(Grid.M));
	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);
	glBindVertexArray(Grid.mesh.vertexArrayObjID);
	glDrawArrays(GL_TRIANGLES, 0, Grid.mesh.vertices.size());
	glDisableVertexAttribArray(0);
	glDisableVertexAttribArray(1);
	
	glUseProgram(shaders_IDs[Axis.shader]);
	glUniformMatrix4fv(base_uniforms[Axis.shader].M_Matrix_pointer, 1, GL_FALSE, value_ptr(Axis.M));
	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);
	glBindVertexArray(Axis.mesh.vertexArrayObjID);
	glDrawArrays(GL_TRIANGLES, 0, Axis.mesh.vertices.size());
	glDisableVertexAttribArray(0);
	glDisableVertexAttribArray(1);
}


void printSceneInfo()
{
	std::string refSystem =	"Ref. System: " + getCoordinateSystemName(coordinateSystem);
	std::string mode =		"Operation: " + getOperationModeName(operationMode);
	std::string axis =		"Axis: " + getWorkingAxisName(workingAxis);
	std::string object =	"Object: " + objects[selectedObj].name;
	std::string position =	" Pos.: " + objects[selectedObj].name;
	std::string rotation =	" Rot.: " + objects[selectedObj].name;
	std::string scale =		" Scale: " + objects[selectedObj].name;
	std::string material =	" Material: " + objects.at(selectedObj).material.name;
	std::string shader = " Shading: " + getShaderName(objects.at(selectedObj).shader);

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
