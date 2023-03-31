// 2D_JUMPING_BALL.cpp : 
//
#include <iostream>
#include "ShaderMaker.h"
#include <GL/glew.h>
#include <GL/freeglut.h>

static unsigned int programId;
#define PI 3.14159265358979323846

unsigned int VAO_MONTAGNE, VAO_PRATO, VAO_SOLE, VAO_CIELO, VAO_PALLA, VAO_PalaEolica;
unsigned int VBO_M, VBO_P, VBO_S, VBO_C, VBO_Pa, VBO_PE, MatProj, MatModel;

// Include GLM; libreria matematica per le opengl
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/transform.hpp>
#include <glm/gtc/type_ptr.hpp>
using namespace glm;

mat4 Projection;  //Matrice di proiezione
mat4 Model; //Matrice per il cambiamento da Sistema di riferimento dell'oggetto OCS a sistema di riferimento nel Mondo WCS

// viewport size
int width = 1200;
int height = 720;

float angolo = 0.0;
typedef struct { float x, y, r, g, b, a; } Pointxy;
int Numero_di_pezzi = 128;
int nVertices_montagna = 2 * Numero_di_pezzi;
Pointxy* Punti = new Pointxy[nVertices_montagna];
int vertices_Prato = 6;
Pointxy* Prato = new Pointxy[vertices_Prato];
int vertices_Cielo = 6;
Pointxy* Cielo = new Pointxy[vertices_Cielo];
int nTriangles_sole = 30;
int vertices_sole = 3 * 2 * nTriangles_sole;
Pointxy* Sole = new Pointxy[vertices_sole];
int nTriangles_palla = 30;
int vertices_palla = 3 * 2 * nTriangles_palla;
Pointxy* Palla = new Pointxy[vertices_palla];
int nTriangles_PalaEolica = 4;
int vertices_PalaEolica = 3 * (nTriangles_PalaEolica + 2);
Pointxy* PalaEolica = new Pointxy[vertices_PalaEolica];

// parametri della palla
int		distacco_da_terra = 0;			// distacco da terra 
float	delta = 15;
double	VelocitaOrizzontale = 0; //velocita orizzontale (pixel per frame)

float	posx = float(width) / 2; //coordinate sul piano della posizione iniziale della palla
float	posy = float(height) * 0.2;

bool pressing_left = false;
bool pressing_right = false;

vec4 col_rosso = { 1.0,0.0,0.0,1.0 };
vec4 col_nero = { 0.0,0.0,0.0,1.0 };
/// ///////////////////////////////////////////////////////////////////////////////////
///									Gestione eventi
///////////////////////////////////////////////////////////////////////////////////////
void keyboardPressedEvent(unsigned char key, int x, int y)
{
	switch (key)
	{
	case 'a':
		pressing_left = true;
		break;
	case 'd':
		pressing_right = true;
		break;
	case 27:
		exit(0);
		break;
	default:
		break;
	}
}

void keyboardReleasedEvent(unsigned char key, int x, int y)
{
	switch (key)
	{
	case 'a':
		pressing_left = false;
		break;
	case 'd':
		pressing_right = false;
		break;
	default:
		break;
	}
}
//Movimento della palla in orizzontale
void update(int a)
{
	bool moving = false;

	if (pressing_left)
	{
		VelocitaOrizzontale -= 1;
		moving = true;
	}
	if (pressing_right)
	{
		VelocitaOrizzontale += 1;
		moving = true;
	}
	if (!moving) {   // Se non mi sto muovendo con i tasti a e d,
					 // decremento od incremento la velocita' iniziale fino a portarla
					 // a zero e la palla continua a rimbalzare sul posto
		if (VelocitaOrizzontale > 0)
		{
			VelocitaOrizzontale -= 1;
			if (VelocitaOrizzontale < 0)
				VelocitaOrizzontale = 0;
		}
		if (VelocitaOrizzontale < 0)
		{
			VelocitaOrizzontale += 1;
			if (VelocitaOrizzontale > 0)
				VelocitaOrizzontale = 0;
		}
	}

	//Aggioramento della posizione in x della pallina, che regola il movimento orizzontale
	posx += VelocitaOrizzontale;

	//Se la palla urta i bordi dello schermo 
	// ovvero assume una posizione x<0 o x> width (bordi viewport)
	// la pallina rimbalza ai bordi dello schermo attenuando la velocità
	if (posx < 0.0) {
		posx = 0.0;
		VelocitaOrizzontale = -VelocitaOrizzontale * 0.8;
	}
	if (posx > width) {
		posx = (float) width;
		VelocitaOrizzontale = -VelocitaOrizzontale * 0.8;
	}

	// Gestione del rimbalzo in relazione all'altezza da terra
	//Rimbalzo -20 < distacco_da_terra < 100
	distacco_da_terra += (delta * (1 - (distacco_da_terra / 120)));
	//invertire direzione del jump
	if ((distacco_da_terra > 100) && (delta > 0))  
		delta = -delta;
	if ((distacco_da_terra < -20) && (delta < 0))
		delta = -delta;

	glutPostRedisplay();
	glutTimerFunc(24, update, 0);
}

