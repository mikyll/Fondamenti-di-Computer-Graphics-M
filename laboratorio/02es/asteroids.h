#pragma once

#include "commons.h"

void initAsteroids();
void spawnAsteroid(Point3D pos, Point3D speed, int type, float size);
void destroyAsteroid(int i);
void updateAsteroids(float deltaTime);
void drawAsteroids();

extern unsigned int MatProj, MatModel;

extern Entity spaceship;

extern bool showLines;
extern bool gameOver;