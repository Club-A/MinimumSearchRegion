// BoolCheckFramework.cpp : 定义控制台应用程序的入口点。
//


#include "stdafx.h"

#include<iostream>
#include<sys/timeb.h>
#include <stdio.h>
#include "Expression.h"
#include "Mutants.h"
#include "DnfMutant.h"
#include "CommonFuncs.h"
#include <time.h>
#include "GLPK_Solve.h"
#include "LIFGenerateCellsForMutants.h"
#include "LOFGenerateCellsForMutants.h"
#include "LRFGenerateCellsForMutants.h"
#include "TOFGenerateCellsForMutants.h"
#include "HierarchyNode.h"
#include "Utility.h"
#include "DnfMutant.h"
#include "FaultDetection.h"
#include "public.h"
#include "DoubleLIRFHandler.h"
#include "DoubleLORFHandler.h"
#include "DoubleLRRFHandler.h"
#include "LRFHandler.h"
#include "DTLIFhandler.h"
#include "DTLIRFHandler.h"
#include "DTLRFHandler.h"
#include "DTTOFLRFhandler.h"
#include "DTDFCommonFuncs.h"
#include "TwoFaultDiagnosis.h"
#include<fstream>
#include<string>
#include<algorithm>
#include<windows.h>
#include "TestCase.h"
#include "Literal.h"
#include "UTPorNFP.h"
#include <unordered_map>

using namespace std;
//#include "TwoFaultDiagnosis.cpp"

#define DNF 1
#define CNF 0

#pragma warning(disable:4996)

//根据表达式获取变体
//参数exp，为原表达式
//参数faultType，为缺陷类型
//参数iModel，为范式类型
//生成DNF表达式


//string generateDNF(vector<string> terms){
//	vector<string>::iterator iter;
//	for (int i = 0; i < terms.size(); i++){
//		terms[i] = generateIDNForICNF(terms[i], true);
//	}
//	string oriExp;
//	sort(terms.begin(), terms.end());
//	iter = unique(terms.begin(), terms.end());
//	if (iter != terms.end()){
//		terms.erase(iter, terms.end());
//	}
//	for (int i = 0; i < terms.size(); i++){
//		if (i == 0)oriExp = terms[i];
//		else oriExp += "+" + terms[i];
//	}
//	return oriExp;
//}


//coded by ZHANGXIAOXIONG
void COLOR_PRINT2(ofstream& oFile,const char* s, int front_color, int back_color)
{
	HANDLE handle = GetStdHandle(STD_OUTPUT_HANDLE);
	SetConsoleTextAttribute(handle, BACKGROUND_INTENSITY | back_color * 16 | FOREGROUND_INTENSITY | front_color);
	oFile<<s;
	SetConsoleTextAttribute(handle, FOREGROUND_INTENSITY | 7);
}



bool checkTestSet(
	vector<string> testset/*生成的测试用例集合，如果测试用例全部通过，则表示没有问题*/,
	string exprOri/*原表达式*/,
	string expr/*变体*/,
	vector<int>& unformityTCsol, /*结果一致的节点，为无效测试点*/
	vector<int>& differTCsol, /*结果不一致的节点，为有效测试点*/
	vector<string>& unformityStr, /*结果一致的节点，为无效测试点*/
	vector<string>& differTStr, /*结果不一致的节点，为有效测试点*/
	vector<string>& unformityType,
	vector<string>& differType,
	bool vOdd)
{
	Utility uti;
	CCommonFuncs CCommonFuncs;

	string oriexp = exprOri;
	string faultexp = expr;
	for (int i = 0; i<testset.size(); i++)
	{
		string testcase = testset[i];
		bool v1 = (uti.evaluate(oriexp, testcase));
		bool v2 = (uti.evaluateMutant(oriexp, faultexp, testcase));
		bool v = v1 ^ v2;//两个值相同则为假，不同则为真,不过这两个资源好浪费时间呀，明明有更优的算法
		if (v){
			if (v1 && !v2)
			{
				differType.push_back("s");  //s 表示什么意思呢？
			}else {
				differType.push_back("e");
			}
			differTStr.push_back(testset[i]);
			differTCsol.push_back(CCommonFuncs.getTModelIndexOf01Str(testset[i], vOdd));
		}
		else
		{
			if (v1) unformityType.push_back("t");
			else unformityType.push_back("f");
			unformityStr.push_back(testset[i]);
			unformityTCsol.push_back(CCommonFuncs.getTModelIndexOf01Str(testset[i], vOdd));
		}
	}
	if (differTCsol.size() == 0)
		return true;
	else
		return false;
}

bool checkTestSetCalHd(
	vector<string> testset/*生成的测试用例集合，如果测试用例全部通过，则表示没有问题*/,
	string exprOri/*原表达式*/,
	string expr/*变体*/,
	vector<int>& unformityTCsol, /*结果一致的节点，为无效测试点*/
	vector<int>& differTCsol, /*结果不一致的节点，为有效测试点*/
	vector<string>& unformityStr, /*结果一致的节点，为无效测试点*/
	vector<string>& differTStr, /*结果不一致的节点，为有效测试点*/
	vector<string>& unformityType,
	vector<string>& differType,
	hash_set<int>& hdDistance,
	PRE_PROCESS_DATA& preData,
	bool vOdd)
{
	Utility uti;
	CCommonFuncs CCommonFuncs;

	string oriexp = exprOri;
	string faultexp = expr;
	for (int i = 0; i < testset.size(); i++)
	{
		string testcase = testset[i];
		bool v1 = (uti.evaluate(oriexp, testcase));
		bool v2 = (uti.evaluateMutant(oriexp, faultexp, testcase));
		bool v = v1 ^ v2;//两个值相同则为假，不同则为真,不过这两个资源好浪费时间呀，明明有更优的算法
		if (v){
			if (v1 && !v2)
			{
				differType.push_back("s");
				if (std::count(preData.up.begin(), preData.up.end(), testcase))
				{
					hdDistance.insert(1);
				}
				else if (std::count(preData.op.begin(), preData.op.end(), testcase))
				{
					hdDistance.insert(2);
				}
				//3代表大于2
				else{
					hdDistance.insert(3);
				}
			}
			else {
				differType.push_back("e");
				if (preData.fp1.count(testcase))
				{
					hdDistance.insert(-1);
				}
				else if (preData.fp2.count(testcase))
				{
					hdDistance.insert(-2);
				}
				//3代表大于2
				else{
					hdDistance.insert(-3);
				}
			}
			differTStr.push_back(testset[i]);
			differTCsol.push_back(CCommonFuncs.getTModelIndexOf01Str(testset[i], vOdd));
		}
		else
		{
			if (v1) unformityType.push_back("t");
			else unformityType.push_back("f");
			unformityStr.push_back(testset[i]);
			unformityTCsol.push_back(CCommonFuncs.getTModelIndexOf01Str(testset[i], vOdd));
		}
	}
	if (differTCsol.size() == 0)
		return true;
	else
		return false;
}

//剔除约束矩阵中全零的行
void removeAllZeroRows(vector<vector<int>> finalCoefficientMatrix, vector<vector<int>>& coefficientMatrix, vector<int>& deletedRows){
	for (int i = 0; i < finalCoefficientMatrix.size(); i++){
		int l = 0;
		for (int j = 0; j < finalCoefficientMatrix[i].size(); j++){
			if (finalCoefficientMatrix[i][j] == 1){
				l = 1;
			}
		}
		if (l != 0){
			coefficientMatrix.push_back(finalCoefficientMatrix[i]);
		}
		else deletedRows.push_back(i);
	}
}

bool getLIFxLIFCells2(CCommonFuncs commonFuncs, vector<string> terms, int dimensionNum, vector<vector<string>>termsDimension, vector<string> opover2, vector<int> allOPs, bool vOdd, vector<vector<CCell *>>&matrixCells, vector<string>& mutants, vector<string>& faultTypes, vector<string>& faultTerms){
	if (terms.size() <= 1)
	{
		printf("表达式只包含一项，不可能发生这种Double Fault LIFxLIF！");
		return false;
	}
	//double fault lifxlif 每次需要处理两项
	// 1.1预处理，将每个term的dimensionValuesStr存起来
	vector<string> dimensionValuesStrs; //存储每个term的dimension value string
	int termNum = terms.size();
	for (int i = 0; i<termNum; i++)//逐个term进行解析
	{
		string dimensionValuesStr = "";
		for (int j = 0; j < dimensionNum; j++)
		{
			//将各维的01表示法，转换为1,2,3,4表示法
			string line = termsDimension[i][j];
			string tempStr;
			commonFuncs.transDimensionLineFrom01totModelIndex(line, tempStr);
			dimensionValuesStr.append(tempStr);
			if (j != dimensionNum - 1)
				dimensionValuesStr.append("\n");
		}
		dimensionValuesStrs.push_back(dimensionValuesStr);
	}
	//1.2 预处理读取三项以上otp
	vector<int> all3OPs;
	for (int i = 0; i<opover2.size(); i++)
	{
		int value = commonFuncs.getTModelIndexOf01Str(opover2.at(i), vOdd);
		all3OPs.push_back(value);
	}
	sort(all3OPs.begin(), all3OPs.end());

	for (int i = 0; i<termNum - 1; i++)
	{
		for (int j = i + 1; j < termNum; j++)
		{
			DTLIFhandler DTLIFhandler;
			int ln = matrixCells.size();
			DTLIFhandler.run2(dimensionValuesStrs.at(i), dimensionValuesStrs.at(j),
				matrixCells, allOPs, all3OPs);
			for (int k = ln; k < matrixCells.size(); k++){
				mutants.push_back("");
				faultTypes.push_back("LIFxLIF");
				faultTerms.push_back(terms[i] + "+" + terms[j]);
			}
		}
	}
	return true;
}
bool getLIFxLRFCells2(CCommonFuncs commonFuncs, vector<string> terms, int dimensionNum, vector<vector<string>>termsDimension, vector<string> opover2, vector<int> allUPs, vector<int> allOPs, bool vOdd, vector<vector<CCell *>>&matrixCells, vector<string>& mutants, vector<string>& faultTypes, vector<string>& faultTerms){
	if (terms.size() <= 1)
	{
		printf("表达式只包含一项，不可能发生这种Double Fault LIFxLRF！");
		return false;
	}
	//double fault lifxlrf 每次需要处理两项
	// 1.1预处理，将每个term的dimensionValuesStr存起来
	int termNum = terms.size();
	vector<string> dimensionValuesStrs; //存储每个term的dimension value string
	for (int i = 0; i<termNum; i++)//逐个term进行解析
	{
		string dimensionValuesStr = "";
		for (int j = 0; j < dimensionNum; j++)
		{
			//将各维的01表示法，转换为1,2,3,4表示法
			string line = termsDimension[i][j];
			string tempStr;
			commonFuncs.transDimensionLineFrom01totModelIndex(line, tempStr);
			dimensionValuesStr.append(tempStr);
			if (j != dimensionNum - 1)
				dimensionValuesStr.append("\n");
		}
		dimensionValuesStrs.push_back(dimensionValuesStr);
	}
	//1.2 预处理读取三项以上otp
	vector<int> all3OPs;
	for (int i = 0; i<opover2.size(); i++)
	{
		int value = commonFuncs.getTModelIndexOf01Str(opover2.at(i), vOdd);
		all3OPs.push_back(value);
	}
	sort(all3OPs.begin(), all3OPs.end());

	//生成LRF矩阵
	/*for (int i = 0; i<termNum; i++)
	{
		CLRFHandler lrfHandler;
		lrfHandler.run(dimensionValuesStrs.at(i), vOdd, matrixCells, allOPs, allUPs);
	}*/
	//
	for (int i = 0; i < termNum; i++)
	{
		bool hasEqualLRF = false;
		for (int j = 0; j < termNum; j++)
		{
			if (i == j)
			{
				continue;
			}
			CDTLIRFHandler dtlirfHandler;
			int ln = matrixCells.size();
			dtlirfHandler.run2(dimensionValuesStrs.at(i), dimensionValuesStrs.at(j), vOdd, matrixCells, allOPs, allUPs, hasEqualLRF);
			for (int k = ln; k < matrixCells.size(); k++){
				mutants.push_back("");
				faultTypes.push_back("LIFxLRF");
				faultTerms.push_back(terms[i] + "+" + terms[j]);
			}
		}
	}
	return true;
}
bool getLRFxLRFCells2(CCommonFuncs commonFuncs, vector<string> terms, int dimensionNum, vector<vector<string>>termsDimension, vector<int> allUPs, vector<int> allOPs, bool vOdd, vector<vector<CCell *>>&matrixCells, vector<string>& mutants, vector<string>& faultTypes, vector<string>& faultTerms){
	int termNum = terms.size();
	if (termNum <= 1)
	{
		printf("表达式只包含一项，不可能发生这种Double Fault LRFxLRF！");
		return false;
	}
	//double fault lrfxlrf 每次需要处理两项
	// 1.1预处理，将每个term的dimensionValuesStr存起来
	vector<string> dimensionValuesStrs; //存储每个term的dimension value string
	for (int i = 0; i<termNum; i++)//逐个term进行解析
	{
		string dimensionValuesStr = "";
		bool isSingleLitral = false;
		//格子占全的维度数目
		int fullDemNum = 0;
		//格子占半的维度数目
		int halfDemNum = 0;
		for (int j = 0; j < dimensionNum; j++)
		{
			//将各维的01表示法，转换为1,2,3,4表示法
			string line = termsDimension[i][j];
			string tempStr;
			commonFuncs.transDimensionLineFrom01totModelIndex(line, tempStr);
			dimensionValuesStr.append(tempStr);
			int strLength = tempStr.size();
			if (vOdd != true || j != dimensionNum - 1) //格子占全时候字符串长度应为7,占半长度为3
			{
				if (strLength == 7)
					fullDemNum++;
				else if (strLength == 3)
					halfDemNum++;
			}
			else //格子占全时候字符串长度应为3,占半长度为1
			{
				if (strLength == 3)
					fullDemNum++;
				else if (strLength == 1)
					halfDemNum++;
			}
			if (j != dimensionNum - 1)
				dimensionValuesStr.append("\n");
		}
		if (fullDemNum == dimensionNum - 1 && halfDemNum == 1)
			isSingleLitral = true;
		dimensionValuesStrs.push_back(dimensionValuesStr);
		//生成DLRF矩阵
		CDLRFHandler dlrfHandler;
		int ln = matrixCells.size();
		dlrfHandler.run2(dimensionValuesStr, vOdd, matrixCells, allOPs, allUPs);
		for (int k = ln; k < matrixCells.size(); k++){
			mutants.push_back("");
			faultTypes.push_back("LRFxLRF");
			faultTerms.push_back(terms[i]);
		}
	}
	return true;
}
string getLiterInTerm(string term, int literNo){
	for (int i = 0, j = 0; i < term.size() && j <= literNo; i++){
		if (term[i] == '!'){
			continue;
		}
		else{
			if (j == literNo){
				if (i > 0 && term[i - 1] == '!'){
					return string("!") + term[i];
				}
				else{
					return string({ term[i] });
				}
			}
			else j++;
		}
	}
	return "";
}
bool checkExistLiterInTerm(string term, string Liter){
	const char * ms = term.c_str();
	char ldt;
	if (Liter[0] == '!')ldt = Liter[1];
	else ldt = Liter[0];
	for (int i = 0; i < term.size(); i++){
		if (ms[i] == ldt){
			return true;
		}
	}
	return false;
}
int getDimensionNo(string model, string Liter, int &literalNo){
	const char * ms = model.c_str();
	int dimensionNo = -1;
	char ldt;
	if (Liter[0] == '!')ldt = Liter[1];
	else ldt = Liter[0];
	for (int i = 0; i < model.size(); i++){
		if (ms[i] == ldt){
			literalNo = i;
			dimensionNo = i / 2;
			break;
		}
	}
	return dimensionNo;
}
int getTermNo(vector<string> terms, string Term){
	int dimensionNo = -1;
	for (int i = 0; i < terms.size(); i++){
		if (terms[i] == Term){
			dimensionNo = i;
			break;
		}
	}
	return dimensionNo;
}
vector<int> checkShrinkTerms(string oriExp, vector<string>terms, vector<string>testcase){
	vector<int> faultTermsNo;
	Utility uti;
	for (int i = 0; i < terms.size(); i++){
		for (int j = 0; j < testcase.size(); j++){
			bool v1 = uti.evaluate(oriExp, testcase[j]);
			if (!v1)continue;//不是收缩点，舍去
			bool v2 = uti.evaluateMutant(oriExp, terms[i], testcase[j]);
			bool v = v1 ^ v2;//异或，值同为假，值不同为真
			if (v == false){//如果为假，说明这个项发生了缺陷
				faultTermsNo.push_back(i);
				break;
			}
		}
	}
	return faultTermsNo;
}
vector<vector<vector<string>>> changeDimensionToVector(vector<vector<string>> termsDimension){
	vector<vector<vector<string>>> a;
	CCommonFuncs commonFunc;
	for (int x = 0; x < termsDimension.size(); x++){
		vector<vector<string>> b;
		for (int y = 0; y < termsDimension[x].size(); y++){
			vector<string> c;
			c = commonFunc.split(termsDimension[x][y], " ");
			b.push_back(c);
			c.clear();
		}
		a.push_back(b);
		b.clear();
	}
	return a;
}
vector<vector<string>> getLOFChangeCells(string expression, string model, int termNo, string Liter1){
	//分解表达式
	PRE_PROCESS_DATA preData;

	//转换为IDNF
	Utility uti;
	vector<string> terms = uti.extractDNFstyle(expression);
	vector<string>::iterator iter;
	string newExp = "";
	//得到IDNF
	newExp = expression;// generateIDNForICNF(expression, true);
	//printf("newExp:%s\n", newExp.c_str());

	getPreProcessData(newExp, preData, DNF);
	CCommonFuncs commonFuncs;
	int literalNum = model.size();
	int dimensionNum = (literalNum + 1) / 2;
	bool vOdd = (literalNum % 2 == 0) ? false : true;
	//printf("%d,%d,%d\n", __LINE__, vOdd, literalNum);

	//获取变化词的维度编号
	int literalNo1;
	int dimensionNo1 = getDimensionNo(model, Liter1, literalNo1);

	//读取所有的OTP
	vector<string> op = preData.op;
	vector<int> allOPs;
	for (int i = 0; i<op.size(); i++)
	{
		int value = commonFuncs.getTModelIndexOf01Str(op.at(i), vOdd);
		allOPs.push_back(value);
	}
	sort(allOPs.begin(), allOPs.end());

	//读取所有的UTP
	vector<int> allUPs;
	vector<string> up = preData.up;
	for (int i = 0; i<up.size(); i++)
	{
		int value = commonFuncs.getTModelIndexOf01Str(up.at(i), vOdd);
		allUPs.push_back(value);
	}
	sort(allUPs.begin(), allUPs.end());

	//获取是否有单词的判断
	bool isSingleLitral = false;

	/*printf("preData.termsDimension:\n");
	for (int x = 0; x < preData.termsDimension.size(); x++){
		for (int y = 0; y < preData.termsDimension[x].size(); y++){
			printf("%s\n", preData.termsDimension[x][y].c_str());
		}
		printf("\n\n");
	}
	printf("\n");*/

	vector<vector<vector<string>>>termsDimensions = changeDimensionToVector(preData.termsDimension);
	vector<vector<string>> termsDimension = termsDimensions[termNo];
	string mutantTerm = terms[termNo];
	if (!checkExistLiterInTerm(mutantTerm, Liter1)){
		printf("cannot occur lof!\n");
		vector<vector<string>> a;
		return a;
	}
	if (mutantTerm.size() == 1 || mutantTerm.size() == 2 && mutantTerm[0] == '!'){
		printf("just can occur tof");
		vector<vector<string>> a;
		return a;
	}
	string mutantChangeTerm;
	//一个term对应的维度值，有几个维度，dimensionNum就会有多大
	vector<string> testcase;
	CCommonFuncs commonFunc;
	mutantChangeTerm = mutantTerm;
	string lm = Liter1;
	int lz = mutantChangeTerm.find(lm);
	string::iterator itr = mutantChangeTerm.begin() + lz;
	mutantChangeTerm.erase(itr);
	//printf("LOF:mutantChangeTerm[%s]\n", mutantChangeTerm.c_str());

	vector<string> addValues;
	//首先判断是哪个维度，其次判断是
	/*printf("termsDimension[%d]:\n", dimensionNo1);
	for (int x = 0; x < termsDimension[dimensionNo1].size(); x++){
		printf("[%s]", termsDimension[dimensionNo1][x].c_str());
	}
	printf("\n");*/
	//没有对非做处理
	if (literalNo1 == dimensionNo1 * 2){//如果是4个数，则不变，否则增加该词的01值
		//新增，如果是1，则增加为2，如果是2，则增加为4
		if (termsDimension[dimensionNo1].size() < 4){
			int lsize = termsDimension[dimensionNo1].size();
			for (int x = 0; x < lsize; x++){
				string m;
				if (termsDimension[dimensionNo1][x][0] == '1'){
					m = '0';
				}
				else{
					m = '1';
				}
				m = m + termsDimension[dimensionNo1][x][1];
				termsDimension[dimensionNo1].push_back(m);
				addValues.push_back(m);
			}
		}
	}
	else{
		if (termsDimension[dimensionNo1].size() < 4){
			int lsize = termsDimension[dimensionNo1].size();
			for (int x = 0; x < lsize; x++){
				string m;
				if (termsDimension[dimensionNo1][x][1] == '1'){
					m = '0';
				}
				else{
					m = '1';
				}
				m = termsDimension[dimensionNo1][x][0] + m;
				termsDimension[dimensionNo1].push_back(m);
				addValues.push_back(m);
			}
		}
	}
	//变化后的格覆盖
	/*printf("\nchanged:\n");
	for (int x = 0; x < dimensionNum; x++){
		for (int y = 0; y < termsDimension[x].size(); y++){
			printf("[%s]", termsDimension[x][y].c_str());
		}
		printf("\n");
	}*/
	//变化的格覆盖点
	//printf("\nchanging:\n");
	for (int x = 0; x < dimensionNum; x++){
		if (x == dimensionNo1){
			termsDimension[x].clear();
			termsDimension[x] = addValues;
		}
		/*for (int y = 0; y < termsDimension[x].size(); y++){
			printf("[%s]", termsDimension[x][y].c_str());
		}
		printf("\n");*/
	}
	return termsDimension;
}
bool checkLOFOccur(vector<vector<string>>LOFChangeCells, string model, string testCase){
	for (int i = 0; i < LOFChangeCells.size(); i++){
		string subTestCase = testCase.substr(i * 2, 2);
		//printf("[%s]", subTestCase.c_str());
		bool m = false;
		for (int j = 0; j < LOFChangeCells[i].size(); j++){
			if (subTestCase == LOFChangeCells[i][j]){
				m = true;
				break;
			}
		}
		if (m == false)return false;
	}
	//printf("\n");
	return true;
}

