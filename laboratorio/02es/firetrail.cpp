#include "firetrail.h"

Firetrail firetrail;

const float decreaseFactor = 0.95f;

std::vector<Point3D> points;
std::vector<ColorRGBA> colors;

void initFiretrail()
{
	glGenVertexArrays(1, &firetrail.VAO);
	glBindVertexArray(firetrail.VAO);
	
	// Vertices
	glGenBuffers(1, &firetrail.VBO_Geom);
	glBindBuffer(GL_ARRAY_BUFFER, firetrail.VBO_Geom);

	// Colors
	glGenBuffers(1, &firetrail.VBO_Col);
	glBindBuffer(GL_ARRAY_BUFFER, firetrail.VBO_Geom);
}

void spawnFiretrailParticles(Point3D pStart, float speed, float heading, int num, float circleRadius)
{
	for (int i = 0; i < num; i++)
	{
		Particle p = {};
		p.pos = pStart;

		// Calculate the speed component (x,y,z) from polar coordinates (speed, heading)
		p.speed = { cos(heading) * speed, sin(heading) * speed, 0.0f };

		p.pos = getRandomPoint2DinsideCircle(pStart.x, pStart.y, circleRadius);
		float greenComponent = getRandomFloat(0.0f, 1.0f);
		p.color = { 1.0f, (float)i / num, 0.0f, 1.0f};

		firetrail.particles.push_back(p);
	}
}

void updateFiretrail(float deltaTime)
{
	//glBindBuffer(GL_ARRAY_BUFFER, fig->VBO_Geom);
	//glBufferData(GL_ARRAY_BUFFER, fig->vertices.size() * sizeof(Point3D), fig->vertices.data(), GL_STATIC_DRAW);

	points.clear();
	colors.clear();

	for (int i = 0; i < firetrail.particles.size(); i++)
	{
		Particle p = firetrail.particles.at(i);
		
		p.speed.x *= decreaseFactor;
		p.speed.y *= decreaseFactor;

		p.pos.x += firetrail.particles.at(i).speed.x * deltaTime;
		p.pos.y += firetrail.particles.at(i).speed.y * deltaTime;

		p.color.a -= 0.5f * deltaTime;

		if (p.color.a <= 0.0f)
		{
			firetrail.particles.erase(firetrail.particles.begin() + i);
		}
		else
		{
			firetrail.particles.at(i) = p;
			points.push_back(p.pos);
			colors.push_back(p.color);
		}
	}

	// Vertices
	glBindBuffer(GL_ARRAY_BUFFER, firetrail.VBO_Geom);
	glBufferData(GL_ARRAY_BUFFER, points.size() * sizeof(Point3D), points.data(), GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Point3D), (void*)0);
	glEnableVertexAttribArray(0);
	
	// Colors
	glBindBuffer(GL_ARRAY_BUFFER, firetrail.VBO_Col);
	glBufferData(GL_ARRAY_BUFFER, colors.size() * sizeof(ColorRGBA), colors.data(), GL_STATIC_DRAW);
	glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, sizeof(ColorRGBA), (void*)0);
	glEnableVertexAttribArray(1);
}

void drawFiretrail()
{
	glm::mat4 mat = glm::mat4(1.0);

	glUniformMatrix4fv(MatModel, 1, GL_FALSE, value_ptr(mat));
	
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	glBindVertexArray(firetrail.VAO);

	glPointSize(3.0);
	glDrawArrays(GL_POINTS, 0, points.size());
	glBindVertexArray(0);

	glDisable(GL_BLEND);
}