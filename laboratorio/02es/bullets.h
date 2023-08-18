#pragma once

#include "commons.h"

void initBullets();
void fireBullet(float x, float y, float heading);
void updateBullets(float deltaTime);
void drawBullets();

extern void spawnAsteroid(Point3D pos, Point3D speed, int type, float size);
extern void destroyAsteroid(int i);

extern Game game;
extern unsigned int MatProj, MatModel;
extern std::vector<Asteroid> asteroids;