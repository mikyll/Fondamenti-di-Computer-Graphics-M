#include <cmath>
#include <stdlib.h>
#include <time.h>
#include <GL\glew.h>
#include <GL\freeglut.h>
#include <vector>

#include "HUD_Logger.h"
#include "ShaderMaker.h"
#include <glm/fwd.hpp>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <string>

static unsigned int programId;
#define PI 3.14159265358979323846

unsigned int VAO, VAO_O, VAO_BACK;
unsigned int VBO, VBO_O, VBO_BACK, MatProj, MatModel;

using namespace glm;
mat4 Projection;
mat4 Model;

typedef struct {
	float r;
	float g;
	float b;
} color;


typedef struct {
	float x;
	float y;
	float alpha;
	float xFactor;
	float yFactor;
	float drag;
	color color;
} PARTICLE;


vector <PARTICLE> particles;

typedef struct { float x, y, r, g, b, a; } Point;
int nPoint = 5000;
Point* Punti = new Point[nPoint];

int vertices_back = 12;
Point* Back = new Point[vertices_back];

float width = 1920;
float height = 1080;

float	posx = float(width) / 2;
float	posy = float(height) / 2;

bool pressing_left = false;
bool pressing_right = false;
bool pressing_up = false;
bool pressing_down = false;

double	horizontal_speed = 0;
double	vertical_speed = 0;
double	acceleration = 2.0;

int game_points = 0;
int object_count = 0;
float object_factor_x;
float object_factor_y;
float hitbox = 100.0;
float obj_posx = 0;
float obj_posy = 0;
float obj_rel_posx = 0;
float obj_rel_posy = 0;
time_t timer;
int time_end;
float psych = 0.0;

int tposx = 0;
int tposy = 0;

bool hit = false;
bool win_flag = false;

vec4 color_top_pre = { 0.0, 0.0, 0.0, 1.0 };
vec4 color_bot_pre = { 1.0, 1.0, 1.0, 1.0 };

vec4 color_top_post = color_bot_pre;
vec4 color_bot_post = color_top_pre;

int nTriangles_obj = 30;
int vertices_obj = 3 * 2 * nTriangles_obj;
Point* Obj = new Point[vertices_obj];

void setup() {
	float r = 0.0, g = 0.0, b = 0.0;
	glClearColor(r, g, b, 1.0);
	glClear(GL_COLOR_BUFFER_BIT);
}

void resize(int w, int h)
{
	glViewport(0, 0, w, h);
	setup();
}

color computeRainbow() {
	static float rgb[3] = { 1.0, 0.0, 0.0 };
	static int fase = 0, counter = 0;
	const float step = 0.1;
	color paint;

	switch (fase) {
	case 0: rgb[1] += step;
		break;
	case 1: rgb[0] -= step;
		break;
	case 2: rgb[2] += step;
		break;
	case 3: rgb[1] -= step;
		break;
	case 4: rgb[0] += step;
		break;
	case 5: rgb[2] -= step;
		break;
	default:
		break;
	}

	counter++;
	if (counter > 1.0 / step) {
		counter = 0;
		fase < 5 ? fase++ : fase = 0;
	}

	paint.r = rgb[0];
	paint.g = rgb[1];
	paint.b = rgb[2];
	return paint;
}

void createObject() {

	object_count = 1;

	obj_posx = (rand() % ((int)width - (2 * (int)hitbox))) + hitbox;
	obj_posy = (rand() % ((int)height - (2 * (int)hitbox))) + hitbox;

	obj_posx = (hitbox / width) + ((obj_posx - hitbox) * ((width - 2 * hitbox) / width) / (width - hitbox));
	obj_posy = (hitbox / height) + ((obj_posy - hitbox) * ((height - 2 * hitbox) / height) / (height - hitbox));
}

