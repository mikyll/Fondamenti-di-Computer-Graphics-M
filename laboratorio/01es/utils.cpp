#include "utils.h"

// UTILS ==================================================
bool equals(Point2D p1, Point2D p2)
{
	return p1.x == p2.x && p1.y == p2.y;
}

void printArray(Point2D* pts, int num)
{
	for (int i = 0; i < num; i++)
	{
		std::cout << "P" << i + 1 << "(" << pts[i].x << "," << pts[i].y << ")" << std::endl;
	}

}

void printArrayFancy(std::string str, Point2D* pts, int num)
{
	std::cout << str << std::endl;
	printArray(pts, num);
	std::cout << "=================" << std::endl;
}

/*
Return the linear iterpolation of two points a and b.
*/
Point2D lerp(Point2D a, Point2D b, float t)
{
	Point2D res = { 0.0f, 0.0f };
	res.x = (1 - t) * a.x + t * b.x;
	res.y = (1 - t) * a.y + t * b.y;

	return res;
}