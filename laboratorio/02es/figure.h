#pragma once

#include "commons.h"

void createVAOvector(Figure* fig);
void buildCircle(Figure* fig, float radius, int step, ColorRGBA colorExtern, ColorRGBA colorIntern);
void buildHollowCircle(Figure* fig, float radiusExtern, float radiusIntern, int step, ColorRGBA colorExtern, ColorRGBA colorIntern);