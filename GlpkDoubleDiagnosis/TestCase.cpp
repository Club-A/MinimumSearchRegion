#include "stdafx.h"
//#include "DiagnosisUtility.h"
#include "TestCase.h"
//#include "DiagnosisUtility.h"
#include "Utility.h"
#include <iostream>
#include <list> 
#include <unordered_set>
#include <iostream>
#include <string>
#include<sys/timeb.h>
#include <time.h>

TestCase::TestCase(void)
{

}
TestCase::TestCase(string exp, string t) :testcase(t)
{

}
TestCase::TestCase(string exp, string t, bool check) :testcase(t)
{
	time_interval = 0;
	timeb start;
	ftime(&start);
	Utility uti;
	string model = uti.generateModel(exp);

	if (check){
		for (UTP utp : getUTP(exp, t)){
			info.push_back(utp);
		}
		//其它值为FALSE的项的敏感点集合

		vector<string> terms = uti.extractDNFstyle(exp);
		for (int i = 0; i<terms.size(); i++){
			string term = terms[i];

			if (uti.evaluateDNF(term, t,model)){
				continue;
			}
			vector<string> literals = uti.extractCNFstyle(term);
			for (int j = 0; j<literals.size(); j++){
				string literal = literals[j];
				char l;
				if (literal.at(0) == '!'){
					if (t.at(model.find(literal.at(1))) == '1')
					susceptibleLiterSet.insert(Literal(literal.at(1), false));
				}
				else {
					if (t.at(model.find(literal.at(0))) == '0')
					susceptibleLiterSet.insert(Literal(literal.at(0), true));
				}				
			}
		}
	}
	else{
		for (NFP nfp : getNFP(exp, t)){
			//cout << "term is " << nfp.term << "key is " << nfp.getKey().getChar() << endl;

			info.push_back(nfp);
			Literal key = nfp.getKey();
			susceptibleLiterSet.insert(key);
		}		
	}
	timeb end;
	ftime(&end);
	time_interval += (end.time * 1000 + end.millitm - (start.time * 1000 + start.millitm));
}


list<UTPorNFP> TestCase::getInfo(string expression, string testcase, bool check){
	timeb start;
	ftime(&start);
	Utility uti;
	vector<string> terms = uti.extractDNFstyle(expression);
	string model = uti.generateModel(expression);
	list<UTPorNFP> list;
	if (check){
		//获取该测试用例UTP信息
		//需更新！！！
		int t = 0;
		for (t; t < terms.size(); t++){
			vector<string> literalsInTerm = uti.extractCNFstyle(terms[t]);
			string tc;
			if (testcase != ""){
				for (int l = 0; l < literalsInTerm.size(); l++){
					if (literalsInTerm[l].at(0) == '!'){
						tc = tc + testcase.at(model.find(literalsInTerm[l].at(1)));
					}
					else{
						tc = tc + testcase.at(model.find(literalsInTerm[l].at(0)));
					}
				}
			}
			if (uti.evaluate(terms[t], tc)){
				break;
			}
		}
		UTPorNFP utp(testcase, model, terms[t], true);
		list.push_back(utp);

	}
	else{
		//获取该测试用例NFP信息
		vector<string> NFPTerms;
		for (int t = 0; t < terms.size(); t++){
			string curTerm = uti.sortCNF(terms[t]);
			vector<string> literalInTerm = uti.extractCNFstyle(curTerm);
			for (int l = 0; l < literalInTerm.size(); l++){

				int* testData = (int*)malloc(model.length()*sizeof(int));
				for (int i = 0; i < model.length(); i++)
				{
					if (testcase[i] == '0')
						testData[i] = 0;
					else
						testData[i] = 1;
				}
				string curL = literalInTerm[l];
				char changeL = curL[0] == '!' ? curL.at(1) : curL.at(0);
				int changePos = model.find(changeL);
				testData[changePos] = testData[changePos] == 0 ? 1 : 0;
				if (uti.evaluateDNF(curTerm, testData, model)){
					NFPTerms.push_back(terms[t]);
				}
				free(testData);
			}
		}
		for (int i = 0; i < NFPTerms.size(); i++){
			UTPorNFP nfp(testcase, model, NFPTerms[i], false);
			list.push_back(nfp);
		}
	}
	timeb end;
	ftime(&end);
	time_interval += (end.time * 1000 + end.millitm - (start.time * 1000 + start.millitm));
	return list;
}

