#include "light.h"

PointLight light;
Object lightObject;

void initLight()
{
	light = {};
	light.position = { 5.0, 5.0, -5.0 };
	light.color = DEFAULT_LIGHT_COLOR;
	light.power = 1.f;
}

void initLightObject()
{
	Object obj;
	Mesh mesh;

	// Point Light
	if (loadMesh("sphere_n_t_smooth.obj", FACE_NORMALS, &mesh) == 0)
	{
		obj = createObject("light", mesh, materials.at(NONE), shaders.at(GOURAUD), light.position, glm::vec3(), glm::vec3(light.power / LIGHT_SCALE_FACTOR, light.power / LIGHT_SCALE_FACTOR, light.power / LIGHT_SCALE_FACTOR));
		objects.push_back(obj);
		lightObject = obj;
	}
	else exit(EXIT_FAILURE);
}

void updateLight(glm::vec3 position, glm::vec3 color, GLfloat power)
{
	light.position = position;
	if (color.x >= 0.0f && color.y >= 0.0f && color.z >= 0.0f)
		light.color = color;
	if (power >= 0.0f)
		light.power = power;

	initShaders(light);
	initObjects();
}