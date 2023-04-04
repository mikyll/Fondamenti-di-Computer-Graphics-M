#include "utils.h"

/*
Return the linear iterpolation of two points a and b.
*/
glm::vec2 lerp(glm::vec2 a, glm::vec2 b, float t)
{
	glm::vec2 res = { 0.0f, 0.0f };
	res.x = (1 - t) * a.x + t * b.x;
	res.y = (1 - t) * a.y + t * b.y;

	return res;
}

float distance(Point3D p1, Point3D p2)
{
	return sqrt(pow(p2.x - p1.x, 2) + pow(p2.y - p1.y, 2) + pow(p2.z - p1.z, 2));
}

/*
Cosin function:

y = Acos(B(x - C)) + D

A: vertical stretch
B: horizontal stretch
C: x position
D: y position
*/
float myCos(float x, float A, float B, float C, float D)
{
	return A * cos(B * (x - C)) + D;
}

double uniform()
{
	return (double)rand() / RAND_MAX;
}

float getRandomFloat(float min, float max)
{
	if (min >= max)
		return 0.0f;

	return min + (float)rand() / (float)(RAND_MAX / (max - min));
}

Point3D getRandomPoint2DinsideRectangle(float xMin, float yMin, float xMax, float yMax)
{
	Point3D res = {
		getRandomFloat(xMin, xMax),
		getRandomFloat(yMin, yMax),
		0.0f,
	};

	return res;
}

Point3D getRandomPoint2DinsideCircle(float xCenter, float yCenter, float radius)
{
	float r = radius * sqrt(uniform());
	float theta = uniform() * PI * 2;

	Point3D res = {
		xCenter + r * radius * cos(theta),
		yCenter + r * radius * sin(theta),
		0.0f,
	};

	return res;
}

bool isColliding(Point3D center1, float radius1, Point3D center2, float radius2)
{
	return distance(center1, center2) < radius1 + radius2;
}