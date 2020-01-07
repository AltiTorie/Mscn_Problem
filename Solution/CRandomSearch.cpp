#include "pch.h"
#include "CRandomSearch.h"


CRandomSearch::CRandomSearch()
{
	problem = new CMscnProblem(1, 1, 1, 1);
}

CRandomSearch::CRandomSearch(CMscnProblem * _problem)
{
	problem = _problem;
}

void CRandomSearch::getValuesFromFile(int& errCode)
{
	problem->getInfoFromFile(errCode);
}

void CRandomSearch::generateRandomValues(int seed=0)
{
	problem->generateInstance(seed);
}

//void CRandomSearch::searchByTime(int timeInSeconds)
//{
//	int errorCode = 0;
//	int index = 0;
//	double value = 0.0;
//	double *temp;
//	Timer timer;
//	timer.StartCounter();
//	double lastQuality = problem->getQuality();
//	while (timer.GetCounter() < timeInSeconds) {
//		temp = problem->getSolution();
//		searchNewValue(index, value);
//		temp[index] = value;
//		if (lastQuality < problem->getQuality(temp, errorCode) 
//			&& problem->ConstraintsSatisfied(temp, errorCode)) {
//			problem->setSolutionValueAt(index, value);
//			lastQuality = problem->getQuality(temp, errorCode);
//		}
//		else {
//			temp = problem->getSolution();
//		}
//	}
//
//}

//void CRandomSearch::searchByTries(int tries)
//{
//	int currentTries = 0;
//	int errorCode = 0;
//	int index = 0;
//	double value = 0.0;
//	double *temp;
//	double lastQuality = problem->getQuality();
//	while (currentTries <= tries) {
//		temp = problem->getSolution();
//		searchNewValue(index, value);
//		temp[index] = value;
//		if (lastQuality < problem->getQuality(temp, errorCode)
//			&& problem->ConstraintsSatisfied(temp, errorCode)) {
//			problem->setSolutionValueAt(index, value);
//			lastQuality = problem->getQuality(temp, errorCode);
//		}
//		else {
//			temp = problem->getSolution();
//		}
//		currentTries++;
//	}
//}
//
void CRandomSearch::searchByImprovementTries(int tries)
{
	int allTries = 0;
	int improvedTries = 0;

	int currentTries = 0;
	int errorCode = 0;
	int index = 0;
	double value = 0.0;
	double lastQuality = problem->getQuality();
	while (currentTries <= tries) {
		searchNewValue(index, value);
		double oldVal = problem->getSolution()->get(index, errorCode);
		problem->setSolutionValueAt(index, value);
		//jesli nowa wartosc jest lepsza i spelnia warunki
		if (lastQuality < problem->getQuality() 
			&& problem->ConstraintsSatisfied(problem->getSol(), errorCode)) {
			problem->setSolutionValueAt(index, value);
			lastQuality = problem->getQuality();
			currentTries = 0;
			//std::cout << "index: " << index << " value: " << value <<"\n";
			improvedTries++;
		}else {
			problem->setSolutionValueAt(index, oldVal);
		}
		currentTries++;
		allTries++;
	}
	/*std::cout << "Imp tries = " << improvedTries << "\n";
	std::cout << "All tries = " << allTries <<"\n";*/
}

double CRandomSearch::searchNewValue(int &newIndex, double& newValue)
{
	CRandom random;
	int index = random.getRandomInt(0, problem->getSolutionRange());
	double _newValue = random.getRandomInt(problem->getMin(problem->getSol(), index),
												problem->getMax(problem->getSol(), index) / (problem->getAvg(index)));
	newIndex = index;
	newValue = _newValue;
	return _newValue;
}

