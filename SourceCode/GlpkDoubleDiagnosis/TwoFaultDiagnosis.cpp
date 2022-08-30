// TwoFaultDiagnosis.cpp : 定义控制台应用程序的入口点。
//

#include "stdafx.h"
#include <stdio.h>
#include<iostream>
//#include <algorithm>
//#include <map>
//#include <vector>
//#include "Expression.h"
//#include "calExpValue.h"
//#include "expTransform.h"
//#include "Utility.h"
//#include "DnfMutant.h"
//#include "Mutants.h"
//#include <time.h>
#include "TwoFaultDiagnosis.h"

using namespace std;


TwoFaultDiagnosis::TwoFaultDiagnosis(void)
{
}


TwoFaultDiagnosis::~TwoFaultDiagnosis(void)
{
}

#pragma warning(disable:4996)

Utility utility;
bool TRUE = true;
bool TwoFaultDiagnosis::checkTestDiffer(string test, string exprOri/*原表达式*/, string expr/*变体*/){
	Utility uti;
	bool v = (uti.evaluate(exprOri, test)) ^ (uti.evaluateMutant(exprOri, expr, test));//异或：不同为真，相同为假
	return v;
}

//在vector中查找string
int TwoFaultDiagnosis::findFromVector(vector<string> components, string needFound){
	int pos = 0;
	for (string component : components){
		//printf("[%s,%s]\n", component.c_str(), needFound.c_str());
		if (component == needFound)return pos;
		pos++;
	}
	return -1;
}
//诊断——用最优测试用例
bool TwoFaultDiagnosis::diagnosis(
	string oriExp, //原表达式
	string model,
	Mutants mutants,
	vector<string> shirkCaseSet,
	vector<string> expandCaseSet,
	vector<string> utpPoints)
{
	Mutants realMutants(oriExp);
	for (int j = 0; j < mutants.mutants.size(); j++){
		Mutant mutant = mutants.mutants[j];
		bool flagShirk = true;//无效点是否全部通过
		bool flagExpand = true;
		bool flagOnShirk = true;
		//用有效点进行探测
		for (int i = 0; i < shirkCaseSet.size(); i++){
			//printf("point: %s, faultExp: %s, mutant: %s\n", optiUniformitySet[i][0].c_str(), faultExp.c_str(), mutant.c_str());
			if (!checkTestDiffer(shirkCaseSet[i], oriExp, mutant.mutant)){
				flagShirk = false;
				break;
			}
		}

		if (flagShirk == true){//收缩点全部通过
			for (int i = 0; i < expandCaseSet.size(); i++){
				if (!checkTestDiffer(expandCaseSet[i], oriExp, mutant.mutant)){
					flagExpand = false;
					break;
				}
			}
		}

		if (flagShirk == true && flagExpand == true){//收缩点扩张点全部通过
			for (string utpPoint : utpPoints){
				int uu = findFromVector(shirkCaseSet, utpPoint);
				if (uu < 0){//不是收缩点
					if (checkTestDiffer(utpPoint, oriExp, mutant.mutant)){
						flagOnShirk = false;
						break;
					}
				}
			}
			if (flagOnShirk == true){
				realMutants.addMutant(mutant.mutant,mutant.type);
			}
		}
	}
	if (realMutants.mutants.size() == 0){
		return false;
	}
	else{
		/*  gong cui f
		for (Mutant realMutant : realMutants.mutants){
			printf("\n\n\n\n-------------------------\nrealFaultExpress is\n[%s][%s]\n-------------\n", realMutant.mutant.c_str(), realMutant.type.c_str());
		}*/
		for (Mutant realMutant : realMutants.mutants){
			printf("\n\n\n\n-------------------------\nrealFaultExpress is\n[%s][%s]\n-------------\n", realMutant.mutant.c_str(), realMutant.type.c_str());
			string last_mutant = "";
			printf("双项双缺陷诊断成功，变体是%s\n", realMutant.mutant.c_str());
			cout << "Double-Default on two term" << endl;
			cout << "fault_exp:" << realMutant.mutant.c_str() << endl;
			cout << "fault_type:" << realMutant.type.c_str() << endl;
			Utility uti;
			vector<string> tem = uti.extractDNFstyle(oriExp);
			//string t = realMutants.mutants.at(0).oriterms.at(0);
			//string locat = to_string(uti.getTermNum(tem, t));
			string locat = "1,2";
			last_mutant = last_mutant + "ori_exp:" + oriExp + "||" + "fault_exp:" + realMutants.mutants.at(0).mutant + "||" + "locat: "
				+ locat + "||" + "fault_type:" + realMutants.mutants.at(0).type;
			//cout << "RESULT: " << last_mutant << endl;
			break;
		}
		return true;
	}
}

//生成DNF表达式
string TwoFaultDiagnosis::generateDNF(vector<string> terms){
	vector<string>::iterator iter;
	for (int i = 0; i < terms.size(); i++){
		terms[i] = generateIDNForICNF(terms[i], true);
	}
	string oriExp;
	sort(terms.begin(), terms.end());
	iter = unique(terms.begin(), terms.end());
	if (iter != terms.end()){
		terms.erase(iter, terms.end());
	}
	for (int i = 0; i < terms.size(); i++){
		if (i == 0)oriExp = terms[i];
		else oriExp += "+" + terms[i];
	}
	return oriExp;
}
//获取每一项的utp；例如：ab+cd --> ｛{"ab",{"1100","1110","1101"}}, {"cd",{"0011","1011","0111"}} ｝//termUtps
void TwoFaultDiagnosis::getTermUtps(vector<string> sterms, vector<string> upPoints, string model, map<string, vector<string>> &termUtps){
	for (int i = 0; i < sterms.size(); i++) {
		vector<string> termUtpCase;
		for (string upPoint : upPoints){
			int iCase[30] = { 0 };
			int pp = 0;
			for (char c : upPoint){
				iCase[pp] = c - '0';
				pp++;
			}
			bool idt = utility.isDNFtermTrue(sterms[i], iCase, model);
			if (idt) {
				termUtpCase.push_back(upPoint);
			}
		}
		/*cout << "term[i]: " << sterms[i] << endl;
		cout << "termutpcase: " << endl;
		for (auto str : termUtpCase){
			cout << str << endl;
		}*/
		termUtps.insert(pair<string, vector<string>>(sterms[i], termUtpCase));
	}
}

void TwoFaultDiagnosis::getTermOtp2s(vector<string> sterms, vector<string> otp2Points, string model, map<string, vector<string>> &termOtp2s){
	for (int i = 0; i < sterms.size(); i++) {
		vector<string> termOtp2Case;
		for (string otp2Point : otp2Points){
			int iCase[30] = { 0 };
			int pp = 0;
			for (char c : otp2Point){
				iCase[pp] = c - '0';
				pp++;
			}
			bool idt = utility.isDNFtermTrue(sterms[i], iCase, model);
			if (idt) {
				termOtp2Case.push_back(otp2Point);
			}
		}
		termOtp2s.insert(pair<string, vector<string>>(sterms[i], termOtp2Case));
	}
}

