#include "text.h"

std::vector<Text> texts;

static void addLetter(Figure* fig, ColorRGBA color, char letter, float offset)
{
	fig->drawMode = GL_LINE_STRIP;
	switch (letter)
	{
	case ' ':
		break;
	case 'a':
	case 'A':
		fig->vertices.push_back({ -1.0f + offset, -2.0f, 0.0f });
		fig->vertices.push_back({ 0.0f + offset, 2.0f, 0.0f });
		fig->vertices.push_back({ 1.0f + offset, -2.0f, 0.0f });
		fig->vertices.push_back({ 0.5f + offset, 0.0f, 0.0f });
		fig->vertices.push_back({ -0.5f + offset, 0.0f, 0.0f });
		break;
	case 'b':
	case 'B':
		fig->vertices.push_back({ -1.0f + offset, -2.0f, 0.0f });
		fig->vertices.push_back({ -1.0f + offset, 2.0f, 0.0f });
		fig->vertices.push_back({ 0.0f + offset, 2.0f, 0.0f });
		fig->vertices.push_back({ 0.5f + offset, 1.75f, 0.0f });
		fig->vertices.push_back({ 0.75f + offset, 1.5f, 0.0f });
		fig->vertices.push_back({ 0.875f + offset, 1.25f, 0.0f });
		fig->vertices.push_back({ 1.0f + offset, 1.0f, 0.0f });
		fig->vertices.push_back({ 0.875f + offset, 0.75f, 0.0f });
		fig->vertices.push_back({ 0.75f + offset, 0.5f, 0.0f });
		fig->vertices.push_back({ 0.5f + offset, 0.25f, 0.0f });
		fig->vertices.push_back({ 0.0f + offset, 0.0f, 0.0f });
		fig->vertices.push_back({ -1.0f + offset, 0.0f, 0.0f });
		fig->vertices.push_back({ 0.0f + offset, 0.0f, 0.0f });
		fig->vertices.push_back({ 0.5f + offset, -0.25f, 0.0f });
		fig->vertices.push_back({ 0.75f + offset, -0.5f, 0.0f });
		fig->vertices.push_back({ 0.875f + offset, -0.75f, 0.0f });
		fig->vertices.push_back({ 1.0f + offset, -1.0f, 0.0f });
		fig->vertices.push_back({ 0.875f + offset, -1.25f, 0.0f });
		fig->vertices.push_back({ 0.75f + offset, -1.5f, 0.0f });
		fig->vertices.push_back({ 0.5f + offset, -1.75f, 0.0f });
		fig->vertices.push_back({ 0.0f + offset, -2.0f, 0.0f });
		fig->vertices.push_back({ -1.0f + offset, -2.0f, 0.0f });
		break;
	case 'c':
	case 'C':
		fig->vertices.push_back({ 1.0f + offset, -1.25f, 0.0f });
		fig->vertices.push_back({ 0.75f + offset, -1.5f, 0.0f });
		fig->vertices.push_back({ 0.5f + offset, -1.75f, 0.0f });
		fig->vertices.push_back({ 0.0f + offset, -2.0f, 0.0f });
		fig->vertices.push_back({ -0.5f + offset, -1.75f, 0.0f });
		fig->vertices.push_back({ -0.75f + offset, -1.5f, 0.0f });
		fig->vertices.push_back({ -0.875f + offset, -1.25f, 0.0f });
		fig->vertices.push_back({ -1.0f + offset, -1.0f, 0.0f });
		fig->vertices.push_back({ -1.0f + offset, 1.0f, 0.0f });
		fig->vertices.push_back({ -0.875f + offset, 1.25f, 0.0f });
		fig->vertices.push_back({ -0.75f + offset, 1.5f, 0.0f });
		fig->vertices.push_back({ -0.5f + offset, 1.75f, 0.0f });
		fig->vertices.push_back({ 0.0f + offset, 2.0f, 0.0f });
		fig->vertices.push_back({ 0.5f + offset, 1.75f, 0.0f });
		fig->vertices.push_back({ 0.75f + offset, 1.5f, 0.0f });
		fig->vertices.push_back({ 1.0f + offset, 1.25f, 0.0f });
		break;
	case 'd':
	case 'D':
		fig->vertices.push_back({ -1.0f + offset, -2.0f, 0.0f });
		fig->vertices.push_back({ -1.0f + offset, 2.0f, 0.0f });
		fig->vertices.push_back({ 0.0f + offset, 2.0f, 0.0f });
		fig->vertices.push_back({ 0.5f + offset, 1.75f, 0.0f });
		fig->vertices.push_back({ 0.75f + offset, 1.5f, 0.0f });
		fig->vertices.push_back({ 0.875f + offset, 1.25f, 0.0f });
		fig->vertices.push_back({ 1.0f + offset, 1.0f, 0.0f });
		fig->vertices.push_back({ 1.0f + offset, -1.0f, 0.0f });
		fig->vertices.push_back({ 0.875f + offset, -1.25f, 0.0f });
		fig->vertices.push_back({ 0.75f + offset, -1.5f, 0.0f });
		fig->vertices.push_back({ 0.5f + offset, -1.75f, 0.0f });
		fig->vertices.push_back({ 0.0f + offset, -2.0f, 0.0f });
		fig->vertices.push_back({ -1.0f + offset, -2.0f, 0.0f });
		break;
	case 'e':
	case 'E':
		fig->vertices.push_back({ 1.0f + offset, -2.0f, 0.0f });
		fig->vertices.push_back({ -1.0f + offset, -2.0f, 0.0f });
		fig->vertices.push_back({ -1.0f + offset, 0.0f, 0.0f });
		fig->vertices.push_back({ 0.5f + offset, 0.0f, 0.0f });
		fig->vertices.push_back({ -1.0f + offset, 0.0f, 0.0f });
		fig->vertices.push_back({ -1.0f + offset, 2.0f, 0.0f });
		fig->vertices.push_back({ 1.0f + offset, 2.0f, 0.0f });
		break;
	case 'f':
	case 'F':
		fig->vertices.push_back({ -1.0f + offset, -2.0f, 0.0f });
		fig->vertices.push_back({ -1.0f + offset, 0.0f, 0.0f });
		fig->vertices.push_back({ 0.5f + offset, 0.0f, 0.0f });
		fig->vertices.push_back({ -1.0f + offset, 0.0f, 0.0f });
		fig->vertices.push_back({ -1.0f + offset, 2.0f, 0.0f });
		fig->vertices.push_back({ 1.0f + offset, 2.0f, 0.0f });
		break;
	case 'g':
	case 'G':
		fig->vertices.push_back({ 0.0f + offset, -0.5f, 0.0f });
		fig->vertices.push_back({ 1.0f + offset, -0.5f, 0.0f });
		fig->vertices.push_back({ 1.0f + offset, -2.0f, 0.0f });
		fig->vertices.push_back({ 1.0f + offset, -0.5f, 0.0f });
		fig->vertices.push_back({ 0.65f + offset, -1.5f, 0.0f });
		fig->vertices.push_back({ 0.4f + offset, -1.75f, 0.0f });
		fig->vertices.push_back({ 0.0f + offset, -2.0f, 0.0f });
		fig->vertices.push_back({ -0.5f + offset, -1.75f, 0.0f });
		fig->vertices.push_back({ -0.75f + offset, -1.5f, 0.0f });
		fig->vertices.push_back({ -0.875f + offset, -1.25f, 0.0f });
		fig->vertices.push_back({ -1.0f + offset, -1.0f, 0.0f });
		fig->vertices.push_back({ -1.0f + offset, 1.0f, 0.0f });
		fig->vertices.push_back({ -0.875f + offset, 1.25f, 0.0f });
		fig->vertices.push_back({ -0.75f + offset, 1.5f, 0.0f });
		fig->vertices.push_back({ -0.5f + offset, 1.75f, 0.0f });
		fig->vertices.push_back({ 0.0f + offset, 2.0f, 0.0f });
		fig->vertices.push_back({ 0.5f + offset, 1.75f, 0.0f });
		fig->vertices.push_back({ 0.75f + offset, 1.5f, 0.0f });
		fig->vertices.push_back({ 1.0f + offset, 1.25f, 0.0f });
		break;
	case 'h':
	case 'H':
		fig->vertices.push_back({ -1.0f + offset, -2.0f, 0.0f });
		fig->vertices.push_back({ -1.0f + offset, 0.0f, 0.0f });
		fig->vertices.push_back({ 1.0f + offset, 0.0f, 0.0f });
		fig->vertices.push_back({ 1.0f + offset, -2.0f, 0.0f });
		fig->vertices.push_back({ 1.0f + offset, 2.0f, 0.0f });
		fig->vertices.push_back({ 1.0f + offset, 0.0f, 0.0f });
		fig->vertices.push_back({ -1.0f + offset, 0.0f, 0.0f });
		fig->vertices.push_back({ -1.0f + offset, 2.0f, 0.0f });
		break;
	case 'i':
	case 'I':
		fig->vertices.push_back({ -0.5f + offset, -2.0f, 0.0f });
		fig->vertices.push_back({ 0.5f + offset, -2.0f, 0.0f });
		fig->vertices.push_back({ 0.0f + offset, -2.0f, 0.0f });
		fig->vertices.push_back({ 0.0f + offset, 2.0f, 0.0f });
		fig->vertices.push_back({ -0.5f + offset, 2.0f, 0.0f });
		fig->vertices.push_back({ 0.5f + offset, 2.0f, 0.0f });
		break;
	case 'j':
	case 'J':
		fig->vertices.push_back({ 0.0f + offset, 2.0f, 0.0f });
		fig->vertices.push_back({ 1.0f + offset, 2.0f, 0.0f });
		fig->vertices.push_back({ 0.5f + offset, 2.0f, 0.0f });
		fig->vertices.push_back({ 0.5f + offset, -1.0f, 0.0f });
		fig->vertices.push_back({ 0.3125f + offset, -1.45f, 0.0f });
		fig->vertices.push_back({ 0.125f + offset, -1.65f, 0.0f });
		fig->vertices.push_back({ -0.0625f + offset, -1.75f, 0.0f });
		fig->vertices.push_back({ -0.25f + offset, -2.0f, 0.0f });
		fig->vertices.push_back({ -0.4375f + offset, -1.75f, 0.0f });
		fig->vertices.push_back({ -0.625f + offset, -1.65f, 0.0f });
		fig->vertices.push_back({ -0.8125f + offset, -1.45f, 0.0f });
		fig->vertices.push_back({ -1.0f + offset, -1.0f, 0.0f });
		break;
	case 'k':
	case 'K':
		fig->vertices.push_back({ -1.0f + offset, -2.0f, 0.0f });
		fig->vertices.push_back({ -1.0f + offset, 2.0f, 0.0f });
		fig->vertices.push_back({ -1.0f + offset, 0.0f, 0.0f });
		fig->vertices.push_back({ 0.0f + offset, 0.0f, 0.0f });
		fig->vertices.push_back({ 1.0f + offset, 2.0f, 0.0f });
		fig->vertices.push_back({ 0.0f + offset, 0.0f, 0.0f });
		fig->vertices.push_back({ 1.0f + offset, -2.0f, 0.0f });
		break;
	case 'l':
	case 'L':
		fig->vertices.push_back({ -1.0f + offset, 2.0f, 0.0f });
		fig->vertices.push_back({ -1.0f + offset, -2.0f, 0.0f });
		fig->vertices.push_back({ 1.0f + offset, -2.0f, 0.0f });
		break;
	case 'm':
	case 'M':
		fig->vertices.push_back({ -1.0f + offset, -2.0f, 0.0f });
		fig->vertices.push_back({ -1.0f + offset, 2.0f, 0.0f });
		fig->vertices.push_back({ 0.0f + offset, 0.0f, 0.0f });
		fig->vertices.push_back({ 1.0f + offset, 2.0f, 0.0f });
		fig->vertices.push_back({ 1.0f + offset, -2.0f, 0.0f });
		break;
	case 'n':
	case 'N':
		fig->vertices.push_back({ -1.0f + offset, -2.0f, 0.0f });
		fig->vertices.push_back({ -1.0f + offset, 2.0f, 0.0f });
		fig->vertices.push_back({ 1.0f + offset, -2.0f, 0.0f });
		fig->vertices.push_back({ 1.0f + offset, 2.0f, 0.0f });
		break;
	case 'o':
	case 'O':
		fig->vertices.push_back({ 1.0f + offset, -1.25f, 0.0f });
		fig->vertices.push_back({ 0.75f + offset, -1.5f, 0.0f });
		fig->vertices.push_back({ 0.5f + offset, -1.75f, 0.0f });
		fig->vertices.push_back({ 0.0f + offset, -2.0f, 0.0f });
		fig->vertices.push_back({ -0.5f + offset, -1.75f, 0.0f });
		fig->vertices.push_back({ -0.75f + offset, -1.5f, 0.0f });
		fig->vertices.push_back({ -0.875f + offset, -1.25f, 0.0f });
		fig->vertices.push_back({ -1.0f + offset, -1.0f, 0.0f });
		fig->vertices.push_back({ -1.0f + offset, 1.0f, 0.0f });
		fig->vertices.push_back({ -0.875f + offset, 1.25f, 0.0f });
		fig->vertices.push_back({ -0.75f + offset, 1.5f, 0.0f });
		fig->vertices.push_back({ -0.5f + offset, 1.75f, 0.0f });
		fig->vertices.push_back({ 0.0f + offset, 2.0f, 0.0f });
		fig->vertices.push_back({ 0.5f + offset, 1.75f, 0.0f });
		fig->vertices.push_back({ 0.75f + offset, 1.5f, 0.0f });
		fig->vertices.push_back({ 1.0f + offset, 1.25f, 0.0f });
		fig->vertices.push_back({ 1.0f + offset, -1.25f, 0.0f });
		break;
	case 'p':
	case 'P':
		fig->vertices.push_back({ -1.0f + offset, -2.0f, 0.0f });
		fig->vertices.push_back({ -1.0f + offset, 2.0f, 0.0f });
		fig->vertices.push_back({ 0.0f + offset, 2.0f, 0.0f });
		fig->vertices.push_back({ 0.5f + offset, 1.75f, 0.0f });
		fig->vertices.push_back({ 0.75f + offset, 1.5f, 0.0f });
		fig->vertices.push_back({ 0.875f + offset, 1.25f, 0.0f });
		fig->vertices.push_back({ 1.0f + offset, 1.0f, 0.0f });
		fig->vertices.push_back({ 0.875f + offset, 0.75f, 0.0f });
		fig->vertices.push_back({ 0.75f + offset, 0.5f, 0.0f });
		fig->vertices.push_back({ 0.5f + offset, 0.25f, 0.0f });
		fig->vertices.push_back({ 0.0f + offset, 0.0f, 0.0f });
		fig->vertices.push_back({ -1.0f + offset, 0.0f, 0.0f });
		break;
	case 'q':
	case 'Q':
		fig->vertices.push_back({ 0.75f + offset, -1.5f, 0.0f });
		fig->vertices.push_back({ 0.5f + offset, -1.75f, 0.0f });
		fig->vertices.push_back({ 0.0f + offset, -2.0f, 0.0f });
		fig->vertices.push_back({ -0.5f + offset, -1.75f, 0.0f });
		fig->vertices.push_back({ -0.75f + offset, -1.5f, 0.0f });
		fig->vertices.push_back({ -0.875f + offset, -1.25f, 0.0f });
		fig->vertices.push_back({ -1.0f + offset, -1.0f, 0.0f });
		fig->vertices.push_back({ -1.0f + offset, 1.0f, 0.0f });
		fig->vertices.push_back({ -0.875f + offset, 1.25f, 0.0f });
		fig->vertices.push_back({ -0.75f + offset, 1.5f, 0.0f });
		fig->vertices.push_back({ -0.5f + offset, 1.75f, 0.0f });
		fig->vertices.push_back({ 0.0f + offset, 2.0f, 0.0f });
		fig->vertices.push_back({ 0.5f + offset, 1.75f, 0.0f });
		fig->vertices.push_back({ 0.75f + offset, 1.5f, 0.0f });
		fig->vertices.push_back({ 1.0f + offset, 1.25f, 0.0f });
		fig->vertices.push_back({ 1.0f + offset, -1.25f, 0.0f });
		fig->vertices.push_back({ 0.75f + offset, -1.5f, 0.0f });
		fig->vertices.push_back({ 1.0f + offset, -2.0f, 0.0f });
		fig->vertices.push_back({ 0.25f + offset, -1.0f, 0.0f });
		break;
	case 'r':
	case 'R':
		fig->vertices.push_back({ -1.0f + offset, -2.0f, 0.0f });
		fig->vertices.push_back({ -1.0f + offset, 2.0f, 0.0f });
		fig->vertices.push_back({ 0.0f + offset, 2.0f, 0.0f });
		fig->vertices.push_back({ 0.5f + offset, 1.75f, 0.0f });
		fig->vertices.push_back({ 0.75f + offset, 1.5f, 0.0f });
		fig->vertices.push_back({ 0.875f + offset, 1.25f, 0.0f });
		fig->vertices.push_back({ 1.0f + offset, 1.0f, 0.0f });
		fig->vertices.push_back({ 0.875f + offset, 0.75f, 0.0f });
		fig->vertices.push_back({ 0.75f + offset, 0.5f, 0.0f });
		fig->vertices.push_back({ 0.5f + offset, 0.25f, 0.0f });
		fig->vertices.push_back({ 0.0f + offset, 0.0f, 0.0f });
		fig->vertices.push_back({ -1.0f + offset, 0.0f, 0.0f });
		fig->vertices.push_back({ 0.0f + offset, 0.0f, 0.0f });
		fig->vertices.push_back({ 1.0f + offset, -2.0f, 0.0f });
		break;
	case 's':
	case 'S':
		fig->vertices.push_back({ -1.0f + offset, -2.0f, 0.0f });
		fig->vertices.push_back({ 0.0f + offset, -2.0f, 0.0f });
		fig->vertices.push_back({ 0.5f + offset, -1.75f, 0.0f });
		fig->vertices.push_back({ 0.75f + offset, -1.5f, 0.0f });
		fig->vertices.push_back({ 0.875f + offset, -1.25f, 0.0f });
		fig->vertices.push_back({ 1.0f + offset, -1.0f, 0.0f });
		fig->vertices.push_back({ 0.875f + offset, -0.75f, 0.0f });
		fig->vertices.push_back({ 0.75f + offset, -0.5f, 0.0f });
		fig->vertices.push_back({ 0.5f + offset, -0.25f, 0.0f });
		fig->vertices.push_back({ 0.0f + offset, 0.0f, 0.0f });
		fig->vertices.push_back({ -0.5f + offset, 0.25f, 0.0f });
		fig->vertices.push_back({ -0.75f + offset, 0.5f, 0.0f });
		fig->vertices.push_back({ -0.875f + offset, 0.75f, 0.0f });
		fig->vertices.push_back({ -1.0f + offset, 1.0f, 0.0f });
		fig->vertices.push_back({ -0.875f + offset, 1.25f, 0.0f });
		fig->vertices.push_back({ -0.75f + offset, 1.5f, 0.0f });
		fig->vertices.push_back({ -0.5f + offset, 1.75f, 0.0f });
		fig->vertices.push_back({ 0.0f + offset, 2.0f, 0.0f });
		fig->vertices.push_back({ 1.0f + offset, 2.0f, 0.0f });
		break;
	case 't':
	case 'T':
		fig->vertices.push_back({ 0.0f + offset, -2.0f, 0.0f });
		fig->vertices.push_back({ 0.0f + offset, 2.0f, 0.0f });
		fig->vertices.push_back({ -1.0f + offset, 2.0f, 0.0f });
		fig->vertices.push_back({ 1.0f + offset, 2.0f, 0.0f });
		break;
	case 'u':
	case 'U':
		fig->vertices.push_back({ -1.0f + offset, 2.0f, 0.0f });
		fig->vertices.push_back({ -1.0f + offset, -1.0f, 0.0f });
		fig->vertices.push_back({ -0.875f + offset, -1.25f, 0.0f });
		fig->vertices.push_back({ -0.75f + offset, -1.75f, 0.0f });
		fig->vertices.push_back({ -0.45f + offset, -2.0f, 0.0f });
		fig->vertices.push_back({ 0.0f + offset, -2.0f, 0.0f });
		fig->vertices.push_back({ 0.45f + offset, -2.0f, 0.0f });
		fig->vertices.push_back({ 0.75f + offset, -1.75f, 0.0f });
		fig->vertices.push_back({ 0.875f + offset, -1.25f, 0.0f });
		fig->vertices.push_back({ 1.0f + offset, -1.0f, 0.0f });
		fig->vertices.push_back({ 1.0f + offset, 2.0f, 0.0f });
		break;
	case 'v':
	case 'V':
		fig->vertices.push_back({ -1.0f + offset, 2.0f, 0.0f });
		fig->vertices.push_back({ 0.0f + offset, -2.0f, 0.0f });
		fig->vertices.push_back({ 1.0f + offset, 2.0f, 0.0f });
		break;
	case 'w':
	case 'W':
		fig->vertices.push_back({ -1.0f + offset, 2.0f, 0.0f });
		fig->vertices.push_back({ -0.5f + offset, -2.0f, 0.0f });
		fig->vertices.push_back({ 0.0f + offset, 2.0f, 0.0f });
		fig->vertices.push_back({ 0.5f + offset, -2.0f, 0.0f });
		fig->vertices.push_back({ 1.0f + offset, 2.0f, 0.0f });
		break;
	case 'x':
	case 'X':
		fig->vertices.push_back({ -1.0f + offset, 2.0f, 0.0f });
		fig->vertices.push_back({ 1.0f + offset, -2.0f, 0.0f });
		fig->vertices.push_back({ 0.0f + offset, 0.0f, 0.0f });
		fig->vertices.push_back({ -1.0f + offset, -2.0f, 0.0f });
		fig->vertices.push_back({ 1.0f + offset, 2.0f, 0.0f });
		break;
	case 'y':
	case 'Y':
		fig->vertices.push_back({ -1.0f + offset, 2.0f, 0.0f });
		fig->vertices.push_back({ 0.0f + offset, 0.0f, 0.0f });
		fig->vertices.push_back({ 1.0f + offset, 2.0f, 0.0f });
		fig->vertices.push_back({ 0.0f + offset, 0.0f, 0.0f });
		fig->vertices.push_back({ 0.0f + offset, -2.0f, 0.0f });
		break;
	case 'z':
	case 'Z':
		fig->vertices.push_back({ -1.0f + offset, 2.0f, 0.0f });
		fig->vertices.push_back({ 1.0f + offset, 2.0f, 0.0f });
		fig->vertices.push_back({ -1.0f + offset, -2.0f, 0.0f });
		fig->vertices.push_back({ 1.0f + offset, -2.0f, 0.0f });
		break;
	case '0':
		fig->vertices.push_back({ 1.0f + offset, 1.25f, 0.0f });
		fig->vertices.push_back({ 1.0f + offset, -1.25f, 0.0f });
		fig->vertices.push_back({ 0.75f + offset, -1.5f, 0.0f });
		fig->vertices.push_back({ 0.5f + offset, -1.75f, 0.0f });
		fig->vertices.push_back({ 0.0f + offset, -2.0f, 0.0f });
		fig->vertices.push_back({ -0.5f + offset, -1.75f, 0.0f });
		fig->vertices.push_back({ -0.75f + offset, -1.5f, 0.0f });
		fig->vertices.push_back({ -0.875f + offset, -1.25f, 0.0f });
		fig->vertices.push_back({ -1.0f + offset, -1.0f, 0.0f });
		fig->vertices.push_back({ 1.0f + offset, 1.0f, 0.0f });
		fig->vertices.push_back({ 1.0f + offset, 1.25f, 0.0f });
		fig->vertices.push_back({ 0.75f + offset, 1.5f, 0.0f });
		fig->vertices.push_back({ 0.5f + offset, 1.75f, 0.0f });
		fig->vertices.push_back({ 0.0f + offset, 2.0f, 0.0f });
		fig->vertices.push_back({ -0.5f + offset, 1.75f, 0.0f });
		fig->vertices.push_back({ -0.75f + offset, 1.5f, 0.0f });
		fig->vertices.push_back({ -0.875f + offset, 1.25f, 0.0f });
		fig->vertices.push_back({ -1.0f + offset, 1.0f, 0.0f });
		fig->vertices.push_back({ -1.0f + offset, -1.0f, 0.0f });
		break;
	case '1':
		fig->vertices.push_back({ -0.75f + offset, -2.0f, 0.0f });
		fig->vertices.push_back({ 0.75f + offset, -2.0f, 0.0f });
		fig->vertices.push_back({ 0.0f + offset, -2.0f, 0.0f });
		fig->vertices.push_back({ 0.0f + offset, 2.0f, 0.0f });
		fig->vertices.push_back({ -0.85f + offset, 1.5f, 0.0f });
		break;
	case '2':
		fig->vertices.push_back({ -1.0f + offset, 1.0f, 0.0f });
		fig->vertices.push_back({ -0.875f + offset, 1.25f, 0.0f });
		fig->vertices.push_back({ -0.75f + offset, 1.5f, 0.0f });
		fig->vertices.push_back({ -0.5f + offset, 1.75f, 0.0f });
		fig->vertices.push_back({ 0.0f + offset, 2.0f, 0.0f });
		fig->vertices.push_back({ 0.5f + offset, 1.75f, 0.0f });
		fig->vertices.push_back({ 0.75f + offset, 1.5f, 0.0f });
		fig->vertices.push_back({ 0.875f + offset, 1.25f, 0.0f });
		fig->vertices.push_back({ 1.0f + offset, 1.0f, 0.0f });
		fig->vertices.push_back({ -1.0f + offset, -2.0f, 0.0f });
		fig->vertices.push_back({ 1.0f + offset, -2.0f, 0.0f });
		break;
	case '3':
		fig->vertices.push_back({ -1.0f + offset, 1.0f, 0.0f });
		fig->vertices.push_back({ -0.875f + offset, 1.25f, 0.0f });
		fig->vertices.push_back({ -0.75f + offset, 1.5f, 0.0f });
		fig->vertices.push_back({ -0.5f + offset, 1.75f, 0.0f });
		fig->vertices.push_back({ 0.0f + offset, 2.0f, 0.0f });
		fig->vertices.push_back({ 0.5f + offset, 1.75f, 0.0f });
		fig->vertices.push_back({ 0.75f + offset, 1.5f, 0.0f });
		fig->vertices.push_back({ 0.875f + offset, 1.25f, 0.0f });
		fig->vertices.push_back({ 1.0f + offset, 1.0f, 0.0f });
		fig->vertices.push_back({ 0.875f + offset, 0.75f, 0.0f });
		fig->vertices.push_back({ 0.75f + offset, 0.5f, 0.0f });
		fig->vertices.push_back({ 0.5f + offset, 0.25f, 0.0f });
		fig->vertices.push_back({ 0.0f + offset, 0.0f, 0.0f });
		fig->vertices.push_back({ 0.5f + offset, -0.25f, 0.0f });
		fig->vertices.push_back({ 0.75f + offset, -0.5f, 0.0f });
		fig->vertices.push_back({ 0.875f + offset, -0.75f, 0.0f });
		fig->vertices.push_back({ 1.0f + offset, -1.0f, 0.0f });
		fig->vertices.push_back({ 0.875f + offset, -1.25f, 0.0f });
		fig->vertices.push_back({ 0.75f + offset, -1.5f, 0.0f });
		fig->vertices.push_back({ 0.5f + offset, -1.75f, 0.0f });
		fig->vertices.push_back({ 0.0f + offset, -2.0f, 0.0f });
		fig->vertices.push_back({ -0.5f + offset, -1.75f, 0.0f });
		fig->vertices.push_back({ -0.75f + offset, -1.5f, 0.0f });
		fig->vertices.push_back({ -0.875f + offset, -1.25f, 0.0f });
		fig->vertices.push_back({ -1.0f + offset, -1.0f, 0.0f });
		break;
	case '4':
		fig->vertices.push_back({ -0.5f + offset, 2.0f, 0.0f });
		fig->vertices.push_back({ -0.75f + offset, 0.0f, 0.0f });
		fig->vertices.push_back({ 1.0f + offset, 0.0f, 0.0f });
		fig->vertices.push_back({ 0.5f + offset, 0.0f, 0.0f });
		fig->vertices.push_back({ 0.5f + offset, 2.0f, 0.0f });
		fig->vertices.push_back({ 0.5f + offset, -2.0f, 0.0f });
		break;
	case '5':
		fig->vertices.push_back({ 1.0f + offset, 2.0f, 0.0f });
		fig->vertices.push_back({ -1.0f + offset, 2.0f, 0.0f });
		fig->vertices.push_back({ -1.0f + offset, 0.0f, 0.0f });

		fig->vertices.push_back({ -0.875f + offset, 0.1f, 0.0f });
		fig->vertices.push_back({ -0.75f + offset, 0.2f, 0.0f });
		fig->vertices.push_back({ -0.5f + offset, 0.3f, 0.0f });
		fig->vertices.push_back({ 0.0f + offset, 0.4f, 0.0f });
		fig->vertices.push_back({ 0.5f + offset, 0.2f, 0.0f });
		fig->vertices.push_back({ 0.75f + offset, 0.0f, 0.0f });
		fig->vertices.push_back({ 0.875f + offset, -0.2f, 0.0f });
		fig->vertices.push_back({ 1.0f + offset, -0.4f, 0.0f });

		fig->vertices.push_back({ 1.0f + offset, -0.6f, 0.0f });
		fig->vertices.push_back({ 0.9f + offset, -0.8f, 0.0f });
		fig->vertices.push_back({ 0.8f + offset, -1.0f, 0.0f });
		fig->vertices.push_back({ 0.6f + offset, -1.4f, 0.0f });
		fig->vertices.push_back({ 0.3f + offset, -1.7f, 0.0f });
		fig->vertices.push_back({ 0.0f + offset, -1.8f, 0.0f });
		fig->vertices.push_back({ -0.5f + offset, -1.75f, 0.0f });
		fig->vertices.push_back({ -0.75f + offset, -1.5f, 0.0f });
		fig->vertices.push_back({ -0.875f + offset, -1.25f, 0.0f });
		fig->vertices.push_back({ -1.0f + offset, -1.0f, 0.0f });
		break;
	case '6':
		fig->vertices.push_back({ -0.9f + offset, -0.75f, 0.0f });
		fig->vertices.push_back({ -0.85f + offset, -0.5f, 0.0f });
		fig->vertices.push_back({ -0.5f + offset, -0.25f, 0.0f });
		fig->vertices.push_back({ 0.0f + offset, 0.0f, 0.0f });
		fig->vertices.push_back({ 0.5f + offset, -0.25f, 0.0f });
		fig->vertices.push_back({ 0.75f + offset, -0.5f, 0.0f });
		fig->vertices.push_back({ 0.875f + offset, -0.75f, 0.0f });
		fig->vertices.push_back({ 1.0f + offset, -1.0f, 0.0f });
		fig->vertices.push_back({ 0.875f + offset, -1.25f, 0.0f });
		fig->vertices.push_back({ 0.75f + offset, -1.5f, 0.0f });
		fig->vertices.push_back({ 0.5f + offset, -1.75f, 0.0f });
		fig->vertices.push_back({ 0.0f + offset, -2.0f, 0.0f });
		fig->vertices.push_back({ -0.5f + offset, -1.75f, 0.0f });
		fig->vertices.push_back({ -0.75f + offset, -1.5f, 0.0f });
		fig->vertices.push_back({ -0.875f + offset, -1.25f, 0.0f });
		fig->vertices.push_back({ -1.0f + offset, -1.0f, 0.0f });
		fig->vertices.push_back({ -0.9f + offset, -0.75f, 0.0f });
		fig->vertices.push_back({ 0.4f + offset, 2.0f, 0.0f });
		break;
	case '7':
		fig->vertices.push_back({ -1.0f + offset, 2.0f, 0.0f });
		fig->vertices.push_back({ 1.0f + offset, 2.0f, 0.0f });
		fig->vertices.push_back({ -0.5f + offset, -2.0f, 0.0f });
		break;
	case '8':
		fig->vertices.push_back({ 0.0f + offset, 0.0f, 0.0f });
		fig->vertices.push_back({ 0.5f + offset, -0.25f, 0.0f });
		fig->vertices.push_back({ 0.75f + offset, -0.5f, 0.0f });
		fig->vertices.push_back({ 0.875f + offset, -0.75f, 0.0f });
		fig->vertices.push_back({ 1.0f + offset, -1.0f, 0.0f });
		fig->vertices.push_back({ 0.875f + offset, -1.25f, 0.0f });
		fig->vertices.push_back({ 0.75f + offset, -1.5f, 0.0f });
		fig->vertices.push_back({ 0.5f + offset, -1.75f, 0.0f });
		fig->vertices.push_back({ 0.0f + offset, -2.0f, 0.0f });
		fig->vertices.push_back({ -0.5f + offset, -1.75f, 0.0f });
		fig->vertices.push_back({ -0.75f + offset, -1.5f, 0.0f });
		fig->vertices.push_back({ -0.875f + offset, -1.25f, 0.0f });
		fig->vertices.push_back({ -1.0f + offset, -1.0f, 0.0f });
		fig->vertices.push_back({ -0.9f + offset, -0.75f, 0.0f });
		fig->vertices.push_back({ -0.85f + offset, -0.5f, 0.0f });
		fig->vertices.push_back({ -0.5f + offset, -0.25f, 0.0f });
		fig->vertices.push_back({ 0.0f + offset, 0.0f, 0.0f });
		fig->vertices.push_back({ 0.5f + offset, 0.25f, 0.0f });
		fig->vertices.push_back({ 0.75f + offset, 0.5f, 0.0f });
		fig->vertices.push_back({ 0.875f + offset, 0.75f, 0.0f });
		fig->vertices.push_back({ 1.0f + offset, 1.0f, 0.0f });
		fig->vertices.push_back({ 0.875f + offset, 1.25f, 0.0f });
		fig->vertices.push_back({ 0.75f + offset, 1.5f, 0.0f });
		fig->vertices.push_back({ 0.5f + offset, 1.75f, 0.0f });
		fig->vertices.push_back({ 0.0f + offset, 2.0f, 0.0f });
		fig->vertices.push_back({ -0.5f + offset, 1.75f, 0.0f });
		fig->vertices.push_back({ -0.75f + offset, 1.5f, 0.0f });
		fig->vertices.push_back({ -0.875f + offset, 1.25f, 0.0f });
		fig->vertices.push_back({ -1.0f + offset, 1.0f, 0.0f });
		fig->vertices.push_back({ -0.9f + offset, 0.75f, 0.0f });
		fig->vertices.push_back({ -0.85f + offset, 0.5f, 0.0f });
		fig->vertices.push_back({ -0.5f + offset, 0.25f, 0.0f });
		fig->vertices.push_back({ 0.0f + offset, 0.0f, 0.0f });
		break;
	case '9':
		fig->vertices.push_back({ 0.75f + offset, 0.5f, 0.0f });
		fig->vertices.push_back({ 0.875f + offset, 0.75f, 0.0f });
		fig->vertices.push_back({ 1.0f + offset, 1.0f, 0.0f });
		fig->vertices.push_back({ 0.875f + offset, 1.25f, 0.0f });
		fig->vertices.push_back({ 0.75f + offset, 1.5f, 0.0f });
		fig->vertices.push_back({ 0.5f + offset, 1.75f, 0.0f });
		fig->vertices.push_back({ 0.0f + offset, 2.0f, 0.0f });
		fig->vertices.push_back({ -0.5f + offset, 1.75f, 0.0f });
		fig->vertices.push_back({ -0.75f + offset, 1.5f, 0.0f });
		fig->vertices.push_back({ -0.875f + offset, 1.25f, 0.0f });
		fig->vertices.push_back({ -1.0f + offset, 1.0f, 0.0f });
		fig->vertices.push_back({ -0.9f + offset, 0.75f, 0.0f });
		fig->vertices.push_back({ -0.85f + offset, 0.5f, 0.0f });
		fig->vertices.push_back({ -0.5f + offset, 0.25f, 0.0f });
		fig->vertices.push_back({ 0.5f + offset, 0.25f, 0.0f });
		fig->vertices.push_back({ 0.75f + offset, 0.5f, 0.0f });
		fig->vertices.push_back({ -0.6f + offset, -2.0f, 0.0f });
		break;
	case ',':
		fig->vertices.push_back({ -0.5f + offset, -2.0f, 0.0f });
		fig->vertices.push_back({ -0.8f + offset, -2.8f, 0.0f });
		break;
	case '.':
		fig->drawMode = GL_POINTS;
		fig->vertices.push_back({ -0.5f + offset, -2.0f, 0.0f });
		break;
	case ':':
		fig->drawMode = GL_POINTS;
		fig->vertices.push_back({ -0.5f + offset, 1.0f, 0.0f });
		fig->vertices.push_back({ -0.5f + offset, -1.0f, 0.0f });
		break;
	case '>':
	case '<':
	case '=':
	case '-':
	default:
		fig->vertices.push_back({ -0.5f + offset, 0.0f, 0.0f });
		fig->vertices.push_back({ 0.5f + offset, 0.0f, 0.0f });
		break;
	}

	for (int i = 0; i < fig->vertices.size(); i++)
	{
		fig->colors.push_back(color);
	}
}

Text createText(float posx, float posy, bool center, float scale, bool visibility, const char* message)
{
	float textWidth = strlen(message) * 1.35f * scale;

	Text text = {};
	text.pos.x = center ? posx - textWidth : posx;
	text.pos.y = posy;
	text.scale = scale;
	text.visible = visibility;

	for (int i = 0; i < strlen(message); i++)
	{
		Figure letter = {};
		letter.widthLines = scale / 3;
		letter.sizePoints = scale / 1.5f;
		addLetter(&letter, { 1.0f, 1.0f, 1.0f, 1.0f }, message[i], i * 3);
		createFigureVAO(&letter);
		text.figures.push_back(letter);
	}

	return text;
}

void updateText(Text* text, char* newMessage)
{
	float widthLines = text->figures.at(0).widthLines;
	float sizePoints = text->figures.at(0).sizePoints;
	text->figures.clear();

	for (int i = 0; i < strlen(newMessage); i++)
	{
		Figure letter = {};
		letter.widthLines = widthLines;
		letter.sizePoints = sizePoints;
		addLetter(&letter, { 1.0f, 1.0f, 1.0f, 1.0f }, newMessage[i], i * 3);
		createFigureVAO(&letter);
		text->figures.push_back(letter);
	}
}
