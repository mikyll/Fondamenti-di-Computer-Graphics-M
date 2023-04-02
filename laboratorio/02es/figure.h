#pragma once

#include "commons.h"

void createFigureVAO(Figure* fig);
void updateFigureVertices(Figure* fig);

void buildCircle(Figure* fig, Point3D center, float radius, int step, ColorRGBA colorExtern, ColorRGBA colorIntern);
void buildHollowCircle(Figure* fig, Point3D center, float radiusExtern, float radiusIntern, int step, ColorRGBA colorExtern, ColorRGBA colorIntern);