#include "pch.h"
#include "CMscnProblem.h"


CMscnProblem::CMscnProblem(int _D, int _F, int _M, int _S)
{

	deliverer = _D;
	factory = _F;
	magazine = _M;
	shop = _S;

	//BETTER VERSION
	//how much stuff these can send
	sd = new Array<double>(deliverer);
	sf = new Array<double>(factory);
	sm = new Array<double>(magazine);
	ss = new Array<double>(shop);

	//how much it costs to deliver form d->f / f-m / m->s
	cdf = new Matrix<double>(deliverer, factory);
	cfm = new Matrix<double>(factory, magazine);
	cms = new Matrix<double>(magazine, shop);

	//one time cost of signing the deal
	ud = new Array<double>(deliverer);
	uf = new Array<double>(factory);
	um = new Array<double>(magazine);
	
	//price of product in each shop
	p = new Array<double>(shop);

	//minimum and maximum count of items that can be sent out
	xdminmax = new Matrix<double>(2*deliverer, factory);
	xfminmax = new Matrix<double>(2*factory, magazine);
	xmminmax = new Matrix<double>(2*magazine, shop);

	//solution values
	xdf = new Matrix<double>(deliverer, factory);
	xfm = new Matrix<double>(factory, magazine);
	xms = new Matrix<double>(magazine, shop);


	//getInfoFromFile(a);
}//CMscnProblem::CMscnProblem(int _D, int _F, int _M, int _S)


CMscnProblem::~CMscnProblem()
{
	delete sd;
	delete sf;
	delete sm;
	delete ss;

	delete cdf;
	delete cfm;
	delete cms;

	delete ud;
	delete uf;
	delete um;

	delete p;

	delete xdminmax;
	delete xfminmax;
	delete xmminmax;

	delete xdf;
	delete xfm;
	delete xms;


}//CMscnProblem::~CMscnProblem()


void CMscnProblem::setDelivererCount(int count, int & errCode)
{
	
	if (count <= 0) {
		errCode = INCORRECT_VALUE;
	}
	else if (count == deliverer) {
		errCode = NOTHING_CHANGED;
	}
	else {

		//better version
		sd->changeSize(count, errCode);
		cdf->changeSizeX(count, errCode);
		ud->changeSize(count, errCode);
		xdminmax->changeSizeX(2*count, errCode);
		xdf->changeSizeX(count, errCode);
		//
		deliverer = count;


	}
}//void CMscnProblem::setDelivererCount(int count, int & errCode)

void CMscnProblem::setFactoryCount(int count, int & errCode)
{
	if (count <= 0) {
		errCode = INCORRECT_VALUE;
	}
	else if (count == factory) {
		errCode = NOTHING_CHANGED;
	}
	else {
		//better version

		sf->changeSize(count, errCode);
		cdf->changeSizeY(count, errCode);
		cfm->changeSizeX(count, errCode);
		uf->changeSize(count, errCode);
		xdminmax->changeSizeY(count, errCode);
		xfminmax->changeSizeX(2*count, errCode);
		xdf->changeSizeY(count, errCode);
		xfm->changeSizeX(count, errCode);
		//
		factory = count;
	}
}//void CMscnProblem::setFactoryCount(int count, int & errCode)

void CMscnProblem::setMagazineCount(int count, int & errCode)
{
	
	if (count <= 0) {
		errCode = INCORRECT_VALUE;
	}
	else if (count == magazine) {
		errCode = NOTHING_CHANGED;
	}
	else {
		//better version

		sm->changeSize(count,errCode);
		cms->changeSizeX(count, errCode);
		cfm->changeSizeY(count, errCode);
		um->changeSize(count, errCode);
		xmminmax->changeSizeX(2*count, errCode);
		xfminmax->changeSizeY(count, errCode);
		xms->changeSizeX(count, errCode);
		xfm->changeSizeY(count, errCode);

		//

		magazine = count;
	}
}//void CMscnProblem::setMagazineCount(int count, int & errCode)

