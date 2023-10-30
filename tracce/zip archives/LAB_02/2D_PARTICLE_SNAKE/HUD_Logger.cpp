#include "HUD_Logger.h"
#include <GL/glew.h>
#include <GL/freeglut.h>


using namespace std;

extern float width;
extern float height;

HUD_Logger* HUD_Logger::logger;
bool HUD_Logger::ready;

void stroke_char_output(int x, int y, int z, std::string s, void * font)
{
	int i;
	glRasterPos3f(x, y, 0);
	for (i = 0; i < s.length(); i++)
		glutStrokeCharacter(font, s[i]);
}

HUD_Logger::HUD_Logger() {
}

HUD_Logger* HUD_Logger::get()
{
	if (!ready) {
		logger =  new HUD_Logger();
		ready = true;
	}
	return logger;
}

void HUD_Logger::printInfo(std::vector<std::string> lines)
{
	int total_width = 0, total_height = glutStrokeHeight(GLUT_STROKE_ROMAN);

	for (unsigned int j = 0; j < lines[0].length(); j++)
	{
		total_width += glutStrokeWidth(GLUT_STROKE_ROMAN, lines[0][j]);
	}
	glPushMatrix();
		glTranslatef(10.0, height - (total_height * 0.2) - 20.0, 0.0);
		glScalef(0.2, 0.2, 1.0);
		glColor4f(1.0, 1.0, 1.0, 1.0);
		glLineWidth(3.0f);
		stroke_char_output(0.0, 0.0, 0.0, lines[0], GLUT_STROKE_ROMAN);
	glPopMatrix();
	
	total_width = 0;

	for (unsigned int j = 0; j < lines[1].length(); j++)
	{
		total_width += glutStrokeWidth(GLUT_STROKE_ROMAN, lines[1][j]);
	}
	glPushMatrix();
		glTranslatef(width - (total_width * 0.2) - 20.0 , height - (total_height * 0.2) - 20.0, 0.0);
		glScalef(0.2, 0.2, 1.0);
		glColor4f(1.0, 1.0, 1.0, 1.0);
		glLineWidth(3.0f);
		stroke_char_output(0.0, 0.0, 0.0, lines[1], GLUT_STROKE_ROMAN);
	glPopMatrix();

	if (lines.size() > 2) {
		total_width = 0;
		total_height = glutStrokeHeight(GLUT_STROKE_ROMAN);

		for (unsigned int j = 0; j < lines[2].length(); j++)
		{
			total_width += glutStrokeWidth(GLUT_STROKE_ROMAN, lines[2][j]);
		}
		glPushMatrix();
		glTranslatef(10.0, height/2 , 0.0);
		glScalef(0.32, 0.32, 0.32);
		glColor4f(1.0, 1.0, 1.0, 1.0);
		glLineWidth(5.0f);
		stroke_char_output(0.0, 0.0, 0.0, lines[2], GLUT_STROKE_ROMAN);
		glPopMatrix();

		total_width = 0;
		total_height = glutStrokeHeight(GLUT_STROKE_ROMAN);

		string restart = "Press ' r ' to restart";
		for (unsigned int j = 0; j < restart.length(); j++)
		{
			total_width += glutStrokeWidth(GLUT_STROKE_ROMAN, restart[j]);
		}
		glPushMatrix();
		glTranslatef(10.0, height / 2 - 60 , 0.0);
		glScalef(0.32, 0.32, 0.32);
		glColor4f(1.0, 1.0, 1.0, 1.0);
		glLineWidth(5.0f);
		stroke_char_output(0.0, 0.0, 0.0, restart, GLUT_STROKE_ROMAN);
		glPopMatrix();



	}
	
	

}


