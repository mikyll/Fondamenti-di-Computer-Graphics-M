#include "stage.h"

extern Input input;

static unsigned int programId;

unsigned int VAO;
unsigned int VBO;

float deltaTime = 0.0f;
unsigned long timeSinceStart; // milliseconds from glutInit() call

unsigned int MatProj, MatModel;

glm::mat4 Projection;  //Matrice di proiezione
glm::mat4 Model; //Matrice per il cambiamento da Sistema di riferimento dell'oggetto OCS a sistema di riferimento nel Mondo WCS

int numTrianglesBall = 30;
int numVerticesBall = 4 * numTrianglesBall;

int numTrianglesPorthole = 30;
int numVerticesPorthole = 4 * 3 * numTrianglesPorthole;

typedef struct {
	float x, y, r, g, b, a;
} Pointxy;

Pointxy* ball = new Pointxy[numVerticesBall];

Pointxy* porthole = new Pointxy[numVerticesPorthole];

float posX = WINDOW_WIDTH / 2.0f;
float posY = WINDOW_HEIGHT / 2.0f;

/*typedef struct {
	float x, y;
} Point2D;

typedef struct {
	float r, g, b, a;
} ColorRGBA;

typedef struct {
	Point2D* vertex = new Point2D[numTrianglesBall];
	int numTriangles;
	int numVertices;
	
} Ball;

Point2D* ball = new Point2D[numVerticesBall];*/

typedef struct {
	float x, y, z;
} Point3D;

typedef struct {
	float r, g, b, a;
} ColorRGBA;

typedef struct {
	GLuint VAO;
	GLuint VBO_Geom; // VBO della geometria dei vertici
	GLuint VBO_Col; // VBO dei colori
	int numTriangles;
	// Vertices
	std::vector<Point3D> vertices;
	std::vector<ColorRGBA> colors;

	// Modellation Matrix: Translation * Rotation * Scaling
	glm::mat4 modelMatrix;
	int drawMode;
	float sizePoints;
	float widthLines;
} Figure;


static void buildSpaceship();

std::vector<Figure> spaceship;

// INIT ===================================================
void initShader()
{
	GLenum ErrorCheckValue = glGetError();

	char* vertexShader = (char*)"vertexShader_C_M.glsl";
	char* fragmentShader = (char*)"fragmentShader_C_M.glsl";

	programId = ShaderMaker::createProgram(vertexShader, fragmentShader);
	glUseProgram(programId);

}

void init()
{
	Projection = glm::ortho(0.0f, float(WINDOW_WIDTH), 0.0f, float(WINDOW_HEIGHT));
	MatProj = glGetUniformLocation(programId, "Projection");
	MatModel = glGetUniformLocation(programId, "Model");

	buildSpaceship();

	// set background color
	glClearColor(1.0, 1.0, 1.0, 1.0);
}


// INPUT ==================================================
// TO-DO

// UPDATE =================================================
static void update(int value)
{
	// Wait until 16ms has elapsed since last frame
	while (glutGet(GLUT_ELAPSED_TIME) - timeSinceStart <= 16);
	
	// Delta time = time elapsed from last frame (in seconds)
	deltaTime = (glutGet(GLUT_ELAPSED_TIME) - timeSinceStart) / 1000.0f;
	
	// Clamp delta time so that it doesn't exceed ~60 fps
	deltaTime = MIN(0.05f, deltaTime);

	// Update elapsed time (for next frame)
	timeSinceStart = glutGet(GLUT_ELAPSED_TIME);

	// Update game logic

	// Test
	//std::cout << "DeltaTime: " << deltaTime << ", FPS: " << (1.0f / deltaTime) << std::endl;

	glutPostRedisplay();
	glutTimerFunc(DELAY, update, 0);
}

// DRAW ===================================================


void createVAOvector(Figure* fig)
{
	glGenVertexArrays(1, &fig->VAO);
	glBindVertexArray(fig->VAO);
	//Genero , rendo attivo, riempio il VBO della geometria dei vertici
	glGenBuffers(1, &fig->VBO_Geom);
	glBindBuffer(GL_ARRAY_BUFFER, fig->VBO_Geom);
	glBufferData(GL_ARRAY_BUFFER, fig->vertices.size() * sizeof(Point3D), fig->vertices.data(), GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);
	glEnableVertexAttribArray(0);

	//Genero , rendo attivo, riempio il VBO dei colori
	glGenBuffers(1, &fig->VBO_Col);
	glBindBuffer(GL_ARRAY_BUFFER, fig->VBO_Col);
	glBufferData(GL_ARRAY_BUFFER, fig->colors.size() * sizeof(ColorRGBA), fig->colors.data(), GL_STATIC_DRAW);
	//Adesso carico il VBO dei colori nel layer 2
	glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, 0, (void*)0);
	glEnableVertexAttribArray(1);
}

