#include "shader.h"

// Shaders Uniforms 
std::vector<LightShaderUniform> light_uniforms; // for shaders with light
std::vector<BaseShaderUniform> base_uniforms; // for ALL shaders
std::vector<GLuint> shaders_IDs; //Pointers to the shader programs


void initShaders(PointLight light)
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

void updateUniforms(Object object, PointLight light)
{
	switch (object.shader)
	{
	case PASS_THROUGH:
		// Caricamento matrice trasformazione del modello
		glUniformMatrix4fv(base_uniforms[PASS_THROUGH].M_Matrix_pointer, 1, GL_FALSE, value_ptr(object.M));
		break;

	case GOURAUD:
		// Caricamento matrice trasformazione del modello
		glUniformMatrix4fv(base_uniforms[GOURAUD].M_Matrix_pointer, 1, GL_FALSE, value_ptr(object.M));
		// Material loading
		glUniform3fv(light_uniforms[GOURAUD].material_ambient, 1, glm::value_ptr(object.material.ambient));
		glUniform3fv(light_uniforms[GOURAUD].material_diffuse, 1, glm::value_ptr(object.material.diffuse));
		glUniform3fv(light_uniforms[GOURAUD].material_specular, 1, glm::value_ptr(object.material.specular));
		glUniform1f(light_uniforms[GOURAUD].material_shininess, object.material.shininess);
		// Update light properties
		glUniform3f(light_uniforms[GOURAUD].light_position_pointer, light.position.x, light.position.y, light.position.z);
		glUniform1f(light_uniforms[GOURAUD].light_power_pointer, light.power);
		break;

	case PHONG:
		// Caricamento matrice trasformazione del modello
		glUniformMatrix4fv(base_uniforms[PHONG].M_Matrix_pointer, 1, GL_FALSE, value_ptr(object.M));
		// Material loading
		glUniform3fv(light_uniforms[PHONG].material_ambient, 1, glm::value_ptr(object.material.ambient));
		glUniform3fv(light_uniforms[PHONG].material_diffuse, 1, glm::value_ptr(object.material.diffuse));
		glUniform3fv(light_uniforms[PHONG].material_specular, 1, glm::value_ptr(object.material.specular));
		glUniform1f(light_uniforms[PHONG].material_shininess, object.material.shininess);
		// Update light properties
		glUniform3f(light_uniforms[PHONG].light_position_pointer, light.position.x, light.position.y, light.position.z);
		glUniform1f(light_uniforms[PHONG].light_power_pointer, light.power);
		break;

	case BLINN:
		// Caricamento matrice trasformazione del modello
		glUniformMatrix4fv(base_uniforms[BLINN].M_Matrix_pointer, 1, GL_FALSE, value_ptr(object.M));
		// Material loading
		glUniform3fv(light_uniforms[BLINN].material_ambient, 1, glm::value_ptr(object.material.ambient));
		glUniform3fv(light_uniforms[BLINN].material_diffuse, 1, glm::value_ptr(object.material.diffuse));
		glUniform3fv(light_uniforms[BLINN].material_specular, 1, glm::value_ptr(object.material.specular));
		glUniform1f(light_uniforms[BLINN].material_shininess, object.material.shininess);
		// Update light properties
		glUniform3f(light_uniforms[BLINN].light_position_pointer, light.position.x, light.position.y, light.position.z);
		glUniform1f(light_uniforms[BLINN].light_power_pointer, light.power);
		break;

	case WAVE:
		// Caricamento matrice trasformazione del modello
		glUniformMatrix4fv(base_uniforms[WAVE].M_Matrix_pointer, 1, GL_FALSE, value_ptr(object.M));
		// Time setting: clock() returns the amount of milliseconds from the application start
		glUniform1f(base_uniforms[WAVE].time_delta_pointer, clock());
		break;

	case WAVE_COLOR:
		// Caricamento matrice trasformazione del modello
		glUniformMatrix4fv(base_uniforms[WAVE_COLOR].M_Matrix_pointer, 1, GL_FALSE, value_ptr(object.M));
		// Time setting: clock() returns the amount of milliseconds from the application start
		glUniform1f(base_uniforms[WAVE_COLOR].time_delta_pointer, clock());
		break;

	case WAVE_LIGHT:
		// Caricamento matrice trasformazione del modello
		glUniformMatrix4fv(base_uniforms[WAVE_LIGHT].M_Matrix_pointer, 1, GL_FALSE, value_ptr(object.M));
		// Time setting: clock() returns the amount of milliseconds from the application start
		glUniform1f(base_uniforms[WAVE_LIGHT].time_delta_pointer, clock());
		// Material loading
		glUniform3fv(light_uniforms[WAVE_LIGHT].material_ambient, 1, glm::value_ptr(object.material.ambient));
		glUniform3fv(light_uniforms[WAVE_LIGHT].material_diffuse, 1, glm::value_ptr(object.material.diffuse));
		glUniform3fv(light_uniforms[WAVE_LIGHT].material_specular, 1, glm::value_ptr(object.material.specular));
		glUniform1f(light_uniforms[WAVE_LIGHT].material_shininess, object.material.shininess);
		// Update light properties
		glUniform3f(light_uniforms[WAVE_LIGHT].light_position_pointer, light.position.x, light.position.y, light.position.z);
		glUniform1f(light_uniforms[WAVE_LIGHT].light_power_pointer, light.power);
		break;

	case TOON:
		// Caricamento matrice trasformazione del modello
		glUniformMatrix4fv(base_uniforms[TOON].M_Matrix_pointer, 1, GL_FALSE, value_ptr(object.M));
		glUniform3f(light_uniforms[TOON].light_position_pointer, light.position.x, light.position.y, light.position.z);
		glUniform1f(light_uniforms[TOON].light_power_pointer, light.power);
		break;

	case TOON_V2:
		// Caricamento matrice trasformazione del modello
		glUniformMatrix4fv(base_uniforms[TOON_V2].M_Matrix_pointer, 1, GL_FALSE, value_ptr(object.M));
		// Material loading
		glUniform3fv(light_uniforms[TOON_V2].material_ambient, 1, glm::value_ptr(object.material.ambient));
		glUniform3fv(light_uniforms[TOON_V2].material_diffuse, 1, glm::value_ptr(object.material.diffuse));
		glUniform3fv(light_uniforms[TOON_V2].material_specular, 1, glm::value_ptr(object.material.specular));
		glUniform1f(light_uniforms[TOON_V2].material_shininess, object.material.shininess);
		// Update light properties
		glUniform3f(light_uniforms[TOON_V2].light_position_pointer, light.position.x, light.position.y, light.position.z);
		glUniform1f(light_uniforms[TOON_V2].light_power_pointer, light.power);
		break;

	default:
		break;
	}
}

