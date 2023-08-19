#include "ui.h"

Text textMenuTitle;
Text textMenuStart;
Text textMenuInfo;

// Controls background
Text textControls;

Text textGameScore;
Text textGameScoreValue;
Text textGameStage;
// Lives
Text textGamePaused;

Text textStageCompleted;
Text textStageCompletedCurrentScore;
Text textStageCompletedNextStage;

Text textGameOver;
Text textGameOverScore;

Life lifeBase;

std::vector<Text*> displayedText;
std::vector<Life> lives;

static void blinkStartText(int value);
static Life buildLifeFigure(float posx, float posy, float scale);


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
	textMenuStart = createText(WINDOW_WIDTH / 2, WINDOW_HEIGHT / 5, true, TEXT_SCALE / 2, true, "Press SPACE to start");

	// Controls
	// TO-DO

	// Score
	textGameScore = createText(50, WINDOW_HEIGHT - 50, false, TEXT_SCALE / 2, false, "SCORE: ");
	textGameScoreValue = createText(200, WINDOW_HEIGHT - 50, false, TEXT_SCALE / 2, false, "0");

	// Stage
	textGameStage = createText(WINDOW_WIDTH / 2, WINDOW_HEIGHT - 50, true, TEXT_SCALE / 2, false, "STAGE 1");

	// Lives
	lifeBase = buildLifeFigure(WINDOW_WIDTH - 200, WINDOW_HEIGHT - 50, LIFE_SCALE);

	// Paused
	textGamePaused = createText(WINDOW_WIDTH / 2, WINDOW_HEIGHT / 2, true, TEXT_SCALE, false, "GAME PAUSED");

	// Stage Level Completed
	textStageCompleted = createText(WINDOW_WIDTH / 2, WINDOW_HEIGHT * 2 / 3, true, TEXT_SCALE, false, "STAGE 1 COMPLETED");
	textStageCompletedCurrentScore = createText(WINDOW_WIDTH / 2, WINDOW_HEIGHT * 2 / 3 - 100, true, TEXT_SCALE / 2, false, "CURRENT SCORE: 0");
	textStageCompletedNextStage = createText(WINDOW_WIDTH / 2, WINDOW_HEIGHT / 5, true, TEXT_SCALE / 2, false, "NEXT STAGE WILL START IN 3...");

	// Game Over
	textGameOver = createText(WINDOW_WIDTH / 2, WINDOW_HEIGHT * 2 / 3, true, TEXT_SCALE, false, "GAME OVER");
	textGameOverScore = createText(WINDOW_WIDTH / 2, WINDOW_HEIGHT * 2 / 3 - 100, true, TEXT_SCALE / 2, false, "SCORE: 0");
}

static void blinkStartText(int value)
{
	if (game.state == GAME_MENU)
	{
		if (displayedText.size() == 2)
			displayedText.pop_back();
		else displayedText.push_back(&textMenuStart);

		glutTimerFunc(displayedText.size() == 2 ? 1000 : 500, blinkStartText, 0);
	}
}

void showMenuUI()
{
	displayedText.clear();

	// ASTEROIDS
	displayedText.push_back(&textMenuTitle);

	// PRESS SPACE TO START GAME
	displayedText.push_back(&textMenuStart);

	glutTimerFunc(1000, blinkStartText, 0);
}

void showControlsUI()
{

}

void showGameUI()
{
	displayedText.clear();

	displayedText.push_back(&textGameScore);
	displayedText.push_back(&textGameScoreValue);
	displayedText.push_back(&textGameStage);
}

void showGamePausedUI()
{
	displayedText.clear();

	displayedText.push_back(&textGamePaused);
}

void showStageCompletedUI()
{
	// Update score value
	char buffer[32];
	snprintf(buffer, 32, "STAGE %d COMPLETED", game.stageLevel);
	updateText(&textStageCompleted, buffer);

	snprintf(buffer, 32, "CURRENT SCORE: %d", game.score);
	updateText(&textStageCompletedCurrentScore, buffer);

	displayedText.clear();

	displayedText.push_back(&textStageCompleted);
	displayedText.push_back(&textStageCompletedCurrentScore);
	displayedText.push_back(&textStageCompletedCurrentScore);
}

void showGameOverUI()
{
	// Update score value
	char buffer[10];
	snprintf(buffer, 10, "SCORE: %d", game.score);
	updateText(&textGameOverScore, buffer);

	displayedText.clear();

	displayedText.push_back(&textGameOver);
	displayedText.push_back(&textGameOverScore);
}

void updateUI()
{
	if (game.state == GAME_RUNNING)
	{
		char buffer[32];

		// Update score value
		snprintf(buffer, 32, "%d", game.score);
		updateText(&textGameScoreValue, buffer);

		// Update stage level
		snprintf(buffer, 32, "STAGE %d", game.stageLevel);
		updateText(&textGameStage, buffer);

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

	// Text
	for (int i = 0; i < displayedText.size(); i++)
	{
		Text* text = displayedText.at(i);

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
				glDrawArrays(fig->drawMode, 0, fig->vertices.size());

				glBindVertexArray(0);
			}
		}
	}

	glBindVertexArray(0);
	glDisable(GL_BLEND);
}