/*
* ES 5b: Adaptive Subdivision
* 
* Main idea: break a curve into smaller and smaller subcurves
* until each subcurve is sufficiently close to being a
* straight line (Flat Test), so that rendering the
* subcurves as straight lines gives adequate results.
* 
* Explaination: if the straight lines are lower a given
* threshold the line segments give the "illusion" of being
* a curve.
*/

#pragma once

#include "defs.h"
#include "utils.h"

void getCurve_adaptiveSubdivision(Point2D* ctrlPts, int numCtrl, Point2D* curvePts, int* numCurve);