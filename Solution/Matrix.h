#pragma once
#include <iostream>
//kurna nie wiem
template<typename  T>class Matrix
{
public:
	Matrix();
	Matrix(int _sizeX, int _sizeY);
	~Matrix();

	void changeSizeX(int _newSizeX, int& errCode);
	void changeSizeY(int _newSizeY, int& errCode);

	void setAt(int _xOffset, int _yOffset, T value, int& errCode);
	int getSizeX() { return sizeX; }
	int getSizeY() { return sizeY; }

	T get(int _xOffset, int _yOffset, int& errCode);

private:
	int sizeX;
	int sizeY;
	T **table;

};

template<typename T>
inline Matrix<T>::Matrix()
{
}

template<typename T>
inline Matrix<T>::Matrix(int _sizeX, int _sizeY)
{
	if (_sizeX > 0 && _sizeY > 0) {
		table = new T*[_sizeX];
		for (int i = 0; i < _sizeX; i++) {
			table[i] = new T[_sizeY];
		}
		sizeX = _sizeX;
		sizeY = _sizeY;
	}
}

template<typename T>
inline Matrix<T>::~Matrix()
{
	for (int i = 0; i < sizeX; i++) {
		delete[] table[i];
	}
	delete[] table;
}

template<typename T>
inline void Matrix<T>::changeSizeX(int _newSizeX, int& errCode)
{
	if (_newSizeX <= 0) {
		errCode = INCORRECT_VALUE;
	}
	else if (_newSizeX == sizeX) {
		errCode = NOTHING_CHANGED;
	}
	else {
		int stopper = sizeX < _newSizeX ? sizeX : _newSizeX;

		T **tempTab = new T*[_newSizeX];

		//fill tempTab
		for (int i = 0; i < stopper; i++) {
			tempTab[i] = table[i];
		}
		for (int i = stopper; i < _newSizeX; i++) {
			tempTab[i] = new T[sizeY];
		}
		//delete elements of original tab
		for (int i = 0; i < sizeX; i++) {
			delete table[i];
		}

		table = tempTab;

		sizeX = _newSizeX;
	}


}

template<typename T>
inline void Matrix<T>::changeSizeY(int _newSizeY, int& errCode)
{
	if (_newSizeY <= 0) {
		errCode = INCORRECT_VALUE;
	}
	else if (_newSizeY == sizeY) {
		errCode = NOTHING_CHANGED;
	}
	else {
		int stopper = sizeY < _newSizeY ? sizeY : _newSizeY;

		//creating empty temporary table
		T **tempTab = new T*[sizeX];
		for (int i = 0; i < sizeX; i++) {
			tempTab[i] = new T[_newSizeY];
		}

		//fill tempTab
		for (int i = 0; i < sizeX; i++) {
			for (int j = 0; j < stopper; j++) {
				tempTab[i][j] = table[i][j];
			}
		}
		
		//delete elements of original tab
		
		for (int i = 0; i < sizeX; i++) {
			
			delete[] table[i];
		}

		table = tempTab;

		sizeY = _newSizeY;
	}


}

template<typename T>
inline void Matrix<T>::setAt(int _xOffset, int _yOffset, T value, int & errCode)
{
	if (_xOffset < 0 || _xOffset >= sizeX) {
		errCode = INVALID_OFFSET;
	}
	else {
		table[_xOffset][_yOffset] = value;
	}
}

template<typename T>
inline T Matrix<T>::get(int _xOffset, int _yOffset, int& errCode)
{
	if (_xOffset < 0 || _xOffset >= sizeX || _yOffset < 0 || _yOffset >= sizeY) {
		errCode = INVALID_OFFSET;
		return NULL;
	}
	return table[_xOffset][_yOffset];
}
