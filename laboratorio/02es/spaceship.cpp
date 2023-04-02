#include "spaceship.h"

Entity spaceship;

extern Input input;

bool showLines = false;

enum {
	HULL,
	NOSE,
	FIN,
	PROPULSOR,
	ASTRONAUT,
	PORTHOLE,
};

// BUILD ==================================================
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

static void buildLatFins(Figure* fig, ColorRGBA color)
{
	// Left
	fig->vertices.push_back({ -2.8f, -3.7f, 0.0f });
	fig->vertices.push_back({ -1.8f, -3.7f, 0.0f });
	fig->vertices.push_back({ -1.8f, -2.0f, 0.0f });

	fig->vertices.push_back({ -2.8f, -3.7f, 0.0f });
	fig->vertices.push_back({ -1.8f, -3.7f, 0.0f });
	fig->vertices.push_back({ -2.8f, -5.4f, 0.0f });

	// Right
	fig->vertices.push_back({ 2.8f, -3.7f, 0.0f });
	fig->vertices.push_back({ 1.8f, -3.7f, 0.0f });
	fig->vertices.push_back({ 1.8f, -2.0f, 0.0f });

	fig->vertices.push_back({ 2.8f, -3.7f, 0.0f });
	fig->vertices.push_back({ 1.8f, -3.7f, 0.0f });
	fig->vertices.push_back({ 2.8f, -5.4f, 0.0f });

	for (int i = 0; i < fig->vertices.size(); i++)
	{
		fig->colors.push_back(color);
	}
}

static void buildCentrFin(Figure* fig, ColorRGBA color)
{
	fig->vertices.push_back({ 0.0f, -2.0f, 0.0f });
	fig->vertices.push_back({ 0.0f, -5.4f, 0.0f });

	for (int i = 0; i < fig->vertices.size(); i++)
	{
		fig->colors.push_back(color);
	}
}

static void buildHull(Figure* fig, ColorRGBA color)
{
	float height = 6.2f;
	int numPieces = fig->numTriangles;
	float dimPieces = height / numPieces;
	float freq = PI;

	for (int i = 0; i < numPieces; i++)
	{
		// Extern vertices
		Point3D v0 = {
			-1.5f - 0.5f * cos(i / (float)numPieces * freq),
			-3.7f + (i * dimPieces),
			0.0f
		};
		fig->vertices.push_back(v0);

		Point3D v1 = {
			1.5f + 0.5f * cos(i / (float)numPieces * freq),
			-3.7f + (i * dimPieces),
			0.0f
		};
		fig->vertices.push_back(v1);
	}


	for (int i = 0; i < fig->vertices.size(); i++)
	{
		fig->colors.push_back(color);
	}
}

static void buildPropulsor(Figure* fig, ColorRGBA colorLight, ColorRGBA colorDark)
{
	int i;

	fig->vertices.push_back({ 1.8f, -3.7f, 0.0f });
	fig->vertices.push_back({ -1.8f, -3.7f, 0.0f });
	fig->vertices.push_back({ -1.6f, -4.0f, 0.0f });

	fig->vertices.push_back({ -1.6f, -4.0f, 0.0f });
	fig->vertices.push_back({ 1.6f, -4.0f, 0.0f });
	fig->vertices.push_back({ 1.8f, -3.7f, 0.0f });

	for (i = 0; i <= fig->vertices.size(); i++)
	{
		fig->colors.push_back(colorLight);
	}

	fig->vertices.push_back({ 1.0f, -4.0f, 0.0f });
	fig->vertices.push_back({ -1.0f, -4.0f, 0.0f });
	fig->vertices.push_back({ -1.2f, -4.5f, 0.0f });

	fig->vertices.push_back({ -1.2f, -4.5f, 0.0f });
	fig->vertices.push_back({ 1.2f, -4.5f, 0.0f });
	fig->vertices.push_back({ 1.0f, -4.0f, 0.0f });

	for (; i <= fig->vertices.size(); i++)
	{
		fig->colors.push_back(colorDark);
	}
}

