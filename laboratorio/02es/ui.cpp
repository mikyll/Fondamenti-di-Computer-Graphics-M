#include "ui.h"

Text textMenuTitle;
Text textMenuControlsHint;
Text textMenuInfo;
Text textMenuStart;

Text textControls;
Text textControlMoveForward;
Text textControlRotate;
Text textControlFire;
Text textControlColliders;
Text textControlLines;
Text textControlBigShip;
Text textPause;
Text textReturnToMenu;

Text textGameScore;
Text textGameScoreValue;
Text textGameStage;
Text textInvulnerability;
Text textInvulnerabilityTimeLeft;

Text textGamePaused;

Text textStageCompleted;
Text textStageCompletedNextStage;

Text textGameOver;
Text textGameOverReturnToMenu;

Text textStageScore;
Text textStageAccuracy;
Text textStageTime;
Text textTotalScore;

Life lifeBase;

// Controls background
Figure backgroundControls;
std::vector<Text*> displayedText;
std::vector<Life> lives;

static Life buildLifeFigure(float posx, float posy, float scale);

static void blinkStartText(int value);
static void addScoreToTotal(int value);
static void calculateNewScore(int value);
static void countdownNextStageText(int value);
static void blinkGameOverText(int value);

// Draw the controls background box
static Figure buildBackgroundControls()
{
	Figure fig = {};

	fig.id = 0;
	fig.drawMode = GL_TRIANGLE_STRIP;
	
	fig.vertices.push_back({ WINDOW_WIDTH / 10, WINDOW_HEIGHT * 9 / 10, 0.0f });
	fig.vertices.push_back({ WINDOW_WIDTH * 9 / 10, WINDOW_HEIGHT * 9 / 10, 0.0f });
	fig.vertices.push_back({ WINDOW_WIDTH * 9 / 10, WINDOW_HEIGHT / 10, 0.0f });
	
	fig.vertices.push_back({ WINDOW_WIDTH * 9 / 10, WINDOW_HEIGHT / 10, 0.0f });
	fig.vertices.push_back({ WINDOW_WIDTH / 10, WINDOW_HEIGHT * 9 / 10, 0.0f });
	fig.vertices.push_back({ WINDOW_WIDTH / 10, WINDOW_HEIGHT / 10, 0.0f });


	// Color
	for (int i = 0; i < fig.vertices.size(); i++)
	{
		fig.colors.push_back({ 0.0f, 0.0f, 0.0f, 0.8f });
	}
	createFigureVAO(&fig);

	return fig;
}

