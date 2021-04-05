#include"polynomial.h"
#include<iostream>
#include <cmath>
#include <algorithm>

Polynomial::Polynomial()
	:min_(0), max_(0), coefficients_(new int[1]) {
	coefficients_[0] = 0;
}

Polynomial::Polynomial(int min, int max, int* coefficients)
	: min_(min), max_(max) {
	coefficients_ = new int[max_ - min_ + 1];
	for (int i = 0; i < max_ - min_ + 1; i++)
		coefficients_[i] = coefficients[i];
}

Polynomial::Polynomial(const Polynomial& other)
	: min_(other.min_), max_(other.max_) {
	coefficients_ = new int[max_ - min_ + 1];
	for (int i = 0; i < max_ - min_ + 1; i++)
		coefficients_[i] = other.coefficients_[i];
}

Polynomial& Polynomial::operator=(const Polynomial& other) {
	if (&other != this) {
		min_ = other.min_;
		max_ = other.max_;
		delete[] coefficients_;
		coefficients_ = new int[other.max_ - other.min_ + 1];
		for (int i = 0; i < max_ - min_ + 1; i++)
			coefficients_[i] = other.coefficients_[i];
	}
	return *this;
}

bool Polynomial::operator==(const Polynomial& other) const {
	int thisMin = min_;
	int thisMax = max_;
	int otherMin = other.min_;
	int otherMax = other.max_;

	while (thisMin < otherMin) {
		if (coefficients_[thisMin - min_] != 0)
			return false;
		thisMin++;
	}
	while (otherMin < thisMin) {
		if (other.coefficients_[otherMin - other.min_] != 0)
			return false;
		otherMin++;
	}

	while (thisMax > otherMax) {
		if (coefficients_[thisMax - min_] != 0)
			return false;
		thisMax--;
	}
	while (otherMax > thisMax) {
		if (other.coefficients_[otherMax - other.min_] != 0)
			return false;
		otherMax--;
	}

	if (otherMin != thisMin || otherMax != thisMax)
		return false;

	for (; thisMin != thisMax; ++thisMin, ++otherMin) {
		if (coefficients_[thisMin - min_] != other.coefficients_[otherMin - other.min_])
			return false;
	}
	return true;
}

bool Polynomial::operator!=(const Polynomial& other) const {
	return !(*this == other);
}

Polynomial& Polynomial::operator+=(const Polynomial& other) {

	int* prevCoef = new int[max_ - min_ + 1];
	for (int i = 0; i < max_ - min_ + 1; i++)
		prevCoef[i] = coefficients_[i];

	int newMin, newMax;
	other.min_ < min_ ? newMin = other.min_ : newMin = min_;
	other.max_ > max_ ? newMax = other.max_ : newMax = max_;
	
	delete[] coefficients_;
	coefficients_ = new int[newMax - newMin + 1];

	int otherBegin = other.min_;
	int otherEnd = other.max_;

	if (min_ > other.min_) {

		min_ > other.max_ ? otherBegin = other.max_ + 1: otherBegin = min_;
		for (int i = 0; i < otherBegin - other.min_; ++i)
			coefficients_[i] = other.coefficients_[i];
		for (int i = otherBegin - other.min_; i < min_ - other.min_; ++i)
			coefficients_[i] = 0;
	}

	if (max_ < other.max_) {

		max_ < other.min_ ? otherEnd = other.min_ - 1: otherEnd = max_;
		for (int i = newMax - newMin, j = other.max_ - other.min_; i > otherEnd - newMin; --i, --j)
			coefficients_[i] = other.coefficients_[j];
		for (int i = otherEnd - newMin; i > max_ - newMin; --i)
			coefficients_[i] = 0;
	}

	for (int i = min_ - newMin, j = 0; i < max_ - newMin + 1; ++i, ++j)
		coefficients_[i] = prevCoef[j];

	if (otherBegin != otherEnd) {

		for (int i = otherBegin - newMin; i < newMax - newMin + 1; ++i, ++otherBegin) {
			if (otherBegin > otherEnd)
				break;
			coefficients_[i] += other.coefficients_[otherBegin - other.min_];
		}
	}

	delete[] prevCoef;

	min_ = newMin;
	max_ = newMax;

	return *this;
}

Polynomial& Polynomial::operator*=(const int number) {
	//fixed for_each
	std::for_each(coefficients_, coefficients_ + max_ - min_ + 1,
		[&](int& coef) { coef *= number; });
	return *this;
}

Polynomial& Polynomial::operator-=(const Polynomial& other) {
	*this *= -1;
	*this += other;
	*this *= -1;
	return *this;
}

