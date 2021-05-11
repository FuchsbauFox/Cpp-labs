#ifndef PREDICATE_H
#define PREDICATE_H

#include <iostream>

template<class T, class Func>
bool allOf(const T& itBegin, const T& itEnd, Func func) {
	for (auto it = itBegin; it != itEnd; ++it) {
		if (!func(*it))
			return false;
	}
	return true;
}

template<class T, class Func>
bool anyOf(const T& itBegin, const T& itEnd, Func func) {
	for (auto it = itBegin; it != itEnd; ++it) {
		if (func(*it))
			return true;
	}
	return false;
}

template<class T, class Func>
bool noneOf(const T& itBegin, const T& itEnd, Func func) {
	return !anyOf(itBegin, itEnd, func);
}

template<class T, class Func>
bool oneOf(const T& itBegin, const T& itEnd, Func func) {
	bool answer = false;
	for (auto it = itBegin; it != itEnd; ++it) {
		if (func(*it) && !answer)
			answer = true;
		else if (func(*it) && answer)
			return !answer;
	}
	return answer;
}

template<class T, class Func = std::less<>>
bool isSorted(const T& itBegin, const T& itEnd, Func func = Func()) {
	for (auto it = itBegin + 1; it != itEnd; ++it) {
		if (!func(*(it - 1), *it))
			return false;
	}
	return true;
}

template<class T, class Func>
bool isPartitioned(const T& itBegin, const T& itEnd, Func func = Func()) {
	bool div = false;
	for (auto it = itBegin + 1; it != itEnd; ++it) {
		if ((func(*(it - 1)) && !func(*it)) || (!func(*(it - 1)) && func(*it))) {
			if (div)
				return false;
			div = true;
		}
	}
	return true;
}

template<class T>
T findNot(const T& itBegin, const T& itEnd, int value) {
	for (auto it = itBegin + 1; it != itEnd; ++it) {
		if (*it != value)
			return it;
	}
	return itEnd;
}

template<class T>
T findBackward(const T& itBegin, const T& itEnd, int value) {
	auto answer = itEnd;
	for (auto it = itBegin; it != itEnd; it++) {
		if (*it == value)
			answer = it;
	}
	return answer;
}

template<class T, class Func>
bool isPalindrome(T itBegin, T itEnd, Func func) {

	itEnd--;
	while (itBegin != itEnd) {
		itEnd++;
		if (itBegin != itEnd)
			itEnd--;
		else
			break;
		if (!func(*itBegin, *itEnd)) {
			return false;
		}
		itEnd--;
		itBegin++;
	}

	return true;
}

#endif