static Life buildLifeFigure(float posx, float posy, float scale)
{
	Life life = {};
	Figure figBackground = {};
	Figure figSuit = {};
	Figure figVisor = {};
	int numTriangles = 15;
	float stepA = PI / numTriangles; // PI -> half a circle
	Point3D center;
	float radius;

	// Background
	figBackground.id = 0;
	figBackground.drawMode = GL_TRIANGLES;
	buildCircle(&figBackground, { 0.0f, 0.0f, 0.0f }, 0.9f, numTriangles * 2, { 0.3f, 0.3f, 0.3f, 1.0f }, { 0.3f, 0.3f, 0.3f, 1.0f });
	createFigureVAO(&figBackground);
	life.figures.push_back(figBackground);

	// Astronaut Suit
	figSuit.id = 1;
	figSuit.drawMode = GL_TRIANGLE_FAN;
	center = { 0.0f, 0.0f, 0.0f };
	radius = 0.5f;

	figSuit.vertices.push_back(center);
	for (int i = 0; i <= numTriangles; i++)
	{
		float t = (float)i * stepA + PI * 2;
		figSuit.vertices.push_back({ center.x + radius * cos(t), center.y + radius * sin(t), 0.0f });
	}
	figSuit.vertices.push_back(center);

	figSuit.vertices.push_back({ center.x - radius, center.y, 0.0f });
	figSuit.vertices.push_back({ center.x + radius, center.y, 0.0f });
	figSuit.vertices.push_back({ center.x + radius, center.y - radius * 1.5f, 0.0f });

	figSuit.vertices.push_back({ center.x - radius, center.y, 0.0f });
	figSuit.vertices.push_back({ center.x - radius, center.y - radius * 1.5f, 0.0f });
	figSuit.vertices.push_back({ center.x + radius, center.y - radius * 1.5f, 0.0f });

	for (int i = 0; i < figSuit.vertices.size(); i++)
	{
		figSuit.colors.push_back({ 1.0f, 1.0f, 1.0f, 1.0f });
	}

	createFigureVAO(&figSuit);
	life.figures.push_back(figSuit);

	// Astronaut Visor
	figVisor.id = 1;
	figVisor.drawMode = GL_TRIANGLE_FAN;
	radius = 0.25f;
	center = { -radius * 1.8f, 0.0f, 0.0f };

	// Left semicircle
	figVisor.vertices.push_back(center);
	for (int i = 0; i <= numTriangles; i++)
	{
		float t = (float)i * stepA + PI * 0.5f;
		figVisor.vertices.push_back({ center.x + radius * cos(t), center.y + radius * sin(t), 0.0f });
	}

	// Middle section
	figVisor.vertices.push_back({ center.x, center.y + radius, 0.0f });
	figVisor.vertices.push_back({ center.x + radius * 1.5f, center.y + radius, 0.0f });
	figVisor.vertices.push_back({ center.x + radius * 1.5f, center.y - radius, 0.0f });
	figVisor.vertices.push_back({ center.x + radius * 1.5f, center.y - radius, 0.0f });
	figVisor.vertices.push_back({ center.x, center.y - radius, 0.0f });

	center = { -radius * 2 + radius * 1.5f, 0.0f, 0.0f };

	// Right semicircle
	figVisor.vertices.push_back(center);
	figVisor.vertices.push_back(center);
	for (int i = 0; i <= numTriangles; i++)
	{
		float t = (float)i * stepA + PI * 1.5f;
		figVisor.vertices.push_back({ center.x + radius * cos(t), center.y + radius * sin(t), 0.0f });
	}

	// Color
	for (int i = 0; i < figVisor.vertices.size(); i++)
	{
		figVisor.colors.push_back({ 0.0f, 0.0f, 0.0f, 1.0f });
	}

	createFigureVAO(&figVisor);
	life.figures.push_back(figVisor);

	life.pos = { posx, posy, 0.0f };
	life.scale = scale;
	return life;
}

