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

    //
//    CircularBuffer(const CircularBuffer& otherObj)
//            : _size(otherObj._size),
//              writeCount(otherObj.writeCount),
//              firstPosition(otherObj.firstPosition)
//    {
//        _data = new T[_size];
//        for (int i = 0; i < _size; ++i) {
//            _data[i] = otherObj._data[i];
//            if (otherObj._first == &otherObj._data[i])
//                _first = &_data[i];
//            if (otherObj._last == &otherObj._data[i])
//                _last = &_data[i];
//        }
//    }

    ~CircularBuffer() {
        _first = nullptr;
        _last = nullptr;
        delete _first;
        delete _last;
        delete[] _data;
    }

    //
//    CircularBuffer& operator=(const CircularBuffer& otherObj) {
//        if (&otherObj != this) {
//            _size = otherObj._size;
//            writeCount = otherObj.writeCount;
//            firstPosition = otherObj.firstPosition;
//
//            _first = nullptr;
//            _last = nullptr;
//            delete[] _data;
//            _data = new T[_size];
//
//            for (int i = 0; i < _size; ++i) {
//                _data[i] = otherObj._data[i];
//                if (otherObj._first == &otherObj._data[i])
//                    _first = &_data[i];
//                if (otherObj._last == &otherObj._data[i])
//                    _last = &_data[i];
//            }
//        }
//        return *this;
//    }

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
        return *PointerDecrease(_last);                                 //
    }

    void delFirst() {
        if (_first != PointerDecrease(_last))                            //
            _first = PointerIncrease(_first, true);
        writeCount--;
    }

    void delLast() {
        if (_first != PointerDecrease(_last))                                       //
            _last = PointerDecrease(_last);
        writeCount--;
    }

    T& operator[](const int position) {
        if (position < 0 || position > _size)
            throw std::runtime_error("\nOut of range\n");
        else if (position > writeCount - 1)
            throw std::runtime_error("\nThe buffer has not yet been filled up to this element\n");
        return _data[(firstPosition + position) % (_size + 1)];
    }

    T operator[](const int position) const {
        if (position < 0 || position > _size)
            throw std::runtime_error("\nOut of range\n");
        else if (position > writeCount - 1)
            throw std::runtime_error("\nThe buffer has not yet been filled up to this element\n");
        return _data[(firstPosition + position) % (_size + 1)];
    }

    //
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
        std::cout << "\n---------\n";
    }

};

#endif