/*
* ES 3: de Casteljau
* 
* Subdivide a Bézier curve by applying de Casteljau's
* method.
* 
* Uniform method:
* "Discretize the parametric interval into N equidistant
* points, then plot the polygonal joining corresponding
* evaluated points on the curve."
* 
* The keyword is "equidistant": we have to consider a t
* (for each segment) which is a fraction of the number
* of segments.
* 
* given N = number of segments we want to use to render 
* the curve,
* 
* segment 0 -> t = 0 / N
* segment 1 -> t = 1 / N
* ...
* segment N-1 / N
* 
*/

#pragma once

#include "defs.h"
#include "utils.h"

void getCurve_deCasteljau(Point2D* ctrlPts, int numCtrl, Point2D* curvePts, int* numCurve);