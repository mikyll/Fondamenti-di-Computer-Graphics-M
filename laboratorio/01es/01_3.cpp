#include "01_3.h"

static Point2D deCasteljau(Point2D* points, int numPoints, float t);

/*
Given an array of points, evaluate a point of the Bézier curve in t.
*/
static Point2D deCasteljau(Point2D* points, int numPoints, float t)
{
	// Auxiliary array (since we edit it in each iteration, we don't want to overwrite and lose the initial array)
	Point2D pointArrayAux[MAX_NUM_PTS] = { 0.0f, 0.0f };

	// Copy to the auxiliary array
	for (int i = 0; i < numPoints; i++)
	{
		pointArrayAux[i] = points[i];
	}

	// "Depth" loop
	for (int i = 1; i < numPoints; i++)
	{
		// Points loop
		for (int j = 0; j < numPoints - 1; j++)
		{
			pointArrayAux[j] = lerp(pointArrayAux[j], pointArrayAux[j + 1], t);
		}
	}

	return pointArrayAux[0];
}

/*
Given numCtrl control points, return numCurve points
representing the ends of the segments forming a curve
obtained using "de Casteljau algorithm".
*/
void getCurve_deCasteljau(Point2D* ctrlPts, int numCtrl, Point2D* curvePts, int* numCurve)
{
	if (numCtrl < 2)
		return;

	*numCurve = MIN(100, MAX(MAX_NUM_PTS, numCtrl * 2));

	for (int i = 0; i < *numCurve; i++)
	{
		curvePts[i] = deCasteljau(ctrlPts, numCtrl, (float)i / *numCurve);
	}
}