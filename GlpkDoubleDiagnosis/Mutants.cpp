#include "stdafx.h"
#include "Mutants.h"
#include "Utility.h"


Mutant::Mutant(string m, string faulttype) : mutant(m), type(faulttype)
{
}

Mutant::Mutant(string term1, string term2, string type)
{
	faultterms.push_back(term1);
	faultterms.push_back(term2);
}

Mutant::Mutant(string m, string faulttype, vector<string>oriterms, vector<string>faultterms)
:mutant(m), type(faulttype), oriterms(oriterms), faultterms(faultterms)
{
}
Mutant::~Mutant(void)
{
}
 
//////////Mutants

Mutants::Mutants(string originalExpression):originalExp(originalExpression)
{
	mutants.clear();
}


Mutants::~Mutants(void)
{
}

void Mutants::operator +=(Mutants mut)
{
	for (int i = 0; i < mut.mutants.size(); i++)
	{
		mutants.push_back(mut.mutants[i]);
	}
}

void Mutants::addMutant(string exp, string type, vector<string> oriterms, vector<string> faultterms)
{
	Mutant mutant(exp, type, oriterms, faultterms);
	mutants.push_back(mutant);
}


//Mutant
void Mutants::addMutant(string exp, string type)
{
	Mutant mutant(exp, type);
	mutants.push_back(mutant);
}

void Mutants::addMutant(string term1, string term2, string type)
{
	Mutant mutant(term1, term2, type);
	mutants.push_back(mutant);
}

