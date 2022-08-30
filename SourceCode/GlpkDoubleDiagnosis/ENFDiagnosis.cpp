#include "stdafx.h"
#include "ENFDiagnosis.h"

bool detectENF(string newExp, string faultExpression, string UTP){
	vector<string> ENFTestCases = { UTP };
	printf("----------ENF testcase:");
	for (int i = 0; i < ENFTestCases.size(); i++){
		printf("[%s]", ENFTestCases[i].c_str());
	}
	printf("\n");
	DnfMutant dnfxMutant;
	Mutants xMutant = dnfxMutant.getMutants(newExp, "ENF");
	Utility uti;
	for (int i = 0; i < ENFTestCases.size(); i++){
		bool v = (uti.evaluate(newExp, ENFTestCases[i])) ^ (uti.evaluateMutant(newExp, faultExpression, ENFTestCases[i]));
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
