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