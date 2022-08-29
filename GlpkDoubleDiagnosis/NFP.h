//
// Created by gaofei on 2022/2/9.
//

#pragma once
#include <list>
#include"Literal.h"
using namespace std;


class NFP : public UTPorNFP
{
public:
	string term;  //对应项
private:
	Literal key;  //关键变量
	list<Literal> other;  //其它变量
	list<Literal> valid;  //有效替换变量
public:
	NFP();
	NFP(string testcase, string t, string model);
	Literal getKey();
	list<Literal> getOther();
	list<Literal> getValid();
};