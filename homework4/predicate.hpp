#ifndef PREDICATE_H
#define PREDICATE_H

#include <iostream>
//fixed T is a bad name for iterator type
template<class Iter_type, class Func>
bool allOf(const Iter_type& itBegin, const Iter_type& itEnd, Func func) {
	for (auto it = itBegin; it != itEnd; ++it) {
		if (!func(*it))
			return false;
	}
	return true;
}

template<class Iter_type, class Func>
bool anyOf(const Iter_type& itBegin, const Iter_type& itEnd, Func func) {
	for (auto it = itBegin; it != itEnd; ++it) {
		if (func(*it))
			return true;
	}
	return false;
}

template<class Iter_type, class Func>
bool noneOf(const Iter_type& itBegin, const Iter_type& itEnd, Func func) {
	return !anyOf(itBegin, itEnd, func);
}

template<class Iter_type, class Func>
bool oneOf(const Iter_type& itBegin, const Iter_type& itEnd, Func func) {
	bool answer = false;
	for (auto it = itBegin; it != itEnd; ++it) {
		if (func(*it) && !answer)
			answer = true;
		else if (func(*it) && answer)
			return !answer;
	}
	return answer;
}

template<class Iter_type, class Func = std::less<>>
bool isSorted(const Iter_type& itBegin, const Iter_type& itEnd, Func func = Func()) {
	for (auto it = itBegin + 1; it != itEnd; ++it) {
		if (!func(*(it - 1), *it))
			return false;
	}
	return true;
}

template<class Iter_type, class Func>
bool isPartitioned(const Iter_type& itBegin, const Iter_type& itEnd, Func func = Func()) {
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

template<class Iter_type>
Iter_type findNot(const Iter_type& itBegin, const Iter_type& itEnd, int value) {
	for (auto it = itBegin + 1; it != itEnd; ++it) {
		if (*it != value)
			return it;
	}
	return itEnd;
}

template<class Iter_type>
Iter_type findBackward(const Iter_type& itBegin, const Iter_type& itEnd, int value) {
	auto answer = itEnd;
	for (auto it = itBegin; it != itEnd; it++) {
		if (*it == value)
			answer = it;
	}
	return answer;
}

template<class Iter_type, class Func>
bool isPalindrome(Iter_type itBegin, Iter_type itEnd, Func func) {

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