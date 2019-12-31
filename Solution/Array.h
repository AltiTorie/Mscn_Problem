#pragma once
#include "ErrorCodes.h"

template <typename T>class Array {

public:
	Array();
	Array(int _size) { table = new T[_size]; size = _size;}
	~Array() { delete table; }

	void changeSize(int _newSize, int& errCode);
	void setAt(int _offset, T value, int& errCode);
	int getSize() { return size; }
	T get(int _offset, int& errCode);

private:
	int size;
	T *table;
};

template<typename T>
inline void Array<T>::changeSize(int _newSize, int & errCode)
{
	if (_newSize < 0) {
		errCode = INCORRECT_VALUE;
	}
	else if (_newSize == size) {
		errCode = NOTHING_CHANGED;
	}
	else {
		T *tempTab = new T[_newSize];

		int stopper = size < _newSize ? size : _newSize;

		for (int i = 0; i < stopper; i++) {
			tempTab[i] = table[i];
		}
		/*for (int i = stopper; i < _newSize; i++) {
			tempTab[i] = new T;
		}*/
		table = tempTab;

		size = _newSize;
	}
}

template<typename T>
inline void Array<T>::setAt(int _offset, T value, int & errCode)
{
	if (_offset < 0 || _offset >= size) {
		errCode = INVALID_OFFSET;
	}
	else {
		table[_offset] = value;
	}
	
}

template<typename T>
inline T Array<T>::get(int _offset, int & errCode)
{
	if (_offset < 0 || _offset >= size) {
		errCode = INVALID_OFFSET;
		return NULL;
	}
	return table[_offset];
}
