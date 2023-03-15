#include "text.h"

void RenderString(float x, float y, float z, void* font, const char* string, GLfloat r, GLfloat g, GLfloat b)
{
	char* c;
	glColor3f(r, g, b);
	glRasterPos3f(x, y, z);
	glutBitmapString(font, (const unsigned char*)string);
}