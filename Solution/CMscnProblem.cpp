#include "pch.h"
#include "CMscnProblem.h"


CMscnProblem::CMscnProblem()
{
	CMscnProblem(1, 1, 1, 1);
}

CMscnProblem::CMscnProblem(int _D, int _F, int _M, int _S)
{

	deliverer = _D;
	factory = _F;
	magazine = _M;
	shop = _S;

	D = _D;
	F = _F;
	M = _M;
	S = _S;

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
	xdminmax = new Matrix<double>(deliverer, 2 * factory);
	xfminmax = new Matrix<double>(factory, 2 * magazine);
	xmminmax = new Matrix<double>(magazine, 2 * shop);

	//solution values
	xdf = new Matrix<double>(deliverer, factory);
	xfm = new Matrix<double>(factory, magazine);
	xms = new Matrix<double>(magazine, shop);

	solution = new double[(D*F + F * M + M * S)];
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
	delete solution;

}//CMscnProblem::~CMscnProblem()

void CMscnProblem::setDelivererCount(int count, int & errCode)
{

	if (count <= 0)
	{
		errCode = INCORRECT_VALUE;
	}
	else if (count == deliverer)
	{
		errCode = NOTHING_CHANGED;
	}
	else
	{

		//better version
		sd->changeSize(count, errCode);
		cdf->changeSizeX(count, errCode);
		ud->changeSize(count, errCode);
		xdminmax->changeSizeX(count, errCode);
		xdf->changeSizeX(count, errCode);
		//
		deliverer = count;


	}
}//void CMscnProblem::setDelivererCount(int count, int & errCode)

void CMscnProblem::setFactoryCount(int count, int & errCode)
{
	if (count <= 0)
	{
		errCode = INCORRECT_VALUE;
	}
	else if (count == factory)
	{
		errCode = NOTHING_CHANGED;
	}
	else
	{
		//better version

		sf->changeSize(count, errCode);
		cdf->changeSizeY(count, errCode);
		cfm->changeSizeX(count, errCode);
		uf->changeSize(count, errCode);
		xdminmax->changeSizeY(2 * count, errCode);
		xfminmax->changeSizeX(count, errCode);
		xdf->changeSizeY(count, errCode);
		xfm->changeSizeX(count, errCode);
		//
		factory = count;
	}
}//void CMscnProblem::setFactoryCount(int count, int & errCode)

void CMscnProblem::setMagazineCount(int count, int & errCode)
{

	if (count <= 0)
	{
		errCode = INCORRECT_VALUE;
	}
	else if (count == magazine)
	{
		errCode = NOTHING_CHANGED;
	}
	else
	{
		//better version

		sm->changeSize(count, errCode);
		cms->changeSizeX(count, errCode);
		cfm->changeSizeY(count, errCode);
		um->changeSize(count, errCode);
		xmminmax->changeSizeX(count, errCode);
		xfminmax->changeSizeY(2 * count, errCode);
		xms->changeSizeX(count, errCode);
		xfm->changeSizeY(count, errCode);

		//

		magazine = count;
	}
}//void CMscnProblem::setMagazineCount(int count, int & errCode)

void CMscnProblem::setShopCount(int count, int & errCode)
{
	if (count <= 0)
	{
		errCode = INCORRECT_VALUE;
	}
	else if (count == shop)
	{
		errCode = NOTHING_CHANGED;
	}
	else
	{


		//better version

		ss->changeSize(count, errCode);
		cms->changeSizeY(count, errCode);
		p->changeSize(count, errCode);
		xmminmax->changeSizeY(2 * count, errCode);
		xms->changeSizeY(count, errCode);

		shop = count;
	}
}//void CMscnProblem::setShopCount(int count, int & errCode)

int epsilon(double a) {
	return a > 0 ? 1 : 0;
}

