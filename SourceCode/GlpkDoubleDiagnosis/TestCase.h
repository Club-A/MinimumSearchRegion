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
	list<UTPorNFP> info; //测试用例对应UTP/NFP项信息列表

	//nanjia:因为这里没有get敏感变量集的函数，所以我把它public了，并且重新构造了它的定义，加入了hash规则，就是第二个参数
public:
	string testcase;
	unordered_set<Literal, Literal_hash> susceptibleLiterSet; //敏感变量集
	int time_interval;

public:
	TestCase();//构造函数
	TestCase(string expression,string t);//构造函数 OTP
	TestCase(string expression, string t, bool check);//构造函数 UTP/NFP

	list<UTPorNFP> getInfo(string expression, string testcase, bool check);
	list<UTP> getUTP(string expression, string testcase);
	list<NFP> getNFP(string expression, string testcase);
	list<OTP> getOTP(string expression, string testcase);

};
