#pragma once

#include <boost\optional.hpp>
#include <cmath>
#include <vector>

int Compare(double a, double b);

inline bool IsEqual(double a, double b)
{
	return Compare(a, b) == 0;
}

struct Point2D
{
	double x;
	double y;

	Point2D(double x, double y) : x(x), y(y) {}

	bool operator <(const Point2D& pt) const
	{
		return (Compare(x, pt.x) == -1) || (IsEqual(x, pt.x) && Compare(y, pt.y) == -1);
	}

	bool operator ==(const Point2D& pt) const
	{
		return !(*this < pt) && !( pt < *this);
	}
};

class LinearEquation   // A*x + B*y + C = 0
{
	double A = 0;
	double B = 0;
	double C = 0;

public:
	LinearEquation(double a, double b, double c) : A(a), B(b), C(c) 
	{}

	LinearEquation(const Point2D& lhs, const Point2D& rhs);
	
	LinearEquation GetNormal();

	boost::optional<Point2D> intersectPoint(const LinearEquation& eq);

private:

	bool valid() const;

};

bool pointsMakeCircle(const std::vector<Point2D>& pts);
