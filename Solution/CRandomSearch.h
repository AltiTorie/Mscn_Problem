#pragma once
#include "CMscnProblem.h"
#include "Timer.h"
#include "Array.h"
#include "CRandom.h"
class CRandomSearch
{
public:
	CRandomSearch();
	CRandomSearch(CMscnProblem* problem);
	~CRandomSearch() { delete problem; }
	void getValuesFromFile(int &errCode);
	void generateRandomValues(int seed);
	void searchByTime(int timeInSeconds);
	void searchByTries(int tries);
	void searchByImprovementTries(int tries);

private:
	double searchNewValue(int &newIndex, double& newValue);
	CMscnProblem *problem;
};


