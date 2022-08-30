#pragma once
#include <list>
#include <unordered_set>
#include"Literal.h"
#include"UTPorNFP.h"
#include <string>
#include "UTP.h"
#include "NFP.h"
#include "OTP.h"
using namespace std;
class TestCase
{
private:
	list<UTPorNFP> info; //����������ӦUTP/NFP����Ϣ�б�

	//nanjia:��Ϊ����û��get���б������ĺ����������Ұ���public�ˣ��������¹��������Ķ��壬������hash���򣬾��ǵڶ�������
public:
	string testcase;
	unordered_set<Literal, Literal_hash> susceptibleLiterSet; //���б�����
	int time_interval;

public:
	TestCase();//���캯��
	TestCase(string expression,string t);//���캯�� OTP
	TestCase(string expression, string t, bool check);//���캯�� UTP/NFP

	list<UTPorNFP> getInfo(string expression, string testcase, bool check);
	list<UTP> getUTP(string expression, string testcase);
	list<NFP> getNFP(string expression, string testcase);
	list<OTP> getOTP(string expression, string testcase);

};
