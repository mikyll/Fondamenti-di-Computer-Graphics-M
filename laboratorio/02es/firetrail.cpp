#include "firetrail.h"

static Firetrail firetrail;
static std::vector<Point3D> vertices;
static std::vector<ColorRGBA> colors;

void initFiretrail()
{
	firetrail = {};

	glGenVertexArrays(1, &firetrail.VAO);
	glBindVertexArray(firetrail.VAO);
	
	// Vertices
	glGenBuffers(1, &firetrail.VBO_Geom);
	glBindBuffer(GL_ARRAY_BUFFER, firetrail.VBO_Geom);
	glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Point3D), vertices.data(), GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);
	glEnableVertexAttribArray(0);

	// Colors
	glGenBuffers(1, &firetrail.VBO_Col);
	glBindBuffer(GL_ARRAY_BUFFER, firetrail.VBO_Col);
	glBufferData(GL_ARRAY_BUFFER, colors.size() * sizeof(ColorRGBA), colors.data(), GL_STATIC_DRAW);
	glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, 0, (void*)0);
	glEnableVertexAttribArray(1);
}

void spawnFiretrailParticles(Point3D pStart, float speed, float heading, int num, float circleRadius)
{
	for (int i = 0; i < num; i++)
	{
		Particle p = {};
		p.pos = pStart;

		// Calculate the speed component (x,y,z) from polar coordinates (speed, heading)
		p.speed = { cos(heading) * speed, sin(heading) * speed, 0.0f };

		// Spawn a particle in a random point inside a circle (the probability decreases moving away from the center)
		p.pos = getRandomPoint2DinsideCircle(pStart.x, pStart.y, MIN(circleRadius, circleRadius * ((i + 1.0f) / num)));
		
		float greenComponent = getRandomFloat(0.0f, 1.0f - ((i + 1.0f) / num / 4));
		p.color = { 1.0f, greenComponent, 0.0f, 1.0f};

		firetrail.particles.push_back(p);
	}
}

// UPDATE =================================================
void updateFiretrail(float deltaTime)
{
	vertices.clear();
	colors.clear();

	for (int i = 0; i < firetrail.particles.size(); i++)
	{
		Particle p = firetrail.particles.at(i);
		
		p.speed.x *= SPEED_DECREASE_FACTOR;
		p.speed.y *= SPEED_DECREASE_FACTOR;

		p.pos.x += firetrail.particles.at(i).speed.x * deltaTime;
		p.pos.y += firetrail.particles.at(i).speed.y * deltaTime;
		
		p.color.a -= ALPHA_DECREASE_FACTOR * deltaTime;

		if (p.color.a <= 0.0f)
		{
			firetrail.particles.erase(firetrail.particles.begin() + i);
		}
		else
		{
			firetrail.particles.at(i) = p;
			vertices.push_back(p.pos);
			colors.push_back(p.color);
		}
	}

	// Vertices
	glBindBuffer(GL_ARRAY_BUFFER, firetrail.VBO_Geom);
	glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Point3D), vertices.data(), GL_STATIC_DRAW);
	
	// Colors
	glBindBuffer(GL_ARRAY_BUFFER, firetrail.VBO_Col);
	glBufferData(GL_ARRAY_BUFFER, colors.size() * sizeof(ColorRGBA), colors.data(), GL_STATIC_DRAW);
}

// DRAW ===================================================
void drawFiretrail()
{
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	glBindVertexArray(firetrail.VAO);

	glPointSize(3.0);
	glDrawArrays(GL_POINTS, 0, vertices.size());
	glBindVertexArray(0);

	glDisable(GL_BLEND);
}