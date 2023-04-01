#pragma once

#include "commons.h"

void initStars();
void updateStars(float deltaTime);
void drawStars();

extern unsigned int MatProj, MatModel;

extern Entity spaceship;