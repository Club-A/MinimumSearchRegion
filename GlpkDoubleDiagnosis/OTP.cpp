//
// Created by gaofei on 2022/3/25.
//
#include "stdafx.h"
#include "TestCase.h"
#include "Utility.h"
#include "Literal.h"
#include "OTP.h"

OTP::OTP(void){

}
OTP::OTP(string testcase,string t):term(t){
	Utility uti;

	vector<string> literalInTerm = uti.extractCNFstyle(t);
	for (int i = 0; i<literalInTerm.size(); i++){
		string literal = literalInTerm[i];
		if (literal.at(0) == '!')
		{
			inner.push_back(Literal(literal.at(1), false));
		}
		else
		{
			inner.push_back(Literal(literal.at(0), true));
		}
	}
}
list<Literal> OTP::getInner()
{
	return inner;
}