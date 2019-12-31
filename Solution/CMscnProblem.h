#pragma once
#include "ErrorCodes.h"
#include "Matrix.h"
#include "Array.h"
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
	//double *pdSolution, int& errorCode
	bool bConstraintsSatisfied(double *pdSolution, int& errCode);
	bool isInRange(double *tab);

	//double getMin(double *pdSolution, int index);

	void getInfoFromFile(int &errCode);
	void putInfoToFile(int &errCode);

	double* makeSolution();

	//void printAll();
	//void printTab(double *tab);
	//void printDoubletab(double **tab);


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

