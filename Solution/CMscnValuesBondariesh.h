#pragma once



class CMscnValuesBoundaries
{
public:
	CMscnValuesBoundaries() {}
	~CMscnValuesBoundaries() {}
	/*
	sd/sf/sm min - 100, inaczej sa za male na taki problem
	sd/sf/sm max - 10000, wieksze wartosci moglyby byc problematyczne, do losowych przypadkow
	ss min - 10 bo jw.
	ss max - 500, (np jakis supermarket czy cos)
	*/
	const int MIN_SD_SF_SM = 100;
	const int MAX_SD_SF_SM = 1000;

	const int MIN_SS = 10;
	const int MAX_SS = 500;

	/*
	cdf/cfm/cms min - 5% * mozliwego przetworzenia dla nizszego stopnia * losowy czynnik "paliwa" * wsp. wielkosci tego stopnia(1/rand(1,10) * 5% s)
	(cdf-sf,cfm-sm,cms-ss)(dla cdf 5/100 * sf * rand * 1/rand(1,10) * 5/100 * sd)
	*/
	const double NORMAL_COST = 0.05;
	const double MIN_FUEL_COST = 2;
	const double MAX_FUEL_COST = 10;
	
	/*
	ud/uf/um min - 
	*/


	/*
	xd/xf/xm min - 1% mozliwego wytworzenia(sd/sf/sm)(dla xd 1/100 * sd)
	xd/xf/xm max - 25% mozliwego wytworzenia(sd/sf/sm)(dla xd 25/100 * sd)
	*/
	const double MIN_XD_XF_XM = 0.1;
	const double MAX_XD_XF_XM = 0.5;


};

