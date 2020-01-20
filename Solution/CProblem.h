#pragma once

class CProblem
{
public:
	virtual double getQuality(double *pdSolution, int&errCode) = 0;
	virtual	bool ConstraintsSatisfied(double *pdSolution, int& errCode) = 0;
	virtual double getMin(int index) = 0;
	virtual double getMax(int index) = 0;

	virtual int getSolutionRange() = 0;
	virtual double* getSolution() = 0;
	virtual double* generateSolutionArray(int instanceSeed) = 0;
	virtual void setSolutionValueAt(int index, double _newValue) = 0;

private:
	double *solutionTable;
};