//
//bool checkTestDiffer(string test, string exprOri/*原表达式*/, string expr/*变体*/){
//	Utility uti;
//	bool v = (uti.evaluate(exprOri, test)) ^ (uti.evaluateMutant(exprOri, expr, test));//异或：不同为真，相同为假
//	return v;
//}

//诊断——用最优测试用例+层次关系
//生成变体的同时进行判断，而不是把变体全部生成后判断
/*
bool diagnosisByHierarchy(
	string oriExp,//原表达式
	string faultExp,//待测表达式
	vector<vector<string>> optiUniformitySet,//最优无效点
	vector<vector<string>> optiDifferSet,//最优有效点
	vector<HierarchyNode> hierarchyEntrySet,
	hash_map<string, HierarchyNode>& hierarchyMap
)
{
	for (int i = 0; i < hierarchyEntrySet.size; i++){
		//对每个连通图进行遍历
		HierarchyNode root = hierarchyEntrySet.at(i);
		string faultType = root.getValue;
		string result = diagnosisGivenFaultType(oriExp, faultExp, "liflif", optiUniformitySet, optiDifferSet, root, hierarchyMap);
	}

}
*/
Mutants diagnosisOneFault(string oriExp,
string faultExp,
string type,
vector<int> terms){
	DnfMutant dnfMutant;
	if (type == ""){
		// cout << "type is rong!" << endl;
		return "no";
	}
	if (type == "LIF"){
		Mutants result = dnfMutant.diagLIF(oriExp, faultExp, terms);
		return result;
	}
	if (type == "LRF"){
		Mutants result = dnfMutant.diagLRF(oriExp, faultExp, terms);
		return result;
	}
	if (type == "LOF"){
		Mutants result = dnfMutant.diagLOF(oriExp, faultExp, terms);
		return result;
	}
	if (type == "LNF"){
		Mutants result = dnfMutant.diagLNF(oriExp, faultExp, terms);
		return result;
	}
	if (type == "TOF"){
		Utility uti;
		vector<int> terms = uti.countDNFstyle(oriExp);
		Mutants result = dnfMutant.diagTOF(oriExp, faultExp, terms);
		return result;
	}
	if (type == "TNF"){
		Mutants result = dnfMutant.diagTNF(oriExp, faultExp, terms);
		return result;
	}
	if (type == "CORF"){
		Mutants result = dnfMutant.diagCORF(oriExp, faultExp, terms);
		return result;
	}
	if (type == "DORF"){
		Mutants result = dnfMutant.diagDORF(oriExp, faultExp, terms);
		return result;
	}
	if (type == "ENF"){
		Mutants result = dnfMutant.diagENF(oriExp, faultExp, terms);
		return result;
	}
}

Mutants diagnosisGivenFaultType(
	string oriExp,//原表达式
	string faultExp,//待测表达式
	string faultType, //缺陷类型
	vector<vector<string>> optiUniformitySet,//最优无效点
	vector<vector<string>> optiDifferSet,//最优有效点
	hash_set<string>& outMutant,//排除的变体格式
	HierarchyNode root,//当前缺陷对应节点
	hash_map<string, HierarchyNode> hierarchyMap,
	vector<int> terms,//可能发生缺陷的项
	PRE_PROCESS_DATA& preData
	){
	DnfMutant dnfMutant;
	if (root.getValue().empty()){
		return "no";
	}
	//// cout << "value====" << root.getValue() << endl;
	Mutants finalresult(oriExp);
	if (root.getValue().compare("liflif") == 0){
		Mutants result = dnfMutant.diagnosisSingleTermLIFxLIFdoublemutants(oriExp, faultExp, outMutant, optiUniformitySet, optiDifferSet, hierarchyMap, terms, preData);
		return result;
	}
	if (root.getValue().compare("liflrf") == 0){
		Mutants result = dnfMutant.diagnosisSingleTermLIFxLRFdoublemutants(oriExp, faultExp, outMutant, optiUniformitySet, optiDifferSet, hierarchyMap, terms, preData);
		return result;
	}
	if (root.getValue().compare("liflnf") == 0){
		Mutants result = dnfMutant.diagnosisSingleTermLIFxLNFdoublemutants(oriExp, faultExp, outMutant, optiUniformitySet, optiDifferSet, hierarchyMap, terms, preData);
		return result;
	}
	if (root.getValue().compare("lifcorf") == 0){
		Mutants result = dnfMutant.diagnosisSingleTermLIFxCORFdoublemutants(oriExp, faultExp, outMutant, optiUniformitySet, optiDifferSet, hierarchyMap, terms, preData);
		return result;
	}
	if (root.getValue().compare("liftnf") == 0){
		Mutants result = dnfMutant.diagnosisSingleTermLIFxTNFdoublemutants(oriExp, faultExp, outMutant, optiUniformitySet, optiDifferSet, hierarchyMap, terms, preData);
		return result;
	}



	if (root.getValue().compare("lrftnf") == 0){
		Mutants result = dnfMutant.diagnosisSingleTermLRFxTNFdoublemutants(oriExp, faultExp, outMutant, optiUniformitySet, optiDifferSet, hierarchyMap, terms, preData);
		return result;
	}

	if (root.getValue().compare("lrfcorf") == 0){
		Mutants result = dnfMutant.diagnosisSingleTermLRFxCORFdoublemutants(oriExp, faultExp, outMutant, optiUniformitySet, optiDifferSet, hierarchyMap, terms, preData);
		return result;
	}

	if (root.getValue().compare("lrflof") == 0){
		Mutants result = dnfMutant.diagnosisSingleTermLRFxLOFdoublemutants(oriExp, faultExp, outMutant, optiUniformitySet, optiDifferSet, hierarchyMap, terms, preData);
		return result;
	}

	if (root.getValue().compare("lrflnf") == 0){
		Mutants result = dnfMutant.diagnosisSingleTermLRFxLNFdoublemutants(oriExp, faultExp, outMutant, optiUniformitySet, optiDifferSet, hierarchyMap, terms, preData);
		return result;
	}

	if (root.getValue().compare("lrfcorf") == 0){
		Mutants result = dnfMutant.diagnosisSingleTermLRFxCORFdoublemutants(oriExp, faultExp, outMutant, optiUniformitySet, optiDifferSet, hierarchyMap, terms, preData);
		return result;
	}


	if (root.getValue().compare("loflof") == 0){
		Mutants result = dnfMutant.diagnosisSingleTermLOFxLOFdoublemutants(oriExp, faultExp, outMutant, optiUniformitySet, optiDifferSet, hierarchyMap, terms, preData);
		return result;
	}
	if (root.getValue().compare("lofcorf") == 0){
		Mutants result = dnfMutant.diagnosisSingleTermLOFxCORFdoublemutants(oriExp, faultExp, outMutant, optiUniformitySet, optiDifferSet, hierarchyMap, terms, preData);
		return result;
	}
	if (root.getValue().compare("loflnf") == 0){
		Mutants result = dnfMutant.diagnosisSingleTermLOFxLNFdoublemutants(oriExp, faultExp, outMutant, optiUniformitySet, optiDifferSet, hierarchyMap, terms, preData);
		return result;
	}
	if (root.getValue().compare("loftnf") == 0){
		Mutants result = dnfMutant.diagnosisSingleTermLOFxTNFdoublemutants(oriExp, faultExp, outMutant, optiUniformitySet, optiDifferSet, hierarchyMap, terms, preData);
		return result;
	}

	if (root.getValue().compare("dorflif") == 0){
		Mutants result = dnfMutant.diagnosisSingleTermDORFxLIFdoublemutants(oriExp, faultExp, outMutant, optiUniformitySet, optiDifferSet, hierarchyMap, terms, preData);
		return result;
	}
	if (root.getValue().compare("dorflof") == 0){
		Mutants result = dnfMutant.diagnosisSingleTermDORFxLOFdoublemutants(oriExp, faultExp, outMutant, optiUniformitySet, optiDifferSet, hierarchyMap, terms, preData);
		return result;
	}
	if (root.getValue().compare("dorflnf") == 0){
		Mutants result = dnfMutant.diagnosisSingleTermDORFxLNFdoublemutants(oriExp, faultExp, outMutant, optiUniformitySet, optiDifferSet, hierarchyMap, terms, preData);
		return result;
	}

	if (root.getValue().compare("lrfdorf") == 0){
		Mutants result = dnfMutant.diagnosisSingleTermLRFxDORFdoublemutants(oriExp, faultExp, outMutant, optiUniformitySet, optiDifferSet, hierarchyMap, terms, preData);
		return result;
	}
	if (root.getValue().compare("dorfdorf") == 0){
		Mutants result = dnfMutant.diagnosisSingleTermDORFxDORFdoublemutants(oriExp, faultExp, outMutant, optiUniformitySet, optiDifferSet, hierarchyMap, terms, preData);
		return result;
	}
	if (root.getValue().compare("dorftnf") == 0){
		Mutants result = dnfMutant.diagnosisSingleTermDORFxTNFdoublemutants(oriExp, faultExp, outMutant, optiUniformitySet, optiDifferSet, hierarchyMap, terms, preData);
		return result;
	}
	if (root.getValue().compare("tnfdorf") == 0){
		Mutants result = dnfMutant.diagnosisSingleTermTNFxDORFdoublemutants(oriExp, faultExp, outMutant, optiUniformitySet, optiDifferSet, hierarchyMap, terms, preData);
		return result;
	}
	if (root.getValue().compare("corflif") == 0){
		Mutants result = dnfMutant.diagnosisSingleTermCORFxLIFdoublemutants(oriExp, faultExp, outMutant, optiUniformitySet, optiDifferSet, hierarchyMap, terms, preData);
		return result;
	}
	
	if (root.getValue().compare("corflrf") == 0){
		Mutants result = dnfMutant.diagnosisSingleTermCORFxLRFdoublemutants(oriExp, faultExp, outMutant, optiUniformitySet, optiDifferSet, hierarchyMap, terms, preData);
		return result;
	}
	if (root.getValue().compare("corftof") == 0){
		Mutants result = dnfMutant.diagnosisSingleTermCORFxTOFdoublemutants(oriExp, faultExp, outMutant, optiUniformitySet, optiDifferSet, hierarchyMap, terms, preData);
		return result;
	}
	if (root.getValue().compare("corfdorf") == 0){
		Mutants result = dnfMutant.diagnosisSingleTermCORFxDORFdoublemutants(oriExp, faultExp, outMutant, optiUniformitySet, optiDifferSet, hierarchyMap, terms, preData);
		return result;
	}
	if (root.getValue().compare("corftnf") == 0){
		Mutants result = dnfMutant.diagnosisSingleTermCORFxTNFdoublemutants(oriExp, faultExp, outMutant, optiUniformitySet, optiDifferSet, hierarchyMap, terms, preData);
		return result;
	}

	if (root.getValue().compare("tnflif") == 0){
		Mutants result = dnfMutant.diagnosisSingleTermTNFxLIFdoublemutants(oriExp, faultExp, outMutant, optiUniformitySet, optiDifferSet, hierarchyMap, terms, preData);
		return result;
	}

	if (root.getValue().compare("tnflrf") == 0){
		Mutants result = dnfMutant.diagnosisSingleTermTNFxLRFdoublemutants(oriExp, faultExp, outMutant, optiUniformitySet, optiDifferSet, hierarchyMap, terms, preData);
		return result;
	}

	if (root.getValue().compare("tnflnf") == 0){
		Mutants result = dnfMutant.diagnosisSingleTermTNFxLNFdoublemutants(oriExp, faultExp, outMutant, optiUniformitySet, optiDifferSet, hierarchyMap, terms, preData);
		return result;
	}

	if (root.getValue().compare("tnfcorf") == 0){
		Mutants result = dnfMutant.diagnosisSingleTermTNFxCORFdoublemutants(oriExp, faultExp, outMutant, optiUniformitySet, optiDifferSet, hierarchyMap, terms, preData);
		return result;
	}

	if (root.getValue().compare("lnflnf") == 0){
		Mutants result = dnfMutant.diagnosisSingleTermLNFxLNFdoublemutants(oriExp, faultExp, outMutant, optiUniformitySet, optiDifferSet, hierarchyMap, terms, preData);
		return result;
	}

	if (root.getValue().compare("enflif") == 0){
		Mutants result = dnfMutant.diagnosisSingleTermENFxLIFdoublemutants(oriExp, faultExp, outMutant, optiUniformitySet, optiDifferSet, hierarchyMap, terms, preData);
		return result;
	}
	if (root.getValue().compare("enflrf") == 0){
		Mutants result = dnfMutant.diagnosisSingleTermENFxLRFdoublemutants(oriExp, faultExp, outMutant, optiUniformitySet, optiDifferSet, hierarchyMap, terms, preData);
		return result;
	}

	if (root.getValue().compare("enflof") == 0){
		Mutants result = dnfMutant.diagnosisSingleTermENFxLOFdoublemutants(oriExp, faultExp, outMutant, optiUniformitySet, optiDifferSet, hierarchyMap, terms, preData);
		return result;
	}

	if (root.getValue().compare("enflnf") == 0){
		Mutants result = dnfMutant.diagnosisSingleTermENFxLNFdoublemutants(oriExp, faultExp, outMutant, optiUniformitySet, optiDifferSet, hierarchyMap, terms, preData);
		return result;
	}

	if (root.getValue().compare("enftof") == 0){
		Mutants result = dnfMutant.diagnosisSingleTermENFxTOFdoublemutants(oriExp, faultExp, outMutant, optiUniformitySet, optiDifferSet, hierarchyMap, terms, preData);
		return result;
	}

	if (root.getValue().compare("enftnf") == 0){
		Mutants result = dnfMutant.diagnosisSingleTermENFxTNFdoublemutants(oriExp, faultExp, outMutant, optiUniformitySet, optiDifferSet, hierarchyMap, terms, preData);
		return result;
	}

	if (root.getValue().compare("enfcorf") == 0){
		Mutants result = dnfMutant.diagnosisSingleTermENFxCORFdoublemutants(oriExp, faultExp, outMutant, optiUniformitySet, optiDifferSet, hierarchyMap, terms, preData);
		return result;
	}

	if (root.getValue().compare("enfdorf") == 0){
		Mutants result = dnfMutant.diagnosisSingleTermENFxDORFdoublemutants(oriExp, faultExp, outMutant, optiUniformitySet, optiDifferSet, hierarchyMap, terms, preData);
		return result;
	}

	if (root.getValue().compare("enfenf") == 0){
		Utility uti;
		vector<int> terms = uti.countDNFstyle(oriExp);
		Mutants result = dnfMutant.diagnosisSingleTermENFxENFdoublemutants(oriExp, faultExp, outMutant, optiUniformitySet, optiDifferSet, hierarchyMap, terms, preData);
		return result;
	}

	if (root.getValue().compare("lrflrf") == 0){
		Utility uti;
		vector<int> terms = uti.countDNFstyle(oriExp);
		Mutants result = dnfMutant.diagnosisSingleTermLRFxLRFdoublemutants(oriExp, faultExp, outMutant, optiUniformitySet, optiDifferSet, hierarchyMap, terms, preData);
		return result;
	}
}


////诊断——用最优测试用例
//bool diagnosis(
//	string oriExp, //原表达式
//	string faultExp, //待测表达式
//	string faultType, //缺陷类型
//	vector<string> oriTerms,
//	vector<string> faultTerms,
//	string model,
//	vector<vector<string>> optiUniformitySet,//最优无效点
//	vector<vector<string>> optiDifferSet//最优有效点
//	)
//{
//	DnfMutant dnfMutant;
//	//printf("\nfaultType is [%s]\n", faultType.c_str());
//	Mutants z = dnfMutant.getMutants(oriExp, faultType, oriTerms, faultTerms, model);
//	if (z.mutants.size() == 0 || z.mutants[0].mutant == ""){
//		printf("\nI cannot diagnose the type of faults.\n");
//		return false;
//	}
//	//printf("\nThe number of mutants is [%d]\n", z.mutants.size());
//	/*
//	for (int i = 0; i < z.mutants.size(); i++){
//		printf("%s\n", z.mutants[i].mutant.c_str());
//	}
//	*/
//	
//
//	Mutants realMutants(oriExp);
//	for (int j = 0; j < z.mutants.size(); j++){
//		string mutant = z.mutants[j].mutant;
//		bool flagUniform = true;//无效点是否全部通过
//		//（1）先用无效点进行探测
//		for (int i = 0; i < optiUniformitySet.size(); i++){
//			//printf("point: %s, faultExp: %s, mutant: %s\n", optiUniformitySet[i][0].c_str(), faultExp.c_str(), mutant.c_str());
//			if (checkTestDiffer(optiUniformitySet[i][0], faultExp, mutant)){
//				flagUniform = false;
//				break;
//			}
//		}
//
//		if (flagUniform == true){//无效点全部通过
//			bool flagDiffer = true;
//			for (int i = 0; i < optiDifferSet.size(); i++){
//				if (checkTestDiffer(optiDifferSet[i][0], faultExp, mutant)){
//					flagDiffer = false;
//					break;
//				}
//			}
//			if (flagDiffer == true){//有效点全部通过
//				realMutants.addMutant(mutant, z.mutants[j].type, z.mutants[j].oriterms, z.mutants[j].faultterms);
//			}
//		}
//	}
//	if (realMutants.mutants.size() == 0){
//		return false;
//	}
//	else{
//		//printf("mutant is [%s]   faultExp is [%s]", realMutants.mutants[0].mutant.c_str(), faultType.c_str());
//		if (checkEqualment(faultExp, realMutants.mutants[0].mutant)){
//			printf("realFaultExpress is[%s][%s][more terms]", realMutants.mutants[0].mutant.c_str(), faultType.c_str());
//			return true;
//		}
//		return true;
//	}
//}




