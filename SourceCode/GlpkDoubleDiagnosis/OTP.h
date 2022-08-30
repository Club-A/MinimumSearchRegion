//
// Created by gaofei on 2022/3/25.
//
#pragma once
#include <list>
#include"Literal.h"
using namespace std;

class OTP 
{
public:
	string term;  //对应项
private:
	list<Literal> inner;  //本项内词
	
public:
	OTP();
	OTP(string testcase,string t);
	list<Literal> getInner();
};
