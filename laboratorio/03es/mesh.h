#pragma once

#include "commons.h"


int loadMesh(std::string filename, bool verticesNormals, Mesh* mesh);
//void initBrokenMesh(std::string filename, std::string name, bool vertices_normals, glm::vec3 position, glm::vec3 rotation, glm::vec3 scale, Material material, ShadingType shading);