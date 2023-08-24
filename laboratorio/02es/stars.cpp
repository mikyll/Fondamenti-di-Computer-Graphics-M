#include "stars.h"

Figure stars;

void initStars()
{
	for (int i = 0; i < NUM_TOT_STARS; i++)
	{
		stars.vertices.push_back(getRandomPoint2DinsideRectangle(0.0f, 0.0f, WINDOW_WIDTH, WINDOW_HEIGHT));
		stars.colors.push_back({ 1.0f, 1.0f, 1.0f, 1.0f});
	}
	createFigureVAO(&stars);
}

// UPDATE =================================================
void updateStars(float deltaTime)
{
	if (spaceship.respawning || stars.vertices.size() == 0)
		return;

	int i;

	float xMovement = cos(spaceship.heading) * spaceship.forwardSpeed * deltaTime;
	float yMovement = sin(spaceship.heading) * spaceship.forwardSpeed * deltaTime;

	for (i = 0; i < STAR_OFFSET_MIDDLE; i++)
	{
		stars.vertices.at(i).x -= xMovement / 1.5;
		stars.vertices.at(i).y -= yMovement / 1.5;
	}
	for (; i < STAR_OFFSET_FAR; i++)
	{
		stars.vertices.at(i).x -= xMovement / 3;
		stars.vertices.at(i).y -= yMovement / 3;
	}
	for (; i < NUM_TOT_STARS; i++)
	{
		stars.vertices.at(i).x -= xMovement / 5;
		stars.vertices.at(i).y -= yMovement / 5;
	}

	for (i = 0; i < NUM_TOT_STARS; i++)
	{
		// Wrap x
		if (stars.vertices.at(i).x < 0.0f)
			stars.vertices.at(i).x += WINDOW_WIDTH;
		if (stars.vertices.at(i).x > WINDOW_WIDTH)
			stars.vertices.at(i).x -= WINDOW_WIDTH;

		// Wrap y
		if (stars.vertices.at(i).y < 0.0f)
			stars.vertices.at(i).y += WINDOW_HEIGHT;
		if (stars.vertices.at(i).y > WINDOW_HEIGHT)
			stars.vertices.at(i).y -= WINDOW_HEIGHT;
	}

	updateFigureVertices(&stars);
}

// DRAW ===================================================
void drawStars()
{
	if (stars.vertices.size() == 0)
		return;

	glm::mat4 mat = glm::mat4(1.0);

	glUniformMatrix4fv(MatModel, 1, GL_FALSE, value_ptr(mat));
	glBindVertexArray(stars.VAO);
	
	glPointSize(4.0f);
	glDrawArrays(GL_POINTS, STAR_OFFSET_NEAR, STAR_OFFSET_MIDDLE);
	glPointSize(2.5f);
	glDrawArrays(GL_POINTS, STAR_OFFSET_MIDDLE, STAR_OFFSET_FAR);
	glPointSize(1.0f);
	glDrawArrays(GL_POINTS, STAR_OFFSET_FAR, NUM_TOT_STARS);
	glBindVertexArray(0);
}