void MutantsandValidInvalidTestCase(string type, string expression){
	Mutants mutants1(expression);
	DnfMutant dnf;
	int length;
	if (type == "single")
		mutants1 = dnf.getMutants(expression, "SALL");
	else
		mutants1 = dnf.getMutants(expression);
	length = mutants1.mutants.size();
	//------------------------------------------------------
	Utility uti;
	//分解表达式   保存表达式的utp otp fp 等等
	PRE_PROCESS_DATA preData; MutantsandValidInvalidTestCase;
	vector<string> terms = uti.extractDNFstyle(expression); //分解表达式的项
	string model = uti.generateModel(expression);  //model就是原表达式所包含的所有的字母(排好序的），例如 abce+cd+ef  abcdef
	string newExp = "";
	//得到IDNF
	//newExp = generateIDNForICNF(expression, true);
	//newExp = generateDNF(terms);
	newExp = expression;
	getPreProcessData(newExp, preData, true);
	vector<string> allPoint;
	CCommonFuncs commonFuncs;
	int literalNum = preData.literalCount;
	int dimensionNum = (literalNum + 1) / 2;
	bool vOdd = (literalNum % 2 == 0) ? false : true;
	//读取所有的OTP
	vector<string> op = preData.op;
	vector<int> allOPs;
	for (int i = 0; i < op.size(); i++)
	{
		int value = commonFuncs.getTModelIndexOf01Str(op.at(i), vOdd);
		allOPs.push_back(value);
	}

	sort(allOPs.begin(), allOPs.end());

	//读取所有的UTP·
	vector<int> allUPs;
	vector<string> up = preData.up;
	for (int i = 0; i < up.size(); i++)
	{
		int value = commonFuncs.getTModelIndexOf01Str(up.at(i), vOdd);
		allUPs.push_back(value);
	}
	sort(allUPs.begin(), allUPs.end());
	/**8888888888888888888888888888**/
	/*termsDimension 表示的是每一项的能使表达式值取1的取值，例如 ab+cd  model=“abcd” 没2个字母是一维
	11(第一项的 第一维的取值）
	00 01 11 10（第一项的其他维的取值）

	00 01 11 10（第二项的第一维的取值）
	11（第二项的其他维的取值）
	*/
	/*
		nanjia：这是一个或许可使用的数据结构，帮助我们构造敏感点、可替换项信息
	*/
	printf("\npreData.termsDimension:\n");
	for (int i = 0; i < preData.termsDimension.size(); i++){
		for (int j = 0; j < preData.termsDimension[i].size(); j++){
			printf("%s\n", preData.termsDimension[i][j].c_str());
		}
		printf("\n");
	}
	//层次关系map
	hash_map<string, HierarchyNode> hierarchyMap;

	vector<HierarchyNode> hierarchyEntry;

	//初始化层次关系
	uti.initHierarchyRelation(&hierarchyMap, &hierarchyEntry);
	//实现对层次关系进行剪枝的函数，需要Ttrue和Tfalse检测结果
	/**8888888888888888888888888888**/

	//获取是否有单词的判断
	bool isSingleLitral = false;
	//格子占全的维度数目
	int fullDemNum = 0;
	//格子占半的维度数目
	int halfDemNum = 0;
	vector<vector<string>> termsDimension = preData.termsDimension;

	vector<vector<vector<int>>> mutantsDimensionValue;
	vector<vector<CCell*>> corresponding_tests;
	vector<int> tModelIndexAppeared;
	vector<vector<CCell*>> matrixCells;
	vector<vector<int>> coefficientMatrix;
	vector<string> mutants;
	vector<vector<CCell *>> TNFCCells;
	vector<string> faultTypes;
	vector<string> faultTerms;

	int startIndex = 0;
	if (type == "single"){
		for (int i = 0; i < preData.termsDimension.size(); i++){//逐个term进行解析
			/*
			dimensionValuesStr保存第i项的每一维的转换值，eg
			3
			1 2 3 4
			第一行表示第i项第一维的值，第二行表示第二维的值。
			*/
			string dimensionValuesStr;
			//将一行转换为多维变换，例如11 10 00 01 11 10 ，就是3 4 1 2 3 4，即34 1234
			for (int j = 0; j < dimensionNum; j++)
			{
				//将各维的01表示法，转换为1,2,3,4表示法
				string line = termsDimension[i][j];
				string tempStr;
				commonFuncs.transDimensionLineFrom01totModelIndex(line, tempStr);
				//// cout <<"tempstr: "<< tempStr << endl;
				dimensionValuesStr.append(tempStr);
				int strLength = tempStr.length();
				if (vOdd != true || j != dimensionNum - 1) //格子占全时候字符串长度应为7,占半长度为3
				{
					if (strLength == 7)
						fullDemNum++;
					else if (strLength == 3)
						halfDemNum++;
				}
				else //格子占全时候字符串长度应为3,占半长度为1
				{
					if (strLength == 3)
						fullDemNum++;
					else if (strLength == 1)
						halfDemNum++;
				}
				if (j != dimensionNum - 1)
					dimensionValuesStr.append("\n");
			}
			//// cout << "dimensionValuesStr: " << dimensionValuesStr << endl;
			if (fullDemNum == dimensionNum - 1 && halfDemNum == 1)
				isSingleLitral = true;
			//根据缺陷信息转换成需要的格覆盖,在此处要进行改造
			mutantsDimensionValue.clear();
			corresponding_tests.clear();
			//整个思路
			//首先根据本体得到所有变体的待测点，所谓待测点，就是变体与本体不同的真值点，即包括本体真值点，也包括变体真值点
			//然后根据待测点组方程组，一个变体一个方程组，方程组变量的个数为所有待测点
			//然后根据每个变体的待测点确定每个方程的变量个数
			//结方程组得到最优的变量个数即可，其中不涉及到表达式的真假值，至于整个处理的逻辑原理请参考表达式

			int matrix_size1 = matrixCells.size();
			LIFGenerateCellsForMutants(commonFuncs,
				dimensionValuesStr,
				vOdd,
				matrixCells,
				mutantsDimensionValue,
				corresponding_tests, allOPs);
			for (int x = startIndex; x < matrixCells.size(); x++){
				faultTypes.push_back("LIF");
				faultTerms.push_back(terms[i]);
			}
			startIndex = matrixCells.size();
			int matrix_size2 = matrixCells.size();
			if (isSingleLitral) //如果只有一个词 则退化成TOF
			{
				TOFGenerateCellsForMutants(commonFuncs, dimensionValuesStr, vOdd, matrixCells,
					mutantsDimensionValue,
					corresponding_tests, allOPs);

				for (int x = startIndex; x < matrixCells.size(); x++){
					faultTypes.push_back("TOF");
					faultTerms.push_back(terms[i]);
				}
				startIndex = matrixCells.size();
			}

			LRFGenerateCellsForMutants(commonFuncs, dimensionValuesStr, vOdd, matrixCells,
				mutantsDimensionValue,
				corresponding_tests, allOPs, allUPs);

			for (int x = startIndex; x < matrixCells.size(); x++){
				faultTypes.push_back("LRF");
				faultTerms.push_back(terms[i]);
			}
			startIndex = matrixCells.size();

			LOFGenerateCellsForMutants(commonFuncs, dimensionValuesStr, vOdd, matrixCells,
				mutantsDimensionValue,
				corresponding_tests, allOPs, allUPs);

			for (int x = startIndex; x < matrixCells.size(); x++){
				faultTypes.push_back("LOF");
				faultTerms.push_back(terms[i]);
			}
			startIndex = matrixCells.size();

			get_mutant_constrains(mutantsDimensionValue, vOdd, corresponding_tests, newExp, i, mutants);

		}
	}
	else{
		for (int i = 0; i < preData.termsDimension.size(); i++){//逐个term进行解析
			/*
			dimensionValuesStr保存第i项的每一维的转换值，eg
			3
			1 2 3 4
			第一行表示第i项第一维的值，第二行表示第二维的值。
			*/
			string dimensionValuesStr;
			//将一行转换为多维变换，例如11 10 00 01 11 10 ，就是3 4 1 2 3 4，即34 1234
			for (int j = 0; j < dimensionNum; j++)
			{
				//将各维的01表示法，转换为1,2,3,4表示法
				string line = termsDimension[i][j];
				string tempStr;
				commonFuncs.transDimensionLineFrom01totModelIndex(line, tempStr);
				//// cout <<"tempstr: "<< tempStr << endl;
				dimensionValuesStr.append(tempStr);
				int strLength = tempStr.length();
				if (vOdd != true || j != dimensionNum - 1) //格子占全时候字符串长度应为7,占半长度为3
				{
					if (strLength == 7)
						fullDemNum++;
					else if (strLength == 3)
						halfDemNum++;
				}
				else //格子占全时候字符串长度应为3,占半长度为1
				{
					if (strLength == 3)
						fullDemNum++;
					else if (strLength == 1)
						halfDemNum++;
				}
				if (j != dimensionNum - 1)
					dimensionValuesStr.append("\n");
			}
			//// cout << "dimensionValuesStr: " << dimensionValuesStr << endl;
			if (fullDemNum == dimensionNum - 1 && halfDemNum == 1)
				isSingleLitral = true;
			//根据缺陷信息转换成需要的格覆盖,在此处要进行改造
			mutantsDimensionValue.clear();
			corresponding_tests.clear();
			//整个思路
			//首先根据本体得到所有变体的待测点，所谓待测点，就是变体与本体不同的真值点，即包括本体真值点，也包括变体真值点
			//然后根据待测点组方程组，一个变体一个方程组，方程组变量的个数为所有待测点
			//然后根据每个变体的待测点确定每个方程的变量个数
			//结方程组得到最优的变量个数即可，其中不涉及到表达式的真假值，至于整个处理的逻辑原理请参考表达式
			int matrix_size1 = matrixCells.size();
			LIFGenerateCellsForMutants(commonFuncs,
				dimensionValuesStr,
				vOdd,
				matrixCells,
				mutantsDimensionValue,
				corresponding_tests, allOPs);
			for (int x = startIndex; x < matrixCells.size(); x++){
				faultTypes.push_back("LIF");
				faultTerms.push_back(terms[i]);
			}
			startIndex = matrixCells.size();
			int matrix_size2 = matrixCells.size();
			if (isSingleLitral) //如果只有一个词 则退化成TOF
			{
				TOFGenerateCellsForMutants(commonFuncs, dimensionValuesStr, vOdd, matrixCells,
					mutantsDimensionValue,
					corresponding_tests, allOPs);

				for (int x = startIndex; x < matrixCells.size(); x++){
					faultTypes.push_back("TOF");
					faultTerms.push_back(terms[i]);
				}
				startIndex = matrixCells.size();
			}

			LRFGenerateCellsForMutants(commonFuncs, dimensionValuesStr, vOdd, matrixCells,
				mutantsDimensionValue,
				corresponding_tests, allOPs, allUPs);

			for (int x = startIndex; x < matrixCells.size(); x++){
				faultTypes.push_back("LRF");
				faultTerms.push_back(terms[i]);
			}
			startIndex = matrixCells.size();

			LOFGenerateCellsForMutants(commonFuncs, dimensionValuesStr, vOdd, matrixCells,
				mutantsDimensionValue,
				corresponding_tests, allOPs, allUPs);

			for (int x = startIndex; x < matrixCells.size(); x++){
				faultTypes.push_back("LOF");
				faultTerms.push_back(terms[i]);
			}
			startIndex = matrixCells.size();

			get_mutant_constrains(mutantsDimensionValue, vOdd, corresponding_tests, newExp, i, mutants);

			//添加6个非CC-S可检测的双缺陷
			CDoubleLIRFHandler dlirfHandler;
			dlirfHandler.run2(dimensionValuesStr, vOdd, matrixCells, allOPs, allUPs);
			for (int x = startIndex; x < matrixCells.size(); x++){
				mutants.push_back("");
				faultTypes.push_back("LIRF");
				faultTerms.push_back(terms[i]);
			}
			startIndex = matrixCells.size();


			CDoubleLORFHandler dlorfHandler;
			dlorfHandler.run2(dimensionValuesStr, vOdd, matrixCells, allOPs, allUPs);
			for (int x = startIndex; x < matrixCells.size(); x++){
				mutants.push_back("");
				faultTypes.push_back("LORF");
				faultTerms.push_back(terms[i]);
			}
			startIndex = matrixCells.size();


			CDoubleLRRFHandler dlrrfHandler;
			dlrrfHandler.run2(dimensionValuesStr, vOdd, matrixCells, allOPs, allUPs);
			for (int x = startIndex; x < matrixCells.size(); x++){
				mutants.push_back("");
				faultTypes.push_back("LRRF");
				faultTerms.push_back(terms[i]);
			}
			startIndex = matrixCells.size();

		}

		getLIFxLIFCells2(commonFuncs, terms, dimensionNum, termsDimension, preData.opover2, allOPs, vOdd, matrixCells, mutants, faultTypes, faultTerms);
		getLIFxLRFCells2(commonFuncs, terms, dimensionNum, termsDimension, preData.opover2, allUPs, allOPs, vOdd, matrixCells, mutants, faultTypes, faultTerms);
		getLRFxLRFCells2(commonFuncs, terms, dimensionNum, termsDimension, allUPs, allOPs, vOdd, matrixCells, mutants, faultTypes, faultTerms);

	}

	//转换约束条件
	//得到所有的真值点，这一步必须进入循环吗？
	commonFuncs.getAlltModelIndexAppearedInMatrix(matrixCells, tModelIndexAppeared);
	//对列向量进行排序，如311排在312之前
	sort(tModelIndexAppeared.begin(), tModelIndexAppeared.end());

	commonFuncs.getCoefficientMatrix(tModelIndexAppeared, matrixCells, coefficientMatrix);

	vector<vector<int>>finalCoefficientMatrix;
	vector<int>finalTModelIndex;
	removeAllZeroCols(coefficientMatrix, tModelIndexAppeared, finalCoefficientMatrix, finalTModelIndex);

	vector<int>().swap(tModelIndexAppeared);//只是为了打印用
	vector<vector<int>>().swap(coefficientMatrix);//只是为了打印用
	matrixCells.clear();

	//Glpk解约束方程
	GLPK_Solve glpk_Solve;
	vector<int> colSelected;
	vector<int> deletedRows;
	removeAllZeroRows(finalCoefficientMatrix, coefficientMatrix, deletedRows);
	coefficientMatrix.swap(finalCoefficientMatrix);
	coefficientMatrix.clear();
	for (int i = deletedRows.size() - 1; i >= 0; i--){
		vector<string>::iterator itrl = mutants.begin() + i;
		mutants.erase(itrl);
	}
	//可以修改，不需要每次都生成
	vector<string> mutants_DP;
	for (int num = 0; num<mutants.size(); num++)
	{
		//printf("%s,%d,mutants[%d]:[%s]\r\n", __FILE__, __LINE__, num, mutants[num].c_str());
		mutants_DP.push_back(mutants[num]);
	}
	glpk_Solve.run(colSelected, &finalCoefficientMatrix);
	//将得到的格覆盖转换成测试用例
	vector<string> testset;
	for (int j = 0; j < colSelected.size(); j++)
	{
		char tmp[40];
		sprintf(tmp, "%d", finalTModelIndex[colSelected[j]]);
		//printf("%d:", finalTModelIndex[colSelected[j]]);
		string inde = "";
		inde.append(tmp);
		string point01Str = commonFuncs.transTModelIndexStrTo01Str(inde, vOdd);
		testset.push_back(point01Str);
	}
	//test
	// cout << endl;
	// cout << "Test-Case" << endl;
	for (int i = 0; i < testset.size(); ++i){
		// cout << testset[i] << endl;
	}
	// cout << "Test-Case-End" << endl;

	
	vector<string>& valid = vector<string>();
	vector<string>& invalid = vector<string>();
	FaultDetection fd = FaultDetection();
	//输出到csv文件

	// cout <<"------------------------------"<<testset.size();
	vector<string> temp1, temp2;
	for (int i = 0; i < testset.size(); i++){
		if (uti.evaluate(expression, testset[i])){
			temp1.push_back(testset[i]);
		}
		else{
			temp2.push_back(testset[i]);
		}
	}


	// cout <<"--------------------------------------" <<temp1.size() << "	" << temp2.size()<<endl;



	vector<string> terms__ = uti.extractDNFstyle(expression);
	vector<string> temp3 = temp1;
	temp1.clear();
	vector<string> literals_in_this_term;
	string model__ = uti.generateModel(expression);
	
	string testcase__ = "";
	for (int i = 0; i < terms__.size(); i++){
		literals_in_this_term = uti.extractCNFstyle(terms__[i]);
		for (int m = 0; m < literals_in_this_term.size(); m++){
			if (literals_in_this_term[m][0] == '!')
				literals_in_this_term[m] = literals_in_this_term[m][1];
		}
		for (int j = 0; j < temp3.size(); j++){
			if (temp3[j] != ""){
				for (int k = 0; k < literals_in_this_term.size(); k++){
					testcase__ += temp3[j][model__.find(literals_in_this_term[k])];
				}

				if (uti.evaluate(terms__[i], testcase__) == true){
					temp1.push_back(temp3[j]);
					temp3[j] = "";
				}
				testcase__ = "";
			}

		}
	}

	for (int i = 0; i < temp2.size(); i++){
			temp1.push_back(temp2[i]);
	}

	vector<string> temp11 = temp1;

	for (int i = 0; i < temp11.size(); i++){
		boolean flag;
		for (int j = 0; j < mutants1.mutants.size(); j++){
			flag = true;
			if (uti.evaluateMutant(expression, mutants1.mutants[j].mutant, temp11[i]) != uti.evaluate(expression, temp11[i])){

				for (int ii = 0; ii < temp11.size(); ii++){
					if (ii != i && uti.evaluateMutant(expression, mutants1.mutants[j].mutant, temp11[ii]) != uti.evaluate(expression, temp11[ii])){
						flag = false;
						break;
					}
				}
				if (flag == true){
					break;
				}
			}
			
		}

		if (flag == false){
			temp11.erase(temp11.begin() + i);
			i = i - 1;
		}
	}


	temp1 = temp11;


	ofstream oFile;
	string path = "E:\\FaultTable\\" + type + "_" + expression+".csv";
	oFile.open(path, ios::out | ios::trunc);
	oFile << "mutantsType" << "," << "mutants";
	for (int i = 0; i < temp1.size(); i++)
		oFile << "," << "." + temp1[i] + ".";
	oFile << endl;



	oFile << "Specification:" << "," << "S=" + expression;
	for (int i = 0; i < temp1.size(); i++){
		if (uti.evaluate(expression, temp1[i])){
			oFile << ",";
			COLOR_PRINT2(oFile, "1", 2, 7);
		}
		else{
			oFile << ",";
			COLOR_PRINT2(oFile, "0", 2, 7);
		}
	}
	oFile << endl;

	for (int i = 0; i < mutants1.mutants.size(); i++){
		oFile << mutants1.mutants[i].type << "," << mutants1.mutants[i].mutant;
		for (int j = 0; j < temp1.size(); j++){
			if (uti.evaluateMutant(expression, mutants1.mutants[i].mutant, temp1[j])){
				if (uti.evaluate(expression, temp1[j]))
					oFile << "," << "1";
				else
					//oFile << "," << "4";
					oFile << "," << "1";
			}
			else{
				if (uti.evaluate(expression, temp1[j]))
					//oFile << "," << "3";
					oFile << "," << "0";
				else
					oFile << "," << "0";
			}
		}
		oFile << endl;
	}

}

//以下起是我的代码，主干模块、纯扩张模块、纯扩张子模块

int call_generateNearTestset_times = 0;
time_t preprocess_interval = 0;
set<string> near_testcase_set;
bool algo_mode=false; //表示使用的算法类型，取1为旧算法，取0为新算法

//临近法生成测试用例，输入待修改的变量集合和当前测试用例，输出修改后的测试用例
string generateNearTestset(string test, vector<char> wordset, unordered_map<char, int> literal_abstract_pos)
{

	for (int i = 0; i < wordset.size(); i++)
	{
		int k = literal_abstract_pos[wordset[i]]; 
		//// cout << "改变变量在变量序列中的位置"<< k << endl;
		if (test[k] == '0') test[k] = '1';
		else test[k] = '0';
	}
	call_generateNearTestset_times += 1;
	near_testcase_set.insert(test);
	//// cout << "加入用例集" << test << "当前集合大小" << near_testcase_set.size() << endl;
	return test;
}

//维度值计算函数：输入表达式和测试用例，输出该测试用例在该表达式上的维度值与原表达式的差值，
int level_in_mutant(string expression, string false_expression, string testcase, unordered_map<char, int> literal_abstract_pos)
{
	Utility uti;
	vector<string> terms = uti.extractDNFstyle(expression);
	int exp_val = 0;
	for (string term : terms)
	{
		string model = uti.generateModel(term); //子变量集，如ab、cd、bc
		string child_testcase;
		//根据子变量位置生成子测试用例进行检测
		for (char key : model)
		{
			int pos = literal_abstract_pos[key];
			child_testcase+=testcase[pos];
		}
		//// cout << "term " << term << " " << uti.evaluate(term, child_testcase) << endl;
		if ((uti.evaluate(term, child_testcase)) == true) exp_val += 1;
	}
	terms = uti.extractDNFstyle(false_expression);
	int f_exp_val = 0;
	for (string term : terms)
	{
		string model = uti.generateModel(term); //子变量集，如ab、cd、bc
		string child_testcase;
		//根据子变量位置生成子测试用例进行检测
		for (char key : model)
		{
			int pos = literal_abstract_pos[key];
			child_testcase += testcase[pos];
		}
		//// cout << "term " << term << " " << uti.evaluate(term, child_testcase) << endl;
		if ((uti.evaluate(term, child_testcase)) == true) f_exp_val += 1;
	}
	
	//// cout << "exp_val: " << exp_val << " f_exp_val: " << f_exp_val << endl;
	return f_exp_val - exp_val;
}

vector<string> fault_in_LRF(string exp,string fault_exp,TestCase testcase, NFP nfp, unordered_map<char, int> literal_abstract_pos)
{
	vector<string> res;
	vector<string> sesusceptible_res;
	//// cout << "LRF检测" << endl;
	DnfMutant dnf_mutant;
	Utility uti;
	list<Literal> valid = nfp.getValid();
	list<Literal>::iterator plist;
	//// cout << "当前nfp：" << nfp.term << endl;
	for (plist = valid.begin(); plist != valid.end(); plist++)
	{
		//检查当前元素是否是敏感变量，否，则，生成临近测试用例检测
		//是，则插入涉敏感集备用返回集
		if (testcase.susceptibleLiterSet.find(*plist) == testcase.susceptibleLiterSet.end()) //20211211：去掉是否是敏感集的判断
		{
			//// cout << "    理想的可替换词：" << (*plist).getChar() << endl;
			//生成临近测试用例测试，有效则将re加入结果集
			vector<char> change_literal;
			change_literal.push_back((*plist).getChar());
			string near_testcase = generateNearTestset(testcase.testcase, change_literal, literal_abstract_pos); //临近法求新测试用例
			//// cout << "   改变的变量 " << (*plist).getChar() << endl;
			//// cout << "   LRF中单变量临近测试用例 " << near_testcase << endl;
			bool check_if_true = uti.evaluateMutant(exp,fault_exp, near_testcase);
			if (check_if_true) continue;  //20211211：升维跳过，没升则加入结果集
			string re = dnf_mutant.generateLRFmutant_inPos(exp, nfp, *plist);
			res.push_back(re);
			//// cout << re << endl;
		}
		else   //生成临近测试用例检测
		{
			//// cout << "    备用的可替换词：" << (*plist).getChar() << endl;
			string re = dnf_mutant.generateLRFmutant_inPos(exp, nfp, *plist);
			sesusceptible_res.push_back(re);
			//// cout << re << endl;
		}
		
	}
	//// cout << "理想返回集规模：" << res.size() << endl;
	//// cout << "备用返回集规模：" << sesusceptible_res.size() << endl;
	if(res.size()!=0) return res;
	else return sesusceptible_res;
}

