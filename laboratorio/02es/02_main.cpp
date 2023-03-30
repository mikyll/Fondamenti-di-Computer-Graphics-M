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
int numVerticesBall = 3 * 2 * numTrianglesBall;

typedef struct {
	float x, y, r, g, b, a;
} Pointxy;
Pointxy* ball = new Pointxy[numVerticesBall];

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
} Figure;

void drawBall(int numTriangles, Pointxy* ball);

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

	//Costruzione geometria e colori della PALLA
	drawBall(numTrianglesBall, ball);
	glGenVertexArrays(1, &VAO);
	glBindVertexArray(VAO);
	glGenBuffers(1, &VBO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, numVerticesBall * sizeof(Pointxy), &ball[0], GL_STATIC_DRAW);
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(2 * sizeof(float)));
	glEnableVertexAttribArray(1);
	glBindVertexArray(0);

	// set background color
	glClearColor(0.0, 0.0, 0.0, 1.0);
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
	std::cout << "DeltaTime: " << deltaTime << ", FPS: " << (1.0f / deltaTime) << std::endl;

	glutPostRedisplay();
	glutTimerFunc(DELAY, update, 0);
}

// DRAW ===================================================
// test
double lerp(double a, double b, double amount) {
	//Interpolazione lineare tra a e b secondo amount
	return (1 - amount) * a + amount * b;
}

static void drawCircle(int nTriangles, int step, glm::vec4 color_top, glm::vec4 color_bot, Pointxy* Cerchio) {
	int i;
	float stepA = (2 * 3.14) / nTriangles;

	int comp = 0;
	// step = 1 -> triangoli adiacenti, step = n -> triangoli distanti step l'uno dall'altro
	for (i = 0; i < nTriangles; i += step)
	{
		Cerchio[comp].x = cos((double)i * stepA);
		Cerchio[comp].y = sin((double)i * stepA);
		Cerchio[comp].r = color_top.r; Cerchio[comp].g = color_top.g; Cerchio[comp].b = color_top.b; Cerchio[comp].a = color_top.a;

		Cerchio[comp + 1].x = cos((double)(i + 1) * stepA);
		Cerchio[comp + 1].y = sin((double)(i + 1) * stepA);
		Cerchio[comp + 1].r = color_top.r;
		Cerchio[comp + 1].g = color_top.g;
		Cerchio[comp + 1].b = color_top.b;
		Cerchio[comp + 1].a = color_top.a;

		Cerchio[comp + 2].x = 0.0;
		Cerchio[comp + 2].y = 0.0;
		Cerchio[comp + 2].r = color_bot.r;
		Cerchio[comp + 2].g = color_bot.g;
		Cerchio[comp + 2].b = color_bot.b;
		Cerchio[comp + 2].a = color_bot.a;
		comp += 3;
	}
}

void drawBall(int numTriangles, Pointxy* ball)
{
	int i, cont;
	Pointxy* shadow;
	int vertici = 3 * numTriangles;
	shadow = new Pointxy[vertici];

	//Costruisco la geometria della palla ed i suoi colori
	glm::vec4 col_rosso = { 1.0,0.0,0.0,1.0 };
	glm::vec4 col_bottom = { 1.0, 0.8, 0.0, 1.0 };
	drawCircle(numTriangles, 1, col_rosso, col_bottom, ball);

	//Costruisco la geometria dell'ombra ed i suoi colori.
	glm::vec4 col_top = { 0.49,0.49,0.49, 0.0 };
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
	Model = glm::translate(Model, glm::vec3(posX - bwidth / 2 * shadow_scale, posY + 10 + 10 * (1 - shadow_scale), 0.0f));
	Model = glm::scale(Model, glm::vec3(float(bwidth) * shadow_scale, (50.0 * shadow_scale), 1.0));
	glUniformMatrix4fv(MatModel, 1, GL_FALSE, value_ptr(Model));
	glBindVertexArray(VAO);
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	glDrawArrays(GL_TRIANGLES, numVerticesBall / 2, numVerticesBall / 2);
	//Matrice per il cambiamento del sistema di riferimento per la  PALLA
	Model = glm::mat4(1.0);
	Model = glm::translate(Model, glm::vec3(posX - bwidth / 2, posY + bheight + distacco_da_terra, 0.0f));
	Model = glm::scale(Model, glm::vec3(float(bwidth) / 2, float(bheight) / 2, 1.0));
	glUniformMatrix4fv(MatModel, 1, GL_FALSE, value_ptr(Model));
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	glDrawArrays(GL_TRIANGLES, 0, numVerticesBall / 2);
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
	//glutTimerFunc(DELAY, update, 0);

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