void CMscnProblem::setShopCount(int count, int & errCode)
{
	if (count <= 0) {
		errCode = INCORRECT_VALUE;
	}
	else if (count == shop) {
		errCode = NOTHING_CHANGED;
	}
	else {
		

		//better version

		ss->changeSize(count, errCode);
		cms->changeSizeY(count, errCode);
		p->changeSize(count, errCode);
		xmminmax->changeSizeY(count, errCode);
		xms->changeSizeY(count, errCode);

		shop = count;
	}
}//void CMscnProblem::setShopCount(int count, int & errCode)

int epsilon(double a) {
	return a > 0 ? 1 : 0;
}



double CMscnProblem::dGetQuality(double *pdSolution, int &errCode)
{
	errCode = 1;
	if (pdSolution == nullptr) {
		errCode = NULL_ARRAY;
	}
	try {
		for (int i = 0; i < D*F + F * M + M * S; i++) {
			if (pdSolution[i] < 0) {
				errCode = INCORRECT_VALUE;
			}
		}
	}
	catch (...) {
		errCode = INVALID_ARRAY_BOUNDARIES;
		return 0;
	}

	//double * pdSolution, int & errorCode
	int index = 0;
	double KT = 0;
	for (int i = 0; i < D; i++) {
		for (int j = 0; j < F; j++) {
			//KT += cost_df[i][j] * pdSolution[index];
			KT += (cdf->get(i, j, errCode)) * pdSolution[index];
			index++;
			//KT += cost_df[i][j] * x_df[i][j];

		}
	}
	std::cout << "KT1: " << KT << "\n";
	for (int i = 0; i < F; i++) {
		for (int j = 0; j < M; j++) {
			//KT += cost_fm[i][j] * pdSolution[index];
			KT += (cfm->get(i, j, errCode)) * pdSolution[index];
			index++;
			//KT += cost_fm[i][j] * x_fm[i][j];
		}
	}
	std::cout << "KT2: " << KT << "\n";
	for (int i = 0; i < M; i++) {
		for (int j = 0; j < S; j++) {
			//KT += cost_ms[i][j] * pdSolution[index];
			KT += (cms->get(i, j, errCode)) * pdSolution[index];
			index++;
			//KT += cost_ms[i][j] * x_ms[i][j];
		}
	}
	std::cout << "KT: " << KT << "\n";
	index = 0;
	double KU = 0;
	double helper = 0;
	for (int i = 0; i < D; i++) {
		helper = 0;
		for (int j = 0; j < F; j++) {
			helper += pdSolution[index];
			index++;
			//			helper += x_df[i][j];

		}
		//KU += (epsilon(helper)*oneTimeCost_d[i]);
		KU += (epsilon(helper) * (ud->get(i,errCode)));
	}
	std::cout << "KU1: " << KU << "\n";
	for (int i = 0; i < F; i++) {
		helper = 0;
		for (int j = 0; j < M; j++) {
			helper += pdSolution[index];
			index++;
			//helper += x_fm[i][j];

		}
		//KU += (epsilon(helper)*oneTimeCost_f[i]);
		KU += (epsilon(helper) * (uf->get(i, errCode)));
	}
	std::cout << "KU2: " << KU << "\n";

	for (int i = 0; i < M; i++) {
		helper = 0;
		for (int j = 0; j < S; j++) {
			helper += pdSolution[index];
			index++;
			//			helper += x_ms[i][j];
		}
		//KU += (epsilon(helper)*oneTimeCost_m[i]);
		KU += (epsilon(helper) * (um->get(i, errCode)));
	}
	std::cout << "KU3: " << KU << "\n";
	double P = 0;
	index = D * F + F * M;
	for (int i = 0; i < M; i++) {
		for (int j = 0; j < S; j++) {
			//P += price_s[j] * pdSolution[index];
			P += p->get(j,errCode) * pdSolution[index];

			index++;
			//P += price_s[j] * x_ms[i][j];
		}
	}
	std::cout << "P: " << P << "\n";
	double z = P - KT - KU;
	return z;
}//double CMscnProblem::dGetQuality2(double *pdSolution, int &errCode)

