#include "explosions.h"

static Explosion explosion;

static std::vector<Point3D> vertices;
static std::vector<ColorRGBA> colors;

void initExplosions()
{
	explosion = {};

	glGenVertexArrays(1, &explosion.VAO);
	glBindVertexArray(explosion.VAO);

	// Vertices
	glGenBuffers(1, &explosion.VBO_Geom);
	glBindBuffer(GL_ARRAY_BUFFER, explosion.VBO_Geom);
	glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Point3D), vertices.data(), GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);
	glEnableVertexAttribArray(0);

	// Colors
	glGenBuffers(1, &explosion.VBO_Col);
	glBindBuffer(GL_ARRAY_BUFFER, explosion.VBO_Col);
	glBufferData(GL_ARRAY_BUFFER, colors.size() * sizeof(ColorRGBA), colors.data(), GL_STATIC_DRAW);
	glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, 0, (void*)0);
	glEnableVertexAttribArray(1);
}

void spawnExplosion(int type, Point3D center, float radius, float speed, int num)
{
	for (int i = 0; i < num; i++)
	{
		Particle p = {};
		p.pos = getRandomPoint2DinsideCircle(center.x, center.y, MIN(radius, radius * ((i + 1.0f) / num / 2)));
		p.speed = { getRandomFloat(-speed, speed), getRandomFloat(-speed, speed), 0.0f };

		int randColor = getRandomInt(0, 100);
		switch (type)
		{
			case EXPLOSION_SPACESHIP:
			{
				if (randColor < 5)
					p.color = { 0.1f, 0.1f, 0.1f, 1.0f };
				else if (randColor < 10)
					p.color = { 1.0f, 1.0f, 0.1f, 1.0f };
				else if (randColor < 30)
					p.color = { 1.0f, 0.5f, 0.1f, 1.0f };
				else if (randColor < 70)
					p.color = { 0.9f, 0.9f, 0.9f, 1.0f };
				else if (randColor < 90)
					p.color = { 1.0f, 0.0f, 0.0f, 1.0f };
				else if (randColor < 98)
					p.color = { 0.4f, 0.4f, 0.4f, 1.0f };
				else p.color = { 0.0f, 0.8f, 1.0f, 0.3f };

				break;
			}
			case EXPLOSION_ASTEROID:
			{
				if (randColor < 15)
					p.color = { 0.21f, 0.11f, 0.4f, 1.0f };
				else if (randColor < 30)
					p.color = { 0.26f, 0.15f, 0.49f, 1.0f };
				else if (randColor < 90)
					p.color = { 0.78f, 0.83f, 0.95f, 1.0f };
				else if (randColor < 95)
					p.color = { 0.38f, 0.0f, 0.33f, 1.0f };
				else p.color = { 0.25f, 0.4f, 0.4f, 1.0f };

				break;
			}
			default:
				p.color = { 1.0f, 1.0f, 1.0f, 1.0f };
		}

		explosion.particles.push_back(p);
	}
}

void updateExplosions(float deltaTime)
{
	vertices.clear();
	colors.clear();

	for (int i = 0; i < explosion.particles.size(); i++)
	{
		Particle p = explosion.particles.at(i);

		p.speed.x *= SPEED_DECREASE_FACTOR;
		p.speed.y *= SPEED_DECREASE_FACTOR;

		p.pos.x += explosion.particles.at(i).speed.x * deltaTime;
		p.pos.y += explosion.particles.at(i).speed.y * deltaTime;

		p.color.a -= (ALPHA_DECREASE_FACTOR * deltaTime / 2.0f);

		if (p.color.a <= 0.0f)
		{
			explosion.particles.erase(explosion.particles.begin() + i);
		}
		else
		{
			explosion.particles.at(i) = p;
			vertices.push_back(p.pos);
			colors.push_back(p.color);
		}
		// Vertices
		glBindBuffer(GL_ARRAY_BUFFER, explosion.VBO_Geom);
		glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Point3D), vertices.data(), GL_STATIC_DRAW);

		// Colors
		glBindBuffer(GL_ARRAY_BUFFER, explosion.VBO_Col);
		glBufferData(GL_ARRAY_BUFFER, colors.size() * sizeof(ColorRGBA), colors.data(), GL_STATIC_DRAW);
	}
}

void drawExplosions()
{
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	glBindVertexArray(explosion.VAO);

	glPointSize(5.0f);
	glDrawArrays(GL_POINTS, 0, vertices.size());
	glBindVertexArray(0);

	glDisable(GL_BLEND);
}