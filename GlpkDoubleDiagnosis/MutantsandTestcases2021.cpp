// BoolCheckFramework.cpp : �������̨Ӧ�ó������ڵ㡣
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

//���ݱ��ʽ��ȡ����
//����exp��Ϊԭ���ʽ
//����faultType��Ϊȱ������
//����iModel��Ϊ��ʽ����
//����DNF���ʽ


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
	vector<string> testset/*���ɵĲ����������ϣ������������ȫ��ͨ�������ʾû������*/,
	string exprOri/*ԭ���ʽ*/,
	string expr/*����*/,
	vector<int>& unformityTCsol, /*���һ�µĽڵ㣬Ϊ��Ч���Ե�*/
	vector<int>& differTCsol, /*�����һ�µĽڵ㣬Ϊ��Ч���Ե�*/
	vector<string>& unformityStr, /*���һ�µĽڵ㣬Ϊ��Ч���Ե�*/
	vector<string>& differTStr, /*�����һ�µĽڵ㣬Ϊ��Ч���Ե�*/
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
		bool v = v1 ^ v2;//����ֵ��ͬ��Ϊ�٣���ͬ��Ϊ��,������������Դ���˷�ʱ��ѽ�������и��ŵ��㷨
		if (v){
			if (v1 && !v2)
			{
				differType.push_back("s");  //s ��ʾʲô��˼�أ�
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
	vector<string> testset/*���ɵĲ����������ϣ������������ȫ��ͨ�������ʾû������*/,
	string exprOri/*ԭ���ʽ*/,
	string expr/*����*/,
	vector<int>& unformityTCsol, /*���һ�µĽڵ㣬Ϊ��Ч���Ե�*/
	vector<int>& differTCsol, /*�����һ�µĽڵ㣬Ϊ��Ч���Ե�*/
	vector<string>& unformityStr, /*���һ�µĽڵ㣬Ϊ��Ч���Ե�*/
	vector<string>& differTStr, /*�����һ�µĽڵ㣬Ϊ��Ч���Ե�*/
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
		bool v = v1 ^ v2;//����ֵ��ͬ��Ϊ�٣���ͬ��Ϊ��,������������Դ���˷�ʱ��ѽ�������и��ŵ��㷨
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
				//3�������2
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
				//3�������2
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

//�޳�Լ��������ȫ�����
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
		printf("���ʽֻ����һ������ܷ�������Double Fault LIFxLIF��");
		return false;
	}
	//double fault lifxlif ÿ����Ҫ��������
	// 1.1Ԥ������ÿ��term��dimensionValuesStr������
	vector<string> dimensionValuesStrs; //�洢ÿ��term��dimension value string
	int termNum = terms.size();
	for (int i = 0; i<termNum; i++)//���term���н���
	{
		string dimensionValuesStr = "";
		for (int j = 0; j < dimensionNum; j++)
		{
			//����ά��01��ʾ����ת��Ϊ1,2,3,4��ʾ��
			string line = termsDimension[i][j];
			string tempStr;
			commonFuncs.transDimensionLineFrom01totModelIndex(line, tempStr);
			dimensionValuesStr.append(tempStr);
			if (j != dimensionNum - 1)
				dimensionValuesStr.append("\n");
		}
		dimensionValuesStrs.push_back(dimensionValuesStr);
	}
	//1.2 Ԥ�����ȡ��������otp
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
		printf("���ʽֻ����һ������ܷ�������Double Fault LIFxLRF��");
		return false;
	}
	//double fault lifxlrf ÿ����Ҫ��������
	// 1.1Ԥ������ÿ��term��dimensionValuesStr������
	int termNum = terms.size();
	vector<string> dimensionValuesStrs; //�洢ÿ��term��dimension value string
	for (int i = 0; i<termNum; i++)//���term���н���
	{
		string dimensionValuesStr = "";
		for (int j = 0; j < dimensionNum; j++)
		{
			//����ά��01��ʾ����ת��Ϊ1,2,3,4��ʾ��
			string line = termsDimension[i][j];
			string tempStr;
			commonFuncs.transDimensionLineFrom01totModelIndex(line, tempStr);
			dimensionValuesStr.append(tempStr);
			if (j != dimensionNum - 1)
				dimensionValuesStr.append("\n");
		}
		dimensionValuesStrs.push_back(dimensionValuesStr);
	}
	//1.2 Ԥ�����ȡ��������otp
	vector<int> all3OPs;
	for (int i = 0; i<opover2.size(); i++)
	{
		int value = commonFuncs.getTModelIndexOf01Str(opover2.at(i), vOdd);
		all3OPs.push_back(value);
	}
	sort(all3OPs.begin(), all3OPs.end());

	//����LRF����
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
		printf("���ʽֻ����һ������ܷ�������Double Fault LRFxLRF��");
		return false;
	}
	//double fault lrfxlrf ÿ����Ҫ��������
	// 1.1Ԥ������ÿ��term��dimensionValuesStr������
	vector<string> dimensionValuesStrs; //�洢ÿ��term��dimension value string
	for (int i = 0; i<termNum; i++)//���term���н���
	{
		string dimensionValuesStr = "";
		bool isSingleLitral = false;
		//����ռȫ��ά����Ŀ
		int fullDemNum = 0;
		//����ռ���ά����Ŀ
		int halfDemNum = 0;
		for (int j = 0; j < dimensionNum; j++)
		{
			//����ά��01��ʾ����ת��Ϊ1,2,3,4��ʾ��
			string line = termsDimension[i][j];
			string tempStr;
			commonFuncs.transDimensionLineFrom01totModelIndex(line, tempStr);
			dimensionValuesStr.append(tempStr);
			int strLength = tempStr.size();
			if (vOdd != true || j != dimensionNum - 1) //����ռȫʱ���ַ�������ӦΪ7,ռ�볤��Ϊ3
			{
				if (strLength == 7)
					fullDemNum++;
				else if (strLength == 3)
					halfDemNum++;
			}
			else //����ռȫʱ���ַ�������ӦΪ3,ռ�볤��Ϊ1
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
		//����DLRF����
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
			if (!v1)continue;//���������㣬��ȥ
			bool v2 = uti.evaluateMutant(oriExp, terms[i], testcase[j]);
			bool v = v1 ^ v2;//���ֵͬΪ�٣�ֵ��ͬΪ��
			if (v == false){//���Ϊ�٣�˵����������ȱ��
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
	//�ֽ���ʽ
	PRE_PROCESS_DATA preData;

	//ת��ΪIDNF
	Utility uti;
	vector<string> terms = uti.extractDNFstyle(expression);
	vector<string>::iterator iter;
	string newExp = "";
	//�õ�IDNF
	newExp = expression;// generateIDNForICNF(expression, true);
	//printf("newExp:%s\n", newExp.c_str());

	getPreProcessData(newExp, preData, DNF);
	CCommonFuncs commonFuncs;
	int literalNum = model.size();
	int dimensionNum = (literalNum + 1) / 2;
	bool vOdd = (literalNum % 2 == 0) ? false : true;
	//printf("%d,%d,%d\n", __LINE__, vOdd, literalNum);

	//��ȡ�仯�ʵ�ά�ȱ��
	int literalNo1;
	int dimensionNo1 = getDimensionNo(model, Liter1, literalNo1);

	//��ȡ���е�OTP
	vector<string> op = preData.op;
	vector<int> allOPs;
	for (int i = 0; i<op.size(); i++)
	{
		int value = commonFuncs.getTModelIndexOf01Str(op.at(i), vOdd);
		allOPs.push_back(value);
	}
	sort(allOPs.begin(), allOPs.end());

	//��ȡ���е�UTP
	vector<int> allUPs;
	vector<string> up = preData.up;
	for (int i = 0; i<up.size(); i++)
	{
		int value = commonFuncs.getTModelIndexOf01Str(up.at(i), vOdd);
		allUPs.push_back(value);
	}
	sort(allUPs.begin(), allUPs.end());

	//��ȡ�Ƿ��е��ʵ��ж�
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
	//һ��term��Ӧ��ά��ֵ���м���ά�ȣ�dimensionNum�ͻ��ж��
	vector<string> testcase;
	CCommonFuncs commonFunc;
	mutantChangeTerm = mutantTerm;
	string lm = Liter1;
	int lz = mutantChangeTerm.find(lm);
	string::iterator itr = mutantChangeTerm.begin() + lz;
	mutantChangeTerm.erase(itr);
	//printf("LOF:mutantChangeTerm[%s]\n", mutantChangeTerm.c_str());

	vector<string> addValues;
	//�����ж����ĸ�ά�ȣ�����ж���
	/*printf("termsDimension[%d]:\n", dimensionNo1);
	for (int x = 0; x < termsDimension[dimensionNo1].size(); x++){
		printf("[%s]", termsDimension[dimensionNo1][x].c_str());
	}
	printf("\n");*/
	//û�жԷ�������
	if (literalNo1 == dimensionNo1 * 2){//�����4�������򲻱䣬�������Ӹôʵ�01ֵ
		//�����������1��������Ϊ2�������2��������Ϊ4
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
	//�仯��ĸ񸲸�
	/*printf("\nchanged:\n");
	for (int x = 0; x < dimensionNum; x++){
		for (int y = 0; y < termsDimension[x].size(); y++){
			printf("[%s]", termsDimension[x][y].c_str());
		}
		printf("\n");
	}*/
	//�仯�ĸ񸲸ǵ�
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
//bool checkTestDiffer(string test, string exprOri/*ԭ���ʽ*/, string expr/*����*/){
//	Utility uti;
//	bool v = (uti.evaluate(exprOri, test)) ^ (uti.evaluateMutant(exprOri, expr, test));//��򣺲�ͬΪ�棬��ͬΪ��
//	return v;
//}

//��ϡ��������Ų�������+��ι�ϵ
//���ɱ����ͬʱ�����жϣ������ǰѱ���ȫ�����ɺ��ж�
/*
bool diagnosisByHierarchy(
	string oriExp,//ԭ���ʽ
	string faultExp,//������ʽ
	vector<vector<string>> optiUniformitySet,//������Ч��
	vector<vector<string>> optiDifferSet,//������Ч��
	vector<HierarchyNode> hierarchyEntrySet,
	hash_map<string, HierarchyNode>& hierarchyMap
)
{
	for (int i = 0; i < hierarchyEntrySet.size; i++){
		//��ÿ����ͨͼ���б���
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
	string oriExp,//ԭ���ʽ
	string faultExp,//������ʽ
	string faultType, //ȱ������
	vector<vector<string>> optiUniformitySet,//������Ч��
	vector<vector<string>> optiDifferSet,//������Ч��
	hash_set<string>& outMutant,//�ų��ı����ʽ
	HierarchyNode root,//��ǰȱ�ݶ�Ӧ�ڵ�
	hash_map<string, HierarchyNode> hierarchyMap,
	vector<int> terms,//���ܷ���ȱ�ݵ���
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


////��ϡ��������Ų�������
//bool diagnosis(
//	string oriExp, //ԭ���ʽ
//	string faultExp, //������ʽ
//	string faultType, //ȱ������
//	vector<string> oriTerms,
//	vector<string> faultTerms,
//	string model,
//	vector<vector<string>> optiUniformitySet,//������Ч��
//	vector<vector<string>> optiDifferSet//������Ч��
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
//		bool flagUniform = true;//��Ч���Ƿ�ȫ��ͨ��
//		//��1��������Ч�����̽��
//		for (int i = 0; i < optiUniformitySet.size(); i++){
//			//printf("point: %s, faultExp: %s, mutant: %s\n", optiUniformitySet[i][0].c_str(), faultExp.c_str(), mutant.c_str());
//			if (checkTestDiffer(optiUniformitySet[i][0], faultExp, mutant)){
//				flagUniform = false;
//				break;
//			}
//		}
//
//		if (flagUniform == true){//��Ч��ȫ��ͨ��
//			bool flagDiffer = true;
//			for (int i = 0; i < optiDifferSet.size(); i++){
//				if (checkTestDiffer(optiDifferSet[i][0], faultExp, mutant)){
//					flagDiffer = false;
//					break;
//				}
//			}
//			if (flagDiffer == true){//��Ч��ȫ��ͨ��
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
	//�ֽ���ʽ   ������ʽ��utp otp fp �ȵ�
	PRE_PROCESS_DATA preData; MutantsandValidInvalidTestCase;
	vector<string> terms = uti.extractDNFstyle(expression); //�ֽ���ʽ����
	string model = uti.generateModel(expression);  //model����ԭ���ʽ�����������е���ĸ(�ź���ģ������� abce+cd+ef  abcdef
	string newExp = "";
	//�õ�IDNF
	//newExp = generateIDNForICNF(expression, true);
	//newExp = generateDNF(terms);
	newExp = expression;
	getPreProcessData(newExp, preData, true);
	vector<string> allPoint;
	CCommonFuncs commonFuncs;
	int literalNum = preData.literalCount;
	int dimensionNum = (literalNum + 1) / 2;
	bool vOdd = (literalNum % 2 == 0) ? false : true;
	//��ȡ���е�OTP
	vector<string> op = preData.op;
	vector<int> allOPs;
	for (int i = 0; i < op.size(); i++)
	{
		int value = commonFuncs.getTModelIndexOf01Str(op.at(i), vOdd);
		allOPs.push_back(value);
	}

	sort(allOPs.begin(), allOPs.end());

	//��ȡ���е�UTP��
	vector<int> allUPs;
	vector<string> up = preData.up;
	for (int i = 0; i < up.size(); i++)
	{
		int value = commonFuncs.getTModelIndexOf01Str(up.at(i), vOdd);
		allUPs.push_back(value);
	}
	sort(allUPs.begin(), allUPs.end());
	/**8888888888888888888888888888**/
	/*termsDimension ��ʾ����ÿһ�����ʹ���ʽֵȡ1��ȡֵ������ ab+cd  model=��abcd�� û2����ĸ��һά
	11(��һ��� ��һά��ȡֵ��
	00 01 11 10����һ�������ά��ȡֵ��

	00 01 11 10���ڶ���ĵ�һά��ȡֵ��
	11���ڶ��������ά��ȡֵ��
	*/
	/*
		nanjia������һ�������ʹ�õ����ݽṹ���������ǹ������е㡢���滻����Ϣ
	*/
	printf("\npreData.termsDimension:\n");
	for (int i = 0; i < preData.termsDimension.size(); i++){
		for (int j = 0; j < preData.termsDimension[i].size(); j++){
			printf("%s\n", preData.termsDimension[i][j].c_str());
		}
		printf("\n");
	}
	//��ι�ϵmap
	hash_map<string, HierarchyNode> hierarchyMap;

	vector<HierarchyNode> hierarchyEntry;

	//��ʼ����ι�ϵ
	uti.initHierarchyRelation(&hierarchyMap, &hierarchyEntry);
	//ʵ�ֶԲ�ι�ϵ���м�֦�ĺ�������ҪTtrue��Tfalse�����
	/**8888888888888888888888888888**/

	//��ȡ�Ƿ��е��ʵ��ж�
	bool isSingleLitral = false;
	//����ռȫ��ά����Ŀ
	int fullDemNum = 0;
	//����ռ���ά����Ŀ
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
		for (int i = 0; i < preData.termsDimension.size(); i++){//���term���н���
			/*
			dimensionValuesStr�����i���ÿһά��ת��ֵ��eg
			3
			1 2 3 4
			��һ�б�ʾ��i���һά��ֵ���ڶ��б�ʾ�ڶ�ά��ֵ��
			*/
			string dimensionValuesStr;
			//��һ��ת��Ϊ��ά�任������11 10 00 01 11 10 ������3 4 1 2 3 4����34 1234
			for (int j = 0; j < dimensionNum; j++)
			{
				//����ά��01��ʾ����ת��Ϊ1,2,3,4��ʾ��
				string line = termsDimension[i][j];
				string tempStr;
				commonFuncs.transDimensionLineFrom01totModelIndex(line, tempStr);
				//// cout <<"tempstr: "<< tempStr << endl;
				dimensionValuesStr.append(tempStr);
				int strLength = tempStr.length();
				if (vOdd != true || j != dimensionNum - 1) //����ռȫʱ���ַ�������ӦΪ7,ռ�볤��Ϊ3
				{
					if (strLength == 7)
						fullDemNum++;
					else if (strLength == 3)
						halfDemNum++;
				}
				else //����ռȫʱ���ַ�������ӦΪ3,ռ�볤��Ϊ1
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
			//����ȱ����Ϣת������Ҫ�ĸ񸲸�,�ڴ˴�Ҫ���и���
			mutantsDimensionValue.clear();
			corresponding_tests.clear();
			//����˼·
			//���ȸ��ݱ���õ����б���Ĵ���㣬��ν����㣬���Ǳ����뱾�岻ͬ����ֵ�㣬������������ֵ�㣬Ҳ����������ֵ��
			//Ȼ����ݴ�����鷽���飬һ������һ�������飬����������ĸ���Ϊ���д����
			//Ȼ�����ÿ������Ĵ����ȷ��ÿ�����̵ı�������
			//�᷽����õ����ŵı����������ɣ����в��漰�����ʽ�����ֵ����������������߼�ԭ����ο����ʽ

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
			if (isSingleLitral) //���ֻ��һ���� ���˻���TOF
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
		for (int i = 0; i < preData.termsDimension.size(); i++){//���term���н���
			/*
			dimensionValuesStr�����i���ÿһά��ת��ֵ��eg
			3
			1 2 3 4
			��һ�б�ʾ��i���һά��ֵ���ڶ��б�ʾ�ڶ�ά��ֵ��
			*/
			string dimensionValuesStr;
			//��һ��ת��Ϊ��ά�任������11 10 00 01 11 10 ������3 4 1 2 3 4����34 1234
			for (int j = 0; j < dimensionNum; j++)
			{
				//����ά��01��ʾ����ת��Ϊ1,2,3,4��ʾ��
				string line = termsDimension[i][j];
				string tempStr;
				commonFuncs.transDimensionLineFrom01totModelIndex(line, tempStr);
				//// cout <<"tempstr: "<< tempStr << endl;
				dimensionValuesStr.append(tempStr);
				int strLength = tempStr.length();
				if (vOdd != true || j != dimensionNum - 1) //����ռȫʱ���ַ�������ӦΪ7,ռ�볤��Ϊ3
				{
					if (strLength == 7)
						fullDemNum++;
					else if (strLength == 3)
						halfDemNum++;
				}
				else //����ռȫʱ���ַ�������ӦΪ3,ռ�볤��Ϊ1
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
			//����ȱ����Ϣת������Ҫ�ĸ񸲸�,�ڴ˴�Ҫ���и���
			mutantsDimensionValue.clear();
			corresponding_tests.clear();
			//����˼·
			//���ȸ��ݱ���õ����б���Ĵ���㣬��ν����㣬���Ǳ����뱾�岻ͬ����ֵ�㣬������������ֵ�㣬Ҳ����������ֵ��
			//Ȼ����ݴ�����鷽���飬һ������һ�������飬����������ĸ���Ϊ���д����
			//Ȼ�����ÿ������Ĵ����ȷ��ÿ�����̵ı�������
			//�᷽����õ����ŵı����������ɣ����в��漰�����ʽ�����ֵ����������������߼�ԭ����ο����ʽ
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
			if (isSingleLitral) //���ֻ��һ���� ���˻���TOF
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

			//���6����CC-S�ɼ���˫ȱ��
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

	//ת��Լ������
	//�õ����е���ֵ�㣬��һ���������ѭ����
	commonFuncs.getAlltModelIndexAppearedInMatrix(matrixCells, tModelIndexAppeared);
	//������������������311����312֮ǰ
	sort(tModelIndexAppeared.begin(), tModelIndexAppeared.end());

	commonFuncs.getCoefficientMatrix(tModelIndexAppeared, matrixCells, coefficientMatrix);

	vector<vector<int>>finalCoefficientMatrix;
	vector<int>finalTModelIndex;
	removeAllZeroCols(coefficientMatrix, tModelIndexAppeared, finalCoefficientMatrix, finalTModelIndex);

	vector<int>().swap(tModelIndexAppeared);//ֻ��Ϊ�˴�ӡ��
	vector<vector<int>>().swap(coefficientMatrix);//ֻ��Ϊ�˴�ӡ��
	matrixCells.clear();

	//Glpk��Լ������
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
	//�����޸ģ�����Ҫÿ�ζ�����
	vector<string> mutants_DP;
	for (int num = 0; num<mutants.size(); num++)
	{
		//printf("%s,%d,mutants[%d]:[%s]\r\n", __FILE__, __LINE__, num, mutants[num].c_str());
		mutants_DP.push_back(mutants[num]);
	}
	glpk_Solve.run(colSelected, &finalCoefficientMatrix);
	//���õ��ĸ񸲸�ת���ɲ�������
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
	//�����csv�ļ�

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

//���������ҵĴ��룬����ģ�顢������ģ�顢��������ģ��

int call_generateNearTestset_times = 0;
time_t preprocess_interval = 0;
set<string> near_testcase_set;
bool algo_mode=false; //��ʾʹ�õ��㷨���ͣ�ȡ1Ϊ���㷨��ȡ0Ϊ���㷨

//�ٽ������ɲ���������������޸ĵı������Ϻ͵�ǰ��������������޸ĺ�Ĳ�������
string generateNearTestset(string test, vector<char> wordset, unordered_map<char, int> literal_abstract_pos)
{

	for (int i = 0; i < wordset.size(); i++)
	{
		int k = literal_abstract_pos[wordset[i]]; 
		//// cout << "�ı�����ڱ��������е�λ��"<< k << endl;
		if (test[k] == '0') test[k] = '1';
		else test[k] = '0';
	}
	call_generateNearTestset_times += 1;
	near_testcase_set.insert(test);
	//// cout << "����������" << test << "��ǰ���ϴ�С" << near_testcase_set.size() << endl;
	return test;
}

//ά��ֵ���㺯����������ʽ�Ͳ�������������ò��������ڸñ��ʽ�ϵ�ά��ֵ��ԭ���ʽ�Ĳ�ֵ��
int level_in_mutant(string expression, string false_expression, string testcase, unordered_map<char, int> literal_abstract_pos)
{
	Utility uti;
	vector<string> terms = uti.extractDNFstyle(expression);
	int exp_val = 0;
	for (string term : terms)
	{
		string model = uti.generateModel(term); //�ӱ���������ab��cd��bc
		string child_testcase;
		//�����ӱ���λ�������Ӳ����������м��
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
		string model = uti.generateModel(term); //�ӱ���������ab��cd��bc
		string child_testcase;
		//�����ӱ���λ�������Ӳ����������м��
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
	//// cout << "LRF���" << endl;
	DnfMutant dnf_mutant;
	Utility uti;
	list<Literal> valid = nfp.getValid();
	list<Literal>::iterator plist;
	//// cout << "��ǰnfp��" << nfp.term << endl;
	for (plist = valid.begin(); plist != valid.end(); plist++)
	{
		//��鵱ǰԪ���Ƿ������б��������������ٽ������������
		//�ǣ�����������м����÷��ؼ�
		if (testcase.susceptibleLiterSet.find(*plist) == testcase.susceptibleLiterSet.end()) //20211211��ȥ���Ƿ������м����ж�
		{
			//// cout << "    ����Ŀ��滻�ʣ�" << (*plist).getChar() << endl;
			//�����ٽ������������ԣ���Ч��re��������
			vector<char> change_literal;
			change_literal.push_back((*plist).getChar());
			string near_testcase = generateNearTestset(testcase.testcase, change_literal, literal_abstract_pos); //�ٽ������²�������
			//// cout << "   �ı�ı��� " << (*plist).getChar() << endl;
			//// cout << "   LRF�е������ٽ��������� " << near_testcase << endl;
			bool check_if_true = uti.evaluateMutant(exp,fault_exp, near_testcase);
			if (check_if_true) continue;  //20211211����ά������û�����������
			string re = dnf_mutant.generateLRFmutant_inPos(exp, nfp, *plist);
			res.push_back(re);
			//// cout << re << endl;
		}
		else   //�����ٽ������������
		{
			//// cout << "    ���õĿ��滻�ʣ�" << (*plist).getChar() << endl;
			string re = dnf_mutant.generateLRFmutant_inPos(exp, nfp, *plist);
			sesusceptible_res.push_back(re);
			//// cout << re << endl;
		}
		
	}
	//// cout << "���뷵�ؼ���ģ��" << res.size() << endl;
	//// cout << "���÷��ؼ���ģ��" << sesusceptible_res.size() << endl;
	if(res.size()!=0) return res;
	else return sesusceptible_res;
}

vector<string> fault_in_LRF_ByLevel(string exp, string fault_exp, TestCase testcase, NFP nfp, unordered_map<char, int> literal_abstract_pos)
{
	vector<string> res;
	vector<string> sesusceptible_res;
	// cout << "LRF���(ά�ȼ���)" << endl;
	DnfMutant dnf_mutant;
	Utility uti;
	list<Literal> valid = nfp.getValid();
	list<Literal>::iterator plist;
	for (plist = valid.begin(); plist != valid.end(); plist++)
	{
		//��鵱ǰԪ���Ƿ������б��������������ٽ������������
		//�ǣ�����������м����÷��ؼ�
		//if (testcase.susceptibleLiterSet.find(*plist) == testcase.susceptibleLiterSet.end()) //20211211��ȥ���Ƿ������м����ж�
		//{
			//�����ٽ������������ԣ���Ч��re��������
			vector<char> change_literal;
			change_literal.push_back((*plist).getChar());
			string near_testcase = generateNearTestset(testcase.testcase, change_literal, literal_abstract_pos); //�ٽ������²�������
			// cout << "   �ı�ı��� " << (*plist).getChar() << endl;
			// cout << "   LRF�е������ٽ��������� " << near_testcase << endl;
			//bool check_if_true = uti.evaluate(fault_exp, near_testcase);
			bool check_if_true = false;
			if (level_in_mutant(exp, fault_exp, near_testcase, literal_abstract_pos) > 0)  check_if_true = true;
			if (check_if_true) continue;  //20211211����ά������û�����������
			string re = dnf_mutant.generateLRFmutant_inPos(exp, nfp, *plist);
			res.push_back(re);
			// cout << re << endl;
		/*}
		else   //�����ٽ������������
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
	// cout << "CORF��LOF���" << endl;
	// cout << "�Ƿ�������е� : " << (testcase.susceptibleLiterSet.find(nfp.getKey()) == testcase.susceptibleLiterSet.end()) << endl;
	//���0��ʾ���ҵ���1��ʾ�Ҳ���
	bool b = testcase.susceptibleLiterSet.find(nfp.getKey()) == testcase.susceptibleLiterSet.end();
	Utility uti;
	DnfMutant dnf_mutant;
	string model = uti.generateModel(exp);
	list<Literal> otherLiterals = nfp.getOther();
	//�����������е㼯��
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
				//�ж�ΪLOFȱ��
				string re = dnf_mutant.generateLOFmutant(exp, nfp);
				res.push_back(re);
			}
			else{
				//��λΪCORF
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
	//ȥ��
	set<string>s(res.begin(), res.end());
	res.assign(s.begin(), s.end());
	return res;
}

vector<string> fault_in_CORForLOF_ByRule6(string exp, string fault_exp, TestCase testcase, NFP nfp)
{
	vector<string> res;
	// cout << "CORF��LOF���" << endl;
	// cout << "�Ƿ�������е� : " << (testcase.susceptibleLiterSet.find(nfp.getKey()) == testcase.susceptibleLiterSet.end()) << endl;
	//���0��ʾ���ҵ���1��ʾ�Ҳ���
	bool b = testcase.susceptibleLiterSet.find(nfp.getKey()) == testcase.susceptibleLiterSet.end();
	Utility uti;
	DnfMutant dnf_mutant;
	string model = uti.generateModel(exp);
	list<Literal> otherLiterals = nfp.getOther();
	//�����������е㼯��
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
				//�ж�ΪLOFȱ��
				string re = dnf_mutant.generateLOFmutant(exp, nfp);
				res.push_back(re);
			}
			else{
				//��λΪCORF
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
	//ȥ��
	set<string>s(res.begin(), res.end());
	res.assign(s.begin(), s.end());
	return res;
}

vector<string> fault_in_CORF_ByRule6(string exp, string fault_exp, TestCase testcase, NFP nfp)
{
	vector<string> res;
	// cout << "CORF��LOF���" << endl;
	// cout << "�Ƿ�������е� : " << (testcase.susceptibleLiterSet.find(nfp.getKey()) == testcase.susceptibleLiterSet.end()) << endl;
	//���0��ʾ���ҵ���1��ʾ�Ҳ���
	bool b = testcase.susceptibleLiterSet.find(nfp.getKey()) == testcase.susceptibleLiterSet.end();
	Utility uti;
	DnfMutant dnf_mutant;
	string model = uti.generateModel(exp);
	list<Literal> otherLiterals = nfp.getOther();
	//�����������е㼯��
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
				//�ж�ΪLOFȱ��
				string re = dnf_mutant.generateLOFmutant(exp, nfp);
				res.push_back(re);
			}
			else{
				//��λΪCORF
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
	//ȥ��
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

//���һ�����弯���Ƿ�ȫ��ͨ�����
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

//���˽������0Ϊ��ȫͨ����⣬1Ϊ�����ţ�2Ϊ��������3Ϊ������
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
		//// cout << "����:��������С��������ԭ���ʽһ�£�ͨ�����" << endl;
		return 0;
	}
	if (isAllPassed_T == true)
	{
		//// cout << "���ˣ�������ֻ�����ŵ�" << endl;
		return 1;
	}
	else if (isAllPassed_F == true)
	{
		//// cout << "���ˣ�������ֻ��������" << endl;
		return 2;
	}
	else
	{
		//// cout << "���ˣ�����������������Ҳ�����ŵ�" << endl;
		return 3;
	}
}

//ֻ�����ŵ��֧�����ɳ���expΪ������ʽ��testsetΪ����㼯���˴�Ϊ�ٵ㼯
vector<string> judgeAllExpand(string exp, string fault_exp, vector<string> testset, vector<string> all_testset)
{
	vector<string> res;
	Utility uti;
	string model = uti.generateModel(exp); //ȫ��������abcd
	unordered_map<char, int> literal_abstract_pos;  //ȫ����������Ӧλ�ù�ϵ
	for (int i = 0; i < model.size(); i++)
	{
		literal_abstract_pos.insert(pair<char, int>{ model[i], i });
	}
	//�������ݽṹ����ƣ��Դ��������������¿̻�
	//�����Ų��������
	for (string testset_item : testset) 
	{
		bool is_vaild = uti.checkTestDiffer(testset_item, exp, fault_exp);
		if (is_vaild == false) continue;  //������Ч��������
		TestCase testcase= TestCase(exp,testset_item,false);
		//��ÿ�����������е�UTP/NFP�����Ų飬���ڴ����ź�����������NFP
		//list<UTPorNFP> nfps = testcase.getInfo(exp, testset_item, false);
		list<NFP> nfps = testcase.getNFP(exp, testset_item);
		for (NFP nfp : nfps)
		{
			//// cout << "������ǰnfp��" << nfp.term<<endl;
			//����ʹ��ȫ�ٽ����ж��Ƿ�����LRF���Ӷ�����������֧
			vector<char> change_literal;
			vector<char> backup_literal;
			list<Literal> valid_literals = nfp.getValid();
			bool is_sensitive = 1;  //�ж��Ƿ�ȫ�����е㣬ȡ1Ϊ���е�
			for (Literal literal : valid_literals)
			{
				//// cout << "������� " << literal.getChar() << endl;
				is_sensitive = is_sensitive & (testcase.susceptibleLiterSet.find(literal) != testcase.susceptibleLiterSet.end());
				auto got = testcase.susceptibleLiterSet.find(literal);
				if (got != testcase.susceptibleLiterSet.end()){
					backup_literal.push_back(literal.getChar());
					continue;
				}//���������м��д��ڣ������ñ���
				// cout << "�������дʣ�����Ч�滻 " << literal.getChar() << endl;
				change_literal.push_back(literal.getChar()); //������޸ı�������
			}
			bool check_if_LRF=false;
			if (is_sensitive == 1)  //ȫ�����е㣬������·
			{
				/*vector<string> res_items1 = fault_in_LRF(exp, fault_exp, testcase, nfp, literal_abstract_pos);
				for (string res_item : res_items1)
				{
					//// cout << "������ǰnfp��" << nfp.term << " ��Ը���ļ����: " << "��" << res_item << "��" << endl;
					//�ٴθ��˱����Ƿ�Ϊ�����ţ�����������������򲻼���
					int check_val = checkResultType(exp, res_item, all_testset);
					if (check_val == 1) res.push_back(res_item);
				}*/
				check_if_LRF = true;

			}
			else{
				string near_testcase = generateNearTestset(testset_item, change_literal, literal_abstract_pos); //�ٽ������²�������
				//// cout << "ȫ�ٽ����ٽ��������� " << near_testcase << endl;
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
				//// cout << "11111���ս��: " << res_items.size() << endl;
				//if (backup_literal.size() != 0){
					vector<string> res_items1 = fault_in_LRF(exp, fault_exp, testcase, nfp, literal_abstract_pos);
					for (string res_item : res_items1)
					{
						//// cout << "������ǰnfp��" << nfp.term << " ��Ը���ļ����: " << "��" << res_item << "��" << endl;
						//�ٴθ��˱����Ƿ�Ϊ�����ţ�����������������򲻼���
						int check_val = checkResultType(exp, res_item, all_testset);
						if (check_val == 1) res.push_back(res_item);
					}
				//}
			}
			for (string res_item : res_items)
			{
				//// cout <<"������ǰnfp��"<<nfp.term<<" ��Ը���ļ����: "<<"��"<< res_item <<"��"<< endl;
				//�ٴθ��˱����Ƿ�Ϊ�����ţ�����������������򲻼���
				int check_val=checkResultType(exp, res_item, all_testset);
				if (check_val==1) res.push_back(res_item);
			}
		}
		preprocess_interval += testcase.time_interval;
		cout << "������" << testcase.time_interval << endl;
	}
	//// cout << "���ս��: " << endl;
	for (string res_item :res)
	{
		//// cout << "��" << res_item << "��" << endl;
	}
	// vector<char> change;
	// change.push_back('c');
	// change.push_back('d');
	// string ressult=generateNearTestset("1010", change);
	// // cout << "�ٽ������ɺ��������"<<ressult << endl;
	// �������ɳ�������һ�����������Դ�����ʽ���вⶨ,�Ӷ�������֧����
	return res;
}

//ֻ�����ŵ��֧�����ɳ���ά��ֵ�ж��棩��expΪ������ʽ��testsetΪ����㼯���˴�Ϊ�ٵ㼯
vector<string> judgeAllExpand_ByLevel(string exp, string fault_exp, vector<string> testset, vector<string> all_testset)
{
	vector<string> res;
	Utility uti;
	string model = uti.generateModel(exp); //ȫ��������abcd
	unordered_map<char, int> literal_abstract_pos;  //ȫ����������Ӧλ�ù�ϵ
	for (int i = 0; i < model.size(); i++)
	{
		literal_abstract_pos.insert(pair<char, int>{ model[i], i });
	}
	//�������ݽṹ����ƣ��Դ��������������¿̻�
	//�����Ų��������
	for (string testset_item : testset)
	{
		bool is_vaild = uti.checkTestDiffer(testset_item, exp, fault_exp);
		if (is_vaild == false) continue;  //������Ч��������
		TestCase testcase = TestCase(exp, testset_item, false);
		//��ÿ�����������е�UTP/NFP�����Ų飬���ڴ����ź�����������NFP
		//list<UTPorNFP> nfps = testcase.getInfo(exp, testset_item, false);
		list<NFP> nfps = testcase.getNFP(exp, testset_item);
		for (NFP nfp : nfps)
		{
			// cout << "������ǰnfp��" << nfp.term << endl;
			//����ʹ��ȫ�ٽ����ж��Ƿ�����LRF���Ӷ�����������֧
			vector<char> change_literal;
			list<Literal> valid_literals = nfp.getValid();
			//bool is_sensitive = 1;  //�ж��Ƿ�ȫ�����е㣬ȡ1Ϊ���е�
			for (Literal literal : valid_literals)
			{
				change_literal.push_back(literal.getChar()); //������޸ı�������
			}
			bool check_if_LRF;
			string near_testcase = generateNearTestset(testset_item, change_literal, literal_abstract_pos); //�ٽ������²�������
			// cout << "ȫ�ٽ����ٽ��������� " << near_testcase << endl;
			//check_if_LRF = uti.evaluateMutant(exp, fault_exp, near_testcase);//��Ϊ�ж�����Ƿ���ά��δ��ά��LRF
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
			//// cout << "���ս��: " << endl;
			for (string res_item : res_items)
			{
				// cout << "������ǰnfp��" << nfp.term << " ��Ը���ļ����: " << "��" << res_item << "��" << endl;
				//�ٴθ��˱����Ƿ�Ϊ�����ţ�����������������򲻼���
				//int check_val = checkResultType(exp, res_item, all_testset);
				//if (check_val == 1) res.push_back(res_item);
				res.push_back(res_item);
			}
		}
		preprocess_interval += testcase.time_interval;
	}
	// cout << "���ս��: " << endl;
	for (string res_item : res)
	{
		// cout << "��" << res_item << "��" << endl;
	}
	// vector<char> change;
	// change.push_back('c');
	// change.push_back('d');
	// string ressult=generateNearTestset("1010", change);
	// // cout << "�ٽ������ɺ��������"<<ressult << endl;
	// �������ɳ�������һ�����������Դ�����ʽ���вⶨ,�Ӷ�������֧����
	return res;
}

//ֻ�����ŵ��֧�����ɳ�������Rule6���°棬expΪ������ʽ��testsetΪ����㼯���˴�Ϊ�ٵ㼯
vector<string> judgeAllExpand_ByRule6(string exp, string fault_exp, vector<string> testset, vector<string> all_testset){
	vector<string> res;
	Utility uti;
	string model = uti.generateModel(exp); //ȫ��������abcd
	unordered_map<char, int> literal_abstract_pos;  //ȫ����������Ӧλ�ù�ϵ
	for (int i = 0; i < model.size(); i++)
	{
		literal_abstract_pos.insert(pair<char, int>{ model[i], i });
	}
	//����һ��map�����ڼ�¼ÿ��nfp������Ч���������Ĵ���
	map<string, vector<string>> nfp_cell_map;
	map<string, set<char>> nfp_key_literal_map;
	map<string, int> nfp_times_map;
	map<string, int>::iterator iter;
	int useful_testcase_count = 0; //ͳ����Ч������������
    set<string> useful_testcase;
	//�������ݽṹ����ƣ��Դ��������������¿̻�
	//�����Ų��������
	for (string testset_item : testset)
	{
		bool is_vaild = uti.checkTestDiffer(testset_item, exp, fault_exp);
		if (is_vaild == false); //continue;  //������Ч��������
		else{
			useful_testcase_count += 1;
			useful_testcase.insert(testset_item);
		}
		TestCase testcase = TestCase(exp, testset_item, false);
		//��ÿ�����������е�UTP/NFP�����Ų飬���ڴ����ź�����������NFP
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
		// cout << "��ǰterm:" << iter->first << " ��ǰterm��nfc����:" << iter->second << " Tfail_0����:" << useful_testcase_count << "�ؼ�����������" << nfp_key_literal_map[iter->first].size() << endl;
		// cout << "��ǰterm��nfc:" ;
		for (string cell : nfp_cell_map[iter->first]){
			// cout << " " << cell;
		}
		// cout << endl;
		if (iter->second != useful_testcase_count || nfp_key_literal_map[iter->first].size()<=1) continue;
		bool equal = true;
		for (string cell : nfp_cell_map[iter->first]){
			equal = equal&(useful_testcase.find(cell) != useful_testcase.end());
		}
		// cout << "�����Ƿ����:"<<equal<<endl;
		if (equal == false) continue;
		corf_relate_terms.insert(iter->first);
	}
	if (corf_relate_terms.size() != 0 && useful_testcase_count>1){
		//CORF
		//����������Rule6��ǿ�������������ǿ������ֱ�����CORF��λ���
		//�������ǿ������������ִ���Ľ������

		// cout << "����CORF" << endl;
		map<string, int> res_testcase_map;//ͳ��ÿ��������ֵĴ���
		for (string testset_item : testset)
		{
			bool is_vaild = uti.checkTestDiffer(testset_item, exp, fault_exp);
			if (is_vaild == false) continue;  //������Ч��������
			TestCase testcase = TestCase(exp, testset_item, false);
			//��ÿ�����������е�UTP/NFP�����Ų飬���ڴ����ź�����������NFP
			//list<UTPorNFP> nfps = testcase.getInfo(exp, testset_item, false);
			list<NFP> nfps = testcase.getNFP(exp, testset_item);
			for (NFP nfp : nfps)
			{
				//TODO:������Ҫ�޸�һ�£���������LOF�ķ�֧
				//// cout << "CORF��֧" << endl;
				if (corf_relate_terms.find(nfp.term) == corf_relate_terms.end()){
					//�������CORF����ֱ�Ӹɵ�
					continue;
				}
				vector<string> res_items = fault_in_CORF_ByRule6(exp, fault_exp, testcase, nfp);
				for (string res_item : res_items)
				{
					//// cout << "������ǰnfp��" << nfp.term << " ��Ը���ļ����: " << "��" << res_item << "��" << endl;
					//�ٴθ��˱����Ƿ�Ϊ�����ţ�����������������򲻼���
					int check_val = checkResultType(exp, res_item, all_testset);
					if (check_val == 1)
					{
						if (res_testcase_map.find(res_item) == res_testcase_map.end()) res_testcase_map[res_item] = 1;
						else  res_testcase_map[res_item] = res_testcase_map[res_item] + 1;

					}
				}
			}
		}
		//ÿ�ֶ����ֵĽ���������ս����
		//// cout << " ��Ч���������� " << useful_testcase_count << endl;
		for (auto& res_testcase : res_testcase_map){
			//// cout << res_testcase.first << " ������ " << res_testcase.second << endl;
			//if (res_testcase.second == useful_testcase_count){
				res.push_back(res_testcase.first);
			//}
		}
	}
	//else{
		//LRF��LOF
		// cout << "����CORF��LOF��LRF" << endl;
		for (string testset_item : testset)
		{
			bool is_vaild = uti.checkTestDiffer(testset_item, exp, fault_exp);
			if (is_vaild == false) continue;  //������Ч��������
			TestCase testcase = TestCase(exp, testset_item, false);
			//��ÿ�����������е�UTP/NFP�����Ų飬���ڴ����ź�����������NFP
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
						//���滻�������м��в����ڣ������ٽ������������м�⣬�Ϸ������Ψһȡ����
						//�����ٽ������������ԣ���Ч��re��������
						vector<char> change_literal;
						change_literal.push_back(literal.getChar());
						string near_testcase = generateNearTestset(testcase.testcase, change_literal, literal_abstract_pos); //�ٽ������²�������
						//// cout << "   �ı�ı��� " << literal.getChar() << endl;
						//// cout << "   LRF�е������ٽ��������� " << near_testcase << endl;
						bool check_if_true = uti.evaluateMutant(exp,fault_exp, near_testcase);
						if (check_if_true) continue;  //ȡ1������ȡ0����Ψһȡ����
						first_replace_literals.push_back(literal);
					}
					else{
						back_replace_literals.push_back(literal);
					}
				}
				if (first_replace_literals.size() != 0){
					//����LRF
					// cout << "�������LRF" << first_replace_literals.size() << endl;
					vector<string> res_items = fault_in_LRF_ByRule6(exp, fault_exp, first_replace_literals, nfp);
					for (string res_item : res_items)
					{
						//// cout << "������ǰnfp��" << nfp.term << " ��Ը���ļ����: " << "��" << res_item << "��" << endl;
						//�ٴθ��˱����Ƿ�Ϊ�����ţ�����������������򲻼���
						int check_val = checkResultType(exp, res_item, all_testset);
						if (check_val == 1) res.push_back(res_item);
					}
				}
				else{
					if(back_replace_literals.size() != 0){
						//����LRF
						// cout << "���ɱ���LRF:" << back_replace_literals.size() << endl;
						vector<string> res_items = fault_in_LRF_ByRule6(exp, fault_exp, back_replace_literals, nfp);
						for (string res_item : res_items)
						{
							//// cout << "������ǰnfp��" << nfp.term << " ��Ը���ļ����: " << "��" << res_item << "��" << endl;
							//�ٴθ��˱����Ƿ�Ϊ�����ţ�����������������򲻼���
							int check_val = checkResultType(exp, res_item, all_testset);
							if (check_val == 1) res.push_back(res_item);
						}
					}
					//����LOF
					vector<string> res_items = fault_in_LOF_ByRule6(exp, fault_exp, testcase, nfp);
					for (string res_item : res_items)
					{
						 res.push_back(res_item);
					}
					//����LOF��CORF
					if (corf_relate_terms.size() == 0 || useful_testcase_count <= 1){
						res_items = fault_in_CORForLOF_ByRule6(exp, fault_exp, testcase, nfp);
						for (string res_item : res_items)
						{
							//// cout << "������ǰnfp��" << nfp.term << " ��Ը���ļ����: " << "��" << res_item << "��" << endl;
							//�ٴθ��˱����Ƿ�Ϊ�����ţ�����������������򲻼���
							int check_val = checkResultType(exp, res_item, all_testset);
							if (check_val == 1) res.push_back(res_item);
						}
					}
				}
			}
		}
	//}
	//// cout << "���ս��: " << endl;
	//for (string res_item : res)
	//{
		//// cout << "��" << res_item << "��" << endl;
	//}
	return res;
}

vector<string> generate_TOForDORF_mutant_inPos(string exp, string fault_exp, TestCase testCase, UTP utp, Literal insert_literal)
{
	// cout<<"��ǰutp:"<<utp.term<<"  TOF DORF" << endl;
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
		//�õ�ʹ��ǰtermΪ1��testData
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
	//����TOF���
	res.push_back(dnf_mutant.generateTOFmutants(exp, utp.term));
	//}

	return res;
}//�ڲ���������ɳ���

vector<string> fault_in_Inside(string exp, string fault_exp, TestCase testcase, UTP utp, unordered_map<char, int> literal_abstract_pos)
{
	vector<string> res;
	//vector<string> sesusceptible_res;
	//// cout << "���������" << endl;
	DnfMutant dnf_mutant;
	Utility uti;
	list<Literal> valid = utp.getInner();
	list<Literal>::iterator plist;
	vector<Literal> backup_not_set;
	for (plist = valid.begin(); plist != valid.end(); plist++)
	{
		//��鵱ǰԪ���Ƿ������б��������������ٽ������������
		//�ǣ�����������м����÷��ؼ�
		if (testcase.susceptibleLiterSet.find(*plist) == testcase.susceptibleLiterSet.end())
		{
			//�����ٽ������������ԣ���Ч��re��������
			vector<char> change_literal;
			change_literal.push_back((*plist).getChar());
			string near_testcase = generateNearTestset(testcase.testcase, change_literal, literal_abstract_pos); //�ٽ������²�������
			//// cout << "   �ı�ı��� " << (*plist).getChar() << endl;
			//// cout << "   �������ڴ��󡿵������ٽ��������� " << near_testcase << endl;
			//// cout << "   �������ڴ��󡿵������ٽ����������������  " << uti.evaluateMutant(exp, fault_exp, near_testcase) << endl;
			bool check_if_true = uti.evaluateMutant(exp, fault_exp, near_testcase);
			if (!check_if_true) continue;  //ȡ0������ȡ1�����Ψһȡ����
			//ȡ���λ�ɹ�������LNF
			//ע�⣺generateLNFmutant_inPosδʵ��
			string re = dnf_mutant.generateLNFmutant_inPos(exp, utp, *plist);
			res.push_back(re);
			// cout << re << endl;
			break;
		}
		else   //�ǣ��������п��ܵ�ȡ�����
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
		//����TOF��DORF����
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
			// cout << "TOF DORF branch �ⲿ" << endl;
			//if (testcase.susceptibleLiterSet.find(l) == testcase.susceptibleLiterSet.end()){
				// cout << "TOF DORF branch �ڲ�" << endl;
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
	//// cout << " ���ݲ��뼯��ȱʧ�����ɿ��ܵ�LRF "<< endl;
	vector<string> res;
	DnfMutant dnf_mutant;
	for (Literal insert_literal : insert_literals){
		for (Literal out_literal : out_literals){
			//// cout << "������:" << insert_literal.getChar() << "  ���滻��:" << out_literal.getChar() << endl;
			res.push_back(dnf_mutant.generateLRFmutant_inPos_UTP(exp,utp,insert_literal,out_literal));
		}
	}
	//������Ѿ���λ����λȱʧ�ʣ����Զ�λ�򷵻�LRF��������޷���λ�򷵻�LIF�����
	return res;
}

vector<string> generate_LIFmutant_inPos(string exp, UTP utp, vector<Literal> insert_literals)
{
	vector<string> res;
	DnfMutant dnf_mutant;
	//������Ѿ���λ����λȱʧ�ʣ����Զ�λ�򷵻�LRF��������޷���λ�򷵻�LIF�����
	for (Literal insert_literal : insert_literals){
		res.push_back(dnf_mutant.generateLIFmutant_inPos_UTP(exp, utp, insert_literal));
	}
	return res;
}

//�ⲿ��������ɳ���
vector<string> fault_in_Outside(string exp, string fault_exp, TestCase testcase, UTP utp, unordered_map<char, int> literal_abstract_pos, vector<Literal> insert_literal)
{
	//// cout << "�Ǳ��������" << endl;
	vector<string>  res;
	//ȡ������Ч����ʣ�ȫ��ȡ��������һ���µ��ٽ���������
	vector<char> change_literal;
	for (Literal literal : insert_literal)
	{
		change_literal.push_back(literal.getChar());
	}
	string testset_item = testcase.testcase;
	string near_testcase = generateNearTestset(testset_item, change_literal, literal_abstract_pos); //�ٽ������²�������
	//// cout << "�²���������㣺" <<near_testcase<< endl;
	//// cout << "ԭ���ʽ��" << exp << endl;
	TestCase new_testcase = TestCase(exp, near_testcase, true);
	//���²����������õ�����utp������ԭutp������utp��
	list<OTP> new_utps = new_testcase.getOTP(exp, near_testcase);
	/*// cout << "�²���������" << new_testcase.testcase << endl;
	for (OTP otp:new_utps)
	{
		// cout << "��utp��" << otp.term<< endl;
	}
	for (Literal literal1 : new_testcase.susceptibleLiterSet)
	{
		// cout << "�����б�����" << literal1.getChar() << endl;
	}*/
	//// cout << "��utp������" << new_utps.size() << endl;
	vector<Literal> possible_out_literals; //����ȱʧ��
	vector<Literal> backup_out_literals; //����ȱʧ��
	Utility uti;
	list<Literal> valid = utp.getInner();
	list<Literal>::iterator plist;
	//ȡ���������дʣ����μ���Ƿ������дʣ���������д���ֱ�Ӽ��뱸��ȱʧ��
	//�������������ȥ����ȱʧ�����õ�����ȱʧ��
	for (plist = valid.begin(); plist != valid.end(); plist++)
	{
		//��鵱ǰԪ���Ƿ������б��������������һ���ж�
		//�ǣ�����뱸��ȱʧ��
		if (new_testcase.susceptibleLiterSet.find(*plist) == new_testcase.susceptibleLiterSet.end())
		{
			//�������б���
			//������������utp���������Ƿ���ÿ��utp�ж����ڣ�����������ȱʧ����������뱸��ȱʧ��
			Literal literal_char = (*plist);
			if (new_utps.size() <= 1){
				//���û������utp��ֱ�Ӽ������ȱʧ��
				possible_out_literals.push_back(literal_char);
				continue;
			}
			int in_new_utps_count = 0;
			for (OTP new_utp : new_utps)
			{
				//// cout << "��ǰOTP��" << new_utp.term << endl;
				list<Literal> new_valid = new_utp.getInner();
				int is_in_this_utp = 0;
				for (Literal new_literal : new_valid){
					if (new_literal.getChar() == literal_char.getChar()){
						//��ǰ��������UTP�д���
						is_in_this_utp = 1;
						break;
					}
				}
				in_new_utps_count += is_in_this_utp;
			}
			if (in_new_utps_count == new_utps.size()){
				//��ǰ��������������utp�ж����ڻ򶼲�����
				possible_out_literals.push_back(literal_char);
			}
			else{
				backup_out_literals.push_back(literal_char);
			}
		}
		else
		{
			//�����б��������뱸��ȱʧ��
			backup_out_literals.push_back((*plist));
		}
	}
	//��������ȱʧ����������ʽ������ֵ���Ƿ�ɶ�λ��ȱʧ
	vector<Literal> unique_out_literals;
	for (Literal possible_out_literal : possible_out_literals){
		//����һ���ٽ�����������������ʽ������ֵ
		//Ϊ����ʹ��Ψһȱʧ�����ɲ�������������ʹ�ñ���ȱʧ�����ɲ�������
		vector<char> change_literal2;
		change_literal2.push_back(possible_out_literal.getChar());
		string near_testcase = generateNearTestset(new_testcase.testcase, change_literal2, literal_abstract_pos); //�ٽ������²�������
		//// cout << "   ����ȱʧ�� " << possible_out_literal.getChar() << endl;
		//// cout << "   ���ɵ��ٽ��������� " << near_testcase << endl;
		//// cout << "  ����������� " << fault_exp << endl;
		bool check_if_true = uti.evaluateMutant(exp,fault_exp, near_testcase);
		//// cout << " ����� " << check_if_true << endl;
		if (!check_if_true) continue;  //ȡ0�����Ǹ��������ֱ������
		//�Ѿ���λ��Ψһȱʧ��
		unique_out_literals.push_back(possible_out_literal);
		break;
	}
	if (unique_out_literals.size() != 0){
		//��λ��һ��ȱʧ������LRF
		vector<string>res_items = generate_LRFmutant_inPos(exp, utp, insert_literal, unique_out_literals);
		for (string res_item : res_items){
			//// cout << "���ɼ���LRF��"<<res_items.size() << endl;
			res.push_back(res_item);
		}
	}
	else{
		//δ��λ��ȱʧ�����ɱ��ü���LRF��LIF
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
//ֻ���������֧�����ɳ���,����Rule6���°棬expΪ������ʽ��testsetΪ����㼯���˴�Ϊ��㼯
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
			//����TOF
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
					//����DORF
					isDORF = true;
					res.push_back(dnfmutant.generateDORFmutants(exp, term1, term2));
				}
			}
		}
	}
	//�Ǳ������ LIF LRF
	if (isTOF || !isDORF){
		string model = uti.generateModel(exp);
		unordered_map<char, int> literal_abstract_pos;  //ȫ����������Ӧλ�ù�ϵ
		for (int i = 0; i < model.size(); i++)
		{
			literal_abstract_pos.insert(pair<char, int>{ model[i], i });
		}

		for (string testset_item : Tfail1){
			TestCase testcase = TestCase(exp, testset_item, true);
			list<UTP> utps = testcase.getUTP(exp, testset_item);
			UTP utp = *utps.begin(); //Ψһ���
			list<Literal> valid_literals = utp.getValid();
			bool is_sensitive = 1;  //�жϵ�ǰ���Ƿ������е㣬ȡ1Ϊ���е�
			vector<Literal> backup_insert_set;
			vector<Literal> insert_set;
			bool check_if_outside = false;
			for (Literal literal : valid_literals)
			{
				is_sensitive = (testcase.susceptibleLiterSet.find(literal) != testcase.susceptibleLiterSet.end());
				if (is_sensitive)
				{
					//�����б���
					//// cout << "�����б���" << literal.getChar() << literal.getPos() << endl;
					backup_insert_set.push_back(literal);
					continue;
				}
				vector<char> change_literal;
				change_literal.push_back(literal.getChar()); //������޸ı�������
				//�����ٽ�����������������ֵ
				string near_testcase = generateNearTestset(testset_item, change_literal, literal_abstract_pos); //�ٽ������²�������
				check_if_outside = uti.evaluateMutant(exp, fault_exp, near_testcase);
				if (check_if_outside)
				{
					//��λ����Ч�����������Ч���������outside
					insert_set.push_back(literal);
					break;
				}
			}
			if (check_if_outside) //��λ�����������
			{
				//�ⲿ����
				vector<string> res_items1 = fault_in_Outside(exp, fault_exp, testcase, utp, literal_abstract_pos, insert_set);
				for (string res_item : res_items1)
				{
					res.push_back(res_item);
				}
			}
			else	//û�ж�λ�������Ч����������ò�������outside��ͬʱ��ҲҪ���inside
			{
				if (backup_insert_set.size() != 0)
				{
					//�ⲿ����
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

//ֻ���������֧�����ɳ���expΪ������ʽ��testsetΪ����㼯���˴�Ϊ��㼯
//�°�ʵ�֣������ɳ����м���λ������ʼ���
vector<string> judgeAllShrink(string exp, string fault_exp, vector<string> testset, vector<string> all_testset)
{
	vector<string> res;
	Utility uti;
	string model = uti.generateModel(exp); //ȫ��������abcd
	unordered_map<char, int> literal_abstract_pos;  //ȫ����������Ӧλ�ù�ϵ
	for (int i = 0; i < model.size(); i++)
	{
		literal_abstract_pos.insert(pair<char, int>{ model[i], i });
	}
	//�������ݽṹ����ƣ��Դ��������������¿̻�
	//�����Ų��������
	int useful_testcase_count=0; //��Ч���������ĸ���
	for (string testset_item : testset)
	{
		bool is_vaild = uti.checkTestDiffer(testset_item, exp, fault_exp);
		if (is_vaild == false) continue;  //������Ч��������
		else useful_testcase_count += 1;
		TestCase testcase = TestCase(exp, testset_item, true);
		//��ÿ�����������е�UTP�����Ų飬���ڴ����ź�����������UTP
		list<UTP> utps = testcase.getUTP(exp, testset_item);
		UTP utp = *utps.begin(); //Ψһ���
		//// cout << "������ǰutp��" << utp.term << endl;
		//// cout << "����������" << testcase.testcase << endl;
		//for (Literal literal1 : testcase.susceptibleLiterSet)
		//{
		//	// cout << "���б�����" << literal1.getChar() << endl;
		//}
		list<Literal> valid_literals = utp.getValid();
		bool is_sensitive = 1;  //�жϵ�ǰ���Ƿ������е㣬ȡ1Ϊ���е�
		bool check_if_outside=false; //����Ƿ���Զ�λ�������
		//������ж��Ƿ������е�
		vector<Literal> insert_set;
		vector<Literal> backup_insert_set;
		for (Literal literal : valid_literals)
		{
			is_sensitive = (testcase.susceptibleLiterSet.find(literal) != testcase.susceptibleLiterSet.end());
			if (is_sensitive)
			{
				//�����б���
				//// cout << "�����б���" << literal.getChar() << literal.getPos() << endl;
				backup_insert_set.push_back(literal);
				continue;
			}
			//// cout << "��Ч���� " << literal.getChar() << literal.getPos() << endl;
			vector<char> change_literal;
			change_literal.push_back(literal.getChar()); //������޸ı�������
			//�����ٽ�����������������ֵ
			string near_testcase = generateNearTestset(testset_item, change_literal, literal_abstract_pos); //�ٽ������²�������
			check_if_outside = uti.evaluateMutant(exp, fault_exp, near_testcase);
			if (check_if_outside)
			{
				//��λ����Ч�����������Ч���������outside
				insert_set.push_back(literal);
				break;
			}
			preprocess_interval += testcase.time_interval;
		}
		if (check_if_outside) //��λ�����������
		{
			//�ⲿ����
			vector<string> res_items1 = fault_in_Outside(exp, fault_exp, testcase, utp, literal_abstract_pos,insert_set);
			for (string res_item : res_items1)
			{
				//���������
				if (checkResultType(exp, res_item, all_testset) != 2) continue;
				//// cout << "������ǰutp��" << utp.term << " ��Ը���ļ����: " << "��" << res_item << "��" << endl;
				res.push_back(res_item);
			}
		}
		else	//û�ж�λ�������Ч����������ò�������outside��ͬʱ��ҲҪ���inside
		{
			if (backup_insert_set.size()!=0)
			{
				//�ⲿ����
				vector<string> res_items1 = fault_in_Outside(exp, fault_exp, testcase, utp, literal_abstract_pos, backup_insert_set);
				for (string res_item : res_items1)
				{
					if (checkResultType(exp, res_item, all_testset) != 2) continue;
					//// cout << "������ǰutp��" << utp.term << " ��Ը���ļ����: " << "��" << res_item << "��" << endl;
					res.push_back(res_item);
				}
			}
			//�ڲ�����
			vector<string> res_items2 = fault_in_Inside(exp, fault_exp, testcase, utp, literal_abstract_pos);
			// cout << "TOF\DORF��֧����������ģ��" << res_items2.size() << endl;
			for (string res_item : res_items2)
			{
				//if (checkResultType(exp, res_item, all_testset) != 2) continue;
				// cout << "������ǰutp��" << utp.term << " ��Ը���ļ����: " << "��" << res_item << "��" << endl;
				res.push_back(res_item);
			}
		}
		preprocess_interval += testcase.time_interval;
	}
	//// cout << "���ս��: " << endl;
	map<string, int> res_map;
	map<string, int>::iterator iter;
	for (string res_item : res)
	{
		iter = res_map.find(res_item);
		if (iter == res_map.end()) res_map[res_item] = 1;
		else res_map[res_item] = res_map[res_item] + 1;
		//ʹ��һ��map��ͳ��ÿ��������������Ĵ�����ֻ���ÿ�ֶ����������Ľ��
	}
	vector<string> res_new;
	for (iter = res_map.begin(); iter != res_map.end(); iter++){
		//if (iter->second != useful_testcase_count) continue;
		res_new.push_back(iter->first);
		//// cout << "��" << iter->first << "��" << endl;
	}
	return res;
}

//�Ǵʴ��������ϴ��󣩴����ɳ���

vector<string> fault_in_Term(string exp, string fault_exp, vector<string> true_testset, vector<string> false_testset, vector<string> all_testset){
	vector<string> res;
	Utility uti;
	DnfMutant dnfmutant;
	bool all_valid = true;
	// cout << "���������" << endl;
	for (int i = 0; i < true_testset.size(); ++i)
	{
		all_valid = all_valid&uti.checkTestDiffer(true_testset[i], exp, fault_exp);
	}
	if (all_valid){
		//ENF
		//���ENF���
		string mutant = dnfmutant.generateENFmutant(exp);
		res.push_back(mutant);
		// cout << " ENF---> " << mutant << endl;
	}
	else{
		// cout << "����������TNF��λ" << endl;
		//TNF
		//��λTNF����TNF���
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
//�ʴ���LNF��Ч�������

vector<string> fault_in_Literal_LNF_OLD(string exp, string fault_exp, TestCase testcase, UTP utp, unordered_map<char, int> literal_abstract_pos)
{
	vector<string> res;
	//vector<string> sesusceptible_res;
	//// cout << "���������" << endl;
	DnfMutant dnf_mutant;
	Utility uti;
	list<Literal> valid = utp.getInner();
	list<Literal>::iterator plist;
	vector<Literal> backup_not_set;
	for (plist = valid.begin(); plist != valid.end(); plist++)
	{
		//��鵱ǰԪ���Ƿ������б��������������ٽ������������
		//�ǣ�����������м����÷��ؼ�
		if (testcase.susceptibleLiterSet.find(*plist) == testcase.susceptibleLiterSet.end())
		{
			//�����ٽ������������ԣ���Ч��re��������
			vector<char> change_literal;
			change_literal.push_back((*plist).getChar());
			string near_testcase = generateNearTestset(testcase.testcase, change_literal, literal_abstract_pos); //�ٽ������²�������
			//// cout << "   �ı�ı��� " << (*plist).getChar() << endl;
			//// cout << "   �������ڴ��󡿵������ٽ��������� " << near_testcase << endl;
			bool check_if_true = uti.evaluate(fault_exp, near_testcase);
			if (!check_if_true) continue;  //ȡ0������ȡ1�����Ψһȡ����
			//ȡ���λ�ɹ�������LNF
			//ע�⣺generateLNFmutant_inPosδʵ��
			string re = dnf_mutant.generateLNFmutant_inPos(exp, utp, *plist);
			res.push_back(re);
			//// cout << re << endl;
			break;
		}
		else   //�ǣ��������п��ܵ�ȡ�����
		{
			backup_not_set.push_back(*plist);

		}

	}
	if (res.size() != 0) return res;
	else
	{
		//����TOF��DORF����
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
	//// cout << "�ʴ���LNF���" << endl;
	Utility uti;
	DnfMutant dnfmutant;
	vector<string> res;
	//�����ٵ㼯���ҵ���Ч���ŵ�
	for (string point : false_testset){
		if (uti.checkTestDiffer(point, exp, fault_exp)){
			TestCase tc = TestCase(exp, point, false);
			list<NFP> nfps = tc.getNFP(exp, point);
			//�õ���Ч���ŵ��NFP
			//��UTPȡ����
			//�õ���LNF���ÿ�ζ�ȡ����
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

//�ʴ���LRF��Ч�����Ӳ�

vector<string> fault_in_Literal_LRF(string exp, string fault_exp, TestCase testcase, UTP utp, unordered_map<char, int> literal_abstract_pos, vector<Literal> insert_literal, vector<string> false_testset)
{
	//// cout << "�Ǳ��������" << endl;
	vector<string>  res;
	Utility uti;
	for (string testset_item:false_testset)
	{
		//�ȼ����Ч�ԣ�������Ч��������
		bool valid=uti.checkTestDiffer(testset_item, exp, fault_exp);
		if (!valid) continue;
		TestCase false_testcase = TestCase(exp,testset_item,false);
		list<NFP> nfps = false_testcase.getNFP(exp, testset_item);
		for (NFP nfp : nfps)
		{
			if (nfp.term != utp.term) continue;
			//�ҵ����滻�ʣ�ֱ������LRF
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

//�ʴ�������ɳ���
vector<string> fault_in_Literal(string exp, string fault_exp, vector<string> true_testset, vector<string> false_testset, vector<string> all_testset){
	vector<string> res;
	//// cout << " �ʴ�������� " << endl;
	Utility uti;
	string model = uti.generateModel(exp); //ȫ��������abcd
	unordered_map<char, int> literal_abstract_pos;  //ȫ����������Ӧλ�ù�ϵ
	for (int i = 0; i < model.size(); i++)
	{
		literal_abstract_pos.insert(pair<char, int>{ model[i], i });
	}
	//�������ݽṹ����ƣ��Դ��������������¿̻�
	//�����Ų��������
	int useful_testcase_count = 0; //��Ч���������ĸ���
	for (string testset_item : true_testset)
	{
		bool is_vaild = uti.checkTestDiffer(testset_item, exp, fault_exp);
		if (is_vaild == false) continue;  //������Ч��������
		else useful_testcase_count += 1;
		TestCase testcase = TestCase(exp, testset_item, true);
		//��ÿ�����������е�UTP�����Ų飬���ڴ����ź�����������UTP
		list<UTP> utps = testcase.getUTP(exp, testset_item);
		UTP utp = *utps.begin(); //Ψһ���
		//// cout << "������ǰutp��" << utp.term << endl;
		list<Literal> valid_literals = utp.getValid();
		bool is_sensitive = 1;  //�жϵ�ǰ���Ƿ������е㣬ȡ1Ϊ���е�
		bool check_if_outside = false; //����Ƿ���Զ�λ�������
		//������ж��Ƿ������е�
		vector<Literal> insert_set;
		vector<Literal> backup_insert_set;
		for (Literal literal : valid_literals)
		{
			is_sensitive = (testcase.susceptibleLiterSet.find(literal) != testcase.susceptibleLiterSet.end());
			if (is_sensitive)
			{
				//�����б���
				//// cout << "�����б���" << literal.getChar() << literal.getPos() << endl;
				backup_insert_set.push_back(literal);
				continue;
			}
			//// cout << "��Ч���� " << literal.getChar() << literal.getPos() << endl;
			vector<char> change_literal;
			change_literal.push_back(literal.getChar()); //������޸ı�������
			//�����ٽ�����������������ֵ
			string near_testcase = generateNearTestset(testset_item, change_literal, literal_abstract_pos); //�ٽ������²�������
			check_if_outside = uti.evaluateMutant(exp, fault_exp, near_testcase);
			if (check_if_outside)
			{
				//��λ����Ч�����������Ч���������outside
				insert_set.push_back(literal);
				break;
			}
			preprocess_interval += testcase.time_interval;
		}
		if (check_if_outside) //��λ�����������
		{
			//LRF��������+���Ž���
			vector<string> res_items1 = fault_in_Literal_LRF(exp, fault_exp, testcase, utp, literal_abstract_pos, insert_set,false_testset);
			for (string res_item : res_items1)
			{
				//���������
				if (checkResultType(exp, res_item, all_testset) != 3) continue;
				//// cout << "������ǰutp��" << utp.term << " ��Ը���ļ����: " << "��" << res_item << "��" << endl;
				res.push_back(res_item);
			}
		}
		else	//û�ж�λ�������Ч����������ò�������outside��ͬʱ��ҲҪ���inside
		{
			if (backup_insert_set.size() != 0)
			{
				//LRF��������+���Ž���
				vector<string> res_items1 = fault_in_Literal_LRF(exp, fault_exp, testcase, utp, literal_abstract_pos, backup_insert_set,false_testset);
				for (string res_item : res_items1)
				{
					if (checkResultType(exp, res_item, all_testset) != 3) continue;
					//// cout << "������ǰutp��" << utp.term << " ��Ը���ļ����: " << "��" << res_item << "��" << endl;
					res.push_back(res_item);
				}
			}
			//LNF��������+���Ž���
			vector<string> res_items2 = fault_in_Literal_LNF(exp, fault_exp, testcase, utp, literal_abstract_pos, false_testset);
			for (string res_item : res_items2)
			{
				if (checkResultType(exp, res_item, all_testset) != 3) continue;
				//// cout << "������ǰutp��" << utp.term << " ��Ը���ļ����: " << "��" << res_item << "��" << endl;
				res.push_back(res_item);
			}
		}
	}
	//// cout << "���ս��: " << endl;
	map<string, int> res_map;
	map<string, int>::iterator iter;
	for (string res_item : res)
	{
		iter = res_map.find(res_item);
		if (iter == res_map.end()) res_map[res_item] = 1;
		else res_map[res_item] = res_map[res_item] + 1;
		//ʹ��һ��map��ͳ��ÿ��������������Ĵ�����ֻ���ÿ�ֶ����������Ľ��
	}
	vector<string> res_new;
	for (iter = res_map.begin(); iter != res_map.end(); iter++){
		if (iter->second != useful_testcase_count) continue;
		res_new.push_back(iter->first);
		//// cout << "��" << iter->first << "��" << endl;
	}
	return res;
}

//�������������ɳ���
vector<string> judgeShrinkAndExpand(string exp, string fault_exp, vector<string> true_testset, vector<string> false_testset, vector<string> all_testset){
	vector<string> res;
	//���ȱ����ٵ㼯�������Ч���ŵĸ����������Ч���Ÿ���С�ڼٵ㼯������תLRF��LNF
	//����תTNF��ENF
	Utility uti;
	bool all_valid = true;
	for (int i = 0; i < false_testset.size(); ++i)
	{
		all_valid = all_valid&uti.checkTestDiffer(false_testset[i], exp, fault_exp);
	}
	if (all_valid){
		//�����ϴ��󣬼�ENF��TNF
		vector<string> res_items1 = fault_in_Term(exp, fault_exp, true_testset, false_testset, all_testset);
		for (string res_item : res_items1)
		{
			//���������
			if (checkResultType(exp, res_item, all_testset) != 3) continue;
			//// cout << " �����ϴ�������: " << "��" << res_item << "��" << endl;
			res.push_back(res_item);
		}
	}
	else{
		//�ʴ��󣬼�LRF��LNF
		vector<string> res_items1 = fault_in_Literal(exp, fault_exp, true_testset, false_testset,all_testset);
		for (string res_item : res_items1)
		{
			//���������
			if (checkResultType(exp, res_item, all_testset) != 3) continue;
			//// cout <<" �ʴ�������: " << "��" << res_item << "��" << endl;
			res.push_back(res_item);
		}
	}
	return res;
}
//�����ֺ���
void extractFalseType(string expression, vector<string>testset)
{
	//�������������ֹ���
	/*
	�ҹ��ܺ���:��������������ʽ�õ����ʽֵ  bool Utility::evaluate(string exp,string testString)
	��/д����ٵ���ຯ��
	�ң������Լ��Ƿ�ȫͨ�����ʽ���ĺ���
	ʵ�֣��Բ�ι�ϵͼ���м�֦
	*/
	// cout << "������������������������������������������������" << endl;
	// cout << "�����������ʽ" << endl;
	string false_expression;
	while (cin >> false_expression)
	{
		Utility uti;
		// << "�����������ʽ" << endl;
		//false_expression = "a+cd+b!c";
		//���ݲ�����������ԭ���ʽ��ȡֵ������������Ϊ��㼯�ͼٵ㼯
		vector<string> true_testset;
		vector<string> false_testset;
		for (int i = 0; i < testset.size(); ++i)
		{
			if (uti.evaluate(expression, testset[i]) == true) true_testset.push_back(testset[i]);
			else false_testset.push_back(testset[i]);
		}
		/*// cout << "���������" << endl;
		for (int i = 0; i < true_testset.size(); ++i){
		// cout << true_testset[i] << endl;
		}*/
		//ʹ����㼯�ͼٵ㼯�ֱ�Դ�����ʽ���м�⣬�ж��Ƿ�ȫ��ͨ����⣬�Դ���Ϊ�����ֵ����
		bool isAllPassed_T = isAllPassedTest(expression, false_expression, true_testset);
		bool isAllPassed_F = isAllPassedTest(expression, false_expression, false_testset);
		if (isAllPassed_T && isAllPassed_F)
		{
			// cout << "��С��������ԭ���ʽһ�£�ͨ�����" << endl;
			return;
		}
		vector<string> res;
		if (isAllPassed_T == true)
		{
			// cout << "ֻ�����ŵ�" << endl;
			if(algo_mode==true) res=judgeAllExpand(expression, false_expression, false_testset,testset);
			//judgeAllExpand_ByLevel(expression, false_expression, false_testset, testset);
			else res=judgeAllExpand_ByRule6(expression, false_expression, false_testset, testset);
		}
		else if (isAllPassed_F == true)
		{
			// cout << "ֻ��������" << endl;
			if (algo_mode == true) res = judgeAllShrink(expression, false_expression, true_testset, testset);
			else res = judgeAllShrink_ByRules(expression, false_expression, true_testset, testset);
		}
		else
		{
			// cout << "����������Ҳ�����ŵ�" << endl;
			res=judgeShrinkAndExpand(expression, false_expression, true_testset, false_testset,testset);
		}
		//ȥ��
		set<string>s(res.begin(), res.end());
		res.assign(s.begin(), s.end());
		// cout << "�����ս����: " << endl;
		for (string res_item : res){
			// cout << "     ��" << res_item << "��" << endl;
		}
		// cout << "�����ּ���ٽ������������ɴ�����: " << call_generateNearTestset_times << endl;
		call_generateNearTestset_times = 0;
		// cout<< "������������������������������������������������" << endl;
		// cout<< "�����������ʽ" << endl;
	}
}

//����ԭ���ʽ�ʹ�����ʽ����������
vector<string> ordinaryAlgoTestOne(string expression, vector<string>testset, string false_expression){
	Utility uti;
	vector<string> res;
	//���μ�⣬�õ�������ʽ����������
	string magnitude = string();
	for (int i = 0; i < testset.size(); ++i)
	{
		if (uti.evaluateMutant(expression, false_expression, testset[i]) == true) magnitude.push_back(1);
		else magnitude.push_back(0);
	}
	//���ɵ�ǰ���ʽ�����б��壬��ÿ���������������������������ʽһ�µļ�������
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

//�����㷨������
void ordinaryAlgo(string expression, vector<string>testset)
{
	//����ȫ������
	//��ÿ��������ò���ר�ú���
	// cout << "������������������������������������������������" << endl;
	// cout << "�����������ʽ" << endl;
	string false_expression;
	while (cin >> false_expression)
	{
		vector<string> res;
		res = ordinaryAlgoTestOne(expression, testset, false_expression);
		//ȥ��
		set<string>s(res.begin(), res.end());
		res.assign(s.begin(), s.end());
		// cout << "�����ս����: " << endl;
		for (string res_item : res){
			// cout << "     ��" << res_item << "��" << endl;
		}
		// cout << "�����ּ���ٽ������������ɴ�����: " << call_generateNearTestset_times << endl;
		call_generateNearTestset_times = 0;
		// cout << "������������������������������������������������" << endl;
		// cout << "�����������ʽ" << endl;
	}

}

//����ר�����ݽṹ

string filename = "test2";
string filename_old = filename;
string filename_new = filename + "\\byRule6";
string filename_ordinary = filename + "\\byOrdinary";

class Character_Of_Mutant
{
	public:
		string false_expression; //������ʽ
		string character; //�������������Ż��Ǹ�����
		vector<string> res; //�����
		string false_type; //LRF��LOF��
		int generateNearTestset_times=0; //��;�����ٽ����������Ĵ���
		time_t time_interval; //��⵱ǰ������ʱʱ��
		time_t preprocess_interval; //Ԥ����ģ�鴦��ʱ��
		time_t detect_interval; //���ʱ��
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
		int origin_testcase_size=0; //ԭ���Ĳ�����������С
		int add_testcase_size=0; //����������������С
		int all_testcase_size=0; //�ܲ�����������С
		int avg_res_size = 0;//��ƽ���������С
		map<string,int> max_add_testcase_size;//ÿ�ִ�����Ҫ��չ�Ĳ����������������ֵ
		map<string,int> min_add_testcase_size;//ÿ�ִ�����Ҫ��չ�Ĳ����������������ֵ
		double avg_generateNearTestset_times=0;//���б�����Ϊ������ʽ������ɲ�������������ƽ��ֵ
		map<string, int> max_res_size;//ÿ�ִ���õ������������С
		map<string, int> min_res_size;//ÿ�ִ���õ�����С�������С
		time_t avg_time_interval; //��⵱ǰ������ʱʱ��
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
				//����ÿ�ִ�����ô������ֵ
				if (max_res_size[character_of_mutant.false_type]<character_of_mutant.res.size())
					max_res_size[character_of_mutant.false_type] = character_of_mutant.res.size();
				//����ÿ�ִ�����ô�����Сֵ
				if (min_res_size[character_of_mutant.false_type]>character_of_mutant.res.size())
					min_res_size[character_of_mutant.false_type] = character_of_mutant.res.size();
			}
		}

};

Exp_Mutants_Result exp_mutants_result;
//�����ֺ���,����ר��
Character_Of_Mutant extractFalseType_ForTest(string expression, vector<string>testset, string false_expression) 
{
	//�������������ֹ���
	/*
	�ҹ��ܺ���:��������������ʽ�õ����ʽֵ  bool Utility::evaluate(string exp,string testString)
	��/д����ٵ���ຯ��
	�ң������Լ��Ƿ�ȫͨ�����ʽ���ĺ���
	ʵ�֣��Բ�ι�ϵͼ���м�֦
	*/
		Character_Of_Mutant character_of_mutant;
		timeb start;
		ftime(&start);
		character_of_mutant.false_expression = false_expression;
		Utility uti;
		// << "�����������ʽ" << endl;
		//false_expression = "a+cd+b!c";
		//���ݲ�����������ԭ���ʽ��ȡֵ������������Ϊ��㼯�ͼٵ㼯
		vector<string> true_testset;
		vector<string> false_testset;
		for (int i = 0; i < testset.size(); ++i)
		{
			if (uti.evaluate(expression, testset[i]) == true) true_testset.push_back(testset[i]);
			else false_testset.push_back(testset[i]);
		}
		/*// cout << "���������" << endl;
		for (int i = 0; i < true_testset.size(); ++i){
		// cout << true_testset[i] << endl;
		}*/
		//ʹ����㼯�ͼٵ㼯�ֱ�Դ�����ʽ���м�⣬�ж��Ƿ�ȫ��ͨ����⣬�Դ���Ϊ�����ֵ����
		bool isAllPassed_T = isAllPassedTest(expression, false_expression, true_testset);
		bool isAllPassed_F = isAllPassedTest(expression, false_expression, false_testset);
		if ((isAllPassed_T || true_testset.size() == 0) &&(isAllPassed_F ||false_testset.size() == 0))
		{
			// cout << "��С��������ԭ���ʽһ�£�ͨ�����" << endl;
			character_of_mutant.character = "��С��������ԭ���ʽһ�£�ͨ�����";
		}
		else if (isAllPassed_T == true)
		{
			// cout << "ֻ�����ŵ�" << endl;
			//judgeAllExpand(expression, false_expression, false_testset,testset);
			character_of_mutant.character = "������";
			if (algo_mode) character_of_mutant.res = judgeAllExpand(expression, false_expression, false_testset, testset);
			else character_of_mutant.res = judgeAllExpand_ByRule6(expression, false_expression, false_testset, testset);
		}
		else if (isAllPassed_F == true)
		{
			// cout << "ֻ��������" << endl;
			character_of_mutant.character = "������";
			if (algo_mode) character_of_mutant.res = judgeAllShrink(expression, false_expression, true_testset, testset);
			else character_of_mutant.res = judgeAllShrink_ByRules(expression, false_expression, true_testset, testset);
		}
		else
		{
			// cout << "����������Ҳ�����ŵ�" << endl;
			character_of_mutant.character = "����������";
			character_of_mutant.res = judgeShrinkAndExpand(expression, false_expression, true_testset,false_testset,testset);
		}
		//�������ݺ���
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

//�����㷨������ר��
Character_Of_Mutant ordinaryAlgo_ForTest(string expression, vector<string>testset, string false_expression){
	Character_Of_Mutant character_of_mutant;
	timeb start;
	ftime(&start);
	character_of_mutant.false_expression = false_expression;
	character_of_mutant.character = "�����㷨";
	character_of_mutant.res = ordinaryAlgoTestOne(expression, testset, false_expression);
	//�������ݺ���
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
	д��һ���ļ����ļ���Ϊ������ʽ_NearTestResult��
	�����ű�����ʽ+�������С+�Ƿ�����

	д��һ�����ļ����ļ���Ϊ25all__NearTestResult
	����Ϊ��ԭ���ʽ+������
	25�����ʽƽ��������
	*/
	for (string testcase : testset){
		near_testcase_set.insert(testcase);
	}
	exp_mutants_result.origin_testcase_size = testset.size();
	ofstream oFile;
	string path = "E:\\FaultTable\\" + filename + "\\" + to_string(ti) + "_" + expression.substr(0,15) + "_NearTestResult.csv";
	oFile.open(path);
	oFile << "ԭ���ʽ" << "," << expression << endl;
	//���ļ��ж���ԭ���ʽ
	//�Ա��ʽ��������LRF�ͱ���
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
				character_of_mutant.get_is_hit(); //�����Ƿ�����
				//����ÿ�ִ�����ô������ֵ
				if (exp_mutants_result.max_add_testcase_size[character_of_mutant.false_type]<character_of_mutant.generateNearTestset_times)
					exp_mutants_result.max_add_testcase_size[character_of_mutant.false_type] = character_of_mutant.generateNearTestset_times;
				//����ÿ�ִ�����ô�����Сֵ
				if (exp_mutants_result.min_add_testcase_size[character_of_mutant.false_type]>character_of_mutant.generateNearTestset_times)
					exp_mutants_result.min_add_testcase_size[character_of_mutant.false_type] = character_of_mutant.generateNearTestset_times;
				test_new_algo.push_back(character_of_mutant);
				exp_mutants_result.avg_generateNearTestset_times += character_of_mutant.generateNearTestset_times;
				exp_mutants_result.avg_res_size += character_of_mutant.res.size();
			}
			catch (...)
			{
				oFile << "������ʽ" << "," << fault_expression << "," << "ִ���쳣" << "," << endl;
				continue; 
			}
			//if(i>=4) break;//ÿ������ֻ������
		//}
	}
	exp_mutants_result.avg_generateNearTestset_times /= mutants1.mutants.size();
	exp_mutants_result.all_testcase_size = near_testcase_set.size();
	exp_mutants_result.add_testcase_size = exp_mutants_result.all_testcase_size - exp_mutants_result.origin_testcase_size;
	exp_mutants_result.avg_res_size /= mutants1.mutants.size();
	exp_mutants_result.character_of_mutants = test_new_algo;
	preprocess_interval = 0;
	near_testcase_set.clear();
	int mutant_count;//�������
	int all_expand_multant_count;//�����ű������
	int is_hit_count;//��������б������
	oFile << "������ʽ" << "," << "�������С" << "," << "������Ƿ����д����" << "," << "��������" << "," << "��������" << "," << "�����ٽ�������������" << "," << "���ʱ����ms��" << "," << "Ԥ����ʱ��" << "," << "����ʱ��" << endl;
	for (Character_Of_Mutant cha : test_new_algo)
	{
		//// cout << cha.false_expression << " : " << cha.character << " " << cha.res.size() << " ������Ƿ����д�����ʽ��"<<cha.get_is_hit()<< endl;
		mutant_count += 1;
		if (true)//(cha.character == "ֻ�����ŵ�")
		{
			bool is_hit=cha.is_hit;
			all_expand_multant_count += 1;
			if (is_hit || cha.character == "��С��������ԭ���ʽһ�£�ͨ�����") is_hit_count += 1;
			//���ļ�д��
			//oFile.open(path, ios::out | ios::trunc);
			oFile << cha.false_expression << "," << cha.res.size() << "," << is_hit << "," << cha.character << "," << cha.false_type << "," << cha.generateNearTestset_times << "," << cha.time_interval << "," << cha.preprocess_interval << "," << cha.detect_interval << endl;
		}
	}
	double hit_rate = ((double)is_hit_count / (double)all_expand_multant_count) * 100;
	//oFile.open(path, ios::out | ios::trunc);
	oFile << "��������" << "," << hit_rate << endl;
	return hit_rate;
}

//���㷨��������ڣ�ǰ�벿�ֺ�ѧ�����ɱ�����ͬ��ɾȥ�����ɱ��,���ر���
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
	//�ֽ���ʽ   ������ʽ��utp otp fp �ȵ�
	PRE_PROCESS_DATA preData; MutantsandValidInvalidTestCase;
	vector<string> terms = uti.extractDNFstyle(expression); //�ֽ���ʽ����
	string model = uti.generateModel(expression);  //model����ԭ���ʽ�����������е���ĸ(�ź���ģ������� abce+cd+ef  abcdef
	string newExp = "";
	//�õ�IDNF
	//newExp = generateIDNForICNF(expression, true);
	//newExp = generateDNF(terms);
	newExp = expression;
	getPreProcessData(newExp, preData, true);
	vector<string> allPoint;
	CCommonFuncs commonFuncs;
	int literalNum = preData.literalCount;
	int dimensionNum = (literalNum + 1) / 2;
	bool vOdd = (literalNum % 2 == 0) ? false : true;
	//��ȡ���е�OTP
	vector<string> op = preData.op;
	vector<int> allOPs;
	for (int i = 0; i < op.size(); i++)
	{
		int value = commonFuncs.getTModelIndexOf01Str(op.at(i), vOdd);
		allOPs.push_back(value);
	}

	sort(allOPs.begin(), allOPs.end());

	//��ȡ���е�UTP��
	vector<int> allUPs;
	vector<string> up = preData.up;
	for (int i = 0; i < up.size(); i++)
	{
		int value = commonFuncs.getTModelIndexOf01Str(up.at(i), vOdd);
		allUPs.push_back(value);
	}
	sort(allUPs.begin(), allUPs.end());
	/**8888888888888888888888888888**/
	/*termsDimension ��ʾ����ÿһ�����ʹ���ʽֵȡ1��ȡֵ������ ab+cd  model=��abcd�� û2����ĸ��һά
	11(��һ��� ��һά��ȡֵ��
	00 01 11 10����һ�������ά��ȡֵ��

	00 01 11 10���ڶ���ĵ�һά��ȡֵ��
	11���ڶ��������ά��ȡֵ��
	*/
	/*
	nanjia������һ�������ʹ�õ����ݽṹ���������ǹ������е㡢���滻����Ϣ
	*/
	printf("\npreData.termsDimension:\n");
	for (int i = 0; i < preData.termsDimension.size(); i++){
		for (int j = 0; j < preData.termsDimension[i].size(); j++){
			printf("%s\n", preData.termsDimension[i][j].c_str());
		}
		printf("\n");
	}
	//��ι�ϵmap
	hash_map<string, HierarchyNode> hierarchyMap;

	vector<HierarchyNode> hierarchyEntry;

	//��ʼ����ι�ϵ
	uti.initHierarchyRelation(&hierarchyMap, &hierarchyEntry);
	//ʵ�ֶԲ�ι�ϵ���м�֦�ĺ�������ҪTtrue��Tfalse�����
	/**8888888888888888888888888888**/

	//��ȡ�Ƿ��е��ʵ��ж�
	bool isSingleLitral = false;
	//����ռȫ��ά����Ŀ
	int fullDemNum = 0;
	//����ռ���ά����Ŀ
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
		for (int i = 0; i < preData.termsDimension.size(); i++){//���term���н���
			/*
			dimensionValuesStr�����i���ÿһά��ת��ֵ��eg
			3
			1 2 3 4
			��һ�б�ʾ��i���һά��ֵ���ڶ��б�ʾ�ڶ�ά��ֵ��
			*/
			string dimensionValuesStr;
			//��һ��ת��Ϊ��ά�任������11 10 00 01 11 10 ������3 4 1 2 3 4����34 1234
			for (int j = 0; j < dimensionNum; j++)
			{
				//����ά��01��ʾ����ת��Ϊ1,2,3,4��ʾ��
				string line = termsDimension[i][j];
				string tempStr;
				commonFuncs.transDimensionLineFrom01totModelIndex(line, tempStr);
				//// cout <<"tempstr: "<< tempStr << endl;
				dimensionValuesStr.append(tempStr);
				int strLength = tempStr.length();
				if (vOdd != true || j != dimensionNum - 1) //����ռȫʱ���ַ�������ӦΪ7,ռ�볤��Ϊ3
				{
					if (strLength == 7)
						fullDemNum++;
					else if (strLength == 3)
						halfDemNum++;
				}
				else //����ռȫʱ���ַ�������ӦΪ3,ռ�볤��Ϊ1
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


			//����ȱ����Ϣת������Ҫ�ĸ񸲸�,�ڴ˴�Ҫ���и���
			mutantsDimensionValue.clear();
			corresponding_tests.clear();
			//����˼·
			//���ȸ��ݱ���õ����б���Ĵ���㣬��ν����㣬���Ǳ����뱾�岻ͬ����ֵ�㣬������������ֵ�㣬Ҳ����������ֵ��
			//Ȼ����ݴ�����鷽���飬һ������һ�������飬����������ĸ���Ϊ���д����
			//Ȼ�����ÿ������Ĵ����ȷ��ÿ�����̵ı�������
			//�᷽����õ����ŵı����������ɣ����в��漰�����ʽ�����ֵ����������������߼�ԭ����ο����ʽ

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
			if (hasAllLitral || isSingleLitral) //�����ǰ�����������ĸ���޷�����LIF ���� ֻ��һ���� ���˻���TOF
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

	//ת��Լ������
	//�õ����е���ֵ�㣬��һ���������ѭ����
	commonFuncs.getAlltModelIndexAppearedInMatrix(matrixCells, tModelIndexAppeared);
	//������������������311����312֮ǰ
	sort(tModelIndexAppeared.begin(), tModelIndexAppeared.end());

	commonFuncs.getCoefficientMatrix(tModelIndexAppeared, matrixCells, coefficientMatrix);

	vector<vector<int>>finalCoefficientMatrix;
	vector<int>finalTModelIndex;
	removeAllZeroCols(coefficientMatrix, tModelIndexAppeared, finalCoefficientMatrix, finalTModelIndex);

	vector<int>().swap(tModelIndexAppeared);//ֻ��Ϊ�˴�ӡ��
	vector<vector<int>>().swap(coefficientMatrix);//ֻ��Ϊ�˴�ӡ��
	matrixCells.clear();

	//Glpk��Լ������
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
	//�����޸ģ�����Ҫÿ�ζ�����
	vector<string> mutants_DP;
	for (int num = 0; num < mutants.size(); num++)
	{
		//printf("%s,%d,mutants[%d]:[%s]\r\n", __FILE__, __LINE__, num, mutants[num].c_str());
		mutants_DP.push_back(mutants[num]);
	}
	glpk_Solve.run(colSelected, &finalCoefficientMatrix);
	//���õ��ĸ񸲸�ת���ɲ�������
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
	//���������ɺ���
	if (mode == "sync")
	{
		//�ٽ��㷨���¾ɣ�����
		extractFalseType(expression, testset);
		return 0.1;
	}
	else if(mode == "batch"){
		//ȫ���㷨���ɲ���ģʽ
		double hit_rate = TestNewAlgo(expression, testset);
		return hit_rate;
	}
	else if (mode == "ordinary_single"){
		//�����㷨����
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
	//cout <<"��������" <<size << endl;
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
	oFile << "���" << "," << "ԭ���ʽ" << "," << "������" << "," << "ԭ������������С" << "," << "���ӵĲ�����������С" << "," << "���ռ��ϴ�С" ;
	//���ݽṹ���е�˳�������������
	exp_mutants_result = Exp_Mutants_Result();
	auto iter = exp_mutants_result.max_add_testcase_size.begin();
	for (iter; iter != exp_mutants_result.max_add_testcase_size.end(); iter++)
	{
		oFile << "," << iter->first << "," << " " << "," << " "<< "," << " ";
	}
	oFile << "," << "�ϲ�" << "," << " " << "," << " " << "," << " " << "," << "ƽ��" << "," << "�㷨����" << "," << "��ƽ���������С" << "," << "�㷨��ʱ" << endl;
	oFile << "�����ٽ�������������" << "," << " " << "," << " " << "," << " " << "," << " " << "," << " ";
	iter = exp_mutants_result.max_add_testcase_size.begin();
	for (iter; iter != exp_mutants_result.max_add_testcase_size.end(); iter++)
	{
		oFile << "," << "max" << "," << "min" << "," << "max_result" << "," << "min_result";
	}
	oFile << "," << "max" << "," << "min" << "," << "max_result" << "," << "min_result" << endl;
	string expression;
	double old_avg_hit_rate = 0, avg_hit_rate = 0, ordinary_avg_hit_rate = 0;
	double num=0;
	if (in) // �и��ļ�
	{
		//// cout << "���һ�к�" << endl;
		while (getline(in, expression)) // line�в�����ÿ�еĻ��з�
		{
			//���ʽȥ�ո�
			ti += 1;
			if (ti == 12 && filename == "test20")ti += 1;
			expression = deleteTab(expression);
			string algo_tag = "new";
			filename = filename_new;
			if (algo_mode){
				algo_tag = "old";
				filename = filename_old;
			}
			//MutantsandValidInvalidTestCase("single", expression); //���ɶ�Ӧ���ʽ�ı��
			//// cout << expression << endl;
			exp_mutants_result = Exp_Mutants_Result();
			exp_mutants_result.exp = expression;
			double hit_rate=UseNearTestSingleFault("single", expression,"batch");
			oFile << to_string(ti) << "," << expression << "," << hit_rate << "," << exp_mutants_result.origin_testcase_size << "," << exp_mutants_result.add_testcase_size << "," << exp_mutants_result.all_testcase_size;
			exp_mutants_result.generateResMessage();
			exp_mutants_result.generateAvgTimeInterval();
			//д���Ʊ��ļ�
			write_for_draw.input_testcase_size(exp_mutants_result.origin_testcase_size, exp_mutants_result.add_testcase_size, exp_mutants_result.all_testcase_size);
			auto iter1 = exp_mutants_result.max_add_testcase_size.begin();
			auto iter2 = exp_mutants_result.min_add_testcase_size.begin();
			auto iter3 = exp_mutants_result.max_res_size.begin();
			auto iter4 = exp_mutants_result.min_res_size.begin();
			//// cout << "map��С"<<exp_mutants_result.max_add_testcase_size.size() << endl;
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
			//������㷨���
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
			//д���Ʊ��ļ�
			write_for_draw.input_testcase_size(exp_mutants_result.origin_testcase_size, exp_mutants_result.add_testcase_size, exp_mutants_result.all_testcase_size);
			iter1 = exp_mutants_result.max_add_testcase_size.begin();
			iter2 = exp_mutants_result.min_add_testcase_size.begin();
			iter3 = exp_mutants_result.max_res_size.begin();
			iter4 = exp_mutants_result.min_res_size.begin();
			//// cout << "map��С"<<exp_mutants_result.max_add_testcase_size.size() << endl;
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
			//��������㷨���
			algo_tag = "ordinary";
			filename = filename_ordinary;
			exp_mutants_result = Exp_Mutants_Result();
			exp_mutants_result.exp = expression;
			hit_rate = generateAllTestcase(expression);
			//hit_rate = UseNearTestSingleFault("single", expression, "batch"); //�������㷨
			oFile << to_string(ti) << "," << expression << "," << hit_rate << "," << exp_mutants_result.origin_testcase_size << "," << exp_mutants_result.add_testcase_size << "," << exp_mutants_result.all_testcase_size;
			exp_mutants_result.generateResMessage();
			exp_mutants_result.generateAvgTimeInterval();
			//д���Ʊ��ļ�
			write_for_draw.input_testcase_size(exp_mutants_result.origin_testcase_size, exp_mutants_result.add_testcase_size, exp_mutants_result.all_testcase_size);
			iter1 = exp_mutants_result.max_add_testcase_size.begin();
			iter2 = exp_mutants_result.min_add_testcase_size.begin();
			iter3 = exp_mutants_result.max_res_size.begin();
			iter4 = exp_mutants_result.min_res_size.begin();
			//// cout << "map��С"<<exp_mutants_result.max_add_testcase_size.size() << endl;
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
			//д���Ʊ���
			write_for_draw.write_line();
			write_for_draw.clear_testcase_size();
		}
		avg_hit_rate /= num;
		old_avg_hit_rate /= num;
		oFile << "��ƽ��������" << "," << avg_hit_rate << "," << "new" << endl;
		oFile << "��ƽ��������" << "," << old_avg_hit_rate << "," << "old" << endl;
		oFile << "��ƽ��������" << "," << ordinary_avg_hit_rate << "," << "ordinary" << endl;
	}
	else // û�и��ļ�
	{
		// cout << "no such file" << endl;
	}
	return;
}

void Input_Exp_FromCmd(){
	string expression;
	// cout << "������ԭ���ʽ" << endl;
	cin >> expression;
	algo_mode = true;
	//UseNearTestSingleFault("single", expression,"sync");
	//�����㷨ģʽ
	UseNearTestSingleFault("single", expression, "ordinary_single");
}

int main()
{
	//generateAllTestcase("ab+cd");
	//�Զ�������
	Input_Exp_FromFile();
	//�ܵ�������
	//Input_Exp_FromCmd();
	//���ɱ����
	//MutantsandValidInvalidTestCase("single", expression);
	//string expression;
	// cout << "������ԭ���ʽ" << endl;
	////cin >> expression;
	//MutantsandValidInvalidTestCase("double", expression);
	system("pause");
	return 0;
}