double CMscnProblem::getQuality(double *pdSolution, int &errCode)
{
	errCode = 1;
	if (pdSolution == nullptr)
	{
		errCode = NULL_ARRAY;
	}
	try
	{
		for (int i = 0; i < D*F + F * M + M * S; i++)
		{
			if (pdSolution[i] < 0)
			{
				errCode = INCORRECT_VALUE;
			}
		}
	}
	catch (...)
	{
		errCode = INVALID_ARRAY_BOUNDARIES;
		return 0;
	}

	int index = 0;
	double KT = 0;
	for (int i = 0; i < D; i++)
	{
		for (int j = 0; j < F; j++)
		{
			KT += (cdf->get(i, j, errCode)) * pdSolution[index];
			index++;

		}
	}
	for (int i = 0; i < F; i++)
	{
		for (int j = 0; j < M; j++)
		{
			KT += (cfm->get(i, j, errCode)) * pdSolution[index];
			index++;
		}
	}
	for (int i = 0; i < M; i++)
	{
		for (int j = 0; j < S; j++)
		{
			KT += (cms->get(i, j, errCode)) * pdSolution[index];
			index++;
		}
	}
	//std::cout << "Koszta transportu: " << KT<<"\n";
	index = 0;
	double KU = 0;
	double helper = 0;
	for (int i = 0; i < D; i++)
	{
		helper = 0;
		for (int j = 0; j < F; j++)
		{
			helper += pdSolution[index];
			index++;

		}
		KU += (epsilon(helper) * (ud->get(i, errCode)));
	}
	for (int i = 0; i < F; i++)
	{
		helper = 0;
		for (int j = 0; j < M; j++)
		{
			helper += pdSolution[index];
			index++;

		}
		KU += (epsilon(helper) * (uf->get(i, errCode)));
	}

	for (int i = 0; i < M; i++)
	{
		helper = 0;
		for (int j = 0; j < S; j++)
		{
			helper += pdSolution[index];
			index++;
		}
		KU += (epsilon(helper) * (um->get(i, errCode)));
	}
	//std::cout << "Koszta umow: " << KU << "\n";
	double P = 0;
	index = D * F + F * M;
	for (int i = 0; i < M; i++)
	{
		for (int j = 0; j < S; j++)
		{
			P += p->get(j, errCode) * pdSolution[index];

			index++;
		}
	}
	//std::cout << "Przychod: " << P << "\n";
	double z = P - KT - KU;
	return z;
}
double CMscnProblem::getQuality()
{
	int err = 0;
	return getQuality(solution, err);
}
//double CMscnProblem::dGetQuality(double *pdSolution, int &errCode)

