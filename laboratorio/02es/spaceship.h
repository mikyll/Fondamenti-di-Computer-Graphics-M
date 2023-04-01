#pragma once

#include "commons.h"

//typedef struct Entity Spaceship;

void spawnSpaceship();
void destroySpaceship();
void inputSpaceship();
void updateSpaceship(float deltaTime);
void drawSpaceship();

void buildSpaceship();

extern unsigned int MatProj, MatModel;