#include "spaceship.h"

Entity spaceship;

extern Input input;

float originalRadius;

bool showLines = false;
bool openPorthole = false;

static Figure collider;

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

static void buildHull(Figure* fig, int numPieces, ColorRGBA color)
{
	float height = 6.2f;
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
	fig->vertices.push_back({ 1.8f, -3.7f, 0.0f });
	fig->vertices.push_back({ -1.8f, -3.7f, 0.0f });
	fig->vertices.push_back({ -1.6f, -4.0f, 0.0f });

	fig->vertices.push_back({ -1.6f, -4.0f, 0.0f });
	fig->vertices.push_back({ 1.6f, -4.0f, 0.0f });
	fig->vertices.push_back({ 1.8f, -3.7f, 0.0f });

	for (int i = 0; i <= fig->vertices.size(); i++)
	{
		fig->colors.push_back(colorLight);
	}

	fig->vertices.push_back({ 1.0f, -4.0f, 0.0f });
	fig->vertices.push_back({ -1.0f, -4.0f, 0.0f });
	fig->vertices.push_back({ -1.2f, -4.5f, 0.0f });

	fig->vertices.push_back({ -1.2f, -4.5f, 0.0f });
	fig->vertices.push_back({ 1.2f, -4.5f, 0.0f });
	fig->vertices.push_back({ 1.0f, -4.0f, 0.0f });

	for (int i = fig->colors.size(); i <= fig->vertices.size(); i++)
	{
		fig->colors.push_back(colorDark);
	}
}

static void buildAstronautCabinIntern(Figure* fig, ColorRGBA color)
{
	buildCircle(fig, { 0.0f, 0.0f, 0.0f }, 0.9f, 30, color, color);
}

static void buildAstronautCabinBorder(Figure* fig, ColorRGBA color)
{
	buildHollowCircle(fig, { 0.0f, 0.0f, 0.0f }, 1.2f, 0.8f, 30, color, color);
}

static void buildAstronautSuit(Figure* fig)
{
	int numTriangles = 15;

	// PI -> half a circle
	float stepA = PI / numTriangles;

	Point3D center = { 0.0f, 0.0f, 0.0f };
	float radius = 0.5f;

	fig->vertices.push_back(center);
	for (int i = 0; i <= numTriangles; i++)
	{
		float t = (float)i * stepA + PI * 2;
		fig->vertices.push_back({ center.x + radius * cos(t), center.y + radius * sin(t), 0.0f });
	}
	fig->vertices.push_back(center);

	fig->vertices.push_back({center.x - radius, center.y, 0.0f});
	fig->vertices.push_back({ center.x + radius, center.y, 0.0f });
	fig->vertices.push_back({ center.x + radius, center.y - radius * 2.0f, 0.0f });

	fig->vertices.push_back({ center.x - radius, center.y, 0.0f });
	fig->vertices.push_back({ center.x - radius, center.y - radius * 2.0f, 0.0f });
	fig->vertices.push_back({ center.x + radius, center.y - radius * 2.0f, 0.0f });

	for (int i = 0; i < fig->vertices.size(); i++)
	{
		fig->colors.push_back({ 1.0f, 1.0f, 1.0f, 1.0f });
	}
}

static void buildAstronautVisor(Figure* fig)
{
	int numTriangles = 15;

	// PI -> half a circle
	float stepA = PI / numTriangles;

	float radius = 0.25f;
	Point3D center = { -radius * 1.8, 0.0f, 0.0f };
	
	// Left semicircle
	fig->vertices.push_back(center);
	for (int i = 0; i <= numTriangles; i++)
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
	for (int i = 0; i <= numTriangles; i++)
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
	buildHollowCircle(fig, { 0.0f, 0.0f, 0.0f },  1.0f, 0.8f, 30, color, color);
}

static void buildPortholeGlass(Figure* fig, ColorRGBA color)
{
	// draw internal circle (glass part)
	buildCircle(fig, { 0.0f, 0.0f, 0.0f }, 0.8f, 30, color, color);
}

