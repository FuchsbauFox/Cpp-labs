#include "geometry.h"
#include <cmath>
#include <iostream>

#define M_PI 3.14159265358979323846

//class Point

Point::Point()
	:x_(0), y_(0) {}

Point::Point(int x, int y)
	: x_(x), y_(y) {}

Point::Point(const Point& other)
	: x_(other.x_), y_(other.y_) {}

Point& Point::operator=(const Point& other) {
	if (&other != this) {
		x_ = other.x_;
		y_ = other.y_;
	}
	return *this;
}

int Point::getX() const {
	return x_;
}

int Point::getY() const {
	return y_;
}

//side length counting function

//fixed const
double edgePow(const Point& point1, const Point& point2) {
	return pow(point1.getX() - point2.getX(), 2) + pow(point1.getY() - point2.getY(), 2);
}

//class PolygonalChain

PolygonalChain::PolygonalChain()
	: n_(0), points_(nullptr) {}

PolygonalChain::PolygonalChain(int n, Point* points)
	: n_(n) {
	if (n < 2) {
		std::cout << "error: not enough points\n";
		exit(3);
	}
	points_ = new Point[n_];
	for (int i = 0; i < n_; i++) {
		points_[i] = points[i];
	}
}

PolygonalChain::PolygonalChain(const PolygonalChain& other)
	: n_(other.n_) {
	points_ = new Point[n_];
	for (int i = 0; i < n_; i++) {
		points_[i] = other.points_[i];
	}
}

PolygonalChain::~PolygonalChain() {
	delete[] points_;
}

PolygonalChain& PolygonalChain::operator=(const PolygonalChain& other) {
	if (&other != this) {
		n_ = other.n_;
		delete[] points_;
		points_ = new Point[n_];
		for (int i = 0; i < n_; i++) {
			points_[i] = other.points_[i];
		}
	}
	return *this;
}

int PolygonalChain::getN() const {
	return n_;
}

Point& PolygonalChain::getPoint(int pos) const {
	if (pos < n_)
		return points_[pos];
	std::cout << "error: out of range of array points\n";
	exit(1);
}

double PolygonalChain::perimeter() const {
	double sum = 0;
	for (int i = 1; i < n_; i++) {
		sum += sqrt(edgePow(points_[i - 1], points_[i]));
	}
	return sum;
}

//class ClosedPolygonalChain

ClosedPolygonalChain::ClosedPolygonalChain()
	: PolygonalChain(0, nullptr) {}

ClosedPolygonalChain::ClosedPolygonalChain(int n, Point* points)
	: PolygonalChain(n, points) {
	if (n < 3) {
		std::cout << "error: not enough points\n";
		exit(3);
	}
}

double ClosedPolygonalChain::perimeter() const {
	return PolygonalChain::perimeter() + sqrt(edgePow(getPoint(0), getPoint(getN() - 1)));
}

//class Polygon

Polygon::Polygon()
	: ClosedPolygonalChain() {}

Polygon::Polygon(int n, Point* points)
	: ClosedPolygonalChain(n, points) {}

double Polygon::area() const {
	int s = 0;
	//fixed remove doubles
	for (int i = 0; i < getN(); i++) {
		if (i + 1 == getN()) {
			s = s + getPoint(i).getX() * getPoint(0).getY() - getPoint(0).getX() * getPoint(i).getY();
			break;
		}
		s = s + getPoint(i).getX() * getPoint(i + 1).getY() - getPoint(i + 1).getX() * getPoint(i).getY();
	}
	return abs(1.0 * s / 2);
}

//class Triangle

Triangle::Triangle()
	: Polygon() {}

Triangle::Triangle(int n, Point* points)
	: Polygon(n, points) {}

//fixed without sqrt
bool Triangle::hasRightAngle() const {
	double sideA = edgePow(getPoint(0), getPoint(1));
	double sideB = edgePow(getPoint(1), getPoint(2));
	double sideC = edgePow(getPoint(2), getPoint(0));
	//fixed return expression
	return (sideA == sideB + sideC || sideB == sideA + sideC || sideC == sideB + sideA);
}

//class Trapezoid

Trapezoid::Trapezoid()
	: Polygon() {}

Trapezoid::Trapezoid(int n, Point* points)
	: Polygon(n, points) {
	if (n < 4) {
		std::cout << "error: not enough points\n";
		exit(3);
	}
}

double Trapezoid::height() const {
	double sideB = sqrt(edgePow(getPoint(1), getPoint(2)));
	double sideD = sqrt(edgePow(getPoint(3), getPoint(0)));
	return 2 * area() / (sideB + sideD);
}

//class RegularPolygon

//fixed regular polygon perimeter area
RegularPolygon::RegularPolygon()
	: Polygon() {}

RegularPolygon::RegularPolygon(int n, Point* points)
	: Polygon(n, points) {}

double RegularPolygon::perimeter() const {
	return sqrt(edgePow(getPoint(1), getPoint(2))) * getN();
}

double RegularPolygon::area() const {
	double a = sqrt(edgePow(getPoint(1), getPoint(2))) / (2 * tan(M_PI / getN()));
	return a * perimeter() / 2;
}