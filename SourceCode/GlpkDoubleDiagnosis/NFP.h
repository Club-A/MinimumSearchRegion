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
	string term;  //��Ӧ��
private:
	Literal key;  //�ؼ�����
	list<Literal> other;  //��������
	list<Literal> valid;  //��Ч�滻����
public:
	NFP();
	NFP(string testcase, string t, string model);
	Literal getKey();
	list<Literal> getOther();
	list<Literal> getValid();
};