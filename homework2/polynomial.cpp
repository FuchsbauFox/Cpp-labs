#include"polynomial.h"
#include<iostream>
#include <cmath>
#include <sstream>
#include <fstream>

Polynomial::Polynomial()
	:min_(0), max_(0) {
	coefficients_.push_back(0);
}

Polynomial::Polynomial(int min, int max, int* coefficients)
	: min_(min), max_(max) {
	for (int i = 0; i < max_ - min_ + 1; i++)
		coefficients_.push_back(coefficients[i]);
}

Polynomial::Polynomial(const Polynomial& other)
	: min_(other.min_), max_(other.max_) {
	for (auto coef : other.coefficients_)
		coefficients_.push_back(coef);
}

Polynomial& Polynomial::operator=(const Polynomial& other) {
	if (&other != this) {
		min_ = other.min_;
		max_ = other.max_;
		coefficients_.clear();
		for (auto coef : other.coefficients_)
			coefficients_.push_back(coef);
	}
	return *this;
}

bool Polynomial::operator==(const Polynomial& other) const {
	int thisMin = min_;
	int thisMax = max_;
	std::vector<int>::const_iterator itThisBegin = coefficients_.begin();
	std::vector<int>::const_iterator itThisEnd = coefficients_.end() - 1;
	int otherMin = other.min_;
	int otherMax = other.max_;
	std::vector<int>::const_iterator itOtherBegin = other.coefficients_.begin();
	std::vector<int>::const_iterator itOtherEnd = other.coefficients_.end() - 1;
	while (thisMin < otherMin) {
		if (*itThisBegin != 0)
			return false;
		thisMin++;
		itThisBegin++;
	}
	while (otherMin < thisMin) {
		if (*itOtherBegin != 0)
			return false;
		otherMin++;
		itOtherBegin++;
	}

	while (thisMax > otherMax) {
		if (*itThisEnd != 0)
			return false;
		thisMax--;
		itThisEnd--;
	}
	while (otherMax > thisMax) {
		if (*itOtherEnd != 0)
			return false;
		otherMax--;
		itOtherEnd--;
	}

	for (; itThisBegin != itThisEnd; ++itThisBegin) {
		if (*itThisBegin != *itOtherBegin)
			return false;
		itOtherBegin++;
	}
	return true;
}

bool Polynomial::operator!=(const Polynomial& other) const {
	return !(*this == other);
}

Polynomial& Polynomial::operator+=(const Polynomial& other) {

	std::vector<int>::const_iterator itOtherBegin = other.coefficients_.begin();
	std::vector<int>::const_iterator itOtherEnd = other.coefficients_.end();
	std::vector<int>::iterator itThisBegin = coefficients_.begin();

	if (min_ > other.min_) {
		min_ > other.max_ ? itOtherBegin = other.coefficients_.end() : itOtherBegin = other.coefficients_.begin() + min_ - other.min_;
		coefficients_.insert(coefficients_.begin(), other.coefficients_.begin(), itOtherBegin);
		if (min_ > other.max_ + 1) {
			for (int i = 0; i < min_ - other.max_ - 1; ++i)
				coefficients_.insert(coefficients_.begin() + min_ - other.max_ + 1, 0);
		}
		itThisBegin = coefficients_.begin() + min_ - other.min_;
		min_ = other.min_;
	}
	if (max_ < other.max_) {
		if (max_ + 1 < other.min_) {
			itOtherEnd = other.coefficients_.begin();
			coefficients_.insert(coefficients_.end(), other.min_ - max_ - 1, 0);
		}
		else
			itOtherEnd = other.coefficients_.end() - other.max_ + max_;
		coefficients_.insert(coefficients_.end(), itOtherEnd, other.coefficients_.end());
		itThisBegin = coefficients_.begin();
		max_ = other.max_;
	}
	if (itOtherBegin != itOtherEnd) {
		for (itThisBegin += other.min_ - min_; itOtherBegin != itOtherEnd; itThisBegin++, itOtherBegin++) {
			*itThisBegin += *itOtherBegin;
		}
	}
	return *this;
}

