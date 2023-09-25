#pragma once

#include "commons.h"

typedef struct {
	std::vector<glm::vec3> vertices;
	std::vector<glm::vec3> normals;
	std::vector<glm::vec2> texCoords;
	GLuint vertexArrayObjID;
	GLuint vertexBufferObjID;
	GLuint normalBufferObjID;
	GLuint uvBufferObjID;
} Mesh;

typedef struct {
	std::string name;
	glm::vec3 ambient;
	glm::vec3 diffuse;
	glm::vec3 specular;
	GLfloat shininess;
} Material;

typedef struct {
	GLuint P_Matrix_pointer;
	GLuint V_Matrix_pointer;
	GLuint M_Matrix_pointer;
	GLfloat time_delta_pointer;
} BaseShaderUniform;

typedef struct {
	GLuint light_position_pointer;
	GLuint light_color_pointer;
	GLuint light_power_pointer;
	GLuint material_diffuse;
	GLuint material_ambient;
	GLuint material_specular;
	GLuint material_shininess;
} LightShaderUniform;

typedef struct {
	GLuint id;
	ShadingType type;
	std::string name;
	BaseShaderUniform baseUniform;
	LightShaderUniform lightUniform;
} Shader;

typedef struct {
	std::string name;
	Mesh mesh;
	Material material;
	Shader shader;
	glm::mat4 M;
} Object;

typedef struct {
	glm::vec3 position;
	glm::vec3 color;
	GLfloat power;
} PointLight;

// Camera "look at": è indirizzata verso un punto specifico.
struct ViewSetup {
	glm::vec4 position;	// Posizione della camera
	glm::vec4 target;	// Punto verso cui la camera è puntata
	glm::vec4 upVector;	// Vettore VUP, "View Up Vector"
};

struct PerspectiveSetup {
	float fovY, aspect, near_plane, far_plane;
};

typedef struct {
	struct ViewSetup viewSetup;
	struct PerspectiveSetup perspectiveSetup;
} Camera;

// todo
/*typedef struct {
	int windowWidth;
	int windowHeight;
	CoordinateSystem coordinateSystem = WCS;
	OperationMode operationMode = MODE_NAVIGATION;
	WorkingAxis workingAxis = AXIS_X;
} Application;*/