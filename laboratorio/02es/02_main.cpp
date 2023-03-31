#include "commons.h"

extern Input input;

static unsigned int programId;

unsigned int VAO;
unsigned int VBO;

float deltaTime = 0.0f;
unsigned long timeSinceStart; // milliseconds from glutInit() call

unsigned int MatProj, MatModel;

glm::mat4 Projection;  //Matrice di proiezione


extern std::vector<Figure> spaceship;
float heading = 0;

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
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
}


// INPUT ==================================================
static void doSpaceShipInput()
{
	if (input.keyboard.keys['a'])
	{
		heading += 180.0f * deltaTime;
		if (heading >= 360.0f)
			heading = 0.0f;
	}
	if (input.keyboard.keys['d'])
	{
		heading -= 180.0f * deltaTime;
		if (heading < 0.0f)
			heading = 359.0f;
	}
}

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
	doSpaceShipInput();
	
	
	
	// Test
	//std::cout << "DeltaTime: " << deltaTime << ", FPS: " << (1.0f / deltaTime) << std::endl;

	glutPostRedisplay();
	glutTimerFunc(DELAY, update, 0);
}

// DRAW ===================================================








static void drawSpaceship()
{
	glm::mat4 mat = glm::mat4(1.0);
	mat = translate(mat, glm::vec3(WINDOW_WIDTH / 2, WINDOW_HEIGHT / 2, 0.0f));
	mat = scale(mat, glm::vec3(20.0f, 20.0f, 0.0f));
	mat = rotate(mat, (float)(heading * PI / 180.0f), glm::vec3(0.0f, 0.0f, 1.0f));

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

static void drawScene()
{
	glClear(GL_COLOR_BUFFER_BIT);

	glUniformMatrix4fv(MatProj, 1, GL_FALSE, value_ptr(Projection));
	

	// Draw
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