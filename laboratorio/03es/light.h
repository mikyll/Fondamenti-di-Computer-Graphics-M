#pragma once

#include "commons.h"

void initLight();
void initLightObject();
void updateLight(glm::vec3 position, glm::vec3 color, GLfloat power);

extern void initShaders(PointLight light);
//extern void updateUniforms(Object object, PointLight light);
extern void modifyModelMatrix(glm::vec3 translation_vector, glm::vec3 rotation_vector, GLfloat angle, GLfloat scale_factor);

extern std::vector<Object> objects;
