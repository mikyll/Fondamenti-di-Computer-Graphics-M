#include "stars.h"

extern Entity spaceship;

const int totNumStars = 300;

const int numStarsBig = 50;			// move fast
const int numStarsMedium = 100;		// move slower than big ones
const int numStarsSmall = 150;		// move slower than medium ones

Figure stars;
//std::vector<Star> positions;

void initStars()
{
	float xMax = 65.0f;
	float yMax = 35.0f;

	for (int i = 0; i < totNumStars; i++)
	{
		float xPos = (float)rand() / (float)(RAND_MAX / xMax);
		float yPos = (float)rand() / (float)(RAND_MAX / yMax);

		stars.vertices.push_back({ xPos, yPos, 0.0f });
		stars.colors.push_back({ 1.0f, 1.0f, 1.0f, 1.0f});
	}
	createVAOvector(&stars);
}

void updateStars(float deltaTime)
{
	int i;
	for (i = 0; i < numStarsBig; i++)
	{
		//stars.vertices.at(i).x += 100.0f * deltaTime;
		//stars.vertices.at(i).y -= sin(spaceship.heading) * spaceship.forwardSpeed * deltaTime;
	}
	for (; i < numStarsMedium; i++)
	{
		//stars.vertices.at(i).x += 100.0f * deltaTime;
		//stars.vertices.at(i).y -= sin(spaceship.heading) * spaceship.forwardSpeed * deltaTime;
	}
	for (; i < numStarsSmall; i++)
	{
		//stars.vertices.at(i).x += 100.0f * deltaTime;
		//stars.vertices.at(i).y -= sin(spaceship.heading) * spaceship.forwardSpeed * deltaTime;
	}
}

void drawStars()
{
	glm::mat4 mat = glm::mat4(1.0);
	mat = scale(mat, glm::vec3(20.0f, 20.0f, 0.0f));
	//mat = translate(mat, glm::vec3(spaceship.pos.x, spaceship.pos.y, 0.0f));

	glUniformMatrix4fv(MatModel, 1, GL_FALSE, value_ptr(mat));
	glBindVertexArray(stars.VAO);
	
	glPointSize(5.0f);
	glDrawArrays(GL_POINTS, 0, numStarsBig);
	glPointSize(2.5f);
	glDrawArrays(GL_POINTS, numStarsBig, numStarsMedium);
	glPointSize(1.0f);
	glDrawArrays(GL_POINTS, numStarsMedium, numStarsSmall);
	glBindVertexArray(0);
}