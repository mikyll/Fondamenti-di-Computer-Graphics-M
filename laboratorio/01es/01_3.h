/*
* ES 3: de Casteljau Algorithm
* 
*/

#pragma once

#include "defs.h"
#include "utils.h"

void getCurve_deCasteljau(Point2D* ctrlPts, int numCtrl, Point2D* curvePts, int* numCurve);