void mouseClick(int button, int state, int x, int y) {

	float xPos = ((float)x) / ((float)(width));
	float yPos = ((float)y) / ((float)(height));

	float offset = 1.25;

	switch (button)
	{
	case GLUT_LEFT_BUTTON:
		if ((xPos * (width * offset) < obj_posx * width + hitbox)
			&& (xPos * (width * offset) > obj_posx * width - hitbox)
			&& (yPos * (height * offset) < (1 - obj_posy) * height + hitbox)
			&& (yPos * (height * offset) > (1 - obj_posy) * height - hitbox)) {
			hit = true;
		}
		else {
			hit = false;
		}
		break;
	}
}

void keyMotion(unsigned char key, int x, int y) {
	switch (key)
	{
	case 'a':
		pressing_left = true;
		break;
	case 'd':
		pressing_right = true;
		break;
	case 'w':
		pressing_up = true;
		break;
	case 's':
		pressing_down = true;
		break;
	case 'r':
		if (win_flag) {
			game_points = 0;
			object_count = 0;
			hitbox = 100.0;
			//posx = 0;
			//posy = 0;
			createObject();
			timer = time(NULL);
			hit = false;
			win_flag = false;
		}
		//else {
		//	win_flag = true;
		//	hitbox = 0;
		//}
		break;
	case 27:
		exit(0);
		break;
	default:
		break;
	}
}
void keyMotionReleased(unsigned char key, int x, int y) {
	switch (key)
	{
	case 'a':
		pressing_left = false;
		break;
	case 'd':
		pressing_right = false;
		break;
	case 'w':
		pressing_up = false;
		break;
	case 's':
		pressing_down = false;
		break;

	case 27:
		exit(0);
		break;
	default:
		break;
	}
}

void createParticles() {
	color rgb = computeRainbow();
	for (int i = 0; i < 20; i++) {
		PARTICLE p;
		p.x = posx;
		p.y = height - posy;
		p.alpha = 1.0;
		p.drag = 1.25;
		p.xFactor = (rand() % 1000 + 1) / 300 * (rand() % 2 == 0 ? -1 : 1);
		p.yFactor = (rand() % 1000 + 1) / 300 * (rand() % 2 == 0 ? -1 : 1);
		p.color.r = rgb.r;
		p.color.g = rgb.g;
		p.color.b = rgb.b;
		particles.push_back(p);
	}
};


void init_obj(int nTriangles, Point* Obj) {

	createObject();

	int i;
	float stepA = (2 * PI) / nTriangles;

	vec4 color_top = color_top_pre;
	vec4 color_bot = color_top_pre;

	int comp = 0;
	for (i = 0; i < nTriangles; i++)
	{
		Obj[comp].x = cos((double)i * stepA);
		Obj[comp].y = sin((double)i * stepA);
		Obj[comp].r = color_top.r; Obj[comp].g = color_top.g; Obj[comp].b = color_top.b; Obj[comp].a = color_top.a;

		Obj[comp + 1].x = cos((double)(i + 1) * stepA);
		Obj[comp + 1].y = sin((double)(i + 1) * stepA);
		Obj[comp + 1].r = color_top.r; Obj[comp + 1].g = color_top.g; Obj[comp + 1].b = color_top.b; Obj[comp + 1].a = color_top.a;

		Obj[comp + 2].x = 0.0;
		Obj[comp + 2].y = 0.0;
		Obj[comp + 2].r = color_bot.r; Obj[comp + 2].g = color_bot.g; Obj[comp + 2].b = color_bot.b; Obj[comp + 2].a = color_bot.a;
		comp += 3;
	}
}

void change_color(int nTriangles, Point* Obj) {

	vec4 color_top = color_top_pre;
	vec4 color_bot = color_bot_pre;

	if (hit) {
		color_top = color_top_post;
		color_bot = color_bot_post;
	}
	else {
		color_top = color_top_pre;
		color_bot = color_bot_pre;
	}

	int comp = 0;
	for (int i = 0; i < nTriangles; i++)
	{
		Obj[comp].r = color_top.r; Obj[comp].g = color_top.g; Obj[comp].b = color_top.b; Obj[comp].a = color_top.a;
		Obj[comp + 1].r = color_top.r; Obj[comp + 1].g = color_top.g; Obj[comp + 1].b = color_top.b; Obj[comp + 1].a = color_top.a;
		Obj[comp + 2].r = color_bot.r; Obj[comp + 2].g = color_bot.g; Obj[comp + 2].b = color_bot.b; Obj[comp + 2].a = color_bot.a;
		comp += 3;
	}
}

