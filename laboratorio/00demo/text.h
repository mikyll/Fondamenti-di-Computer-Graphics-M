#ifndef TEXT_H_
#define TEXT_H_

#ifdef __cplusplus
extern "C" {
#endif

	#include <GL/freeglut.h>

	void RenderString(float x, float y, void* font, const char* string, GLfloat r, GLfloat g, GLfloat b);

	/*
	* Example:
	* 
	* char* name = "Mario";
	+ char out[32];
	* sprintf_s(out, 32, "Ciao, %s!", name);
	* RenderString(-0.99, -0.99, GLUT_BITMAP_HELVETICA_18, out, 1.0f, 1.0f, 1.0f);
	*/

#ifdef __cplusplus
}
#endif

#endif // !TEXT_H_