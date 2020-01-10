#include "pch.h"
#include "CRandomSearch.h"



CRandomSearch::CRandomSearch()
{
	problem = new CMscnProblem(1, 1, 1, 1);
	problem->generateInstance(0);
	problem->generateSolution(0);
}

CRandomSearch::CRandomSearch(CMscnProblem * _problem)
{
	problem = _problem;
}

//Metody do losowania pojedynczych wartosci z solution
void CRandomSearch::searchByTimeOneValue(int timeInSeconds)
{
	Timer timer;
	timer.StartCounter();

	while (timer.GetCounter() <= timeInSeconds) {
		oneValueSearch();
	}
}

void CRandomSearch::searchByTriesOneValue(int tries)
{
	int currentTries = 0;

	while (currentTries <= tries) {
		oneValueSearch();
		currentTries++;
	}
}

void CRandomSearch::searchByImprovedTriesOneValue(int tries)
{
	int currentTries = 0;

	while (currentTries <= tries) {
		currentTries++;
		if (oneValueSearch())
		{
			currentTries = 0;
		}
	}
}

bool CRandomSearch::oneValueSearch() {

	bool found = false;

	int index = 0;
	double value = 0.0;

	double lastQuality = problem->getQuality();
	bool lastConstraints = problem->ConstraintsSatisfied();

	getNewValue(index, value);
	double oldVal = problem->getSolution()[index];
	problem->setSolutionValueAt(index, value);

	double quality = problem->getQuality();
	double constraints = problem->ConstraintsSatisfied();
	//jesli nowa wartosc jest lepsza i spelnia warunki
	if ((lastQuality < quality && constraints) || (constraints && !lastConstraints)) {
		problem->setSolutionValueAt(index, value);
		lastQuality = problem->getQuality();
		found = true;
	}
	else {
		problem->setSolutionValueAt(index, oldVal);
	}
	return found;
}


//Metody do znajdowania losowej liczby nowych wartosci
void CRandomSearch::searchByTimeRandomCount(int timeInSeconds)
{
	Timer timer;
	timer.StartCounter();

	while (timer.GetCounter() <= timeInSeconds) {
		findRandomCountValues();
	}
	//std::cout << "Constraintsy po randomCouncie:" << problem->ConstraintsSatisfied()<<"\n";
}

void CRandomSearch::searchByTriesRandomCount(int tries)
{
	int currentTries = 0;

	while (currentTries <= tries) {
		findRandomCountValues();
		currentTries++;
	}
}

void CRandomSearch::searchByImprovedTriesRandomCount(int tries)
{
	int currentTries = 0;

	while (currentTries <= tries) {
		currentTries++;
		if (findRandomCountValues())
		{
			currentTries = 0;
		}
	}
}

bool CRandomSearch::findRandomCountValues()
{
	bool found = false;
	int randomSize = random.getRandomInt(1, problem->getSolutionRange()/3);
	 int size = problem->getSolutionRange();
	double *oldSolution = new double[size];
	for (int i = 0; i < size; i++) {
		oldSolution[i] = problem->getSolution()[i];
	}

	int index = 0;
	double value = 0;

	bool oldConstraints = problem->ConstraintsSatisfied();
	double lastQuality = problem->getQuality();

	//generowanie nowych wartosci
	for (int i = 0; i < randomSize; i++) {
		getNewValue(index, value);
		problem->setSolutionValueAt(index, value);
	}

	//jesli nowa wartosc jest lepsza i spelnia warunki to zamien
	double quality = problem->getQuality();
	bool constraints = problem->ConstraintsSatisfied();

	if (((lastQuality < quality) && constraints) || (constraints && !oldConstraints))
	{
	
		for (int i = 0; i < size; i++) {
			oldSolution[i] = problem->getSolution()[i];
		}
		lastQuality = quality;
		oldConstraints = true;
		found = true;
		
	}
	//
	for (int i = 0; i < size; i++) {
		problem->setSolutionValueAt(i, oldSolution[i]);
	}
	return found;
}


