#pragma once

#include "commons.h"

glm::vec3 getPosition(glm::mat4 modelMatrix);
glm::vec3 getRotation(glm::mat4 modelMatrix, bool degrees);
float getScalingFactor(glm::mat4 modelMatrix);

std::string getCoordinateSystemName(ReferenceSystem coordinateSystem);
std::string getOperationModeName(OperationMode operationMode);
std::string getWorkingAxisName(WorkingAxis workingAxis);
std::string getShaderName(ShadingType shadingType);

Object* getObject(std::string name);

extern std::vector<Object> objects;