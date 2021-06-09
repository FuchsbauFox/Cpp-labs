#ifndef ITERATOR_H
#define ITERATOR_H

#include <iostream>
#include <fstream>
#include <iterator>

using namespace std;

template<class T>
class Iterator {
private:
    T* _begin;
    int _position;
    int _currentPos;
    int _size;

public:
    using iterator_category = random_access_iterator_tag;
    using value_type = T;
    using difference_type = typename iterator<random_access_iterator_tag, T>::difference_type;
    using pointer = value_type*;
    using reference = value_type&;

    Iterator(T* begin, int firstPos, int curentPos, int size)
            : _begin(begin), _position(firstPos), _currentPos(curentPos), _size(size) {}

    T& operator*() const {
        return _begin[(_position + _currentPos) % _size];
    }
    T* operator->() {
        return _begin + (_position + _currentPos) % _size;
    }
    T& operator[](difference_type n) {
        return _begin[(_position + n) % _size];
    }

    Iterator<T>& operator=(const Iterator<T>& other) {
        if (&other != this) {
            _currentPos = other._currentPos;
        }
        return *this;
    }

    Iterator<T>& operator++() {
        ++_currentPos;
        return *this;
    }
    Iterator<T>& operator--() {
        --_currentPos;
        return *this;
    }

    Iterator<T>& operator+=(difference_type n) {
        _currentPos += n;
        return *this;
    }

    Iterator<T>& operator-=(difference_type n) {
        _currentPos -= n;
        return *this;
    }

    Iterator<T> operator+(difference_type n) {
        return Iterator<T>(_begin, _position, _currentPos + n, _size);
    }

    Iterator<T> operator-(difference_type n) {
        return Iterator<T>(_begin, _position, _currentPos - n, _size);
    }

    friend Iterator<T> operator+(difference_type n, const Iterator<T>& otherIt) {
        Iterator<T> outIt = otherIt;
        outIt._currentPos += n;
        return outIt;
    }
    friend Iterator<T> operator-(difference_type n, const Iterator<T>& otherIt) {
        Iterator<T> outIt = otherIt;
        outIt._currentPos -= n;
        return outIt;
    }

    difference_type operator- (const Iterator<T>& otherIt) const {
        return *(_begin + (_position + _currentPos - otherIt._currentPos) % _size);
    }

    bool operator==(const Iterator<T>& otherIt) const {
        return _currentPos == otherIt._currentPos;
    }
    bool operator!=(const Iterator<T>& otherIt) const {
        return _currentPos != otherIt._currentPos;
    }
    bool operator>(const Iterator<T>& otherIt) const {
        return _currentPos > otherIt._currentPos;
    }
    bool operator<(const Iterator<T>& otherIt) const {
        return _currentPos < otherIt._currentPos;
    }
    bool operator>=(const Iterator<T>& otherIt) const {
        return _currentPos >= otherIt._currentPos;
    }
    bool operator<=(const Iterator<T>& otherIt) const {
        return _currentPos <= otherIt._currentPos;
    }

    friend ostream& operator<< (ostream& fout, const Iterator<T>& it) {
        fout << it._begin + (it._position + it._currentPos) % it._size;
        return fout;
    }

    difference_type operator-(const Iterator<T>& other){
        return _currentPos - other._currentPos;
    }
};

#endif