vector<string> fault_in_LRF_ByLevel(string exp, string fault_exp, TestCase testcase, NFP nfp, unordered_map<char, int> literal_abstract_pos)
{
	vector<string> res;
	vector<string> sesusceptible_res;
	// cout << "LRF检测(维度检测版)" << endl;
	DnfMutant dnf_mutant;
	Utility uti;
	list<Literal> valid = nfp.getValid();
	list<Literal>::iterator plist;
	for (plist = valid.begin(); plist != valid.end(); plist++)
	{
		//检查当前元素是否是敏感变量，否，则，生成临近测试用例检测
		//是，则插入涉敏感集备用返回集
		//if (testcase.susceptibleLiterSet.find(*plist) == testcase.susceptibleLiterSet.end()) //20211211：去掉是否是敏感集的判断
		//{
			//生成临近测试用例测试，有效则将re加入结果集
			vector<char> change_literal;
			change_literal.push_back((*plist).getChar());
			string near_testcase = generateNearTestset(testcase.testcase, change_literal, literal_abstract_pos); //临近法求新测试用例
			// cout << "   改变的变量 " << (*plist).getChar() << endl;
			// cout << "   LRF中单变量临近测试用例 " << near_testcase << endl;
			//bool check_if_true = uti.evaluate(fault_exp, near_testcase);
			bool check_if_true = false;
			if (level_in_mutant(exp, fault_exp, near_testcase, literal_abstract_pos) > 0)  check_if_true = true;
			if (check_if_true) continue;  //20211211：升维跳过，没升则加入结果集
			string re = dnf_mutant.generateLRFmutant_inPos(exp, nfp, *plist);
			res.push_back(re);
			// cout << re << endl;
		/*}
		else   //生成临近测试用例检测
		{
			string re = dnf_mutant.generateLRFmutant_inPos(exp, nfp, *plist);
			sesusceptible_res.push_back(re);
			// cout << re << endl;
		}*/

	}
	if (res.size() == 1) return res;
	else return sesusceptible_res;
}

vector<string> fault_in_LRF_ByRule6(string exp, string fault_exp, vector<Literal> replace_literals, NFP nfp)
{
	vector<string> res;
	DnfMutant dnf_mutant;
	for (Literal literal : replace_literals){
		string re = dnf_mutant.generateLRFmutant_inPos(exp, nfp, literal);
		res.push_back(re);
		//// cout << re << endl;
	}
	return res;
}

vector<string> fault_in_CORForLOF(string exp, string fault_exp, TestCase testcase, NFP nfp)
{
	vector<string> res;
	// cout << "CORF、LOF检测" << endl;
	// cout << "是否存在敏感点 : " << (testcase.susceptibleLiterSet.find(nfp.getKey()) == testcase.susceptibleLiterSet.end()) << endl;
	//输出0表示能找到，1表示找不到
	bool b = testcase.susceptibleLiterSet.find(nfp.getKey()) == testcase.susceptibleLiterSet.end();
	Utility uti;
	DnfMutant dnf_mutant;
	string model = uti.generateModel(exp);
	list<Literal> otherLiterals = nfp.getOther();
	//非其他项敏感点集合
	list<Literal> notOtherTermKey;
	for (Literal l : otherLiterals){
		Literal newL = Literal(l.getChar(), !l.getPos());
		if (testcase.susceptibleLiterSet.find(newL) == testcase.susceptibleLiterSet.end()){
			notOtherTermKey.push_back(l);
		}
	}
	string t = testcase.testcase;
	int len = model.length();
	/*
	char* loforcorf = new char[len];
	for (int i = 0; i < len; i++){
	loforcorf[i] = t.at(i);
	}*/
	//strncpy(loforcorf, t.c_str(), t.length() + 1);

	for (Literal l : otherLiterals){
		if (!(testcase.susceptibleLiterSet.find(Literal(l.getChar(), !l.getPos())) == testcase.susceptibleLiterSet.end())){
			string re = dnf_mutant.generateLOFmutant(exp, nfp);
			res.push_back(re);
		}
		else{
			char* loforcorf = new char[len];
			for (int i = 0; i < len; i++){
				loforcorf[i] = t.at(i);
			}
			char c = l.getChar();
			int index = model.find(c);
			loforcorf[model.find(c)] = l.getPos() == true ? '0' : '1';
			loforcorf[model.find(nfp.getKey().getChar())] = nfp.getKey().getPos() == true ? '1' : '0';
			string LOForCORF(&loforcorf[0], &loforcorf[len]);
			//delete[]loforcorf;
			call_generateNearTestset_times += 1;
			near_testcase_set.insert(LOForCORF);
			bool expTrue = uti.evaluate(exp, LOForCORF);
			if (expTrue || !uti.evaluateDNF(fault_exp, LOForCORF, model)){
				//判断为LOF缺陷
				string re = dnf_mutant.generateLOFmutant(exp, nfp);
				res.push_back(re);
			}
			else{
				//定位为CORF
				char* tcorf = new char[len];
				strncpy(tcorf, t.c_str(), t.length() + 1);
				for (Literal l : notOtherTermKey){
					int index = model.find(l.getChar());
					tcorf[index] = tcorf[index] == '1' ? '0' : '1';

					string tCORF(&tcorf[0], &tcorf[len]);
					call_generateNearTestset_times += 1;
					near_testcase_set.insert(tCORF);
					//delete[]tcorf;
					// cout << "tCORF:" + tCORF << endl;
					if (!uti.evaluate(exp, tCORF)){
						string re = dnf_mutant.generateCORFmutants(exp, nfp, l);
						// cout << re << endl;
						res.push_back(re);
					}
				}
			}
		}
	}
	//去重
	set<string>s(res.begin(), res.end());
	res.assign(s.begin(), s.end());
	return res;
}

vector<string> fault_in_CORForLOF_ByRule6(string exp, string fault_exp, TestCase testcase, NFP nfp)
{
	vector<string> res;
	// cout << "CORF、LOF检测" << endl;
	// cout << "是否存在敏感点 : " << (testcase.susceptibleLiterSet.find(nfp.getKey()) == testcase.susceptibleLiterSet.end()) << endl;
	//输出0表示能找到，1表示找不到
	bool b = testcase.susceptibleLiterSet.find(nfp.getKey()) == testcase.susceptibleLiterSet.end();
	Utility uti;
	DnfMutant dnf_mutant;
	string model = uti.generateModel(exp);
	list<Literal> otherLiterals = nfp.getOther();
	//非其他项敏感点集合
	list<Literal> notOtherTermKey;
	for (Literal l : otherLiterals){
		Literal newL = Literal(l.getChar(), !l.getPos());
		if (testcase.susceptibleLiterSet.find(newL) == testcase.susceptibleLiterSet.end()){
			notOtherTermKey.push_back(l);
		}
	}
	string t = testcase.testcase;
	int len = model.length();
	/*
	char* loforcorf = new char[len];
	for (int i = 0; i < len; i++){
	loforcorf[i] = t.at(i);
	}*/
	//strncpy(loforcorf, t.c_str(), t.length() + 1);

	for (Literal l : otherLiterals){
		if (!(testcase.susceptibleLiterSet.find(Literal(l.getChar(), !l.getPos())) == testcase.susceptibleLiterSet.end())){
			string re = dnf_mutant.generateLOFmutant(exp, nfp);
			res.push_back(re);
		}
		else{
			char* loforcorf = new char[len];
			for (int i = 0; i < len; i++){
				loforcorf[i] = t.at(i);
			}
			char c = l.getChar();
			int index = model.find(c);
			loforcorf[model.find(c)] = l.getPos() == true ? '0' : '1';
			loforcorf[model.find(nfp.getKey().getChar())] = nfp.getKey().getPos() == true ? '1' : '0';
			string LOForCORF(&loforcorf[0], &loforcorf[len]);
			//delete[]loforcorf;
			call_generateNearTestset_times += 1;
			near_testcase_set.insert(LOForCORF);
			bool expTrue = uti.evaluate(exp, LOForCORF);
			if (expTrue || !uti.evaluateDNF(fault_exp, LOForCORF, model)){
				//判断为LOF缺陷
				string re = dnf_mutant.generateLOFmutant(exp, nfp);
				res.push_back(re);
			}
			else{
				//定位为CORF
				char* tcorf = new char[len];
				strncpy(tcorf, t.c_str(), t.length() + 1);
				for (Literal l : notOtherTermKey){
					int index = model.find(l.getChar());
					tcorf[index] = tcorf[index] == '1' ? '0' : '1';

					string tCORF(&tcorf[0], &tcorf[len]);
					call_generateNearTestset_times += 1;
					near_testcase_set.insert(tCORF);
					//delete[]tcorf;
					// cout << "tCORF:" + tCORF << endl;
					if (!uti.evaluate(exp, tCORF)){
						string re = dnf_mutant.generateCORFmutants(exp, nfp, l);
						// cout << re << endl;
						res.push_back(re);
					}
				}
			}
		}
	}
	//去重
	set<string>s(res.begin(), res.end());
	res.assign(s.begin(), s.end());
	return res;
}

vector<string> fault_in_CORF_ByRule6(string exp, string fault_exp, TestCase testcase, NFP nfp)
{
	vector<string> res;
	// cout << "CORF、LOF检测" << endl;
	// cout << "是否存在敏感点 : " << (testcase.susceptibleLiterSet.find(nfp.getKey()) == testcase.susceptibleLiterSet.end()) << endl;
	//输出0表示能找到，1表示找不到
	bool b = testcase.susceptibleLiterSet.find(nfp.getKey()) == testcase.susceptibleLiterSet.end();
	Utility uti;
	DnfMutant dnf_mutant;
	string model = uti.generateModel(exp);
	list<Literal> otherLiterals = nfp.getOther();
	//非其他项敏感点集合
	list<Literal> notOtherTermKey;
	for (Literal l : otherLiterals){
		Literal newL = Literal(l.getChar(), !l.getPos());
		if (testcase.susceptibleLiterSet.find(newL) == testcase.susceptibleLiterSet.end()){
			notOtherTermKey.push_back(l);
		}
	}
	string t = testcase.testcase;
	int len = model.length();
	/*
	char* loforcorf = new char[len];
	for (int i = 0; i < len; i++){
	loforcorf[i] = t.at(i);
	}*/
	//strncpy(loforcorf, t.c_str(), t.length() + 1);

	for (Literal l : otherLiterals){
		if (!(testcase.susceptibleLiterSet.find(Literal(l.getChar(), !l.getPos())) == testcase.susceptibleLiterSet.end())){
			string re = dnf_mutant.generateLOFmutant(exp, nfp);
			res.push_back(re);
		}
		else{
			char* loforcorf = new char[len];
			for (int i = 0; i < len; i++){
				loforcorf[i] = t.at(i);
			}
			char c = l.getChar();
			int index = model.find(c);
			loforcorf[model.find(c)] = l.getPos() == true ? '0' : '1';
			loforcorf[model.find(nfp.getKey().getChar())] = nfp.getKey().getPos() == true ? '1' : '0';
			string LOForCORF(&loforcorf[0], &loforcorf[len]);
			//delete[]loforcorf;
			call_generateNearTestset_times += 1;
			near_testcase_set.insert(LOForCORF);
			bool expTrue = uti.evaluate(exp, LOForCORF);
			if (expTrue || !uti.evaluateDNF(fault_exp, LOForCORF, model)){
				//判断为LOF缺陷
				string re = dnf_mutant.generateLOFmutant(exp, nfp);
				res.push_back(re);
			}
			else{
				//定位为CORF
				char* tcorf = new char[len];
				strncpy(tcorf, t.c_str(), t.length() + 1);
				for (Literal l : notOtherTermKey){
					int index = model.find(l.getChar());
					tcorf[index] = tcorf[index] == '1' ? '0' : '1';

					string tCORF(&tcorf[0], &tcorf[len]);
					call_generateNearTestset_times += 1;
					near_testcase_set.insert(tCORF);
					//delete[]tcorf;
					// cout << "tCORF:" + tCORF << endl;
					if (!uti.evaluate(exp, tCORF)){
						string re = dnf_mutant.generateCORFmutants(exp, nfp, l);
						// cout << re << endl;
						res.push_back(re);
					}
				}
			}
		}
	}
	//去重
	set<string>s(res.begin(), res.end());
	res.assign(s.begin(), s.end());
	return res;
}

vector<string> fault_in_LOF_ByRule6(string exp, string fault_exp, TestCase testcase, NFP nfp)
{
	vector<string>res;
	DnfMutant dnf_mutant;
	string re = dnf_mutant.generateLOFmutant(exp, nfp);
	res.push_back(re);
	return res;
}

//检查一个变体集合是否全部通过检测
bool isAllPassedTest(string expression, string false_expression, vector<string>testset)
{
	Utility uti;
	bool res = true;
	for (int i = 0; i < testset.size(); ++i)
	{
		res = res&(!uti.checkTestDiffer(testset[i], expression, false_expression));
	}
	return res;
}

//复核结果集，0为完全通过检测，1为纯扩张，2为纯收缩，3为复合型
int checkResultType(string expression, string  false_expression, vector<string> testset)
{
	Utility uti;
	vector<string> true_testset;
	vector<string> false_testset;
	for (int i = 0; i < testset.size(); ++i)
	{
		if (uti.evaluate(expression, testset[i]) == true) true_testset.push_back(testset[i]);
		else false_testset.push_back(testset[i]);
	}
	bool isAllPassed_T = isAllPassedTest(expression, false_expression, true_testset);
	bool isAllPassed_F = isAllPassedTest(expression, false_expression, false_testset);
	if (isAllPassed_T && isAllPassed_F)
	{
		//// cout << "复核:该用例最小集表现与原表达式一致，通过检测" << endl;
		return 0;
	}
	if (isAllPassed_T == true)
	{
		//// cout << "复核：该用例只有扩张点" << endl;
		return 1;
	}
	else if (isAllPassed_F == true)
	{
		//// cout << "复核：该用例只有收缩点" << endl;
		return 2;
	}
	else
	{
		//// cout << "复核：该用例既有收缩点也有扩张点" << endl;
		return 3;
	}
}

//只有扩张点分支的主干程序，exp为待测表达式，testset为待测点集，此处为假点集
vector<string> judgeAllExpand(string exp, string fault_exp, vector<string> testset, vector<string> all_testset)
{
	vector<string> res;
	Utility uti;
	string model = uti.generateModel(exp); //全部变量集abcd
	unordered_map<char, int> literal_abstract_pos;  //全部变量集对应位置关系
	for (int i = 0; i < model.size(); i++)
	{
		literal_abstract_pos.insert(pair<char, int>{ model[i], i });
	}
	//按照数据结构的设计，对待测点情况进行重新刻画
	//逐项排查测试用例
	for (string testset_item : testset) 
	{
		bool is_vaild = uti.checkTestDiffer(testset_item, exp, fault_exp);
		if (is_vaild == false) continue;  //跳过无效测试用例
		TestCase testcase= TestCase(exp,testset_item,false);
		//对每个测试用例中的UTP/NFP进行排查，对于纯扩张函数，这里是NFP
		//list<UTPorNFP> nfps = testcase.getInfo(exp, testset_item, false);
		list<NFP> nfps = testcase.getNFP(exp, testset_item);
		for (NFP nfp : nfps)
		{
			//// cout << "……当前nfp：" << nfp.term<<endl;
			//首先使用全临近法判断是否属于LRF，从而走向两个分支
			vector<char> change_literal;
			vector<char> backup_literal;
			list<Literal> valid_literals = nfp.getValid();
			bool is_sensitive = 1;  //判断是否全是敏感点，取1为敏感点
			for (Literal literal : valid_literals)
			{
				//// cout << "待检验词 " << literal.getChar() << endl;
				is_sensitive = is_sensitive & (testcase.susceptibleLiterSet.find(literal) != testcase.susceptibleLiterSet.end());
				auto got = testcase.susceptibleLiterSet.find(literal);
				if (got != testcase.susceptibleLiterSet.end()){
					backup_literal.push_back(literal.getChar());
					continue;
				}//变量在敏感集中存在，跳过该变量
				// cout << "不是敏感词，是有效替换 " << literal.getChar() << endl;
				change_literal.push_back(literal.getChar()); //加入待修改变量集合
			}
			bool check_if_LRF=false;
			if (is_sensitive == 1)  //全是敏感点，走这条路
			{
				/*vector<string> res_items1 = fault_in_LRF(exp, fault_exp, testcase, nfp, literal_abstract_pos);
				for (string res_item : res_items1)
				{
					//// cout << "……当前nfp：" << nfp.term << " 针对该项的检测结果: " << "【" << res_item << "】" << endl;
					//再次复核表征是否为纯扩张，是则加入结果集，否则不加入
					int check_val = checkResultType(exp, res_item, all_testset);
					if (check_val == 1) res.push_back(res_item);
				}*/
				check_if_LRF = true;

			}
			else{
				string near_testcase = generateNearTestset(testset_item, change_literal, literal_abstract_pos); //临近法求新测试用例
				//// cout << "全临近的临近测试用例 " << near_testcase << endl;
				check_if_LRF = uti.evaluateMutant(exp, fault_exp, near_testcase);
			}
			vector<string> res_items;
			if (check_if_LRF == false)
			{
				res_items = fault_in_LRF(exp,fault_exp,testcase,nfp,literal_abstract_pos);
			}
			else
			{
				res_items = fault_in_CORForLOF(exp, fault_exp,testcase, nfp);
				//// cout << "11111最终结果: " << res_items.size() << endl;
				//if (backup_literal.size() != 0){
					vector<string> res_items1 = fault_in_LRF(exp, fault_exp, testcase, nfp, literal_abstract_pos);
					for (string res_item : res_items1)
					{
						//// cout << "……当前nfp：" << nfp.term << " 针对该项的检测结果: " << "【" << res_item << "】" << endl;
						//再次复核表征是否为纯扩张，是则加入结果集，否则不加入
						int check_val = checkResultType(exp, res_item, all_testset);
						if (check_val == 1) res.push_back(res_item);
					}
				//}
			}
			for (string res_item : res_items)
			{
				//// cout <<"……当前nfp："<<nfp.term<<" 针对该项的检测结果: "<<"【"<< res_item <<"】"<< endl;
				//再次复核表征是否为纯扩张，是则加入结果集，否则不加入
				int check_val=checkResultType(exp, res_item, all_testset);
				if (check_val==1) res.push_back(res_item);
			}
		}
		preprocess_interval += testcase.time_interval;
		cout << "纯扩张" << testcase.time_interval << endl;
	}
	//// cout << "最终结果: " << endl;
	for (string res_item :res)
	{
		//// cout << "【" << res_item << "】" << endl;
	}
	// vector<char> change;
	// change.push_back('c');
	// change.push_back('d');
	// string ressult=generateNearTestset("1010", change);
	// // cout << "临近法生成函数结果："<<ressult << endl;
	// 按照主干程序生成一个测试用例对待测表达式进行测定,从而决定分支走向
	return res;
}

//只有扩张点分支的主干程序（维度值判定版），exp为待测表达式，testset为待测点集，此处为假点集
vector<string> judgeAllExpand_ByLevel(string exp, string fault_exp, vector<string> testset, vector<string> all_testset)
{
	vector<string> res;
	Utility uti;
	string model = uti.generateModel(exp); //全部变量集abcd
	unordered_map<char, int> literal_abstract_pos;  //全部变量集对应位置关系
	for (int i = 0; i < model.size(); i++)
	{
		literal_abstract_pos.insert(pair<char, int>{ model[i], i });
	}
	//按照数据结构的设计，对待测点情况进行重新刻画
	//逐项排查测试用例
	for (string testset_item : testset)
	{
		bool is_vaild = uti.checkTestDiffer(testset_item, exp, fault_exp);
		if (is_vaild == false) continue;  //跳过无效测试用例
		TestCase testcase = TestCase(exp, testset_item, false);
		//对每个测试用例中的UTP/NFP进行排查，对于纯扩张函数，这里是NFP
		//list<UTPorNFP> nfps = testcase.getInfo(exp, testset_item, false);
		list<NFP> nfps = testcase.getNFP(exp, testset_item);
		for (NFP nfp : nfps)
		{
			// cout << "……当前nfp：" << nfp.term << endl;
			//首先使用全临近法判断是否属于LRF，从而走向两个分支
			vector<char> change_literal;
			list<Literal> valid_literals = nfp.getValid();
			//bool is_sensitive = 1;  //判断是否全是敏感点，取1为敏感点
			for (Literal literal : valid_literals)
			{
				change_literal.push_back(literal.getChar()); //加入待修改变量集合
			}
			bool check_if_LRF;
			string near_testcase = generateNearTestset(testset_item, change_literal, literal_abstract_pos); //临近法求新测试用例
			// cout << "全临近的临近测试用例 " << near_testcase << endl;
			//check_if_LRF = uti.evaluateMutant(exp, fault_exp, near_testcase);//改为判定检测是否升维，未升维走LRF
			if (level_in_mutant(exp, fault_exp, near_testcase, literal_abstract_pos) > 0) check_if_LRF =true;
			else check_if_LRF = false;
			vector<string> res_items;
			if (check_if_LRF == false)
			{
				//res_items = fault_in_LRF(exp, fault_exp, testcase, nfp, literal_abstract_pos);
				res_items = fault_in_LRF_ByLevel(exp, fault_exp, testcase, nfp, literal_abstract_pos);
			}
			else
			{
				res_items = fault_in_CORForLOF(exp, fault_exp,testcase, nfp);
			}
			//// cout << "最终结果: " << endl;
			for (string res_item : res_items)
			{
				// cout << "……当前nfp：" << nfp.term << " 针对该项的检测结果: " << "【" << res_item << "】" << endl;
				//再次复核表征是否为纯扩张，是则加入结果集，否则不加入
				//int check_val = checkResultType(exp, res_item, all_testset);
				//if (check_val == 1) res.push_back(res_item);
				res.push_back(res_item);
			}
		}
		preprocess_interval += testcase.time_interval;
	}
	// cout << "最终结果: " << endl;
	for (string res_item : res)
	{
		// cout << "【" << res_item << "】" << endl;
	}
	// vector<char> change;
	// change.push_back('c');
	// change.push_back('d');
	// string ressult=generateNearTestset("1010", change);
	// // cout << "临近法生成函数结果："<<ressult << endl;
	// 按照主干程序生成一个测试用例对待测表达式进行测定,从而决定分支走向
	return res;
}

