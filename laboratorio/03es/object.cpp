#include "object.h"

Object createObject(
	std::string name,
	Mesh mesh, 
	Material material, 
	Shader shader,
	glm::vec3 position,
	glm::vec3 rotation,
	glm::vec3 scale)
{
	Object obj = {};
	obj.name = name;
	obj.mesh = mesh;
	obj.material = material;
	obj.shader = shader;
	obj.M = glm::mat4(1);

	// Transformations
	obj.M = glm::translate(obj.M, position);
	if (rotation != glm::vec3(0.0f, 0.0f, 0.0f))
	{
		obj.M = glm::rotate(obj.M, glm::radians(rotation.x), glm::vec3(1.0f, 0.0f, 0.0f));
		obj.M = glm::rotate(obj.M, glm::radians(rotation.y), glm::vec3(0.0f, 1.0f, 0.0f));
		obj.M = glm::rotate(obj.M, glm::radians(rotation.z), glm::vec3(0.0f, 0.0f, 1.0f));
	}
	obj.M = glm::scale(obj.M, scale);

	return obj;
}