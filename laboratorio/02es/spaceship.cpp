#include "spaceship.h"

std::vector<Figure> spaceship;

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
	float altezza = 6.2f;
	int numPezzi = fig->numTriangles;
	float dimPezzi = altezza / numPezzi;
	float freq = PI;

	for (int i = 0; i < numPezzi; i++)
	{
		// Extern vertices
		Point3D v0 = {
			-1.5f - 0.5f * cos(i / (float)numPezzi * freq),
			-3.7f + (i * dimPezzi),
			0.0f
		};
		fig->vertices.push_back(v0);

		Point3D v1 = {
			1.5f + 0.5f * cos(i / (float)numPezzi * freq),
			-3.7f + (i * dimPezzi),
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

static void buildPortholeBorder(Figure* fig, ColorRGBA color)
{
	// draw external hollow circle (metal part)
	buildHollowCircle(fig, 1.0f, 0.7f, 1, color, color);
}

static void buildPortholeGlass(Figure* fig, ColorRGBA color)
{
	// draw internal circle (glass part)
	buildCircle(fig, 0.7f, 1, color, color);
}

void buildSpaceship()
{
	glm::mat4 spaceshipMatrix = glm::mat4(1.0);
	spaceshipMatrix = translate(spaceshipMatrix, glm::vec3(WINDOW_WIDTH / 2, WINDOW_HEIGHT / 2, 0.0f));
	spaceshipMatrix = scale(spaceshipMatrix, glm::vec3(20.0f, 20.0f, 0.0f));

	// Figure for hull
	Figure figHull = {};
	figHull.numTriangles = 15;
	figHull.drawMode = GL_TRIANGLE_STRIP;
	figHull.modelMatrix = spaceshipMatrix;
	buildHull(&figHull, { 0.9f, 0.9f, 0.9f, 1.0f });
	createVAOvector(&figHull);
	spaceship.push_back(figHull);

	// Figure for propulsor
	Figure figPropulsor = {};
	figPropulsor.drawMode = GL_TRIANGLES;
	figPropulsor.modelMatrix = spaceshipMatrix;
	buildPropulsor(&figPropulsor, { 0.6f, 0.6f, 0.6f, 1.0f }, { 0.4f, 0.4f, 0.4f, 1.0f }); // grey
	createVAOvector(&figPropulsor);
	spaceship.push_back(figPropulsor);

	// Figure for nose
	Figure figNose = {};
	figNose.numTriangles = 1;
	figNose.drawMode = GL_TRIANGLES;
	figNose.modelMatrix = spaceshipMatrix;
	buildNose(&figNose, { 1.0f, 0.0f, 0.0f, 1.0f });
	createVAOvector(&figNose);
	spaceship.push_back(figNose);

	// Figure for lateral fins
	Figure figLatFins = {};
	figNose.numTriangles = 4;
	figLatFins.drawMode = GL_TRIANGLES;
	figLatFins.modelMatrix = spaceshipMatrix;
	buildLatFins(&figLatFins, { 1.0f, 0.0f, 0.0f, 1.0f });
	createVAOvector(&figLatFins);
	spaceship.push_back(figLatFins);

	// Figure for central fin
	Figure figCentrFin = {};
	figCentrFin.drawMode = GL_LINES;
	figCentrFin.widthLines = 5.0f;
	figCentrFin.modelMatrix = spaceshipMatrix;
	buildCentrFin(&figCentrFin, { 1.0f, 0.0f, 0.0f, 1.0f });
	createVAOvector(&figCentrFin);
	spaceship.push_back(figCentrFin);

	// Figure for porthole border
	Figure figPortholeBorder = {};
	figPortholeBorder.numTriangles = 30;
	figPortholeBorder.drawMode = GL_TRIANGLE_STRIP;
	figPortholeBorder.modelMatrix = spaceshipMatrix;
	buildPortholeBorder(&figPortholeBorder, { 0.6f, 0.6f, 0.6f, 1.0f }); // grey
	createVAOvector(&figPortholeBorder);
	spaceship.push_back(figPortholeBorder);

	// Figure for porthole glass
	Figure figPortholeGlass = {};
	figPortholeGlass.numTriangles = 30;
	figPortholeGlass.drawMode = GL_TRIANGLES;
	figPortholeGlass.modelMatrix = spaceshipMatrix;
	buildPortholeGlass(&figPortholeGlass, { 0.0f, 0.8f, 1.0f, 0.5f }); // light blue
	createVAOvector(&figPortholeGlass);
	spaceship.push_back(figPortholeGlass);
}

void spawnSpaceship()
{

}

void destroySpaceship()
{

}

void fire()
{

}

// UPDATE =================================================
void updateSpaceship()
{
	
}

void drawSpaceship()
{

}