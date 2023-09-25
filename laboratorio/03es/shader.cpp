#include "shader.h"


std::map<ShadingType, Shader> shaders;

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
	switch (object.shader.type)
	{
	case PASS_THROUGH:
		// Caricamento matrice trasformazione del modello
		glUniformMatrix4fv(object.shader.baseUniform.M_Matrix_pointer, 1, GL_FALSE, value_ptr(object.M));
		break;

	case GOURAUD:
		// Caricamento matrice trasformazione del modello
		glUniformMatrix4fv(object.shader.baseUniform.M_Matrix_pointer, 1, GL_FALSE, value_ptr(object.M));
		// Material loading
		glUniform3fv(object.shader.lightUniform.material_ambient, 1, glm::value_ptr(object.material.ambient));
		glUniform3fv(object.shader.lightUniform.material_diffuse, 1, glm::value_ptr(object.material.diffuse));
		glUniform3fv(object.shader.lightUniform.material_specular, 1, glm::value_ptr(object.material.specular));
		glUniform1f(object.shader.lightUniform.material_shininess, object.material.shininess);
		// Update light properties
		glUniform3f(object.shader.lightUniform.light_position_pointer, light.position.x, light.position.y, light.position.z);
		glUniform1f(object.shader.lightUniform.light_power_pointer, light.power);
		break;

	case PHONG:
		// Caricamento matrice trasformazione del modello
		glUniformMatrix4fv(object.shader.baseUniform.M_Matrix_pointer, 1, GL_FALSE, value_ptr(object.M));
		// Material loading
		glUniform3fv(object.shader.lightUniform.material_ambient, 1, glm::value_ptr(object.material.ambient));
		glUniform3fv(object.shader.lightUniform.material_diffuse, 1, glm::value_ptr(object.material.diffuse));
		glUniform3fv(object.shader.lightUniform.material_specular, 1, glm::value_ptr(object.material.specular));
		glUniform1f(object.shader.lightUniform.material_shininess, object.material.shininess);
		// Update light properties
		glUniform3f(object.shader.lightUniform.light_position_pointer, light.position.x, light.position.y, light.position.z);
		glUniform1f(object.shader.lightUniform.light_power_pointer, light.power);
		break;

	case BLINN:
		// Caricamento matrice trasformazione del modello
		glUniformMatrix4fv(object.shader.baseUniform.M_Matrix_pointer, 1, GL_FALSE, value_ptr(object.M));
		// Material loading
		glUniform3fv(object.shader.lightUniform.material_ambient, 1, glm::value_ptr(object.material.ambient));
		glUniform3fv(object.shader.lightUniform.material_diffuse, 1, glm::value_ptr(object.material.diffuse));
		glUniform3fv(object.shader.lightUniform.material_specular, 1, glm::value_ptr(object.material.specular));
		glUniform1f(object.shader.lightUniform.material_shininess, object.material.shininess);
		// Update light properties
		glUniform3f(object.shader.lightUniform.light_position_pointer, light.position.x, light.position.y, light.position.z);
		glUniform1f(object.shader.lightUniform.light_power_pointer, light.power);
		break;

	case WAVE:
		// Caricamento matrice trasformazione del modello
		glUniformMatrix4fv(object.shader.baseUniform.M_Matrix_pointer, 1, GL_FALSE, value_ptr(object.M));
		// Time setting: clock() returns the amount of milliseconds from the application start
		glUniform1f(object.shader.baseUniform.time_delta_pointer, clock());
		break;

	case WAVE_COLOR:
		// Caricamento matrice trasformazione del modello
		glUniformMatrix4fv(object.shader.baseUniform.M_Matrix_pointer, 1, GL_FALSE, value_ptr(object.M));
		// Time setting: clock() returns the amount of milliseconds from the application start
		glUniform1f(object.shader.baseUniform.time_delta_pointer, clock());
		break;

	case WAVE_LIGHT:
		// Caricamento matrice trasformazione del modello
		glUniformMatrix4fv(object.shader.baseUniform.M_Matrix_pointer, 1, GL_FALSE, value_ptr(object.M));
		// Time setting: clock() returns the amount of milliseconds from the application start
		glUniform1f(object.shader.baseUniform.time_delta_pointer, clock());
		// Material loading
		glUniform3fv(object.shader.lightUniform.material_ambient, 1, glm::value_ptr(object.material.ambient));
		glUniform3fv(object.shader.lightUniform.material_diffuse, 1, glm::value_ptr(object.material.diffuse));
		glUniform3fv(object.shader.lightUniform.material_specular, 1, glm::value_ptr(object.material.specular));
		glUniform1f(object.shader.lightUniform.material_shininess, object.material.shininess);
		// Update light properties
		glUniform3f(object.shader.lightUniform.light_position_pointer, light.position.x, light.position.y, light.position.z);
		glUniform1f(object.shader.lightUniform.light_power_pointer, light.power);
		break;

	case TOON:
		// Caricamento matrice trasformazione del modello
		glUniformMatrix4fv(object.shader.baseUniform.M_Matrix_pointer, 1, GL_FALSE, value_ptr(object.M));
		glUniform3f(object.shader.lightUniform.light_position_pointer, light.position.x, light.position.y, light.position.z);
		glUniform1f(object.shader.lightUniform.light_power_pointer, light.power);
		break;

	case TOON_V2:
		// Caricamento matrice trasformazione del modello
		glUniformMatrix4fv(object.shader.baseUniform.M_Matrix_pointer, 1, GL_FALSE, value_ptr(object.M));
		// Material loading
		glUniform3fv(object.shader.lightUniform.material_ambient, 1, glm::value_ptr(object.material.ambient));
		glUniform3fv(object.shader.lightUniform.material_diffuse, 1, glm::value_ptr(object.material.diffuse));
		glUniform3fv(object.shader.lightUniform.material_specular, 1, glm::value_ptr(object.material.specular));
		glUniform1f(object.shader.lightUniform.material_shininess, object.material.shininess);
		// Update light properties
		glUniform3f(object.shader.lightUniform.light_position_pointer, light.position.x, light.position.y, light.position.z);
		glUniform1f(object.shader.lightUniform.light_power_pointer, light.power);
		break;

	default:
		break;
	}
}