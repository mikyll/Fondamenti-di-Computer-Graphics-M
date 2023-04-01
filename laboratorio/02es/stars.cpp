#include "stars.h"

const int numStarsBig = 50;			// move fast
const int numStarsMedium = 100;		// move slower than big ones
const int numStarsSmall = 150;		// move slower than medium ones

const int offsetStarsBig = numStarsBig;
const int offsetStarsMedium = offsetStarsBig + numStarsMedium;
const int offsetStarsSmall = offsetStarsMedium + numStarsSmall;

const int totNumStars = numStarsBig + numStarsMedium + numStarsSmall;

Figure stars;

void initStars()
{
	for (int i = 0; i < totNumStars; i++)
	{
		float xPos = (float)rand() / (float)(RAND_MAX / WINDOW_WIDTH);
		float yPos = (float)rand() / (float)(RAND_MAX / WINDOW_HEIGHT);

		stars.vertices.push_back({ xPos, yPos, 0.0f });
		stars.colors.push_back({ 1.0f, 1.0f, 1.0f, 1.0f});
	}
	createFigureVAO(&stars);
}

void updateStars(float deltaTime)
{
	int i;

	float xMovement = cos(spaceship.heading) * spaceship.forwardSpeed * deltaTime;
	float yMovement = sin(spaceship.heading) * spaceship.forwardSpeed * deltaTime;

	for (i = 0; i < offsetStarsBig; i++)
	{
		stars.vertices.at(i).x -= xMovement;
		stars.vertices.at(i).y -= yMovement;
	}
	for (; i < offsetStarsMedium; i++)
	{
		stars.vertices.at(i).x -= xMovement / 2;
		stars.vertices.at(i).y -= yMovement / 2;
	}
	for (; i < offsetStarsSmall; i++)
	{
		stars.vertices.at(i).x -= xMovement / 5;
		stars.vertices.at(i).y -= yMovement / 5;
	}

	for (i = 0; i < totNumStars; i++)
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

void drawStars()
{
	glm::mat4 mat = glm::mat4(1.0);

	glUniformMatrix4fv(MatModel, 1, GL_FALSE, value_ptr(mat));
	glBindVertexArray(stars.VAO);
	
	glPointSize(4.0f);
	glDrawArrays(GL_POINTS, 0, offsetStarsBig - 1);
	glPointSize(2.5f);
	glDrawArrays(GL_POINTS, offsetStarsBig, offsetStarsMedium - 1);
	glPointSize(1.0f);
	glDrawArrays(GL_POINTS, offsetStarsMedium, offsetStarsSmall);
	glBindVertexArray(0);
}