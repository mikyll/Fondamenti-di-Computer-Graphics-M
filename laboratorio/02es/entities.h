#pragma once

#include "commons.h"

struct Entity {
	glm::vec2 pos;
	glm::vec2 vel;
	int dir;
	glm::vec4 col;
	void (*tick)(void);
	void (*draw)(void);
};