bool CMscnProblem::bConstraintsSatisfied(double * pdSolution, int & errCode)
{
	double sumHelper = 0;
	int index = 0;
	//Sumaryczna iloœæ surowca zamówiona u danego dostawcy nie mo¿e przekroczyæ
	//	jego mocy produkcyjnych sd
	for (int i = 0; i < D; i++) {
		for (int j = 0; j < F; j++) {
			sumHelper += pdSolution[index];
			index++;
		}
		if (sumHelper > sd->get(i,errCode)) {
			return false;
		}
		sumHelper = 0;
	}

	//Sumaryczna iloœæ produktów zamówiona w danej fabryce nie mo¿e przekroczyæ jej
	//mocy produkcyjnych sf
	sumHelper = 0;
	for (int i = 0; i < F; i++) {
		for (int j = 0; j < M; j++) {
			sumHelper += pdSolution[index];
			index++;
		}
		if (sumHelper > sf->get(i,errCode)) {
			return false;
		}
		sumHelper = 0;
	}
	//Sumaryczna iloœæ produktów zamówiona pobierana z danego magazynu nie mo¿e
	//	przekroczyæ jego pojemnoœci sm
	sumHelper = 0;
	for (int i = 0; i < M; i++) {
		for (int j = 0; j < S; j++) {
			sumHelper += pdSolution[index];
			index++;
		}
		if (sumHelper > sm->get(i,errCode)) {
			return false;
		}
		sumHelper = 0;
	}
	//Sumaryczna iloœæ produktów dostarczana do danego sklepu nie mo¿e przekroczyæ
	//zapotrzebowania rynkowego dla tego sklepu ss
	index = D * F + M * F;
	sumHelper = 0;
	for (int i = 0; i < S; i++) {
		for (int j = 0; j < M; j++) {
			sumHelper += pdSolution[index];
			index++;
		}
		if (sumHelper > ss->get(i,errCode)) {
			return false;
		}
		sumHelper = 0;
	}
	//Suma surowców od dostawców wchodz¹cych do jednej fabryki f, nie
	//mo¿e byæ mniejsza ni¿ suma produktów wychodz¹cych z tej fabryki f
	index = 0;
	sumHelper = 0;
	double secondSum = 0;
	for (int i = 0; i < F; i++)
	{
		for (int j = i; j < D*F; j += F) {
			sumHelper += pdSolution[j];
		}
	}
	index = D * F;
	for (int i = 0; i < F; i++)
	{
		for (int j = 0; j < M; j++)
		{
			secondSum += pdSolution[index];
			index++;
		}
	}
	if (secondSum > sumHelper) {
		return false;
	}
	//Suma wszystkich produktów dostarczanych do jednego centrum
	//dystrybucyjnego m nie mo¿e byæ mniejsza ni¿ suma wszystkich produktów
	//dostarczanych z tego centrum dystrybucyjnego m do sklepów.
	index = D * F;
	sumHelper = 0;
	secondSum = 0;
	for (int i = 0; i < M; i++)
	{
		for (int j = i; j < F*M; j += M) {
			sumHelper += pdSolution[index + j];
		}
	}
	index = D * F + F * M;
	for (int i = 0; i < M; i++)
	{
		for (int j = 0; j < S; j++)
		{
			secondSum += pdSolution[index];
			index++;
		}
	}
	if (secondSum > sumHelper) {
		return false;
	}

	return true;
}//bool CMscnProblem::bConstraintsSatisfied(double * pdSolution, int & errCode)


void checker(char name, int& errCode) {
	if (name != ';') {
		errCode = INVALID_SEPARATOR;
	}
}