static void buildAstronautCabin(Figure* fig, ColorRGBA color)
{
	buildCircle(fig, { 0.0f, 0.0f, 0.0f }, 0.9f, 1, color, color);
}

static void buildAstronautSuit(Figure* fig)
{
	fig->numTriangles += 15;

	// PI -> half a circle
	float stepA = PI / fig->numTriangles;

	Point3D center = { 0.0f, 0.0f, 0.0f };
	float radius = 0.5f;

	fig->vertices.push_back(center);
	for (int i = 0; i <= fig->numTriangles; i++)
	{
		float t = (float)i * stepA + PI * 2;
		fig->vertices.push_back({ center.x + radius * cos(t), center.y + radius * sin(t), 0.0f });
	}
	fig->vertices.push_back(center);

	fig->vertices.push_back({center.x - radius, center.y, 0.0f});
	fig->vertices.push_back({ center.x + radius, center.y, 0.0f });
	fig->vertices.push_back({ center.x + radius, center.y - radius * 1.6f, 0.0f });

	fig->vertices.push_back({ center.x - radius, center.y, 0.0f });
	fig->vertices.push_back({ center.x - radius, center.y - radius * 1.6f, 0.0f });
	fig->vertices.push_back({ center.x + radius, center.y - radius * 1.6f, 0.0f });

	for (int i = 0; i < fig->vertices.size(); i++)
	{
		fig->colors.push_back({ 1.0f, 1.0f, 1.0f, 1.0f });
	}
}

static void buildAstronautVisor(Figure* fig)
{
	fig->numTriangles += 15;

	// PI -> half a circle
	float stepA = PI / fig->numTriangles;

	float radius = 0.25f;
	Point3D center = { -radius * 1.8, 0.0f, 0.0f };
	
	// Left semicircle
	fig->vertices.push_back(center);
	for (int i = 0; i <= fig->numTriangles; i++)
	{
		float t = (float)i * stepA + PI * 0.5f;
		fig->vertices.push_back({ center.x + radius * cos(t), center.y + radius * sin(t), 0.0f });
	}
	
	// Middle section
	fig->vertices.push_back({ center.x, center.y + radius, 0.0f });
	fig->vertices.push_back({ center.x + radius * 1.5f, center.y + radius, 0.0f });
	fig->vertices.push_back({ center.x + radius * 1.5f, center.y - radius, 0.0f });
	fig->vertices.push_back({ center.x + radius * 1.5f, center.y - radius, 0.0f });
	fig->vertices.push_back({ center.x, center.y - radius, 0.0f });

	center = { -radius * 2 + radius * 1.5f, 0.0f, 0.0f };
	
	// Right semicircle
	fig->vertices.push_back(center);
	fig->vertices.push_back(center);
	for (int i = 0; i <= fig->numTriangles; i++)
	{
		float t = (float)i * stepA + PI * 1.5f;
		fig->vertices.push_back({ center.x + radius * cos(t), center.y + radius * sin(t), 0.0f });
	}

	// Color
	for (int i = 0; i < fig->vertices.size(); i++)
	{
		fig->colors.push_back({ 0.0f, 0.0f, 0.0f, 1.0f });
	}
}

static void buildPortholeBorder(Figure* fig, ColorRGBA color)
{
	// draw external hollow circle (metal part)
	buildHollowCircle(fig, { 0.0f, 0.0f, 0.0f },  1.0f, 0.8f, 1, color, color);
}

static void buildPortholeGlass(Figure* fig, ColorRGBA color)
{
	// draw internal circle (glass part)
	buildCircle(fig, { 0.0f, 0.0f, 0.0f }, 0.8f, 1, color, color);
}