void initUI()
{
	// Menu
	textMenuTitle = createText(WINDOW_WIDTH / 2, WINDOW_HEIGHT * 2 / 3, true, TEXT_SCALE * 2, true, "ASTEROIDS");
	textMenuControlsHint = createText(20, 20, false, TEXT_SCALE / 4, true, "Press 'H' to show controls");
	textMenuInfo = createText(WINDOW_WIDTH - 100, 20, true, TEXT_SCALE / 4, true, "Game by Michele Righi");
	textMenuStart = createText(WINDOW_WIDTH / 2, WINDOW_HEIGHT / 5, true, TEXT_SCALE / 2, true, "Press 'SPACE' to start");

	// Controls
	backgroundControls = buildBackgroundControls();
	textControls = createText(WINDOW_WIDTH / 2, WINDOW_HEIGHT * 9 / 10 - 50, true, TEXT_SCALE , true, "CONTROLS");
	textControlMoveForward = createText(WINDOW_WIDTH / 2 - 200, WINDOW_HEIGHT / 2 + 150, false, TEXT_SCALE / 2, true,     "  W     - MOVE FORWARD");
	textControlRotate = createText(WINDOW_WIDTH / 2 - 200, WINDOW_HEIGHT / 2 + 100, false, TEXT_SCALE / 2, true,     " A/D    - ROTATE LEFT/RIGHT");
	textControlFire = createText(WINDOW_WIDTH / 2 - 200, WINDOW_HEIGHT / 2 + 25, false, TEXT_SCALE / 2, true,      "SPACE   - FIRE BULLET");
	textControlColliders = createText(WINDOW_WIDTH / 2 - 200, WINDOW_HEIGHT / 2 - 25, false, TEXT_SCALE / 2, true, "  C     - TOGGLE COLLIDERS");
	textControlLines = createText(WINDOW_WIDTH / 2 - 200, WINDOW_HEIGHT / 2 - 75, false, TEXT_SCALE / 2, true,     "  L     - TOGGLE RENDERING MODE");
	textControlBigShip = createText(WINDOW_WIDTH / 2 - 200, WINDOW_HEIGHT / 2 - 125, false, TEXT_SCALE / 2, true,   "  B     - TOGGLE BIG SHIP");
	textPause = createText(WINDOW_WIDTH / 2 - 200, WINDOW_HEIGHT / 2 - 200, false, TEXT_SCALE / 2, true,            "  P     - TOGGLE PAUSE GAME");
	textReturnToMenu = createText(WINDOW_WIDTH / 2 - 200, WINDOW_HEIGHT / 2 - 250, false, TEXT_SCALE / 2, true,     " ESC    - GO BACK TO MENU");

	// Game
	textGameScore = createText(50, WINDOW_HEIGHT - 50, false, TEXT_SCALE / 2, true, "SCORE: ");
	textGameScoreValue = createText(150, WINDOW_HEIGHT - 50, false, TEXT_SCALE / 2, true, "0");
	textGameStage = createText(WINDOW_WIDTH / 2, WINDOW_HEIGHT - 50, true, TEXT_SCALE / 2, true, "STAGE 1");
	lifeBase = buildLifeFigure(WINDOW_WIDTH - 200, WINDOW_HEIGHT - 50, LIFE_SCALE);
	textInvulnerability = createText(50, 50, false, TEXT_SCALE / 2, true, "SHIELD: ");
	textInvulnerabilityTimeLeft = createText(170, 50, false, TEXT_SCALE / 2, true, "3");

	// Paused
	textGamePaused = createText(WINDOW_WIDTH / 2, WINDOW_HEIGHT / 2, true, TEXT_SCALE, true, "GAME PAUSED");

	// Stage Level Completed
	textStageCompleted = createText(WINDOW_WIDTH / 2, WINDOW_HEIGHT * 4 / 5, true, TEXT_SCALE, true, "STAGE 1 COMPLETED");
	textStageCompletedNextStage = createText(WINDOW_WIDTH / 2, WINDOW_HEIGHT / 5, true, TEXT_SCALE / 2, true, "                             ");

	// Game Over
	textGameOver = createText(WINDOW_WIDTH / 2, WINDOW_HEIGHT * 4 / 5, true, TEXT_SCALE, true, "GAME OVER");
	textGameOverReturnToMenu = createText(WINDOW_WIDTH / 2, WINDOW_HEIGHT / 5, true, TEXT_SCALE / 2, true, "Press 'ESC' to go back to the menu");

	// Stats
	textStageScore = createText(WINDOW_WIDTH / 2, WINDOW_HEIGHT * 3 / 5 , true, TEXT_SCALE / 2, true,			"STAGE SCORE:        0");
	textStageAccuracy = createText(WINDOW_WIDTH / 2, WINDOW_HEIGHT * 3 / 5 - 50, true, TEXT_SCALE / 2, true,	"STAGE ACCURACY:  100%");
	textStageTime = createText(WINDOW_WIDTH / 2, WINDOW_HEIGHT * 3 / 5 - 100, true, TEXT_SCALE / 2, true,		"STAGE TIME:  00:00:00");
	textTotalScore = createText(WINDOW_WIDTH / 2, WINDOW_HEIGHT * 3 / 5 - 200, true, TEXT_SCALE / 2, true,		"TOTAL SCORE:        0");
}

static void blinkStartText(int value)
{
	if (game.state == GAME_MENU)
	{
		textMenuStart.visible = !textMenuStart.visible;

		glutTimerFunc(textMenuStart.visible ? 1000 : 500, blinkStartText, 0);
	}
}

static void addScoreToTotal(int value)
{
	if (game.state == GAME_STAGE_COMPLETED || game.state == GAME_OVER)
	{
		char buffer[32];
		snprintf(buffer, 32, "TOTAL SCORE: %8d +%d", game.totalScore, game.stageScore);
		updateText(&textTotalScore, buffer);

		glutTimerFunc(1000, calculateNewScore, value);
	}
}

static void calculateNewScore(int value)
{
	if (game.state == GAME_STAGE_COMPLETED || game.state == GAME_OVER)
	{
		if (game.totalScore < value)
		{
			char buffer[32];
			game.totalScore++;
			snprintf(buffer, 32, "TOTAL SCORE: %8d", game.totalScore);
			updateText(&textTotalScore, buffer);

			glutTimerFunc(0, calculateNewScore, value);
		}
		else
		{
			if (game.state == GAME_STAGE_COMPLETED)
			{
				glutTimerFunc(1000, countdownNextStageText, 3);
			}
			else
			{
				glutTimerFunc(1000, blinkGameOverText, 0);
			}
		}
	}
}