void CMscnProblem::getInfoFromFile(int &errCode)
{

	char ProblemFile[] = "plikProblemu2.txt";
	char SolutionFile[] = "plikRozwiazania2.txt";

	char name[256];
	char test[] = ";";

	double number;

	std::ifstream file;
	file.open(ProblemFile);
	if (file.is_open())
	{
		//D
		file >> name;
		file >> number;
		setDelivererCount(number, errCode);
		//F
		file >> name;
		file >> number;
		setFactoryCount(number, errCode);
		//M
		file >> name;
		file >> number;
		setMagazineCount(number, errCode);
		//S
		file >> name;
		file >> number;
		setShopCount(number, errCode);
		//sd
		file >> name;
		for (int i = 0; i < deliverer; i++)
		{
			file >> number;
			sd->setAt(i, number,errCode);
		}
		//sf
		file >> name;
		for (int i = 0; i < factory; i++)
		{
			file >> number;
			sf->setAt(i, number, errCode);
		}
		//sm
		file >> name;
		for (int i = 0; i < magazine; i++)
		{
			file >> number;
			sm->setAt(i, number, errCode);
		}
		//ss
		file >> name;
		for (int i = 0; i < shop; i++)
		{
			file >> number;
			ss->setAt(i, number, errCode);
		}
		//cd
		file >> name;
		for (int i = 0; i < deliverer; i++)
		{
			for (int j = 0; j < factory; j++)
			{
				file >> number;
				cdf->setAt(i,j, number, errCode);
			}
		}
		//cf
		file >> name;
		for (int i = 0; i < factory; i++)
		{
			for (int j = 0; j < magazine; j++)
			{
				file >> number;
				cfm->setAt(i, j, number, errCode);
			}
		}
		//cm
		file >> name;
		for (int i = 0; i < magazine; i++)
		{
			for (int j = 0; j < shop; j++)
			{
				file >> number;
				cms->setAt(i, j, number, errCode);
			}
		}

		//ud
		file >> name;
		for (int i = 0; i < deliverer; i++)
		{
			file >> number;
			ud->setAt(i, number, errCode);
		}
		//uf
		file >> name;
		for (int i = 0; i < factory; i++)
		{
			file >> number;
			uf->setAt(i, number, errCode);
		}
		//um
		file >> name;
		for (int i = 0; i < magazine; i++)
		{
			file >> number;
			um->setAt(i, number, errCode);
		}
		//p
		file >> name;
		for (int i = 0; i < shop; i++)
		{
			file >> number;
			p->setAt(i, number, errCode);
		}
		//xdminmax
		file >> name;
		for (int i = 0; i < deliverer * 2; i++)
		{
			for (int j = 0; j < factory; j++)
			{
				file >> number;
				xdminmax->setAt(i, j, number, errCode);
			}
		}
		//xfminmax
		file >> name;
		for (int i = 0; i < factory * 2; i++)
		{
			for (int j = 0; j < magazine; j++)
			{
				file >> number;
				xfminmax->setAt(i, j, number, errCode);
			}
		}
		//xmminmax
		file >> name;
		for (int i = 0; i < magazine * 2; i++)
		{
			for (int j = 0; j < shop; j++)
			{
				file >> number;
				xmminmax->setAt(i, j, number, errCode);
			}
		}

		file.close();
		file.clear();
	}//if (file.is_open())

	//plik wynikowy
	std::ifstream file2;
	file2.open(SolutionFile);
	if (file2.is_open())
	{
		//D
		file2 >> name;
		file2 >> number;
		D = number;
		//F
		file2 >> name;
		file2 >> number;
		F = number;
		//M
		file2 >> name;
		file2 >> number;
		M = number;
		//S
		file2 >> name;
		file2 >> number;
		S = number;
		//xd
		file2 >> name;
		for (int i = 0; i < D; i++) {
			for (int j = 0; j < F; j++) {
				file2 >> number;
				xdf->setAt(i, j, number, errCode);

			}
		}
		//xf
		file2 >> name;
		for (int i = 0; i < F; i++) {
			for (int j = 0; j < M; j++) {
				file2 >> number;
				xfm->setAt(i, j, number, errCode);
			}
		}
		//xm
		file2 >> name;
		for (int i = 0; i < M; i++) {
			for (int j = 0; j < S; j++) {
				file2 >> number;
				xms->setAt(i, j, number, errCode);
			}
		}

		file2.close();
	}
	else {
		errCode = FILE_COULD_NOT_BE_OPPENED;
	}
	/*if (!isInRange(makeSolution())) {
		errCode = INCORRECT_VALUE;
	}*/
}
void CMscnProblem::putInfoToFile(int & errCode)
{
	std::ofstream file;
	file.open("SolutionOutput.txt");
	//std::cout <<"\nSOL_OPENED:"<< file.is_open();
	if (file.is_open()) {
		file << "D ";
		file << D;
		file << ";\n";
		file << "F ";
		file << F;
		file << ";\n";
		file << "M ";
		file << M;
		file << ";\n";
		file << "S ";
		file << S;
		file << ";\n";

		file << "xd \n";
		for (int i = 0; i < D; i++) {
			for (int j = 0; j < F; j++) {
				file << xdf->get(i,j,errCode)<< ";";
			}
			file << "\n";
		}
		file << "xf \n";
		for (int i = 0; i < F; i++) {
			for (int j = 0; j < M; j++) {
				file << xfm->get(i, j, errCode) << ";";
			}
			file << "\n";
		}
		file << "xm \n";
		for (int i = 0; i < M; i++) {
			for (int j = 0; j < S; j++) {
				file << xms->get(i, j, errCode) << ";";
			}
			file << "\n";
		}
		file.close();
	}

}
//void CMscnProblem::getInfoFromFile(int &errCode)


