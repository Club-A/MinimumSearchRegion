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
	string term;  //��Ӧ��
private:
	list<Literal> inner;  //�����ڴ�
	list<Literal> other;  //�����ڴ�
	list<Literal> valid;  //��Ч�����
public:
	UTP();
	UTP(string testcase, string t, string model);
	list<Literal> getInner();
	list<Literal> getOther();
	list<Literal> getValid();
};
