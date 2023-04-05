#include "asteroids.h"

std::vector<Asteroid> asteroids;

bool showColliders = false;

static void buildAsteroid1(Figure* fig)
{
	fig->vertices.push_back({0.0f, 0.0f, 0.0f});
	fig->colors.push_back({ 0.25f, 0.17f, 0.45f, 1.0f });

	fig->vertices.push_back({ -1.5f, 1.0f, 0.0f });
	fig->colors.push_back({ 0.19f, 0.15f, 0.3f, 1.0f });

	fig->vertices.push_back({ 0.0f, 1.5f, 0.0f });
	fig->colors.push_back({ 0.78f, 0.83f, 0.95f, 1.0f });
	
	fig->vertices.push_back({ 1.0f, 1.4f, 0.0f});
	fig->colors.push_back({ 0.78f, 0.83f, 0.95f, 1.0f });

	fig->vertices.push_back({ 1.4f, 0.9f, 0.0f });
	fig->colors.push_back({ 0.65f, 0.76f, 0.9f, 1.0f });

	fig->vertices.push_back({ 1.2f, 0.0f, 0.0f });
	fig->colors.push_back({ 0.4f, 0.2f, 0.7f, 1.0f });
	
	fig->vertices.push_back({ 1.7f, -0.9f, 0.0f });
	fig->colors.push_back({ 0.78f, 0.83f, 0.95f, 1.0f });

	fig->vertices.push_back({ 1.7f, -1.5f, 0.0f });
	fig->colors.push_back({ 0.55f, 0.56f, 0.95f, 1.0f });
	
	fig->vertices.push_back({ 1.3f, -1.6f, 0.0f });
	fig->colors.push_back({ 0.55f, 0.56f, 0.95f, 1.0f });

	fig->vertices.push_back({ 0.5f, -2.0f, 0.0f });
	fig->colors.push_back({ 0.26f, 0.15f, 0.49f, 1.0f });
	
	fig->vertices.push_back({ -0.7f, -1.6f, 0.0f });
	fig->colors.push_back({ 0.15f, 0.01f, 0.3f, 1.0f });

	fig->vertices.push_back({ -1.2f, -1.4f, 0.0f });
	fig->colors.push_back({ 0.15f, 0.01f, 0.3f, 1.0f });

	fig->vertices.push_back({ -1.6f, -0.2f, 0.0f });
	fig->colors.push_back({ 0.25f, 0.4f, 0.4f, 1.0f });

	fig->vertices.push_back({ -1.5f, 1.0f, 0.0f });
	fig->colors.push_back({ 0.19f, 0.15f, 0.3f, 1.0f });
}

static void buildAsteroid2(Figure* fig)
{
	fig->vertices.push_back({ 0.0f, 0.0f, 0.0f });
	fig->colors.push_back({ 0.21f, 0.11f, 0.4f, 1.0f });

	fig->vertices.push_back({ -2.2f, 0.5f, 0.0f });
	fig->colors.push_back({ 0.57f, 0.59f, 0.71f, 1.0f });

	fig->vertices.push_back({ -1.0f, 0.6f, 0.0f });
	fig->colors.push_back({ 0.57f, 0.59f, 0.71f, 1.0f });

	fig->vertices.push_back({ -0.8f, 0.8f, 0.0f });
	fig->colors.push_back({ 0.57f, 0.59f, 0.71f, 1.0f });

	fig->vertices.push_back({ 1.3f, 1.9f, 0.0f });
	fig->colors.push_back({ 0.78f, 0.83f, 0.95f, 1.0f });

	fig->vertices.push_back({ 1.5f, 1.7f, 0.0f });
	fig->colors.push_back({ 0.78f, 0.83f, 0.95f, 1.0f });

	fig->vertices.push_back({ 1.6f, -0.2f, 0.0f });
	fig->colors.push_back({ 0.78f, 0.83f, 0.95f, 1.0f });

	fig->vertices.push_back({ 2.0f, -0.6f, 0.0f });
	fig->colors.push_back({ 0.57f, 0.59f, 0.71f, 1.0f });

	fig->vertices.push_back({ 1.6f, -1.2f, 0.0f });
	fig->colors.push_back({ 0.57f, 0.59f, 0.71f, 1.0f });

	fig->vertices.push_back({ 1.2f, -1.4f, 0.0f });
	fig->colors.push_back({ 0.28f, 0.18f, 0.5f, 1.0f });

	fig->vertices.push_back({ -0.5f, -1.0f, 0.0f });
	fig->colors.push_back({ 0.28f, 0.18f, 0.5f, 1.0f });

	fig->vertices.push_back({ -2.5f, -0.3f, 0.0f });
	fig->colors.push_back({ 0.28f, 0.29f, 0.4f, 1.0f });

	fig->vertices.push_back({ -2.2f, 0.5f, 0.0f });
	fig->colors.push_back({ 0.57f, 0.59f, 0.71f, 1.0f });

	fig->vertices.push_back({ 0.0f, 0.0f, 0.0f });
	fig->colors.push_back({ 0.21f, 0.11f, 0.4f, 1.0f });

	fig->vertices.push_back({ 0.3f, 0.3f, 0.0f });
	fig->colors.push_back({ 0.57f, 0.59f, 0.71f, 1.0f });
}

