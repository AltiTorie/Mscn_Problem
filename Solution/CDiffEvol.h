#pragma once
#include "pch.h"
#include "CMscnProblem.h"
#include "Timer.h"
#include "CRandom.h"
#include "COptimizer.h"
#include <stdlib.h> 
#include <string>
#include <fstream>
#include <iostream>
class CDiffEvol:public COptimizer
{
public:
	CDiffEvol() { }
	CDiffEvol(CProblem * _problem, int populationSize, double crossProb, double diffWeight, int time);
	~CDiffEvol();

	double* simulate();

private:
	double repairGene(double gene, int offset);
	bool checkStopCondition();
	void initPopulation();
	double* getRandomInd();
	bool individualsAreDifferent(double* individual, double* baseIndividual, double* additionalIndividual_1, double* additionalIndividual_2);
	double getRandomReal(double lowerBound = 0.0, double upperBound = 1.0);
	double fitness(double* individual);
	void makeCsv();

	CProblem *problem;
	Timer timer;
	double** population;
	int populationSize;
	double crossProb;
	double diffWeight;
	double time;
};

