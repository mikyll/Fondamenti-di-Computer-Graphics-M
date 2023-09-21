#include "materials.h"

std::vector<Material> materials;

// Available materials
glm::vec3 red_plastic_ambient = { 0.1, 0.0, 0.0 }, red_plastic_diffuse = { 0.6, 0.1, 0.1 }, red_plastic_specular = { 0.7, 0.6, 0.6 }; GLfloat red_plastic_shininess = 32.0f;
glm::vec3 brass_ambient = { 0.1, 0.06, 0.015 }, brass_diffuse = { 0.78, 0.57, 0.11 }, brass_specular = { 0.99, 0.91, 0.81 }; GLfloat brass_shininess = 27.8f;
glm::vec3 emerald_ambient = { 0.0215, 0.04745, 0.0215 }, emerald_diffuse = { 0.07568, 0.61424, 0.07568 }, emerald_specular = { 0.633, 0.727811, 0.633 }; GLfloat emerald_shininess = 78.8f;
glm::vec3 slate_ambient = { 0.02, 0.02, 0.02 }, slate_diffuse = { 0.1, 0.1, 0.1 }, slate_specular{ 0.4, 0.4, 0.4 }; GLfloat slate_shininess = 1.78125f;

// New materials
glm::vec3 black_plastic_ambient = { 0, 0, 0 }, black_plastic_diffuse = { 0.01, 0.01, 0.01 }, black_plastic_specular{ 0.5, 0.5, 0.5 }; GLfloat black_plastic_shininess = 32.0f;
glm::vec3 gold_ambient = { 0.24725, 0.1995, 0.0745 }, gold_diffuse = { 0.75164, 0.60648, 0.22648 }, gold_specular{ 0.628281, 0.555802, 0.366065 }; GLfloat gold_shininess = 51.2f;
glm::vec3 silver_ambient = { 0.19225, 0.19225, 0.19225 }, silver_diffuse = { 0.50754, 0.50754, 0.50754 }, silver_specular{ 0.508273, 0.508273, 0.508273 }; GLfloat silver_shininess = 51.2f;
glm::vec3 bronze_ambient = { 0.2125, 0.1275, 0.054 }, bronze_diffuse = { 0.714, 0.4284, 0.18144 }, bronze_specular{ 0.393548, 0.271906, 0.166721 }; GLfloat bronze_shininess = 25.6f;
glm::vec3 copper_ambient = { 0.19125, 0.0735, 0.0225 }, copper_diffuse = { 0.7038, 0.27048, 0.0828 }, copper_specular{ 0.256777, 0.137622, 0.086014 }; GLfloat copper_shininess = 12.8f;
glm::vec3 turquoise_ambient = { 0.1, 0.18725, 0.1745 }, turquoise_diffuse = { 0.396, 0.74151, 0.69102 }, turquoise_specular{ 0.297254, 0.30829, 0.306678 }; GLfloat turquoise_shininess = 12.8f;


void initMaterials()
{
	// Materials setup
	materials.resize(NUM_MATERIALS);
	materials[MaterialType::NO_MATERIAL].name = "NO_MATERIAL";
	materials[MaterialType::NO_MATERIAL].ambient = glm::vec3(1, 1, 1);
	materials[MaterialType::NO_MATERIAL].diffuse = glm::vec3(0, 0, 0);
	materials[MaterialType::NO_MATERIAL].specular = glm::vec3(0, 0, 0);
	materials[MaterialType::NO_MATERIAL].shininess = 1.f;

	materials[MaterialType::RED_PLASTIC].name = "Red Plastic";
	materials[MaterialType::RED_PLASTIC].ambient = red_plastic_ambient;
	materials[MaterialType::RED_PLASTIC].diffuse = red_plastic_diffuse;
	materials[MaterialType::RED_PLASTIC].specular = red_plastic_specular;
	materials[MaterialType::RED_PLASTIC].shininess = red_plastic_shininess;

	materials[MaterialType::EMERALD].name = "Emerald";
	materials[MaterialType::EMERALD].ambient = emerald_ambient;
	materials[MaterialType::EMERALD].diffuse = emerald_diffuse;
	materials[MaterialType::EMERALD].specular = emerald_specular;
	materials[MaterialType::EMERALD].shininess = emerald_shininess;

	materials[MaterialType::BRASS].name = "Brass";
	materials[MaterialType::BRASS].ambient = brass_ambient;
	materials[MaterialType::BRASS].diffuse = brass_diffuse;
	materials[MaterialType::BRASS].specular = brass_specular;
	materials[MaterialType::BRASS].shininess = brass_shininess;

	materials[MaterialType::SLATE].name = "Slate";
	materials[MaterialType::SLATE].ambient = slate_ambient;
	materials[MaterialType::SLATE].diffuse = slate_diffuse;
	materials[MaterialType::SLATE].specular = slate_specular;
	materials[MaterialType::SLATE].shininess = slate_shininess;

	// New materials
	materials[MaterialType::BLACK_PLASTIC].name = "Black Plastic";
	materials[MaterialType::BLACK_PLASTIC].ambient = black_plastic_ambient;
	materials[MaterialType::BLACK_PLASTIC].diffuse = black_plastic_diffuse;
	materials[MaterialType::BLACK_PLASTIC].specular = black_plastic_specular;
	materials[MaterialType::BLACK_PLASTIC].shininess = black_plastic_shininess;

	materials[MaterialType::GOLD].name = "Gold";
	materials[MaterialType::GOLD].ambient = gold_ambient;
	materials[MaterialType::GOLD].diffuse = gold_diffuse;
	materials[MaterialType::GOLD].specular = gold_specular;
	materials[MaterialType::GOLD].shininess = gold_shininess;

	materials[MaterialType::SILVER].name = "Silver";
	materials[MaterialType::SILVER].ambient = silver_ambient;
	materials[MaterialType::SILVER].diffuse = silver_diffuse;
	materials[MaterialType::SILVER].specular = silver_specular;
	materials[MaterialType::SILVER].shininess = silver_shininess;

	materials[MaterialType::BRONZE].name = "Bronze";
	materials[MaterialType::BRONZE].ambient = bronze_ambient;
	materials[MaterialType::BRONZE].diffuse = bronze_diffuse;
	materials[MaterialType::BRONZE].specular = bronze_specular;
	materials[MaterialType::BRONZE].shininess = bronze_shininess;

	materials[MaterialType::COPPER].name = "Copper";
	materials[MaterialType::COPPER].ambient = copper_ambient;
	materials[MaterialType::COPPER].diffuse = copper_diffuse;
	materials[MaterialType::COPPER].specular = copper_specular;
	materials[MaterialType::COPPER].shininess = copper_shininess;

	materials[MaterialType::TURQUOISE].name = "Turquoise";
	materials[MaterialType::TURQUOISE].ambient = turquoise_ambient;
	materials[MaterialType::TURQUOISE].diffuse = turquoise_diffuse;
	materials[MaterialType::TURQUOISE].specular = turquoise_specular;
	materials[MaterialType::TURQUOISE].shininess = turquoise_shininess;
}