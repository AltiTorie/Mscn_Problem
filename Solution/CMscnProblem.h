#pragma once
#include "ErrorCodes.h"
#include "CMscnValuesBondariesh.h"
#include "Matrix.h"
#include "Array.h"
#include "CRandom.h"
#include <string>
#include <fstream>
#include <iostream>

class CMscnProblem
{
public:
	CMscnProblem() {};
	CMscnProblem(int D, int F, int M, int S);
	~CMscnProblem();

	void setDelivererCount(int count, int& errCode);
	void setFactoryCount(int count, int& errCode);
	void setMagazineCount(int count, int& errCode);
	void setShopCount(int count, int& errCode);

	double dGetQuality(double *pdSolution, int&errCode);
	bool bConstraintsSatisfied(double *pdSolution, int& errCode);
	bool isInRange(double *tab);

	void generateInstance(int instanceSeed);
	void generateSolution(int instanceSeed);

	double getMin(double *pdSolution, int index);
	double getMax(double *pdSolution, int index);

	
	double* makeSolution();

	int ddd() { return deliverer; }
	int dd() { return D; }

	void getInfoFromFile(int &errCode);
	void putInfoToFile(int &errCode);

	void printValues();
	void printSolution();

private:
	int deliverer;
	int factory;
	int magazine;
	int shop;

	//better version
	Array<double> *sd;
	Array<double> *sf;
	Array<double> *sm;
	Array<double> *ss;

	Matrix<double> *cdf;
	Matrix<double> *cfm;
	Matrix<double> *cms;

	Array<double> *ud;
	Array<double> *uf;
	Array<double>* um;

	Array<double> *p;

	Matrix<double> *xdminmax;
	Matrix<double> *xfminmax;
	Matrix<double> *xmminmax;

	//solution values
	int D;
	int F;
	int M;
	int S;

	Matrix<double>*xdf;
	Matrix<double>*xfm;
	Matrix<double>*xms;
};