static void buildCircle(Figure* fig, float radius, int step, ColorRGBA colorExtern, ColorRGBA colorIntern)
{
	// 2 * PI = complete circle => divide by num of triangles we want to use
	float stepA = (2 * PI) / fig->numTriangles;

	for (int i = 0; i < fig->numTriangles; i += step)
	{
		// Extern vertices
		Point3D v0 = { radius * cos((double)i * stepA), radius * sin((double)i * stepA), 0.0f };
		fig->vertices.push_back(v0);
		fig->colors.push_back(colorExtern);

		Point3D v1 = { radius * cos((double)(i + 1) * stepA), radius * sin((double)(i + 1) * stepA), 0.0f };
		fig->vertices.push_back(v1);
		fig->colors.push_back(colorExtern);

		// Intern vertices
		Point3D v2 = { 0.0f, 0.0f, 0.0f };
		fig->vertices.push_back(v2);
		fig->colors.push_back(colorIntern);
	}
}

static void buildHollowCircle(Figure* fig, float radiusExtern, float radiusIntern, int step, ColorRGBA colorExtern, ColorRGBA colorIntern)
{
	float stepA = (2 * PI) / fig->numTriangles;

	for (int i = 0; i < fig->numTriangles; i += step)
	{
		// Extern vertices
		Point3D v0 = { radiusExtern * cos((double)i * stepA), radiusExtern * sin((double)i * stepA), 0.0f };
		fig->vertices.push_back(v0);
		fig->colors.push_back(colorExtern);

		Point3D v1 = { radiusExtern * cos((double)(i + 1) * stepA), radiusExtern * sin((double)(i + 1) * stepA), 0.0f };
		fig->vertices.push_back(v1);
		fig->colors.push_back(colorExtern);

		// Intern vertices
		Point3D v2 = { radiusIntern * cos((double)i * stepA), radiusIntern * sin((double)i * stepA), 0.0f };
		fig->vertices.push_back(v2);
		fig->colors.push_back(colorIntern);

		Point3D v3 = { radiusIntern * cos((double)(i + 1) * stepA), radiusIntern * sin((double)(i + 1) * stepA), 0.0f };
		fig->vertices.push_back(v3);
		fig->colors.push_back(colorIntern);
	}
}

