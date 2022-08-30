#include "stdafx.h"
#include "expTransform.h"
#include "calExpValue.h"


//确定测试用例在DNF的项中的真值
bool isDNFtermTrue(const char *  term, const char * testData, string model)
{
	//printf("%s,%s\n",term,testData);
	for (int i = 0; i<strlen(term); i++)
	{
		char ch = term[i];
		if ('!' == ch){
			i++;
			ch = term[i];
			//printf("%c[%d]=%c\n", ch, model.find(ch), testData[model.find(ch)]);
			if (testData[model.find(ch)] == '1'){
				return false;
			}
		}
		else{
			//printf("%c[%d]=%c\n", ch, model.find(ch), testData[model.find(ch)]);
			if (testData[model.find(ch)] == '0'){
				return false;
			}
		}
	}

	return true;
}
//确定测试用例在DNF表达式中的真值
bool evaluateDNF(string exp, const char * testData, string model)
{
	vector<string> terms;
	size_t start = 0;
	size_t pos = exp.find('+');
	while (pos != string::npos){
		terms.push_back(exp.substr(start, pos - start));
		start = pos + 1;
		pos = exp.find('+', start);
	}
	terms.push_back(exp.substr(start));

	for (int i = 0; i<terms.size(); i++)
	{
		const char * term = terms.at(i).c_str();
		if (isDNFtermTrue(term, testData, model))
			return true;
	}

	return false;
}
//确定测试用例在任意表达式中的真值，方法就是递归拆解为DNF，递归求真值
bool evaluate(string exp, const char * testData, string model)
{
	if (exp.length() == 0)
		return false;
	vector<string> dnfStyle = extractDNFstyle(exp);

	for (int i = 0; i<dnfStyle.size(); i++)
	{
		bool result = false;
		vector<string> cnfStyle = extractCNFstyle(dnfStyle[i]);
		for (int j = 0; j<cnfStyle.size(); j++)
		{
			string cnfStyleTerm = cnfStyle[j];
			if (cnfStyleTerm[0] == '!'&&cnfStyleTerm[1] == '('){
				string inBracketExp = cnfStyleTerm.substr(2, cnfStyleTerm.length() - 2);
				if (inBracketExp.find('(') != string::npos)
					result = evaluate(inBracketExp, testData, model);//recursion
				else
					result = evaluateDNF(inBracketExp, testData, model);//DNF
				result = !result;
			}
			else if (cnfStyleTerm[0] == '('){
				string inBracketExp = cnfStyleTerm.substr(1, cnfStyleTerm.length() - 2);
				if (inBracketExp.find('(') != string::npos)
					result = evaluate(inBracketExp, testData, model);//recursion
				else
					result = evaluateDNF(inBracketExp, testData, model);//DNF

			}
			else{
				result = evaluateDNF(cnfStyleTerm, testData, model);//DNF
			}
			if (!result)
				break;
		}//for
		if (result)
			return true;
	}//for

	return false;
}

//检查测试用例是否属于该测试用例集
bool checkCharacter(vector<string> uterms,vector<string> oterms,vector<map<string, short>> characters, const char * testData, string model){
	for (map<string, short> character : characters){
		map<string, short>::iterator iter;
		for (iter = character.begin(); iter != character.end(); iter++){
			string term = iter->first;
			short value = iter->second;
			if (value == 2)continue;
			if (term == "other"){
				for (string oterm : oterms){
					bool TorF = isDNFtermTrue(oterm.c_str(), testData, model);
					if (TorF == true && value == 0 || TorF == false && value == 1)return false;
				}
				continue;
			}
			if (term == "all"){
				for (string oterm : oterms){
					bool TorF = isDNFtermTrue(oterm.c_str(), testData, model);
					if (TorF == true && value == 0 || TorF == false && value == 1)return false;
				}
				for (string uterm : uterms){
					bool TorF = isDNFtermTrue(uterm.c_str(), testData, model);
					if (TorF == true && value == 0 || TorF == false && value == 1)return false;
				}
				continue;
			}
			bool TorF = isDNFtermTrue(term.c_str(), testData, model);
			if (TorF == true && value == 0 || TorF == false && value == 1)return false;
		}
	}
	return true;
}
