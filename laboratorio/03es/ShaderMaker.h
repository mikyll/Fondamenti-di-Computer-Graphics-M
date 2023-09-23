#pragma once

#include "commons.h"

// Creates a GLSL program object from vertex and fragment shader files
GLuint createProgram(std::string vShaderFile, std::string fShaderFile);
