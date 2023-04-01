#include "firetrail.h"

Firetrail firetrail;

void initFiretrail()
{
	/*glGenVertexArrays(1, &firetrail.VAO);
	glBindVertexArray(firetrail.VAO);
	glGenBuffers(1, &VBO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);*/
}

void spawnFiretrail(float x, float y, float heading)
{

}

void updateFiretrail(float deltaTime)
{

}

void drawFiretrail()
{
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	glDisable(GL_BLEND);
}