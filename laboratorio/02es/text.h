#pragma once

#include "commons.h"

void initText();
int addMessage(float posx, float posy, bool center, float scale, bool visibility, const char* message);
bool updateMessage(int index, char* newMessage);
bool isVisible(int index);
bool setVisibility(int index, bool visibility);
void drawText();

extern Game game;
extern unsigned int MatProj, MatModel;