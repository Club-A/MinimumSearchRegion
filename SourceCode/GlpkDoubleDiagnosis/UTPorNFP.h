#pragma once
#include <list>
#include"Literal.h"
using namespace std;

class UTPorNFP
{
	public:
		string term;        //��Ӧ��
	private:
		Literal key;		//�ؼ�����
		list<Literal> other;//��������
		list<Literal> valid;//��Ч�滻����
	public:
		UTPorNFP();
		UTPorNFP(string testcase,string model,string term,bool check);
		Literal getKey();
		list<Literal> getOther();
		list<Literal> getValid();

};