static void buildSpaceship()
{
	// Figure for hull
	Figure figHull = {};
	figHull.id = SPACESHIP_HULL;
	figHull.drawMode = GL_TRIANGLE_STRIP;
	buildHull(&figHull, 15, { 0.9f, 0.9f, 0.9f, 1.0f });
	createFigureVAO(&figHull);
	spaceship.figures.push_back(figHull);

	// Figure for propulsor
	Figure figPropulsor = {};
	figPropulsor.id = SPACESHIP_PROPULSOR;
	figPropulsor.drawMode = GL_TRIANGLES;
	buildPropulsor(&figPropulsor, { 0.6f, 0.6f, 0.6f, 1.0f }, { 0.4f, 0.4f, 0.4f, 1.0f }); // grey
	createFigureVAO(&figPropulsor);
	spaceship.figures.push_back(figPropulsor);

	// Figure for nose
	Figure figNose = {};
	figNose.id = SPACESHIP_NOSE;
	figNose.drawMode = GL_TRIANGLES;
	buildNose(&figNose, { 1.0f, 0.0f, 0.0f, 1.0f });
	createFigureVAO(&figNose);
	spaceship.figures.push_back(figNose);

	// Figure for lateral fins
	Figure figLatFins = {};
	figLatFins.id = SPACESHIP_FIN;
	figLatFins.drawMode = GL_TRIANGLES;
	buildLatFins(&figLatFins, { 1.0f, 0.0f, 0.0f, 1.0f });
	createFigureVAO(&figLatFins);
	spaceship.figures.push_back(figLatFins);

	// Figure for central fin
	Figure figCentrFin = {};
	figCentrFin.id = SPACESHIP_FIN;
	figCentrFin.drawMode = GL_LINES;
	figCentrFin.widthLines = 5.0f;
	buildCentrFin(&figCentrFin, { 1.0f, 0.0f, 0.0f, 1.0f });
	createFigureVAO(&figCentrFin);
	spaceship.figures.push_back(figCentrFin);

	// Figure for astronaut cabin intern
	Figure figAstronautCabinIntern = {};
	figAstronautCabinIntern.id = SPACESHIP_HULL;
	figAstronautCabinIntern.drawMode = GL_TRIANGLES;
	buildAstronautCabinIntern(&figAstronautCabinIntern, { 0.3f, 0.3f, 0.3f, 1.0f });
	createFigureVAO(&figAstronautCabinIntern);
	spaceship.figures.push_back(figAstronautCabinIntern);

	// Figure for astronaut suit
	Figure figAstronautSuit = {};
	figAstronautSuit.id = SPACESHIP_ASTRONAUT;
	figAstronautSuit.drawMode = GL_TRIANGLE_FAN;
	figAstronautSuit.modelMatrix = glm::mat4(1.0f);
	figAstronautSuit.modelMatrix = rotate(figAstronautSuit.modelMatrix, (float)PI / 2, glm::vec3(0.0f, 0.0f, 1.0f));
	buildAstronautSuit(&figAstronautSuit);
	createFigureVAO(&figAstronautSuit);
	spaceship.figures.push_back(figAstronautSuit);

	// Figure for astronaut visor
	Figure figAstronautVisor = {};
	figAstronautVisor.id = SPACESHIP_ASTRONAUT;
	figAstronautVisor.drawMode = GL_TRIANGLE_FAN;
	figAstronautVisor.modelMatrix = glm::mat4(1.0f);
	figAstronautVisor.modelMatrix = rotate(figAstronautVisor.modelMatrix, (float)PI / 2, glm::vec3(0.0f, 0.0f, 1.0f));
	buildAstronautVisor(&figAstronautVisor);
	createFigureVAO(&figAstronautVisor);
	spaceship.figures.push_back(figAstronautVisor);

	// Figure for astronaut cabin border
	Figure figAstronautCabinBorder = {};
	figAstronautCabinBorder.id = SPACESHIP_HULL;
	figAstronautCabinBorder.drawMode = GL_TRIANGLE_STRIP;
	buildAstronautCabinBorder(&figAstronautCabinBorder, { 0.9f, 0.9f, 0.9f, 1.0f });
	createFigureVAO(&figAstronautCabinBorder);
	spaceship.figures.push_back(figAstronautCabinBorder);

	// Figure for porthole border
	Figure figPortholeBorder = {};
	figPortholeBorder.id = SPACESHIP_PORTHOLE;
	figPortholeBorder.drawMode = GL_TRIANGLE_STRIP;
	buildPortholeBorder(&figPortholeBorder, { 0.6f, 0.6f, 0.6f, 1.0f }); // grey
	createFigureVAO(&figPortholeBorder);
	spaceship.figures.push_back(figPortholeBorder);

	// Figure for porthole glass
	Figure figPortholeGlass = {};
	figPortholeGlass.id = SPACESHIP_PORTHOLE;
	figPortholeGlass.drawMode = GL_TRIANGLES;
	buildPortholeGlass(&figPortholeGlass, { 0.0f, 0.8f, 1.0f, 0.3f }); // light blue (transparent)
	createFigureVAO(&figPortholeGlass);
	spaceship.figures.push_back(figPortholeGlass);

	// Collider
	collider = {};
	collider.drawMode = GL_LINE_STRIP;
	buildCircumference(&collider, { 0.0f, 0.0f, 0.0f }, originalRadius, 15, { 1.0f, 1.0f, 0.0f, 1.0f });
	createFigureVAO(&collider);
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
	Point3D p1 = { 0.0f, 0.0f, 0.0f }, p2 = { 0.0f, 3.7f, 0.0f };
	spaceship.radius = originalRadius = distance(p1, p2) * spaceship.scale;
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
	if (input.keyboard.keys[' '])
	{
		fireBullet(spaceship.pos.x, spaceship.pos.y, spaceship.heading);
	}
	if (input.keyboard.keys['r'])
	{
		spaceship.forwardSpeed = 0.0f;
		spaceship.angularSpeed = 0.0f;
		spaceship.pos.x = WINDOW_WIDTH / 2;
		spaceship.pos.y = WINDOW_HEIGHT / 2;
		spaceship.heading = PI / 2;
	}

	// Do some fancy stuff over the speed to make the movement smooth
	spaceship.forwardSpeed = MIN(SPACESHIP_MAX_FORWARD_SPEED, 
		MAX(-SPACESHIP_MAX_FORWARD_SPEED, spaceship.forwardSpeed + forwardNew));
	spaceship.angularSpeed = forwardNew ? MIN(SPACESHIP_MAX_ANGULAR_SPEED, MAX(-SPACESHIP_MAX_ANGULAR_SPEED, spaceship.angularSpeed + angularNew))
		: MIN(SPACESHIP_MIN_ANGULAR_SPEED, MAX(-SPACESHIP_MIN_ANGULAR_SPEED, spaceship.angularSpeed + angularNew));
	
	// TEST: Show lines
	if (input.keyboard.keys['l'])
	{
		input.keyboard.keys['l'] = 0;

		showLines = !showLines;
	}
	if (input.keyboard.keys['b'])
	{
		input.keyboard.keys['b'] = 0;

		spaceship.scale = spaceship.scale == SPACESHIP_SCALE ? SPACESHIP_SCALE * 4 : SPACESHIP_SCALE;
		spaceship.radius = spaceship.radius == originalRadius ? originalRadius * 4 : originalRadius;
	}
	if (input.keyboard.keys['o'])
	{
		input.keyboard.keys['o'] = 0;

		openPorthole = !openPorthole;
	}
	if (input.keyboard.keys['p'])
	{
		input.keyboard.keys['p'] = 0;

		running = !running;
	}
	if (input.keyboard.keys['c'])
	{
		input.keyboard.keys['c'] = 0;

		showColliders = !showColliders;
	}
}

