#pragma once

#include "commons.h"

typedef struct {
	int state;
	float deltaTime;
	unsigned long timeSinceStart; // milliseconds from glutInit() call
	int fps;
	int stageLevel;
	int totalScore;
	int stageScore;
	int lives;
	unsigned int MatProj;
	unsigned int MatModel;
	glm::mat4 Projection; // Projection matrix
	bool showLines;
	bool showColliders;
} Game;

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
	int drawMode;
	float sizePoints;
	float widthLines;
} Figure;

typedef struct {
	Figure figure;
	Point3D pos;
	float radius;
	ColorRGBA color;
} CircleCollider;

typedef struct {
	std::vector<Figure> figures;
	CircleCollider collider;
	Point3D pos;
	float heading;
	float originalRadius;
	float radius;
	float scale;
	float forwardSpeed;
	float angularSpeed;
	bool openPorthole;
	bool respawning;
	bool invulnerable;
	float invulnerabilityTime;
} Spaceship;

typedef struct {
	Point3D pos;
	ColorRGBA color;
	Point3D speed;
	float size;
} Particle;

typedef struct {
	Figure figure;
	CircleCollider collider;
	Point3D pos;
	Point3D speed;
	float heading;
	float radius;
	int type;
	float scale;
} Asteroid;
