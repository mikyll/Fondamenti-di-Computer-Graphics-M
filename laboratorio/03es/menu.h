#pragma once

#include "commons.h"

void buildOpenGLMenu();

extern std::string getShaderName(ShadingType shadingType);

extern std::vector<Object> objects;
extern int selectedObj;
extern std::vector<Material> materials;

extern int transformMode;
extern int operationMode;
extern int workingAxis;