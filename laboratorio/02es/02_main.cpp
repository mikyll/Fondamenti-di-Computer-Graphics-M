/*
* Lab 02 - 2D Animation & Gameplay
*
* Solution by Michele Righi (0001025005)
* GitHub: https://github.com/mikyll/Fondamenti-di-Computer-Graphics-M/tree/main/laboratorio/02es
*
*
* Description:
* This program implements a simple version of the well-know game
* "Asteroids" (https://it.wikipedia.org/wiki/Asteroids).
* Some notable parts/features are:
* - usage of a simple pass-through shader;
* - game loop using delta time (plus cap at ~60 FPS);
* - spaceship:
*	- can be controlled by the user;
*	- is composed of different parts:
*		- hull (using cos function for the curves);
*		- propulsor;
*		- fins & nose;
*		- porthole (circle & hollow circle using sin & cos);
*		- astronaut always facing the same direction to make
*			it *pretty*;
* - background made more realistic using the parallax effect
*	(3 layers of stars moving at different speeds);
* - spaceship firetrail implemented using a particle system;
* - bullets that can be fired by the spaceship;
* - asteroids:
*	- 3 different types (with different shapes and colors);
*	- 3 different shapes;
*	- moving in a random direction;
*	- when broken by a bullet are split in 2 smaller asteroids;
* - circle collision system (rect for the spaceship yet to
*	be implemented);
* 
* 
* Controls:
* - Character controls:
*	- press 'w' to move the spaceship forward;
*	- press 'a'/'d' to rotate the spaceship heading
*		clockwise/counter clockwise;
*	- press 'SPACE' to fire a bullet;
* - Misc. controls:
*	- press 'p' to pause/resume the game;
*	- press 'r'	to reset the spaceship position to the center;
*	- press 'c' to display/hide the objects colliders;
*	- press 'b' to enlarge/shrink the spaceship;
*	- press 'ESC' to quit the game.
* 
* 
* Gameplay:
* Destroy each asteroid without being touched by them.
*/

#include "commons.h"

static unsigned int programId;

unsigned int MatProj, MatModel;

glm::mat4 Projection;  // Matrice di proiezione

extern Input input;

Game game = {};

static void startMenu();
static void blinkStartText(int value);

// INIT ===================================================
static void initShader()
{
	GLenum ErrorCheckValue = glGetError();

	char* vertexShader = (char*)"resources/shaders/vertexShader_C_M.glsl";
	char* fragmentShader = (char*)"resources/shaders/fragmentShader_C_M.glsl";

	programId = ShaderMaker::createProgram(vertexShader, fragmentShader);
	glUseProgram(programId);
}

static void initGame()
{
	game = {};
	game.state = GAME_MENU;
	game.deltaTime = 0.0f;
	game.fps = 0;
	game.stageLevel = 1;
	game.score = 0;
}

static void init()
{
	Projection = glm::ortho(0.0f, float(WINDOW_WIDTH), 0.0f, float(WINDOW_HEIGHT));
	MatProj = glGetUniformLocation(programId, "Projection");
	MatModel = glGetUniformLocation(programId, "Model");

	initGame();

	spawnSpaceship();
	initStars();
	initFiretrail();
	initAsteroids();
	initBullets();
	initText();

	startMenu();
}

static void printControls()
{
	std::cout << "CONTROLS ==============================" << std::endl;
	std::cout << "  W     - move forward" << std::endl;
	std::cout << "  A/D   - rotate left/right" << std::endl;
	std::cout << "  SPACE - fire bullet" << std::endl << std::endl;
	std::cout << "  P     - toggle pause game" << std::endl;
	std::cout << "  R     - reset spaceship position" << std::endl;
	std::cout << "  C     - toggle colliders" << std::endl;
	std::cout << "  B     - toggle big spaceship" << std::endl;
	std::cout << "  L     - toggle render using only lines" << std::endl;
	std::cout << std::endl;
	std::cout << "  ESC   - quit" << std::endl;
	std::cout << "=======================================" << std::endl << std::endl;
}

static void startMenu()
{
	clearAsteroids();
	initAsteroids();
	resetSpaceship();

	setVisibility(game.textIndexes.iMenuTitle, true);
	setVisibility(game.textIndexes.iMenuStart, true);
	setVisibility(game.textIndexes.iScore, false);
	setVisibility(game.textIndexes.iScoreValue, false);
	setVisibility(game.textIndexes.iStage, false);
	setVisibility(game.textIndexes.iStageValue, false);

	game.state = GAME_MENU;

	glutTimerFunc(1000, blinkStartText, 0);
}

