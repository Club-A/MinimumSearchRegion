//
// Created by gaofei17 on 2022/2/9.
//
#include "stdafx.h"
#include "TestCase.h"
#include "Utility.h"
#include "Literal.h"
#include "NFP.h"

NFP::NFP(void){

}
NFP::NFP(string testcase, string t, string model) :term(t)
{
	Utility uti;
	vector<string> literalInTerm = uti.extractCNFstyle(t);
	for (int i = 0; i < literalInTerm.size(); i++){
		if (literalInTerm[i].at(0) == '!'){
			char c = testcase[model.find(literalInTerm[i].at(1))];
			if (c == '1'){
				key = Literal(literalInTerm[i].at(1), false);
			}
			else{
				other.push_back(Literal(literalInTerm[i].at(1), false));
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
Literal NFP::getKey(){
	return key;
}
list<Literal> NFP::getOther(){
	return other;
}
list<Literal> NFP::getValid(){
	return valid;
}
