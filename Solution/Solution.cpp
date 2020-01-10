#include "pch.h"
#include "CMscnProblem.h"
#include "CRandom.h"
#include "CRandomSearch.h"
#include "Matrix.h"
#include "Timer.h"

#include <iostream>
#include <ctime>
#include <time.h>


//testing
int main()
{

	int f = 0;
	double tq = 0;

	for (int i = 0; i < 1; i++) {
		CMscnProblem *problem = new CMscnProblem(2,3,4,6);

		//problem->getInfoFromFile(f);
		problem->generateInstance(5);
		problem->printValues();

		problem->generateSolution(0);
		problem->makeSolution();
		problem->printSolution();
		double fq = problem->getQuality();
		tq = problem->getQuality();
		std::cout << "\nQuality Before: " << problem->getQuality() << "\n";
		std::cout << "Constraints Before: " << problem->ConstraintsSatisfied() << "\n";
		CRandomSearch search(problem);
		
		search.searchByTriesSolution(1000000);
		std::cout << "Quality Solution: " << problem->getQuality() << "\n";
		std::cout << "Constraints: " << problem->ConstraintsSatisfied() << "\n";
		search.searchByTriesRandomCount(1000000);
		std::cout << "Quality RandomCount: " << problem->getQuality() << "\n";
		std::cout << "Constraints: " << problem->ConstraintsSatisfied() << "\n";
		search.searchByTriesOneValue(1000000);
		std::cout << "Quality Single: " << problem->getQuality() << "\n";
		std::cout << "Constraints: " << problem->ConstraintsSatisfied() << "\n";

		tq = problem->getQuality();

		std::cout << "Solution: ";
		problem->printSolution();
	}
	//std::cout << "Best solution: " << q;
}

/*

DANE:
D<liczbaD> - l. dostawcow
F<liczbaF> - l. fabryk
M<liczbaM> - l. magazynow
S<liczbaS> - l. sklepow
sd<liczbaD wartosci> - moc produkcyjna dostawcy
sf<liczbaF wartosci> - moc produkcyjna fabryki
sm<liczbaM wartosci> - pojemnosc magazynu
ss<liczbaS wartosci> - zapotrzebowanie sklepu
cd<macierz[d*f]> - koszt wytworzenia i przewiezienia surowca od dostawcy d do fabryki f
cf<macierz[f*m]> - koszt wyprodukowania produktu w fabryce f i przewiezienia go do magazynu m
cm<macierz[m*s]> - koszt przechowania produktu w magazynie m i przewiezienia go do sklepu s
ud<liczbaD wartosci> - jednorazowy koszt korzystania z usług dostawcy d
uf<liczbaF wartosci> - jednorazowy koszt korzystania z fabryki f
um<liczbaM wartosci> - jednorazowy koszt korzystania z magazynu m
ps<liczbaS wartosci> - Sprzedanie (dostarczenie) produktu w sklepie s, daje przychód p
xdminmax<liczbaD*2*liczbaF wartości> - minimalna i maksymalna możliwa ilość surowca do wysłania(ile f może zamowic)
xfminmax<liczbaF*2*liczbaM wartości> - minimalna i maksymalna możliwa ilość produktów do wysłania(ile m może zamowic)
xmminmax<liczbaM*2*liczbaS wartości> - minimalna i maksymalna mozliwa ilość produktów do wysłania(ile s może zamowic)
___________________________________________________________________________________________________
ROZWIĄZANIE:
D<liczbaD> -
F<liczbaF> -
M<liczbaM> -
S<liczbaS> -
xd<macierz[d*f]> - ilosc towaru wysylana od dostawcy d do fabryki f
xf<macierz[f*m]> - ilosc towaru wysylana z fabryki f do magazynu m
xm<macierz[m*s]> - ilosc towaru wysylana z magazynu m do sklepu s

*//**/