static void countdownNextStageText(int value)
{
	if (game.state == GAME_STAGE_COMPLETED)
	{
		if (value > 0)
		{
			char buffer[32];
			snprintf(buffer, 32, "NEXT STAGE WILL START IN %d...", value);
			updateText(&textStageCompletedNextStage, buffer);
			textStageCompletedNextStage.visible = true;

			glutTimerFunc(1000, countdownNextStageText, value - 1);
		}
		else
		{
			textStageCompletedNextStage.visible = false;
			game.state = GAME_NEXT_STAGE_STARTING;
		}
	}
}

static void blinkGameOverText(int value)
{
	if (game.state == GAME_OVER)
	{
		textGameOverReturnToMenu.visible = !textGameOverReturnToMenu.visible;

		glutTimerFunc(textGameOverReturnToMenu.visible ? 1000 : 500, blinkGameOverText, 0);
	}
}

void showMenuUI()
{
	displayedText.clear();

	// ASTEROIDS
	displayedText.push_back(&textMenuTitle);

	// INFO
	displayedText.push_back(&textMenuControlsHint);
	displayedText.push_back(&textMenuInfo);

	// PRESS SPACE TO START GAME
	displayedText.push_back(&textMenuStart);

	glutTimerFunc(1000, blinkStartText, 0);
}

void showControlsUI()
{
	displayedText.clear();

	displayedText.push_back(&textControls);
	displayedText.push_back(&textControlMoveForward);
	displayedText.push_back(&textControlRotate);
	displayedText.push_back(&textControlFire);
	displayedText.push_back(&textControlColliders);
	displayedText.push_back(&textControlLines);
	displayedText.push_back(&textControlBigShip);
	displayedText.push_back(&textPause);
	displayedText.push_back(&textReturnToMenu);
}

void showGameUI()
{
	displayedText.clear();

	displayedText.push_back(&textGameScore);
	displayedText.push_back(&textGameScoreValue);
	displayedText.push_back(&textGameStage);
	displayedText.push_back(&textInvulnerability);
	displayedText.push_back(&textInvulnerabilityTimeLeft);
}

void showGamePausedUI()
{
	displayedText.clear();

	displayedText.push_back(&textGamePaused);
}

void showStageCompletedUI()
{
	char buffer[32];
	snprintf(buffer, 32, "STAGE %d COMPLETED", game.stageLevel);
	updateText(&textStageCompleted, buffer);

	snprintf(buffer, 32, "STAGE SCORE: %8d", game.stageScore);
	updateText(&textStageScore, buffer);

	snprintf(buffer, 32, "STAGE ACCURACY:  %3d%%", game.stageBulletShot == 0 ? 0 : (int) (((float) game.stageAsteroidsHit / (float) game.stageBulletShot) * 100.0f));
	updateText(&textStageAccuracy, buffer);

	snprintf(buffer, 32, "STAGE TIME:  %02d:%02d:%02d", game.stageTime / 3600, game.stageTime / 60, game.stageTime);
	updateText(&textStageTime, buffer);

	snprintf(buffer, 32, "TOTAL SCORE: %8d", game.totalScore);
	updateText(&textTotalScore, buffer);

	textStageCompletedNextStage.visible = false;

	std::cout << "Stage time: " << game.stageTime << std::endl << "Total time: " << game.totalTime << std::endl;

	displayedText.clear();

	displayedText.push_back(&textStageCompleted);
	displayedText.push_back(&textStageScore);
	displayedText.push_back(&textStageAccuracy);
	displayedText.push_back(&textStageTime);
	displayedText.push_back(&textTotalScore);
	displayedText.push_back(&textStageCompletedNextStage);

	glutTimerFunc(1000, addScoreToTotal, game.totalScore + game.stageScore);
}

