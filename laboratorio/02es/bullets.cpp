#include "bullets.h"

typedef Firetrail Weapon;

static Weapon weapon;
static std::vector<Point3D> vertices;
static std::vector<ColorRGBA> colors;

float reloadTime = 0.0f;

void initBullets()
{
	weapon = {};

	glGenVertexArrays(1, &weapon.VAO);
	glBindVertexArray(weapon.VAO);

	// Vertices
	glGenBuffers(1, &weapon.VBO_Geom);
	glBindBuffer(GL_ARRAY_BUFFER, weapon.VBO_Geom);
	glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Point3D), vertices.data(), GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);
	glEnableVertexAttribArray(0);

	// Colors
	glGenBuffers(1, &weapon.VBO_Col);
	glBindBuffer(GL_ARRAY_BUFFER, weapon.VBO_Col);
	glBufferData(GL_ARRAY_BUFFER, colors.size() * sizeof(ColorRGBA), colors.data(), GL_STATIC_DRAW);
	glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, 0, (void*)0);
	glEnableVertexAttribArray(1);
}

void fireBullet(float x, float y, float heading)
{
	if (reloadTime == 0.0f)
	{
		Particle p = {};
		p.pos = { x, y, 0.0f };
		p.color = { 1.0f, 0.0f, 0.0f, 1.0f };

		// Calculate the speed component (x,y,z) from polar coordinates (speed, heading)
		p.speed = { cos(heading) * BULLET_MAX_SPEED, sin(heading) * BULLET_MAX_SPEED, 0.0f };
		
		weapon.particles.push_back(p);

		reloadTime = BULLET_RELOAD_COOLDOWN;
	}
}

void updateBullets(float deltaTime)
{
	reloadTime = MAX(0.0f, reloadTime - BULLET_RELOAD_COOLDOWN * deltaTime);

	vertices.clear();
	colors.clear();

	for (int i = 0; i < weapon.particles.size(); i++)
	{
		Particle p = weapon.particles.at(i);

		p.pos.x += p.speed.x * deltaTime;
		p.pos.y += p.speed.y * deltaTime;

		if (p.pos.x < 0.0f || p.pos.x > WINDOW_WIDTH ||
			p.pos.y < 0.0f || p.pos.y > WINDOW_HEIGHT)
		{
			weapon.particles.erase(weapon.particles.begin() + i);
		}
		else
		{
			weapon.particles.at(i) = p;
			vertices.push_back(p.pos);
			colors.push_back(p.color);
		}
		
		std::cout << "Asteroid: (" << asteroids.at(0).pos.x << "," << asteroids.at(0).pos.y << ")" << std::endl;
		std::cout << "Bullet: (" << p.pos.x << "," << p.pos.y << ")" << std::endl;
		std::cout << "Distance: " << distance(p.pos, asteroids.at(0).pos) << std::endl;

		// TO-DO: FIX Check collisions
		for (int j = 0; j < asteroids.size(); j++)
		{
			Asteroid asteroid = asteroids.at(j);

			if (isColliding(p.pos, BULLET_RADIUS, asteroid.pos, asteroid.radius))
			{
				weapon.particles.erase(weapon.particles.begin() + i);
				asteroids.erase(asteroids.begin() + j);

				if (asteroid.scale / ASTEROID_SCALE_BASE > 0.5f)
				{
					Point3D speed = {
						getRandomFloat(-100.0f, 100.0f),
						getRandomFloat(-100.0f, 100.0f),
						0.0f
					};
					
					// spawn 2 smaller asteroids
					//spawnAsteroid(asteroid.pos, speed, (int)getRandomFloat(1.0f, 3.999999f), (asteroid.scale / ASTEROID_SCALE_BASE) - 0.5f);
					//speed.x = -speed.x;
					//speed.y = -speed.y;
					//spawnAsteroid(asteroid.pos, speed, (int)getRandomFloat(1.0f, 3.999999f), (asteroid.scale / ASTEROID_SCALE_BASE) - 0.5f);
				}
			}
		}
	}

	// Vertices
	glBindBuffer(GL_ARRAY_BUFFER, weapon.VBO_Geom);
	glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Point3D), vertices.data(), GL_STATIC_DRAW);

	// Colors
	glBindBuffer(GL_ARRAY_BUFFER, weapon.VBO_Col);
	glBufferData(GL_ARRAY_BUFFER, colors.size() * sizeof(ColorRGBA), colors.data(), GL_STATIC_DRAW);
}

void drawBullets()
{
	glm::mat4 mat = glm::mat4(1.0);

	glUniformMatrix4fv(MatModel, 1, GL_FALSE, value_ptr(mat));

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	glBindVertexArray(weapon.VAO);

	glPointSize(BULLET_RADIUS * 2);
	glDrawArrays(GL_POINTS, 0, vertices.size());
	glBindVertexArray(0);

	glDisable(GL_BLEND);
}