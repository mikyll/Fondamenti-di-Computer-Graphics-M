#include "stars.h"

const int numStarsBig = 50;			// move fast
const int numStarsMedium = 100;		// move slower than big ones
const int numStarsSmall = 150;		// move slower than medium ones

const int offsetStarsBig = 0;
const int offsetStarsMedium = numStarsBig;
const int offsetStarsSmall = numStarsBig + numStarsMedium;

const int totNumStars = numStarsBig + numStarsMedium + numStarsSmall;

Figure stars;

void initStars()
{
	for (int i = 0; i < totNumStars; i++)
	{
		float xPos = getRandomFloat(0.0, WINDOW_WIDTH);
		float yPos = getRandomFloat(0.0, WINDOW_HEIGHT);

		stars.vertices.push_back({ xPos, yPos, 0.0f });
		stars.colors.push_back({ 1.0f, 1.0f, 1.0f, 1.0f});
	}
	createFigureVAO(&stars);
}

// UPDATE =================================================
void updateStars(float deltaTime)
{
	int i;

	float xMovement = cos(spaceship.heading) * spaceship.forwardSpeed * deltaTime;
	float yMovement = sin(spaceship.heading) * spaceship.forwardSpeed * deltaTime;

	for (i = 0; i < offsetStarsMedium; i++)
	{
		stars.vertices.at(i).x -= xMovement / 1.5;
		stars.vertices.at(i).y -= yMovement / 1.5;
	}
	for (; i < offsetStarsSmall; i++)
	{
		stars.vertices.at(i).x -= xMovement / 3;
		stars.vertices.at(i).y -= yMovement / 3;
	}
	for (; i < totNumStars; i++)
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

// DRAW ===================================================
void drawStars()
{
	glm::mat4 mat = glm::mat4(1.0);

	glUniformMatrix4fv(MatModel, 1, GL_FALSE, value_ptr(mat));
	glBindVertexArray(stars.VAO);
	
	glPointSize(4.0f);
	glDrawArrays(GL_POINTS, offsetStarsBig, offsetStarsMedium);
	glPointSize(2.5f);
	glDrawArrays(GL_POINTS, offsetStarsMedium, offsetStarsSmall);
	glPointSize(1.0f);
	glDrawArrays(GL_POINTS, offsetStarsSmall, totNumStars);
	glBindVertexArray(0);
}