//只有扩张点分支的主干程序，适用Rule6更新版，exp为待测表达式，testset为待测点集，此处为假点集
vector<string> judgeAllExpand_ByRule6(string exp, string fault_exp, vector<string> testset, vector<string> all_testset){
	vector<string> res;
	Utility uti;
	string model = uti.generateModel(exp); //全部变量集abcd
	unordered_map<char, int> literal_abstract_pos;  //全部变量集对应位置关系
	for (int i = 0; i < model.size(); i++)
	{
		literal_abstract_pos.insert(pair<char, int>{ model[i], i });
	}
	//构造一个map，用于记录每个nfp命中有效测试用例的次数
	map<string, vector<string>> nfp_cell_map;
	map<string, set<char>> nfp_key_literal_map;
	map<string, int> nfp_times_map;
	map<string, int>::iterator iter;
	int useful_testcase_count = 0; //统计有效测试用例个数
    set<string> useful_testcase;
	//按照数据结构的设计，对待测点情况进行重新刻画
	//逐项排查测试用例
	for (string testset_item : testset)
	{
		bool is_vaild = uti.checkTestDiffer(testset_item, exp, fault_exp);
		if (is_vaild == false); //continue;  //跳过无效测试用例
		else{
			useful_testcase_count += 1;
			useful_testcase.insert(testset_item);
		}
		TestCase testcase = TestCase(exp, testset_item, false);
		//对每个测试用例中的UTP/NFP进行排查，对于纯扩张函数，这里是NFP
		//list<UTPorNFP> nfps = testcase.getInfo(exp, testset_item, false);
		list<NFP> nfps = testcase.getNFP(exp, testset_item);
		for (NFP nfp : nfps)
		{
			iter = nfp_times_map.find(nfp.term);
			if (iter == nfp_times_map.end()){
				nfp_times_map[nfp.term] = 1;
				vector<string> val;
				nfp_cell_map[nfp.term] = val;
				nfp_cell_map[nfp.term].push_back(testset_item);
				set<char> set_val;
				nfp_key_literal_map[nfp.term] = set_val;
				nfp_key_literal_map[nfp.term].insert(nfp.getKey().getChar());
			}
			else{
				nfp_times_map[nfp.term] = nfp_times_map[nfp.term] + 1;
				nfp_cell_map[nfp.term].push_back(testset_item);
				nfp_key_literal_map[nfp.term].insert(nfp.getKey().getChar());
			}
		}
		preprocess_interval += testcase.time_interval;
	}
	set<string> corf_relate_terms;
	for (iter = nfp_times_map.begin(); iter != nfp_times_map.end(); iter++){
		// cout << "当前term:" << iter->first << " 当前term的nfc个数:" << iter->second << " Tfail_0个数:" << useful_testcase_count << "关键变量个数：" << nfp_key_literal_map[iter->first].size() << endl;
		// cout << "当前term的nfc:" ;
		for (string cell : nfp_cell_map[iter->first]){
			// cout << " " << cell;
		}
		// cout << endl;
		if (iter->second != useful_testcase_count || nfp_key_literal_map[iter->first].size()<=1) continue;
		bool equal = true;
		for (string cell : nfp_cell_map[iter->first]){
			equal = equal&(useful_testcase.find(cell) != useful_testcase.end());
		}
		// cout << "集合是否相等:"<<equal<<endl;
		if (equal == false) continue;
		corf_relate_terms.insert(iter->first);
	}
	if (corf_relate_terms.size() != 0 && useful_testcase_count>1){
		//CORF
		//在这里适用Rule6加强条件，，满足加强条件：直接输出CORF定位结果
		//不满足加强条件，输出三种错误的解析结果

		// cout << "生成CORF" << endl;
		map<string, int> res_testcase_map;//统计每个结果出现的次数
		for (string testset_item : testset)
		{
			bool is_vaild = uti.checkTestDiffer(testset_item, exp, fault_exp);
			if (is_vaild == false) continue;  //跳过无效测试用例
			TestCase testcase = TestCase(exp, testset_item, false);
			//对每个测试用例中的UTP/NFP进行排查，对于纯扩张函数，这里是NFP
			//list<UTPorNFP> nfps = testcase.getInfo(exp, testset_item, false);
			list<NFP> nfps = testcase.getNFP(exp, testset_item);
			for (NFP nfp : nfps)
			{
				//TODO:这里需要修改一下，砍掉生成LOF的分支
				//// cout << "CORF分支" << endl;
				if (corf_relate_terms.find(nfp.term) == corf_relate_terms.end()){
					//此项不关联CORF错误，直接干掉
					continue;
				}
				vector<string> res_items = fault_in_CORF_ByRule6(exp, fault_exp, testcase, nfp);
				for (string res_item : res_items)
				{
					//// cout << "……当前nfp：" << nfp.term << " 针对该项的检测结果: " << "【" << res_item << "】" << endl;
					//再次复核表征是否为纯扩张，是则加入结果集，否则不加入
					int check_val = checkResultType(exp, res_item, all_testset);
					if (check_val == 1)
					{
						if (res_testcase_map.find(res_item) == res_testcase_map.end()) res_testcase_map[res_item] = 1;
						else  res_testcase_map[res_item] = res_testcase_map[res_item] + 1;

					}
				}
			}
		}
		//每轮都出现的结果加入最终结果集
		//// cout << " 有效测试用例： " << useful_testcase_count << endl;
		for (auto& res_testcase : res_testcase_map){
			//// cout << res_testcase.first << " 次数： " << res_testcase.second << endl;
			//if (res_testcase.second == useful_testcase_count){
				res.push_back(res_testcase.first);
			//}
		}
	}
	//else{
		//LRF和LOF
		// cout << "生成CORF、LOF和LRF" << endl;
		for (string testset_item : testset)
		{
			bool is_vaild = uti.checkTestDiffer(testset_item, exp, fault_exp);
			if (is_vaild == false) continue;  //跳过无效测试用例
			TestCase testcase = TestCase(exp, testset_item, false);
			//对每个测试用例中的UTP/NFP进行排查，对于纯扩张函数，这里是NFP
			//list<UTPorNFP> nfps = testcase.getInfo(exp, testset_item, false);
			list<NFP> nfps = testcase.getNFP(exp, testset_item);
			for (NFP nfp : nfps)
			{
				list<Literal> valid_literals = nfp.getValid();
				vector<Literal> first_replace_literals;
				vector<Literal> back_replace_literals;
				for (Literal literal : valid_literals)
				{
					auto got = testcase.susceptibleLiterSet.find(literal);
					if (got == testcase.susceptibleLiterSet.end()){
						//可替换项在敏感集中不存在，生成临近测试用例进行检测，合法则加入唯一取反集
						//生成临近测试用例测试，有效则将re加入结果集
						vector<char> change_literal;
						change_literal.push_back(literal.getChar());
						string near_testcase = generateNearTestset(testcase.testcase, change_literal, literal_abstract_pos); //临近法求新测试用例
						//// cout << "   改变的变量 " << literal.getChar() << endl;
						//// cout << "   LRF中单变量临近测试用例 " << near_testcase << endl;
						bool check_if_true = uti.evaluateMutant(exp,fault_exp, near_testcase);
						if (check_if_true) continue;  //取1跳过，取0加入唯一取反集
						first_replace_literals.push_back(literal);
					}
					else{
						back_replace_literals.push_back(literal);
					}
				}
				if (first_replace_literals.size() != 0){
					//生成LRF
					// cout << "生成最佳LRF" << first_replace_literals.size() << endl;
					vector<string> res_items = fault_in_LRF_ByRule6(exp, fault_exp, first_replace_literals, nfp);
					for (string res_item : res_items)
					{
						//// cout << "……当前nfp：" << nfp.term << " 针对该项的检测结果: " << "【" << res_item << "】" << endl;
						//再次复核表征是否为纯扩张，是则加入结果集，否则不加入
						int check_val = checkResultType(exp, res_item, all_testset);
						if (check_val == 1) res.push_back(res_item);
					}
				}
				else{
					if(back_replace_literals.size() != 0){
						//生成LRF
						// cout << "生成备用LRF:" << back_replace_literals.size() << endl;
						vector<string> res_items = fault_in_LRF_ByRule6(exp, fault_exp, back_replace_literals, nfp);
						for (string res_item : res_items)
						{
							//// cout << "……当前nfp：" << nfp.term << " 针对该项的检测结果: " << "【" << res_item << "】" << endl;
							//再次复核表征是否为纯扩张，是则加入结果集，否则不加入
							int check_val = checkResultType(exp, res_item, all_testset);
							if (check_val == 1) res.push_back(res_item);
						}
					}
					//生成LOF
					vector<string> res_items = fault_in_LOF_ByRule6(exp, fault_exp, testcase, nfp);
					for (string res_item : res_items)
					{
						 res.push_back(res_item);
					}
					//生成LOF和CORF
					if (corf_relate_terms.size() == 0 || useful_testcase_count <= 1){
						res_items = fault_in_CORForLOF_ByRule6(exp, fault_exp, testcase, nfp);
						for (string res_item : res_items)
						{
							//// cout << "……当前nfp：" << nfp.term << " 针对该项的检测结果: " << "【" << res_item << "】" << endl;
							//再次复核表征是否为纯扩张，是则加入结果集，否则不加入
							int check_val = checkResultType(exp, res_item, all_testset);
							if (check_val == 1) res.push_back(res_item);
						}
					}
				}
			}
		}
	//}
	//// cout << "最终结果: " << endl;
	//for (string res_item : res)
	//{
		//// cout << "【" << res_item << "】" << endl;
	//}
	return res;
}

vector<string> generate_TOForDORF_mutant_inPos(string exp, string fault_exp, TestCase testCase, UTP utp, Literal insert_literal)
{
	// cout<<"当前utp:"<<utp.term<<"  TOF DORF" << endl;
	vector<string> res;
	Utility uti;
	DnfMutant dnf_mutant;
	string model = uti.generateModel(exp);
	char inner = insert_literal.getChar();
	string testcase = testCase.testcase;
	int pos = model.find(inner);
	string rep = insert_literal.getPos() ? "0" : "1";
	string start_testcase = testcase;
	start_testcase.replace(start_testcase.begin() + pos, start_testcase.begin() + pos + 1, rep);
	call_generateNearTestset_times += 1;
	near_testcase_set.insert(start_testcase);
	vector<string> terms = uti.extractDNFstyle(exp);
	int termPos = -1;
	for (int i = 0; i < terms.size(); i++){
		if (terms.at(i) == utp.term) termPos = i;
	}
	vector<string> possibleTerms;
	if (termPos == 0) possibleTerms.push_back(terms.at(1));
	else {
		possibleTerms.push_back(terms.at(termPos - 1));
		if (termPos+1<terms.size()) possibleTerms.push_back(terms.at(termPos + 1));
	}
	for (string term : possibleTerms){
		if (term == utp.term) continue;
		int* testData = (int*)malloc(model.length()*sizeof(int));
		uti.stringTointArray(start_testcase, testData, model.length());

		string testdata = start_testcase;
		//得到使当前term为1的testData
		vector<string> literals_in_term = uti.extractCNFstyle(term);
		for (string literal : literals_in_term){
			if (literal.at(0) == '!' && start_testcase.at(model.find(literal.at(1))) == '1'){
				testData[model.find(literal.at(1))] = 0;
				int p = model.find(literal.at(1));
				testdata.replace(testdata.begin() + p, testdata.begin() + p + 1, "0");
			}
			else if (literal.at(0) != '!' && start_testcase.at(model.find(literal.at(0))) == '0'){
				testData[model.find(literal.at(0))] = 1;
				int p = model.find(literal.at(0));
				testdata.replace(testdata.begin() + p, testdata.begin() + p + 1, "1");
			}
		}
		call_generateNearTestset_times += 1;
		near_testcase_set.insert(testdata);

		int trueTermNum = 0;
		for (string t : terms){
			if (uti.isDNFtermTrue(t, testData, model)){
				// cout << "true term : " << t << endl;
				trueTermNum++;
			}
		}
		// cout << "true term number : " << trueTermNum << endl;
		if (trueTermNum>1 || !uti.evaluateMutant(exp, fault_exp, testdata)){
			res.push_back(dnf_mutant.generateDORFmutants(exp, term, utp.term));
		}
	}
	//if (res.size() == 0){
	//加入TOF结果
	res.push_back(dnf_mutant.generateTOFmutants(exp, utp.term));
	//}

	return res;
}//内部错误次主干程序

vector<string> fault_in_Inside(string exp, string fault_exp, TestCase testcase, UTP utp, unordered_map<char, int> literal_abstract_pos)
{
	vector<string> res;
	//vector<string> sesusceptible_res;
	//// cout << "本项错误检测" << endl;
	DnfMutant dnf_mutant;
	Utility uti;
	list<Literal> valid = utp.getInner();
	list<Literal>::iterator plist;
	vector<Literal> backup_not_set;
	for (plist = valid.begin(); plist != valid.end(); plist++)
	{
		//检查当前元素是否是敏感变量，否，则，生成临近测试用例检测
		//是，则插入涉敏感集备用返回集
		if (testcase.susceptibleLiterSet.find(*plist) == testcase.susceptibleLiterSet.end())
		{
			//生成临近测试用例测试，有效则将re加入结果集
			vector<char> change_literal;
			change_literal.push_back((*plist).getChar());
			string near_testcase = generateNearTestset(testcase.testcase, change_literal, literal_abstract_pos); //临近法求新测试用例
			//// cout << "   改变的变量 " << (*plist).getChar() << endl;
			//// cout << "   【本项内错误】单变量临近测试用例 " << near_testcase << endl;
			//// cout << "   【本项内错误】单变量临近测试用例检测结果：  " << uti.evaluateMutant(exp, fault_exp, near_testcase) << endl;
			bool check_if_true = uti.evaluateMutant(exp, fault_exp, near_testcase);
			if (!check_if_true) continue;  //取0跳过，取1则加入唯一取反集
			//取反项定位成功，生成LNF
			//注意：generateLNFmutant_inPos未实现
			string re = dnf_mutant.generateLNFmutant_inPos(exp, utp, *plist);
			res.push_back(re);
			// cout << re << endl;
			break;
		}
		else   //是，生成所有可能的取反情况
		{
			backup_not_set.push_back(*plist);

		}

	}
	if (res.size() != 0){
		/*for (Literal l : valid){
			if (testcase.susceptibleLiterSet.find(l) == testcase.susceptibleLiterSet.end()){
				vector<string> re1 = generate_TOForDORF_mutant_inPos(exp, fault_exp, testcase, utp, l);
				for (string rei : re1)
				{
					res.push_back(rei);
					// cout << rei << endl;
				}
				//break;
			}
		}*/
		return res;
	}
	else
	{
		//进入TOF和DORF辨析
		if (backup_not_set.size() != 0)
		{
			for (Literal not_literal : backup_not_set)
			{
				string re = dnf_mutant.generateLNFmutant_inPos(exp, utp, not_literal);
				res.push_back(re);
				// cout << re << endl;
			}
		}
		for (Literal l : valid){
			// cout << "TOF DORF branch 外部" << endl;
			//if (testcase.susceptibleLiterSet.find(l) == testcase.susceptibleLiterSet.end()){
				// cout << "TOF DORF branch 内部" << endl;
				vector<string> re1 = generate_TOForDORF_mutant_inPos(exp, fault_exp, testcase, utp, l);
				for (string rei : re1)
				{
					res.push_back(rei);
					// cout << rei << endl;
				}
				//break;
			//}
		}
	}
}

vector<string> generate_LRFmutant_inPos(string exp, UTP utp, vector<Literal> insert_literals,vector<Literal> out_literals)
{
	//// cout << " 根据插入集和缺失集生成可能的LRF "<< endl;
	vector<string> res;
	DnfMutant dnf_mutant;
	for (Literal insert_literal : insert_literals){
		for (Literal out_literal : out_literals){
			//// cout << "插入项:" << insert_literal.getChar() << "  被替换项:" << out_literal.getChar() << endl;
			res.push_back(dnf_mutant.generateLRFmutant_inPos_UTP(exp,utp,insert_literal,out_literal));
		}
	}
	//插入词已经定位，定位缺失词，可以定位则返回LRF结果集，无法定位则返回LIF结果集
	return res;
}

vector<string> generate_LIFmutant_inPos(string exp, UTP utp, vector<Literal> insert_literals)
{
	vector<string> res;
	DnfMutant dnf_mutant;
	//插入词已经定位，定位缺失词，可以定位则返回LRF结果集，无法定位则返回LIF结果集
	for (Literal insert_literal : insert_literals){
		res.push_back(dnf_mutant.generateLIFmutant_inPos_UTP(exp, utp, insert_literal));
	}
	return res;
}

//外部错误次主干程序
vector<string> fault_in_Outside(string exp, string fault_exp, TestCase testcase, UTP utp, unordered_map<char, int> literal_abstract_pos, vector<Literal> insert_literal)
{
	//// cout << "非本项错误检测" << endl;
	vector<string>  res;
	//取所有有效插入词，全部取反，生成一个新的临近测试用例
	vector<char> change_literal;
	for (Literal literal : insert_literal)
	{
		change_literal.push_back(literal.getChar());
	}
	string testset_item = testcase.testcase;
	string near_testcase = generateNearTestset(testset_item, change_literal, literal_abstract_pos); //临近法求新测试用例
	//// cout << "新测试用例起点：" <<near_testcase<< endl;
	//// cout << "原表达式：" << exp << endl;
	TestCase new_testcase = TestCase(exp, near_testcase, true);
	//对新测试用例，得到所有utp（包含原utp和新增utp）
	list<OTP> new_utps = new_testcase.getOTP(exp, near_testcase);
	/*// cout << "新测试用例：" << new_testcase.testcase << endl;
	for (OTP otp:new_utps)
	{
		// cout << "新utp：" << otp.term<< endl;
	}
	for (Literal literal1 : new_testcase.susceptibleLiterSet)
	{
		// cout << "新敏感变量：" << literal1.getChar() << endl;
	}*/
	//// cout << "新utp数量：" << new_utps.size() << endl;
	vector<Literal> possible_out_literals; //可能缺失集
	vector<Literal> backup_out_literals; //备用缺失集
	Utility uti;
	list<Literal> valid = utp.getInner();
	list<Literal>::iterator plist;
	//取本项中所有词，依次检查是否是敏感词，如果是敏感词则直接加入备用缺失集
	//本项中所有项减去备用缺失集，得到可能缺失集
	for (plist = valid.begin(); plist != valid.end(); plist++)
	{
		//检查当前元素是否是敏感变量，否，则进行下一步判断
		//是，则加入备用缺失集
		if (new_testcase.susceptibleLiterSet.find(*plist) == new_testcase.susceptibleLiterSet.end())
		{
			//不是敏感变量
			//遍历所有新增utp，检查变量是否在每个utp中都存在，是则加入可能缺失集，否则加入备用缺失集
			Literal literal_char = (*plist);
			if (new_utps.size() <= 1){
				//如果没有新增utp则直接加入可能缺失集
				possible_out_literals.push_back(literal_char);
				continue;
			}
			int in_new_utps_count = 0;
			for (OTP new_utp : new_utps)
			{
				//// cout << "当前OTP：" << new_utp.term << endl;
				list<Literal> new_valid = new_utp.getInner();
				int is_in_this_utp = 0;
				for (Literal new_literal : new_valid){
					if (new_literal.getChar() == literal_char.getChar()){
						//当前词在新增UTP中存在
						is_in_this_utp = 1;
						break;
					}
				}
				in_new_utps_count += is_in_this_utp;
			}
			if (in_new_utps_count == new_utps.size()){
				//当前变量在所有新增utp中都存在或都不存在
				possible_out_literals.push_back(literal_char);
			}
			else{
				backup_out_literals.push_back(literal_char);
			}
		}
		else
		{
			//是敏感变量，加入备用缺失集
			backup_out_literals.push_back((*plist));
		}
	}
	//遍历可能缺失集，代入表达式检测真假值，是否可定位出缺失
	vector<Literal> unique_out_literals;
	for (Literal possible_out_literal : possible_out_literals){
		//生成一个临近测试用例，代入表达式检测真假值
		//为真则使用唯一缺失集生成测试用例，否则使用备用缺失集生成测试用例
		vector<char> change_literal2;
		change_literal2.push_back(possible_out_literal.getChar());
		string near_testcase = generateNearTestset(new_testcase.testcase, change_literal2, literal_abstract_pos); //临近法求新测试用例
		//// cout << "   可能缺失集 " << possible_out_literal.getChar() << endl;
		//// cout << "   生成的临近测试用例 " << near_testcase << endl;
		//// cout << "  错误测试用例 " << fault_exp << endl;
		bool check_if_true = uti.evaluateMutant(exp,fault_exp, near_testcase);
		//// cout << " 检测结果 " << check_if_true << endl;
		if (!check_if_true) continue;  //取0，则不是该项发生错误，直接跳过
		//已经定位出唯一缺失项
		unique_out_literals.push_back(possible_out_literal);
		break;
	}
	if (unique_out_literals.size() != 0){
		//定位出一个缺失，生成LRF
		vector<string>res_items = generate_LRFmutant_inPos(exp, utp, insert_literal, unique_out_literals);
		for (string res_item : res_items){
			//// cout << "生成几种LRF："<<res_items.size() << endl;
			res.push_back(res_item);
		}
	}
	else{
		//未定位出缺失，生成备用集的LRF和LIF
		vector<string>res_items = generate_LRFmutant_inPos(exp, utp, insert_literal, backup_out_literals);
		for (string res_item : res_items){
			res.push_back(res_item);
		}
		res_items = generate_LIFmutant_inPos(exp, utp, insert_literal);
		for (string res_item : res_items){
			res.push_back(res_item);
		}
	}
	return res;
}

