#include "spaceship.h"

const float maxForwardSpeed = 300.0f;
const float maxAngularSpeed = PI * 2;

extern Input input;

std::vector<Figure> spaceship;

float xPos = WINDOW_WIDTH / 2, yPos = WINDOW_HEIGHT / 2;
float heading = PI / 2;


float forwardSpeed = 0.0f;
float angularSpeed = 0.0f;

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
// INPUT ==================================================
void inputSpaceship()
{
	forwardSpeed = 0.0f;
	angularSpeed = 0.0f;

	if (input.keyboard.keys['w'])
	{
		forwardSpeed += maxForwardSpeed;
	}
	if (input.keyboard.keys['s'])
	{
		forwardSpeed -= maxForwardSpeed;
	}
	if (input.keyboard.keys['a'])
	{
		angularSpeed += maxAngularSpeed;
	}
	if (input.keyboard.keys['d'])
	{
		angularSpeed -= maxAngularSpeed;
	}
}

// UPDATE =================================================
void updateSpaceship(float deltaTime)
{
	if (angularSpeed != 0.0f)
	{
		heading += angularSpeed * deltaTime;
		if (heading > 2 * PI)
			heading -= 2 * PI;
		if (heading < 0.0f)
			heading += 2 * PI;
	}

	if (forwardSpeed != 0.0f)
	{
		xPos += cos(heading) * forwardSpeed * deltaTime;
		yPos += sin(heading) * forwardSpeed * deltaTime;

		if (xPos <= 0.0f)
			xPos = 0.0f;
		if (xPos >= WINDOW_WIDTH)
			xPos = WINDOW_WIDTH;
		if (yPos <= 0.0f)
			yPos = 0.0f;
		if (yPos >= WINDOW_HEIGHT)
			yPos = WINDOW_HEIGHT;
	}
}


void drawSpaceship()
{
	glm::mat4 mat = glm::mat4(1.0);
	mat = translate(mat, glm::vec3(xPos, yPos, 0.0f));
	mat = scale(mat, glm::vec3(20.0f, 20.0f, 0.0f));
	mat = rotate(mat, heading - ((float)PI / 2), glm::vec3(0.0f, 0.0f, 1.0f));

	for (int i = 0; i < spaceship.size(); i++)
	{
		Figure* fig = &spaceship.at(i);
		fig->modelMatrix = mat;

		glUniformMatrix4fv(MatModel, 1, GL_FALSE, value_ptr(mat));
		glBindVertexArray(fig->VAO);
		if (fig->sizePoints > 0.0f)
			glPointSize(fig->sizePoints);
		else glPointSize(1.0f);
		if (fig->widthLines > 0.0f)
			glLineWidth(fig->widthLines);
		else glLineWidth(1.0f);
		glDrawArrays(fig->drawMode, 0, fig->vertices.size());
		glBindVertexArray(0);
	}
}