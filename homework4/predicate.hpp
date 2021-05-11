#ifndef PREDICATE_H
#define PREDICATE_H

#include <iostream>
//fixed T is a bad name for iterator type
template<class Iterator, class Func>
bool allOf(const Iterator& itBegin, const Iterator& itEnd, Func func) {
	for (auto it = itBegin; it != itEnd; ++it) {
		if (!func(*it))
			return false;
	}
	return true;
}

template<class Iterator, class Func>
bool anyOf(const Iterator& itBegin, const Iterator& itEnd, Func func) {
	for (auto it = itBegin; it != itEnd; ++it) {
		if (func(*it))
			return true;
	}
	return false;
}

template<class Iterator, class Func>
bool noneOf(const Iterator& itBegin, const Iterator& itEnd, Func func) {
	return !anyOf(itBegin, itEnd, func);
}

template<class Iterator, class Func>
bool oneOf(const Iterator& itBegin, const Iterator& itEnd, Func func) {
	bool answer = false;
	for (auto it = itBegin; it != itEnd; ++it) {
		if (func(*it) && !answer)
			answer = true;
		else if (func(*it) && answer)
			return !answer;
	}
	return answer;
}

template<class Iterator, class Func = std::less<>>
bool isSorted(const Iterator& itBegin, const Iterator& itEnd, Func func = Func()) {
	for (auto it = itBegin + 1; it != itEnd; ++it) {
		if (!func(*(it - 1), *it))
			return false;
	}
	return true;
}

template<class Iterator, class Func>
bool isPartitioned(const Iterator& itBegin, const Iterator& itEnd, Func func = Func()) {
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

template<class Iterator>
Iterator findNot(const Iterator& itBegin, const Iterator& itEnd, int value) {
	for (auto it = itBegin + 1; it != itEnd; ++it) {
		if (*it != value)
			return it;
	}
	return itEnd;
}

template<class Iterator>
Iterator findBackward(const Iterator& itBegin, const Iterator& itEnd, int value) {
	auto answer = itEnd;
	for (auto it = itBegin; it != itEnd; it++) {
		if (*it == value)
			answer = it;
	}
	return answer;
}

template<class Iterator, class Func>
bool isPalindrome(Iterator itBegin, Iterator itEnd, Func func) {

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