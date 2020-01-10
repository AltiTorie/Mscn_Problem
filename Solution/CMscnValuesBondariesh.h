#pragma once

class CMscnValuesBoundaries
{
public:
	CMscnValuesBoundaries() {}
	~CMscnValuesBoundaries() {}
	/*
	sd/sf/sm min - 100, inaczej sa za male na taki problem
	sd/sf/sm max - 1000, wieksze wartosci moglyby byc problematyczne, do losowych przypadkow
	ss min - 10 bo jw.
	ss max - 500, (np jakis supermarket czy cos)
	*/
	const int MIN_SD_SF_SM = 100;
	const int MAX_SD_SF_SM = 1000;

	const int MIN_SS = 10;
	const int MAX_SS = 500;

	/*
	cdf/cfm/cms min - 5% * mozliwego przetworzenia dla nizszego stopnia * losowy czynnik "paliwa" * wsp. wielkosci tego stopnia(1/rand(1,10))
	(cdf-sf,cfm-sm,cms-ss)(dla cdf 5/100 * sf * rand * 1/rand(1,10) * 5/100 * sd)
	*/
	const double SIZE_FACTOR = 0.05;
	const double NORMAL_COST = 0.05;
	const double MIN_FUEL_COST = 0.2;
	const double MAX_FUEL_COST = 1;
	
	/*
	ud/uf/um min - taniej dla mniejszych firm (10% s)
	*/
	const double AGREEMENT_SIZE_FACTOR = 0.1;
	/*
	p - drozej dla mniejszych sklepow(niewiele ale zawsze)(10% s)
	*/
	const double PRICE_FACTOR = 0.1;
	/*
	xd/xf/xm min - 2% mozliwego wytworzenia(sd/sf/sm)(dla xd 2/100 * sd)
	
	*/
	const double MIN_XD_XF_XM = 0.02;
	const double MAX_XD = 0.7;
	const double MAX_XF = 0.6;
	const double MAX_XM = 0.5;


};

