#pragma once

#include "commons.h"

void buildOpenGLMenu();

extern std::string getShaderName(ShadingType shadingType);

extern std::map<MaterialType, Material> materials;
extern std::map<ShadingType, Shader> shaders;
extern std::vector<Object> objects;
extern int selectedObj;

extern CoordinateSystem coordinateSystem;
extern OperationMode operationMode;
extern WorkingAxis workingAxis;