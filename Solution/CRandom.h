#pragma once
#include <random>
#include <stdlib.h>
#include <time.h>
#include <iostream>
#include <random>


class CRandom
{
public:
	CRandom();
	CRandom(int seed);
	~CRandom();
	int getRandomInt(int lower_bound, int upper_bound);
	double getRandomDouble(double lower_bound, double upper_bound);

private:

	std::mt19937 *gen;
};

inline CRandom::CRandom()
{
	std::random_device rd;
	gen = new std::mt19937(rd());
}

inline CRandom::CRandom(int seed)
{
	if (seed == 0) 
	{ 
		std::random_device rd;
		gen = new std::mt19937(rd());
	}
	else 
	{
		gen = new std::mt19937(seed);
	}
}

inline CRandom::~CRandom()
{
	delete gen;
}

inline int CRandom::getRandomInt(int lower_bound = 0, int upper_bound = 100)
{
	std::uniform_int_distribution<> dis(lower_bound, upper_bound);

	return dis(*gen);
}

inline double CRandom::getRandomDouble(double lower_bound = 0, double upper_bound = 100)
{
	std::uniform_real_distribution<> dis(lower_bound, upper_bound);
	return dis(*gen);
}