void draw_back(float x, float y, float width, float height, Point* Back)
{
	vec4 color_top = { 0.0, 0.0, 0.0, 0.8 };
	vec4 color_bot = { abs(cos(psych)) - 0.5, abs(sin(psych)) - 0.5, abs(sin(psych)) - 0.5, 0.8 };
	vec4 color_bot2 = { abs(cos(psych * 2.37)) - 0.2, abs(cos(psych * 2.37)) - 0.2, abs(sin(psych * 2.37)) - 0.2,  0.8 };
	//color_bot2 = color_bot; //per un effetto diverso

	Back[0].x = x;	Back[0].y = y; 
	Back[0].r = color_bot.r; Back[0].g = color_bot.g; Back[0].b = color_bot.b; Back[0].a = color_bot.a;
	Back[1].x = x + width / 2;	Back[1].y = y + height / 2;	
	Back[1].r = color_top.r; Back[1].g = color_top.g; Back[1].b = color_top.b; Back[1].a = color_top.a;
	Back[2].x = x + width;	Back[2].y = y; 
	Back[2].r = color_bot.r; Back[2].g = color_bot.g; Back[2].b = color_bot.b; Back[2].a = color_bot.a;

	Back[3].x = x + width;	Back[3].y = y; 
	Back[3].r = color_bot2.r; Back[3].g = color_bot2.g; Back[3].b = color_bot2.b; Back[3].a = color_bot2.a;
	Back[4].x = x + width / 2;	Back[4].y = y + height / 2; 
	Back[4].r = color_top.r; Back[4].g = color_top.g; Back[4].b = color_top.b; Back[4].a = color_top.a;
	Back[5].x = x + width;	Back[5].y = y + height; 
	Back[5].r = color_bot2.r; Back[5].g = color_bot2.g; Back[5].b = color_bot2.b; Back[5].a = color_bot2.a;

	Back[6].x = x + width;	Back[6].y = y + height; 
	Back[6].r = color_bot.r; Back[6].g = color_bot.g; Back[6].b = color_bot.b; Back[6].a = color_bot.a;
	Back[7].x = x + width / 2;	Back[7].y = y + height / 2;	
	Back[7].r = color_top.r; Back[7].g = color_top.g; Back[7].b = color_top.b; Back[7].a = color_top.a;
	Back[8].x = x;	Back[8].y = y + height; 
	Back[8].r = color_bot.r; Back[8].g = color_bot.g; Back[8].b = color_bot.b; Back[8].a = color_bot.a;

	Back[9].x = x;	Back[9].y = y + height; 
	Back[9].r = color_bot2.r; Back[9].g = color_bot2.g; Back[9].b = color_bot2.b; Back[9].a = color_bot2.a;
	Back[10].x = x + width / 2;	Back[10].y = y + height / 2; 
	Back[10].r = color_top.r; Back[10].g = color_top.g; Back[10].b = color_top.b; Back[10].a = color_top.a;
	Back[11].x = x;	Back[11].y = y; 
	Back[11].r = color_bot2.r; Back[11].g = color_bot2.g; Back[11].b = color_bot2.b; Back[11].a = color_bot2.a;
}

