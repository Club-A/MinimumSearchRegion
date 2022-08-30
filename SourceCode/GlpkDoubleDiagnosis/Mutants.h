#pragma once
#include <vector>
using namespace std;

class Mutant
{
public:
	string mutant;
	string type;
	vector<string> oriterms;
	vector<string> faultterms;
public:

	Mutant(string m, string type);

	Mutant(string term1, string term2, string type);

	Mutant(string m, string type, vector<string> oriterms, vector<string> faultterms);
	~Mutant(void);
	
};



class Mutants
{
public:
	string originalExp;
	vector<Mutant> mutants;
	vector<Mutant> detected;
	vector<Mutant> unDetected;
	vector<Mutant> equivalent;

public:
	Mutants(string originalExpression);
	~Mutants(void);

	void addMutant(string exp, string type, vector<string> oriterms, vector<string> faultterms);
	void checkequivalent();

	void operator +=(Mutants mut);

	void addMutant(string exp, string type);

	void addMutant(string term1, string term2, string type);
};

