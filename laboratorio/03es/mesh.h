#pragma once

#include "commons.h"


/*
* verticesNormals specifies if the normals must be set:
*	- on vertices (true), -> smooth surface;
*	- on faces (false), -> checkered surface;
* rotation is a vec3 containing the angle the object must be
*	rotated (in degrees) for each axis (X, Y, Z)
*/
Mesh loadMesh(std::string filename, bool verticesNormals);
//void initBrokenMesh(std::string filename, std::string name, bool vertices_normals, glm::vec3 position, glm::vec3 rotation, glm::vec3 scale, Material material, ShadingType shading);