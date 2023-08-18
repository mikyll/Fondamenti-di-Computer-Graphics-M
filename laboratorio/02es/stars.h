#pragma once

#include "commons.h"

void initStars();
void updateStars(float deltaTime);
void drawStars();

extern Game game;
extern unsigned int MatProj, MatModel;
extern Spaceship spaceship;