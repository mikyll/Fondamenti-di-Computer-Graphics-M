#pragma once

#include "commons.h"

void spawnSpaceship();
void destroySpaceship();
void inputSpaceship();
void resetSpaceship();
void updateSpaceship(float deltaTime);
void drawSpaceship();

extern Game game;
extern unsigned int MatProj, MatModel;