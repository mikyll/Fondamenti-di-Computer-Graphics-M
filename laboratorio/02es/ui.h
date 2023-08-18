#pragma once

#include "commons.h"
#include "text.h"

void initUI();
void showMenuUI();
void showGameUI();
void showGamePausedUI();
void showStageCompletedUI();
void showGameOverUI();
void updateUI();
void drawUI();

extern Game game;
extern unsigned int MatProj, MatModel;