void TwoFaultDiagnosis::getTermNfps(string expr, map<string, vector<string>> termUtps, string model, map<string, vector<string>> &termNfps){
	map<string, vector<string>>::iterator iter;
	for (iter = termUtps.begin(); iter != termUtps.end(); iter++){
		string term = iter->first;
		vector<string> termUtpCase = iter->second;
		vector<string> termNfpCase;
		for (string upPoint : termUtpCase){
			int cP = 0;
			for (char c : model){
				int pos = term.find(c);
				if (pos >= 0){//项中有这个词
					string nfpPoint = upPoint;
					if (nfpPoint[cP] == '1')nfpPoint[cP] = '0';//将测试用例进行值翻转得到一个可能nfp
					else nfpPoint[cP] = '1';
					//判断这个测试用例是否是nfp
					int iCase[30] = { 0 };
					int pp = 0;
					for (char c : nfpPoint){
						iCase[pp] = c - '0';
						pp++;
					}
					if (!utility.evaluateDNF(expr, iCase, model)){
						termNfpCase.push_back(nfpPoint);
					}
				}
				cP++;
			}
		}
		termNfps.insert(pair<string, vector<string>>(term, termNfpCase));
	}
}
void TwoFaultDiagnosis::getExpandCases(string expression, string faultExpr, vector<string> fpCases, string model, vector<string> &expandCases){
	//printf("------faultExpr-----------%s\n", faultExpr.c_str());
	for (string fpCase : fpCases){
		bool ret = utility.evaluateMutant(expression, faultExpr, fpCase);
		//printf("-----------------%s:%d\n", fpCase.c_str(),ret);
		if (ret){
			expandCases.push_back(fpCase);
		}
	}
}
//判定扩张节点是否具有包含关系
int TwoFaultDiagnosis::IsContain(vector<string>expandCase1, vector<string>expandCase2){
	if (expandCase1.size() >= expandCase2.size()){
		for (string expendCase20 : expandCase2){
			bool isExist = false;
			for (string expendCase10 : expandCase1){
				if (expendCase20 == expendCase10){
					isExist = true;
					break;
				}
			}
			if (isExist == false){
				//printf("uncontained1 terms:%s\n", expendCase20.c_str());
				//不是包含关系
				return 0;
			}
		}
		return 1;//case1包含case2
	}
	else{
		for (string expendCase10 : expandCase1){
			bool isExist = false;
			for (string expendCase20 : expandCase2){
				if (expendCase10 == expendCase20){
					isExist = true;
					break;
				}
			}
			if (isExist == false){
				//printf("uncontained2 terms:%s\n", expendCase10.c_str());
				//不是包含关系
				return 0;
			}
		}
		return -1;//case2包含case1
	}
}
//过滤出相互不包含的项
void TwoFaultDiagnosis::filterUncontain(map<string, vector<string>>expandNfpTerms, map<string, vector<string>>&realExpands){
	if (expandNfpTerms.size() == 0)return;
	map<string, vector<string>> tmpExpands;
	map<string, vector<string>>::iterator iter, iter1;
	iter1 = expandNfpTerms.begin();
	for (iter = ++(expandNfpTerms.begin()); iter != expandNfpTerms.end(); iter++)
	{
		int ret = IsContain(iter->second, iter1->second);
		//printf("IsContain1:%s:%s[%d]\n", iter->first.c_str(), iter1->first.c_str(), ret);
		if (ret == 0){
			tmpExpands.insert(pair<string, vector<string>>(iter->first, iter->second));
		}
		else{
			if (ret == 1){
				iter1 = iter;
			}
		}
	}
	//printf("filterUncontain:%s[%d]\n", iter1->first.c_str(), tmpExpands.size());
	realExpands.insert(pair<string, vector<string>>(iter1->first, iter1->second));
	//将可能遗漏的成员过滤一下
	map<string, vector<string>> tmpExpands1;
	for (iter = tmpExpands.begin(); iter != tmpExpands.end(); iter++){
		int ret = IsContain(iter1->second, iter->second);
		//printf("IsContain2:%s:%s[%d]\n", iter->first.c_str(), iter1->first.c_str(), ret);
		if (ret == 0){
			tmpExpands1.insert(pair<string, vector<string>>(iter->first, iter->second));
		}
	}
	filterUncontain(tmpExpands1, realExpands);
	return;
}
//根据Term生成变体，用于生成测试数据
Mutants TwoFaultDiagnosis::generateALLmutants(string type, string expression, string model, vector<string>terms1, vector<string>terms2, string term1){
	DnfMutant dnfMutant;
	Mutants mutants = Mutants(expression);
	Mutants uuMutants = Mutants(expression);
	if (type == "ALL"){
		mutants += dnfMutant.generateLIFmutants(expression, terms1, model);
		mutants += dnfMutant.generateLRFmutants(expression, terms1, model);
		mutants += dnfMutant.generateLOFmutants(expression, terms1);
		mutants += dnfMutant.generateTOFmutants(expression, terms1);
		mutants += dnfMutant.generateLNFmutants(expression, terms1);
		mutants += dnfMutant.generateCORFmutants(expression, terms1);
		mutants += dnfMutant.generateTNFmutants(expression, terms1);
		terms1.push_back(term1);
		mutants += dnfMutant.generateDORFmutants(expression, terms1);
		mutants += dnfMutant.generateENFmutants(expression, terms1);
	}
	else if (type == "LIF")mutants += dnfMutant.generateLIFmutants(expression, terms1, model);
	else if (type == "LRF")mutants += dnfMutant.generateLRFmutants(expression, terms1, model);
	else if (type == "LOF")mutants += dnfMutant.generateLOFmutants(expression, terms1);
	else if (type == "TOF")mutants += dnfMutant.generateTOFmutants(expression, terms1);
	else if (type == "DORF"){
		terms1.push_back(term1);
		mutants += dnfMutant.generateDORFmutants(expression, terms1);
	}
	else if (type == "LNF")mutants += dnfMutant.generateLNFmutants(expression, terms1);
	else if (type == "CORF")mutants += dnfMutant.generateCORFmutants(expression, terms1);
	else if (type == "TNF")mutants += dnfMutant.generateTNFmutants(expression, terms1);
	else if (type == "ENF"){
		terms1.push_back(term1);
		mutants += dnfMutant.generateENFmutants(expression, terms1);
	}

	for (Mutant mutant : mutants.mutants){
		uuMutants += dnfMutant.generateLIFmutants(mutant.type, mutant.mutant, terms2, model);
		uuMutants += dnfMutant.generateLRFmutants(mutant.type, mutant.mutant, terms2, model);
		uuMutants += dnfMutant.generateLOFmutants(mutant.type, mutant.mutant, terms2);
		uuMutants += dnfMutant.generateTOFmutants(mutant.type, mutant.mutant, terms2);
		uuMutants += dnfMutant.generateDORFmutants(mutant.type, mutant.mutant, terms2);
		uuMutants += dnfMutant.generateLNFmutants(mutant.type, mutant.mutant, terms2);
		uuMutants += dnfMutant.generateCORFmutants(mutant.type, mutant.mutant, terms2);
		uuMutants += dnfMutant.generateTNFmutants(mutant.type, mutant.mutant, terms2);
		uuMutants += dnfMutant.generateENFmutants(mutant.type, mutant.mutant, terms2);
	}
	return uuMutants;
}



