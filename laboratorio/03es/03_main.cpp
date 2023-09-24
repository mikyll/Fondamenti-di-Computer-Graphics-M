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


const std::string ShaderDir = "Shaders/";

extern int windowWidth, windowHeight;
extern ViewSetup viewSetup;
extern PerspectiveSetup perspectiveSetup;

static Object Axis, Grid;
extern std::vector<Object> objects;
extern std::vector<Material> materials;

int selectedObj = 0;
static PointLight light;

int transformMode = WCS;
int operationMode = MODE_NAVIGATION;
int workingAxis = AXIS_X;

static bool moving_trackball = 0;
static int last_mouse_pos_Y;
static int last_mouse_pos_X;

// Shaders Uniforms 
static std::vector<LightShaderUniform> light_uniforms; // for shaders with light
static std::vector<BaseShaderUniform> base_uniforms; // for ALL shaders
static std::vector<GLuint> shaders_IDs; //Pointers to the shader programs




void initMeshes();
extern std::string getShaderName(ShadingType shadingType);

void initMeshes()
{
	// Axis for reference
	initMesh("axis.obj", "axis_", true, glm::vec3(), glm::vec3(), glm::vec3(2., 2., 2.), COPPER, BLINN);
	Axis = objects.at(objects.size() - 1);

	// White Grid Plane for reference
	initMesh("reference_grid.obj", "grid_", true, glm::vec3(), glm::vec3(), glm::vec3(1., 1., 1.), NO_MATERIAL, PASS_THROUGH);
	Grid = objects.at(objects.size() - 1);
	objects.pop_back();

	// Point Light
	initMesh("sphere_n_t_smooth.obj", "light", false, light.position, glm::vec3(), glm::vec3(0.2, 0.2, 0.2), NO_MATERIAL, GOURAUD);

	// FLAT Sphere (face normals)
	initMesh("sphere_n_t_flat.obj", "Sphere FLAT", false, glm::vec3(3., 1., -6.), glm::vec3(), glm::vec3(1., 1., 1.), EMERALD, PHONG);

	// SMOOTH Sphere (vertex normals)
	initMesh("sphere_n_t_smooth.obj", "Sphere SMOOTH", false, glm::vec3(6., 1., -3.), glm::vec3(), glm::vec3(1., 1., 1.), SILVER, BLINN);

	// Waving plane
	initMesh("GridPlane.obj", "Waves", false, glm::vec3(0., -2., 0.), glm::vec3(), glm::vec3(8., 8., 8.), TURQUOISE, WAVE_LIGHT);

	// Bunny model
	initMesh("bunny.obj", "Bunny", true, glm::vec3(0., 0., -2.), glm::vec3(), glm::vec3(2., 2., 2.), RED_PLASTIC, TOON);

	// Airplane model
	initMesh("airplane.obj", "Airplane", true, glm::vec3(-10., 5., 0.), glm::vec3(0.0, 0.0, -40.0), glm::vec3(5., 5., 5.), RED_PLASTIC, PHONG);

	// Horse model
	initMesh("horse.obj", "Horse", true, glm::vec3(-3., 2., 5.), glm::vec3(0.0, 225.0f, 0.0), glm::vec3(0.5, 0.5, 0.5), GOLD, PHONG);

	// Horse model
	initMesh("horse.obj", "Horse", false, glm::vec3(-5., 2., 7.), glm::vec3(0.0, 225.0f, 0.0), glm::vec3(0.5, 0.5, 0.5), SLATE, PHONG);
}

// Main initialization funtion
void init();
// Reshape Function
void resize(int w, int h);
// Calls glutPostRedisplay each millis milliseconds
void refresh_monitor(int millis);
// Mouse Function
void mouseInput(int button, int state, int x, int y);
// Keyboard: g traslate r rotate s scale x,y,z axis esc 
void keyboardDown(unsigned char key, int x, int y);
// Special key arrow: select active object (arrows left,right)
void special(int key, int x, int y);
// gestione delle voci principali del sub menu per i matriali
void material_menu_function(int option);
// costruisce i menu openGL
void buildOpenGLMenu();
// Trackball: Converte un punto 2D sullo schermo in un punto 3D sulla trackball
glm::vec3 getTrackBallPoint(float x, float y);
// Trackball: Effettua la rotazione del vettore posizione sulla trackball
void mouseActiveMotion(int x, int y);



