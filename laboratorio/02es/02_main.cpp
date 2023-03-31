#include "stage.h"

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
	float x, y;
} Point2D;

typedef struct {
	float r, g, b, a;
} ColorRGBA;

typedef struct {
	GLuint VAO;
	GLuint VBO_Geom; // VBO della geometria dei vertici
	GLuint VBO_Col; // VBO dei colori
	int numTriangles;
	// Vertices
	std::vector<Point2D> vertices;
	std::vector<ColorRGBA> colors;
	int numVertices;

	// Modellation Matrix: Translation * Rotation * Scaling
	glm::mat4 modelMatrix;
	int drawMode;
} Figure;

void drawBall(int numTriangles, Pointxy* ball);
void buildPorthole(int numTriangles, Pointxy* porthole);

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

	//Costruzione geometria e colori del Porthole
	buildPorthole(numTrianglesPorthole, porthole);
	glGenVertexArrays(1, &VAO);
	glBindVertexArray(VAO);
	glGenBuffers(1, &VBO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, numVerticesPorthole * sizeof(Pointxy), &porthole[0], GL_STATIC_DRAW);
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(2 * sizeof(float)));
	glEnableVertexAttribArray(1);
	glBindVertexArray(0);

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
// test
double lerp(double a, double b, double amount) {
	//Interpolazione lineare tra a e b secondo amount
	return (1 - amount) * a + amount * b;
}

static void disegna_cerchio(int nTriangles, int step, glm::vec4 color_top, glm::vec4 color_bot, Pointxy* Cerchio) {
	int i;
	float stepA = (2 * 3.14) / nTriangles;

	int comp = 0;
	// step = 1 -> triangoli adiacenti, step = n -> triangoli distanti step l'uno dall'altro
	for (i = 0; i < nTriangles; i += step)
	{
		Cerchio[comp].x = cos((double)i * stepA);
		Cerchio[comp].y = sin((double)i * stepA);
		Cerchio[comp].r = color_top.r;
		Cerchio[comp].g = color_top.g;
		Cerchio[comp].b = color_top.b;
		Cerchio[comp].a = color_top.a;

		Cerchio[comp + 1].x = cos((double)(i + 1) * stepA);
		Cerchio[comp + 1].y = sin((double)(i + 1) * stepA);
		Cerchio[comp + 1].r = color_top.r;
		Cerchio[comp + 1].g = color_top.g;
		Cerchio[comp + 1].b = color_top.b;
		Cerchio[comp + 1].a = color_top.a;

		//std::cout << "X: " << Cerchio[comp+1].x << ", Y: " << Cerchio[comp+1].y << std::endl;

		Cerchio[comp + 2].x = 0.8 * cos((double)i * stepA);
		Cerchio[comp + 2].y = 0.8 * sin((double)i * stepA);
		Cerchio[comp + 2].r = color_bot.r;
		Cerchio[comp + 2].g = color_bot.g;
		Cerchio[comp + 2].b = color_bot.b;
		Cerchio[comp + 2].a = color_bot.a;

		// second
		Cerchio[comp + 3].x = 0.8 * cos((double)(i + 1) * stepA);
		Cerchio[comp + 3].y = 0.8 * sin((double)(i + 1) * stepA);
		Cerchio[comp + 3].r = color_bot.r;
		Cerchio[comp + 3].g = color_bot.g;
		Cerchio[comp + 3].b = color_bot.b;
		Cerchio[comp + 3].a = color_bot.a;

		/*Cerchio[comp + 4].x = 0.5 * cos((double)(i)*stepA);
		Cerchio[comp + 4].y = 0.5 * sin((double)(i)*stepA);
		Cerchio[comp + 4].r = color_bot.r;
		Cerchio[comp + 4].g = color_bot.g;
		Cerchio[comp + 4].b = color_bot.b;
		Cerchio[comp + 4].a = color_bot.a;

		Cerchio[comp + 5].x = cos((double)(i + 1) * stepA);
		Cerchio[comp + 5].y = sin((double)(i + 1) * stepA);
		Cerchio[comp + 5].r = color_top.r;
		Cerchio[comp + 5].g = color_top.g;
		Cerchio[comp + 5].b = color_top.b;
		Cerchio[comp + 5].a = color_top.a;*/
		comp += 4;
	}
}

