#pragma once

#include "commons.h"

typedef struct {
	GLuint VAO;
	GLuint VBO_Geom; // VBO della geometria dei vertici
	GLuint VBO_Col; // VBO dei colori
	std::vector<Particle> particles;
} Explosion;

void initExplosions();
void spawnExplosion(int type);

void updateExplosions(float deltaTime);
void drawExplosions();