//	Crea ed applica la matrice di trasformazione alla matrice dell'oggetto discriminando tra WCS e OCS.
//	La funzione è gia invocata con un input corretto, è sufficiente concludere la sua implementazione.
void modifyModelMatrix(glm::vec3 translation_vector, glm::vec3 rotation_vector, GLfloat angle, GLfloat scale_factor);
/* Mesh Functions*/
// Genera i buffer per la mesh in input e ne salva i puntatori di openGL
void generate_and_load_buffers(bool generate, Mesh* mesh);
// legge un file obj ed inizializza i vector della mesh in input
void loadObjFile(std::string file_path, Mesh* mesh, bool vertices_normals);
// disegna l'origine del assi
void drawAxisAndGrid();
// 2D fixed pipeline Font rendering on screen
void printToScreen();



void initShader()
{
	// SHADERS configuration section
	shaders_IDs.resize(NUM_SHADERS);
	light_uniforms.resize(NUM_SHADERS); // allocate space for uniforms of PHONG, BLINN and GOURAND + TOON
	base_uniforms.resize(NUM_SHADERS); // allocate space for uniforms of PHONG,BLINN,GOURAND,TOON,WAVE

	BaseShaderUniform base_unif = {};
	LightShaderUniform light_unif = {};

	// Pass-Through Shader loading
	shaders_IDs[PASS_THROUGH] = createProgram(ShaderDir + "v_passthrough.glsl", ShaderDir + "f_passthrough.glsl");
	// Otteniamo i puntatori alle variabili uniform per poterle utilizzare in seguito
	base_unif.P_Matrix_pointer = glGetUniformLocation(shaders_IDs[PASS_THROUGH], "P");
	base_unif.V_Matrix_pointer = glGetUniformLocation(shaders_IDs[PASS_THROUGH], "V");
	base_unif.M_Matrix_pointer = glGetUniformLocation(shaders_IDs[PASS_THROUGH], "M");
	base_uniforms[PASS_THROUGH] = base_unif;
	glUseProgram(shaders_IDs[PASS_THROUGH]);
	glUniform4fv(glGetUniformLocation(shaders_IDs[PASS_THROUGH], "Color"), 1, value_ptr(glm::vec4(1.0, 1.0, 1.0, 1.0)));

	// Gourand Shader loading
	shaders_IDs[GOURAUD] = createProgram(ShaderDir + "v_gouraud.glsl", ShaderDir + "f_gouraud.glsl");
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
	shaders_IDs[PHONG] = createProgram(ShaderDir + "v_phong.glsl", ShaderDir + "f_phong.glsl");
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
	shaders_IDs[BLINN] = createProgram(ShaderDir + "v_blinn.glsl", ShaderDir + "f_blinn.glsl");
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
	shaders_IDs[WAVE] = createProgram(ShaderDir + "v_wave.glsl", ShaderDir + "f_wave.glsl");
	base_unif.P_Matrix_pointer = glGetUniformLocation(shaders_IDs[WAVE], "P");
	base_unif.V_Matrix_pointer = glGetUniformLocation(shaders_IDs[WAVE], "V");
	base_unif.M_Matrix_pointer = glGetUniformLocation(shaders_IDs[WAVE], "M");
	base_unif.time_delta_pointer = glGetUniformLocation(shaders_IDs[WAVE], "time");
	base_uniforms[WAVE] = base_unif;
	// Rendiamo attivo lo shader
	glUseProgram(shaders_IDs[WAVE]);

	// Wave Shader Loading
	// TO-DO Extra: OK
	shaders_IDs[WAVE_COLOR] = createProgram(ShaderDir + "v_wave_color.glsl", ShaderDir + "f_wave_color.glsl");
	base_unif.P_Matrix_pointer = glGetUniformLocation(shaders_IDs[WAVE_COLOR], "P");
	base_unif.V_Matrix_pointer = glGetUniformLocation(shaders_IDs[WAVE_COLOR], "V");
	base_unif.M_Matrix_pointer = glGetUniformLocation(shaders_IDs[WAVE_COLOR], "M");
	base_unif.time_delta_pointer = glGetUniformLocation(shaders_IDs[WAVE_COLOR], "time");
	base_uniforms[WAVE_COLOR] = base_unif;
	// Rendiamo attivo lo shader
	glUseProgram(shaders_IDs[WAVE_COLOR]);

	// Wave Light Shader Loading
	// TO-DO Extra: OK
	shaders_IDs[WAVE_LIGHT] = createProgram(ShaderDir + "v_wave_light.glsl", ShaderDir + "f_wave_light.glsl");
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
	shaders_IDs[TOON] = createProgram(ShaderDir + "v_toon.glsl", ShaderDir + "f_toon.glsl");
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
	shaders_IDs[TOON_V2] = createProgram(ShaderDir + "v_toon_v2.glsl", ShaderDir + "f_toon_v2.glsl");
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

	initCamera();

	initMeshes();
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
				glUniform3fv(light_uniforms[GOURAUD].material_ambient, 1, glm::value_ptr(materials[objects.at(i).material].ambient));
				glUniform3fv(light_uniforms[GOURAUD].material_diffuse, 1, glm::value_ptr(materials[objects.at(i).material].diffuse));
				glUniform3fv(light_uniforms[GOURAUD].material_specular, 1, glm::value_ptr(materials[objects.at(i).material].specular));
				glUniform1f(light_uniforms[GOURAUD].material_shininess, materials[objects.at(i).material].shininess);
				glUniform3f(light_uniforms[GOURAUD].light_position_pointer, light.position.x, light.position.y, light.position.z);
				break;

			case PHONG:
				glUseProgram(shaders_IDs[PHONG]);
				// Caricamento matrice trasformazione del modello
				glUniformMatrix4fv(base_uniforms[PHONG].M_Matrix_pointer, 1, GL_FALSE, value_ptr(objects.at(i).M));
				// Material loading
				glUniform3fv(light_uniforms[PHONG].material_ambient, 1, glm::value_ptr(materials[objects.at(i).material].ambient));
				glUniform3fv(light_uniforms[PHONG].material_diffuse, 1, glm::value_ptr(materials[objects.at(i).material].diffuse));
				glUniform3fv(light_uniforms[PHONG].material_specular, 1, glm::value_ptr(materials[objects.at(i).material].specular));
				glUniform1f(light_uniforms[PHONG].material_shininess, materials[objects.at(i).material].shininess);
				glUniform3f(light_uniforms[PHONG].light_position_pointer, light.position.x, light.position.y, light.position.z);
				break;

			case BLINN:
				glUseProgram(shaders_IDs[BLINN]);
				// Caricamento matrice trasformazione del modello
				glUniformMatrix4fv(base_uniforms[BLINN].M_Matrix_pointer, 1, GL_FALSE, value_ptr(objects.at(i).M));
				// Material loading
				glUniform3fv(light_uniforms[BLINN].material_ambient, 1, glm::value_ptr(materials[objects.at(i).material].ambient));
				glUniform3fv(light_uniforms[BLINN].material_diffuse, 1, glm::value_ptr(materials[objects.at(i).material].diffuse));
				glUniform3fv(light_uniforms[BLINN].material_specular, 1, glm::value_ptr(materials[objects.at(i).material].specular));
				glUniform1f(light_uniforms[BLINN].material_shininess, materials[objects.at(i).material].shininess);
				glUniform3f(light_uniforms[BLINN].light_position_pointer, light.position.x, light.position.y, light.position.z);
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
				glUniform3fv(light_uniforms[WAVE_LIGHT].material_ambient, 1, glm::value_ptr(materials[objects.at(i).material].ambient));
				glUniform3fv(light_uniforms[WAVE_LIGHT].material_diffuse, 1, glm::value_ptr(materials[objects.at(i).material].diffuse));
				glUniform3fv(light_uniforms[WAVE_LIGHT].material_specular, 1, glm::value_ptr(materials[objects.at(i).material].specular));
				glUniform1f(light_uniforms[WAVE_LIGHT].material_shininess, materials[objects.at(i).material].shininess);
				glUniform3f(light_uniforms[WAVE_LIGHT].light_position_pointer, light.position.x, light.position.y, light.position.z);
				break;

			case TOON:
				glUseProgram(shaders_IDs[TOON]);
				// Caricamento matrice trasformazione del modello
				glUniformMatrix4fv(base_uniforms[TOON].M_Matrix_pointer, 1, GL_FALSE, value_ptr(objects.at(i).M));
				glUniform3f(light_uniforms[TOON].light_position_pointer, light.position.x, light.position.y, light.position.z);
				break;

			case TOON_V2:
				glUseProgram(shaders_IDs[TOON_V2]);
				// Caricamento matrice trasformazione del modello
				glUniformMatrix4fv(base_uniforms[TOON_V2].M_Matrix_pointer, 1, GL_FALSE, value_ptr(objects.at(i).M));
				// Material loading
				glUniform3fv(light_uniforms[TOON_V2].material_ambient, 1, glm::value_ptr(materials[objects.at(i).material].ambient));
				glUniform3fv(light_uniforms[TOON_V2].material_diffuse, 1, glm::value_ptr(materials[objects.at(i).material].diffuse));
				glUniform3fv(light_uniforms[TOON_V2].material_specular, 1, glm::value_ptr(materials[objects.at(i).material].specular));
				glUniform1f(light_uniforms[TOON_V2].material_shininess, materials[objects.at(i).material].shininess);
				glUniform3f(light_uniforms[TOON_V2].light_position_pointer, light.position.x, light.position.y, light.position.z);
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
	printToScreen();

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
	gluPerspective(perspectiveSetup.fovY, perspectiveSetup.aspect, perspectiveSetup.near_plane, perspectiveSetup.far_plane);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	gluLookAt(viewSetup.position.x, viewSetup.position.y, viewSetup.position.z,
		viewSetup.target.x, viewSetup.target.y, viewSetup.target.z,
		viewSetup.upVector.x, viewSetup.upVector.y, viewSetup.upVector.z);

	// Programmable Pipeline matrices for object rendering
	glm::mat4 P = glm::perspective(perspectiveSetup.fovY, perspectiveSetup.aspect, perspectiveSetup.near_plane, perspectiveSetup.far_plane);
	glm::mat4 V = glm::lookAt(glm::vec3(viewSetup.position), glm::vec3(viewSetup.target), glm::vec3(viewSetup.upVector));

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

void mouseInput(int button, int state, int x, int y)
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

			operationMode = MODE_NAVIGATION;
			last_mouse_pos_X = x;
			last_mouse_pos_Y = y;
			break;

		default:
			break;
	}
	// Selezione dell'asse per le trasformazioni
	switch (workingAxis)
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

	switch (operationMode)
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
	if (objects.at(selectedObj).name == "light")
	{
		light.position = objects.at(selectedObj).M[3];
	}
}

