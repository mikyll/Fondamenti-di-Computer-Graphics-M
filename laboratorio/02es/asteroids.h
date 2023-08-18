#pragma once

#include "commons.h"

void initAsteroids();
void spawnAsteroid(Point3D pos, Point3D speed, int type, float size);
void destroyAsteroid(int i);
void clearAsteroids();
void updateAsteroids(float deltaTime);
void drawAsteroids();

extern Game game;
extern unsigned int MatProj, MatModel;
extern Spaceship spaceship;