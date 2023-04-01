#include "spaceship.h"

const float maxForwardSpeed = 300.0f;
const float maxAngularSpeed = PI * 2;

Entity spaceship;

extern Input input;

bool showLines = false;

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
	spaceship.figures.push_back(figHull);

	// Figure for propulsor
	Figure figPropulsor = {};
	figPropulsor.drawMode = GL_TRIANGLES;
	figPropulsor.modelMatrix = spaceshipMatrix;
	buildPropulsor(&figPropulsor, { 0.6f, 0.6f, 0.6f, 1.0f }, { 0.4f, 0.4f, 0.4f, 1.0f }); // grey
	createVAOvector(&figPropulsor);
	spaceship.figures.push_back(figPropulsor);

	// Figure for nose
	Figure figNose = {};
	figNose.numTriangles = 1;
	figNose.drawMode = GL_TRIANGLES;
	figNose.modelMatrix = spaceshipMatrix;
	buildNose(&figNose, { 1.0f, 0.0f, 0.0f, 1.0f });
	createVAOvector(&figNose);
	spaceship.figures.push_back(figNose);

	// Figure for lateral fins
	Figure figLatFins = {};
	figNose.numTriangles = 4;
	figLatFins.drawMode = GL_TRIANGLES;
	figLatFins.modelMatrix = spaceshipMatrix;
	buildLatFins(&figLatFins, { 1.0f, 0.0f, 0.0f, 1.0f });
	createVAOvector(&figLatFins);
	spaceship.figures.push_back(figLatFins);

	// Figure for central fin
	Figure figCentrFin = {};
	figCentrFin.drawMode = GL_LINES;
	figCentrFin.widthLines = 5.0f;
	figCentrFin.modelMatrix = spaceshipMatrix;
	buildCentrFin(&figCentrFin, { 1.0f, 0.0f, 0.0f, 1.0f });
	createVAOvector(&figCentrFin);
	spaceship.figures.push_back(figCentrFin);

	// Figure for porthole border
	Figure figPortholeBorder = {};
	figPortholeBorder.numTriangles = 30;
	figPortholeBorder.drawMode = GL_TRIANGLE_STRIP;
	figPortholeBorder.modelMatrix = spaceshipMatrix;
	buildPortholeBorder(&figPortholeBorder, { 0.6f, 0.6f, 0.6f, 1.0f }); // grey
	createVAOvector(&figPortholeBorder);
	spaceship.figures.push_back(figPortholeBorder);

	// Figure for porthole glass
	Figure figPortholeGlass = {};
	figPortholeGlass.numTriangles = 30;
	figPortholeGlass.drawMode = GL_TRIANGLES;
	figPortholeGlass.modelMatrix = spaceshipMatrix;
	buildPortholeGlass(&figPortholeGlass, { 0.0f, 0.8f, 1.0f, 0.5f }); // light blue
	createVAOvector(&figPortholeGlass);
	spaceship.figures.push_back(figPortholeGlass);
}

void spawnSpaceship()
{
	spaceship.pos.x = WINDOW_WIDTH / 2;
	spaceship.pos.y = WINDOW_HEIGHT / 2;
	spaceship.heading = PI / 2;
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

	spaceship.forwardSpeed = 0.0f;
	spaceship.angularSpeed = 0.0f;

	if (input.keyboard.keys['w'])
	{
		spaceship.forwardSpeed += maxForwardSpeed;
	}
	if (input.keyboard.keys['s'])
	{
		spaceship.forwardSpeed -= maxForwardSpeed;
	}
	if (input.keyboard.keys['a'])
	{
		spaceship.angularSpeed += maxAngularSpeed;
	}
	if (input.keyboard.keys['d'])
	{
		spaceship.angularSpeed -= maxAngularSpeed;
	}

	if (input.keyboard.keys['r'])
	{
		spaceship.forwardSpeed = 0.0f;
		spaceship.angularSpeed = 0.0f;
		spaceship.pos.x = WINDOW_WIDTH / 2;
		spaceship.pos.y = WINDOW_HEIGHT / 2;
		spaceship.heading = PI / 2;
	}

	// test
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

		
		//std::cout << "(" << spaceship.pos.x << "," << spaceship.pos.y << ")" << std::endl;

		// Wrap x
		if (spaceship.pos.x < 0.0f)
			spaceship.pos.x = WINDOW_WIDTH + spaceship.pos.x;
		if (spaceship.pos.x > WINDOW_WIDTH)
			spaceship.pos.x -= WINDOW_WIDTH;

		// Wrap y
		if (spaceship.pos.y < 0.0f)
			spaceship.pos.y = WINDOW_HEIGHT + spaceship.pos.y;
		if (spaceship.pos.y > WINDOW_HEIGHT)
			spaceship.pos.y -= WINDOW_HEIGHT;


		/*spaceship.pos.x = MAX(0.0f, MIN(WINDOW_WIDTH, spaceship.pos.x));
		spaceship.pos.y = MAX(0.0f, MIN(WINDOW_HEIGHT, spaceship.pos.y));*/
	}
}


void drawSpaceship()
{
	if (!spaceship.health)
		return;

	glm::mat4 mat = glm::mat4(1.0);
	mat = translate(mat, glm::vec3(spaceship.pos.x, spaceship.pos.y, 0.0f));
	mat = scale(mat, glm::vec3(20.0f, 20.0f, 0.0f));
	mat = rotate(mat, spaceship.heading - ((float)PI / 2), glm::vec3(0.0f, 0.0f, 1.0f));

	for (int i = 0; i < spaceship.figures.size(); i++)
	{
		Figure* fig = &spaceship.figures.at(i);
		fig->modelMatrix = mat;

		glUniformMatrix4fv(MatModel, 1, GL_FALSE, value_ptr(mat));
		glBindVertexArray(fig->VAO);
		if (fig->sizePoints > 0.0f)
			glPointSize(fig->sizePoints);
		else glPointSize(1.0f);
		if (fig->widthLines > 0.0f)
			glLineWidth(fig->widthLines);
		else glLineWidth(1.0f);

		// test
		int mode = fig->drawMode;
		if (showLines)
		{
			if (fig->drawMode == GL_TRIANGLES)
				mode = GL_LINES;
			if (fig->drawMode == GL_TRIANGLE_STRIP)
				mode = GL_LINE_STRIP;
		}
		glDrawArrays(mode, 0, fig->vertices.size());
		glBindVertexArray(0);
	}
}