void mouseActiveMotion(int x, int y)
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
		glm::vec4 direction = viewSetup.position - viewSetup.target;
		// rotazione del vettore direzione w 
		// determinazione della nuova posizione della camera 
		viewSetup.position = viewSetup.target + glm::rotate(glm::mat4(1.0f), glm::radians(-angle), rotation_vec) * direction;
	}
	last_mouse_pos_X = x; last_mouse_pos_Y = y;
	glutPostRedisplay();
}

void keyboardDown(unsigned char key, int x, int y)
{
	int st = objects.at(selectedObj).shader;
	
	switch (key) {
		// Selezione della modalità di trasformazione
	case 'g':
		operationMode = MODE_TRASLATING;
		break;
	case 'r':
		operationMode = MODE_ROTATING;
		break;
	case 's':
		operationMode = MODE_SCALING;
		break;
	case 27:
		glutLeaveMainLoop();
		break;
		// Selezione dell'asse
	case 'x':
		workingAxis = AXIS_X;
		break;
	case 'y':
		workingAxis = AXIS_Y;
		break;
	case 'z':
		workingAxis = AXIS_Z;
		break;
	case '-':
		st--;
		if (st < 0)
			st = NUM_SHADERS - 1;
		objects.at(selectedObj).shader = st;
		break;
	case '+':
		st++;
		if (st == NUM_SHADERS)
			st = 0;
		objects.at(selectedObj).shader = st;
		break;
	default:
		break;
	}
	glutPostRedisplay();
}

