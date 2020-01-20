#pragma once
#include "CMscnProblem.h"
#include "Timer.h"
#include "Array.h"
#include "CRandom.h"
#include "COptimizer.h"
class CRandomSearch:public COptimizer
{
public:
	CRandomSearch();
	CRandomSearch(CProblem* problem, int timeInSeconds);
	~CRandomSearch() { delete problem; }
	
	//losuje cale solution losowe
	double* simulate();

private:

	bool findNewSolution();

	double* generateSolution(int range);

	void getNewValue(int &newIndex, double& newValue);

	CProblem *problem;
	CRandom random;
	double time;
	int errorFlag = 0;
};


