#pragma once

#include "commons.h"


int loadMesh(std::string filename, bool verticesNormals, Mesh* mesh);
int loadBrokenMesh(std::string filename, bool verticesNormals, Mesh* mesh);