Polynomial Polynomial::operator+(const Polynomial& other) const {
	Polynomial outPolynom = *this;
	outPolynom += other;
	return outPolynom;
}

Polynomial& Polynomial::operator-=(const Polynomial& other) {
	*this *= -1;
	*this += other;
	*this *= -1;
	return *this;
}

Polynomial Polynomial::operator-(const Polynomial& other) const {
	Polynomial outPolynom = *this;
	outPolynom -= other;
	return outPolynom;
}

Polynomial& Polynomial::operator*=(const int number) {
	for (auto& coef : coefficients_)
		coef *= number;
	return *this;
}

Polynomial Polynomial::operator*(const int number) const {
	Polynomial outPolynom = *this;
	outPolynom *= number;
	return outPolynom;
}

Polynomial operator*(const int number, const Polynomial object) {
	return object * number;
}

Polynomial& Polynomial::operator/=(const int number) {
	for (auto& coef : coefficients_) {
		coef /= number;
	}
	return *this;
}

Polynomial Polynomial::operator/(const int number) const {
	Polynomial outPolynom = *this;
	outPolynom /= number;
	return outPolynom;
}

Polynomial& Polynomial::operator*=(const Polynomial& other) {

	if (other.min_ - other.max_ == 0)
		*this = other;
	if (max_ - min_ == 0)
		return *this;


	std::vector<int>::iterator itThisPosition = coefficients_.begin();
	std::vector<int>::iterator itThisCoefs;
	std::vector<int> pastCoefficients;
	pastCoefficients.insert(pastCoefficients.begin(), coefficients_.begin(), coefficients_.end());
	int thisPosition = other.min_ + min_;

	for (auto coef : other.coefficients_) {
		if (coef != 0) {
			if (itThisPosition == coefficients_.begin()) {
				*this *= coef;
				max_ += other.min_;
				min_ += other.min_;
			}
			else {
				itThisCoefs = itThisPosition;
				for (auto pastCoef : pastCoefficients) {
					if (itThisCoefs != coefficients_.end()) {
						*itThisCoefs += (pastCoef * coef);
						itThisCoefs++;
					}
					else {
						coefficients_.push_back(pastCoef * coef);
						itThisPosition = coefficients_.begin() + thisPosition - min_;
						itThisCoefs = coefficients_.end();
						max_++;
					}
				}
			}
		}
		thisPosition++;
		itThisPosition++;
	}
	return *this;
}

Polynomial Polynomial::operator*(const Polynomial& other) const {
	Polynomial outPolynom = *this;
	outPolynom *= other;
	return outPolynom;
}


int& Polynomial::operator[](const int power) {
	std::vector<int>::iterator out;
	if (power > max_) {
		coefficients_.insert(coefficients_.end(), power - max_, 0);
		max_ = power;
	}
	else if (power < min_) {
		coefficients_.insert(coefficients_.begin(), min_ - power, 0);
		min_ = power;
		return *coefficients_.begin();
	}
	return coefficients_[power - min_];
}

int Polynomial::operator[](const int power) const {
	if (power > max_ || power < min_)
		return 0;
	return coefficients_[power];
}

Polynomial operator-(Polynomial other) {
	Polynomial outPolynom = other;
	outPolynom *= -1;
	return outPolynom;
}

std::stringstream& operator<<(std::stringstream& polynom, const Polynomial& object) {
	int power = object.min_;
	std::string str;
	for (auto coef : object.coefficients_) {
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
	while (!in.eof()) {
		in >> coef;
		object.coefficients_.push_back(coef);
	}
	return in;
}

double Polynomial::get(const int x) const {
	double res = 0;
	int power = min_;
	for (auto coef : coefficients_) {
		res += (coef * pow(x, power));
		power++;
	}
	return res;
}