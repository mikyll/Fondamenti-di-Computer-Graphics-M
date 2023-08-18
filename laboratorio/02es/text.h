#pragma once

#include "commons.h"

typedef struct {
	int id;
	std::vector<Figure> figures;
	Point3D pos;
	float scale;
	bool visible;
} Text;

Text createText(float posx, float posy, bool center, float scale, bool visibility, const char* message);
void updateText(Text* text, char* newMessage);

extern Game game;
extern unsigned int MatProj, MatModel;