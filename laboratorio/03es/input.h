#pragma once

#include "commons.h"

void initInput();

extern void initCamera();
extern void modifyModelMatrix(glm::vec3 translation_vector, glm::vec3 rotation_vector, GLfloat angle, GLfloat scale_factor);
extern void initShaders(PointLight light);
extern void initObjects();
extern void updateLight(glm::vec3 position, glm::vec3 color, GLfloat power);

extern int windowWidth, windowHeight;
extern Application app;
extern Camera camera;

extern std::vector<Object> objects;
extern int selectedObj;
extern PointLight light;