Polynomial& Polynomial::operator*=(const Polynomial& other) {

	int newMax = max_;
	int newMin = min_;
	int* prevCoef = new int[max_ - min_ + 1];
	for (int i = 0; i < max_ - min_ + 1; i++)
		prevCoef[i] = coefficients_[i];

	delete[] coefficients_;
	coefficients_ = new int[max_ - min_ + other.max_ - other.min_ + 1];
	for (int i = 0; i < max_ - min_ + 1; i++)
		coefficients_[i] = prevCoef[i];

	int thisPosition = 0;
	int thisCoef;

	for (int i = 0; i < other.max_ - other.min_ + 1; ++i, ++thisPosition) {
		
		int coef = other.coefficients_[i];
		if (coef == 0)
			continue;

		if (thisPosition == 0) {
			*this *= coef;
			newMax += other.min_;
			newMin += other.min_;
		}
		else {
			thisCoef = thisPosition;
			for (int j = 0; j < max_ - min_ + 1; ++j, ++thisCoef) {
				if (thisCoef != newMax - newMin + 1) {
					coefficients_[thisCoef] += (prevCoef[j] * coef);
				}
				else {
					coefficients_[thisCoef] = prevCoef[j] * coef;
					newMax++;
				}
			}
		}
	}

	max_ = newMax;
	min_ = newMin;

	delete[] prevCoef;

	return *this;
}

Polynomial& Polynomial::operator/=(const int number) {
	std::for_each(coefficients_, coefficients_ + max_ - min_ + 1,
		[&](int& coef) { coef /= number; });
	return *this;
}

Polynomial Polynomial::operator+(const Polynomial& other) const {
	Polynomial outPolynom = *this;
	outPolynom += other;
	return outPolynom;
}

Polynomial Polynomial::operator-(const Polynomial& other) const {
	Polynomial outPolynom = *this;
	outPolynom -= other;
	return outPolynom;
}

Polynomial Polynomial::operator*(const int number) const {
	Polynomial outPolynom = *this;
	outPolynom *= number;
	return outPolynom;
}

Polynomial operator*(const int number, const Polynomial object) {
	return object * number;
}

Polynomial Polynomial::operator*(const Polynomial& other) const {
	Polynomial outPolynom = *this;
	outPolynom *= other;
	return outPolynom;
}

Polynomial Polynomial::operator/(const int number) const {
	Polynomial outPolynom = *this;
	outPolynom /= number;
	return outPolynom;
}

int& Polynomial::operator[](const int degree) {

	if (degree > max_) {

		int* prevCoef = new int[max_ - min_ + 1];
		for (int i = 0; i < max_ - min_ + 1; ++i)
			prevCoef[i] = coefficients_[i];

		delete[] coefficients_;
		coefficients_ = new int[degree - min_ + 1];
		for (int i = 0; i < degree - min_ + 1; ++i)
			i <= max_ - min_ ? coefficients_[i] = prevCoef[i] : coefficients_[i] = 0;

		max_ = degree;
		delete[] prevCoef;
	}
	else if (degree < min_) {

		int* prevCoef = new int[max_ - min_ + 1];
		for (int i = 0; i < max_ - min_ + 1; ++i)
			prevCoef[i] = coefficients_[i];

		delete[] coefficients_;
		coefficients_ = new int[max_ - degree + 1];
		for (int i = max_ - degree; i >= 0; --i)
			i >= min_ - degree ? coefficients_[i] = prevCoef[i - min_ + degree] : coefficients_[i] = 0;

		max_ = degree;
		delete[] prevCoef;
	}
	return coefficients_[degree - min_];
}

int Polynomial::operator[](const int degree) const {
	if (degree > max_ || degree < min_)
		return 0;
	return coefficients_[degree - min_];
}

Polynomial operator-(const Polynomial& other) {
	Polynomial outPolynom = other;
	outPolynom *= -1;
	return outPolynom;
}

std::stringstream& operator<<(std::stringstream& polynom, const Polynomial& object) {
	int power = object.min_;
	std::string str;

	for (int i = 0; i < object.max_ - object.min_ + 1; ++i) {
		int coef = object.coefficients_[i];
		if (coef != 0) {
			if (power != 0)
				power == 1 ? str.insert(0, "x") : str.insert(0, "x^" + std::to_string(power));
			if (coef != 1 && coef != -1)
				str.insert(0, std::to_string(coef));
			else {
				if (power == 0)
					str.insert(0, std::to_string(coef));
				if (coef < 0)
					str.insert(0, "-");
			}
			if (power != object.max_ && coef > 0)
				str.insert(0, "+");
		}
		power++;
	}
	if (str.empty())
		str = "0";
	polynom << str;
	return polynom;
}

std::ifstream& operator>>(std::ifstream& in, Polynomial& object) {
	in >> object.min_;
	in >> object.max_;
	int coef;
	delete[] object.coefficients_;
	object.coefficients_ = new int[object.max_ - object.min_ + 1];
	for (int i = 0; i < object.max_ - object.min_ + 1; ++i) {
		if (!in.eof()) {
			in >> coef;
			object.coefficients_[i] = coef;
		}
		else
			object.coefficients_[i] = 0;
	}
	return in;
}

//fixed get O(n)
double Polynomial::get(const int x) const {

	double res = 0;
	double powX = pow(x, min_);
	for (int i = 0; i < max_ - min_ + 1; ++i) {
		res += (coefficients_[i] * powX);
		powX *= x;
	}
	return res;
}