#pragma once

#include "commons.h"

typedef struct {
	std::vector<Figure> figures;
} Shield;

void initSpaceship();
void spawnSpaceship(int value);
void destroySpaceship();
void inputSpaceship();
void resetSpaceship();
void updateSpaceship(float deltaTime);
void drawSpaceship();

extern Game game;
extern unsigned int MatProj, MatModel;