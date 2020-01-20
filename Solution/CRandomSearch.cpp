#include "pch.h"
#include "CRandomSearch.h"



CRandomSearch::CRandomSearch()
{
	problem = NULL;
}

CRandomSearch::CRandomSearch(CProblem * _problem, int time)
{
	problem = _problem;
	this->time = time;
}

double* CRandomSearch::simulate()
{
	Timer timer;
	double* solution = new double[problem->getSolutionRange()];
	timer.StartCounter();


	int size = problem->getSolutionRange();
	double *oldSolution = new double[size];

	for (int i = 0; i < size; i++) {
		oldSolution[i] = problem->getSolution()[i];
	}

	double *pdSolution = new double[size];

	bool oldConstraints = problem->ConstraintsSatisfied(problem->getSolution(), errorFlag);
	double lastQuality = problem->getQuality(problem->getSolution(), errorFlag);
	while (timer.GetCounter() <= time) {
		//generowanie nowych wartosci
		pdSolution = generateSolution(size);

		//jesli nowa wartosc jest lepsza i spelnia warunki to zamien
		double quality = problem->getQuality(pdSolution, errorFlag);
		bool constraints = problem->ConstraintsSatisfied(pdSolution, errorFlag);

		if ((lastQuality < quality && constraints) || (constraints && (oldConstraints == false)))
		{
			for (int i = 0; i < size; i++) {
				oldSolution[i] = pdSolution[i];
			}
			lastQuality = quality;
			oldConstraints = true;
		}
	}
	//

	delete oldSolution;
	return std::move(pdSolution);
}

double* CRandomSearch::generateSolution(int range)
{
	double* solution = new double[range];
	for (int i = 0; i < range; i++)
	{
		solution[i] = random.getRandomInt(problem->getMin(i), problem->getMax(i));
	}
	return std::move(solution);
}


void CRandomSearch::getNewValue(int &newIndex, double& newValue)
{
	int index = random.getRandomInt(0, problem->getSolutionRange());
	double _newValue = random.getRandomInt(problem->getMin(index), problem->getMax(index));
	_newValue = ((random.getRandomInt(0, problem->getMax(index))) == 0) ? 0 : _newValue;
	newIndex = index;
	newValue = _newValue;
}


