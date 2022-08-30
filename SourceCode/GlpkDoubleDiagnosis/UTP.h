//
// Created by gaofei on 2022/2/9.
//
#pragma once
#include <list>
#include"Literal.h"
using namespace std;

class UTP : public UTPorNFP
{
public:
	string term;  //对应项
private:
	list<Literal> inner;  //本项内词
	list<Literal> other;  //非项内词
	list<Literal> valid;  //有效插入词
public:
	UTP();
	UTP(string testcase, string t, string model);
	list<Literal> getInner();
	list<Literal> getOther();
	list<Literal> getValid();
};