static void drawCircle2D(Pointxy* circle, int numSides, float radius, ColorRGBA colorCenter, ColorRGBA colorExtern)
{
	std::cout << colorCenter.r << std::endl;

	float stepA = (2 * 3.14) / numSides;

	for (int comp = 0, i = 0; i < numSides; i++, comp += 3)
	{
		// External circumference (first vertex)
		circle[comp].x = radius * cos((double)i * stepA);
		circle[comp].y = radius * sin((double)i * stepA);
		circle[comp].r = colorExtern.r;
		circle[comp].g = colorExtern.g;
		circle[comp].b = colorExtern.b;
		circle[comp].a = colorExtern.a;
		// External circumference (second vertex)
		circle[comp + 1].x = radius * cos((double)(i + 1) * stepA);
		circle[comp + 1].y = radius * sin((double)(i + 1) * stepA);
		circle[comp + 1].r = colorExtern.r;
		circle[comp + 1].g = colorExtern.g;
		circle[comp + 1].b = colorExtern.b;
		circle[comp + 1].a = colorExtern.a;
		// Center
		circle[comp + 2].x = cos((double)i * stepA);
		circle[comp + 2].y = sin((double)i * stepA);
		circle[comp + 2].r = colorCenter.r;
		circle[comp + 2].g = colorCenter.g;
		circle[comp + 2].b = colorCenter.b;
		circle[comp + 2].a = colorCenter.a;
	}
}

static void drawSemicircle(float cx, float cy, float radius, int numSides)
{

}

static void buildCircle(Pointxy* circle, int numSides, int step, float radius, glm::vec4 colorExtern, glm::vec4 colorCenter)
{
	float stepA = (2 * PI) / numSides;

	for (int comp = 0, i = 0; i < numSides; i += step, comp += 3)
	{
		// External circumference (first vertex)
		circle[comp].x = radius * cos((double)i * stepA);
		circle[comp].y = radius * sin((double)i * stepA);
		circle[comp].r = colorExtern.r;
		circle[comp].g = colorExtern.g;
		circle[comp].b = colorExtern.b;
		circle[comp].a = colorExtern.a;
		
		// External circumference (second vertex)
		circle[comp + 1].x = radius * cos((double)(i + 1) * stepA);
		circle[comp + 1].y = radius * sin((double)(i + 1) * stepA);
		circle[comp + 1].r = colorExtern.r;
		circle[comp + 1].g = colorExtern.g;
		circle[comp + 1].b = colorExtern.b;
		circle[comp + 1].a = colorExtern.a;
		
		// Center
		circle[comp + 2].x = 0.0f;
		circle[comp + 2].y = 0.0f;
		circle[comp + 2].r = colorCenter.r;
		circle[comp + 2].g = colorCenter.g;
		circle[comp + 2].b = colorCenter.b;
		circle[comp + 2].a = colorCenter.a;
	}
}

static void buildHollowCircle(Pointxy* circle, int nTriangles, int step, float width, glm::vec4 colorExtern, glm::vec4 colorIntern)
{
	float stepA = (2 * PI) / nTriangles;

	for (int comp = 0, i = 0; i < nTriangles; comp += 4, i += step)
	{
		// Extern vertices
		circle[comp].x = cos((double)i * stepA);
		circle[comp].y = sin((double)i * stepA);
		circle[comp].r = colorExtern.r;
		circle[comp].g = colorExtern.g;
		circle[comp].b = colorExtern.b;
		circle[comp].a = colorExtern.a;

		circle[comp + 1].x = cos((double)(i + 1) * stepA);
		circle[comp + 1].y = sin((double)(i + 1) * stepA);
		circle[comp + 1].r = colorExtern.r;
		circle[comp + 1].g = colorExtern.g;
		circle[comp + 1].b = colorExtern.b;
		circle[comp + 1].a = colorExtern.a;

		// Intern vertices
		circle[comp + 2].x = width * cos((double)i * stepA);
		circle[comp + 2].y = width * sin((double)i * stepA);
		circle[comp + 2].r = colorIntern.r;
		circle[comp + 2].g = colorIntern.g;
		circle[comp + 2].b = colorIntern.b;
		circle[comp + 2].a = colorIntern.a;

		circle[comp + 3].x = width * cos((double)(i + 1) * stepA);
		circle[comp + 3].y = width * sin((double)(i + 1) * stepA);
		circle[comp + 3].r = colorIntern.r;
		circle[comp + 3].g = colorIntern.g;
		circle[comp + 3].b = colorIntern.b;
		circle[comp + 3].a = colorIntern.a;
	}
}

