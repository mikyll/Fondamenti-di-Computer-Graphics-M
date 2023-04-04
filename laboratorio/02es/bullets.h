#pragma once

#include "commons.h"

void initBullets();
void fireBullet(float x, float y, float heading);
void updateBullets(float deltaTime);
void drawBullets();

extern void spawnAsteroid(Point3D pos, Point3D speed, int type, float size);

extern std::vector<Asteroid> asteroids;