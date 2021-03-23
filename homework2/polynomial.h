#include<vector>
#include <string>

#ifndef POLYNOMIAL_H
#define POLYNOMIAL_H

class Polynomial {
private:
	int min_;
	int max_;
	std::vector<int> coefficients_;
public:
	Polynomial();
	explicit Polynomial(int, int, int*);
	Polynomial(const Polynomial&);

	Polynomial& operator=(const Polynomial&);
	bool operator==(const Polynomial&) const;
	bool operator!=(const Polynomial&) const;

	Polynomial& operator+=(const Polynomial&);
	Polynomial operator+(const Polynomial&) const;
	Polynomial& operator-=(const Polynomial&);
	Polynomial operator-(const Polynomial&) const;

	Polynomial& operator*=(const int);
	Polynomial operator*(const int) const;
	friend Polynomial operator*(const int, const Polynomial);
	Polynomial& operator/=(const int);
	Polynomial operator/(const int) const;

	Polynomial& operator*=(const Polynomial&);
	Polynomial operator*(const Polynomial&) const;


	int& operator[](const int);
	int operator[](const int) const;

	friend Polynomial operator-(Polynomial);

	friend std::stringstream& operator<<(std::stringstream&, const Polynomial&);
	friend std::ifstream& operator>>(std::ifstream&, Polynomial&);

	double get(const int) const;
};

#endif