void changeColorBack(Point* Back) {

	vec4 color_top = { 0.0, 0.0, 0.0, 0.8 };
	vec4 color_bot = { abs(cos(psych)) - 0.5, abs(sin(psych)) - 0.5, abs(sin(psych)) - 0.5, 0.8 };
	vec4 color_bot2 = { abs(cos(psych * 2.37)) - 0.2, abs(cos(psych * 2.37)) - 0.2, abs(sin(psych * 2.37)) - 0.2,  0.8 };
	//color_bot2 = color_bot; //per un effetto diverso

	Back[0].r = color_bot.r; Back[0].g = color_bot.g; Back[0].b = color_bot.b; Back[0].a = color_bot.a;
	Back[1].r = color_top.r; Back[1].g = color_top.g; Back[1].b = color_top.b; Back[1].a = color_top.a;
	Back[2].r = color_bot.r; Back[2].g = color_bot.g; Back[2].b = color_bot.b; Back[2].a = color_bot.a;

	Back[3].r = color_bot2.r; Back[3].g = color_bot2.g; Back[3].b = color_bot2.b; Back[3].a = color_bot2.a;
	Back[4].r = color_top.r; Back[4].g = color_top.g; Back[4].b = color_top.b; Back[4].a = color_top.a;
	Back[5].r = color_bot2.r; Back[5].g = color_bot2.g; Back[5].b = color_bot2.b; Back[5].a = color_bot2.a;

	Back[6].r = color_bot.r; Back[6].g = color_bot.g; Back[6].b = color_bot.b; Back[6].a = color_bot.a;
	Back[7].r = color_top.r; Back[7].g = color_top.g; Back[7].b = color_top.b; Back[7].a = color_top.a;
	Back[8].r = color_bot.r; Back[8].g = color_bot.g; Back[8].b = color_bot.b; Back[8].a = color_bot.a;

	Back[9].r = color_bot2.r; Back[9].g = color_bot2.g; Back[9].b = color_bot2.b; Back[9].a = color_bot2.a;
	Back[10].r = color_top.r; Back[10].g = color_top.g; Back[10].b = color_top.b; Back[10].a = color_top.a;
	Back[11].r = color_bot2.r; Back[11].g = color_bot2.g; Back[11].b = color_bot2.b; Back[11].a = color_bot2.a;
}

void smooth(int value) {

	psych += 0.01;
	changeColorBack(Back);
	glutPostRedisplay();
	glutTimerFunc(30, smooth, 0);
}

void update(int value) {

	bool moving = false;

	if (pressing_left)
	{
		horizontal_speed -= acceleration;
		moving = true;
	}
	if (pressing_right)
	{
		horizontal_speed += acceleration;
		moving = true;
	}
	if (pressing_up)
	{
		vertical_speed -= acceleration;
		moving = true;
	}
	if (pressing_down)
	{
		vertical_speed += acceleration;
		moving = true;
	}

	if (!moving) {

		if (horizontal_speed > 0)
		{
			horizontal_speed -= 1;
			if (horizontal_speed < 0)
				horizontal_speed = 0;
		}
		if (horizontal_speed < 0)
		{
			horizontal_speed += 1;
			if (horizontal_speed > 0)
				horizontal_speed = 0;
		}

		if (vertical_speed > 0)
		{
			vertical_speed -= 1;
			if (vertical_speed < 0)
				vertical_speed = 0;
		}
		if (vertical_speed < 0)
		{
			vertical_speed += 1;
			if (vertical_speed > 0)
				vertical_speed = 0;
		}
	}

	posx += horizontal_speed;
	posy += vertical_speed;

	if (horizontal_speed > 20) {
		horizontal_speed = 20;
	}
	if (horizontal_speed < -20) {
		horizontal_speed = -20;
	}
	if (vertical_speed > 20) {
		vertical_speed = 20;
	}
	if (vertical_speed < -20) {
		vertical_speed = -20;
	}

	if (posx < 0) {
		posx = width;
	}
	if (posx > width) {
		posx = 0;
	}

	if (posy < 0) {
		posy = height;
	}
	if (posy > height) {
		posy = 0;
	}

	if ((posx < obj_posx * width + hitbox)
		&& (posx > obj_posx * width - hitbox)
		&& (posy < (1 - obj_posy) * height + hitbox)
		&& (posy > (1 - obj_posy) * height - hitbox))
	{
		if (hit) {
			object_count--;
			game_points += 50;
			hitbox -= 5;
			if (hitbox < 5) {
				hitbox = 5;
			}
			if (game_points > 1000 && !win_flag) {
				win_flag = true;
				time_end = (int)difftime(time(NULL), timer);
				hitbox = 0;
			}
		}
		else {
			posx = width / 2;
			posy = height / 2;
			vertical_speed = 0;
			horizontal_speed = 0;
			game_points -= 25;
			hitbox += 2.5;
		}
	}
	change_color(nTriangles_obj, Obj);

	createParticles();
	if (object_count == 0) {
		hit = false;
		createObject();
	}
	glutPostRedisplay();
	glutTimerFunc(50, update, 0);
}