static void buildSpaceship()
{
	// Figure for hull
	Figure figHull = {};
	figHull.id = HULL;
	figHull.numTriangles = 15;
	figHull.drawMode = GL_TRIANGLE_STRIP;
	buildHull(&figHull, { 0.9f, 0.9f, 0.9f, 1.0f });
	createFigureVAO(&figHull);
	spaceship.figures.push_back(figHull);

	// Figure for propulsor
	Figure figPropulsor = {};
	figPropulsor.id = PROPULSOR;
	figPropulsor.drawMode = GL_TRIANGLES;
	buildPropulsor(&figPropulsor, { 0.6f, 0.6f, 0.6f, 1.0f }, { 0.4f, 0.4f, 0.4f, 1.0f }); // grey
	createFigureVAO(&figPropulsor);
	spaceship.figures.push_back(figPropulsor);

	// Figure for nose
	Figure figNose = {};
	figNose.id = NOSE;
	figNose.numTriangles = 1;
	figNose.drawMode = GL_TRIANGLES;
	buildNose(&figNose, { 1.0f, 0.0f, 0.0f, 1.0f });
	createFigureVAO(&figNose);
	spaceship.figures.push_back(figNose);

	// Figure for lateral fins
	Figure figLatFins = {};
	figLatFins.id = FIN;
	figLatFins.numTriangles = 4;
	figLatFins.drawMode = GL_TRIANGLES;
	buildLatFins(&figLatFins, { 1.0f, 0.0f, 0.0f, 1.0f });
	createFigureVAO(&figLatFins);
	spaceship.figures.push_back(figLatFins);

	// Figure for central fin
	Figure figCentrFin = {};
	figCentrFin.id = FIN;
	figCentrFin.drawMode = GL_LINES;
	figCentrFin.widthLines = 5.0f;
	buildCentrFin(&figCentrFin, { 1.0f, 0.0f, 0.0f, 1.0f });
	createFigureVAO(&figCentrFin);
	spaceship.figures.push_back(figCentrFin);

	// Figure for astronaut cabin
	Figure figAstronautCabin = {};
	figAstronautCabin.id = HULL;
	figAstronautCabin.numTriangles = 30;
	figAstronautCabin.drawMode = GL_TRIANGLES;
	buildAstronautCabin(&figAstronautCabin, { 0.3f, 0.3f, 0.3f, 1.0f });
	createFigureVAO(&figAstronautCabin);
	spaceship.figures.push_back(figAstronautCabin);

	// Figure for astronaut suit
	Figure figAstronautSuit = {};
	figAstronautSuit.id = ASTRONAUT;
	figAstronautSuit.drawMode = GL_TRIANGLE_FAN;
	figAstronautSuit.modelMatrix = glm::mat4(1.0f);
	figAstronautSuit.modelMatrix = rotate(figAstronautSuit.modelMatrix, (float)PI / 2, glm::vec3(0.0f, 0.0f, 1.0f));
	buildAstronautSuit(&figAstronautSuit);
	createFigureVAO(&figAstronautSuit);
	spaceship.figures.push_back(figAstronautSuit);

	// Figure for astronaut visor
	Figure figAstronautVisor = {};
	figAstronautVisor.id = ASTRONAUT;
	figAstronautVisor.drawMode = GL_TRIANGLE_FAN;
	figAstronautVisor.modelMatrix = glm::mat4(1.0f);
	figAstronautVisor.modelMatrix = rotate(figAstronautVisor.modelMatrix, (float)PI / 2, glm::vec3(0.0f, 0.0f, 1.0f));
	buildAstronautVisor(&figAstronautVisor);
	createFigureVAO(&figAstronautVisor);
	spaceship.figures.push_back(figAstronautVisor);

	// Figure for porthole border
	Figure figPortholeBorder = {};
	figPortholeBorder.id = PORTHOLE;
	figPortholeBorder.numTriangles = 30;
	figPortholeBorder.drawMode = GL_TRIANGLE_STRIP;
	buildPortholeBorder(&figPortholeBorder, { 0.6f, 0.6f, 0.6f, 1.0f }); // grey
	createFigureVAO(&figPortholeBorder);
	spaceship.figures.push_back(figPortholeBorder);

	// Figure for porthole glass
	Figure figPortholeGlass = {};
	figPortholeGlass.id = PORTHOLE;
	figPortholeGlass.numTriangles = 30;
	figPortholeGlass.drawMode = GL_TRIANGLES;
	buildPortholeGlass(&figPortholeGlass, { 0.0f, 0.8f, 1.0f, 0.3f }); // light blue (transparent)
	createFigureVAO(&figPortholeGlass);
	spaceship.figures.push_back(figPortholeGlass);

	for (int i = 0; i < spaceship.figures.size(); i++)
	{
		glm::mat4 mat = glm::mat4(1.0f);
		if(spaceship.figures.at(i).modelMatrix != mat)
			std::cout << "OK" << i << std::endl;
	}
}

