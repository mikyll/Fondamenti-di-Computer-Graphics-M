#pragma once

#include "commons.h"

typedef struct {
	float x, y, z;
} Point3D;

typedef struct {
	float r, g, b, a;
} ColorRGBA;

typedef struct {
	GLuint VAO;
	GLuint VBO_Geom; // VBO della geometria dei vertici
	GLuint VBO_Col; // VBO dei colori
	int numTriangles;
	// Vertices
	std::vector<Point3D> vertices;
	std::vector<ColorRGBA> colors;

	// Modellation Matrix: Translation * Rotation * Scaling
	glm::mat4 modelMatrix;
	int drawMode;
	float sizePoints;
	float widthLines;
} Figure;

void createVAOvector(Figure* fig);
void buildCircle(Figure* fig, float radius, int step, ColorRGBA colorExtern, ColorRGBA colorIntern);
void buildHollowCircle(Figure* fig, float radiusExtern, float radiusIntern, int step, ColorRGBA colorExtern, ColorRGBA colorIntern);