vector<string> getUTC(string expression, string term, vector<string> true_testset){
	Utility uti;
	string model = uti.generateModel(expression);
	vector<string> res;
	for (string testcase : true_testset){
		string tc = "";
		vector<string> literals = uti.extractCNFstyle(uti.sortTerm(term));
		for (string li : literals){
			char l = li.at(0) == '!' ? li.at(1) : li.at(0);
			tc += testcase.at(model.find(l));
		}
		if (uti.evaluate(term, tc)){
			res.push_back(testcase);
		}
	}
	sort(res.begin(), res.end());
	return res;
}
//只有收缩点分支的主干程序,适用Rule6更新版，exp为待测表达式，testset为待测点集，此处为真点集
vector<string> judgeAllShrink_ByRules(string exp, string fault_exp, vector<string> testset, vector<string> all_testset){
	DnfMutant dnfmutant;
	vector<string> res;
	Utility uti;
	vector<string> Tfail1;
	vector<string> Tmin1 = testset;
	for (string testcase : testset){
		bool diff = uti.checkTestDiffer(testcase, exp, fault_exp);
		if (diff){
			Tfail1.push_back(testcase);
		}
	}
	sort(Tfail1.begin(), Tfail1.end());
	vector<string> terms = uti.extractDNFstyle(exp);
	bool isTOF = false;
	bool isDORF = false;
	for (string term : terms){
		vector<string> utcs = getUTC(exp, term, Tmin1);
		if (getUTC(exp, term, Tmin1) == Tfail1){
			//生成TOF
			isTOF = true;
			res.push_back(dnfmutant.generateTOFmutants(exp, term));
			break;
		}
	}
	if (isTOF){

	}
	if (!isTOF){
		for (int i = 0; i < terms.size(); i++){
			for (int j = i + 1; j < terms.size(); j++){
				string term1 = terms[i];
				string term2 = terms[j];
				vector<string> utc1 = getUTC(exp, term1, Tmin1);
				vector<string> utc2 = getUTC(exp, term2, Tmin1);
				for (string utc : utc2){
					utc1.push_back(utc);
				}
				sort(utc1.begin(), utc1.end());
				if (utc1 == Tfail1){
					//生成DORF
					isDORF = true;
					res.push_back(dnfmutant.generateDORFmutants(exp, term1, term2));
				}
			}
		}
	}
	//非本项错误 LIF LRF
	if (isTOF || !isDORF){
		string model = uti.generateModel(exp);
		unordered_map<char, int> literal_abstract_pos;  //全部变量集对应位置关系
		for (int i = 0; i < model.size(); i++)
		{
			literal_abstract_pos.insert(pair<char, int>{ model[i], i });
		}

		for (string testset_item : Tfail1){
			TestCase testcase = TestCase(exp, testset_item, true);
			list<UTP> utps = testcase.getUTP(exp, testset_item);
			UTP utp = *utps.begin(); //唯一真点
			list<Literal> valid_literals = utp.getValid();
			bool is_sensitive = 1;  //判断当前词是否是敏感点，取1为敏感点
			vector<Literal> backup_insert_set;
			vector<Literal> insert_set;
			bool check_if_outside = false;
			for (Literal literal : valid_literals)
			{
				is_sensitive = (testcase.susceptibleLiterSet.find(literal) != testcase.susceptibleLiterSet.end());
				if (is_sensitive)
				{
					//是敏感变量
					//// cout << "是敏感变量" << literal.getChar() << literal.getPos() << endl;
					backup_insert_set.push_back(literal);
					continue;
				}
				vector<char> change_literal;
				change_literal.push_back(literal.getChar()); //加入待修改变量集合
				//生成临近测试用例，检测真假值
				string near_testcase = generateNearTestset(testset_item, change_literal, literal_abstract_pos); //临近法求新测试用例
				check_if_outside = uti.evaluateMutant(exp, fault_exp, near_testcase);
				if (check_if_outside)
				{
					//定位出有效插入项，带着有效插入项进入outside
					insert_set.push_back(literal);
					break;
				}
			}
			if (check_if_outside) //定位出理想插入项
			{
				//外部错误
				vector<string> res_items1 = fault_in_Outside(exp, fault_exp, testcase, utp, literal_abstract_pos, insert_set);
				for (string res_item : res_items1)
				{
					res.push_back(res_item);
				}
			}
			else	//没有定位出最佳有效插入项，带备用插入项走outside的同时，也要检测inside
			{
				if (backup_insert_set.size() != 0)
				{
					//外部错误
					vector<string> res_items1 = fault_in_Outside(exp, fault_exp, testcase, utp, literal_abstract_pos, backup_insert_set);
					for (string res_item : res_items1)
					{
						res.push_back(res_item);
					}
				}
			}
			preprocess_interval += testcase.time_interval;
		}
	}

	return res;

}

//只有收缩点分支的主干程序，exp为待测表达式，testset为待测点集，此处为真点集
//新版实现，在主干程序中即定位出插入词集合
vector<string> judgeAllShrink(string exp, string fault_exp, vector<string> testset, vector<string> all_testset)
{
	vector<string> res;
	Utility uti;
	string model = uti.generateModel(exp); //全部变量集abcd
	unordered_map<char, int> literal_abstract_pos;  //全部变量集对应位置关系
	for (int i = 0; i < model.size(); i++)
	{
		literal_abstract_pos.insert(pair<char, int>{ model[i], i });
	}
	//按照数据结构的设计，对待测点情况进行重新刻画
	//逐项排查测试用例
	int useful_testcase_count=0; //有效测试用例的个数
	for (string testset_item : testset)
	{
		bool is_vaild = uti.checkTestDiffer(testset_item, exp, fault_exp);
		if (is_vaild == false) continue;  //跳过无效测试用例
		else useful_testcase_count += 1;
		TestCase testcase = TestCase(exp, testset_item, true);
		//对每个测试用例中的UTP进行排查，对于纯扩张函数，这里是UTP
		list<UTP> utps = testcase.getUTP(exp, testset_item);
		UTP utp = *utps.begin(); //唯一真点
		//// cout << "……当前utp：" << utp.term << endl;
		//// cout << "测试用例：" << testcase.testcase << endl;
		//for (Literal literal1 : testcase.susceptibleLiterSet)
		//{
		//	// cout << "敏感变量：" << literal1.getChar() << endl;
		//}
		list<Literal> valid_literals = utp.getValid();
		bool is_sensitive = 1;  //判断当前词是否是敏感点，取1为敏感点
		bool check_if_outside=false; //检测是否可以定位出插入项，
		//逐个词判断是否是敏感点
		vector<Literal> insert_set;
		vector<Literal> backup_insert_set;
		for (Literal literal : valid_literals)
		{
			is_sensitive = (testcase.susceptibleLiterSet.find(literal) != testcase.susceptibleLiterSet.end());
			if (is_sensitive)
			{
				//是敏感变量
				//// cout << "是敏感变量" << literal.getChar() << literal.getPos() << endl;
				backup_insert_set.push_back(literal);
				continue;
			}
			//// cout << "有效插入 " << literal.getChar() << literal.getPos() << endl;
			vector<char> change_literal;
			change_literal.push_back(literal.getChar()); //加入待修改变量集合
			//生成临近测试用例，检测真假值
			string near_testcase = generateNearTestset(testset_item, change_literal, literal_abstract_pos); //临近法求新测试用例
			check_if_outside = uti.evaluateMutant(exp, fault_exp, near_testcase);
			if (check_if_outside)
			{
				//定位出有效插入项，带着有效插入项进入outside
				insert_set.push_back(literal);
				break;
			}
			preprocess_interval += testcase.time_interval;
		}
		if (check_if_outside) //定位出理想插入项
		{
			//外部错误
			vector<string> res_items1 = fault_in_Outside(exp, fault_exp, testcase, utp, literal_abstract_pos,insert_set);
			for (string res_item : res_items1)
			{
				//检测结果复核
				if (checkResultType(exp, res_item, all_testset) != 2) continue;
				//// cout << "……当前utp：" << utp.term << " 针对该项的检测结果: " << "【" << res_item << "】" << endl;
				res.push_back(res_item);
			}
		}
		else	//没有定位出最佳有效插入项，带备用插入项走outside的同时，也要检测inside
		{
			if (backup_insert_set.size()!=0)
			{
				//外部错误
				vector<string> res_items1 = fault_in_Outside(exp, fault_exp, testcase, utp, literal_abstract_pos, backup_insert_set);
				for (string res_item : res_items1)
				{
					if (checkResultType(exp, res_item, all_testset) != 2) continue;
					//// cout << "……当前utp：" << utp.term << " 针对该项的检测结果: " << "【" << res_item << "】" << endl;
					res.push_back(res_item);
				}
			}
			//内部错误
			vector<string> res_items2 = fault_in_Inside(exp, fault_exp, testcase, utp, literal_abstract_pos);
			// cout << "TOF\DORF分支输出结果集规模：" << res_items2.size() << endl;
			for (string res_item : res_items2)
			{
				//if (checkResultType(exp, res_item, all_testset) != 2) continue;
				// cout << "……当前utp：" << utp.term << " 针对该项的检测结果: " << "【" << res_item << "】" << endl;
				res.push_back(res_item);
			}
		}
		preprocess_interval += testcase.time_interval;
	}
	//// cout << "最终结果: " << endl;
	map<string, int> res_map;
	map<string, int>::iterator iter;
	for (string res_item : res)
	{
		iter = res_map.find(res_item);
		if (iter == res_map.end()) res_map[res_item] = 1;
		else res_map[res_item] = res_map[res_item] + 1;
		//使用一个map，统计每个结果被检测出来的次数，只输出每轮都被检测出来的结果
	}
	vector<string> res_new;
	for (iter = res_map.begin(); iter != res_map.end(); iter++){
		//if (iter->second != useful_testcase_count) continue;
		res_new.push_back(iter->first);
		//// cout << "【" << iter->first << "】" << endl;
	}
	return res;
}

//非词错误（项以上错误）次主干程序

vector<string> fault_in_Term(string exp, string fault_exp, vector<string> true_testset, vector<string> false_testset, vector<string> all_testset){
	vector<string> res;
	Utility uti;
	DnfMutant dnfmutant;
	bool all_valid = true;
	// cout << "进入项错误" << endl;
	for (int i = 0; i < true_testset.size(); ++i)
	{
		all_valid = all_valid&uti.checkTestDiffer(true_testset[i], exp, fault_exp);
	}
	if (all_valid){
		//ENF
		//输出ENF结果
		string mutant = dnfmutant.generateENFmutant(exp);
		res.push_back(mutant);
		// cout << " ENF---> " << mutant << endl;
	}
	else{
		// cout << "进入项错误的TNF定位" << endl;
		//TNF
		//定位TNF项，输出TNF结果
		for (string point : true_testset){
			if (uti.checkTestDiffer(point, exp, fault_exp)){
				TestCase tc = TestCase(exp, point, true);
				list<UTP> utps = tc.getUTP(exp, point);
				for (UTP utp : utps){
					vector<string> mutants = dnfmutant.generateTNFmutant(exp, utp);
					for (string mutant : mutants)
						res.push_back(mutant);

				}
				preprocess_interval += tc.time_interval;
				break;
			}
		}

	}
	return res;
}
//词错误：LNF有效收缩检测

vector<string> fault_in_Literal_LNF_OLD(string exp, string fault_exp, TestCase testcase, UTP utp, unordered_map<char, int> literal_abstract_pos)
{
	vector<string> res;
	//vector<string> sesusceptible_res;
	//// cout << "本项错误检测" << endl;
	DnfMutant dnf_mutant;
	Utility uti;
	list<Literal> valid = utp.getInner();
	list<Literal>::iterator plist;
	vector<Literal> backup_not_set;
	for (plist = valid.begin(); plist != valid.end(); plist++)
	{
		//检查当前元素是否是敏感变量，否，则，生成临近测试用例检测
		//是，则插入涉敏感集备用返回集
		if (testcase.susceptibleLiterSet.find(*plist) == testcase.susceptibleLiterSet.end())
		{
			//生成临近测试用例测试，有效则将re加入结果集
			vector<char> change_literal;
			change_literal.push_back((*plist).getChar());
			string near_testcase = generateNearTestset(testcase.testcase, change_literal, literal_abstract_pos); //临近法求新测试用例
			//// cout << "   改变的变量 " << (*plist).getChar() << endl;
			//// cout << "   【本项内错误】单变量临近测试用例 " << near_testcase << endl;
			bool check_if_true = uti.evaluate(fault_exp, near_testcase);
			if (!check_if_true) continue;  //取0跳过，取1则加入唯一取反集
			//取反项定位成功，生成LNF
			//注意：generateLNFmutant_inPos未实现
			string re = dnf_mutant.generateLNFmutant_inPos(exp, utp, *plist);
			res.push_back(re);
			//// cout << re << endl;
			break;
		}
		else   //是，生成所有可能的取反情况
		{
			backup_not_set.push_back(*plist);

		}

	}
	if (res.size() != 0) return res;
	else
	{
		//进入TOF和DORF辨析
		if (backup_not_set.size() != 0)
		{
			for (Literal not_literal : backup_not_set)
			{
				string re = dnf_mutant.generateLNFmutant_inPos(exp, utp, not_literal);
				res.push_back(re);
				//// cout << re << endl;
			}
		}
		for (Literal l : valid){
			if (testcase.susceptibleLiterSet.find(l) == testcase.susceptibleLiterSet.end()){
				vector<string> re1 = generate_TOForDORF_mutant_inPos(exp, fault_exp, testcase, utp, l);
				for (string rei : re1)
				{
					res.push_back(rei);
					//// cout << rei << endl;
				}
				//break;
			}
		}

		return res;
	}
}

vector<string> fault_in_Literal_LNF(string exp, string fault_exp, TestCase testcase, UTP utp, unordered_map<char, int> literal_abstract_pos, vector<string> false_testset)
{
	//// cout << "词错误LNF检测" << endl;
	Utility uti;
	DnfMutant dnfmutant;
	vector<string> res;
	//遍历假点集，找到有效扩张点
	for (string point : false_testset){
		if (uti.checkTestDiffer(point, exp, fault_exp)){
			TestCase tc = TestCase(exp, point, false);
			list<NFP> nfps = tc.getNFP(exp, point);
			//得到有效扩张点的NFP
			//与UTP取交集
			//得到的LNF结果每次都取交集
			for (NFP nfp : nfps){
				if (nfp.term == utp.term){
					string mu = dnfmutant.generateLNFmutant_inPos(exp, utp, nfp.getKey());
					if (res.size() == 0) res.push_back(mu);
					else{
						for (string re : res){
							vector<string> tmp;
							if (re == mu){
								tmp.push_back(mu);
							}
							res = tmp;
						}
					}
				}
			}

		}
	}
	for (string re : res){
		//// cout << "LNF--->" << re << endl;
	}
	return res;
}

//词错误：LRF有效收缩加测

vector<string> fault_in_Literal_LRF(string exp, string fault_exp, TestCase testcase, UTP utp, unordered_map<char, int> literal_abstract_pos, vector<Literal> insert_literal, vector<string> false_testset)
{
	//// cout << "非本项错误检测" << endl;
	vector<string>  res;
	Utility uti;
	for (string testset_item:false_testset)
	{
		//先检查有效性，跳过无效测试用例
		bool valid=uti.checkTestDiffer(testset_item, exp, fault_exp);
		if (!valid) continue;
		TestCase false_testcase = TestCase(exp,testset_item,false);
		list<NFP> nfps = false_testcase.getNFP(exp, testset_item);
		for (NFP nfp : nfps)
		{
			if (nfp.term != utp.term) continue;
			//找到被替换词，直接生成LRF
			vector<Literal> out_literal{ nfp.getKey() };
			vector<string>res_items = generate_LRFmutant_inPos(exp, utp, insert_literal, out_literal);
			for (string res_item : res_items){
				res.push_back(res_item);
			}
			break;
		}
	}
	return res;
}

//词错误次主干程序
vector<string> fault_in_Literal(string exp, string fault_exp, vector<string> true_testset, vector<string> false_testset, vector<string> all_testset){
	vector<string> res;
	//// cout << " 词错误次主干 " << endl;
	Utility uti;
	string model = uti.generateModel(exp); //全部变量集abcd
	unordered_map<char, int> literal_abstract_pos;  //全部变量集对应位置关系
	for (int i = 0; i < model.size(); i++)
	{
		literal_abstract_pos.insert(pair<char, int>{ model[i], i });
	}
	//按照数据结构的设计，对待测点情况进行重新刻画
	//逐项排查测试用例
	int useful_testcase_count = 0; //有效测试用例的个数
	for (string testset_item : true_testset)
	{
		bool is_vaild = uti.checkTestDiffer(testset_item, exp, fault_exp);
		if (is_vaild == false) continue;  //跳过无效测试用例
		else useful_testcase_count += 1;
		TestCase testcase = TestCase(exp, testset_item, true);
		//对每个测试用例中的UTP进行排查，对于纯扩张函数，这里是UTP
		list<UTP> utps = testcase.getUTP(exp, testset_item);
		UTP utp = *utps.begin(); //唯一真点
		//// cout << "……当前utp：" << utp.term << endl;
		list<Literal> valid_literals = utp.getValid();
		bool is_sensitive = 1;  //判断当前词是否是敏感点，取1为敏感点
		bool check_if_outside = false; //检测是否可以定位出插入项，
		//逐个词判断是否是敏感点
		vector<Literal> insert_set;
		vector<Literal> backup_insert_set;
		for (Literal literal : valid_literals)
		{
			is_sensitive = (testcase.susceptibleLiterSet.find(literal) != testcase.susceptibleLiterSet.end());
			if (is_sensitive)
			{
				//是敏感变量
				//// cout << "是敏感变量" << literal.getChar() << literal.getPos() << endl;
				backup_insert_set.push_back(literal);
				continue;
			}
			//// cout << "有效插入 " << literal.getChar() << literal.getPos() << endl;
			vector<char> change_literal;
			change_literal.push_back(literal.getChar()); //加入待修改变量集合
			//生成临近测试用例，检测真假值
			string near_testcase = generateNearTestset(testset_item, change_literal, literal_abstract_pos); //临近法求新测试用例
			check_if_outside = uti.evaluateMutant(exp, fault_exp, near_testcase);
			if (check_if_outside)
			{
				//定位出有效插入项，带着有效插入项进入outside
				insert_set.push_back(literal);
				break;
			}
			preprocess_interval += testcase.time_interval;
		}
		if (check_if_outside) //定位出理想插入项
		{
			//LRF收缩解析+扩张解析
			vector<string> res_items1 = fault_in_Literal_LRF(exp, fault_exp, testcase, utp, literal_abstract_pos, insert_set,false_testset);
			for (string res_item : res_items1)
			{
				//检测结果复核
				if (checkResultType(exp, res_item, all_testset) != 3) continue;
				//// cout << "……当前utp：" << utp.term << " 针对该项的检测结果: " << "【" << res_item << "】" << endl;
				res.push_back(res_item);
			}
		}
		else	//没有定位出最佳有效插入项，带备用插入项走outside的同时，也要检测inside
		{
			if (backup_insert_set.size() != 0)
			{
				//LRF收缩解析+扩张解析
				vector<string> res_items1 = fault_in_Literal_LRF(exp, fault_exp, testcase, utp, literal_abstract_pos, backup_insert_set,false_testset);
				for (string res_item : res_items1)
				{
					if (checkResultType(exp, res_item, all_testset) != 3) continue;
					//// cout << "……当前utp：" << utp.term << " 针对该项的检测结果: " << "【" << res_item << "】" << endl;
					res.push_back(res_item);
				}
			}
			//LNF收缩解析+扩张解析
			vector<string> res_items2 = fault_in_Literal_LNF(exp, fault_exp, testcase, utp, literal_abstract_pos, false_testset);
			for (string res_item : res_items2)
			{
				if (checkResultType(exp, res_item, all_testset) != 3) continue;
				//// cout << "……当前utp：" << utp.term << " 针对该项的检测结果: " << "【" << res_item << "】" << endl;
				res.push_back(res_item);
			}
		}
	}
	//// cout << "最终结果: " << endl;
	map<string, int> res_map;
	map<string, int>::iterator iter;
	for (string res_item : res)
	{
		iter = res_map.find(res_item);
		if (iter == res_map.end()) res_map[res_item] = 1;
		else res_map[res_item] = res_map[res_item] + 1;
		//使用一个map，统计每个结果被检测出来的次数，只输出每轮都被检测出来的结果
	}
	vector<string> res_new;
	for (iter = res_map.begin(); iter != res_map.end(); iter++){
		if (iter->second != useful_testcase_count) continue;
		res_new.push_back(iter->first);
		//// cout << "【" << iter->first << "】" << endl;
	}
	return res;
}

