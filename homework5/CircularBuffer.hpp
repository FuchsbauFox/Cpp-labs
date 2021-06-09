#ifndef CIRCULARBUFFER_H
#define CIRCULARBUFFER_H

#include "Iterator.hpp"

template<class T>
class CircularBuffer {
private:
    int _size;
    int writeCount;
    int firstPosition;
    T* _data;
    T* _first;
    T* _last;

    T* PointerIncrease(T* pointer, bool isFirst = false) {
        if (isFirst)
            pointer == &_data[_size] ? firstPosition = 0 : firstPosition++;
        return (pointer == &_data[_size] ? &_data[0] : pointer + 1);
    }

    T* PointerDecrease(T* pointer, bool isFirst = false) {
        if (isFirst)
            pointer == &_data[0] ? firstPosition = _size : firstPosition--;
        return (pointer == &_data[0] ? &_data[_size] : pointer - 1);
    }

public:
    CircularBuffer(int size)
            :_size(size),
             writeCount(0),
             firstPosition(0)
    {
        _data = new T[_size + 1];
        _first = &_data[0];
        _last = &_data[1];
    }

    ~CircularBuffer() {
        _first = nullptr;
        _last = nullptr;
        delete[] _data;
    }

    void addFirst(T newElement) {
        if (writeCount != 0)
            _first = PointerDecrease(_first, true);
        *_first = newElement;
        if (_first == _last)
            _last = PointerDecrease(_last);
        else
            writeCount++;
    }

    void addLast(T newElement) {

        if (writeCount == 0)
            _last = PointerDecrease(_last);
        *_last = newElement;
        _last = PointerIncrease(_last);
        if (_first == _last)
            _first = PointerIncrease(_first, true);
        else
            writeCount++;
    }

    T first() {
        if (writeCount == 0)
            throw std::runtime_error("The buffer is empty");
        return *_first;
    }

    T last() {
        if (writeCount == 0)
            throw std::runtime_error("The buffer is empty");
        return *PointerDecrease(_last);
    }

    void delFirst() {
        if (_first != PointerDecrease(_last))
            _first = PointerIncrease(_first, true);
        writeCount--;
    }

    void delLast() {
        if (_first != PointerDecrease(_last))
            _last = PointerDecrease(_last);
        writeCount--;
    }

    //fixed size and index information in exception
    T& operator[](const int pos) {
        if (pos < 0 || pos > _size) {
            std::cout << "Out of range (buffer size: " << _size << " / requested index: " << pos << ")\n";
            throw out_of_range("out of range");
        }
        else if (pos > writeCount - 1) {
            std::cout << "The buffer has not been filled up to this element (items in the buffer: " << writeCount << " / requested index: " << pos << "). ";
            writeCount != 0 ? std::cout  << "Maximum index: " << writeCount - 1 << '\n' : std::cout  << "The buffer is empty\n";
            throw out_of_range("out of range");
        }
        return _data[(firstPosition + pos) % (_size + 1)];
    }

    T operator[](const int pos) const {
        if (pos < 0 || pos > _size) {
            std::cout << "Out of range (buffer size: " << _size << " / requested index: " << pos << ")\n";
            throw out_of_range("out of range");
        }
        else if (pos > writeCount - 1) {
            std::cout << "The buffer has not been filled up to this element (items in the buffer: " << writeCount << " / requested index: " << pos << "). ";
            writeCount != 0 ? std::cout  << "Maximum index: " << writeCount - 1 : std::cout  << "The buffer is empty\n";
            throw out_of_range("out of range");
        }
        return _data[(firstPosition + pos) % (_size + 1)];
    }

    void changeCapacity(int newSize) {

        T* prevData = new T[_size + 1];
        for (int i = 0; _first != _last; _first = PointerIncrease(_first), ++i)
            prevData[i] = *_first;

        _first = nullptr;
        _last = nullptr;
        delete[] _data;
        _data = new T[newSize + 1];
        _first = &_data[0];
        firstPosition = 0;

        if (newSize >= _size) {
            _last = &_data[_size];
        }
        else {
            if (writeCount > newSize)
                writeCount = newSize;
            _last = &_data[newSize];
        }
        for (int i = 0; i < _size + 1 && i < newSize + 1; ++i) {
            _data[i] = prevData[i];
        }

        _size = newSize;
    }

    Iterator<T> begin() const {
        return Iterator<T>(&_data[0], firstPosition, 0, _size + 1);
    }
    Iterator<T> end() const {
        return Iterator<T>(&_data[0], firstPosition, writeCount, _size + 1);
    }

    void print(){
        for (int i = 0; i < _size + 1; i++)
            std::cout << _data[i] << ' ';
        std::cout << "\n" << *_first << *_last << "\n---------\n";

    }

};

#endif