// BoolCheckFramework.cpp : �������̨Ӧ�ó������ڵ㡣
//


#include "stdafx.h"

#include<iostream>
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
		cout << "type is rong!" << endl;
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
	//cout << "value====" << root.getValue() << endl;
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
				//cout <<"tempstr: "<< tempStr << endl;
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
			//cout << "dimensionValuesStr: " << dimensionValuesStr << endl;
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
				//cout <<"tempstr: "<< tempStr << endl;
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
			//cout << "dimensionValuesStr: " << dimensionValuesStr << endl;
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
	cout << endl;
	cout << "Test-Case" << endl;
	for (int i = 0; i < testset.size(); ++i){
		cout << testset[i] << endl;
	}
	cout << "Test-Case-End" << endl;







	vector<string>& valid = vector<string>();
	vector<string>& invalid = vector<string>();
	FaultDetection fd = FaultDetection();
	//�����csv�ļ�

	cout <<"------------------------------"<<testset.size();
	vector<string> temp1, temp2;
	for (int i = 0; i < testset.size(); i++){
		if (uti.evaluate(expression, testset[i])){
			temp1.push_back(testset[i]);
		}
		else{
			temp2.push_back(testset[i]);
		}
	}


	cout <<"--------------------------------------" <<temp1.size() << "	" << temp2.size()<<endl;



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




int main()
{
	
	string expression;
	cout << "������ԭ���ʽ" << endl;
	cin >> expression;
	MutantsandValidInvalidTestCase("single", expression);
	//MutantsandValidInvalidTestCase("double", expression);
	system("pause");
	return 0;
}
