#pragma once
#include "CProblem.h"
class COptimizer
{
public:
	virtual double* simulate() = 0;
private:
	bool checkStopCondition();
};