//收扩复合型主干程序
vector<string> judgeShrinkAndExpand(string exp, string fault_exp, vector<string> true_testset, vector<string> false_testset, vector<string> all_testset){
	vector<string> res;
	//首先遍历假点集，检查有效扩张的个数，如果有效扩张个数小于假点集个数，转LRF、LNF
	//否则，转TNF、ENF
	Utility uti;
	bool all_valid = true;
	for (int i = 0; i < false_testset.size(); ++i)
	{
		all_valid = all_valid&uti.checkTestDiffer(false_testset[i], exp, fault_exp);
	}
	if (all_valid){
		//项以上错误，即ENF、TNF
		vector<string> res_items1 = fault_in_Term(exp, fault_exp, true_testset, false_testset, all_testset);
		for (string res_item : res_items1)
		{
			//检测结果复核
			if (checkResultType(exp, res_item, all_testset) != 3) continue;
			//// cout << " 项以上错误检测结果: " << "【" << res_item << "】" << endl;
			res.push_back(res_item);
		}
	}
	else{
		//词错误，即LRF、LNF
		vector<string> res_items1 = fault_in_Literal(exp, fault_exp, true_testset, false_testset,all_testset);
		for (string res_item : res_items1)
		{
			//检测结果复核
			if (checkResultType(exp, res_item, all_testset) != 3) continue;
			//// cout <<" 词错误检测结果: " << "【" << res_item << "】" << endl;
			res.push_back(res_item);
		}
	}
	return res;
}
//大类拆分函数
void extractFalseType(string expression, vector<string>testset)
{
	//在这里加入大类拆分功能
	/*
	找功能函数:测试用例代入表达式得到表达式值  bool Utility::evaluate(string exp,string testString)
	找/写：真假点分类函数
	找：辨别测试集是否全通过表达式检测的函数
	实现：对层次关系图进行剪枝
	*/
	// cout << "………………………………………………………………" << endl;
	// cout << "请输入待测表达式" << endl;
	string false_expression;
	while (cin >> false_expression)
	{
		Utility uti;
		// << "请输入待测表达式" << endl;
		//false_expression = "a+cd+b!c";
		//根据测试用例代入原表达式的取值将测试用例分为真点集和假点集
		vector<string> true_testset;
		vector<string> false_testset;
		for (int i = 0; i < testset.size(); ++i)
		{
			if (uti.evaluate(expression, testset[i]) == true) true_testset.push_back(testset[i]);
			else false_testset.push_back(testset[i]);
		}
		/*// cout << "分类后的真点" << endl;
		for (int i = 0; i < true_testset.size(); ++i){
		// cout << true_testset[i] << endl;
		}*/
		//使用真点集和假点集分别对待测表达式进行检测，判断是否全部通过检测，以此作为大类拆分的起点
		bool isAllPassed_T = isAllPassedTest(expression, false_expression, true_testset);
		bool isAllPassed_F = isAllPassedTest(expression, false_expression, false_testset);
		if (isAllPassed_T && isAllPassed_F)
		{
			// cout << "最小集表现与原表达式一致，通过检测" << endl;
			return;
		}
		vector<string> res;
		if (isAllPassed_T == true)
		{
			// cout << "只有扩张点" << endl;
			if(algo_mode==true) res=judgeAllExpand(expression, false_expression, false_testset,testset);
			//judgeAllExpand_ByLevel(expression, false_expression, false_testset, testset);
			else res=judgeAllExpand_ByRule6(expression, false_expression, false_testset, testset);
		}
		else if (isAllPassed_F == true)
		{
			// cout << "只有收缩点" << endl;
			if (algo_mode == true) res = judgeAllShrink(expression, false_expression, true_testset, testset);
			else res = judgeAllShrink_ByRules(expression, false_expression, true_testset, testset);
		}
		else
		{
			// cout << "既有收缩点也有扩张点" << endl;
			res=judgeShrinkAndExpand(expression, false_expression, true_testset, false_testset,testset);
		}
		//去重
		set<string>s(res.begin(), res.end());
		res.assign(s.begin(), s.end());
		// cout << "【最终结果】: " << endl;
		for (string res_item : res){
			// cout << "     【" << res_item << "】" << endl;
		}
		// cout << "【本轮检测临近测试用例生成次数】: " << call_generateNearTestset_times << endl;
		call_generateNearTestset_times = 0;
		// cout<< "………………………………………………………………" << endl;
		// cout<< "请输入待测表达式" << endl;
	}
}

//输入原表达式和待测表达式，输出检测结果
vector<string> ordinaryAlgoTestOne(string expression, vector<string>testset, string false_expression){
	Utility uti;
	vector<string> res;
	//依次检测，得到待测表达式的特征向量
	string magnitude = string();
	for (int i = 0; i < testset.size(); ++i)
	{
		if (uti.evaluateMutant(expression, false_expression, testset[i]) == true) magnitude.push_back(1);
		else magnitude.push_back(0);
	}
	//生成当前表达式的所有变体，对每个变体生成特征向量，与待测表达式一致的加入结果集
	Mutants mutants1(expression);
	DnfMutant dnf;
	mutants1 = dnf.getMutants(expression, "SALL");
	for (int i = 0; i < mutants1.mutants.size(); i++){
		string maybe_fault_expression = mutants1.mutants[i].mutant;
		string cur_magnitude = string();
		for (int i = 0; i < testset.size(); ++i)
		{
			if (uti.evaluateMutant(expression, maybe_fault_expression, testset[i]) == true) cur_magnitude.push_back(1);
			else cur_magnitude.push_back(0);
		}
		if (cur_magnitude == magnitude){
			res.push_back(maybe_fault_expression);
		}
	}
	return res;
}

//蛮力算法，单测
void ordinaryAlgo(string expression, vector<string>testset)
{
	//生成全部变体
	//对每个变体调用测试专用函数
	// cout << "………………………………………………………………" << endl;
	// cout << "请输入待测表达式" << endl;
	string false_expression;
	while (cin >> false_expression)
	{
		vector<string> res;
		res = ordinaryAlgoTestOne(expression, testset, false_expression);
		//去重
		set<string>s(res.begin(), res.end());
		res.assign(s.begin(), s.end());
		// cout << "【最终结果】: " << endl;
		for (string res_item : res){
			// cout << "     【" << res_item << "】" << endl;
		}
		// cout << "【本轮检测临近测试用例生成次数】: " << call_generateNearTestset_times << endl;
		call_generateNearTestset_times = 0;
		// cout << "………………………………………………………………" << endl;
		// cout << "请输入待测表达式" << endl;
	}

}

//测试专用数据结构

string filename = "test2";
string filename_old = filename;
string filename_new = filename + "\\byRule6";
string filename_ordinary = filename + "\\byOrdinary";

class Character_Of_Mutant
{
	public:
		string false_expression; //待测表达式
		string character; //纯收缩、纯扩张还是复合型
		vector<string> res; //结果集
		string false_type; //LRF、LOF等
		int generateNearTestset_times=0; //沿途生成临近测试用例的次数
		time_t time_interval; //检测当前错误用时时长
		time_t preprocess_interval; //预处理模块处理时长
		time_t detect_interval; //检测时长
		bool is_hit = false;
		bool get_is_hit()
		{
			for (string res_item : res)
			{
				if (res_item == false_expression){
					is_hit = true;
					break;
				}
			}
			return is_hit;
		}
};

class Exp_Mutants_Result{
	public:
		string exp;
		vector<Character_Of_Mutant> character_of_mutants;
		int origin_testcase_size=0; //原本的测试用例集大小
		int add_testcase_size=0; //新增测试用例集大小
		int all_testcase_size=0; //总测试用例集大小
		int avg_res_size = 0;//总平均结果集大小
		map<string,int> max_add_testcase_size;//每种错误需要扩展的测试用例数量的最大值
		map<string,int> min_add_testcase_size;//每种错误需要扩展的测试用例数量的最大值
		double avg_generateNearTestset_times=0;//所有变体作为待测表达式检测生成测试用例次数的平均值
		map<string, int> max_res_size;//每种错误得到的最大结果集大小
		map<string, int> min_res_size;//每种错误得到的最小结果集大小
		time_t avg_time_interval; //检测当前错误用时时长
		Exp_Mutants_Result(){
			max_add_testcase_size.insert({ "LIF", 0 });
			max_add_testcase_size.insert({ "LRF", 0 });
			max_add_testcase_size.insert({ "LOF", 0 });
			max_add_testcase_size.insert({ "LNF", 0 });
			max_add_testcase_size.insert({ "TOF", 0 });
			max_add_testcase_size.insert({ "TNF", 0 });
			max_add_testcase_size.insert({ "ENF", 0 });
			max_add_testcase_size.insert({ "CORF", 0 });
			max_add_testcase_size.insert({ "DORF", 0 });
			min_add_testcase_size.insert({ "LIF", INT_MAX });
			min_add_testcase_size.insert({ "LRF", INT_MAX });
			min_add_testcase_size.insert({ "LOF", INT_MAX });
			min_add_testcase_size.insert({ "LNF", INT_MAX });
			min_add_testcase_size.insert({ "TOF", INT_MAX });
			min_add_testcase_size.insert({ "TNF", INT_MAX });
			min_add_testcase_size.insert({ "ENF", INT_MAX });
			min_add_testcase_size.insert({ "CORF", INT_MAX });
			min_add_testcase_size.insert({ "DORF", INT_MAX });
			max_res_size.insert({ "LIF", 0 });
			max_res_size.insert({ "LRF", 0 });
			max_res_size.insert({ "LOF", 0 });
			max_res_size.insert({ "LNF", 0 });
			max_res_size.insert({ "TOF", 0 });
			max_res_size.insert({ "TNF", 0 });
			max_res_size.insert({ "ENF", 0 });
			max_res_size.insert({ "CORF", 0 });
			max_res_size.insert({ "DORF", 0 });
			min_res_size.insert({ "LIF", INT_MAX });
			min_res_size.insert({ "LRF", INT_MAX });
			min_res_size.insert({ "LOF", INT_MAX });
			min_res_size.insert({ "LNF", INT_MAX });
			min_res_size.insert({ "TOF", INT_MAX });
			min_res_size.insert({ "TNF", INT_MAX });
			min_res_size.insert({ "ENF", INT_MAX });
			min_res_size.insert({ "CORF", INT_MAX });
			min_res_size.insert({ "DORF", INT_MAX });
		}
		
		void generateAvgTimeInterval(){
			time_t sum = 0;
			for (Character_Of_Mutant character_of_mutant : character_of_mutants){
				cout << character_of_mutant.time_interval<<endl;
				sum += character_of_mutant.time_interval;
			}
			if (character_of_mutants.size() != 0){
				avg_time_interval = sum / character_of_mutants.size();
			}
			else{
				avg_res_size = -1;
			}
		}
		void generateResMessage(){
			for (Character_Of_Mutant character_of_mutant : character_of_mutants){
				//更新每种错误调用次数最大值
				if (max_res_size[character_of_mutant.false_type]<character_of_mutant.res.size())
					max_res_size[character_of_mutant.false_type] = character_of_mutant.res.size();
				//更新每种错误调用次数最小值
				if (min_res_size[character_of_mutant.false_type]>character_of_mutant.res.size())
					min_res_size[character_of_mutant.false_type] = character_of_mutant.res.size();
			}
		}

};

Exp_Mutants_Result exp_mutants_result;
//大类拆分函数,测试专用
Character_Of_Mutant extractFalseType_ForTest(string expression, vector<string>testset, string false_expression) 
{
	//在这里加入大类拆分功能
	/*
	找功能函数:测试用例代入表达式得到表达式值  bool Utility::evaluate(string exp,string testString)
	找/写：真假点分类函数
	找：辨别测试集是否全通过表达式检测的函数
	实现：对层次关系图进行剪枝
	*/
		Character_Of_Mutant character_of_mutant;
		timeb start;
		ftime(&start);
		character_of_mutant.false_expression = false_expression;
		Utility uti;
		// << "请输入待测表达式" << endl;
		//false_expression = "a+cd+b!c";
		//根据测试用例代入原表达式的取值将测试用例分为真点集和假点集
		vector<string> true_testset;
		vector<string> false_testset;
		for (int i = 0; i < testset.size(); ++i)
		{
			if (uti.evaluate(expression, testset[i]) == true) true_testset.push_back(testset[i]);
			else false_testset.push_back(testset[i]);
		}
		/*// cout << "分类后的真点" << endl;
		for (int i = 0; i < true_testset.size(); ++i){
		// cout << true_testset[i] << endl;
		}*/
		//使用真点集和假点集分别对待测表达式进行检测，判断是否全部通过检测，以此作为大类拆分的起点
		bool isAllPassed_T = isAllPassedTest(expression, false_expression, true_testset);
		bool isAllPassed_F = isAllPassedTest(expression, false_expression, false_testset);
		if ((isAllPassed_T || true_testset.size() == 0) &&(isAllPassed_F ||false_testset.size() == 0))
		{
			// cout << "最小集表现与原表达式一致，通过检测" << endl;
			character_of_mutant.character = "最小集表现与原表达式一致，通过检测";
		}
		else if (isAllPassed_T == true)
		{
			// cout << "只有扩张点" << endl;
			//judgeAllExpand(expression, false_expression, false_testset,testset);
			character_of_mutant.character = "纯扩张";
			if (algo_mode) character_of_mutant.res = judgeAllExpand(expression, false_expression, false_testset, testset);
			else character_of_mutant.res = judgeAllExpand_ByRule6(expression, false_expression, false_testset, testset);
		}
		else if (isAllPassed_F == true)
		{
			// cout << "只有收缩点" << endl;
			character_of_mutant.character = "纯收缩";
			if (algo_mode) character_of_mutant.res = judgeAllShrink(expression, false_expression, true_testset, testset);
			else character_of_mutant.res = judgeAllShrink_ByRules(expression, false_expression, true_testset, testset);
		}
		else
		{
			// cout << "既有收缩点也有扩张点" << endl;
			character_of_mutant.character = "收扩复合型";
			character_of_mutant.res = judgeShrinkAndExpand(expression, false_expression, true_testset,false_testset,testset);
		}
		//各项数据后处理
		timeb end;
		ftime(&end);
		cout << "time_start" << start.time * 1000 + start.millitm << endl;
		cout << "time_end" << end.time * 1000 + end.millitm << endl;
		character_of_mutant.time_interval = end.time * 1000 + end.millitm - (start.time * 1000 + start.millitm);
		character_of_mutant.preprocess_interval = preprocess_interval;
		character_of_mutant.detect_interval = character_of_mutant.time_interval - character_of_mutant.preprocess_interval;
		preprocess_interval = 0;
		character_of_mutant.generateNearTestset_times = call_generateNearTestset_times;
		call_generateNearTestset_times = 0;
		return character_of_mutant;
}

//蛮力算法，测试专用
Character_Of_Mutant ordinaryAlgo_ForTest(string expression, vector<string>testset, string false_expression){
	Character_Of_Mutant character_of_mutant;
	timeb start;
	ftime(&start);
	character_of_mutant.false_expression = false_expression;
	character_of_mutant.character = "蛮力算法";
	character_of_mutant.res = ordinaryAlgoTestOne(expression, testset, false_expression);
	//各项数据后处理
	timeb end;
	ftime(&end);
	cout << "time_start" << start.time * 1000 + start.millitm << endl;
	cout << "time_end" << end.time * 1000 + end.millitm << endl;
	character_of_mutant.time_interval = end.time * 1000 + end.millitm - (start.time * 1000 + start.millitm);
	character_of_mutant.generateNearTestset_times = call_generateNearTestset_times;
	call_generateNearTestset_times = 0;
	return character_of_mutant;
}

int ti;
double TestNewAlgo(string expression, vector<string> testset)
{
	/*
	写出一个文件：文件名为输入表达式_NearTestResult，
	纯扩张变体表达式+结果集大小+是否命中

	写出一个总文件：文件名为25all__NearTestResult
	内容为：原表达式+命中率
	25个表达式平均命中率
	*/
	for (string testcase : testset){
		near_testcase_set.insert(testcase);
	}
	exp_mutants_result.origin_testcase_size = testset.size();
	ofstream oFile;
	string path = "E:\\FaultTable\\" + filename + "\\" + to_string(ti) + "_" + expression.substr(0,15) + "_NearTestResult.csv";
	oFile.open(path);
	oFile << "原表达式" << "," << expression << endl;
	//从文件中读入原表达式
	//对表达式生成所有LRF型变体
	vector<Character_Of_Mutant> test_new_algo;
	Mutants mutants1(expression);
	DnfMutant dnf;
	mutants1 = dnf.getMutants(expression, "SALL");
	cout << mutants1.mutants.size() << endl;
	//return 0;
	for (int i = 0; i < mutants1.mutants.size(); i++){
		//if (mutants1.mutants[i].type == "TOF" || mutants1.mutants[i].type == "DORF"){
			string fault_expression = mutants1.mutants[i].mutant;
			try{
				Character_Of_Mutant character_of_mutant;// = extractFalseType_ForTest(expression, testset, fault_expression);
				if (filename == filename_ordinary){
					character_of_mutant =ordinaryAlgo_ForTest(expression, testset, fault_expression);
				}
				else {
					character_of_mutant = extractFalseType_ForTest(expression, testset, fault_expression);
				}
				character_of_mutant.false_type = mutants1.mutants[i].type;
				character_of_mutant.get_is_hit(); //计算是否命中
				//更新每种错误调用次数最大值
				if (exp_mutants_result.max_add_testcase_size[character_of_mutant.false_type]<character_of_mutant.generateNearTestset_times)
					exp_mutants_result.max_add_testcase_size[character_of_mutant.false_type] = character_of_mutant.generateNearTestset_times;
				//更新每种错误调用次数最小值
				if (exp_mutants_result.min_add_testcase_size[character_of_mutant.false_type]>character_of_mutant.generateNearTestset_times)
					exp_mutants_result.min_add_testcase_size[character_of_mutant.false_type] = character_of_mutant.generateNearTestset_times;
				test_new_algo.push_back(character_of_mutant);
				exp_mutants_result.avg_generateNearTestset_times += character_of_mutant.generateNearTestset_times;
				exp_mutants_result.avg_res_size += character_of_mutant.res.size();
			}
			catch (...)
			{
				oFile << "变体表达式" << "," << fault_expression << "," << "执行异常" << "," << endl;
				continue; 
			}
			//if(i>=4) break;//每个变量只跑五条
		//}
	}
	exp_mutants_result.avg_generateNearTestset_times /= mutants1.mutants.size();
	exp_mutants_result.all_testcase_size = near_testcase_set.size();
	exp_mutants_result.add_testcase_size = exp_mutants_result.all_testcase_size - exp_mutants_result.origin_testcase_size;
	exp_mutants_result.avg_res_size /= mutants1.mutants.size();
	exp_mutants_result.character_of_mutants = test_new_algo;
	preprocess_interval = 0;
	near_testcase_set.clear();
	int mutant_count;//变体个数
	int all_expand_multant_count;//纯扩张变体个数
	int is_hit_count;//结果集命中变体个数
	oFile << "变体表达式" << "," << "结果集大小" << "," << "结果集是否命中待测点" << "," << "收扩类型" << "," << "错误类型" << "," << "生成临近测试用例个数" << "," << "检测时长（ms）" << "," << "预处理时长" << "," << "后处理时长" << endl;
	for (Character_Of_Mutant cha : test_new_algo)
	{
		//// cout << cha.false_expression << " : " << cha.character << " " << cha.res.size() << " 结果集是否命中待测表达式："<<cha.get_is_hit()<< endl;
		mutant_count += 1;
		if (true)//(cha.character == "只有扩张点")
		{
			bool is_hit=cha.is_hit;
			all_expand_multant_count += 1;
			if (is_hit || cha.character == "最小集表现与原表达式一致，通过检测") is_hit_count += 1;
			//向文件写出
			//oFile.open(path, ios::out | ios::trunc);
			oFile << cha.false_expression << "," << cha.res.size() << "," << is_hit << "," << cha.character << "," << cha.false_type << "," << cha.generateNearTestset_times << "," << cha.time_interval << "," << cha.preprocess_interval << "," << cha.detect_interval << endl;
		}
	}
	double hit_rate = ((double)is_hit_count / (double)all_expand_multant_count) * 100;
	//oFile.open(path, ios::out | ios::trunc);
	oFile << "总命中率" << "," << hit_rate << endl;
	return hit_rate;
}