static void startGame()
{
	clearAsteroids();
	initAsteroids();
	game.stageLevel = 1;
	game.score = 0;

	setVisibility(game.textIndexes.iMenuTitle, false);
	setVisibility(game.textIndexes.iMenuStart, false);
	setVisibility(game.textIndexes.iScore, true);
	setVisibility(game.textIndexes.iScoreValue, true);
	setVisibility(game.textIndexes.iStage, true);
	setVisibility(game.textIndexes.iStageValue, true);

	game.state = GAME_RUNNING;
}

static void endGame(int value)
{
	std::cout << "GAME OVER" << std::endl;
	glutExit();

	exit(0);
}

static void blinkStartText(int value)
{
	if (game.state == GAME_MENU)
	{
		setVisibility(game.textIndexes.iMenuStart, !isVisible(game.textIndexes.iMenuStart));

		glutTimerFunc(isVisible(game.textIndexes.iMenuStart) ? 1000 : 500, blinkStartText, 0);
	}
	else setVisibility(game.textIndexes.iMenuStart, false);
}

static void inputApplication()
{
	if (game.state == GAME_RUNNING && input.keyboard.keys[27])
	{
		startMenu();
	}
	if (game.state == GAME_MENU && input.keyboard.keys[' '])
	{
		startGame();
	}
}

// UPDATE =================================================
static void update(int value)
{
	// Wait until 16ms has elapsed since last frame
	while (glutGet(GLUT_ELAPSED_TIME) - game.timeSinceStart <= 16);
	
	// Delta time = time elapsed from last frame (in seconds)
	game.deltaTime = (glutGet(GLUT_ELAPSED_TIME) - game.timeSinceStart) / 1000.0f;
	
	// Clamp delta time so that it doesn't exceed ~60 fps
	game.deltaTime = MIN(0.05f, game.deltaTime);

	// Update elapsed time (for next frame)
	game.timeSinceStart = glutGet(GLUT_ELAPSED_TIME);


	// INPUT PROCESSING -----------------------------------
	inputApplication();
	inputSpaceship();

	// UPDATE GAME LOGIC ----------------------------------
	if (game.state == GAME_MENU)
	{
		updateAsteroids(game.deltaTime);
		updateFiretrail(game.deltaTime);
	}
	if (game.state == GAME_RUNNING)
	{
		updateSpaceship(game.deltaTime);
		updateFiretrail(game.deltaTime);
		updateStars(game.deltaTime);
		
		updateAsteroids(game.deltaTime);
		updateBullets(game.deltaTime);

		// test
		char scoreText[10];
		snprintf(scoreText, 10, "%d", game.score);
		updateMessage(game.textIndexes.iScoreValue, scoreText);
	}
	if (game.state == GAME_PAUSED)
	{
		setVisibility(game.textIndexes.iPaused, true);
	}
	else
	{
		setVisibility(game.textIndexes.iPaused, false);
	}
	if (game.state == GAME_OVER)
	{
		updateSpaceship(game.deltaTime);
		updateFiretrail(game.deltaTime);
		updateStars(game.deltaTime);
		
		glutPostRedisplay();
		glutTimerFunc(3000, endGame, 0);
	}

	glutPostRedisplay();
	glutTimerFunc(UPDATE_DELAY, update, 0);
}

// DRAW ===================================================
static void drawScene()
{
	// set background color
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);

	glClear(GL_COLOR_BUFFER_BIT);

	glUniformMatrix4fv(MatProj, 1, GL_FALSE, value_ptr(Projection));
	

	// DRAW SCENE OBJECTS ---------------------------------
	drawStars();
	drawFiretrail();
	drawAsteroids();
	drawSpaceship();
	drawBullets();

	drawText();

	glutSwapBuffers();
}

// MAIN ===================================================
int main(int argc, char** argv)
{
	printControls();

	glutInit(&argc, argv);
	
	glutInitContextVersion(4, 0);
	glutInitContextProfile(GLUT_CORE_PROFILE);

	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA);

	glutInitWindowSize(WINDOW_WIDTH, WINDOW_HEIGHT);
	glutInitWindowPosition(10, 10);
	glutCreateWindow("2D Asteroids");

	// Input callbacks
	glutKeyboardFunc(keyDown);
	glutKeyboardUpFunc(keyUp);

	// Update callback
	game.timeSinceStart = glutGet(GLUT_ELAPSED_TIME);
	glutTimerFunc(UPDATE_DELAY, update, 0);

	// Draw callback
	glutDisplayFunc(drawScene);

	glewExperimental = GL_TRUE;
	glewInit();

	initShader();
	init();

	glutSetCursor(GLUT_CURSOR_NONE);

	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glutMainLoop();

	return 0;
}