int TwoFaultDiagnosis::dealMain(string expression, string faultExpression, vector<string> terms, string model, vector<string> upPoints, vector<string> otp2Points, vector<string> fpPoints,
	map<string, vector<string>> termUtps, map<string, vector<string>> termOtp2s, map<string, vector<string>> termNfps, map<string, vector<string>> termOfp2s)
{
	vector<string> expandCases;
	getExpandCases(expression, faultExpression, fpPoints, model, expandCases);//获取扩张点
	/*cout << "expandcases: " << endl;
	for (auto str: expandCases){
		cout << str << endl;
	}*/

	map<string, vector<string>>::iterator iter, iter1, iter2;
	//printf("\ntermUtps:\n");
	map<string, vector<string>> shirkUtpTerms, shirkOtp2Terms, expandNfpTerms;
	map<string, vector<string>> UtpTerms, Otp2Terms, NfpTerms;
	for (iter = termUtps.begin(); iter != termUtps.end(); iter++){//确定收缩项
		//printf("term[%s]:\n", iter->first.c_str());
		vector<string>shirkUtps;
		vector<string> termUtpCase = iter->second;
		for (string upPoint : termUtpCase){
			//printf("[%s]\n", upPoint.c_str());
			//判定收缩项
			bool isTrue = utility.evaluateMutant(expression, faultExpression, upPoint);
			if (!isTrue){
				//printf("shirk term:%s[%s]\n", iter->first.c_str(), upPoint.c_str());
				shirkUtps.push_back(upPoint);
			}
		}
		if (shirkUtps.size() > 0){
			UtpTerms.insert(pair<string, vector<string>>(iter->first, termUtpCase));
			shirkUtpTerms.insert(pair<string, vector<string>>(iter->first, shirkUtps));
		}
		printf("\n");
	}
	//获取2重otp
	//printf("\ntermOtp2s:\n");
	for (iter = termOtp2s.begin(); iter != termOtp2s.end(); iter++){//确定二重收缩项
		//if (TRUE)printf("term[%s]:\n", iter->first.c_str());
		vector<string>shirkOtp2s;
		vector<string>termOtp2Case = iter->second;
		for (string opPoint : termOtp2Case){
			//if (TRUE)printf("[%s]\n", opPoint.c_str());
			//判定收缩项
			bool isTrue = utility.evaluateMutant(expression, faultExpression, opPoint);
			if (!isTrue){
				//if (TRUE)printf("shirk 2 term:%s[%s]\n", iter->first.c_str(), opPoint.c_str());
				shirkOtp2s.push_back(opPoint);
			}
		}
		if (shirkOtp2s.size() > 0){
			Otp2Terms.insert(pair<string, vector<string>>(iter->first, termOtp2Case));
			shirkOtp2Terms.insert(pair<string, vector<string>>(iter->first, shirkOtp2s));
		}
		//if (TRUE)printf("\n");
	}

	map<string, vector<string>> shirkTerms, expandTerms;
	for (iter = shirkUtpTerms.begin(); iter != shirkUtpTerms.end(); iter++){
		//if (TRUE)printf("find possible 1 shirk term:%s\n", iter->first.c_str());
		shirkTerms.insert(pair<string, vector<string>>(iter->first, iter->second));
	}
	for (iter = shirkOtp2Terms.begin(); iter != shirkOtp2Terms.end(); iter++){
		//if (TRUE)printf("find possible 2 shirk term:%s\n", iter->first.c_str());
		try{
			vector<string>utps = shirkUtpTerms.at(iter->first);
			if (utps.size() != 0){
				vector<string>tps = iter->second;
				for (string tp : tps){
					utps.push_back(tp);
				}
				shirkTerms.insert(pair<string, vector<string>>(iter->first, utps));
			}
			else{
				shirkTerms.insert(pair<string, vector<string>>(iter->first, iter->second));
			}
		}
		catch (exception e){
			shirkTerms.insert(pair<string, vector<string>>(iter->first, iter->second));
		}
	}

	//printf("\ntermNfps:\n");
	for (iter = termNfps.begin(); iter != termNfps.end(); iter++){
		string termNfp = iter->first;
		//printf("term[%s]:\n", termNfp.c_str());
		vector<string> termNfpCase = iter->second;
		vector<string>expandNfts;
		string termModel = generateModel(termNfp);
		//printf("termModel:%s,%d\n", termModel.c_str(), termModel.size());
		int termModelNum = termModel.size();
		int cPos[30] = { 0 };
		int iii = 0;
		for (char cc : termModel){
			cPos[iii] = cc - 'a';
			iii++;
		}
		vector<string>termCs, termACs;
		for (string nfpPoint : termNfpCase){
			//if (TRUE)printf("[%s]\n", nfpPoint.c_str());
			string termCsCase = "";
			for (int jjj = 0; jjj < termModelNum; jjj++){
				termCsCase[jjj] = nfpPoint[cPos[jjj]];
			}
			termCsCase[termModelNum] = 0;
			//if (TRUE)printf("[%s]\n", termCsCase.c_str());
			int pos = findFromVector(termACs, termCsCase.c_str());
			//if (TRUE)printf("termACs termCsCase:[%d][%s]\n", pos, termCsCase.c_str());
			if (pos < 0){
				//if (TRUE)printf("termACs termCsCase:[%s]\n", termCsCase.c_str());
				termACs.push_back(termCsCase.c_str());
			}
			//判定扩张项
			bool isTrue = utility.evaluateMutant(expression, faultExpression, nfpPoint);
			if (isTrue){
				//if (TRUE)printf("expand term:%s[%s]\n", termNfp.c_str(), nfpPoint.c_str());
				expandNfts.push_back(nfpPoint);
			}
			else{//这是假点，假的扩张项具有这些特点，它的假点包含了项中的全部邻近假点取值
				//printf("not expand termC:%s[%s]\n", termNfp.c_str(), termCsCase.c_str());
				int pos = findFromVector(termCs, termCsCase.c_str());
				//if (TRUE)printf("termCs pos:[%d]\n", pos);
				if (pos < 0){
					//if (TRUE)printf("termCs termCsCase:[%s]\n", termCsCase.c_str());
					termCs.push_back(termCsCase.c_str());
				}
				//if (TRUE)printf("termCs size:[%d]\n", termCs.size());
			}
		}
		if (termACs.size() == termCs.size()){
			//if (TRUE)printf("this is a false expend term:%s:%d:%d\n", termNfp.c_str(), termACs.size(), termCs.size());
		}
		else{
			NfpTerms.insert(pair<string, vector<string>>(termNfp, termNfpCase));
			expandNfpTerms.insert(pair<string, vector<string>>(termNfp, expandNfts));
			//if (TRUE)printf("\n");
		}
	}
	//printf("\n\n\n");
	//根据包含关系剔除假的扩张项，方法如下，如果一个项的扩张节点包含了另一项的扩张节点，则包含节点可能是发生扩张的项，如果超过3个项，说明有ENF
	filterUncontain(expandNfpTerms, expandTerms);
	//判断是否是TNF和ENF
	printf("%d:%d\n", fpPoints.size(), expandCases.size());
	Mutants fMutants(expression);
	if (fpPoints.size() == expandCases.size()){
	if (TRUE)printf("发生了TNF或ENF的变化[%d][%d]\n", shirkTerms.size(),expandTerms.size());//无法通过扩张项判定发生缺陷的项，只能通过收缩项确定
		if (shirkTerms.size() == 0){//如果没有收缩，直接失败
			printf("没有收缩，可能没有发生缺陷，无法诊断\n");
			return 0;
		}
		else if (shirkTerms.size() == 1){//如果只有一项，则就是TNF和一个可扩张项
			if (TRUE)printf("如果只有一项，则就是TNF和一个可扩张项\n");
			DnfMutant dnfMutant;
			Mutants mutants(expression);
			iter = shirkTerms.begin();
			vector<string>firstTerms;
			firstTerms.push_back(iter->first);
			vector<string>secondTerms;
			for (iter = expandTerms.begin(); iter != expandTerms.end(); iter++){
				if (iter->first != firstTerms.at(0)){
					secondTerms.push_back(iter->first);
				}
			}
			mutants = dnfMutant.generateTNFmutants(expression, firstTerms);//首先看发生TNF
			for (Mutant mutant : mutants.mutants){//LOF，CORF
				//printf("mutants:[%s]\n", mutant.mutant.c_str());
				fMutants += dnfMutant.generateLOFmutants("TNF", mutant.mutant.c_str(), secondTerms);
				fMutants += dnfMutant.generateCORFmutants("TNF", mutant.mutant.c_str(), secondTerms);
			}
			//for (Mutant mutant : fMutants.mutants){
				//printf("fMutants:[%s]\n", mutant.mutant.c_str());
			//}
		}
		else if (shirkTerms.size() == 2){//如果有两项，则要么是ENF和一个可扩张项，要么是TNF和一个可收缩项
			if (TRUE)printf("如果有两项，则要么是ENF和一个可扩张项，要么是TNF和一个可收缩项\n");
			DnfMutant dnfMutant;
			iter = shirkTerms.begin();
			vector<string>firstTerms;
			firstTerms.push_back(iter->first);
			vector<string>secondTerms;
			iter = ++shirkTerms.begin();
			secondTerms.push_back(iter->first);
			//首先看发生TNF
			Mutants mutants(expression);
			mutants = dnfMutant.generateTNFmutants(expression, firstTerms);
			for (Mutant mutant : mutants.mutants){//LIF,LRF,LNF,TNF
				//printf("mutants:[%s]\n", mutant.mutant.c_str());
				fMutants += dnfMutant.generateLIFmutants("TNF", mutant.mutant.c_str(), secondTerms, model);
				fMutants += dnfMutant.generateLRFmutants("TNF", mutant.mutant.c_str(), secondTerms, model);
				fMutants += dnfMutant.generateLNFmutants("TNF", mutant.mutant.c_str(), secondTerms);
				fMutants += dnfMutant.generateTNFmutants("TNF", mutant.mutant.c_str(), secondTerms);
			}
			mutants = dnfMutant.generateTNFmutants(expression, secondTerms);//再看第二项发生TNF
			for (Mutant mutant : mutants.mutants){//LIF,LRF,LNF,TNF（上一次已经生成过）
				//printf("mutants:[%s]\n", mutant.mutant.c_str());
				fMutants += dnfMutant.generateLIFmutants("TNF", mutant.mutant.c_str(), firstTerms, model);
				fMutants += dnfMutant.generateLRFmutants("TNF", mutant.mutant.c_str(), firstTerms, model);
				fMutants += dnfMutant.generateLNFmutants("TNF", mutant.mutant.c_str(), firstTerms);
			}
			//再看ENF
			iter = ++shirkTerms.begin();
			firstTerms.push_back(iter->first);
			secondTerms.clear();
			for (iter = expandTerms.begin(); iter != expandTerms.end(); iter++){
				if (iter->first != firstTerms.at(0)){
					secondTerms.push_back(iter->first);
				}
			}
			Mutants mutants1(expression);
			mutants1 = dnfMutant.generateENFmutants(expression, firstTerms);
			for (Mutant mutant : mutants1.mutants){//LOF，CORF
				//printf("mutants:[%s]\n", mutant.mutant.c_str());
				fMutants += dnfMutant.generateLOFmutants("ENF", mutant.mutant.c_str(), secondTerms);
				fMutants += dnfMutant.generateCORFmutants("ENF", mutant.mutant.c_str(), secondTerms);
			}

			//for (Mutant mutant : fMutants.mutants){
				//printf("fMutants:[%s]\n", mutant.mutant.c_str());
			//}
		}
		else if (shirkTerms.size() == 3){//如果有三项，要么是TNF和DORF，要么是ENF和一个可收缩项，要么是ENF和一个可扩张项
			if (TRUE)printf("如果有三项，要么是TNF和DORF，要么是ENF和一个可收缩项，要么是ENF和一个可扩张项\n");
			DnfMutant dnfMutant;
			vector<string>firstTerms;
			vector<string>secondTerms;
			//首先看发生TNF和DORF
			for (iter = shirkTerms.begin(); iter != shirkTerms.end(); iter++){
				firstTerms.clear();
				secondTerms.clear();
				firstTerms.push_back(iter->first);
				for (iter1 = shirkTerms.begin(); iter1 != shirkTerms.end(); iter1++){
					if (iter1->first != iter->first){
						secondTerms.push_back(iter1->first);
					}
				}
				Mutants mutants(expression);
				mutants = dnfMutant.generateTNFmutants(expression, firstTerms);
				for (Mutant mutant : mutants.mutants){//DORF
					//printf("mutants:[%s]\n", mutant.mutant.c_str());
					fMutants += dnfMutant.generateDORFmutants("TNF",mutant.mutant.c_str(), secondTerms);
				}
			}
			//再看ENF对单项收缩
			for (iter = shirkTerms.begin(); iter != shirkTerms.end(); iter++){
				firstTerms.clear();
				secondTerms.clear();
				firstTerms.push_back(iter->first);
				for (iter1 = shirkTerms.begin(); iter1 != shirkTerms.end(); iter1++){
					if (iter1->first != iter->first){
						secondTerms.push_back(iter1->first);
					}
				}
				Mutants mutants(expression);
				mutants = dnfMutant.generateLIFmutants(expression, firstTerms, model);
				for (Mutant mutant : mutants.mutants){//LIF
					//printf("mutants:[%s]\n", mutant.mutant.c_str());
					fMutants += dnfMutant.generateENFmutants("LIF",mutant.mutant.c_str(), secondTerms);
				}
				mutants = dnfMutant.generateLRFmutants(expression, firstTerms, model);
				for (Mutant mutant : mutants.mutants){//LRF
					//printf("mutants:[%s]\n", mutant.mutant.c_str());
					fMutants += dnfMutant.generateENFmutants("LRF", mutant.mutant.c_str(), secondTerms);
				}
				mutants = dnfMutant.generateLNFmutants(expression, firstTerms);
				for (Mutant mutant : mutants.mutants){//LNF
					//printf("mutants:[%s]\n", mutant.mutant.c_str());
					fMutants += dnfMutant.generateENFmutants("LNF", mutant.mutant.c_str(), secondTerms);
				}
				mutants = dnfMutant.generateTNFmutants(expression, firstTerms);
				for (Mutant mutant : mutants.mutants){//TNF
					//printf("mutants:[%s]\n", mutant.mutant.c_str());
					fMutants += dnfMutant.generateENFmutants("TNF", mutant.mutant.c_str(), secondTerms);
				}
			}
			//再看ENF对扩张
			secondTerms.clear();
			for (iter1 = shirkTerms.begin(); iter1 != shirkTerms.end(); iter1++){
				secondTerms.push_back(iter1->first);
			}
			for (iter = expandTerms.begin(); iter != expandTerms.end(); iter++){
				firstTerms.clear();
				firstTerms.push_back(iter->first);
				Mutants mutants(expression);
				mutants = dnfMutant.generateLOFmutants(expression, firstTerms);
				for (Mutant mutant : mutants.mutants){//LOF
					//printf("mutants:[%s]\n", mutant.mutant.c_str());
					fMutants += dnfMutant.generateENFmutants("LOF",mutant.mutant.c_str(), secondTerms);
				}
				mutants = dnfMutant.generateCORFmutants(expression, firstTerms);
				for (Mutant mutant : mutants.mutants){//LOF
					//printf("mutants:[%s]\n", mutant.mutant.c_str());
					fMutants += dnfMutant.generateENFmutants("CORF",mutant.mutant.c_str(), secondTerms);
				}
			}
			//for (Mutant mutant : fMutants.mutants){
				//printf("fMutants:[%s]\n", mutant.mutant.c_str());
			//}
		}
		else{
			//如果大于三项，要么是ENF和DORF，要么是ENF和一个可收缩项，要么是ENF和一个可扩张项,要么是两个ENF（将TNF看成一项ENF）
			//如果都不是，那就是ENF和TNF的组合，直接将其分成两组即可（暂时不实现）
			if (TRUE)printf("如果大于三项，要么是ENF和DORF，要么是ENF和一个可收缩项，要么是ENF和一个可扩张项,要么是两个ENF（将TNF看成一项ENF）\n如果都不是，那就是ENF和TNF的组合，直接将其分成两组即可（暂时不实现）\n");
			DnfMutant dnfMutant;
			vector<string>firstTerms;
			vector<string>secondTerms;
			//首先看发生ENF和DORF
			for (iter = shirkTerms.begin(); iter != --shirkTerms.end(); iter++){
				for (iter1 = iter, iter1++; iter1 != shirkTerms.end(); iter1++){
					firstTerms.clear();
					firstTerms.push_back(iter->first);
					firstTerms.push_back(iter1->first);
					Mutants mutants(expression);
					mutants = dnfMutant.generateDORFmutants(expression, firstTerms);
					secondTerms.clear();
					for (iter2 = shirkTerms.begin(); iter2 != shirkTerms.end(); iter2++){
						if (iter2->first != iter->first && iter2->first != iter1->first){
							secondTerms.push_back(iter2->first);
						}
					}
					for (Mutant mutant : mutants.mutants){//DORF
						//printf("mutants:[%s]\n", mutant.mutant.c_str());
						fMutants += dnfMutant.generateENFmutants("DORF",mutant.mutant.c_str(), secondTerms);
					}
				}
			}
			//再看ENF对单项收缩
			for (iter = shirkTerms.begin(); iter != shirkTerms.end(); iter++){
				firstTerms.clear();
				secondTerms.clear();
				firstTerms.push_back(iter->first);
				for (iter1 = shirkTerms.begin(); iter1 != shirkTerms.end(); iter1++){
					if (iter1->first != iter->first){
						secondTerms.push_back(iter1->first);
					}
				}
				Mutants mutants(expression);
				mutants = dnfMutant.generateLIFmutants(expression, firstTerms, model);
				for (Mutant mutant : mutants.mutants){//LIF
					//printf("mutants:[%s]\n", mutant.mutant.c_str());
					fMutants += dnfMutant.generateENFmutants("LIF", mutant.mutant.c_str(), secondTerms);
				}
				mutants = dnfMutant.generateLRFmutants(expression, firstTerms, model);
				for (Mutant mutant : mutants.mutants){//LRF
					//printf("mutants:[%s]\n", mutant.mutant.c_str());
					fMutants += dnfMutant.generateENFmutants("LRF", mutant.mutant.c_str(), secondTerms);
				}
				mutants = dnfMutant.generateLNFmutants(expression, firstTerms);
				//mutants += dnfMutant.generateTNFmutants(expression, firstTerms);//不考虑TNF，因为在ENFxENF中会统一处理
				for (Mutant mutant : mutants.mutants){//LNF
					//printf("mutants:[%s]\n", mutant.mutant.c_str());
					fMutants += dnfMutant.generateENFmutants("LNF", mutant.mutant.c_str(), secondTerms);
				}
			}
			//再看ENF对扩张
			secondTerms.clear();
			for (iter1 = shirkTerms.begin(); iter1 != shirkTerms.end(); iter1++){
				secondTerms.push_back(iter1->first);
			}
			for (iter = expandTerms.begin(); iter != expandTerms.end(); iter++){
				firstTerms.clear();
				firstTerms.push_back(iter->first);
				Mutants mutants(expression);
				mutants = dnfMutant.generateLOFmutants(expression, firstTerms);
				for (Mutant mutant : mutants.mutants){//LOF
					//printf("mutants:[%s]\n", mutant.mutant.c_str());
					fMutants += dnfMutant.generateENFmutants("LOF",mutant.mutant.c_str(), secondTerms);
				}
				mutants = dnfMutant.generateCORFmutants(expression, firstTerms);
				for (Mutant mutant : mutants.mutants){//CORF
					//printf("mutants:[%s]\n", mutant.mutant.c_str());
					fMutants += dnfMutant.generateENFmutants("CORF",mutant.mutant.c_str(), secondTerms);
				}
			}
			//最后看ENF和ENF，直接按照顺序进行排列，如果只有一组连续项，则拆分成两组ENF，有两组连续项，则是两个ENF，如果超过两组，说明不是双ENF
			//首先判定是否是连续
			firstTerms.clear();
			secondTerms.clear();
			bool isFirstFound = false;
			bool isSecondFound = false;
			for (string oriTerm : terms){
				iter = shirkTerms.find(oriTerm);
				if (isFirstFound){
					if (iter != shirkTerms.end()){//找到了连续节点节点
						firstTerms.push_back(oriTerm);
						continue;
					}
					else{
						isFirstFound = false;
						continue;
					}
				}
				else if (isSecondFound){
					if (iter != shirkTerms.end()){//找到了连续节点节点
						secondTerms.push_back(oriTerm);
						continue;
					}
					else{
						isSecondFound = false;
						continue;
					}
				}
				else{
					if (iter != shirkTerms.end()){
						if (firstTerms.size() == 0){
							isFirstFound = true;
							firstTerms.push_back(oriTerm);
							continue;
						}
						else if (secondTerms.size() == 0){
							isSecondFound = true;
							secondTerms.push_back(oriTerm);
							continue;
						}
						else{
							printf("发生超过二重的ENF，暂时不支持该类型诊断");
							break;
						}
					}
				}
			}
			if (isFirstFound && !isSecondFound){//生成两个ENF
				secondTerms.clear();
				for (string tmpFirstTerm : firstTerms){
					//printf("tmpFirstTerm:[%s]\n", tmpFirstTerm.c_str());
				}
				for (int zz = firstTerms.size() - 1; zz >= 1; zz--){
					//printf("firstTerms:[%d]\n", firstTerms.size());
					secondTerms.push_back(firstTerms.at(zz));//给第二组加一项
					firstTerms.erase(firstTerms.end() - (firstTerms.size()-zz), firstTerms.end());//給第一组减一项
					//printf("firstTerms:[%d]\n", firstTerms.size());
					Mutants mutants(expression);
					mutants = dnfMutant.generateENFmutants(expression, firstTerms);
					for (Mutant mutant : mutants.mutants){//ENF
						//printf("mutants:[%s]\n", mutant.mutant.c_str());
						fMutants += dnfMutant.generateENFmutants("ENF",mutant.mutant.c_str(), secondTerms);
					}
					//printf("secondTerms:[%d]\n", secondTerms.size());
				}
			}
			else if (isFirstFound && isSecondFound){//生成两个ENF
				for (string tmpFirstTerm : firstTerms){
					//printf("tmpFirstTerm:[%s]\n", tmpFirstTerm.c_str());
				}
				for (string tmpSecondTerm : secondTerms){
					//printf("tmpSecondTerm:[%s]\n", tmpSecondTerm.c_str());
				}
				Mutants mutants(expression);
				mutants = dnfMutant.generateENFmutants(expression, firstTerms);
				for (Mutant mutant : mutants.mutants){//ENF
					//printf("mutants:[%s]\n", mutant.mutant.c_str());
					fMutants += dnfMutant.generateENFmutants("ENF",mutant.mutant.c_str(), secondTerms);
				}
			}

			//for (Mutant mutant : fMutants.mutants){
				//printf("fMutants:[%s]\n", mutant.mutant.c_str());
			//}
		}
	}
	else{
		if (TRUE)printf("没有发生TNF或ENF变化\n");
		//结合收缩项判定扩张项，凡是有收缩的项，就是发生了缺陷的项
		vector<string> faultTerms;
		for (iter = shirkTerms.begin(); iter != shirkTerms.end(); iter++){
			//printf("find possible shirk term:%s\n", iter->first.c_str());
			int pos = findFromVector(faultTerms, iter->first.c_str());
			if (pos < 0)faultTerms.push_back(iter->first.c_str());
		}
		for (iter = expandTerms.begin(); iter != expandTerms.end(); iter++){
			//printf("find possible expand term:%s\n", iter->first.c_str());
			int pos = findFromVector(faultTerms, iter->first.c_str());
			if (pos < 0)faultTerms.push_back(iter->first.c_str());
		}
		//不考虑等价条件下有如下结论
		if (expandTerms.size() == 0){
			if (shirkTerms.size() == 0){
				//printf("没有缺陷或无法诊断\n");
				return 0;
			}
			else if (shirkTerms.size() == 1){
				//printf("这是单项收缩缺陷\n");
				return 0;
			}
			//根据缺陷发生的状况确定是发生了什么缺陷
			else if (shirkTerms.size() == 2){
				if (TRUE)printf("这是DORF或LIF、TOF双收缩缺陷，考虑LRF与LIF、TOF\n");
				//如果该项utp全部收缩，但otp2没有变化，说明发生了DORF，不考虑
				bool isDorf = true;
				for (iter2 = UtpTerms.begin(); iter2 != UtpTerms.end(); iter2++){
					if (shirkUtpTerms.find(iter2->first)->second.size() != iter2->second.size()){
						isDorf = false;
						break;
					}
				}
				if (isDorf){
					for (iter2 = Otp2Terms.begin(); iter2 != Otp2Terms.end(); iter2++){
						if (Otp2Terms.find(iter2->first)->second.size() == iter2->second.size()){
							isDorf = false;
							break;
						}
					}
				}
				if (isDorf){
					printf("这是单缺陷DORF，不考虑！");
					return 0;
				}
				else{
					//如果该项utp没有全部收缩，说明发生了LIF或LRF，另一项如果也没有全部收缩，则发生LIF或LRF，否则发生TOF
					//如果该项utp和otp2全部收缩，说明发生了两次TOF
					string term1 = "", term2 = "";
					DnfMutant dnfMutant;
					Mutants mutants(expression);
					//printf("%d:%d\n", shirkUtpTerms.find(UtpTerms.begin()->first)->second.size(), UtpTerms.begin()->second.size());
					if (shirkUtpTerms.find(UtpTerms.begin()->first)->second.size() != UtpTerms.begin()->second.size()){
						term1 = "LIF";
						//printf("第一项为LIF或LRF,或特殊情况TOF与LRF\n");
						vector<string>mutantTerms = { UtpTerms.begin()->first };
						mutants += dnfMutant.generateLIFmutants(expression, mutantTerms, model);
						mutants += dnfMutant.generateLRFmutants(expression, mutantTerms, model);
						mutants += dnfMutant.generateTOFmutants(expression, mutantTerms);
					}
					else{
						term1 = "TOF";
						//printf("第一项为TOF\n");
						vector<string>mutantTerms = { UtpTerms.begin()->first };
						mutants = dnfMutant.generateTOFmutants(expression, mutantTerms);
					}
					//printf("%d:%d\n", shirkUtpTerms.find((++UtpTerms.begin())->first)->second.size(), (++UtpTerms.begin())->second.size());
					if (shirkUtpTerms.find((++UtpTerms.begin())->first)->second.size() != (++UtpTerms.begin())->second.size()){
						term2 = "LIF";
						//printf("第二项为LIF或LRF,或特殊情况TOF与LRF[%s]\n", (++UtpTerms.begin())->first.c_str());
						vector<string>mutantTerms = { (++UtpTerms.begin())->first };
						for (Mutant mutant : mutants.mutants){
							//printf("mutants:[%s]\n", mutant.type.c_str());
							if (mutant.type == "TOF"){
								fMutants += dnfMutant.generateLRFmutants(mutant.type, mutant.mutant.c_str(), mutantTerms, model);
							}
							else if (mutant.type == "LRF"){
								fMutants += dnfMutant.generateLIFmutants(mutant.type, mutant.mutant.c_str(), mutantTerms, model);
								fMutants += dnfMutant.generateLRFmutants(mutant.type, mutant.mutant.c_str(), mutantTerms, model);
								fMutants += dnfMutant.generateTOFmutants(mutant.type, mutant.mutant.c_str(), mutantTerms);
							}
							else{
								fMutants += dnfMutant.generateLIFmutants(mutant.type, mutant.mutant.c_str(), mutantTerms, model);
								fMutants += dnfMutant.generateLRFmutants(mutant.type, mutant.mutant.c_str(), mutantTerms, model);
							}
						}
						//for (Mutant mutant : fMutants.mutants){
							//printf("fMutants:[%s]\n", mutant.mutant.c_str());
						//}
					}
					else{
						term2 = "TOF";
						//printf("第二项为TOF\n");
						vector<string>mutantTerms = { (++UtpTerms.begin())->first };
						for (Mutant mutant : mutants.mutants){
							//printf("mutants:[%s]\n", mutant.mutant.c_str());
							fMutants += dnfMutant.generateTOFmutants(mutant.type, mutant.mutant.c_str(), mutantTerms);
						}
						//for (Mutant mutant : fMutants.mutants){
							//printf("fMutants:[%s]\n", mutant.mutant.c_str());
						//}
					}

				}
			}
			else if (shirkTerms.size() == 3){
				if (TRUE)printf("发生LRF（扩张点被收缩）、LIF、TOF与DORF[%d]\n", shirkTerms.size());
				DnfMutant dnfMutant;
				Mutants mutants(expression);
				for (iter = shirkTerms.begin(); iter != shirkTerms.end(); iter++){
					vector<string>lifTerms;
					lifTerms.push_back(iter->first);
					mutants = dnfMutant.generateLRFmutants(expression, lifTerms, model);//首先看发生LIF
					for (Mutant mutant : mutants.mutants){
						//printf("mutants:[%s]\n", mutant.mutant.c_str());
						vector<string>leftTerms;
						for (iter1 = shirkTerms.begin(); iter1 != shirkTerms.end(); iter1++){
							if (iter->first != iter1->first){
								//printf("term:[%s]\n", iter1->first.c_str());
								leftTerms.push_back(iter1->first);
							}
						}
						fMutants += dnfMutant.generateDORFmutants("LRF", mutant.mutant.c_str(), leftTerms);
					}
					mutants = dnfMutant.generateLIFmutants(expression, lifTerms, model);//首先看发生LIF
					for (Mutant mutant : mutants.mutants){
						//printf("mutants:[%s]\n", mutant.mutant.c_str());
						vector<string>leftTerms;
						for (iter1 = shirkTerms.begin(); iter1 != shirkTerms.end(); iter1++){
							if (iter->first != iter1->first){
								//printf("term:[%s]\n", iter1->first.c_str());
								leftTerms.push_back(iter1->first);
							}
						}
						fMutants += dnfMutant.generateDORFmutants("LIF",mutant.mutant.c_str(), leftTerms);
					}
					mutants = dnfMutant.generateTOFmutants(expression, lifTerms);//再看发生TOF
					for (Mutant mutant : mutants.mutants){
						//printf("mutants:[%s]\n", mutant.mutant.c_str());
						vector<string>leftTerms;
						for (iter1 = shirkTerms.begin(); iter1 != shirkTerms.end(); iter1++){
							if (iter->first != iter1->first){
								//printf("term:[%s]\n", iter1->first.c_str());
								leftTerms.push_back(iter1->first);
							}
						}
						fMutants += dnfMutant.generateDORFmutants("TOF",mutant.mutant.c_str(), leftTerms);
					}
				}
			}
			else if (shirkTerms.size() == 4){
				if (TRUE)printf("发生DORF与DORF[%d]\n", shirkTerms.size());
				DnfMutant dnfMutant;
				Mutants mutants(expression);
				iter = shirkTerms.begin();
				for (iter1 = iter, iter1++; iter1 != shirkTerms.end(); iter1++){
					vector<string> firstTerms;
					firstTerms.push_back(iter->first);
					firstTerms.push_back(iter1->first);
					mutants = dnfMutant.generateDORFmutants(expression, firstTerms);
					for (Mutant mutant : mutants.mutants){
						//printf("mutants:[%s]\n", mutant.mutant.c_str());
						vector<string>secondTerms;
						for (iter2 = shirkTerms.begin(); iter2 != shirkTerms.end(); iter2++){
							if (iter2->first != iter->first && iter2->first != iter1->first){
								//printf("secondTerms:[%s]\n", iter2->first.c_str());
								secondTerms.push_back(iter2->first);
							}
						}
						fMutants += dnfMutant.generateDORFmutants("DORF",mutant.mutant.c_str(), secondTerms);
					}
				}
				//for (Mutant mutant : fMutants.mutants){
					//printf("fMutants:[%s]\n", mutant.mutant.c_str());
				//}
			}

		}
		else if (expandTerms.size() <= 2){//此时扩张项不能大于2，否则说明不是双缺陷
			if (shirkTerms.size() != 0){
				if (shirkTerms.size() == 1 && expandTerms.size() == 1){
					if (shirkTerms.begin()->first == expandTerms.begin()->first){
						printf("这是单项收缩且扩张缺陷\n");
						return 0;
					}
					if (TRUE)printf("只能发生LIF、TOF与LOF、CORF,考虑LRF与LOF这个特例\n");
					string termSign = "";
					if (shirkUtpTerms.find(UtpTerms.begin()->first)->second.size() != UtpTerms.begin()->second.size()){
						termSign = "LIF";
						//printf("收缩项为LIF或LRF\n");
					}
					else{
						termSign = "TOF";
						//printf("收缩项为TOF\n");
					}
					DnfMutant dnfMutant;
					Mutants mutants(expression);
					iter = shirkTerms.begin();
					vector<string>lifTerms;
					lifTerms.push_back(iter->first);
					iter = expandTerms.begin();
					vector<string>lofTerms;
					lofTerms.push_back(iter->first);
					if (termSign == "LIF"){
						mutants += dnfMutant.generateLIFmutants(expression, lifTerms, model);//首先看发生LIF
						mutants += dnfMutant.generateLRFmutants(expression, lifTerms, model);//或LRF
					}
					else if (termSign == "TOF")mutants = dnfMutant.generateTOFmutants(expression, lifTerms);//首先看发生TOF
					for (Mutant mutant : mutants.mutants){//LOF
						//printf("mutants:[%s]\n", mutant.mutant.c_str());
						fMutants += dnfMutant.generateLOFmutants(mutant.type, mutant.mutant.c_str(), lofTerms);
						fMutants += dnfMutant.generateCORFmutants(mutant.type, mutant.mutant.c_str(), lofTerms);
					}
					//for (Mutant mutant : fMutants.mutants){
						//printf("fMutants:[%s]\n", mutant.mutant.c_str());
					//}

				}
				else if (shirkTerms.size() == 1 && expandTerms.size() == 2){
					if (TRUE)printf("只能发生LRF、LNF与LOF、CORF\n");
					DnfMutant dnfMutant;
					Mutants mutants(expression);
					iter = shirkTerms.begin();
					vector<string>lrfTerms;
					lrfTerms.push_back(iter->first);
					iter = expandTerms.begin();
					vector<string>lofTerms;
					lofTerms.push_back(iter->first);
					iter++;
					lofTerms.push_back(iter->first);
					mutants = dnfMutant.generateLRFmutants(expression, lrfTerms, model);//首先看发生LRF
					for (Mutant mutant : mutants.mutants){//LRF
						//printf("mutants:[%s]\n", mutant.mutant.c_str());
						fMutants += dnfMutant.generateLOFmutants("LRF", mutant.mutant.c_str(), lofTerms);
						fMutants += dnfMutant.generateCORFmutants("LRF", mutant.mutant.c_str(), lofTerms);
					}
					mutants = dnfMutant.generateLNFmutants(expression, lrfTerms);//首先看发生LNF
					for (Mutant mutant : mutants.mutants){//LNF
						//printf("mutants:[%s]\n", mutant.mutant.c_str());
						fMutants += dnfMutant.generateLOFmutants("LNF", mutant.mutant.c_str(), lofTerms);
						fMutants += dnfMutant.generateCORFmutants("LNF", mutant.mutant.c_str(), lofTerms);
					}
					//for (Mutant mutant : fMutants.mutants){
						//printf("fMutants:[%s]\n", mutant.mutant.c_str());
					//}
				}
				else if (shirkTerms.size() == 2 && expandTerms.size() == 1){
					if (shirkTerms.begin()->first != expandTerms.begin()->first && (++shirkTerms.begin())->first != expandTerms.begin()->first){
						if (TRUE)printf("只能发生DORF与LOF、CORF\n");
						DnfMutant dnfMutant;
						Mutants mutants(expression);
						//首先看发生LIF
						iter = shirkTerms.begin();
						iter1 = iter, iter1++;
						vector<string> firstTerms;
						firstTerms.push_back(iter->first);
						firstTerms.push_back(iter1->first);
						mutants = dnfMutant.generateDORFmutants(expression, firstTerms);
						for (Mutant mutant : mutants.mutants){
							//printf("mutants:[%s]\n", mutant.mutant.c_str());
							vector<string>secondTerms = { expandTerms.begin()->first };
							fMutants += dnfMutant.generateLOFmutants("DORF",mutant.mutant.c_str(), secondTerms);
							fMutants += dnfMutant.generateCORFmutants("DORF",mutant.mutant.c_str(), secondTerms);
						}
						//for (Mutant mutant : fMutants.mutants){
							//printf("fMutants:[%s]\n", mutant.mutant.c_str());
						//}
					}
					else{
						if (TRUE)printf("只能发生LIF、TOF与LRF、LNF 或 LRF与LNF（此时LRF和LNF扩张域覆盖）\n");
						DnfMutant dnfMutant;
						Mutants mutants(expression);
						vector<string> firstTerms;
						if (shirkTerms.begin()->first == expandTerms.begin()->first){
							iter = ++shirkTerms.begin();
						}
						else{
							iter = shirkTerms.begin();
						}
						firstTerms.push_back(iter->first);
						vector<string>secondTerms = { expandTerms.begin()->first };
						mutants = dnfMutant.generateLIFmutants(expression, firstTerms, model);
						for (Mutant mutant : mutants.mutants){
							//printf("mutants:[%s]\n", mutant.mutant.c_str());
							fMutants += dnfMutant.generateLRFmutants("LIF", mutant.mutant.c_str(), secondTerms, model);
							fMutants += dnfMutant.generateLNFmutants("LIF", mutant.mutant.c_str(), secondTerms);
						}
						mutants = dnfMutant.generateTOFmutants(expression, firstTerms);
						for (Mutant mutant : mutants.mutants){
							//printf("mutants:[%s]\n", mutant.mutant.c_str());
							fMutants += dnfMutant.generateLRFmutants("TOF", mutant.mutant.c_str(), secondTerms, model);
							fMutants += dnfMutant.generateLNFmutants("TOF", mutant.mutant.c_str(), secondTerms);
						}
						mutants = dnfMutant.generateLRFmutants(expression, firstTerms, model);
						for (Mutant mutant : mutants.mutants){
							//printf("mutants1:[%s]\n", mutant.mutant.c_str());
							fMutants += dnfMutant.generateLNFmutants("LRF", mutant.mutant.c_str(), secondTerms);
						}
						mutants = dnfMutant.generateLNFmutants(expression, firstTerms);
						for (Mutant mutant : mutants.mutants){
							//printf("mutants2:[%s]\n", mutant.mutant.c_str());
							fMutants += dnfMutant.generateLRFmutants("LNF", mutant.mutant.c_str(), secondTerms, model);
						}

						//for (Mutant mutant : fMutants.mutants){
							//printf("fMutants:[%s]\n", mutant.mutant.c_str());
						//}
					}
				}
				else if (shirkTerms.size() == 2 && expandTerms.size() == 2){
					if (shirkTerms.begin()->first != expandTerms.begin()->first && shirkTerms.begin()->first != (++expandTerms.begin())->first){
						printf("不是双项双缺陷\n");
						return 0;
					}
					if ((++shirkTerms.begin())->first != expandTerms.begin()->first && (++shirkTerms.begin())->first != (++expandTerms.begin())->first){
						printf("不是双项双缺陷\n");
						return 0;
					}
					//printf("只能发生LRF、LNF与LRF、LNF\n");
					if (shirkTerms.begin()->first == expandTerms.begin()->first){
						iter = ++shirkTerms.begin();
					}
					else{
						iter = shirkTerms.begin();
					}
					DnfMutant dnfMutant;
					Mutants mutants(expression);
					iter = shirkTerms.begin();
					vector<string> firstTerms;
					firstTerms.push_back(iter->first);
					vector<string>secondTerms = { (++shirkTerms.begin())->first };
					mutants = dnfMutant.generateLRFmutants(expression, firstTerms, model);
					for (Mutant mutant : mutants.mutants){
						//printf("mutants1:[%s]\n", mutant.mutant.c_str());
						fMutants += dnfMutant.generateLRFmutants("LRF",mutant.mutant.c_str(), secondTerms, model);
						fMutants += dnfMutant.generateLNFmutants("LRF", mutant.mutant.c_str(), secondTerms);
					}
					mutants = dnfMutant.generateLNFmutants(expression, firstTerms);
					for (Mutant mutant : mutants.mutants){
						//printf("mutants1:[%s]\n", mutant.mutant.c_str());
						fMutants += dnfMutant.generateLRFmutants("LNF", mutant.mutant.c_str(), secondTerms, model);
						fMutants += dnfMutant.generateLNFmutants("LNF", mutant.mutant.c_str(), secondTerms);
					}

					//for (Mutant mutant : fMutants.mutants){
						//printf("fMutants:[%s]\n", mutant.mutant.c_str());
					//}
				}
				else if (shirkTerms.size() == 3 && expandTerms.size() == 1){
					if (TRUE)printf("只能发生DORF与LRF、LNF\n");
					vector<string> secondTerms;
					bool isE = false;
					for (iter = shirkTerms.begin(); iter != shirkTerms.end(); iter++){
						if (iter->first == expandTerms.begin()->first){
							isE = true;
							iter1 = iter;
						}
						else secondTerms.push_back(iter->first);
					}
					DnfMutant dnfMutant;
					Mutants mutants(expression);
					vector<string> firstTerms;
					if (isE == false){
						printf("扩张项和收缩项不同，说明是LRF和DORF，其中LRF在一个收缩项里\n");
						for (iter = shirkTerms.begin(); iter != shirkTerms.end(); iter++){
							vector<string> dd1, dd2;
							for (iter1 = shirkTerms.begin(); iter1 != shirkTerms.end(); iter1++){
								if (iter->first == iter1->first){
									dd1.push_back(iter->first);
								}
								else dd2.push_back(iter1->first);
							}
							mutants = dnfMutant.generateLRFmutants(expression, dd1, model);
							for (Mutant mutant : mutants.mutants){
								//printf("mutants:[%s]\n", mutant.mutant.c_str());
								fMutants += dnfMutant.generateDORFmutants("LRF", mutant.mutant.c_str(), dd2);
							}
						}
					}
					else{
						firstTerms.push_back(iter1->first);
						mutants = dnfMutant.generateLRFmutants(expression, firstTerms, model);
						for (Mutant mutant : mutants.mutants){
							//printf("mutants:[%s]\n", mutant.mutant.c_str());
							fMutants += dnfMutant.generateDORFmutants("LRF", mutant.mutant.c_str(), secondTerms);
						}
						mutants = dnfMutant.generateLNFmutants(expression, firstTerms);
						for (Mutant mutant : mutants.mutants){
							//printf("mutants:[%s]\n", mutant.mutant.c_str());
							fMutants += dnfMutant.generateDORFmutants("LNF", mutant.mutant.c_str(), secondTerms);
						}
					}
					//for (Mutant mutant : fMutants.mutants){
						//printf("fMutants:[%s]\n", mutant.mutant.c_str());
					//}
				}
				else{
					printf("不能判断缺陷类型\n");
					return 0;
				}
			}
			else{
				if (expandTerms.size() == 1){
					printf("这是单项扩张缺陷\n");
					return 0;
				}
				if (TRUE)printf("只能发生LOF或CORF，也考虑LRF或LOF\n");
				DnfMutant dnfMutant;
				Mutants mutants(expression);
				iter = expandTerms.begin();
				iter1 = ++expandTerms.begin();
				vector<string> firstTerms = { iter->first };
				vector<string>secondTerms = { iter1->first };
				mutants = dnfMutant.generateLOFmutants(expression, firstTerms);
				for (Mutant mutant : mutants.mutants){
					//printf("mutants:[%s]\n", mutant.mutant.c_str());
					fMutants += dnfMutant.generateLRFmutants("LOF", mutant.mutant.c_str(), secondTerms, model);
					fMutants += dnfMutant.generateLNFmutants("LOF", mutant.mutant.c_str(), secondTerms);
					fMutants += dnfMutant.generateCORFmutants("LOF", mutant.mutant.c_str(), secondTerms);
				}
				mutants = dnfMutant.generateCORFmutants(expression, firstTerms);
				for (Mutant mutant : mutants.mutants){
					//printf("mutants:[%s]\n", mutant.mutant.c_str());
					fMutants += dnfMutant.generateCORFmutants("CORF", mutant.mutant.c_str(), secondTerms);
					fMutants += dnfMutant.generateLOFmutants("CORF", mutant.mutant.c_str(), secondTerms);
				}
				//for (Mutant mutant : fMutants.mutants){
					//printf("fMutants:[%s]\n", mutant.mutant.c_str());
				//}
			}
		}
		else{
			printf("不是双缺陷无法诊断，或者变化等价于双重以上的缺陷，例如ab+ac+bd+ef->a+b+ac+bd+ef=a+b+ef，这也是本算法的缺憾所在！\n");
			return 0;
		}
	}
	vector<string>shirkCases, shirk2Cases;
	for (iter = shirkTerms.begin(); iter != shirkTerms.end(); iter ++){
		vector<string> termCases = iter->second;
		for (string shirkCase: termCases){
			if (findFromVector(shirkCases, shirkCase) < 0)shirkCases.push_back(shirkCase);
		}
	}
	bool diagnosisRet = diagnosis(expression, model, fMutants, shirkCases, expandCases, upPoints);
	if (diagnosisRet){
		printf("诊断成功！\n");
		return 1;
	}
	else{
		printf("诊断失败！\n");
		return -1;
	}
	return fMutants.mutants.size();
}
//
//int main(int argc, char* argv[])
//{
//	if (argc < 3){
//		printf("please enter:cmd expression\nfor example: TwoFaultDiagnosis.exe ab+cd+ef ab+cd!e+ef");
//		return -1;
//	}
//	string expression = argv[1];
//
//	vector<string> terms = utility.extractDNFstyle(expression);
//	string model = generateModel(expression);
//	printf("model:%s\n", model.c_str());
//	PRE_PROCESS_DATA preData;
//	//expression = generateIDNForICNF(expression, true);
//	//expression = generateDNF(terms);
//	terms = utility.extractDNFstyle(expression);
//	if (terms.size() < 2){
//		printf("It cannot be doube terms fault");
//		return -1;
//	}
//	getPreProcessData(expression, preData, true);
//	vector<string> upPoints = preData.up;
//	vector<string> otp2Points = preData.op2;
//	vector<string> fpPoints = preData.fp;
//	/*printf("\nups:");
//	for (string upPoint : upPoints){
//	printf("[%s]", upPoint.c_str());
//	}
//	printf("\n");*/
//
//	map<string, vector<string>> termUtps, termOtp2s, termNfps, termOfp2s;
//	getTermUtps(terms, upPoints, model, termUtps);
//	getTermOtp2s(terms, otp2Points, model, termOtp2s);//如果收缩，只需要判断utp和otp2
//	getTermNfps(expression, termUtps, model, termNfps);//如果是扩张，只需要判断Nfp
//
//	string faultExpression = "";
//	if (argc == 3){
//		faultExpression = argv[2];
//		printf("faultExpression:[%s]\n", faultExpression.c_str());
//		dealMain(expression, faultExpression, terms, model, upPoints, otp2Points, fpPoints,
//			termUtps, termOtp2s, termNfps, termOfp2s);
//	}
//	else if (argc > 3){
//		string firstType = argv[2];
//		string termIndex1 = argv[3];
//		string termIndex2 = argv[4];
//		int index1 = atoi(termIndex1.c_str());
//		int index2 = atoi(termIndex2.c_str());
//		vector<string> uuTerms1, uuTerms2;
//		printf("terms [%d][%d]:[%s][%s]\n", index1,index2, terms.at(index1).c_str(), terms.at(index2).c_str());
//		uuTerms1.push_back(terms.at(index1).c_str());
//		uuTerms2.push_back(terms.at(index2).c_str());
//		Mutants uuMutants = generateALLmutants(firstType.c_str(), expression, model, uuTerms1, uuTerms2, terms.at(index1 + 1).c_str());
//		FILE *fp = NULL;
//		string filePath = "./" + firstType + ".txt";
//		fp = fopen(filePath.c_str(), "w+");
//		for (Mutant uuM : uuMutants.mutants){
//			clock_t time_start = clock();
//			printf("uuM:[%s][%s]\n", uuM.type.c_str(), uuM.mutant.c_str());
//			int fMutantsNum = dealMain(expression, uuM.mutant.c_str(), terms, model, upPoints, otp2Points, fpPoints,
//				termUtps, termOtp2s, termNfps, termOfp2s);
//			clock_t time_end = clock();
//			fprintf(fp, "%s\t%d\t%d\n", uuM.mutant.c_str(), fMutantsNum, time_end - time_start);
//		}
//		fclose(fp);
//	}
//
//}