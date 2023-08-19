#pragma once

#include "commons.h"

typedef struct {
	GLuint VAO;
	GLuint VBO_Geom; // VBO della geometria dei vertici
	GLuint VBO_Col; // VBO dei colori
	std::vector<Particle> particles;
} Firetrail;

void initFiretrail();
void spawnFiretrailParticles(Point3D pStart, float speed, float heading, int num, float circleRadius);

void updateFiretrail(float deltaTime);
void drawFiretrail();