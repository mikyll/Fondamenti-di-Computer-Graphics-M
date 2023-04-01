#include "figure.h"

void createFigureVAO(Figure* fig)
{
	glGenVertexArrays(1, &fig->VAO);
	glBindVertexArray(fig->VAO);
	// Genero, rendo attivo, riempio il VBO della geometria dei vertici
	glGenBuffers(1, &fig->VBO_Geom);
	glBindBuffer(GL_ARRAY_BUFFER, fig->VBO_Geom);
	glBufferData(GL_ARRAY_BUFFER, fig->vertices.size() * sizeof(Point3D), fig->vertices.data(), GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);
	glEnableVertexAttribArray(0);

	// Genero, rendo attivo, riempio il VBO dei colori
	glGenBuffers(1, &fig->VBO_Col);
	glBindBuffer(GL_ARRAY_BUFFER, fig->VBO_Col);
	glBufferData(GL_ARRAY_BUFFER, fig->colors.size() * sizeof(ColorRGBA), fig->colors.data(), GL_STATIC_DRAW);
	// Adesso carico il VBO dei colori nel layer 2
	glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, 0, (void*)0);
	glEnableVertexAttribArray(1);
}

void updateFigureVertices(Figure* fig)
{
	glBindBuffer(GL_ARRAY_BUFFER, fig->VBO_Geom);
	glBufferData(GL_ARRAY_BUFFER, fig->vertices.size() * sizeof(Point3D), fig->vertices.data(), GL_STATIC_DRAW);
}

void buildCircle(Figure* fig, float radius, int step, ColorRGBA colorExtern, ColorRGBA colorIntern)
{
	// 2 * PI = complete circle => divide by num of triangles we want to use
	float stepA = (2 * PI) / fig->numTriangles;

	for (int i = 0; i < fig->numTriangles; i += step)
	{
		// Extern vertices
		Point3D v0 = { radius * cos((double)i * stepA), radius * sin((double)i * stepA), 0.0f };
		fig->vertices.push_back(v0);
		fig->colors.push_back(colorExtern);

		Point3D v1 = { radius * cos((double)(i + 1) * stepA), radius * sin((double)(i + 1) * stepA), 0.0f };
		fig->vertices.push_back(v1);
		fig->colors.push_back(colorExtern);

		// Intern vertices
		Point3D v2 = { 0.0f, 0.0f, 0.0f };
		fig->vertices.push_back(v2);
		fig->colors.push_back(colorIntern);
	}
}

void buildHollowCircle(Figure* fig, float radiusExtern, float radiusIntern, int step, ColorRGBA colorExtern, ColorRGBA colorIntern)
{
	float stepA = (2 * PI) / fig->numTriangles;

	for (int i = 0; i < fig->numTriangles; i += step)
	{
		// Extern vertices
		Point3D v0 = { radiusExtern * cos((double)i * stepA), radiusExtern * sin((double)i * stepA), 0.0f };
		fig->vertices.push_back(v0);
		fig->colors.push_back(colorExtern);

		Point3D v1 = { radiusExtern * cos((double)(i + 1) * stepA), radiusExtern * sin((double)(i + 1) * stepA), 0.0f };
		fig->vertices.push_back(v1);
		fig->colors.push_back(colorExtern);

		// Intern vertices
		Point3D v2 = { radiusIntern * cos((double)i * stepA), radiusIntern * sin((double)i * stepA), 0.0f };
		fig->vertices.push_back(v2);
		fig->colors.push_back(colorIntern);

		Point3D v3 = { radiusIntern * cos((double)(i + 1) * stepA), radiusIntern * sin((double)(i + 1) * stepA), 0.0f };
		fig->vertices.push_back(v3);
		fig->colors.push_back(colorIntern);
	}
}