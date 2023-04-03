#pragma once

#include "commons.h"

typedef struct {
	float x, y, z;
} Point3D;

typedef struct {
	float r, g, b, a;
} ColorRGBA;

typedef struct {
	float x, y, z;
	float r, g, b, a;
} ColoredPoint;

typedef struct {
	int id;
	GLuint VAO;
	GLuint VBO_Geom;	// VBO vertices geometry
	GLuint VBO_Col;		// VBO vertices colors
	// Vertices
	std::vector<Point3D> vertices;
	std::vector<ColorRGBA> colors;

	// Modellation Matrix: Translation * Rotation * Scaling
	glm::mat4 modelMatrix;
	int drawMode;
	float sizePoints;
	float widthLines;
} Figure;

typedef struct {
	std::vector<Figure> figures;
	glm::mat4 modelMatrix;
	Point3D pos;
	float heading;
	float radius;
	float scale;
	float forwardSpeed;
	float angularSpeed;
	int health;
} Entity;

typedef struct {
	Point3D pos;
	ColorRGBA color;
	Point3D speed;
	float size;
} Particle;

typedef struct {
	GLuint VAO;
	GLuint VBO_Geom; // VBO della geometria dei vertici
	GLuint VBO_Col; // VBO dei colori
	std::vector<Particle> particles;
} Firetrail;

typedef struct {
	Figure figure;
	Point3D pos;
	Point3D speed;
	float heading;
	float radius;
	int type;
	float scale;
} Asteroid;