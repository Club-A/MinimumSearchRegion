//
// Created by gaofei on 2022/2/9.
//
#include "stdafx.h"
#include "TestCase.h"
#include "Utility.h"
#include "Literal.h"
#include "UTP.h"

UTP::UTP(void){

}
UTP::UTP(string testcase, string t, string model) :term(t)
{
	Utility uti;
	//生成本项词inner和非本项词other
	list<Literal> allLiterals;
	for (int i = 0; i<model.length(); i++){
		allLiterals.push_back(Literal(model.at(i), true));
	}
	other = allLiterals;
	vector<string> literalInTerm = uti.extractCNFstyle(t);
	for (int i = 0; i<literalInTerm.size(); i++){
		string literal = literalInTerm[i];
		if (literal.at(0) == '!')
		{
			inner.push_back(Literal(literal.at(1), false));
			other.remove(Literal(literal.at(1), true));
		}
		else
		{
			inner.push_back(Literal(literal.at(0), true));
			other.remove(Literal(literal.at(0), true));
		}
	}

	//生成有效插入词 valid
	for (Literal literal : other){
		char c = testcase[model.find(literal.getChar())];
		if (c == '1'){
			valid.push_back(Literal(literal.getChar(), false));
		}
		else{
			valid.push_back(Literal(literal.getChar(), true));
		}

	}

}
list<Literal> UTP::getInner()
{
	return inner;
}
list<Literal> UTP::getOther()
{
	return other;
}
list<Literal> UTP::getValid()
{
	return valid;
}
