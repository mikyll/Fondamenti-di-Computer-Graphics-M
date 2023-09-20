#pragma once

#include <string>
#include <vector>

#include <GL/glew.h>
#include <GL/freeglut.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

typedef enum {
	NO_MATERIAL,
	RED_PLASTIC,
	EMERALD,
	BRASS,
	SLATE,
	BLACK_PLASTIC,
	GOLD,
	SILVER,
	BRONZE,
	COPPER,
	TURQUOISE,
	MATERIALS_LENGTH
} MaterialType;

typedef struct {
	std::string name;
	glm::vec3 ambient;
	glm::vec3 diffuse;
	glm::vec3 specular;
	GLfloat shininess;
} Material;

void initMaterials();