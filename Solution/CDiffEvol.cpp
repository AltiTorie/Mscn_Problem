#include "pch.h"
#include "CDiffEvol.h"


CDiffEvol::CDiffEvol(CProblem * _problem, int _populationSize, double _crossProb, double _diffWeight, int _time)
{
	problem = _problem;
	populationSize = _populationSize;
	crossProb = _crossProb;
	diffWeight = _diffWeight;
	time = _time;
}

CDiffEvol::~CDiffEvol()
{
}

double * CDiffEvol::simulate()
{
	initPopulation();
	double * ind = new double[problem->getSolutionRange()];
	double * baseInd = getRandomInd();
	double * addInd0 = getRandomInd();
	double * addInd1 = getRandomInd();
	double * indNew = new double[problem->getSolutionRange()];
	timer.StartCounter();
	while (checkStopCondition() == false)
	{

		for (int i = 0; i < populationSize; i++)
		{
			ind = population[i];
			baseInd = getRandomInd();
			addInd0 = getRandomInd();
			addInd1 = getRandomInd();
			if (individualsAreDifferent(ind, baseInd, addInd0, addInd1))
			{
				indNew = new double[problem->getSolutionRange()];
				for (int geneOffset = 0; geneOffset < problem->getSolutionRange(); geneOffset++)
				{
					if (getRandomReal(0, 1) < crossProb)
					{
						indNew[geneOffset] = abs(baseInd[geneOffset] + diffWeight * (addInd0[geneOffset] - addInd1[geneOffset]));
						indNew[geneOffset] = repairGene(indNew[geneOffset], geneOffset);
					}
					else
					{
						indNew[geneOffset] = ind[geneOffset];
					}
				}
				if (fitness(indNew) >= fitness(ind))
				{
					population[i] = indNew;
				}
			}
		}
		/*checker++;
		if (checker == 10) {
			checker = 0;
			if (switched)
			{
				populationSize = populationSize / 2;

			}
			else
			{
				populationSize = populationSize * 2;
			}
			switched = !switched;
		}
		std::cout << populationSize<<"\n";*/
		//makeCsv();
	}
	int errCode = 0;

	for (int i = 0; i < problem->getSolutionRange(); i++)
	{
		ind[i] = 0;
	}

	for (int i = 0; i < populationSize; i++)
	{
		if (problem->getQuality(ind, errCode) < problem->getQuality(population[i], errCode)
			&& problem->ConstraintsSatisfied(population[i], errCode))
		{
			ind = population[i];
		}
	}
	return ind;
}

double CDiffEvol::repairGene(double gene, int offset)
{
	if (gene < problem->getMin(offset))
	{
		return problem->getMin(offset);
	}
	else if (gene > problem->getMax(offset))
	{
		return problem->getMax(offset);
	}
	return gene;
}

bool CDiffEvol::checkStopCondition()
{
	return timer.GetCounter() >= time;
}

void CDiffEvol::initPopulation()
{
	int errCode = 0;
	population = new double*[populationSize];

	for (int i = 0; i < populationSize; i++)
	{
		population[i] = problem->generateSolutionArray(0);

	}
}

double * CDiffEvol::getRandomInd()
{
	CRandom random;
	return population[random.getRandomInt(0, populationSize - 1)];
}

bool CDiffEvol::individualsAreDifferent(double * ind, double * baseInd, double * addInd_1, double * addInd_2)
{
	if (ind != baseInd && ind != addInd_1 && ind != addInd_2) {
		if (baseInd != addInd_1 && baseInd != addInd_2)
		{
			return addInd_1 != addInd_2;
		}
	}
	return false;
}

double CDiffEvol::getRandomReal(double lowerBound, double upperBound)
{
	CRandom rand;
	return rand.getRandomDouble(0, 1);
}

double CDiffEvol::fitness(double * individual)
{
	int errCode = 0;
	return (problem->getQuality(individual, errCode)) * problem->ConstraintsSatisfied(individual, errCode);
}

void CDiffEvol::makeCsv()
{

	int errCode = 0;
	std::ofstream plik;
	plik.open("quality.txt", std::ios::app);
	if (plik.is_open())
	{
		for (int i = 0; i < populationSize - 1; i++)
		{
			plik << problem->getQuality(population[i], errCode);
			plik << ',';
		}
		plik << problem->getQuality(population[populationSize - 1], errCode);
		plik << "\n";
	}
}