static void buildAsteroid3(Figure* fig)
{
	fig->vertices.push_back({ 0.0f, 0.0f, 0.0f });
	fig->colors.push_back({ 0.21f, 0.11f, 0.4f, 1.0f });

	fig->vertices.push_back({ -2.0f, 0.2f, 0.0f });
	fig->colors.push_back({ 1.0f, 1.0f, 1.0f, 1.0f });

	fig->vertices.push_back({ -1.8f, 0.7f, 0.0f });
	fig->colors.push_back({ 0.78f, 0.83f, 0.95f, 1.0f });

	fig->vertices.push_back({ -1.0f, 1.2f, 0.0f });
	fig->colors.push_back({ 1.0f, 1.0f, 1.0f, 1.0f });

	fig->vertices.push_back({ -0.2f, 1.4f, 0.0f });
	fig->colors.push_back({ 0.7f, 0.7f, 0.7f, 1.0f });

	fig->vertices.push_back({ 0.4f, 1.1f, 0.0f });
	fig->colors.push_back({ 0.57f, 0.59f, 0.71f, 1.0f });

	fig->vertices.push_back({ 1.8f, 0.3f, 0.0f });
	fig->colors.push_back({ 0.8f, 0.75f, 0.8f, 1.0f });

	fig->vertices.push_back({ 1.7f, -1.2f, 0.0f });
	fig->colors.push_back({ 0.51f, 0.49f, 0.7f, 1.0f });

	fig->vertices.push_back({ 1.4f, -1.4f, 0.0f });
	fig->colors.push_back({ 0.29f, 0.17f, 0.5f, 1.0f });

	fig->vertices.push_back({ 0.8f, -1.9f, 0.0f });
	fig->colors.push_back({ 0.28f, 0.29f, 0.4f, 1.0f });

	fig->vertices.push_back({ 0.3f, -2.0f, 0.0f });
	fig->colors.push_back({ 0.21f, 0.11f, 0.4f, 1.0f });

	fig->vertices.push_back({ -0.4f, -1.4f, 0.0f });
	fig->colors.push_back({ 0.21f, 0.11f, 0.4f, 1.0f });

	fig->vertices.push_back({ -1.6f, -1.3f, 0.0f });
	fig->colors.push_back({ 0.38f, 0.0f, 0.33f, 1.0f });

	fig->vertices.push_back({ -2.0f, 0.2f, 0.0f });
	fig->colors.push_back({ 0.78f, 0.83f, 0.95f, 1.0f });
}

void initAsteroids()
{
	for (int i = 0; i < NUM_ASTEROIDS; i++)
	{
		Point3D speed = {
			getRandomFloat(-100.0f, 100.0f),
			getRandomFloat(-100.0f, 100.0f),
			0.0f
		};
		Point3D pos = {
			getRandomFloat(0.0f, WINDOW_WIDTH),
			getRandomFloat(0.0f, WINDOW_HEIGHT),
			0.0f
		};
		spawnAsteroid(pos, speed, (int)getRandomFloat(1.0f, 3.999999f), ASTEROID_BIG_FACTOR);
	}
}