/////////////////////////////////////////////////////////////////////////////////////////// 
void disegna_pianoxy(float x, float y, float width, float height, vec4 color_top, vec4 color_bot, Pointxy* piano)
{
	piano[0].x = x;	piano[0].y = y; 
	piano[0].r = color_bot.r; piano[0].g = color_bot.g; piano[0].b = color_bot.b; piano[0].a = color_bot.a;
	piano[1].x = x + width;	piano[1].y = y;
	piano[1].r = color_top.r; piano[1].g = color_top.g; piano[1].b = color_top.b; piano[1].a = color_top.a;
	piano[2].x = x + width;	piano[2].y = y + height; 
	piano[2].r = color_bot.r; piano[2].g = color_bot.g; piano[2].b = color_bot.b; piano[2].a = color_bot.a;

	piano[3].x = x + width;	piano[3].y = y + height; 
	piano[3].r = color_bot.r; piano[3].g = color_bot.g; piano[3].b = color_bot.b; piano[3].a = color_bot.a;
	piano[4].x = x;	piano[4].y = y + height; 
	piano[4].r = color_top.r; piano[4].g = color_top.g; piano[4].b = color_top.b; piano[4].a = color_top.a;
	piano[5].x = x;	piano[5].y = y; 
	piano[5].r = color_bot.r; piano[5].g = color_bot.g; piano[5].b = color_bot.b; piano[5].a = color_bot.a;
}

void disegna_cerchio(int nTriangles, int step, vec4 color_top, vec4 color_bot, Pointxy* Cerchio) {
	int i;
	float stepA = (2 * PI) / nTriangles;

	int comp = 0;
	// step = 1 -> triangoli adiacenti, step = n -> triangoli distanti step l'uno dall'altro
	for (i = 0; i < nTriangles; i += step)
	{
		Cerchio[comp].x = cos((double)i * stepA);
		Cerchio[comp].y = sin((double)i * stepA);
		Cerchio[comp].r = color_top.r; Cerchio[comp].g = color_top.g; Cerchio[comp].b = color_top.b; Cerchio[comp].a = color_top.a;

		Cerchio[comp + 1].x = cos((double)(i + 1) * stepA);
		Cerchio[comp + 1].y = sin((double)(i + 1) * stepA);
		Cerchio[comp + 1].r = color_top.r; Cerchio[comp + 1].g = color_top.g; Cerchio[comp + 1].b = color_top.b; Cerchio[comp + 1].a = color_top.a;

		Cerchio[comp + 2].x = 0.0;
		Cerchio[comp + 2].y = 0.0;
		Cerchio[comp + 2].r = color_bot.r; Cerchio[comp + 2].g = color_bot.g; Cerchio[comp + 2].b = color_bot.b; Cerchio[comp + 2].a = color_bot.a;
		comp += 3;
	}
}

