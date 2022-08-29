#pragma once
#include <list>
#include"Literal.h"
using namespace std;

class UTPorNFP
{
	public:
		string term;        //对应项
	private:
		Literal key;		//关键变量
		list<Literal> other;//其他变量
		list<Literal> valid;//有效替换变量
	public:
		UTPorNFP();
		UTPorNFP(string testcase,string model,string term,bool check);
		Literal getKey();
		list<Literal> getOther();
		list<Literal> getValid();

};