void spawnSpaceship()
{
	glm::mat4 spaceshipMatrix = glm::mat4(1.0);
	spaceshipMatrix = translate(spaceshipMatrix, glm::vec3(WINDOW_WIDTH / 2, WINDOW_HEIGHT / 2, 0.0f));
	spaceshipMatrix = scale(spaceshipMatrix, glm::vec3(SPACESHIP_SCALE, SPACESHIP_SCALE, 0.0f));

	spaceship.pos.x = WINDOW_WIDTH / 2;
	spaceship.pos.y = WINDOW_HEIGHT / 2;
	spaceship.heading = PI / 2;
	spaceship.scale = SPACESHIP_SCALE;
	spaceship.modelMatrix = spaceshipMatrix;
	Point3D p1 = { 0.0f, 0.0f, 0.0f }, p2 = { 0.0f, 3.7f };
	spaceship.radius = distance(p1, p2) * spaceship.scale;
	spaceship.forwardSpeed = 0.0f;
	spaceship.angularSpeed = 0.0f;
	spaceship.health = 3;

	buildSpaceship();
}

void destroySpaceship()
{

}
// INPUT ==================================================
void inputSpaceship()
{
	if (!spaceship.health)
		return;

	spaceship.scale = SPACESHIP_SCALE;

	// Deceleration
	spaceship.forwardSpeed *= SPACESHIP_FORWARD_DECELERATION;
	spaceship.angularSpeed *= SPACESHIP_ANGULAR_DECELERATION;

	float forwardNew = 0.0f;
	float angularNew = 0.0f;

	if (input.keyboard.keys['w'])
	{
		forwardNew += SPACESHIP_MAX_FORWARD_SPEED;
	}
	if (input.keyboard.keys['a'])
	{
		angularNew += SPACESHIP_MAX_ANGULAR_SPEED;
	}
	if (input.keyboard.keys['d'])
	{
		angularNew -= SPACESHIP_MAX_ANGULAR_SPEED;
	}
	if (input.keyboard.keys['r'])
	{
		spaceship.forwardSpeed = 0.0f;
		spaceship.angularSpeed = 0.0f;
		spaceship.pos.x = WINDOW_WIDTH / 2;
		spaceship.pos.y = WINDOW_HEIGHT / 2;
		spaceship.heading = PI / 2;
	}
	if (input.keyboard.keys['b'])
	{
		spaceship.scale = SPACESHIP_SCALE * 2;
	}

	// Do some fancy stuff over the speed to make the movement smooth
	spaceship.forwardSpeed = MIN(SPACESHIP_MAX_FORWARD_SPEED, 
		MAX(-SPACESHIP_MAX_FORWARD_SPEED, spaceship.forwardSpeed + forwardNew));
	spaceship.angularSpeed = forwardNew ? MIN(SPACESHIP_MAX_ANGULAR_SPEED, MAX(-SPACESHIP_MAX_ANGULAR_SPEED, spaceship.angularSpeed + angularNew))
		: MIN(SPACESHIP_MIN_ANGULAR_SPEED, MAX(-SPACESHIP_MIN_ANGULAR_SPEED, spaceship.angularSpeed + angularNew));

	if (spaceship.forwardSpeed > 10.0f)
	{
		float xSpawnCenter, ySpawnCenter;
		xSpawnCenter = spaceship.pos.x + (spaceship.radius + 25.0f) * cos(spaceship.heading + PI);
		ySpawnCenter = spaceship.pos.y + (spaceship.radius + 25.0f) * sin(spaceship.heading + PI);

		spawnFiretrailParticles(
			{ xSpawnCenter, ySpawnCenter, 0.0f },
			spaceship.forwardSpeed,
			spaceship.heading - PI,
			20.0f * spaceship.forwardSpeed / SPACESHIP_MAX_FORWARD_SPEED,
			5.0f
		);
	}
	
	// TEST: Show lines
	showLines = input.keyboard.keys['l'];
}