void showGameOverUI()
{
	// Update score value
	char buffer[32];
	snprintf(buffer, 32, "TOTAL ACCURACY:  %3d%%", game.totalBulletShot == 0 ? 0 : (int)(((float)game.totalAsteroidsHit / (float)game.totalBulletShot) * 100.0f));
	updateText(&textStageAccuracy, buffer);

	snprintf(buffer, 32, "TOTAL TIME:  %02d:%02d:%02d", game.totalTime / 3600, game.totalTime / 60, game.totalTime);
	updateText(&textStageTime, buffer);

	snprintf(buffer, 32, "TOTAL SCORE: %8d", game.totalScore);
	updateText(&textTotalScore, buffer);

	textGameOverReturnToMenu.visible = false;

	displayedText.clear();

	displayedText.push_back(&textGameOver);
	displayedText.push_back(&textStageAccuracy);
	displayedText.push_back(&textStageTime);
	displayedText.push_back(&textTotalScore);
	displayedText.push_back(&textGameOverReturnToMenu);

	if (game.stageScore > 0)
		glutTimerFunc(1000, addScoreToTotal, game.totalScore + game.stageScore);
	else glutTimerFunc(1000, blinkGameOverText, 0);
}

void updateUI()
{
	if (game.state == GAME_RUNNING)
	{
		char buffer[32];

		// Update score value
		snprintf(buffer, 32, "%5d", game.stageScore);
		updateText(&textGameScoreValue, buffer);

		// Update stage level
		snprintf(buffer, 32, "STAGE %d", game.stageLevel);
		updateText(&textGameStage, buffer);

		// Update invulnerability
		snprintf(buffer, 32, "%1.3f", abs(spaceship.invulnerabilityTime));
		updateText(&textInvulnerabilityTimeLeft, buffer);
		if (spaceship.invulnerable && displayedText.size() == 3)
		{
			displayedText.push_back(&textInvulnerability);
			displayedText.push_back(&textInvulnerabilityTimeLeft);
		}
		else if (!spaceship.invulnerable && displayedText.size() == 5)
		{
			displayedText.pop_back();
			displayedText.pop_back();
		}

		// Update lives
		if (game.lives != lives.size())
		{
			lives.clear();
			for (int i = 0; i < game.lives; i++)
			{
				Life lifeTmp = lifeBase;
				lifeTmp.pos.x += i * LIFE_SCALE * 3;
				lives.push_back(lifeTmp);
			}
		}
	}
}

void drawUI()
{
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	if (game.state == GAME_MENU_CONTROLS)
	{
		glBindVertexArray(backgroundControls.VAO);
		glDrawArrays(backgroundControls.drawMode, 0, backgroundControls.vertices.size());
		glBindVertexArray(0);
	}

	// Text
	for (int i = 0; i < displayedText.size(); i++)
	{
		Text* text = displayedText.at(i);

		if (text->visible)
		{
			for (int j = 0; j < text->figures.size(); j++)
			{
				Figure* fig = &text->figures.at(j);

				glm::mat4 modelMatrix = glm::mat4(1.0);
				modelMatrix = translate(modelMatrix, glm::vec3(text->pos.x, text->pos.y, 0.0f));
				modelMatrix = scale(modelMatrix, glm::vec3(text->scale, text->scale, 0.0f));
				glUniformMatrix4fv(MatModel, 1, GL_FALSE, value_ptr(modelMatrix));

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
	}
	glBindVertexArray(0);

	// Figure
	if (game.state == GAME_RUNNING)
	{
		for (int i = 0; i < lives.size(); i++)
		{
			Life* life = &lives.at(i);

			for (int j = 0; j < life->figures.size(); j++)
			{
				Figure* fig = &life->figures.at(j);
				glm::mat4 modelMatrix = glm::mat4(1.0);
				modelMatrix = translate(modelMatrix, glm::vec3(life->pos.x, life->pos.y, 0.0f));
				modelMatrix = scale(modelMatrix, glm::vec3(life->scale, life->scale, 0.0f));
				glUniformMatrix4fv(MatModel, 1, GL_FALSE, value_ptr(modelMatrix));

				glBindVertexArray(fig->VAO);
				
				glLineWidth(1.0f);
				int mode = game.showLines ? GL_LINE_STRIP : fig->drawMode;
				glDrawArrays(mode, 0, fig->vertices.size());

				glBindVertexArray(0);
			}
		}
	}

	glBindVertexArray(0);
	glDisable(GL_BLEND);
}