void initShader(void)
{
	GLenum ErrorCheckValue = glGetError();

	char* vertexShader = (char*)"vertexShader.glsl";
	char* fragmentShader = (char*)"fragmentShader.glsl";

	programId = ShaderMaker::createProgram(vertexShader, fragmentShader);
	glUseProgram(programId);
}

void init(void)
{
	timer = time(NULL);
	Projection = ortho(0.0f, float(width), 0.0f, float(height));
	MatProj = glGetUniformLocation(programId, "Projection");
	MatModel = glGetUniformLocation(programId, "Model");

	draw_back(0.0, 0.0, 1.0, 1.0, Back);
	glGenVertexArrays(1, &VAO_BACK);
	glBindVertexArray(VAO_BACK);
	glGenBuffers(1, &VBO_BACK);
	glBindBuffer(GL_ARRAY_BUFFER, VBO_BACK);
	glBufferData(GL_ARRAY_BUFFER, vertices_back * sizeof(Point), &Back[0], GL_STATIC_DRAW);
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(2 * sizeof(float)));
	glEnableVertexAttribArray(1);
	glBindVertexArray(0);

	glGenVertexArrays(1, &VAO);
	glBindVertexArray(VAO);
	glGenBuffers(1, &VBO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	init_obj(nTriangles_obj, Obj);
	glGenVertexArrays(1, &VAO_O);
	glBindVertexArray(VAO_O);
	glGenBuffers(1, &VBO_O);
	glBindBuffer(GL_ARRAY_BUFFER, VBO_O);
	glBufferData(GL_ARRAY_BUFFER, vertices_obj * sizeof(Point), &Obj[0], GL_STATIC_DRAW);
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(2 * sizeof(float)));
	glEnableVertexAttribArray(1);
	glBindVertexArray(0);

	glutSwapBuffers();
}

void printToScreen()
{
	string time_s = "Time : " + (to_string((int)difftime(time(NULL), timer))) + "s";
	if (win_flag) time_s = "Time : " + to_string(time_end) + "s";
	string score = "Score : " + to_string(game_points);
	string win = "You completed the game in " + to_string(time_end) + "s";

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluOrtho2D(0, width, 0, height);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	vector<string> lines;
	lines.push_back(time_s);
	lines.push_back(score);
	if (win_flag) {
		lines.push_back(win);
	}
	HUD_Logger::get()->printInfo(lines);
}

