#include "01_5b.h"

static float distance(Point2D lineStart, Point2D lineEnd, Point2D point);
static void deCasteljauControlPoints(Point2D* points, int numPoints, float t, Point2D* resCtrlPts, int* numRes);
static void adaptiveSubdivision(Point2D* tmpPtsArray, int numPts);

static Point2D curveSegments[(MAX_NUM_PTS * 2) - 1];
static int numSeg = 0;

static float flatTolerance = 0.001f; // (> 0)

/*
Return the distance of a point from a segment (lineEnd - lineStart)
*/
static float distance(Point2D lineStart, Point2D lineEnd, Point2D point)
{
	// Get line in implicit form. L: Ax + By + C = 0
	typedef struct {
		float A, B, C;
	} ImplicitLine;

	ImplicitLine line = {
		lineEnd.y - lineStart.y,
		lineStart.x - lineEnd.x,
		lineEnd.x * lineStart.y - lineStart.x * lineEnd.y,
	};

	return abs(line.A * point.x + line.B * point.y + line.C) / sqrt(line.A * line.A + line.B * line.B);
}

// We need an array of the control point of the curve
static void deCasteljauControlPoints(Point2D* points, int numPoints, float t, Point2D* resCtrlPts, int* numRes)
{
	// NB: res size is ALWAYS (numInitCtrlPts * 2) + 1
	*numRes = (numPoints * 2) - 1;

	// First and last are already known
	resCtrlPts[0] = points[0];
	resCtrlPts[*numRes - 1] = points[numPoints - 1];

	// Auxiliary array (since we edit it in each iteration, we don't want to overwrite the initial array)
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

		// Save first and last control point for each "depth level"
		resCtrlPts[i] = pointArrayAux[0];
		resCtrlPts[*numRes - i - 1] = pointArrayAux[numPoints - i - 1];
	}
}

/*
Give the Control Points of a curve obtained with the Adaptive Subdivision algorithm
*/
static void adaptiveSubdivision(Point2D* tmpPtsArray, int numPts)
{
	bool isFlat = true; // if true we consider the segment as "flat"

	// Extract external Control Points p1 and p2: 0, N-1
	Point2D p1 = tmpPtsArray[0];
	Point2D pN = tmpPtsArray[numPts - 1];

	// Flat Test: if each point between p1 and pN is <= planary tollerance => we consider the segment (p1,pN) flat
	for (int i = 1; i < numPts - 1; i++)
	{
		// Flat Test: Calculate distance tmpPtsArray[i] from line
		float dist = distance(p1, pN, tmpPtsArray[i]);
		if (dist > flatTolerance)
		{
			isFlat = false;
			break;
		}
	}

	if (isFlat)
	{
		// Draw the segment, as we consider it for the curve
		curveSegments[numSeg] = p1;
		curveSegments[numSeg + 1] = pN;

		numSeg++;
	}
	else
	{
		// Divide the curve into 2
		Point2D res[(MAX_NUM_PTS * 2) - 1];
		int num;
		Point2D subd_1[MAX_NUM_PTS] = { 0.0f, 0.0f };
		Point2D subd_2[MAX_NUM_PTS] = { 0.0f, 0.0f };

		// Apply deCasteljau in t=0.5 saving CtrlPts of the 2 new curves
		deCasteljauControlPoints(tmpPtsArray, numPts, 0.5f, res, &num);

		for (int i = 0; i < num; i++)
		{
			if (i < numPts)
			{
				subd_1[i] = res[i];
			}
			if (i >= numPts-1)
			{
				subd_2[i - numPts+1] = res[i];
			}
		}

		adaptiveSubdivision(subd_1, numPts);
		adaptiveSubdivision(subd_2, numPts);
	}
}

/*
Given numCtrl control points, obtain numCurve points
which represents the ends of the segments forming a curve
obtained using the "adaptive subdivision algorithm".
*/
void getCurve_adaptiveSubdivision(Point2D* ctrlPts, int numCtrl, Point2D* curvePts, int* numCurve)
{
	if (numCtrl < 2)
		return;

	// Since the function is recursive, we need an auxiliary function in order to assing it to the function pointer "getCurve"
	numSeg = 0;
	adaptiveSubdivision(ctrlPts, numCtrl);

	for (int i = 0; i < numSeg; i++)
	{
		curvePts[i] = curveSegments[i];
	}
	*numCurve = numSeg;
}