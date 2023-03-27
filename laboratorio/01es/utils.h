#pragma once

#include <iostream>
#include <glm/glm.hpp>


#define MIN(a,b) (((a)<(b))?(a):(b))
#define MAX(a,b) (((a)>(b))?(a):(b))

// Auxiliary data structure to ease the usage of 2D points
typedef struct {
	float x, y;
} Point2D;

typedef struct {
	Point2D p1, p2;
} Segment;

bool equals(Point2D p1, Point2D p2);
void printArray(Point2D* pts, int num);
void printArrayFancy(std::string str, Point2D* pts, int num);

Point2D lerp(Point2D a, Point2D b, float t);