list<UTP> TestCase::getUTP(string expression, string testcase)
{
	timeb start;
	ftime(&start);
	Utility uti;
	vector<string> terms = uti.extractDNFstyle(expression);
	string model = uti.generateModel(expression);
	model.erase(0, model.find_first_not_of(" "));
	model.erase(model.find_last_not_of(" ") + 1);
	list<UTP> list;

	int t = 0;
	string term;
	while (t < terms.size()){
		term = terms[t];
		term.erase(0, term.find_first_not_of(" "));
		term.erase(term.find_last_not_of(" ") + 1);
		string _term = uti.sortTerm(term);
		vector<string> literalsInTerm = uti.extractCNFstyle(_term);
		string tc;
		if (testcase != ""){
			for (int l = 0; l < literalsInTerm.size(); l++){
				string lit = literalsInTerm[l];
				if (literalsInTerm[l].at(0) == '!'){
					tc = tc + testcase.at(model.find(literalsInTerm[l].at(1)));
					
				}
				else{
					tc = tc + testcase.at(model.find(literalsInTerm[l].at(0)));
				}
			}
		}
		if (uti.evaluate(term, tc)){

			break;
		}
		else{
			t++;
		}
	}

	UTP utp(testcase, term, model);
	list.push_back(utp);
	timeb end;
	ftime(&end);
	time_interval += (end.time * 1000 + end.millitm - (start.time * 1000 + start.millitm));
	return list;
}
list<NFP> TestCase::getNFP(string expression, string testcase)
{
	timeb start;
	ftime(&start);
	Utility uti;
	vector<string> terms = uti.extractDNFstyle(expression);
	string model = uti.generateModel(expression);
	list<NFP> list;

	vector<string> NFPTerms;
	for (int t = 0; t < terms.size(); t++){
		string curTerm = uti.sortCNF(terms[t]);
		vector<string> literalInTerm = uti.extractCNFstyle(curTerm);
		for (int l = 0; l < literalInTerm.size(); l++){

			int* testData = (int*)malloc(model.length()*sizeof(int));
			for (int i = 0; i < model.length(); i++)
			{
				if (testcase[i] == '0')
					testData[i] = 0;
				else
					testData[i] = 1;
			}
			string curL = literalInTerm[l];
			char changeL = curL[0] == '!' ? curL.at(1) : curL.at(0);
			int changePos = model.find(changeL);
			testData[changePos] = testData[changePos] == 0 ? 1 : 0;
			if (uti.evaluateDNF(curTerm, testData, model)){
				NFPTerms.push_back(terms[t]);

			}
			free(testData);
		}
	}
	for (int i = 0; i < NFPTerms.size(); i++){
		NFP nfp(testcase, NFPTerms[i], model);
		list.push_back(nfp);
	}
	timeb end;
	ftime(&end);
	int time_interval1 = (end.time * 1000 + end.millitm - (start.time * 1000 + start.millitm));
	time_interval = time_interval1+time_interval;
	cout << "起点" << start.time * 1000 + start.millitm << endl;
	cout << "终点" << end.time * 1000 + end.millitm << endl;
	cout << "差值" << time_interval1 << endl;
	cout << "时间差" << time_interval << endl;
	return list;
}
list<OTP> TestCase::getOTP(string expression, string testcase)
{
	timeb start;
	ftime(&start);
	Utility uti;
	vector<string> terms = uti.extractDNFstyle(expression);
	string model = uti.generateModel(expression);
	list<OTP> list;

	for (string term : terms){
		string _term = uti.sortCNF(term);
		vector<string> literals = uti.extractCNFstyle(_term);
		string tc = "";
		for (string literal : literals){
			char l = literal.at(0) == '!' ? literal.at(1) : literal.at(0);
			tc += testcase.at(model.find(l));
		}
		if (uti.evaluate(_term, tc)){
			OTP otp(testcase, term);
			list.push_back(otp);
		}
	}
	timeb end;
	ftime(&end);
	time_interval += (end.time * 1000 + end.millitm - (start.time * 1000 + start.millitm));
	return list;
}