#ifndef GEOMETRY_H
#define GEOMETRY_H

class Point {
private:
	int x_;
	int y_;
public:
	Point();
	explicit Point(int, int);
	Point(const Point&);
	Point& operator=(const Point&);
	int getX() const;
	int getY() const;
};

class PolygonalChain {
private:
	int n_;
	Point* points_;
public:
	PolygonalChain();
	explicit PolygonalChain(int, Point*);
	PolygonalChain(const PolygonalChain&);
	virtual ~PolygonalChain();
	PolygonalChain& operator=(const PolygonalChain&);
	int getN() const;
	Point& getPoint(int) const;
	virtual double perimeter() const;
};

class ClosedPolygonalChain : public PolygonalChain {
public:
	ClosedPolygonalChain();
	explicit ClosedPolygonalChain(int, Point*);
	virtual double perimeter() const override;
};

class Polygon : public ClosedPolygonalChain {
public:
	Polygon();
	Polygon(int, Point*);
	virtual double area() const;
};

class Triangle : public Polygon {
public:
	Triangle();
	Triangle(int, Point*);
	bool hasRightAngle() const;
};

class Trapezoid : public Polygon {
public:
	Trapezoid();
	Trapezoid(int, Point*);
	double height() const;
};

class RegularPolygon : public Polygon {
public:
	RegularPolygon();
	RegularPolygon(int, Point*);
	double perimeter() const override;
	double area() const override;
};

#endif