//新算法函数的入口，前半部分和学长生成表格的相同，删去了生成表格,返回变体
double UseNearTestSingleFault(string type, string expression,string mode)
{
	Mutants mutants1(expression);
	DnfMutant dnf;
	int length;
	if (type == "single")
		mutants1 = dnf.getMutants(expression, "SALL");
	else
		mutants1 = dnf.getMutants(expression);
	length = mutants1.mutants.size();
	//------------------------------------------------------
	Utility uti;
	//分解表达式   保存表达式的utp otp fp 等等
	PRE_PROCESS_DATA preData; MutantsandValidInvalidTestCase;
	vector<string> terms = uti.extractDNFstyle(expression); //分解表达式的项
	string model = uti.generateModel(expression);  //model就是原表达式所包含的所有的字母(排好序的），例如 abce+cd+ef  abcdef
	string newExp = "";
	//得到IDNF
	//newExp = generateIDNForICNF(expression, true);
	//newExp = generateDNF(terms);
	newExp = expression;
	getPreProcessData(newExp, preData, true);
	vector<string> allPoint;
	CCommonFuncs commonFuncs;
	int literalNum = preData.literalCount;
	int dimensionNum = (literalNum + 1) / 2;
	bool vOdd = (literalNum % 2 == 0) ? false : true;
	//读取所有的OTP
	vector<string> op = preData.op;
	vector<int> allOPs;
	for (int i = 0; i < op.size(); i++)
	{
		int value = commonFuncs.getTModelIndexOf01Str(op.at(i), vOdd);
		allOPs.push_back(value);
	}

	sort(allOPs.begin(), allOPs.end());

	//读取所有的UTP·
	vector<int> allUPs;
	vector<string> up = preData.up;
	for (int i = 0; i < up.size(); i++)
	{
		int value = commonFuncs.getTModelIndexOf01Str(up.at(i), vOdd);
		allUPs.push_back(value);
	}
	sort(allUPs.begin(), allUPs.end());
	/**8888888888888888888888888888**/
	/*termsDimension 表示的是每一项的能使表达式值取1的取值，例如 ab+cd  model=“abcd” 没2个字母是一维
	11(第一项的 第一维的取值）
	00 01 11 10（第一项的其他维的取值）

	00 01 11 10（第二项的第一维的取值）
	11（第二项的其他维的取值）
	*/
	/*
	nanjia：这是一个或许可使用的数据结构，帮助我们构造敏感点、可替换项信息
	*/
	printf("\npreData.termsDimension:\n");
	for (int i = 0; i < preData.termsDimension.size(); i++){
		for (int j = 0; j < preData.termsDimension[i].size(); j++){
			printf("%s\n", preData.termsDimension[i][j].c_str());
		}
		printf("\n");
	}
	//层次关系map
	hash_map<string, HierarchyNode> hierarchyMap;

	vector<HierarchyNode> hierarchyEntry;

	//初始化层次关系
	uti.initHierarchyRelation(&hierarchyMap, &hierarchyEntry);
	//实现对层次关系进行剪枝的函数，需要Ttrue和Tfalse检测结果
	/**8888888888888888888888888888**/

	//获取是否有单词的判断
	bool isSingleLitral = false;
	//格子占全的维度数目
	int fullDemNum = 0;
	//格子占半的维度数目
	int halfDemNum = 0;
	vector<vector<string>> termsDimension = preData.termsDimension;

	vector<vector<vector<int>>> mutantsDimensionValue;
	vector<vector<CCell*>> corresponding_tests;
	vector<int> tModelIndexAppeared;
	vector<vector<CCell*>> matrixCells;
	vector<vector<int>> coefficientMatrix;
	vector<string> mutants;
	vector<vector<CCell *>> TNFCCells;
	vector<string> faultTypes;
	vector<string> faultTerms;

	int startIndex = 0;
	if (type == "single"){
		for (int i = 0; i < preData.termsDimension.size(); i++){//逐个term进行解析
			/*
			dimensionValuesStr保存第i项的每一维的转换值，eg
			3
			1 2 3 4
			第一行表示第i项第一维的值，第二行表示第二维的值。
			*/
			string dimensionValuesStr;
			//将一行转换为多维变换，例如11 10 00 01 11 10 ，就是3 4 1 2 3 4，即34 1234
			for (int j = 0; j < dimensionNum; j++)
			{
				//将各维的01表示法，转换为1,2,3,4表示法
				string line = termsDimension[i][j];
				string tempStr;
				commonFuncs.transDimensionLineFrom01totModelIndex(line, tempStr);
				//// cout <<"tempstr: "<< tempStr << endl;
				dimensionValuesStr.append(tempStr);
				int strLength = tempStr.length();
				if (vOdd != true || j != dimensionNum - 1) //格子占全时候字符串长度应为7,占半长度为3
				{
					if (strLength == 7)
						fullDemNum++;
					else if (strLength == 3)
						halfDemNum++;
				}
				else //格子占全时候字符串长度应为3,占半长度为1
				{
					if (strLength == 3)
						fullDemNum++;
					else if (strLength == 1)
						halfDemNum++;
				}
				if (j != dimensionNum - 1)
					dimensionValuesStr.append("\n");
			}
			//// cout << "dimensionValuesStr: " << dimensionValuesStr << endl;
			if (fullDemNum == 0 || (fullDemNum == dimensionNum - 1 && halfDemNum == 1)){
				isSingleLitral = true;
			}


			//根据缺陷信息转换成需要的格覆盖,在此处要进行改造
			mutantsDimensionValue.clear();
			corresponding_tests.clear();
			//整个思路
			//首先根据本体得到所有变体的待测点，所谓待测点，就是变体与本体不同的真值点，即包括本体真值点，也包括变体真值点
			//然后根据待测点组方程组，一个变体一个方程组，方程组变量的个数为所有待测点
			//然后根据每个变体的待测点确定每个方程的变量个数
			//结方程组得到最优的变量个数即可，其中不涉及到表达式的真假值，至于整个处理的逻辑原理请参考表达式

			int matrix_size1 = matrixCells.size();
			LIFGenerateCellsForMutants(commonFuncs,
				dimensionValuesStr,
				vOdd,
				matrixCells,
				mutantsDimensionValue,
				corresponding_tests, allOPs);
			for (int x = startIndex; x < matrixCells.size(); x++){
				faultTypes.push_back("LIF");
				faultTerms.push_back(terms[i]);
			}
			startIndex = matrixCells.size();
			int matrix_size2 = matrixCells.size();
			bool hasAllLitral = false;
			string term = terms.at(i);
			vector<string> literals = uti.extractCNFstyle(term);
			if (literals.size() == model.size()) hasAllLitral = true;
			if (hasAllLitral || isSingleLitral) //如果当前项包含所有字母，无法生成LIF 或者 只有一个词 则退化成TOF
			{
				TOFGenerateCellsForMutants(commonFuncs, dimensionValuesStr, vOdd, matrixCells,
					mutantsDimensionValue,
					corresponding_tests, allOPs);

				for (int x = startIndex; x < matrixCells.size(); x++){
					faultTypes.push_back("TOF");
					faultTerms.push_back(terms[i]);
				}
				startIndex = matrixCells.size();
			}

			LRFGenerateCellsForMutants(commonFuncs, dimensionValuesStr, vOdd, matrixCells,
				mutantsDimensionValue,
				corresponding_tests, allOPs, allUPs);

			for (int x = startIndex; x < matrixCells.size(); x++){
				faultTypes.push_back("LRF");
				faultTerms.push_back(terms[i]);
			}
			startIndex = matrixCells.size();

			LOFGenerateCellsForMutants(commonFuncs, dimensionValuesStr, vOdd, matrixCells,
				mutantsDimensionValue,
				corresponding_tests, allOPs, allUPs);

			for (int x = startIndex; x < matrixCells.size(); x++){
				faultTypes.push_back("LOF");
				faultTerms.push_back(terms[i]);
			}
			startIndex = matrixCells.size();

			get_mutant_constrains(mutantsDimensionValue, vOdd, corresponding_tests, newExp, i, mutants);

		}
	}
	//else return;

	//转换约束条件
	//得到所有的真值点，这一步必须进入循环吗？
	commonFuncs.getAlltModelIndexAppearedInMatrix(matrixCells, tModelIndexAppeared);
	//对列向量进行排序，如311排在312之前
	sort(tModelIndexAppeared.begin(), tModelIndexAppeared.end());

	commonFuncs.getCoefficientMatrix(tModelIndexAppeared, matrixCells, coefficientMatrix);

	vector<vector<int>>finalCoefficientMatrix;
	vector<int>finalTModelIndex;
	removeAllZeroCols(coefficientMatrix, tModelIndexAppeared, finalCoefficientMatrix, finalTModelIndex);

	vector<int>().swap(tModelIndexAppeared);//只是为了打印用
	vector<vector<int>>().swap(coefficientMatrix);//只是为了打印用
	matrixCells.clear();

	//Glpk解约束方程
	GLPK_Solve glpk_Solve;
	vector<int> colSelected;
	vector<int> deletedRows;
	removeAllZeroRows(finalCoefficientMatrix, coefficientMatrix, deletedRows);
	coefficientMatrix.swap(finalCoefficientMatrix);
	coefficientMatrix.clear();
	for (int i = deletedRows.size() - 1; i >= 0; i--){
		vector<string>::iterator itrl = mutants.begin() + i;
		mutants.erase(itrl);
	}
	//可以修改，不需要每次都生成
	vector<string> mutants_DP;
	for (int num = 0; num < mutants.size(); num++)
	{
		//printf("%s,%d,mutants[%d]:[%s]\r\n", __FILE__, __LINE__, num, mutants[num].c_str());
		mutants_DP.push_back(mutants[num]);
	}
	glpk_Solve.run(colSelected, &finalCoefficientMatrix);
	//将得到的格覆盖转换成测试用例
	vector<string> testset;
	for (int j = 0; j < colSelected.size(); j++)
	{
		char tmp[40];
		sprintf(tmp, "%d", finalTModelIndex[colSelected[j]]);
		//printf("%d:", finalTModelIndex[colSelected[j]]);
		string inde = "";
		inde.append(tmp);
		string point01Str = commonFuncs.transTModelIndexStrTo01Str(inde, vOdd);
		testset.push_back(point01Str);
	}
	//test
	// cout << endl;
	// cout << "Test-Case" << endl;
	for (int i = 0; i < testset.size(); ++i){
		// cout << testset[i] << endl;
	}
	// cout << "Test-Case-End" << endl;
	//大类拆分主干函数
	if (mode == "sync")
	{
		//临近算法（新旧）单侧
		extractFalseType(expression, testset);
		return 0.1;
	}
	else if(mode == "batch"){
		//全部算法集成测试模式
		double hit_rate = TestNewAlgo(expression, testset);
		return hit_rate;
	}
	else if (mode == "ordinary_single"){
		//蛮力算法单测
		ordinaryAlgo(expression, testset);
		return 0.1;
	}
}

vector<string> generateTestcase(vector<char> testcase, vector<string> testset, int depth, int length){
	if (depth == length){
		string s = "";
		for (int i = 0; i < length; i++){
			s+=testcase.at(i);
		}
		testset.push_back(s);
	}
	else
	{
		testcase.push_back('1');
		testset=generateTestcase(testcase, testset, depth + 1, length);
		testcase.pop_back();
		testcase.push_back('0');
		testset=generateTestcase(testcase, testset, depth + 1, length);
		testcase.pop_back();
	}
	return testset;
}

double generateAllTestcase(string expression){
	Utility uti;
	string model = uti.generateModel(expression);
	vector<char> testcase;
	vector<string> testset;
	int size = model.length();
	//cout <<"变量个数" <<size << endl;
	//return 0;
	testset=generateTestcase(testcase,testset, 0,size);
	double hit_rate = TestNewAlgo(expression, testset);
	return hit_rate;
	/*for (int i = 0; i < testset.size(); i++)
	{
		cout << testset.at(i) << endl;
	}
	return 1;
	*/
}

string deleteTab(string s)
{
	string res = "";
	for (char x : s){
		if (x == ' ') continue;
		res = res + x;
	}
	return res;
}

class Write_For_Draw{
	public:
		vector<int>  origin_testcase_size;
		vector<int>  add_testcase_size;
		vector<int>  all_testcase_size;
		int order = 1;
		ofstream oFile;
		string path = "E:\\FaultTable\\" + filename + "\\draw_NearTestResult.csv";
	
		Write_For_Draw(){
			oFile.open(path);
			oFile << " " << "," << "origin_new" << "," << "origin_old" << "," << "origin_ori" << "," << "add_new" << "," << "add_old" << "," << "add_ori" << "," << "all_new" << "," << "all_old" << "," << "all_ori" << endl;
		}

		void input_testcase_size(int origin, int add, int all)
		{
			origin_testcase_size.push_back(origin);
			add_testcase_size.push_back(add);
			all_testcase_size.push_back(all);
		}

		void write_line(){
			oFile << order;
			for (int item : origin_testcase_size){
				oFile << "," <<item;
			}
			for (int item : add_testcase_size){
				oFile << "," << item;
			}
			for (int item : all_testcase_size){
				oFile << "," << item;
			}
			oFile << endl;
			if (order == 12 && filename == "test20") order += 1;
			order += 1;
		}

		void clear_testcase_size()
		{
			origin_testcase_size.clear();
			add_testcase_size.clear();
			all_testcase_size.clear();
		}
};


void Input_Exp_FromFile()
{
	Write_For_Draw write_for_draw = Write_For_Draw();
	ifstream in("E://FaultTable//" + filename + ".txt");
	ofstream oFile;
	string path = "E:\\FaultTable\\" + filename + "\\all_NearTestResult.csv";
	oFile.open(path);
	oFile << "序号" << "," << "原表达式" << "," << "命中率" << "," << "原测试用例集大小" << "," << "增加的测试用例集大小" << "," << "最终集合大小" ;
	//根据结构体中的顺序输出错误类型
	exp_mutants_result = Exp_Mutants_Result();
	auto iter = exp_mutants_result.max_add_testcase_size.begin();
	for (iter; iter != exp_mutants_result.max_add_testcase_size.end(); iter++)
	{
		oFile << "," << iter->first << "," << " " << "," << " "<< "," << " ";
	}
	oFile << "," << "合并" << "," << " " << "," << " " << "," << " " << "," << "平均" << "," << "算法种类" << "," << "总平均结果集大小" << "," << "算法耗时" << endl;
	oFile << "生成临近测试用例次数" << "," << " " << "," << " " << "," << " " << "," << " " << "," << " ";
	iter = exp_mutants_result.max_add_testcase_size.begin();
	for (iter; iter != exp_mutants_result.max_add_testcase_size.end(); iter++)
	{
		oFile << "," << "max" << "," << "min" << "," << "max_result" << "," << "min_result";
	}
	oFile << "," << "max" << "," << "min" << "," << "max_result" << "," << "min_result" << endl;
	string expression;
	double old_avg_hit_rate = 0, avg_hit_rate = 0, ordinary_avg_hit_rate = 0;
	double num=0;
	if (in) // 有该文件
	{
		//// cout << "输出一行后" << endl;
		while (getline(in, expression)) // line中不包括每行的换行符
		{
			//表达式去空格
			ti += 1;
			if (ti == 12 && filename == "test20")ti += 1;
			expression = deleteTab(expression);
			string algo_tag = "new";
			filename = filename_new;
			if (algo_mode){
				algo_tag = "old";
				filename = filename_old;
			}
			//MutantsandValidInvalidTestCase("single", expression); //生成对应表达式的表格
			//// cout << expression << endl;
			exp_mutants_result = Exp_Mutants_Result();
			exp_mutants_result.exp = expression;
			double hit_rate=UseNearTestSingleFault("single", expression,"batch");
			oFile << to_string(ti) << "," << expression << "," << hit_rate << "," << exp_mutants_result.origin_testcase_size << "," << exp_mutants_result.add_testcase_size << "," << exp_mutants_result.all_testcase_size;
			exp_mutants_result.generateResMessage();
			exp_mutants_result.generateAvgTimeInterval();
			//写入制表文件
			write_for_draw.input_testcase_size(exp_mutants_result.origin_testcase_size, exp_mutants_result.add_testcase_size, exp_mutants_result.all_testcase_size);
			auto iter1 = exp_mutants_result.max_add_testcase_size.begin();
			auto iter2 = exp_mutants_result.min_add_testcase_size.begin();
			auto iter3 = exp_mutants_result.max_res_size.begin();
			auto iter4 = exp_mutants_result.min_res_size.begin();
			//// cout << "map大小"<<exp_mutants_result.max_add_testcase_size.size() << endl;
			int max = 0;
			int min = INT_MAX;
			int max_res = 0;
			int min_res = INT_MAX;
			for (iter1, iter2; iter1 != exp_mutants_result.max_add_testcase_size.end() && iter2 != exp_mutants_result.min_add_testcase_size.end(); iter1++, iter2++)
			{
				if (iter2->second > iter1->second) iter2->second = iter1->second;
				oFile << "," << iter1->second << "," << iter2->second << "," << iter3->second << "," << iter4->second;
				iter3++;
				iter4++;
				if (iter1->second > max) max = iter1->second;
				if (iter2->second < min) min = iter2->second;
				if (iter3->second > max_res) max_res = iter3->second;
				if (iter4->second < min_res) min_res = iter4->second;
			}
			if (min == INT_MAX) min = max;
			if (min_res == INT_MAX) min_res = max_res;
			oFile << "," << max << "," << min << "," << max_res << "," << min_res << "," << exp_mutants_result.avg_generateNearTestset_times << "," << algo_tag << "," << exp_mutants_result.avg_res_size << "," << exp_mutants_result.avg_time_interval << endl;
			num += 1;
			avg_hit_rate += hit_rate;
			algo_mode = !algo_mode;
			//输出旧算法结果
			algo_tag = "new";
			filename = filename_new;
			if (algo_mode){
				algo_tag = "old";
				filename = filename_old;
			}
			exp_mutants_result = Exp_Mutants_Result();
			exp_mutants_result.exp = expression;
			hit_rate = UseNearTestSingleFault("single", expression, "batch");
			oFile << to_string(ti) << "," << expression << "," << hit_rate << "," << exp_mutants_result.origin_testcase_size << "," << exp_mutants_result.add_testcase_size << "," << exp_mutants_result.all_testcase_size;
			exp_mutants_result.generateResMessage();
			exp_mutants_result.generateAvgTimeInterval();
			//写入制表文件
			write_for_draw.input_testcase_size(exp_mutants_result.origin_testcase_size, exp_mutants_result.add_testcase_size, exp_mutants_result.all_testcase_size);
			iter1 = exp_mutants_result.max_add_testcase_size.begin();
			iter2 = exp_mutants_result.min_add_testcase_size.begin();
			iter3 = exp_mutants_result.max_res_size.begin();
			iter4 = exp_mutants_result.min_res_size.begin();
			//// cout << "map大小"<<exp_mutants_result.max_add_testcase_size.size() << endl;
			max = 0;
			min = INT_MAX;
			max_res = 0;
			min_res = INT_MAX;
			for (iter1, iter2; iter1 != exp_mutants_result.max_add_testcase_size.end() && iter2 != exp_mutants_result.min_add_testcase_size.end(); iter1++, iter2++)
			{
				if (iter2->second > iter1->second) iter2->second = iter1->second;
				oFile << "," << iter1->second << "," << iter2->second << "," << iter3->second << "," << iter4->second;
				iter3++;
				iter4++;
				if (iter1->second > max) max = iter1->second;
				if (iter2->second < min) min = iter2->second;
				if (iter3->second > max_res) max_res = iter3->second;
				if (iter4->second < min_res) min_res = iter4->second;
			}
			if (min == INT_MAX) min = max;
			if (min_res == INT_MAX) min_res = max_res;
			oFile << "," << max << "," << min << "," << max_res << "," << min_res << "," << exp_mutants_result.avg_generateNearTestset_times << "," << algo_tag << "," << exp_mutants_result.avg_res_size << "," << exp_mutants_result.avg_time_interval << endl;
			old_avg_hit_rate += hit_rate;
			algo_mode = !algo_mode;
			//输出蛮力算法结果
			algo_tag = "ordinary";
			filename = filename_ordinary;
			exp_mutants_result = Exp_Mutants_Result();
			exp_mutants_result.exp = expression;
			hit_rate = generateAllTestcase(expression);
			//hit_rate = UseNearTestSingleFault("single", expression, "batch"); //旧蛮力算法
			oFile << to_string(ti) << "," << expression << "," << hit_rate << "," << exp_mutants_result.origin_testcase_size << "," << exp_mutants_result.add_testcase_size << "," << exp_mutants_result.all_testcase_size;
			exp_mutants_result.generateResMessage();
			exp_mutants_result.generateAvgTimeInterval();
			//写入制表文件
			write_for_draw.input_testcase_size(exp_mutants_result.origin_testcase_size, exp_mutants_result.add_testcase_size, exp_mutants_result.all_testcase_size);
			iter1 = exp_mutants_result.max_add_testcase_size.begin();
			iter2 = exp_mutants_result.min_add_testcase_size.begin();
			iter3 = exp_mutants_result.max_res_size.begin();
			iter4 = exp_mutants_result.min_res_size.begin();
			//// cout << "map大小"<<exp_mutants_result.max_add_testcase_size.size() << endl;
			max = 0;
			min = INT_MAX;
			max_res = 0;
			min_res = INT_MAX;
			for (iter1, iter2; iter1 != exp_mutants_result.max_add_testcase_size.end() && iter2 != exp_mutants_result.min_add_testcase_size.end(); iter1++, iter2++)
			{
				if (iter2->second > iter1->second) iter2->second = iter1->second;
				oFile << "," << iter1->second << "," << iter2->second << "," << iter3->second << "," << iter4->second;
				iter3++;
				iter4++;
				if (iter1->second > max) max = iter1->second;
				if (iter2->second < min) min = iter2->second;
				if (iter3->second > max_res) max_res = iter3->second;
				if (iter4->second < min_res) min_res = iter4->second;
			}
			if (min == INT_MAX) min = max;
			if (min_res == INT_MAX) min_res = max_res;
			oFile << "," << max << "," << min << "," << max_res << "," << min_res << "," << exp_mutants_result.avg_generateNearTestset_times << "," << algo_tag << "," << exp_mutants_result.avg_res_size << "," << exp_mutants_result.avg_time_interval << endl;
			ordinary_avg_hit_rate += hit_rate;
			//写入制表行
			write_for_draw.write_line();
			write_for_draw.clear_testcase_size();
		}
		avg_hit_rate /= num;
		old_avg_hit_rate /= num;
		oFile << "总平均命中率" << "," << avg_hit_rate << "," << "new" << endl;
		oFile << "总平均命中率" << "," << old_avg_hit_rate << "," << "old" << endl;
		oFile << "总平均命中率" << "," << ordinary_avg_hit_rate << "," << "ordinary" << endl;
	}
	else // 没有该文件
	{
		// cout << "no such file" << endl;
	}
	return;
}

void Input_Exp_FromCmd(){
	string expression;
	// cout << "请输入原表达式" << endl;
	cin >> expression;
	algo_mode = true;
	//UseNearTestSingleFault("single", expression,"sync");
	//蛮力算法模式
	UseNearTestSingleFault("single", expression, "ordinary_single");
}

int main()
{
	//generateAllTestcase("ab+cd");
	//自动化测试
	Input_Exp_FromFile();
	//跑单独测试
	//Input_Exp_FromCmd();
	//生成表格函数
	//MutantsandValidInvalidTestCase("single", expression);
	//string expression;
	// cout << "请输入原表达式" << endl;
	////cin >> expression;
	//MutantsandValidInvalidTestCase("double", expression);
	system("pause");
	return 0;
}