void drawBall(int numTriangles, Pointxy* ball)
{
	int i, cont;
	int vertici = 3 * numTriangles;

	//Costruisco la geometria della palla ed i suoi colori
	glm::vec4 col_rosso = { 1.0,0.0,0.0,1.0 };
	glm::vec4 col_bottom = { 1.0, 0.8, 0.0, 1.0 };

	glm::vec4 col_cyan = { 0.0, 0.8, 1.0, 0.5 };
	glm::vec4 col_grey = { 0.6, 0.6, 0.6, 1.0 };
	disegna_cerchio(numTriangles, 1, col_grey, col_grey, ball);
	//drawCircle2D(ball, numTriangles, 1.0f, { 1.0,0.0,0.0,1.0 }, { 1.0, 0.8, 0.0, 1.0 });

	cont = 4 * numTriangles;
	
	/*ball[cont] = {-1.0f, 2.0f, 0.0f, 0.0f, 1.0f, 1.0f};
	ball[cont + 1] = { 0.0f, 2.5f, 0.0f, 0.0f, 1.0f, 1.0f };
	ball[cont + 2] = { 1.0f, 2.0f, 0.0f, 0.0f, 1.0f, 1.0f };*/


	//Costruisco la geometria dell'ombra ed i suoi colori.
	/*glm::vec4 col_top = {0.49,0.49,0.49, 0.0};
	col_bottom = { 0.0, 0.0, 0.0, 0.6 };
	drawCircle(numTriangles, 1, col_top, col_bottom, shadow);

	// Appendo a Palla la sua Ombra
	cont = 3 * numTriangles;
	for (i = 0; i < 3 * numTriangles; i++)
	{
		ball[cont + i].x = shadow[i].x;
		ball[cont + i].y = shadow[i].y;
		ball[cont + i].r = shadow[i].r;
		ball[cont + i].g = shadow[i].g;
		ball[cont + i].b = shadow[i].b;
		ball[cont + i].a = shadow[i].a;
	}*/
}

static void drawHull(int numTriangles, Pointxy* hull)
{
	// 2 curves
}

static void drawNose(Pointxy* nose)
{
	// triangolo rosso
	glm::vec4 color = { 1.0f, 0.0f, 0.0f, 1.0f };
	nose[0].x = -1.0f;
	nose[0].y = 1.0f;
	nose[1].x = 1.0f;
	nose[1].y = 1.0f;
	nose[2].x = 0.0f;
	nose[2].y = 1.5f;

	for (int i = 0; i < 3; i++)
	{
		nose[i].r = color.r;
		nose[i].g = color.g;
		nose[i].b = color.b;
		nose[i].a = color.a;
	}
}

static void drawFins(int numTriangles, Pointxy* fins)
{
	// triangoli rossi
}

static void drawPropeller(int numTriangles, Pointxy* propeller)
{
	// trapezio grigio
}

//////////////////////////////////////

void createVAOvector(Figure* fig)
{
	glGenVertexArrays(1, &fig->VAO);
	glBindVertexArray(fig->VAO);
	//Genero , rendo attivo, riempio il VBO della geometria dei vertici
	glGenBuffers(1, &fig->VBO_Geom);
	glBindBuffer(GL_ARRAY_BUFFER, fig->VBO_Geom);
	glBufferData(GL_ARRAY_BUFFER, fig->vertices.size() * sizeof(Point2D), fig->vertices.data(), GL_STATIC_DRAW);

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

static void buildSpaceship()
{
	
}

static void buildPorthole(int numTriangles, Pointxy* porthole)
{
	// draw external hollow circle (metal part)
	glm::vec4 colorMetal = { 0.6f, 0.6f, 0.6f, 1.0f };
	buildHollowCircle(porthole, numTriangles, 1, 0.8, colorMetal, colorMetal);

	// draw internal circle (glass part)
	Pointxy* portholeGlass = new Pointxy[3 * numTriangles];
	glm::vec4 colorGlass = { 0.0f, 0.8f, 1.0f, 0.5f };
	buildCircle(portholeGlass, numTriangles, 1, 0.8f, colorGlass, colorGlass);

	int offset = 4 * numTriangles;
	for (int i = 0; i < 3 * numTriangles; i++)
	{
		porthole[offset + i].x = portholeGlass[i].x;
		porthole[offset + i].y = portholeGlass[i].y;
		porthole[offset + i].r = portholeGlass[i].r;
		porthole[offset + i].g = portholeGlass[i].g;
		porthole[offset + i].b = portholeGlass[i].b;
		porthole[offset + i].a = portholeGlass[i].a;
	}
}

static void drawSpaceship()
{


	for (int i = 0; i < spaceship.size(); i++)
	{
		Figure fig = spaceship.at(i);

		glUniformMatrix4fv(MatModel, 1, GL_FALSE, value_ptr(Model));
		// Border
		glPolygonMode(GL_FRONT_AND_BACK, fig.drawMode);
		glDrawArrays(GL_TRIANGLE_STRIP, 0, numVerticesPorthole / 3);
		glUniformMatrix4fv(MatModel, 1, GL_FALSE, value_ptr(Model));
		// Glass
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
		glDrawArrays(GL_TRIANGLES, numVerticesPorthole / 3, numVerticesPorthole / 3);
		glBindVertexArray(0);
	}
}

static void drawScene()
{
	glClear(GL_COLOR_BUFFER_BIT);

	glUniformMatrix4fv(MatProj, 1, GL_FALSE, value_ptr(Projection));
	

	// Draw
	
	
	int	distacco_da_terra = 100;
	
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
	glBindVertexArray(0);

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
	glutInitWindowPosition(100, 100);
	glutCreateWindow("2D Animation");

	// Input callbacks
	/*glutSetKeyRepeat(GLUT_KEY_REPEAT_OFF);
	glutKeyboardFunc(inputKeyboard);
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