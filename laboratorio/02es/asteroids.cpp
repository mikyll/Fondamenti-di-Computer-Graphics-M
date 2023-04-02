#include "asteroids.h"

std::vector<Figure> asteroids;



void buildAsteroid(Figure* fig)
{
	fig->vertices.push_back({ -2.8f, -3.7f, 0.0f });
	fig->vertices.push_back({ -1.8f, -3.7f, 0.0f });
	fig->vertices.push_back({ -1.8f, -2.0f, 0.0f });
	for (int i = 0; i < fig->vertices.size(); i++)
	{
		fig->colors.push_back({1.0f, 0.0f, 0.0f, 1.0f});
	}

	/*fig->vertices.push_back({0.0f, 0.0f, 0.0f});
	fig->colors.push_back({ 0.57f, 0.59f, 0.71f, 1.0f });

	fig->vertices.push_back({ -1.5f, 1.0f, 0.0f });
	fig->colors.push_back({ 0.19f, 0.15f, 0.3f, 1.0f });

	fig->vertices.push_back({ 0.0f, 1.5f, 0.0f });
	fig->colors.push_back({ 0.78f, 0.83f, 0.95f, 1.0f });
	
	fig->vertices.push_back({2.5f, 1.5f, 0.0f});
	fig->colors.push_back({ 0.78f, 0.83f, 0.95f, 1.0f });

	fig->vertices.push_back({ 2.0f, 0.0f, 0.0f });
	fig->colors.push_back({ 0.78f, 0.83f, 0.95f, 1.0f });
	
	fig->vertices.push_back({ 2.5f, -2.0f, 0.0f });
	fig->colors.push_back({ 0.55f, 0.56f, 0.95f, 1.0f });
	
	fig->vertices.push_back({ 0.5f, -3.0f, 0.0f });
	fig->colors.push_back({ 0.26f, 0.15f, 0.49f, 1.0f });
	
	fig->vertices.push_back({ -1.5f, -1.5f, 0.0f });
	fig->colors.push_back({ 0.15f, 0.01f, 0.3f, 1.0f });*/
}

static void buildNose(Figure* fig, ColorRGBA color)
{
	fig->vertices.push_back({ -1.0f, 2.08f, 0.0f });
	fig->vertices.push_back({ 1.0f, 2.08f, 0.0f });
	fig->vertices.push_back({ 0.0f, 3.7f, 0.0f });

	for (int i = 0; i < fig->vertices.size(); i++)
	{
		fig->colors.push_back(color);
	}
}

void initAsteroids()
{
	glm::mat4 spaceshipMatrix = glm::mat4(1.0);
	spaceshipMatrix = translate(spaceshipMatrix, glm::vec3(WINDOW_WIDTH / 2, WINDOW_HEIGHT / 2, 0.0f));
	spaceshipMatrix = scale(spaceshipMatrix, glm::vec3(20.0f, 20.0f, 0.0f));

	/*Figure tmpAsteroid = {};
	tmpAsteroid.drawMode = GL_TRIANGLES;
	tmpAsteroid.modelMatrix = asteroidsMatrix;
	buildAsteroid(&tmpAsteroid);
	createFigureVAO(&tmpAsteroid);
	asteroids.push_back(tmpAsteroid);*/

	// Figure for nose
	Figure figNose = {};
	figNose.numTriangles = 1;
	figNose.drawMode = GL_TRIANGLES;
	figNose.modelMatrix = spaceshipMatrix;
	buildNose(&figNose, { 1.0f, 0.0f, 0.0f, 1.0f });
	createFigureVAO(&figNose);
	asteroids.push_back(figNose);
}

void drawAsteroids()
{
	glm::mat4 mat = glm::mat4(1.0);
	mat = scale(mat, glm::vec3(20.0f, 20.0f, 0.0f));
	
	for (int i = 0; i < asteroids.size(); i++)
	{
		Figure* fig = &asteroids.at(i);
		fig->modelMatrix = mat;

		glUniformMatrix4fv(MatModel, 1, GL_FALSE, value_ptr(mat));
		glBindVertexArray(fig->VAO);

		glDrawArrays(fig->drawMode, 0, fig->vertices.size());
		glBindVertexArray(0);
	}
}