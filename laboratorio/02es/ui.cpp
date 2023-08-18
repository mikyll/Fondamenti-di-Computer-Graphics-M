#include "ui.h"

Text textMenuTitle;
Text textMenuStart;
Text textMenuInfo;

Text textControls;

Text textGameScore;
Text textGameScoreValue;
Text textGameStage;
// Lives
Text textGamePaused;

Text textStageCompleted;

Text textGameOver;
Text textGameOverScore;

std::vector<Text*> displayedText;

void initUI()
{
	// Menu
	textMenuTitle = createText(WINDOW_WIDTH / 2, WINDOW_HEIGHT * 2 / 3, true, TEXT_SCALE * 2, true, "ASTEROIDS");
	textMenuStart = createText(WINDOW_WIDTH / 2, WINDOW_HEIGHT / 5, true, TEXT_SCALE / 2, true, "Press SPACE to start");

	// Controls
	// TO-DO

	// Score
	textGameScore = createText(50, WINDOW_HEIGHT - 50, false, TEXT_SCALE / 2, false, "SCORE: ");
	textGameScoreValue = createText(200, WINDOW_HEIGHT - 50, false, TEXT_SCALE / 2, false, "100");

	// Stage
	textGameStage = createText((WINDOW_WIDTH / 2) - 50, WINDOW_HEIGHT - 50, true, TEXT_SCALE / 2, false, "STAGE 1");

	// Lives

	// Paused
	textGamePaused = createText(WINDOW_WIDTH / 2, WINDOW_HEIGHT / 2, true, TEXT_SCALE, false, "GAME PAUSED");

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

	// Score: XXX		Stage N			Lives: X X X
}

void showGamePausedUI()
{
	displayedText.clear();

	displayedText.push_back(&textGamePaused);
}

void showStageCompletedUI()
{
	// Stage N completed!
	// Score: XXX

	// Next stage starting in 3 ... 2 ... 1
}

void showGameOverUI()
{
	// Game Over

	// Score: XXX
}

void updateUI()
{
	if (game.state == GAME_RUNNING)
	{
		char buffer[10];

		// Update score value
		snprintf(buffer, 10, "%d", game.score);
		updateText(&textGameScoreValue, buffer);

		// Update stage level
		snprintf(buffer, 10, "STAGE %d", game.stageLevel);
		updateText(&textGameStage, buffer);

		// Update lives
		// TO-DO
	}
}

void drawUI()
{
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

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
	glDisable(GL_BLEND);
}