//Metody do zmieniania calego solution
void CRandomSearch::searchByTimeSolution(int timeInSeconds)
{
	Timer timer;

	int size = problem->getSolutionRange();

	//zapamietaj oryginalna tablice
	double *old = new double[size];
	for (int i = 0; i < size; i++) {
		old[i] = problem->getSolution()[i];
	}

	bool oldConstraints = problem->ConstraintsSatisfied();
	double lastQuality = problem->getQuality();

	timer.StartCounter();
	while (timer.GetCounter() <= timeInSeconds) {
		problem->generateSolution(0);


		//jesli nowa wartosc jest lepsza i spelnia warunki
		double quality = problem->getQuality();
		bool constraints = problem->ConstraintsSatisfied();
		if ((lastQuality < quality && constraints) || (constraints && (oldConstraints == false)))
		{
			//zamien oryginalna tablice na lepsza
			for (int i = 0; i < size; i++) {
				old[i] = problem->getSolution()[i];
			}
			lastQuality = quality;
			oldConstraints = true;
		}
	}
	for (int i = 0; i < size; i++) {
		problem->setSolutionValueAt(i, old[i]);
	}

}

void CRandomSearch::searchByTriesSolution(int tries)
{

	int size = problem->getSolutionRange();

	double *old = new double[size];
	for (int i = 0; i < size; i++) {
		old[i] = problem->getSolution()[i];
	}
	int currentTries = 0;

	bool oldConstraints = problem->ConstraintsSatisfied();
	double lastQuality = problem->getQuality();

	while (currentTries <= tries) {
		problem->generateSolution(0);


		//jesli nowa wartosc jest lepsza i spelnia warunki
		double quality = problem->getQuality();
		bool constraints = problem->ConstraintsSatisfied();
		if ((lastQuality < quality && constraints) || (constraints && (oldConstraints == false)))
		{
			for (int i = 0; i < size; i++) {
				old[i] = problem->getSolution()[i];
			}
			lastQuality = quality;
			oldConstraints = true;
		}
		currentTries++;

	}
	for (int i = 0; i < size; i++) {
		problem->setSolutionValueAt(i, old[i]);
	}
}

void CRandomSearch::searchByImprovedTriesSolution(int tries)
{

	int size = problem->getSolutionRange();

	double *old = new double[size];
	for (int i = 0; i < size; i++) {
		old[i] = problem->getSolution()[i];
	}
	CRandom radom;
	int currentTries = 0;
	double lastQuality = problem->getQuality();
	while (currentTries <= tries) {

		problem->generateSolution(0);


		//jesli nowa wartosc jest lepsza i spelnia warunki
		if (lastQuality < problem->getQuality())
		{
			for (int i = 0; i < size; i++) {
				old[i] = problem->getSolution()[i];
			}
			lastQuality = problem->getQuality();
			currentTries = 0;
			std::cout << "Upgraded" << "\n";
		}
		currentTries++;
		for (int i = 0; i < size; i++) {
			problem->setSolutionValueAt(i, old[i]);
		}
	}
}

bool CRandomSearch::findNewSolution()
{
	bool found = false;
	int size = random.getRandomInt(1, problem->getSolutionRange());

	double *oldSolution = new double[problem->getSolutionRange()];
	for (int i = 0; i < size; i++) {
		oldSolution[i] = problem->getSolution()[i];
	}

	int index = 0;
	double value = 0;

	bool oldConstraints = problem->ConstraintsSatisfied();
	double lastQuality = problem->getQuality();

	//generowanie nowych wartosci
	problem->generateSolution(0);

	//jesli nowa wartosc jest lepsza i spelnia warunki to zamien
	double quality = problem->getQuality();
	bool constraints = problem->ConstraintsSatisfied();

	if ((lastQuality < quality && constraints) || (constraints && (oldConstraints == false)))
	{
		for (int i = 0; i < size; i++) {
			oldSolution[i] = problem->getSolution()[i];
		}
		lastQuality = quality;
		oldConstraints = true;
		found = true;
	}
	//
	for (int i = 0; i < size; i++) {
		problem->setSolutionValueAt(i, oldSolution[i]);
	}
	return true;
}


void CRandomSearch::getNewValue(int &newIndex, double& newValue)
{
	int index = random.getRandomInt(0, problem->getSolutionRange());
	double _newValue = random.getRandomInt(problem->getMin(index), problem->getMax(index));
	_newValue = ((random.getRandomInt(0, problem->getMax(index))) == 0) ? 0 : _newValue;
	newIndex = index;
	newValue = _newValue;
}