bool CMscnProblem::isInRange(double *pdSolution)
{
	int index = 0;
	int err = 0;
	for (int i = 0; i < D; i++)
	{
		for (int j = 0; j < F; j++)
		{
			if (pdSolution[index] < xdminmax->get(i,j,err)|| pdSolution[index] > xdminmax->get(i+1,j,err))
			{
				return false;
			}
			index++;
		}
	}
	for (int i = 0; i < F; i++)
	{
		for (int j = 0; j < M; j++)
		{
			if (pdSolution[index] < xfminmax->get(i, j, err) || pdSolution[index] > xfminmax->get(i, j, err))
			{
				return false;
			}
			index++;
		}
	}
	for (int i = 0; i < M; i++)
	{
		for (int j = 0; j < S; j++)
		{
			if (pdSolution[index] < xmminmax->get(i, j, err) || pdSolution[index] > xmminmax->get(i, j, err))
			{
				return false;
			}
			index++;
		}
	}

	return true;

}//bool CMscnProblem::isInRange(double *pdSolution)

/*
double CMscnProblem::getMin(double * pdSolution, int index)
{
	int max = D * F + F * M + M * S;
	int ims = D * F + F * M;
	int ifm = D * F;
	int idf = 0;
	if (index >= ims) {
		std::cout << "@1@";
		return xmm_m[(max - index) % M][((index - ims) * S) % (2 * S)];
	}
	else if (index >= ifm) {
		std::cout << "@2@";
		return xmm_f[(ims - index) % F][((index - ifm) * M) % (2 * M)];
	}
	else {
		std::cout << "@3@ = ";
		return xmm_d[(ifm - index) % D][((index - idf) * F) % (2 * F)];
	}
}
*/


double* CMscnProblem::makeSolution()
{
	int err = 0;
	double* out;
	out = new double[(D*F + F * M + M * S)];
	int index = 0;
	for (int i = 0; i < D; i++)
	{
		for (int j = 0; j < F; j++)
		{
			out[index] = (xdf->get(i,j,err));
			index++;
		}
	}
	for (int i = 0; i < F; i++)
	{
		for (int j = 0; j < M; j++)
		{
			out[index] = (xfm->get(i, j, err));
			index++;

		}
	}
	for (int i = 0; i < M; i++)
	{
		for (int j = 0; j < S; j++)
		{
			out[index] = (xms->get(i, j, err));
			index++;

		}
	}
	return out;
}//double* CMscnProblem::makeSolution()
