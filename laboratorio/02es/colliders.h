#pragma once

#include "commons.h"

void createCircleCollider(CircleCollider* collider, Point3D pos, float radius, ColorRGBA color);
void updateCircleCollider(CircleCollider* collider, Point3D newPos, float newRadius);
void drawCircleCollider(CircleCollider collider, float heading);

bool isColliding(CircleCollider collider, Point3D pos, float radius);
bool isCollidingCircle(CircleCollider collider1, CircleCollider collider2);

extern unsigned int MatProj, MatModel;