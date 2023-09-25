#pragma once

#include "commons.h"

Object createObject(std::string name, Mesh mesh, Material material, Shader shader, glm::vec3 position, glm::vec3 rotation, glm::vec3 scale);