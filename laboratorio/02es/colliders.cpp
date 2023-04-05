#include "colliders.h"

void createCircleCollider(CircleCollider* collider, Point3D pos, float radius, ColorRGBA color)
{
	collider->pos = pos;
	collider->radius = radius;
	collider->figure.drawMode = GL_LINE_STRIP;
	buildCircumference(&collider->figure, { 0.0f, 0.0f, 0.0f }, radius, COLLIDER_NUM_SEGMENTS, color);
	createFigureVAO(&collider->figure);
}

void updateCircleCollider(CircleCollider* collider, Point3D newPos, float newRadius)
{
	collider->pos = newPos;
	collider->radius = newRadius;
}

void drawCircleCollider(CircleCollider collider, float heading)
{
	glm::mat4 modelMatrix = glm::mat4(1.0);
	modelMatrix = translate(modelMatrix, glm::vec3(collider.pos.x, collider.pos.y, 0.0f));
	modelMatrix = rotate(modelMatrix, heading, glm::vec3(0.0f, 0.0f, 1.0f));

	glUniformMatrix4fv(MatModel, 1, GL_FALSE, value_ptr(modelMatrix));
	glBindVertexArray(collider.figure.VAO);
	glDrawArrays(collider.figure.drawMode, 0, collider.figure.vertices.size());
	glBindVertexArray(0);
}

bool isColliding(CircleCollider collider, Point3D pos, float radius)
{
	return distance(collider.pos, pos) < collider.radius + radius;
}

bool isCollidingCircle(CircleCollider collider1, CircleCollider collider2)
{
	return distance(collider1.pos, collider2.pos) < collider1.radius + collider2.radius;
}