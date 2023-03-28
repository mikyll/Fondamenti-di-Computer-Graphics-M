/*
* ES 5c: Catmull - Rom spline
* 
* Main idea:
* we need 4 points to draw a spline segment. We know
* pi-, pi, p(i+1) and p(i+1)+
* 
* To draw the curve we need pi, pi+, p(i+1)- and p(i+1)
* So we have to find the unknown control points pi+ and p(i+1)-
* 
* We can use the estimation of the derivative in pi
* to define the unknown control points
* 
*/

#pragma once

#include "defs.h"
#include "utils.h"

void getCurve_CatmullRomSpline(Point2D* ctrlPts, int numCtrl, Point2D* curvePts, int* numCurve);