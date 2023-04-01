#pragma once

#include "commons.h"

#define MIN(a,b) (((a)<(b))?(a):(b))
#define MAX(a,b) (((a)>(b))?(a):(b))


glm::vec2 lerp(glm::vec2 a, glm::vec2 b, float t);
float distance(Point3D p1, Point3D p2);

float getRandomFloat(float min, float max);

Point3D getRandomPoint2DinsideCircle(float xCenter, float yCenter, float radius);