/*std::map<ShadingType, Shader> shaders;

void initShaders(PointLight light)
{
	Shader shader = {};

	// SHADER: Pass-Through --------------------------------------------------------------
	shader.name = "Pass Through";
	shader.type = PASS_THROUGH;
	// 1. Loading
	shader.id = createProgram(SHADERS_DIR + "v_passthrough.glsl", SHADERS_DIR + "f_passthrough.glsl");
	// 2. Obtain pointers to uniform variables (so that we can use them afterwards)
	shader.baseUniform.P_Matrix_pointer = glGetUniformLocation(shader.id, "P");
	shader.baseUniform.V_Matrix_pointer = glGetUniformLocation(shader.id, "V");
	shader.baseUniform.M_Matrix_pointer = glGetUniformLocation(shader.id, "M");
	shader.lightUniform = {};
	// 3. Activate the shader
	glUseProgram(shader.id);
	glUniform4fv(glGetUniformLocation(shader.id, "Color"), 1, value_ptr(glm::vec4(1.0, 1.0, 1.0, 1.0)));
	// 4. Add the shader to the map
	shaders.insert({ PASS_THROUGH, shader });

	// SHADER: Gourand -------------------------------------------------------------------
	shader = {};
	shader.name = "Gouraud";
	shader.type = GOURAUD;
	// 1. Loading
	shader.id = createProgram(SHADERS_DIR + "v_gouraud.glsl", SHADERS_DIR + "f_gouraud.glsl");
	// 2. Obtain pointers to uniform variables 
	shader.baseUniform.P_Matrix_pointer = glGetUniformLocation(shader.id, "P");
	shader.baseUniform.V_Matrix_pointer = glGetUniformLocation(shader.id, "V");
	shader.baseUniform.M_Matrix_pointer = glGetUniformLocation(shader.id, "M");
	shader.lightUniform.material_ambient = glGetUniformLocation(shader.id, "material.ambient");
	shader.lightUniform.material_diffuse = glGetUniformLocation(shader.id, "material.diffuse");
	shader.lightUniform.material_specular = glGetUniformLocation(shader.id, "material.specular");
	shader.lightUniform.material_shininess = glGetUniformLocation(shader.id, "material.shininess");
	shader.lightUniform.light_position_pointer = glGetUniformLocation(shader.id, "light.position");
	shader.lightUniform.light_color_pointer = glGetUniformLocation(shader.id, "light.color");
	shader.lightUniform.light_power_pointer = glGetUniformLocation(shader.id, "light.power");
	shader.lightUniform = {};
	// 3. Activate the shader
	glUseProgram(shader.id);
	// 4. Shader uniforms initialization
	glUniform3f(shader.lightUniform.light_position_pointer, light.position.x, light.position.y, light.position.z);
	glUniform3f(shader.lightUniform.light_color_pointer, light.color.r, light.color.g, light.color.b);
	glUniform1f(shader.lightUniform.light_power_pointer, light.power);
	// 5. Add the shader to the map
	shaders.insert({ GOURAUD, shader });

	// SHADER: Phong ---------------------------------------------------------------------
	shader = {};
	shader.name = "Phong";
	shader.type = PHONG;
	// 1. Loading
	shader.id = createProgram(SHADERS_DIR + "v_phong.glsl", SHADERS_DIR + "f_phong.glsl");
	// 2. Obtain pointers to uniform variables
	shader.baseUniform.P_Matrix_pointer = glGetUniformLocation(shader.id, "P");
	shader.baseUniform.V_Matrix_pointer = glGetUniformLocation(shader.id, "V");
	shader.baseUniform.M_Matrix_pointer = glGetUniformLocation(shader.id, "M");
	shader.lightUniform.material_ambient = glGetUniformLocation(shader.id, "material.ambient");
	shader.lightUniform.material_diffuse = glGetUniformLocation(shader.id, "material.diffuse");
	shader.lightUniform.material_specular = glGetUniformLocation(shader.id, "material.specular");
	shader.lightUniform.material_shininess = glGetUniformLocation(shader.id, "material.shininess");
	shader.lightUniform.light_position_pointer = glGetUniformLocation(shader.id, "light.position");
	shader.lightUniform.light_color_pointer = glGetUniformLocation(shader.id, "light.color");
	shader.lightUniform.light_power_pointer = glGetUniformLocation(shader.id, "light.power");
	// 3. Activate the shader
	glUseProgram(shader.id);
	// 4. Shader uniforms initialization
	glUniform3f(shader.lightUniform.light_position_pointer, light.position.x, light.position.y, light.position.z);
	glUniform3f(shader.lightUniform.light_color_pointer, light.color.r, light.color.g, light.color.b);
	glUniform1f(shader.lightUniform.light_power_pointer, light.power);
	// 5. Add the shader to the map
	shaders.insert({ PHONG, shader });

	// SHADER: Blinn ---------------------------------------------------------------------
	shader = {};
	shader.name = "Blinn";
	shader.type = BLINN;
	// 1. Loading
	shader.id = createProgram(SHADERS_DIR + "v_blinn.glsl", SHADERS_DIR + "f_blinn.glsl");
	// 2. Obtain pointers to uniform variables
	shader.baseUniform.P_Matrix_pointer = glGetUniformLocation(shader.id, "P");
	shader.baseUniform.V_Matrix_pointer = glGetUniformLocation(shader.id, "V");
	shader.baseUniform.M_Matrix_pointer = glGetUniformLocation(shader.id, "M");
	shader.lightUniform.material_ambient = glGetUniformLocation(shader.id, "material.ambient");
	shader.lightUniform.material_diffuse = glGetUniformLocation(shader.id, "material.diffuse");
	shader.lightUniform.material_specular = glGetUniformLocation(shader.id, "material.specular");
	shader.lightUniform.material_shininess = glGetUniformLocation(shader.id, "material.shininess");
	shader.lightUniform.light_position_pointer = glGetUniformLocation(shader.id, "light.position");
	shader.lightUniform.light_color_pointer = glGetUniformLocation(shader.id, "light.color");
	shader.lightUniform.light_power_pointer = glGetUniformLocation(shader.id, "light.power");
	// 3. Activate the shader
	glUseProgram(shader.id);
	// 4. Shader uniforms initialization
	glUniform3f(shader.lightUniform.light_position_pointer, light.position.x, light.position.y, light.position.z);
	glUniform3f(shader.lightUniform.light_color_pointer, light.color.r, light.color.g, light.color.b);
	glUniform1f(shader.lightUniform.light_power_pointer, light.power);
	// 5. Add the shader to the map
	shaders.insert({ BLINN, shader });

	// TO-DO: OK
	// SHADER: Wave ----------------------------------------------------------------------
	shader = {};
	shader.name = "Wave";
	shader.type = WAVE;
	// 1. Loading
	shader.id = createProgram(SHADERS_DIR + "v_wave.glsl", SHADERS_DIR + "f_wave.glsl");
	// 2. Obtain pointers to uniform variables
	shader.baseUniform.P_Matrix_pointer = glGetUniformLocation(shader.id, "P");
	shader.baseUniform.V_Matrix_pointer = glGetUniformLocation(shader.id, "V");
	shader.baseUniform.M_Matrix_pointer = glGetUniformLocation(shader.id, "M");
	shader.baseUniform.time_delta_pointer = glGetUniformLocation(shader.id, "time");
	shader.lightUniform = {};
	// 3. Activate the shader
	glUseProgram(shader.id);
	// 4. Add the shader to the map
	shaders.insert({ WAVE, shader });

	// TO-DO Extra: OK
	// SHADER: Wave Color ----------------------------------------------------------------
	shader = {};
	shader.name = "Wave Color";
	shader.type = WAVE_COLOR;
	// 1. Loading
	shader.id = createProgram(SHADERS_DIR + "v_wave_color.glsl", SHADERS_DIR + "f_wave_color.glsl");
	// 2. Obtain pointers to uniform variables
	shader.baseUniform.P_Matrix_pointer = glGetUniformLocation(shader.id, "P");
	shader.baseUniform.V_Matrix_pointer = glGetUniformLocation(shader.id, "V");
	shader.baseUniform.M_Matrix_pointer = glGetUniformLocation(shader.id, "M");
	shader.baseUniform.time_delta_pointer = glGetUniformLocation(shader.id, "time");
	shader.lightUniform = {};
	// 3. Activate the shader
	glUseProgram(shader.id);
	// 4. Add the shader to the map
	shaders.insert({ WAVE_COLOR, shader });

	// TO-DO Extra: OK
	// SHADER: Wave Light ----------------------------------------------------------------
	shader = {};
	shader.name = "Wave Light";
	shader.type = WAVE_LIGHT;
	// 1. Loading
	shader.id = createProgram(SHADERS_DIR + "v_wave_light.glsl", SHADERS_DIR + "f_wave_light.glsl");
	// 2. Obtain pointers to uniform variables
	shader.baseUniform.P_Matrix_pointer = glGetUniformLocation(shader.id, "P");
	shader.baseUniform.V_Matrix_pointer = glGetUniformLocation(shader.id, "V");
	shader.baseUniform.M_Matrix_pointer = glGetUniformLocation(shader.id, "M");
	shader.baseUniform.time_delta_pointer = glGetUniformLocation(shader.id, "time");
	shader.lightUniform.light_position_pointer = glGetUniformLocation(shader.id, "light.position");
	shader.lightUniform.light_color_pointer = glGetUniformLocation(shader.id, "light.color");
	shader.lightUniform.light_power_pointer = glGetUniformLocation(shader.id, "light.power");
	shader.lightUniform.material_ambient = glGetUniformLocation(shader.id, "material.ambient");
	shader.lightUniform.material_diffuse = glGetUniformLocation(shader.id, "material.diffuse");
	shader.lightUniform.material_specular = glGetUniformLocation(shader.id, "material.specular");
	shader.lightUniform.material_shininess = glGetUniformLocation(shader.id, "material.shininess");
	// 3. Activate the shader
	glUseProgram(shader.id);
	// 4. Shader uniforms initialization
	glUniform3f(shader.lightUniform.light_position_pointer, light.position.x, light.position.y, light.position.z);
	glUniform3f(shader.lightUniform.light_color_pointer, light.color.r, light.color.g, light.color.b);
	glUniform1f(shader.lightUniform.light_power_pointer, light.power);
	// 5. Add the shader to the map
	shaders.insert({ WAVE_LIGHT, shader });

	// TO-DO Extra: OK
	// SHADER: Toon Shader ---------------------------------------------------------------
	shader = {};
	shader.name = "Toon";
	shader.type = TOON;
	// 1. Loading
	shader.id = createProgram(SHADERS_DIR + "v_toon.glsl", SHADERS_DIR + "f_toon.glsl");
	// 2. Obtain pointers to uniform variables
	shader.baseUniform.P_Matrix_pointer = glGetUniformLocation(shader.id, "P");
	shader.baseUniform.V_Matrix_pointer = glGetUniformLocation(shader.id, "V");
	shader.baseUniform.M_Matrix_pointer = glGetUniformLocation(shader.id, "M");
	shader.lightUniform.light_position_pointer = glGetUniformLocation(shader.id, "light.position");
	// 3. Activate the shader
	glUseProgram(shader.id);
	// 4. Shader uniforms initialization
	glUniform3f(shader.lightUniform.light_position_pointer, light.position.x, light.position.y, light.position.z);
	glUniform4fv(glGetUniformLocation(shader.id, "color"), 1, value_ptr(glm::vec4(1.0, 1.0, 1.0, 1.0)));
	// 5. Add the shader to the map
	shaders.insert({ TOON, shader });

	// TO-DO Extra: OK
	// SHADER: Toon V2 Shader ------------------------------------------------------------
	shader = {};
	shader.name = "Toon V2";
	shader.type = TOON_V2;
	// 1. Loading
	shader.id = createProgram(SHADERS_DIR + "v_toon_v2.glsl", SHADERS_DIR + "f_toon_v2.glsl");
	// 2. Obtain pointers to uniform variables
	shader.baseUniform.P_Matrix_pointer = glGetUniformLocation(shader.id, "P");
	shader.baseUniform.V_Matrix_pointer = glGetUniformLocation(shader.id, "V");
	shader.baseUniform.M_Matrix_pointer = glGetUniformLocation(shader.id, "M");
	shader.lightUniform.light_position_pointer = glGetUniformLocation(shader.id, "light.position");
	shader.lightUniform.light_color_pointer = glGetUniformLocation(shader.id, "light.color");
	shader.lightUniform.light_power_pointer = glGetUniformLocation(shader.id, "light.power");
	shader.lightUniform.material_ambient = glGetUniformLocation(shader.id, "material.ambient");
	shader.lightUniform.material_diffuse = glGetUniformLocation(shader.id, "material.diffuse");
	shader.lightUniform.material_specular = glGetUniformLocation(shader.id, "material.specular");
	shader.lightUniform.material_shininess = glGetUniformLocation(shader.id, "material.shininess");
	// 3. Activate the shader
	glUseProgram(shader.id);
	// 4. Shader uniforms initialization
	glUniform3f(shader.lightUniform.light_position_pointer, light.position.x, light.position.y, light.position.z);
	glUniform3f(shader.lightUniform.light_color_pointer, light.color.r, light.color.g, light.color.b);
	glUniform1f(shader.lightUniform.light_power_pointer, light.power);
	// 5. Add the shader to the map
	shaders.insert({ TOON_V2, shader });
}

void updateUniforms(Object object, PointLight light)
{
	BaseShaderUniform baseUniforms = object.shader.baseUniform;
	LightShaderUniform lightUniforms = object.shader.lightUniform;

	switch (object.shader.type)
	{
	case PASS_THROUGH:
		// Model matrix loading
		glUniformMatrix4fv(baseUniforms.M_Matrix_pointer, 1, GL_FALSE, value_ptr(object.M));
		break;

	case GOURAUD:
		// Model matrix loading
		glUniformMatrix4fv(baseUniforms.M_Matrix_pointer, 1, GL_FALSE, value_ptr(object.M));
		
		// Material loading
		glUniform3fv(lightUniforms.material_ambient, 1, glm::value_ptr(object.material.ambient));
		glUniform3fv(lightUniforms.material_diffuse, 1, glm::value_ptr(object.material.diffuse));
		glUniform3fv(lightUniforms.material_specular, 1, glm::value_ptr(object.material.specular));
		glUniform1f(lightUniforms.material_shininess, object.material.shininess);
		
		// Update light properties
		glUniform3f(lightUniforms.light_position_pointer, light.position.x, light.position.y, light.position.z);
		glUniform1f(lightUniforms.light_power_pointer, light.power);
		break;

	case PHONG:
		// Model matrix loading
		glUniformMatrix4fv(baseUniforms.M_Matrix_pointer, 1, GL_FALSE, value_ptr(object.M));
		
		// Material loading
		glUniform3fv(lightUniforms.material_ambient, 1, glm::value_ptr(object.material.ambient));
		glUniform3fv(lightUniforms.material_diffuse, 1, glm::value_ptr(object.material.diffuse));
		glUniform3fv(lightUniforms.material_specular, 1, glm::value_ptr(object.material.specular));
		glUniform1f(lightUniforms.material_shininess, object.material.shininess);
		
		// Update light properties
		glUniform3f(lightUniforms.light_position_pointer, light.position.x, light.position.y, light.position.z);
		glUniform1f(lightUniforms.light_power_pointer, light.power);
		break;

	case BLINN:
		// Model matrix loading
		glUniformMatrix4fv(baseUniforms.M_Matrix_pointer, 1, GL_FALSE, value_ptr(object.M));
		
		// Material loading
		glUniform3fv(lightUniforms.material_ambient, 1, glm::value_ptr(object.material.ambient));
		glUniform3fv(lightUniforms.material_diffuse, 1, glm::value_ptr(object.material.diffuse));
		glUniform3fv(lightUniforms.material_specular, 1, glm::value_ptr(object.material.specular));
		glUniform1f(lightUniforms.material_shininess, object.material.shininess);
		
		// Update light properties
		glUniform3f(lightUniforms.light_position_pointer, light.position.x, light.position.y, light.position.z);
		glUniform1f(lightUniforms.light_power_pointer, light.power);
		break;

	case WAVE:
		// Model matrix loading
		glUniformMatrix4fv(baseUniforms.M_Matrix_pointer, 1, GL_FALSE, value_ptr(object.M));
		
		// Time setting: clock() returns the amount of milliseconds from the application start
		glUniform1f(baseUniforms.time_delta_pointer, clock());
		break;

	case WAVE_COLOR:
		// Model matrix loading
		glUniformMatrix4fv(baseUniforms.M_Matrix_pointer, 1, GL_FALSE, value_ptr(object.M));
		
		// Time setting: clock() returns the amount of milliseconds from the application start
		glUniform1f(baseUniforms.time_delta_pointer, clock());
		break;

	case WAVE_LIGHT:
		// Model matrix loading
		glUniformMatrix4fv(baseUniforms.M_Matrix_pointer, 1, GL_FALSE, value_ptr(object.M));
		
		// Time setting: clock() returns the amount of milliseconds from the application start
		glUniform1f(baseUniforms.time_delta_pointer, clock());
		
		// Material loading
		glUniform3fv(lightUniforms.material_ambient, 1, glm::value_ptr(object.material.ambient));
		glUniform3fv(lightUniforms.material_diffuse, 1, glm::value_ptr(object.material.diffuse));
		glUniform3fv(lightUniforms.material_specular, 1, glm::value_ptr(object.material.specular));
		glUniform1f(lightUniforms.material_shininess, object.material.shininess);
		
		// Update light properties
		glUniform3f(lightUniforms.light_position_pointer, light.position.x, light.position.y, light.position.z);
		glUniform1f(lightUniforms.light_power_pointer, light.power);
		break;

	case TOON:
		// Model matrix loading
		glUniformMatrix4fv(baseUniforms.M_Matrix_pointer, 1, GL_FALSE, value_ptr(object.M));

		// Update light properties
		glUniform3f(lightUniforms.light_position_pointer, light.position.x, light.position.y, light.position.z);
		glUniform1f(lightUniforms.light_power_pointer, light.power);
		break;

	case TOON_V2:
		// Model matrix loading
		glUniformMatrix4fv(baseUniforms.M_Matrix_pointer, 1, GL_FALSE, value_ptr(object.M));
		
		// Material loading
		glUniform3fv(lightUniforms.material_ambient, 1, glm::value_ptr(object.material.ambient));
		glUniform3fv(lightUniforms.material_diffuse, 1, glm::value_ptr(object.material.diffuse));
		glUniform3fv(lightUniforms.material_specular, 1, glm::value_ptr(object.material.specular));
		glUniform1f(lightUniforms.material_shininess, object.material.shininess);
		
		// Update light properties
		glUniform3f(lightUniforms.light_position_pointer, light.position.x, light.position.y, light.position.z);
		glUniform1f(lightUniforms.light_power_pointer, light.power);
		break;

	default:
		break;
	}
}*/