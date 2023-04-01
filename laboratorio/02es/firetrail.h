#pragma once

#include "commons.h"

void initFiretrail();
void spawnFiretrailParticles(Point3D pStart, float speed, float heading, int num, float circleRadius);

void updateFiretrail(float deltaTime);
void drawFiretrail();