#include "stdafx.h"
#include "DORFDiagnosis.h"

vector<string> DORFGetTestCase(string expression)
{
	vector<string> testcase;
	Utility utility;
	vector<string> terms = utility.extractDNFstyle(expression);
	if (terms.size() <= 1) {
		return testcase;
	}

	vector<vector<string>> pVector(terms.size());

	for (int i = 0; i < terms.size() - 1; i++){
		vector<string> otp;
		vector<string> utp;

		string term1 = terms[i] + "+" + terms[i + 1];
		generateOTPandUTP(term1, otp, utp);//求UTP
		pVector[i] = utp;

		for (int j = 0; j < terms.size(); j++){

			if (j != i && j != i + 1){
				vector<string> fp;
				generateFP(terms[j], fp);//求FP
				pVector[j] = fp;
			}
		}

		//组合
		string res;
		for (int k = 0; k < pVector.size(); k++){
			for (int m = 0; m < pVector[k].size(); m++){
				int index = pVector[k][m].find(" ");
				string str = pVector[k][m].substr(0, index);
				res += str;
				break;
			}
		}
		testcase.push_back(res);
		for (int k = 0; k < terms.size(); k++)
			pVector[k].clear();
	}
	return testcase;
}

bool detectDORF(string newExp, string faultExpression){
	vector<string> DORFTestCases = DORFGetTestCase(newExp);
	printf("----------DORF testcase:");
	for (int i = 0; i < DORFTestCases.size(); i++){
		printf("[%s]", DORFTestCases[i].c_str());
	}
	printf("\n");
	DnfMutant dnfxMutant;
	Mutants xMutant = dnfxMutant.getMutants(newExp, "DORF");
	Utility uti;
	for (int i = 0; i < DORFTestCases.size(); i++){
		bool v = (uti.evaluate(newExp, DORFTestCases[i])) ^ (uti.evaluateMutant(newExp, faultExpression, DORFTestCases[i]));
		if (v == true){
			if (checkEqualment(xMutant.mutants[i].mutant.c_str(), faultExpression)){
				printf("----------detect fault:[%s]\n", xMutant.mutants[i].mutant.c_str());
				return true;
			}
			else{
				printf("----------no fault\n");
			}
			return false;
		}
	}
	printf("----------no fault\n");
	return false;
}