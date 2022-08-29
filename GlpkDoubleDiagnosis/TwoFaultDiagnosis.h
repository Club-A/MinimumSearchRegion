#include "stdafx.h"
#include <stdio.h>
#include <algorithm>
#include <map>
#include <vector>
#include "Expression.h"
#include "calExpValue.h"
#include "expTransform.h"
#include "Utility.h"
#include "DnfMutant.h"
#include "Mutants.h"
#include <time.h>

using namespace std;

class TwoFaultDiagnosis
{
public:
	TwoFaultDiagnosis(void);
	~TwoFaultDiagnosis(void);

	bool checkTestDiffer(string test, string exprOri/*原表达式*/, string expr/*变体*/);
	int findFromVector(vector<string> components, string needFound);
	bool diagnosis(
		string oriExp, //原表达式
		string model,
		Mutants mutants,
		vector<string> shirkCaseSet,
		vector<string> expandCaseSet,
		vector<string> utpPoints);
	string generateDNF(vector<string> terms);
	void getTermUtps(vector<string> sterms, vector<string> upPoints, string model, map<string, vector<string>> &termUtps);
	void getTermOtp2s(vector<string> sterms, vector<string> otp2Points, string model, map<string, vector<string>> &termOtp2s);
	void getTermNfps(string expr, map<string, vector<string>> termUtps, string model, map<string, vector<string>> &termNfps);
	void getExpandCases(string expression, string faultExpr, vector<string> fpCases, string model, vector<string> &expandCases);
	int IsContain(vector<string>expandCase1, vector<string>expandCase2);
	void filterUncontain(map<string, vector<string>>expandNfpTerms, map<string, vector<string>>&realExpands);
	Mutants generateALLmutants(string type, string expression, string model, vector<string>terms1, vector<string>terms2, string term1);
	int dealMain(string expression, string faultExpression, vector<string> terms, string model, vector<string> upPoints, vector<string> otp2Points, vector<string> fpPoints,
		map<string, vector<string>> termUtps, map<string, vector<string>> termOtp2s, map<string, vector<string>> termNfps, map<string, vector<string>> termOfp2s);


};