bool CMscnProblem::ConstraintsSatisfied(double * pdSolution, int & errCode)
{
	double sumHelper = 0;
	int index = 0;
	//Sumaryczna iloœæ surowca zamówiona u danego dostawcy nie mo¿e przekroczyæ
	//	jego mocy produkcyjnych sd
	for (int i = 0; i < D; i++)
	{
		for (int j = 0; j < F; j++)
		{
			sumHelper += pdSolution[index];
			index++;
		}
		if (sumHelper > sd->get(i, errCode))
		{
			return false;
		}
		sumHelper = 0;
	}

	//Sumaryczna iloœæ produktów zamówiona w danej fabryce nie mo¿e przekroczyæ jej
	//mocy produkcyjnych sf
	sumHelper = 0;
	for (int i = 0; i < F; i++)
	{
		for (int j = 0; j < M; j++)
		{
			sumHelper += pdSolution[index];
			index++;
		}
		if (sumHelper > sf->get(i, errCode))
		{
			return false;
		}
		sumHelper = 0;
	}
	//Sumaryczna iloœæ produktów zamówiona pobierana z danego magazynu nie mo¿e
	//	przekroczyæ jego pojemnoœci sm
	sumHelper = 0;
	for (int i = 0; i < M; i++)
	{
		for (int j = 0; j < S; j++)
		{
			sumHelper += pdSolution[index];
			index++;
		}
		if (sumHelper > sm->get(i, errCode))
		{
			return false;
		}
		sumHelper = 0;
	}
	//Sumaryczna iloœæ produktów dostarczana do danego sklepu nie mo¿e przekroczyæ
	//zapotrzebowania rynkowego dla tego sklepu ss
	index = D * F + M * F;
	sumHelper = 0;
	for (int i = 0; i < S; i++)
	{
		for (int j = 0; j < M; j++)
		{
			sumHelper += pdSolution[index];
			index++;
		}
		if (sumHelper > ss->get(i, errCode))
		{
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
		for (int j = i; j < D*F; j += F)
		{
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
	if (secondSum > sumHelper)
	{
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
		for (int j = i; j < F*M; j += M)
		{
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
	if (secondSum > sumHelper)
	{
		return false;
	}

	return true;
}
bool CMscnProblem::ConstraintsSatisfied()
{
	int err = 0;
	return ConstraintsSatisfied(solution, err);
}//bool CMscnProblem::bConstraintsSatisfied(double * pdSolution, int & errCode)

bool CMscnProblem::isInRange(double *pdSolution)
{
	int index = 0;
	int err = 0;
	for (int i = 0; i < D; i++)
	{
		for (int j = 0; j < F; j++)
		{
			if (pdSolution[index] < xdminmax->get(i, j, err) || pdSolution[index] > xdminmax->get(i + 1, j, err))
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

void CMscnProblem::generateInstance(int instanceSeed)
{
	int errCode = 0;
	CRandom random(instanceSeed);
	CMscnValuesBoundaries BOUNDS;
	for (int i = 0; i < sd->getSize(); i++)
	{
		sd->setAt(i, random.getRandomInt(BOUNDS.MIN_SD_SF_SM, BOUNDS.MAX_SD_SF_SM), errCode);
	}
	for (int i = 0; i < sf->getSize(); i++)
	{
		sf->setAt(i, random.getRandomInt(BOUNDS.MIN_SD_SF_SM, BOUNDS.MAX_SD_SF_SM), errCode);
	}
	for (int i = 0; i < sm->getSize(); i++)
	{
		sm->setAt(i, random.getRandomInt(BOUNDS.MIN_SD_SF_SM, BOUNDS.MAX_SD_SF_SM), errCode);
	}
	for (int i = 0; i < ss->getSize(); i++)
	{
		ss->setAt(i, random.getRandomInt(BOUNDS.MIN_SS, BOUNDS.MAX_SS), errCode);
	}

	for (int i = 0; i < cdf->getSizeX(); i++)
	{
		for (int j = 0; j < cdf->getSizeY(); j++)
		{
			double wsp = (1 / random.getRandomDouble(1, 10));
			cdf->setAt(i, j, BOUNDS.NORMAL_COST * sf->get(j, errCode) * random.getRandomDouble(BOUNDS.MIN_FUEL_COST, BOUNDS.MAX_FUEL_COST) * wsp, errCode);
		}
	}

	for (int i = 0; i < cfm->getSizeX(); i++)
	{
		for (int j = 0; j < cfm->getSizeY(); j++)
		{
			double wsp = (1 / random.getRandomDouble(1, 10));
			cfm->setAt(i, j, BOUNDS.NORMAL_COST * sm->get(j, errCode) * random.getRandomDouble(BOUNDS.MIN_FUEL_COST, BOUNDS.MAX_FUEL_COST) * wsp, errCode);
		}
	}
	for (int i = 0; i < cms->getSizeX(); i++)
	{
		for (int j = 0; j < cms->getSizeY(); j++)
		{
			double wsp = (1 / random.getRandomDouble(1, 10));
			cms->setAt(i, j, BOUNDS.NORMAL_COST * ss->get(j, errCode) * random.getRandomDouble(BOUNDS.MIN_FUEL_COST, BOUNDS.MAX_FUEL_COST) * wsp, errCode);
		}
	}

	for (int i = 0; i < ud->getSize(); i++)
	{
		ud->setAt(i, BOUNDS.AGREEMENT_SIZE_FACTOR * sd->get(i, errCode), errCode);
	}
	for (int i = 0; i < uf->getSize(); i++)
	{
		uf->setAt(i, BOUNDS.AGREEMENT_SIZE_FACTOR * sf->get(i, errCode), errCode);
	}
	for (int i = 0; i < um->getSize(); i++)
	{
		um->setAt(i, BOUNDS.AGREEMENT_SIZE_FACTOR * sm->get(i, errCode), errCode);
	}
	for (int i = 0; i < p->getSize(); i++)
	{
		p->setAt(i, BOUNDS.PRICE_FACTOR * ss->get(i, errCode), errCode);
	}

	for (int i = 0; i < xdminmax->getSizeX(); i++)
	{
		for (int j = 0; j < xdminmax->getSizeY(); j += 2)
		{
			xdminmax->setAt(i, j, BOUNDS.MIN_XD_XF_XM * sd->get(i, errCode), errCode);
			xdminmax->setAt(i, j + 1, BOUNDS.MAX_XD_XF_XM * sd->get(i, errCode), errCode);
		}
	}
	for (int i = 0; i < xfminmax->getSizeX(); i++)
	{
		for (int j = 0; j < xfminmax->getSizeY(); j += 2)
		{
			xfminmax->setAt(i, j, BOUNDS.MIN_XD_XF_XM * sf->get(i, errCode), errCode);
			xfminmax->setAt(i, j + 1, BOUNDS.MAX_XD_XF_XM * sf->get(i, errCode), errCode);
		}
	}
	for (int i = 0; i < xmminmax->getSizeX(); i++)
	{
		for (int j = 0; j < xmminmax->getSizeY(); j += 2)
		{
			xmminmax->setAt(i, j, BOUNDS.MIN_XD_XF_XM * sm->get(i, errCode), errCode);
			xmminmax->setAt(i, j + 1, BOUNDS.MAX_XD_XF_XM * sm->get(i, errCode), errCode);
		}
	}

}//void CMscnProblem::GenerateInstance(int instanceSeed)

void CMscnProblem::generateSolution(int instanceSeed) {
	int errCode = 0;

	CRandom random(instanceSeed);
	do
	{
		for (int i = 0; i < xdf->getSizeX(); i++)
		{
			for (int j = 0; j < xdf->getSizeY(); j++)
			{
				int low = xdminmax->get(i, 2 * j, errCode);
				int big = xdminmax->get(i, (2 * j) + 1, errCode);
				xdf->setAt(i, j, random.getRandomInt(low, big), errCode);

			}
		}

		for (int i = 0; i < xfm->getSizeX(); i++)
		{
			for (int j = 0; j < xfm->getSizeY(); j++)
			{
				int low = xfminmax->get(i, 2 * j, errCode);
				int big = xfminmax->get(i, (2 * j) + 1, errCode);
				xfm->setAt(i, j, random.getRandomInt(low, big), errCode);
			}
		}
		for (int i = 0; i < xms->getSizeX(); i++)
		{
			for (int j = 0; j < xms->getSizeY(); j++)
			{
				int low = xmminmax->get(i, 2 * j, errCode);
				int big = xmminmax->get(i, (2 * j) + 1, errCode);
				xms->setAt(i, j, random.getRandomInt(low, big), errCode);
			}
		}
		makeSolution();
	} while (!ConstraintsSatisfied());
}


double CMscnProblem::getMin(double * pdSolution, int index)
{
	int errCode = 0;

	int max = (D * F + F * M + M * S) - 1;
	int ims = (D * F + F * M) - 1;
	int ifm = (D * F) - 1;
	if (index > max)
	{
		return NULL;
	}

	if (index > ims)
	{
		return xmminmax->get(((index - (ims + 1)) / S), 2 * ((index - (ims + 1)) % (S)), errCode);
	}
	else if (index > ifm)
	{
		return xfminmax->get(((index - (ifm + 1)) / M), 2 * ((index - (ifm + 1)) % (M)), errCode);
	}
	else
	{
		return xdminmax->get(((index) / F), 2 * ((index) % (F)), errCode);
	}
}

double CMscnProblem::getMin(int index)
{
	return getMin(solution, index);
}

double CMscnProblem::getMax(double *pdSolution, int index)
{
	int errCode = 0;

	int ims = (D * F + F * M) - 1;
	int ifm = (D * F) - 1;
	if (index >= D * F + F * M + M * S)
	{
		return NULL;
	}

	if (index > ims)
	{
		return xmminmax->get(((index - ims - 1) / S), 2 * ((index - (ims + 1)) % (S)) + 1, errCode);
	}
	else if (index > ifm)
	{
		return xfminmax->get(((index - ifm - 1) / M), 2 * ((index - (ifm + 1)) % (M)) + 1, errCode);
	}
	else
	{
		return xdminmax->get(((index) / F), 2 * ((index) % (F)) + 1, errCode);
	}
}

double CMscnProblem::getMax(int index)
{
	return getMax(solution, index);
}

double CMscnProblem::getAvg(int index)
{
	if (index > D*F + F * M) {
		return S;
	}
	else if (index > D*F) {
		return M;
	}
	else {
		return F;
	}
}



void CMscnProblem::makeSolution()
{
	int err = 0;
	delete solution;
	solution = new double[D*F + F * M + M * S];
	int index = 0;
	for (int i = 0; i < D; i++)
	{
		for (int j = 0; j < F; j++)
		{
			solution[index] = xdf->get(i, j, err);
			index++;
		}
	}
	for (int i = 0; i < F; i++)
	{
		for (int j = 0; j < M; j++)
		{
			solution[index] = xfm->get(i, j, err);
			index++;

		}
	}
	for (int i = 0; i < M; i++)
	{
		for (int j = 0; j < S; j++)
		{
			solution[index] = xms->get(i, j, err);
			index++;

		}
	}

}
void CMscnProblem::setSolutionValueAt(int index, double value)
{
	if (index < getSolutionRange() && index >= 0) {
		solution[index] = value;
	}
}
//double* CMscnProblem::makeSolution()


void CMscnProblem::getInfoFromFile(int &errCode)
{

	char ProblemFile[] = "test.txt";
	char SolutionFile[] = "sol.txt";

	char name[256];

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
			sd->setAt(i, number, errCode);
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
				cdf->setAt(i, j, number, errCode);
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
		for (int i = 0; i < deliverer; i++)
		{
			for (int j = 0; j < 2 * factory; j++)
			{
				file >> number;
				xdminmax->setAt(i, j, number, errCode);
			}
		}
		//xfminmax
		file >> name;
		for (int i = 0; i < factory; i++)
		{
			for (int j = 0; j < 2 * magazine; j++)
			{
				file >> number;
				xfminmax->setAt(i, j, number, errCode);
			}
		}
		//xmminmax
		file >> name;
		for (int i = 0; i < magazine; i++)
		{
			for (int j = 0; j < 2 * shop; j++)
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
		for (int i = 0; i < D; i++)
		{
			for (int j = 0; j < F; j++)
			{
				file2 >> number;
				xdf->setAt(i, j, number, errCode);

			}
		}
		//xf
		file2 >> name;
		for (int i = 0; i < F; i++)
		{
			for (int j = 0; j < M; j++)
			{
				file2 >> number;
				xfm->setAt(i, j, number, errCode);
			}
		}
		//xm
		file2 >> name;
		for (int i = 0; i < M; i++)
		{
			for (int j = 0; j < S; j++)
			{
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
}//void CMscnProblem::getInfoFromFile(int &errCode)

void CMscnProblem::putInfoToFile(int & errCode)
{
	std::ofstream file;
	file.open("SolutionOutput.txt");
	//std::cout <<"\nSOL_OPENED:"<< file.is_open();
	if (file.is_open()) {
		file << "D ";
		file << D;
		file << "\nF ";
		file << F;
		file << "\nM ";
		file << M;
		file << "\nS ";
		file << S;

		file << "\nxd \n";
		for (int i = 0; i < D; i++) {
			for (int j = 0; j < F; j++) {
				file << xdf->get(i, j, errCode) << " ";
			}
			file << "\n";
		}
		file << "xf \n";
		for (int i = 0; i < F; i++) {
			for (int j = 0; j < M; j++) {
				file << xfm->get(i, j, errCode) << " ";
			}
			file << "\n";
		}
		file << "xm \n";
		for (int i = 0; i < M; i++) {
			for (int j = 0; j < S; j++) {
				file << xms->get(i, j, errCode) << " ";
			}
			file << "\n";
		}
		file.close();
	}

}

void CMscnProblem::printValues()
{
	std::cout << "sd: \n";
	sd->print();
	std::cout << "sf: \n";
	sf->print();
	std::cout << "sm: \n";
	sm->print();
	std::cout << "ss: \n";
	ss->print();
	std::cout << "cdf: \n";
	cdf->print();
	std::cout << "cfm: \n";
	cfm->print();
	std::cout << "cms: \n";
	cms->print();
	std::cout << "ud: \n";
	ud->print();
	std::cout << "uf: \n";
	uf->print();
	std::cout << "um: \n";
	um->print();
	std::cout << "p: \n";
	p->print();
	std::cout << "xdminmax: \n";
	xdminmax->print();
	std::cout << "xfminmax: \n";
	xfminmax->print();
	std::cout << "xmminmax: \n";
	xmminmax->print();
}

void CMscnProblem::printSolution()
{
	std::cout << "[";
	for (int i = 0; i < D*F + F * M + M * S; i++) {
		std::cout << solution[i] << " ";
	}
	std::cout << "]";
}