void disegna_PalaEolica(int nTriangles, Pointxy* PalaEolica) {

	int i, cont;
	Pointxy* Sostegno;
	int vertici = 6;
	Sostegno = new Pointxy[vertici];

	//Costruisco la geometria della pala ed i suoi colori
	vec4 col_top = { 0.0, 1.0, 0.0, 1.0 };
	vec4 col_bottom = { 0.5, 0.5, 0.05, 0.8 };

	disegna_cerchio(nTriangles * 2, 2, col_top, col_bottom, PalaEolica);
	disegna_pianoxy(-0.1, -1.0, 1.0, 1.0, col_nero, col_nero, Sostegno);

	cont = 3 * nTriangles;
	for (i = 0; i < 6; i++)
	{
		PalaEolica[cont + i].x = Sostegno[i].x;
		PalaEolica[cont + i].y = Sostegno[i].y;
		PalaEolica[cont + i].r = Sostegno[i].r;	PalaEolica[cont + i].g = Sostegno[i].g;	PalaEolica[cont + i].b = Sostegno[i].b; PalaEolica[cont + i].a = Sostegno[i].a;
	}
}

double lerp(double a, double b, double amount) {
	//Interpolazione lineare tra a e b secondo amount
	return (1 - amount) * a + amount * b;
}

void disegna_palla(int nTriangles, Pointxy* Palla) {

	int i, cont;
	Pointxy* Ombra;
	int vertici = 3 * nTriangles;
	Ombra = new Pointxy[vertici];

	//Costruisco la geometria della palla ed i suoi colori
	vec4 col_bottom = { 1.0, 0.8, 0.0, 1.0 };
	disegna_cerchio(nTriangles, 1, col_rosso, col_bottom, Palla);

	//Costruisco la geometria dell'ombra ed i suoi colori.
	vec4 col_top = { 0.49,0.49,0.49, 0.0 };
	col_bottom = { 0.0, 0.0, 0.0, 0.6 };
	disegna_cerchio(nTriangles, 1, col_top, col_bottom, Ombra);

	//Appendo a Palla la sua Ombra
	cont = 3 * nTriangles;
	for (i = 0; i < 3 * nTriangles; i++)
	{
		Palla[cont + i].x = Ombra[i].x;
		Palla[cont + i].y = Ombra[i].y;
		Palla[cont + i].r = Ombra[i].r;	Palla[cont + i].g = Ombra[i].g;	Palla[cont + i].b = Ombra[i].b;	Palla[cont + i].a = Ombra[i].a;
	}
}

void disegna_sole(int nTriangles, Pointxy* Sole) {
	int i, cont;
	Pointxy* OutSide;
	int vertici = 3 * nTriangles;
	OutSide = new Pointxy[vertici];

	vec4 col_top_sole = { 1.0, 1.0, 1.0, 1.0 };
	vec4 col_bottom_sole = { 1.0, 0.8627, 0.0, 1.0 };
	disegna_cerchio(nTriangles, 1, col_top_sole, col_bottom_sole, Sole);

	col_top_sole = { 1.0, 1.0, 1.0, 0.0 };
	col_bottom_sole = { 1.0, 0.8627, 0.0, 1.0 };
	disegna_cerchio(nTriangles, 1, col_top_sole, col_bottom_sole, OutSide);

	cont = 3 * nTriangles;
	for (i = 0; i < 3 * nTriangles; i++)
	{
		Sole[cont + i].x = OutSide[i].x;
		Sole[cont + i].y = OutSide[i].y;
		Sole[cont + i].r = OutSide[i].r; Sole[cont + i].g = OutSide[i].g; Sole[cont + i].b = OutSide[i].b; Sole[cont + i].a = OutSide[i].a;
	}
}

