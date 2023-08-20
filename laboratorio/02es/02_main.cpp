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
	game.totalScore = 0;
	game.stageScore = 0;
	game.lives = MAX_LIVES;
}

static void init()
{
	Projection = glm::ortho(0.0f, float(WINDOW_WIDTH), 0.0f, float(WINDOW_HEIGHT));
	MatProj = glGetUniformLocation(programId, "Projection");
	MatModel = glGetUniformLocation(programId, "Model");

	initGame();

	initSpaceship();
	initStars();
	initFiretrail();
	initBullets();
	initExplosions();
	initUI();

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
	game.state = GAME_MENU;

	clearAsteroids();
	initAsteroids(10);
	spawnSpaceship(0);
	game.stageLevel = 1;
	game.totalScore = 0;
	game.stageScore = 0;
	game.lives = MAX_LIVES;

	showMenuUI();
}

static void startGame(int stageLevel)
{
	game.state = GAME_RUNNING;
	game.stageScore = 0;

	spawnSpaceship(1);
	clearAsteroids();
	initAsteroids(NUM_ASTEROIDS * stageLevel);

	showGameUI();
}

static void gameOver(int value)
{
	std::cout << "GAME OVER" << std::endl;

	//glutExit();
	//exit(0);
}

static void inputApplication()
{
	if ((game.state == GAME_RUNNING
		|| game.state == GAME_OVER
		|| game.state == GAME_PAUSED)
		&& input.keyboard.keys[27])
	{
		startMenu();
	}
	if ((game.state == GAME_MENU
		|| game.state == GAME_MENU_CONTROLS)
		&& input.keyboard.keys['h'])
	{
		input.keyboard.keys['h'] = 0;

		if (game.state == GAME_MENU)
		{
			game.state = GAME_MENU_CONTROLS;
			showControlsUI();
		}
		else
		{
			game.state = GAME_MENU;
			showMenuUI();
		}
	}
	if (game.state == GAME_MENU && input.keyboard.keys[' '])
	{
		input.keyboard.keys[' '] = 0;

		startGame(game.stageLevel);
	}
	if (input.keyboard.keys['c'])
	{
		input.keyboard.keys['c'] = 0;

		game.showColliders = !game.showColliders;
	}
	if (input.keyboard.keys['l'])
	{
		input.keyboard.keys['l'] = 0;

		game.showLines = !game.showLines;
	}
	if ((game.state == GAME_RUNNING || game.state == GAME_PAUSED)
		&& input.keyboard.keys['p'])
	{
		input.keyboard.keys['p'] = 0;

		if (game.state == GAME_RUNNING)
		{
			game.state = GAME_PAUSED;
			showGamePausedUI();
		}
		else if (game.state == GAME_PAUSED)
		{
			game.state = GAME_RUNNING;
			showGameUI();
		}
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
		updateBullets(game.deltaTime);
	}
	if (game.state == GAME_RUNNING)
	{
		updateSpaceship(game.deltaTime);
		updateFiretrail(game.deltaTime);
		updateStars(game.deltaTime);
		updateExplosions(game.deltaTime);
		
		updateAsteroids(game.deltaTime);
		updateBullets(game.deltaTime);
	}
	if (game.state == GAME_STAGE_COMPLETED)
	{
		updateSpaceship(game.deltaTime);
		updateFiretrail(game.deltaTime);
		updateStars(game.deltaTime);
		updateBullets(game.deltaTime);
		updateExplosions(game.deltaTime);
	}
	if (game.state == GAME_NEXT_STAGE_STARTING)
	{
		game.stageLevel++;
		startGame(game.stageLevel);
	}
	if (game.state == GAME_OVER)
	{
		updateSpaceship(game.deltaTime);
		updateAsteroids(game.deltaTime);
		updateFiretrail(game.deltaTime);
		updateBullets(game.deltaTime);
		updateExplosions(game.deltaTime);
	}
	updateUI();

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
	drawExplosions();
	drawUI();

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