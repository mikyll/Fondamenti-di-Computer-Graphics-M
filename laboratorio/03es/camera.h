#pragma once

#include "commons.h"


void initCamera();

void moveCameraForeward();
void moveCameraBack();
void moveCameraLeft();
void moveCameraRight();
void moveCameraUp();
void moveCameraDown();


extern int windowWidth, windowHeight;
extern Application app;