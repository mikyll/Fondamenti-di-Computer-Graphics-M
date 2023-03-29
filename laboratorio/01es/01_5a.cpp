#include "01_5a.h"

static Point2D catmullRomSpline(Point2D p0, Point2D p1, Point2D p2, Point2D p3, float t);


static Point2D catmullRomSpline(Point2D p0, Point2D p1, Point2D p2, Point2D p3, float t)
{
	Point2D res;

	res.x = 0.5 * (
		(2 * p1.x) +
		(-p0.x + p2.x) * t +
		(2 * p0.x - 5 * p1.x + 4 * p2.x - p3.x) * t * t +
		(-p0.x + 3 * p1.x - 3 * p2.x + p3.x) * t * t * t
	);
	res.y = 0.5 * (
		(2 * p1.y) +
		(-p0.y + p2.y) * t +
		(2 * p0.y - 5 * p1.y + 4 * p2.y - p3.y) * t * t +
		(-p0.y + 3 * p1.y - 3 * p2.y + p3.y) * t * t * t
	);

	return res;
}

void getCurve_CatmullRomSpline(Point2D* ctrlPts, int numCtrl, Point2D* curvePts, int* numCurve)
{
	// NB: we need at least 4 control points
	if (numCtrl < 4)
		return;

	Point2D p1, p2, p3, p4;
	int ptsPerSpline = ((MAX_NUM_PTS * 2) - 2) / (numCtrl * 4);

	*numCurve = 0;

	for (int i = 1; i < numCtrl - 2; i++)
	{
		p1 = ctrlPts[i - 1];
		p2 = ctrlPts[i];
		p3 = ctrlPts[i + 1];
		p4 = ctrlPts[i + 2];

		for (int j = 0; j <= ptsPerSpline; j++)
		{
			float t = (float)j / ptsPerSpline;
			curvePts[*numCurve] = catmullRomSpline(p1, p2, p3, p4, t);
			(*numCurve)++;
		}
	}
}

static void catmullRomSpline2(Point2D p0, Point2D p1, Point2D p2, Point2D p3)
{
	Point2D resP1, resP2;

	

	/*res.x = 0.5 * (
		(2 * p1.x) +
		(-p0.x + p2.x) * t +
		(2 * p0.x - 5 * p1.x + 4 * p2.x - p3.x) * t * t +
		(-p0.x + 3 * p1.x - 3 * p2.x + p3.x) * t * t * t
		);
	res.y = 0.5 * (
		(2 * p1.y) +
		(-p0.y + p2.y) * t +
		(2 * p0.y - 5 * p1.y + 4 * p2.y - p3.y) * t * t +
		(-p0.y + 3 * p1.y - 3 * p2.y + p3.y) * t * t * t
		);*/

	//return res;
}

void getCurve_CatmullRomSpline2(Point2D* ctrlPts, int numCtrl, Point2D* curvePts, int* numCurve)
{
	// NB: we need at least 4 control points
	if (numCtrl < 4)
		return;

	Point2D p1, p2, p3, p4;
	int ptsPerSpline = ((MAX_NUM_PTS * 2) - 2) / (numCtrl * 4);

	*numCurve = 0;

	for (int i = 1; i < numCtrl - 2; i++)
	{
		p1 = ctrlPts[i - 1];
		p2 = ctrlPts[i];
		p3 = ctrlPts[i + 1];
		p4 = ctrlPts[i + 2];

		for (int j = 0; j <= ptsPerSpline; j++)
		{
			float t = (float)j / ptsPerSpline;
			curvePts[*numCurve] = catmullRomSpline(p1, p2, p3, p4, t);
			(*numCurve)++;
		}
	}
}