void special(int key, int x, int y)
{
	switch (key)
	{
	case GLUT_KEY_LEFT:
		selectedObj = selectedObj > 0 ? selectedObj - 1 : objects.size() - 1;
		break;
	case GLUT_KEY_RIGHT:
		selectedObj = (selectedObj + 1) < objects.size() ? selectedObj + 1 : 0;
		break;
	default:
		break;
	}
	glutPostRedisplay();
}

glm::vec3 getTrackBallPoint(float x, float y)
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

void modifyModelMatrix(glm::vec3 translation_vector, glm::vec3 rotation_vector, GLfloat angle, GLfloat scale_factor)
{
	// TO-DO: OK
	glm::mat4 newModelMatrix = objects.at(selectedObj).M;
	// Translation
	newModelMatrix = glm::translate(newModelMatrix, translation_vector);
	// Rotation
	if (transformMode == OCS)
	{
		// Rotation with pivot on the object's center
		newModelMatrix = glm::rotate(newModelMatrix, glm::radians(angle), rotation_vector);
	}
	else if (transformMode == WCS)
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

void printToScreen()
{
	std::string ref = "Transform Mode: ";
	std::string mode = "Navigate/Modify: ";
	std::string axis = "Axis: ";
	std::string obj = "Object: " + objects[selectedObj].name;
	std::string mat = "  Material: " + materials[objects[selectedObj].material].name;
	std::string shad = "  Shading: ";

	switch (transformMode)
	{
		case OCS: ref += "OCS"; break;
		case WCS: ref += "WCS"; break;
	}
	switch (operationMode)
	{
		case MODE_TRASLATING: mode += "Translate"; break;
		case MODE_ROTATING: mode += "Rotate"; break;
		case MODE_SCALING: mode += "Scale"; break;
		case MODE_NAVIGATION: mode += "Navigate"; break;
	}
	switch (workingAxis)
	{
		case AXIS_X: axis += "X"; break;
		case AXIS_Y: axis += "Y"; break;
		case AXIS_Z: axis += "Z"; break;
	}

	shad += getShaderName(objects.at(selectedObj).shader);

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();

	// Handle output info layout 
	gluOrtho2D(0, windowHeight * ASPECT_RATIO, 0, windowHeight);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	std::vector<std::string> lines;
	lines.push_back(shad);
	lines.push_back(mat);
	lines.push_back(obj);
	lines.push_back(axis);
	lines.push_back(mode);
	lines.push_back(ref);
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
	glutKeyboardFunc(keyboardDown);
	glutMouseFunc(mouseInput);
	glutMotionFunc(mouseActiveMotion);
	glutSpecialFunc(special);

	glewExperimental = GL_TRUE;
	GlewInitResult = glewInit();
	fprintf(
		stdout,
		"INFO: OpenGL Version: %s\n",
		glGetString(GL_VERSION)
	);

	init();
	initMaterials();
	initShader();

	buildOpenGLMenu();
	refresh_monitor(16);
	glutMainLoop();
}
