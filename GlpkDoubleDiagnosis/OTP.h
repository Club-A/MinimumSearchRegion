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
	string term;  //��Ӧ��
private:
	list<Literal> inner;  //�����ڴ�
	
public:
	OTP();
	OTP(string testcase,string t);
	list<Literal> getInner();
};
