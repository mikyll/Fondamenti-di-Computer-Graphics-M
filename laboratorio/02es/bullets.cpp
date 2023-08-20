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

	bool collide = false;
	for (int i = 0; i < weapon.particles.size() && !collide; i++)
	{
		// i-th bullet
		Particle bullet = weapon.particles.at(i);

		bullet.pos.x += bullet.speed.x * deltaTime;
		bullet.pos.y += bullet.speed.y * deltaTime;

		if (bullet.pos.x < 0.0f || bullet.pos.x > WINDOW_WIDTH ||
			bullet.pos.y < 0.0f || bullet.pos.y > WINDOW_HEIGHT)
		{
			weapon.particles.erase(weapon.particles.begin() + i);
			continue;
		}
		else
		{
			weapon.particles.at(i) = bullet;
			vertices.push_back(bullet.pos);
			colors.push_back(bullet.color);
		}
		
		// Bullet i-th is colliding with an asteroid
		for (int j = 0; j < asteroids.size() && !collide; j++)
		{
			Asteroid asteroid = asteroids.at(j);

			if (isColliding(asteroid.collider, bullet.pos, BULLET_RADIUS))
			{
				collide = true;

				// Delete i-th bullet & j-th asteroid
				weapon.particles.erase(weapon.particles.begin() + i);
				destroyAsteroid(j);

				// If the asteroid wasn't of tipe "SMALL" -> spawn 2 smaller asteroids
				if (asteroid.scale / ASTEROID_SCALE_BASE > ASTEROID_SMALL_FACTOR)
				{
					Point3D speed = {
						getRandomFloat(-100.0f, 100.0f),
						getRandomFloat(-100.0f, 100.0f),
						0.0f
					};
					
					// Spawn 2 smaller asteroids
					spawnAsteroid(asteroid.pos, speed, (int)getRandomFloat(1.0f, ASTEROID_MAX_SPEED), (asteroid.scale / ASTEROID_SCALE_BASE) - 0.5f);
					speed.x = -speed.x;
					speed.y = -speed.y;
					spawnAsteroid(asteroid.pos, speed, (int)getRandomFloat(1.0f, ASTEROID_MAX_SPEED), (asteroid.scale / ASTEROID_SCALE_BASE) - 0.5f);
				}

				// Add points to score
				game.stageScore += 4 - (asteroid.scale * 2 / ASTEROID_SCALE_BASE);
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