void drawScene() {

	glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);

	glUseProgram(programId);
	glUniformMatrix4fv(MatProj, 1, GL_FALSE, value_ptr(Projection));
	Model = mat4(1.0);
	Model = scale(Model, vec3(float(width), float(height), 1.0));
	Model = translate(Model, vec3(0.0, 0.0, 0.0));
	glUniformMatrix4fv(MatModel, 1, GL_FALSE, value_ptr(Model));

	glGenVertexArrays(1, &VAO_BACK);
	glBindVertexArray(VAO_BACK);
	glGenBuffers(1, &VBO_BACK);
	glBindBuffer(GL_ARRAY_BUFFER, VBO_BACK);
	glBufferData(GL_ARRAY_BUFFER, vertices_back * sizeof(Point), &Back[0], GL_STATIC_DRAW);
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(2 * sizeof(float)));
	glEnableVertexAttribArray(1);
	glDrawArrays(GL_TRIANGLES, 0, vertices_back);
	glBindVertexArray(0);

	int P_size = 0;
	for (int i = 0; i < particles.size(); i++) {
		particles.at(i).xFactor /= particles.at(i).drag;
		particles.at(i).yFactor /= particles.at(i).drag;

		particles.at(i).x += particles.at(i).xFactor;
		particles.at(i).y += particles.at(i).yFactor;

		particles.at(i).alpha -= std::max(0.005, (0.05 / (std::max(0, game_points) / 100 + 1)));

		float xPos = -1.0f + ((float)particles.at(i).x) * 2 / ((float)(width));
		float yPos = -1.0f + ((float)(particles.at(i).y)) * 2 / ((float)(height));

		if (particles.at(i).alpha <= 0.0) {
			particles.erase(particles.begin() + i);
		}
		else {
			Punti[i].x = xPos;
			Punti[i].y = yPos;
			Punti[i].r = particles.at(i).color.r;
			Punti[i].g = particles.at(i).color.g;
			Punti[i].b = particles.at(i).color.b;
			Punti[i].a = particles.at(i).alpha;
			P_size += 1;
		}
	}

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	Model = mat4(1.0);
	Model = translate(Model, vec3(float(width) * 0.5, float(height) * 0.5, 0.0));
	Model = scale(Model, vec3(width / 2, height / 2, 1.0));
	glUniformMatrix4fv(MatModel, 1, GL_FALSE, value_ptr(Model));
	glGenVertexArrays(1, &VAO);
	glBindVertexArray(VAO);
	glGenBuffers(1, &VBO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, P_size * sizeof(Point), &Punti[0], GL_STATIC_DRAW);
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(2 * sizeof(float)));
	glEnableVertexAttribArray(1);
	glPointSize(3.0);
	glDrawArrays(GL_POINTS, 0, P_size);
	glBindVertexArray(0);

	Model = mat4(1.0);
	Model = translate(Model, vec3(float(width) * obj_posx, float(height) * obj_posy, 0.0));
	Model = scale(Model, vec3(float(hitbox * 1.3), float(hitbox * 1.3), 1.0));
	glUniformMatrix4fv(MatModel, 1, GL_FALSE, value_ptr(Model));
	glGenVertexArrays(1, &VAO_O);
	glBindVertexArray(VAO_O);
	glGenBuffers(1, &VBO_O);
	glBindBuffer(GL_ARRAY_BUFFER, VBO_O);
	glBufferData(GL_ARRAY_BUFFER, vertices_obj * sizeof(Point), &Obj[0], GL_STATIC_DRAW);
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(2 * sizeof(float)));
	glEnableVertexAttribArray(1);
	glDrawArrays(GL_TRIANGLES, 0, vertices_obj / 2);
	glBindVertexArray(0);

	glDisable(GL_BLEND);

	glUseProgram(0);
	printToScreen();

	glutSwapBuffers();
}


int main(int argc, char** argv)
{
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA | GLUT_DEPTH | GLUT_MULTISAMPLE);
	glutInitWindowSize(width, height);
	glutInitWindowPosition(0, 0);
	glutCreateWindow("Particellar Snake");

	glutDisplayFunc(drawScene);
	glutReshapeFunc(resize);
	glutTimerFunc(20, update, 0);
	glutTimerFunc(30, smooth, 0);
	glutMouseFunc(mouseClick);
	glutKeyboardFunc(keyMotion);
	glutKeyboardUpFunc(keyMotionReleased);

	srand((unsigned)time(NULL));

	glewExperimental = GL_TRUE;
	glewInit();
	fprintf(stdout, "INFO: OpenGL Version: %s\n", glGetString(GL_VERSION));

	initShader();
	init();
	glutMainLoop();
}