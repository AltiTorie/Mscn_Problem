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
	//losuje tylko pojedyncze wartosci z solution
	void searchByTimeOneValue(int timeInSeconds);
	void searchByTriesOneValue(int tries);
	void searchByImprovedTriesOneValue(int tries);


	//Losuje losowa ilosc liczb ktore maja zostac wygenerowane losowo
	void searchByTimeRandomCount(int timeInSeconds);
	void searchByTriesRandomCount(int tries);
	void searchByImprovedTriesRandomCount(int tries);
	
	//losuje cale solution losowe
	void searchByTimeSolution(int timeInSeconds);
	void searchByTriesSolution(int tries);
	void searchByImprovedTriesSolution(int tries);

private:

	bool oneValueSearch();
	bool findRandomCountValues();
	bool findNewSolution();

	void getNewValue(int &newIndex, double& newValue);

	CMscnProblem *problem;
	CRandom random;

	int errorFlag = 0;
};


