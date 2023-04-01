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