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

void CRandomSearch::generateRandomValues(int seed = 0)
{
	problem->generateInstance(seed);
}

void CRandomSearch::searchByTime(int timeInSeconds)
{
	Timer timer;
	timer.StartCounter();
	int errorCode = 0;
	int index = 0;
	double value = 0.0;
	double lastQuality = problem->getQuality();
	while (timer.GetCounter() <= timeInSeconds) {
		searchNewValue(index, value);
		double oldVal = problem->getSolution()[index];
		problem->setSolutionValueAt(index, value);

		//jesli nowa wartosc jest lepsza i spelnia warunki
		if (lastQuality < problem->getQuality() && problem->ConstraintsSatisfied(problem->getSolution(), errorCode)) {
			problem->setSolutionValueAt(index, value);
			lastQuality = problem->getQuality();
		}
		else {
			problem->setSolutionValueAt(index, oldVal);
		}
	}
	/*std::cout << "Imp tries = " << improvedTries << "\n";
	std::cout << "All tries = " << allTries <<"\n";*/

}

void CRandomSearch::searchByTriesSolution(int tries)
{
	int allTries = 0;
	int improvedTries = 0;
	int errorCode = 0;
	int size = problem->getSolutionRange();

	double *old = new double[size];
	for (int i = 0; i < size; i++) {
		old[i] = problem->getSolution()[i];
	}
	CRandom radom;
	int currentTries = 0;
	double lastQuality = problem->getQuality();
	while (currentTries <= tries) {
		for (int i = 0; i < size; i++) {
				int low = problem->getMin(i);
				int big = problem->getMax(i);
				problem->setSolutionValueAt(i, radom.getRandomInt(low,big));
		}

		//problem->printSolution();
		//jesli nowa wartosc jest lepsza i spelnia warunki
		if (lastQuality < problem->getQuality() && problem->ConstraintsSatisfied()) 
		{
			for (int i = 0; i < size; i++) {
				old[i]=problem->getSolution()[i];
			}
			lastQuality = problem->getQuality();
			improvedTries++;
		}
		currentTries++;
		allTries++;
		for (int i = 0; i < size; i++) {
			problem->setSolutionValueAt(i, old[i]);
		}
	}
	/*std::cout << "Imp tries = " << improvedTries << "\n";
	std::cout << "All tries = " << allTries <<"\n";*/
}

void CRandomSearch::searchByImprovementTriesSolution(int tries)
{
	int allTries = 0;

	int errorCode = 0;
	int size = problem->getSolutionRange();

	double *old = new double[size];
	for (int i = 0; i < size; i++) {
		old[i] = problem->getSolution()[i];
	}
	CRandom radom;
	int currentTries = 0;
	double lastQuality = problem->getQuality();
	while (currentTries <= tries) {

		problem->generateSolution(errorCode);
		//problem->printSolution();
		//jesli nowa wartosc jest lepsza i spelnia warunki
		if (lastQuality < problem->getQuality())
		{
			for (int i = 0; i < size; i++) {
				old[i] = problem->getSolution()[i];
			}
			lastQuality = problem->getQuality();
			currentTries = 0;
		}
		currentTries++;
		allTries++;
		for (int i = 0; i < size; i++) {
			problem->setSolutionValueAt(i, old[i]);
		}
	}
	/*std::cout << "Imp tries = " << improvedTries << "\n";
	std::cout << "All tries = " << allTries <<"\n";*/
}



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
		double oldVal = problem->getSolution()[index];
		problem->setSolutionValueAt(index, value);
		//std::cout << "IN: " << (problem->getQuality() > 0 ? problem->getQuality() : 0)<<"\n";
		//jesli nowa wartosc jest lepsza i spelnia warunki
		if (lastQuality < problem->getQuality()
			&& problem->ConstraintsSatisfied()) {
			problem->setSolutionValueAt(index, value);
			lastQuality = problem->getQuality();
			currentTries = 0;
			//std::cout << "index: " << index << " value: " << value <<"\n";
			improvedTries++;
		}
		else {
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
	double _newValue = random.getRandomInt(problem->getMin(index),
		problem->getMax(index) / (problem->getAvg(index)));
	_newValue = ((random.getRandomInt(0, problem->getMax(index))) == 0) ? 0 : _newValue;
	newIndex = index;
	newValue = _newValue;
	return _newValue;
}

