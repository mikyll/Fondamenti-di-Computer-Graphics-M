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

void buildCircle(Figure* fig, Point3D center, float radius, int step, ColorRGBA colorExtern, ColorRGBA colorIntern)
{
	// PI * 2 = complete circle => divide by num of triangles we want to use
	float stepA = (PI * 2) / fig->numTriangles;

	for (int i = 0; i < fig->numTriangles; i += step)
	{
		// Extern vertices
		Point3D v0 = { center.x + radius * cos((double)i * stepA), center.y + radius * sin((double)i * stepA), 0.0f };
		fig->vertices.push_back(v0);
		fig->colors.push_back(colorExtern);

		Point3D v1 = { center.x + radius * cos((double)(i + 1) * stepA), center.y + radius * sin((double)(i + 1) * stepA), 0.0f };
		fig->vertices.push_back(v1);
		fig->colors.push_back(colorExtern);

		// Intern vertices
		Point3D v2 = center;
		fig->vertices.push_back(v2);
		fig->colors.push_back(colorIntern);
	}
}

void buildSemiCircle(Figure* fig, Point3D center, float radius, float startAngle, int step, ColorRGBA colorExtern, ColorRGBA colorIntern)
{
	// PI = half circle => divide by num of triangles we want to use
	float stepA = (PI * 2) / fig->numTriangles;

	for (int i = 0; i < fig->numTriangles; i += step)
	{
		float t0 = (float)i * stepA + startAngle;
		float t1 = (float)(i+1) * stepA + startAngle;

		// Extern vertices
		Point3D v0 = { center.x + radius * cos(t0), center.y + radius * sin(t0), 0.0f };
		fig->vertices.push_back(v0);
		fig->colors.push_back(colorExtern);

		Point3D v1 = { center.x + radius * cos(t1), center.y + radius * sin(t1), 0.0f };
		fig->vertices.push_back(v1);
		fig->colors.push_back(colorExtern);

		// Intern vertices
		Point3D v2 = center;
		fig->vertices.push_back(v2);
		fig->colors.push_back(colorIntern);
	}
}

void buildHollowCircle(Figure* fig, Point3D center, float radiusExtern, float radiusIntern, int step, ColorRGBA colorExtern, ColorRGBA colorIntern)
{
	float stepA = (PI * 2) / fig->numTriangles;

	for (int i = 0; i < fig->numTriangles; i += step)
	{
		// Extern vertices
		Point3D v0 = { center.x + radiusExtern * cos((double)i * stepA), center.y + radiusExtern * sin((double)i * stepA), 0.0f };
		fig->vertices.push_back(v0);
		fig->colors.push_back(colorExtern);

		Point3D v1 = { center.x + radiusExtern * cos((double)(i + 1) * stepA), center.y + radiusExtern * sin((double)(i + 1) * stepA), 0.0f };
		fig->vertices.push_back(v1);
		fig->colors.push_back(colorExtern);

		// Intern vertices
		Point3D v2 = { center.x + radiusIntern * cos((double)i * stepA), center.y + radiusIntern * sin((double)i * stepA), 0.0f };
		fig->vertices.push_back(v2);
		fig->colors.push_back(colorIntern);

		Point3D v3 = { center.x + radiusIntern * cos((double)(i + 1) * stepA), center.y + radiusIntern * sin((double)(i + 1) * stepA), 0.0f };
		fig->vertices.push_back(v3);
		fig->colors.push_back(colorIntern);
	}
}