void disegna_montagne(float x0, float y0, int altezza_montagna, int larghezza_montagne, int numero_di_montagne, vec4 color_top, vec4 color_bot, Pointxy* v_montagna)
{
	float dimensione_pezzi = larghezza_montagne / (float)Numero_di_pezzi;
	float frequenza = PI * numero_di_montagne;

	int n_vertici = 0;

	for (int i = 0; i < Numero_di_pezzi; i += 1)
	{

		v_montagna[n_vertici].x = x0 + i * dimensione_pezzi;
		v_montagna[n_vertici].y = y0;
		v_montagna[n_vertici].r = color_bot.r; v_montagna[n_vertici].g = color_bot.g; v_montagna[n_vertici].b = color_bot.b; v_montagna[n_vertici].a = color_bot.a;

		v_montagna[n_vertici + 1].x = x0 + i * dimensione_pezzi;
		v_montagna[n_vertici + 1].y = y0 + altezza_montagna * abs(sin(i / (float)Numero_di_pezzi * frequenza));
		v_montagna[n_vertici + 1].r = color_top.r; v_montagna[n_vertici + 1].g = color_top.g; v_montagna[n_vertici + 1].b = color_top.b; v_montagna[n_vertici + 1].a = color_top.a;

		n_vertici += 2;
	}
}

void initShader(void)
{
	GLenum ErrorCheckValue = glGetError();

	char* vertexShader = (char*)"vertexShader_C_M.glsl";
	char* fragmentShader = (char*)"fragmentShader_C_M.glsl";

	programId = ShaderMaker::createProgram(vertexShader, fragmentShader);
	glUseProgram(programId);

}
void init(void)
{
	Projection = ortho(0.0f, float(width), 0.0f, float(height));
	MatProj = glGetUniformLocation(programId, "Projection");
	MatModel = glGetUniformLocation(programId, "Model");

	//Costruzione geometria e colori del CIELO
	vec4 col_top = { 0.3,0.6,1.0,1.0 };
	vec4 col_bottom = { 0.0 , 0.1, 1.0, 1.0 };
	disegna_pianoxy(0, height*0.5, width, height*0.5, col_bottom, col_top, Cielo);
	//Generazione del VAO del Cielo
	glGenVertexArrays(1, &VAO_CIELO);
	glBindVertexArray(VAO_CIELO);
	glGenBuffers(1, &VBO_C);
	glBindBuffer(GL_ARRAY_BUFFER, VBO_C);
	glBufferData(GL_ARRAY_BUFFER, vertices_Cielo * sizeof(Pointxy), &Cielo[0], GL_STATIC_DRAW);
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(2 * sizeof(float)));
	glEnableVertexAttribArray(1);
	glBindVertexArray(0);

	//Costruzione geometria e colori del PRATO
	col_top = { 0.1, 0.5, 0.1, 1.0 };
	col_bottom = { 0.8, 1.0, 0.2, 1.0 };
	disegna_pianoxy(0, 0, width, height * 0.5, col_bottom, col_top, Prato);
	//Genero un VAO Prato
	glGenVertexArrays(1, &VAO_PRATO);
	glBindVertexArray(VAO_PRATO);
	glGenBuffers(1, &VBO_P);
	glBindBuffer(GL_ARRAY_BUFFER, VBO_P);
	glBufferData(GL_ARRAY_BUFFER, vertices_Prato * sizeof(Pointxy), &Prato[0], GL_STATIC_DRAW);
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(2 * sizeof(float)));
	glEnableVertexAttribArray(1);
	glBindVertexArray(0);
	
	//Costruzione geometria e colori del SOLE
	//Genero il VAO del SOLE
	disegna_sole(nTriangles_sole, Sole);
	glGenVertexArrays(1, &VAO_SOLE);
	glBindVertexArray(VAO_SOLE);
	glGenBuffers(1, &VBO_S);
	glBindBuffer(GL_ARRAY_BUFFER, VBO_S);
	glBufferData(GL_ARRAY_BUFFER, vertices_sole * sizeof(Pointxy), &Sole[0], GL_STATIC_DRAW);
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(2 * sizeof(float)));
	glEnableVertexAttribArray(1);
	glBindVertexArray(0);

	
	//Costruzione geometria e colori delle montagne
	col_top = { 0.5451, 0.2706, 0.0745, 1.0000 };
	col_bottom = { 0.4980, 0.0353, 0.1843, 1.0000 };
	disegna_montagne(0, 0, 100, width, 3, col_bottom, col_top, Punti);
	//Genero un VAO Montagne
	glGenVertexArrays(1, &VAO_MONTAGNE);
	glBindVertexArray(VAO_MONTAGNE);
	glGenBuffers(1, &VBO_M);
	glBindBuffer(GL_ARRAY_BUFFER, VBO_M);
	glBufferData(GL_ARRAY_BUFFER, nVertices_montagna * sizeof(Pointxy), &Punti[0], GL_STATIC_DRAW);
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(2 * sizeof(float)));
	glEnableVertexAttribArray(1);
	glBindVertexArray(0);

	//Costruzione geometria e colori della PALLA
	disegna_palla(nTriangles_palla, Palla);
	glGenVertexArrays(1, &VAO_PALLA);
	glBindVertexArray(VAO_PALLA);
	glGenBuffers(1, &VBO_Pa);
	glBindBuffer(GL_ARRAY_BUFFER, VBO_Pa);
	glBufferData(GL_ARRAY_BUFFER, vertices_palla * sizeof(Pointxy), &Palla[0], GL_STATIC_DRAW);
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(2 * sizeof(float)));
	glEnableVertexAttribArray(1);
	glBindVertexArray(0);

	//Costruzione geometria e colori delle PALE EOLICHE
	disegna_PalaEolica(nTriangles_PalaEolica, PalaEolica);
	glGenVertexArrays(1, &VAO_PalaEolica);
	glBindVertexArray(VAO_PalaEolica);
	glGenBuffers(1, &VBO_PE);
	glBindBuffer(GL_ARRAY_BUFFER, VBO_PE);
	glBufferData(GL_ARRAY_BUFFER, vertices_PalaEolica * sizeof(Pointxy), &PalaEolica[0], GL_STATIC_DRAW);
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(2 * sizeof(float)));
	glEnableVertexAttribArray(1);
	glBindVertexArray(0);

	//Definisco il colore assegnato allo schermo
	glClearColor(0.0, 0.0, 0.0, 1.0);

	glutSwapBuffers();
}

