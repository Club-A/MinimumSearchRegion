#include "stdafx.h"
//#include "DiagnosisUtility.h"
#include "TestCase.h"
//#include "DiagnosisUtility.h"
#include "Utility.h"
#include "Literal.h"
#include <list> 

UTPorNFP::UTPorNFP(void)
{

}
UTPorNFP::UTPorNFP(string testcase,string model,string t,bool check) :term(t)
{
	Utility uti;
	//UTP
	if (check){
		                                                                               

	
	}
	//NFP
	else{
		vector<string> literalInTerm = uti.extractCNFstyle(t);
		for (int i = 0; i < literalInTerm.size(); i++){
			if (literalInTerm[i].at(0) == '!'){
				char c = testcase[model.find(literalInTerm[i].at(1))];
				if (c == '1'){
					key = Literal(literalInTerm[i].at(1),false);
				}
				else{
					other.push_back(Literal(literalInTerm[i].at(1),false));
				}
			}
			else{
				char c = testcase[model.find(literalInTerm[i].at(0))];
				if (c == '0'){
					key = Literal(literalInTerm[i].at(0), true);
				}
				else{
					other.push_back(Literal(literalInTerm[i].at(0), true));
				}
			}
		}

		for (int m = 0; m < model.size(); m++){
			if (t.find(model.at(m)) == t.npos){
				if (testcase.at(m) == '0'){
					valid.push_back(Literal(model.at(m), false));
				}
				else{
					valid.push_back(Literal(model.at(m), true));
				}
			}
			else{
				
			}
		}
	}
}
Literal UTPorNFP::getKey(){
	return key;
}
list<Literal> UTPorNFP::getOther(){
	return other;
}
list<Literal> UTPorNFP::getValid(){
	return valid;
}
	
