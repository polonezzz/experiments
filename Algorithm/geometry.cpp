#include "geometry.h"

#include <algorithm>

int Compare(double a, double b)
{
	auto diff = a - b;
	return std::isless(abs(a - b), 1e-3) ? 0 : (std::signbit(diff) ? -1 : 1);
}

LinearEquation::LinearEquation(const Point2D& pt1, const Point2D& pt2)
{
	A = IsEqual(pt2.y, pt1.y) ? 0.0 : pt2.y - pt1.y;
	B = IsEqual(pt2.x, pt1.x) ? 0.0 : pt1.x - pt2.x;
	C = - (A * pt1.x + B * pt1.y);
}

LinearEquation LinearEquation::GetNormal(const Point2D& pt)
{
	return LinearEquation(-B, A, B * pt.x - A * pt.y);   
}

bool LinearEquation::valid() const
{
	return !(IsEqual(A, 0.0) && IsEqual(B, 0.0));	
}

boost::optional<Point2D> LinearEquation::intersectPoint(const LinearEquation& eq)
{	
	if (!valid() || !eq.valid())
		return boost::optional<Point2D>();
	
	double det = A * eq.B - B * eq.A;
	double detX = B * eq.C - C * eq.B;
	double detY = C * eq.A - A * eq.C;

	if (IsEqual(det, 0.0))
	{
		if (IsEqual(detX, 0.0) && IsEqual(detY, 0.0))
			return boost::make_optional(IsEqual(B, 0.0) ? Point2D(-C / A, 0.0) : Point2D(0.0, -C / B));
		else
			return boost::optional<Point2D>();
	}
	else
	{
		return boost::make_optional(Point2D(detX/det, detY/det));	
	}
}

bool pointsMakeCircle(const std::vector<Point2D>& points)
{
	auto pts = points;
	std::sort(pts.begin(), pts.end());
	pts.erase(std::unique(pts.begin(), pts.end()), pts.end());

	if (pts.empty())
		return false;

	if (pts.size() < 3)
		return true;

	size_t i = 0;
	auto eq = LinearEquation(pts[i+1], pts[i]).GetNormal({(pts[i+1].x + pts[i].x) / 2, (pts[i+1].y + pts[i].y) / 2});

	++i;
	auto eq2 = LinearEquation(pts[i+1], pts[i]).GetNormal({(pts[i+1].x + pts[i].x) / 2, (pts[i+1].y + pts[i].y) / 2});
	
	auto pt = eq2.intersectPoint(eq);

	if (!pt)
		return false;

	auto radiusSqr = [&pt](const Point2D& data) 
						{ 
							return std::pow((data.x - pt->x), 2) + std::pow((data.y - pt->y), 2); 
						};
	
	const double distance = radiusSqr(pts[0]);

	return std::all_of(pts.begin() + 3, pts.end(), [distance, &radiusSqr](const auto& pt) { return IsEqual(distance, radiusSqr(pt)); });
}