// UPDATE =================================================
void updateSpaceship(float deltaTime)
{
	if (!spaceship.health)
		return;

	if (spaceship.angularSpeed != 0.0f)
	{
		spaceship.heading += spaceship.angularSpeed * deltaTime;
		if (spaceship.heading > 2 * PI)
			spaceship.heading -= 2 * PI;
		if (spaceship.heading < 0.0f)
			spaceship.heading += 2 * PI;
	}

	if (spaceship.forwardSpeed != 0.0f)
	{
		spaceship.pos.x += cos(spaceship.heading) * spaceship.forwardSpeed * deltaTime;
		spaceship.pos.y += sin(spaceship.heading) * spaceship.forwardSpeed * deltaTime;

		// Wrap x
		if (spaceship.pos.x < 0.0f - spaceship.radius)
			spaceship.pos.x += WINDOW_WIDTH + spaceship.radius * 2;
		if (spaceship.pos.x > WINDOW_WIDTH + spaceship.radius)
			spaceship.pos.x -= WINDOW_WIDTH + spaceship.radius * 2;

		// Wrap y
		if (spaceship.pos.y < 0.0f - spaceship.radius)
			spaceship.pos.y += WINDOW_HEIGHT + spaceship.radius * 2;
		if (spaceship.pos.y > WINDOW_HEIGHT + spaceship.radius)
			spaceship.pos.y -= WINDOW_HEIGHT + spaceship.radius * 2;

	}
}

// DRAW ===================================================
void drawSpaceship()
{
	if (!spaceship.health)
		return;

	spaceship.modelMatrix = glm::mat4(1.0);
	spaceship.modelMatrix = translate(spaceship.modelMatrix, glm::vec3(spaceship.pos.x, spaceship.pos.y, 0.0f));
	spaceship.modelMatrix = scale(spaceship.modelMatrix, glm::vec3(spaceship.scale, spaceship.scale, 0.0f));
	spaceship.modelMatrix = rotate(spaceship.modelMatrix, spaceship.heading - ((float)PI / 2), glm::vec3(0.0f, 0.0f, 1.0f));
	
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	for (int i = 0; i < spaceship.figures.size(); i++)
	{
		Figure* fig = &spaceship.figures.at(i);
		
		if (fig->id == ASTRONAUT)
		{
			glm::mat4 tmpMat = spaceship.modelMatrix;
			tmpMat = rotate(tmpMat, -spaceship.heading + ((float)PI / 2), glm::vec3(0.0f, 0.0f, 1.0f));
			glUniformMatrix4fv(MatModel, 1, GL_FALSE, value_ptr(tmpMat));
		}
		else glUniformMatrix4fv(MatModel, 1, GL_FALSE, value_ptr(spaceship.modelMatrix));

		glBindVertexArray(fig->VAO);
		if (fig->sizePoints > 0.0f)
			glPointSize(fig->sizePoints);
		else glPointSize(1.0f);
		if (fig->widthLines > 0.0f)
			glLineWidth(fig->widthLines);
		else glLineWidth(1.0f);

		// TEST: show lines
		int mode = showLines ? GL_LINE_STRIP : fig->drawMode;

		glDrawArrays(mode, 0, fig->vertices.size());
		glBindVertexArray(0);
	}
	glDisable(GL_BLEND);
}