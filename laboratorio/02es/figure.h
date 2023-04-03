#pragma once

#include "commons.h"

void createFigureVAO(Figure* fig);
void updateFigureVertices(Figure* fig);

void buildCircle(Figure* fig, Point3D center, float radius, int numTriangles, ColorRGBA colorExtern, ColorRGBA colorIntern);
void buildHollowCircle(Figure* fig, Point3D center, float radiusExtern, float radiusIntern, int numTriangles, ColorRGBA colorExtern, ColorRGBA colorIntern);
void buildCircumference(Figure* fig, Point3D center, float radius, int numSegments, ColorRGBA color);