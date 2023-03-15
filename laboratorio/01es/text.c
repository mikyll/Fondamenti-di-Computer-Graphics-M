#include "text.h"

void RenderString(float x, float y, void* font, const char* string, GLfloat r, GLfloat g, GLfloat b)
{
	char* c;
	glColor3f(r, g, b);
	glRasterPos2f(x, y);
	glutBitmapString(font, (const unsigned char*)string);
}