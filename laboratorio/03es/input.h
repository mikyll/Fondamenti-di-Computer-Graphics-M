#pragma once

#include "commons.h"

void initInput();

extern void modifyModelMatrix(glm::vec3 translation_vector, glm::vec3 rotation_vector, GLfloat angle, GLfloat scale_factor);

extern int windowWidth;
extern int windowHeight;
extern Camera camera;

extern OperationMode operationMode;
extern WorkingAxis workingAxis;

extern std::vector<Object> objects;
extern int selectedObj;
extern PointLight light;