// UPDATE =================================================
void updateSpaceship(float deltaTime)
{
	if (!spaceship.health)
		return;

	// Spawn firetrail only if the speed is greater than a threshold
	if (spaceship.forwardSpeed > 10.0f)
	{
		float xSpawnCenter, ySpawnCenter;
		xSpawnCenter = spaceship.pos.x + (spaceship.radius + spaceship.scale * 1.25f) * cos(spaceship.heading + PI);
		ySpawnCenter = spaceship.pos.y + (spaceship.radius + spaceship.scale * 1.25f) * sin(spaceship.heading + PI);

		spawnFiretrailParticles(
			{ xSpawnCenter, ySpawnCenter, 0.0f },
			spaceship.forwardSpeed,
			spaceship.heading - PI,
			20.0f * (spaceship.forwardSpeed / SPACESHIP_MAX_FORWARD_SPEED) * (spaceship.radius / originalRadius),
			5.0f
		);
	}

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
		
		// If the figure represents the astronaut: keep rotation straight up
		if (fig->id == SPACESHIP_ASTRONAUT)
		{
			glm::mat4 tmpMat = spaceship.modelMatrix;
			tmpMat = rotate(tmpMat, -spaceship.heading + ((float)PI / 2), glm::vec3(0.0f, 0.0f, 1.0f));
			glUniformMatrix4fv(MatModel, 1, GL_FALSE, value_ptr(tmpMat));
		}
		// If the figure represents the porthole open/close (rotate on y)
		else if (fig->id == SPACESHIP_PORTHOLE && openPorthole)
		{
			glm::mat4 tmpMat = spaceship.modelMatrix;
			tmpMat = translate(tmpMat, glm::vec3(0.75f, 0.0f, 0.0f));
			tmpMat = rotate(tmpMat, (float)PI / 2.2f, glm::vec3(0.0f, 1.0f, 0.0f));
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

	if (showColliders)
	{
		spaceship.modelMatrix = glm::mat4(1.0);
		spaceship.modelMatrix = translate(spaceship.modelMatrix, glm::vec3(spaceship.pos.x, spaceship.pos.y, 0.0f));
		spaceship.modelMatrix = rotate(spaceship.modelMatrix, spaceship.heading - ((float)PI / 2), glm::vec3(0.0f, 0.0f, 1.0f));
		
		glUniformMatrix4fv(MatModel, 1, GL_FALSE, value_ptr(spaceship.modelMatrix));
		glBindVertexArray(collider.VAO);
		glDrawArrays(collider.drawMode, 0, collider.vertices.size());
		glBindVertexArray(0);
	}
}

/*
// MADNESS INPUT
void inputSpaceship()
{
	if (!spaceship.health)
		return;

	// Deceleration
	spaceship.forwardSpeed *= SPACESHIP_FORWARD_DECELERATION;
	spaceship.angularSpeed *= SPACESHIP_ANGULAR_DECELERATION;

	spaceship.scale = SPACESHIP_SCALE;
	spaceship.radius = originalRadius;

	float forwardNew = 0.0f;
	float angularNew = 0.0f;

	if (input.keyboard.keys['w'])
	{
		spaceship.forwardSpeed += SPACESHIP_MAX_FORWARD_SPEED;
	}
	if (input.keyboard.keys['a'])
	{
		spaceship.angularSpeed += SPACESHIP_MAX_ANGULAR_SPEED;
	}
	if (input.keyboard.keys['d'])
	{
		spaceship.angularSpeed -= SPACESHIP_MAX_ANGULAR_SPEED;
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
		spaceship.scale *= 2;
		spaceship.radius *= 2;
	}

	// Do some fancy stuff over the speed to make the movement smooth
	//spaceship.forwardSpeed = MIN(SPACESHIP_MAX_FORWARD_SPEED,
	//	MAX(-SPACESHIP_MAX_FORWARD_SPEED, spaceship.forwardSpeed + forwardNew));
	//spaceship.angularSpeed = forwardNew ? MIN(SPACESHIP_MAX_ANGULAR_SPEED, MAX(-SPACESHIP_MAX_ANGULAR_SPEED, spaceship.angularSpeed + angularNew))
	//	: MIN(SPACESHIP_MIN_ANGULAR_SPEED, MAX(-SPACESHIP_MIN_ANGULAR_SPEED, spaceship.angularSpeed + angularNew));

	// Spawn firetrail only if the speed is greater than a threshold
	if (spaceship.forwardSpeed > 10.0f)
	{
		float xSpawnCenter, ySpawnCenter;
		xSpawnCenter = spaceship.pos.x + (spaceship.radius + spaceship.scale * 1.25f) * cos(spaceship.heading + PI);
		ySpawnCenter = spaceship.pos.y + (spaceship.radius + spaceship.scale * 1.25f) * sin(spaceship.heading + PI);

		spawnFiretrailParticles(
			{ xSpawnCenter, ySpawnCenter, 0.0f },
			spaceship.forwardSpeed,
			spaceship.heading - PI,
			20.0f * (spaceship.forwardSpeed / SPACESHIP_MAX_FORWARD_SPEED) * (spaceship.radius / originalRadius),
			5.0f * (spaceship.radius / originalRadius)
		);
	}

	// TEST: Show lines
	showLines = input.keyboard.keys['l'];
	openPorthole = input.keyboard.keys['p'];
}
*/