void drawScene(void)
{
	glUniformMatrix4fv(MatProj, 1, GL_FALSE, value_ptr(Projection));
	glClear(GL_COLOR_BUFFER_BIT);

	//Disegna cielo
	Model = mat4(1.0);
	glUniformMatrix4fv(MatModel, 1, GL_FALSE, value_ptr(Model));
	glBindVertexArray(VAO_CIELO);
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	glDrawArrays(GL_TRIANGLES, 0, vertices_Cielo);
	glBindVertexArray(0);

	//Disegna prato
	Model = mat4(1.0);
	glUniformMatrix4fv(MatModel, 1, GL_FALSE, value_ptr(Model));
	glBindVertexArray(VAO_PRATO);
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	glDrawArrays(GL_TRIANGLES, 0, vertices_Prato);
	glBindVertexArray(0);

	// Disegna sole
	Model = mat4(1.0);
	Model = translate(Model, vec3(float(width) * 0.5, float(height) * 0.8, 0.0));
	Model = scale(Model, vec3(30.0, 30.0, 1.0));
	glUniformMatrix4fv(MatModel, 1, GL_FALSE, value_ptr(Model));
	glBindVertexArray(VAO_SOLE);
	//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	glDrawArrays(GL_TRIANGLES, 0, vertices_sole / 2);
	//Disegna Alone del sole
	Model = mat4(1.0);
	Model = translate(Model, vec3(float(width) * 0.5, float(height) * 0.8, 0.0));
	Model = scale(Model, vec3(80.0, 80.0, 1.0));
	glUniformMatrix4fv(MatModel, 1, GL_FALSE, value_ptr(Model));
	glDrawArrays(GL_TRIANGLES, vertices_sole / 2, vertices_sole / 2);
	glBindVertexArray(0);

	//Disegna montagne
	Model = mat4(1.0);
	Model = translate(Model, vec3(0.0, float(height) / 2, 0.0));
	glUniformMatrix4fv(MatModel, 1, GL_FALSE, value_ptr(Model));
	glBindVertexArray(VAO_MONTAGNE);
	glDrawArrays(GL_TRIANGLE_STRIP, 0, nVertices_montagna);
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	glBindVertexArray(0);

	// Disegna palla (ombra+palla)
	Model = mat4(1.0);
	float shadow_scale = lerp(1,0, (float)distacco_da_terra / 255); //distacco grande -> fattore scala piccolo
	// larghezza effettiva in pixel della palla (80 diametro palla normale -- 100 palla dilatata)
	double bwidth = distacco_da_terra < 0 ? lerp(80, 100, (double)abs(distacco_da_terra) / 20) : 80;
	// altezza effettiva in pixel della palla
	double bheight = distacco_da_terra < 0 ? 80 + distacco_da_terra : 80;
	//Matrice per il cambiamento del sistema di riferimento per l'OMBRA della palla
	Model = translate(Model, vec3(posx - bwidth / 2 * shadow_scale, posy + 10 + 10 * (1 - shadow_scale), 0.0f));
	Model = scale(Model, vec3(float(bwidth) * shadow_scale, (50.0 * shadow_scale), 1.0));
	glUniformMatrix4fv(MatModel, 1, GL_FALSE, value_ptr(Model));
	glBindVertexArray(VAO_PALLA);
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	glDrawArrays(GL_TRIANGLES, vertices_palla / 2, vertices_palla / 2);
	//Matrice per il cambiamento del sistema di riferimento per la  PALLA
	Model = mat4(1.0);
	Model = translate(Model, vec3(posx - bwidth / 2, posy + bheight + distacco_da_terra, 0.0f));
	Model = scale(Model, vec3(float(bwidth) / 2, float(bheight) / 2, 1.0));
	glUniformMatrix4fv(MatModel, 1, GL_FALSE, value_ptr(Model));
	glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	glDrawArrays(GL_TRIANGLES, 0, vertices_palla / 2);
	glBindVertexArray(0);

	//Disegna Pala Eolica
	glBindVertexArray(VAO_PalaEolica);
	for (int i = 1; i < 7; i++) /* Disegna 6 istanze di Pala Eolica (sostegno+pale) */
	{
		Model = mat4(1.0);
		Model = translate(Model, vec3(float(width) * 0.15 * i, float(height) * 0.62, 0.0));
		Model = scale(Model, vec3(3.0, 100.0, 1.0));
		glUniformMatrix4fv(MatModel, 1, GL_FALSE, value_ptr(Model));
		glDrawArrays(GL_TRIANGLES, vertices_PalaEolica - 6, 6);
		Model = mat4(1.0);
		Model = translate(Model, vec3(float(width) * 0.15 * i, float(height) * 0.62, 0.0));
		Model = scale(Model, vec3(40.0, 40.0, 1.0));
		Model = rotate(Model, radians(angolo), vec3(0.0f, 0.0f, 1.0f));
		glUniformMatrix4fv(MatModel, 1, GL_FALSE, value_ptr(Model));
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
		glDrawArrays(GL_TRIANGLES, 0, vertices_PalaEolica - 6); // Starting from vertex 0; total vertices
	}
	glBindVertexArray(0);

	glutSwapBuffers();
}

int main(int argc, char* argv[])
{
	glutInit(&argc, argv);

	glutInitContextVersion(4, 0);
	glutInitContextProfile(GLUT_COMPATIBILITY_PROFILE);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA);

	glutInitWindowSize(width, height);
	glutInitWindowPosition(100, 100);
	glutCreateWindow("2D Animation");
	glutDisplayFunc(drawScene);
	//Evento tastiera tasto premuto
	glutKeyboardFunc(keyboardPressedEvent);
	//Evento tastiera tasto rilasciato (per fermare lo spostamento a dx e sx mantenendo il rimbalzo sul posto)
	glutKeyboardUpFunc(keyboardReleasedEvent);

	//gestione animazione
	glutTimerFunc(66, update, 0);
	glewExperimental = GL_TRUE;
	glewInit();

	initShader();
	init();

	glEnable(GL_BLEND);
	glEnable(GL_ALPHA_TEST);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glutMainLoop();
}