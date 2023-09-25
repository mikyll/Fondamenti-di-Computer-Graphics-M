#include "material.h"

std::map<MaterialType, Material> materials;

// Available materials
static const glm::vec3 red_plastic_ambient = { 0.1, 0.0, 0.0 }, red_plastic_diffuse = { 0.6, 0.1, 0.1 }, red_plastic_specular = { 0.7, 0.6, 0.6 }; GLfloat red_plastic_shininess = 32.0f;
static const glm::vec3 brass_ambient = { 0.1, 0.06, 0.015 }, brass_diffuse = { 0.78, 0.57, 0.11 }, brass_specular = { 0.99, 0.91, 0.81 }; GLfloat brass_shininess = 27.8f;
static const glm::vec3 emerald_ambient = { 0.0215, 0.04745, 0.0215 }, emerald_diffuse = { 0.07568, 0.61424, 0.07568 }, emerald_specular = { 0.633, 0.727811, 0.633 }; GLfloat emerald_shininess = 78.8f;
static const glm::vec3 slate_ambient = { 0.02, 0.02, 0.02 }, slate_diffuse = { 0.1, 0.1, 0.1 }, slate_specular{ 0.4, 0.4, 0.4 }; GLfloat slate_shininess = 1.78125f;

// New materials
static const glm::vec3 black_plastic_ambient = { 0, 0, 0 }, black_plastic_diffuse = { 0.01, 0.01, 0.01 }, black_plastic_specular{ 0.5, 0.5, 0.5 }; GLfloat black_plastic_shininess = 32.0f;
static const glm::vec3 gold_ambient = { 0.24725, 0.1995, 0.0745 }, gold_diffuse = { 0.75164, 0.60648, 0.22648 }, gold_specular{ 0.628281, 0.555802, 0.366065 }; GLfloat gold_shininess = 51.2f;
static const glm::vec3 silver_ambient = { 0.19225, 0.19225, 0.19225 }, silver_diffuse = { 0.50754, 0.50754, 0.50754 }, silver_specular{ 0.508273, 0.508273, 0.508273 }; GLfloat silver_shininess = 51.2f;
static const glm::vec3 bronze_ambient = { 0.2125, 0.1275, 0.054 }, bronze_diffuse = { 0.714, 0.4284, 0.18144 }, bronze_specular{ 0.393548, 0.271906, 0.166721 }; GLfloat bronze_shininess = 25.6f;
static const glm::vec3 copper_ambient = { 0.19125, 0.0735, 0.0225 }, copper_diffuse = { 0.7038, 0.27048, 0.0828 }, copper_specular{ 0.256777, 0.137622, 0.086014 }; GLfloat copper_shininess = 12.8f;
static const glm::vec3 turquoise_ambient = { 0.1, 0.18725, 0.1745 }, turquoise_diffuse = { 0.396, 0.74151, 0.69102 }, turquoise_specular{ 0.297254, 0.30829, 0.306678 }; GLfloat turquoise_shininess = 12.8f;


void initMaterials()
{
	Material material;

	// Materials setup
	material = {};
	material.name = "None";
	material.ambient = glm::vec3(1, 1, 1);
	material.diffuse = glm::vec3(0, 0, 0);
	material.specular = glm::vec3(0, 0, 0);
	material.shininess = 1.f;
	materials.insert({ NONE, material });

	material = {};
	material.name = "Red Plastic";
	material.ambient = red_plastic_ambient;
	material.diffuse = red_plastic_diffuse;
	material.specular = red_plastic_specular;
	material.shininess = red_plastic_shininess;
	materials.insert({ RED_PLASTIC, material });

	material = {};
	material.name = "Emerald";
	material.ambient = emerald_ambient;
	material.diffuse = emerald_diffuse;
	material.specular = emerald_specular;
	material.shininess = emerald_shininess;
	materials.insert({ EMERALD, material });

	material = {};
	material.name = "Brass";
	material.ambient = brass_ambient;
	material.diffuse = brass_diffuse;
	material.specular = brass_specular;
	material.shininess = brass_shininess;
	materials.insert({ BRASS, material });

	material = {};
	material.name = "Slate";
	material.ambient = slate_ambient;
	material.diffuse = slate_diffuse;
	material.specular = slate_specular;
	material.shininess = slate_shininess;
	materials.insert({ SLATE, material });

	// New materials
	material = {};
	material.name = "Black Plastic";
	material.ambient = black_plastic_ambient;
	material.diffuse = black_plastic_diffuse;
	material.specular = black_plastic_specular;
	material.shininess = black_plastic_shininess;
	materials.insert({ BLACK_PLASTIC, material });

	material = {};
	material.name = "Gold";
	material.ambient = gold_ambient;
	material.diffuse = gold_diffuse;
	material.specular = gold_specular;
	material.shininess = gold_shininess;
	materials.insert({ GOLD, material });

	material = {};
	material.name = "Silver";
	material.ambient = silver_ambient;
	material.diffuse = silver_diffuse;
	material.specular = silver_specular;
	material.shininess = silver_shininess;
	materials.insert({ SILVER, material });

	material = {};
	material.name = "Bronze";
	material.ambient = bronze_ambient;
	material.diffuse = bronze_diffuse;
	material.specular = bronze_specular;
	material.shininess = bronze_shininess;
	materials.insert({ BRONZE, material });

	material = {};
	material.name = "Copper";
	material.ambient = copper_ambient;
	material.diffuse = copper_diffuse;
	material.specular = copper_specular;
	material.shininess = copper_shininess;
	materials.insert({ COPPER, material });

	material = {};
	material.name = "Turquoise";
	material.ambient = turquoise_ambient;
	material.diffuse = turquoise_diffuse;
	material.specular = turquoise_specular;
	material.shininess = turquoise_shininess;
	materials.insert({ TURQUOISE, material });
}