static void buildNose(Figure* fig, ColorRGBA color)
{
	fig->vertices.push_back({ -1.0f, 2.0f, 0.0f});
	fig->vertices.push_back({ 1.0f, 2.0f, 0.0f });
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

static void buildSpaceship()
{
	glm::mat4 spaceshipMatrix = glm::mat4(1.0);
	spaceshipMatrix = translate(spaceshipMatrix, glm::vec3(WINDOW_WIDTH / 2, WINDOW_HEIGHT / 2, 0.0f));
	spaceshipMatrix = scale(spaceshipMatrix, glm::vec3(40.0f, 40.0f, 0.0f));
	spaceshipMatrix = rotate(spaceshipMatrix, (float)(30.0f * PI / 180.0f), glm::vec3(0.0f, 0.0f, 1.0f));

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



static void drawSpaceship()
{
	for (int i = 0; i < spaceship.size(); i++)
	{
		Figure fig = spaceship.at(i);

		glUniformMatrix4fv(MatModel, 1, GL_FALSE, value_ptr(fig.modelMatrix));
		glBindVertexArray(fig.VAO);
		if (fig.sizePoints > 0.0f)
			glPointSize(fig.sizePoints);
		else glPointSize(1.0f);
		if (fig.widthLines > 0.0f)
			glLineWidth(fig.widthLines);
		else glLineWidth(1.0f);
		glDrawArrays(fig.drawMode, 0, fig.vertices.size());
		glBindVertexArray(0);
	}
}

static void drawScene()
{
	glClear(GL_COLOR_BUFFER_BIT);

	glUniformMatrix4fv(MatProj, 1, GL_FALSE, value_ptr(Projection));
	

	// Draw
	
	
	/*int	distacco_da_terra = 100;
	
	// Disegna palla (ombra+palla)
	Model = glm::mat4(1.0);
	float shadow_scale = lerp(1, 0, (float)distacco_da_terra / 255); //distacco grande -> fattore scala piccolo
	// larghezza effettiva in pixel della palla (80 diametro palla normale -- 100 palla dilatata)
	double bwidth = distacco_da_terra < 0 ? lerp(80, 100, (double)abs(distacco_da_terra) / 20) : 80;
	// altezza effettiva in pixel della palla
	double bheight = distacco_da_terra < 0 ? 80 + distacco_da_terra : 80;
	//Matrice per il cambiamento del sistema di riferimento per l'OMBRA della palla
	//Model = glm::translate(Model, glm::vec3(posX - bwidth / 2 * shadow_scale, posY + 10 + 10 * (1 - shadow_scale), 0.0f));
	Model = glm::scale(Model, glm::vec3(float(bwidth) * shadow_scale, (50.0 * shadow_scale), 1.0));
	glUniformMatrix4fv(MatModel, 1, GL_FALSE, value_ptr(Model));
	glBindVertexArray(VAO);
	//Matrice per il cambiamento del sistema di riferimento per la  PALLA
	Model = glm::mat4(1.0);
	Model = glm::translate(Model, glm::vec3(WINDOW_WIDTH / 2, WINDOW_HEIGHT / 2, 0.0f));
	Model = glm::scale(Model, glm::vec3(float(bwidth) / 2, float(bheight) / 2, 1.0));

	
	glUniformMatrix4fv(MatModel, 1, GL_FALSE, value_ptr(Model));
	// Border
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	glDrawArrays(GL_TRIANGLE_STRIP, 0, numVerticesPorthole / 3);
	glUniformMatrix4fv(MatModel, 1, GL_FALSE, value_ptr(Model));
	// Glass
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	glDrawArrays(GL_TRIANGLES, numVerticesPorthole / 3, numVerticesPorthole / 3);
	glBindVertexArray(0);*/

	drawSpaceship();

	glutSwapBuffers();
}

// MAIN ===================================================
int main(int argc, char** argv)
{
	glutInit(&argc, argv);
	
	glutInitContextVersion(4, 0);
	glutInitContextProfile(GLUT_CORE_PROFILE);

	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA);

	glutInitWindowSize(WINDOW_WIDTH, WINDOW_HEIGHT);
	glutInitWindowPosition(10, 10);
	glutCreateWindow("2D Animation");

	// Input callbacks
	glutKeyboardFunc(keyDown);
	glutKeyboardUpFunc(keyUp);
	/*glutSetKeyRepeat(GLUT_KEY_REPEAT_OFF);
	glutKeyboardFunc(inputKeyboard);
	glutKeyboardUpFunc(inputKeyboard);
	glutSpecialFunc(specialKeyDown);
	glutSpecialUpFunc(specialKeyUp);
	glutMouseFunc(inputMouseClick);
	glutPassiveMotionFunc(inputMousePassiveMove);
	glutMotionFunc(inputMouseDrag);
	*/

	// Update callback
	timeSinceStart = glutGet(GLUT_ELAPSED_TIME);
	glutTimerFunc(DELAY, update, 0);

	// Draw callback
	glutDisplayFunc(drawScene);

	glewExperimental = GL_TRUE;
	glewInit();

	initShader();
	init();

	glEnable(GL_BLEND);
	glEnable(GL_ALPHA_TEST);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glutMainLoop();

	return 0;
}

// Clessidra
/*static void buildHull(Figure* fig, ColorRGBA color)
{
	float altezza = 6.2f;
	int numPezzi = fig->numTriangles;
	float dimPezzi = altezza / numPezzi;
	float freq = PI;

	for (int i = 0; i < numPezzi; i++)
	{
		// Extern vertices
		Point3D v0 = {
			-1.5f - 0.5f * cos(2 * (i / (float)numPezzi * freq)),
			-3.7f + (i * dimPezzi),
			0.0f
		};
		fig->vertices.push_back(v0);

		Point3D v1 = {
			1.5f + 0.5f * cos(2 * (i / (float)numPezzi * freq)),
			-3.7f + (i * dimPezzi),
			0.0f
		};
		fig->vertices.push_back(v1);
	}


	for (int i = 0; i < fig->vertices.size(); i++)
	{
		fig->colors.push_back(color);
	}
}*/