void spawnAsteroid(Point3D pos, Point3D speed, int type, float size)
{
	Asteroid asteroid = {};
	asteroid.pos = pos;
	asteroid.speed = speed;
	asteroid.heading = getRandomFloat(0.0f, PI * 2);
	asteroid.scale = ASTEROID_SCALE_BASE * size;
	asteroid.radius = ASTEROID_RADIUS_BASE * asteroid.scale;
	asteroid.type = type;
	asteroid.figure.drawMode = GL_TRIANGLE_FAN;

	switch (type)
	{
	case ASTEROID_1:
		
		buildAsteroid1(&asteroid.figure);
		break;

	case ASTEROID_2:

		buildAsteroid2(&asteroid.figure);
		break;

	case ASTEROID_3:

		buildAsteroid3(&asteroid.figure);
		break;

	default:
		std::cout << "Invalid asteroid type (" << type << ")" << std::endl;
		return;
	}

	createFigureVAO(&asteroid.figure);
	createCircleCollider(&asteroid.collider, asteroid.pos, asteroid.radius, COLLIDER_COLOR);
	asteroids.push_back(asteroid);
}

void destroyAsteroid(int i)
{
	asteroids.erase(asteroids.begin() + i);
}

void updateAsteroids(float deltaTime)
{
	if (asteroids.size() == 0)
	{
		gameOver = true;

		std::cout << "CONGRATUATIONS, YOU HAVE DESTROYED ALL THE ASTEROIDS!!!" << std::endl << std::endl;

		return;
	}

	for (int i = 0; i < asteroids.size(); i++)
	{
		Asteroid asteroid = asteroids.at(i);

		// update asteroid position
		asteroid.pos.x += asteroid.speed.x * deltaTime;
		asteroid.pos.y += asteroid.speed.y * deltaTime;

		// Wrap x
		if (asteroid.pos.x < 0.0f - asteroid.radius)
			asteroid.pos.x += WINDOW_WIDTH + asteroid.radius * 2;
		if (asteroid.pos.x > WINDOW_WIDTH + asteroid.radius)
			asteroid.pos.x -= WINDOW_WIDTH + asteroid.radius * 2;

		// Wrap y
		if (asteroid.pos.y < 0.0f - asteroid.radius)
			asteroid.pos.y += WINDOW_HEIGHT + asteroid.radius * 2;
		if (asteroid.pos.y > WINDOW_HEIGHT + asteroid.radius)
			asteroid.pos.y -= WINDOW_HEIGHT + asteroid.radius * 2;
		
		// Update collider
		updateCircleCollider(&asteroid.collider, asteroid.pos, asteroid.radius);

		// Check collision with spaceship
		if (isCollidingCircle(asteroid.collider, spaceship.collider))
		{
			gameOver = true;

			// test
			//std::cout << "COLLISION " << glutGet(GLUT_ELAPSED_TIME) << std::endl;
			std::cout << "SPACESHIP DESTROYED, YOU LOST!" << std::endl << std::endl;
			
			return;
		}

		asteroids.at(i) = asteroid;
	}
}

void drawAsteroids()
{
	for (int i = 0; i < asteroids.size(); i++)
	{
		Asteroid* asteroid = &asteroids.at(i);
		Figure* fig = &asteroid->figure;

		glm::mat4 mat = glm::mat4(1.0);
		mat = translate(mat, glm::vec3(asteroid->pos.x, asteroid->pos.y, 0.0f));
		mat = rotate(mat, asteroid->heading, glm::vec3(0.0f, 0.0f, 1.0f));
		mat = scale(mat, glm::vec3(asteroid->scale, asteroid->scale, 0.0f));

		glUniformMatrix4fv(MatModel, 1, GL_FALSE, value_ptr(mat));
		glBindVertexArray(fig->VAO);

		glDrawArrays(showLines ? GL_LINE_STRIP : fig->drawMode, 0, fig->vertices.size());
		glBindVertexArray(0);

		// Draw collider
		if (showColliders)
		{
			drawCircleCollider(asteroid->collider, asteroid->heading);
		}
	}
}