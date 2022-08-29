#include "stdafx.h"
#include "expTransform.h"
#include "calExpValue.h"
#include "Utility.h"

//不在表达式中的词
string notOfExp(string exp, string model){
	char notOf[30] = { 0 };
	int len = 0;
	for (char u : model){
		int pos = exp.find(u);
		if (pos < 0){
			notOf[len] = u;
			len++;
		}
	}
	return notOf;
}
//生成词的个数
string generateModel(string exp)
{
	string model;
	for (int i = 0; i<exp.length(); i++)
	{
		wchar_t ch = exp[i];
		if (ch == '(' || ch == ')' || ch == '+' || ch == '!')
			continue;
		if (model.find(ch) == string::npos)
			model.push_back(exp[i]);
	}

	//insertion sort
	for (int i = 1; i<model.length(); i++)
	{
		for (int j = i; j>0; j--)
		{
			if (model[j]<model[j - 1]){
				wchar_t ch = model[j];
				model[j] = model[j - 1];
				model[j - 1] = ch;
			}
			else
				break;
		}
	}
	return model;
}
//将DNF表达式拆分成一组项
//For example, extract "a!d(e(b+c)+a)+c(d+e)" into two terms: "a!d(e(b+c)+a)" and "c(d+e)"
vector<string> extractDNFstyle(string exp)
{
	vector<string> dnfStyle;
	int inBrackets = 0;
	int termStart = 0;
	for (int i = 0; i<exp.length(); i++)
	{
		wchar_t ch = exp[i];
		if (ch == '(')
			inBrackets++;
		else if (ch == ')')
			inBrackets--;
		else if (ch == '+'&&inBrackets == 0){
			string term = exp.substr(termStart, i - termStart);
			dnfStyle.push_back(term);
			termStart = i + 1;
		}
	}//for
	string term = exp.substr(termStart);
	dnfStyle.push_back(term);
	return dnfStyle;
}
//将一个项拆分成词
//For example, extract "a!d(e(b+c)+a)" into three terms: "a" ,"!d" and "(e(b+c)+a)"
vector<string> extractCNFstyle(string exp)
{
	vector<string> result;

	int inBrackets = 0;
	int bracketTermStart = 0;
	for (int i = 0; i<exp.length(); i++)
	{
		wchar_t ch = exp[i];
		if (inBrackets == 0){
			if (ch == '('){
				inBrackets++;
				bracketTermStart = i;
			}
			else if (ch == '!')
				continue;
			else{
				string s;
				if (i != 0 && exp[i - 1] == '!'){
					s.push_back('!');
					s.push_back(ch);
					result.push_back(s);
				}
				else{
					s.push_back(ch);
					result.push_back(s);
				}
			}
		}
		else{
			if (ch == ')'&&inBrackets == 1){
				if (bracketTermStart != 0 && exp[bracketTermStart - 1] == '!')
					result.push_back(exp.substr(bracketTermStart - 1, i + 1 - (bracketTermStart - 1)));
				else
					result.push_back(exp.substr(bracketTermStart, i + 1 - bracketTermStart));
				inBrackets--;
			}
			else if (ch == '(')
				inBrackets++;
			else if (ch == ')')
				inBrackets--;
			else
				continue;
		}
	}//for
	return result;
}
//将多个项合并为一个表达式
string terms2Exp(vector<string> terms){
	string exp = "";
	int s = 0;
	for (string term : terms){
		if (s == 0){
			exp += term;
			s = 1;
		}
		else exp += "+" + term;
	}
	return exp;
}
//给指定的项插入一个词
void termLif(string &term, const char * ch, vector<map<string, short>> &characters){
	//特征情况如下
	map<string, short> character;
	character.insert(pair<string, short>(term, 1));//当前项为真
	character.insert(pair<string, short>(ch, 0));//插入词为假
	character.insert(pair<string, short>("other", 0));//其他项为假
	characters.push_back(character);
	term = term + ch;
}

string getOneLifCase(string sterm, vector<string> upPoints, string model){
	//只有收缩，所以获取一个收缩测试用例
	Utility utility;
	for (string upPoint : upPoints){
		int iCase[30] = { 0 };
		int pp = 0;
		for (char c : upPoint){
			iCase[pp] = c - '0';
			pp++;
		}
		bool idt = utility.isDNFtermTrue(sterm, iCase, model);
		if (idt == false){
			return upPoint;
		}
	}
	return NULL;
}
//给指定的项变换
void termLrf(string &term, const char * ch, const char *newch, vector<map<string, short>> &characters){
	int y = term.find(ch);
	if (y < 0)return;

	//特征情况如下
	map<string, short> character1;
	character1.insert(pair<string, short>(term, 1));//当前项为真
	character1.insert(pair<string, short>(newch, 0));//替换词为假
	character1.insert(pair<string, short>("other", 0));//其他项为假
	characters.push_back(character1);

	map<string, short> character2;
	character2.insert(pair<string, short>(term, 0));//当前项为假
	if (ch[0] == '!'){//如果是!替换
		term.replace(y, 2, newch);
	}
	else{
		const char * t = term.c_str();
		if (y > 0 && t[y - 1] == '!'){//如果是!替换
			term.replace(y - 1, 2, newch);
		}
		else term.replace(y, 1, newch);
	}
	character2.insert(pair<string, short>(term, 1));//替换后为真
	character2.insert(pair<string, short>("other", 0));//其他项为假
	characters.push_back(character2);
}
int termLrf(string &term, int pos, const char *newch, vector<map<string, short>> &characters){
	int y = pos;
	if (y < 0)return 0;
	if (pos >= term.size())return 0;
	const char * t = term.c_str();
	int ret = 0;
	//特征情况如下
	map<string, short> character1;
	character1.insert(pair<string, short>(term, 1));//当前项为真
	character1.insert(pair<string, short>(newch, 0));//替换词为假
	character1.insert(pair<string, short>("other", 0));//其他项为假
	characters.push_back(character1);

	map<string, short> character2;
	character2.insert(pair<string, short>(term, 0));//当前项为假
	if (t[y] == '!'){//如果是!替换
		term.replace(y, 2, newch);
		ret = 2;
	}
	else{
		if (y > 0 && t[y - 1] == '!'){//如果是!替换
			term.replace(y - 1, 2, newch);
			ret = 2;
		}
		else{
			term.replace(y, 1, newch);
			ret = 1;
		}
	}
	character2.insert(pair<string, short>(term, 1));//替换后为真
	character2.insert(pair<string, short>("other", 0));//其他项为假
	characters.push_back(character2);
	return ret;
}

string getOneLrfECase(string eterm, vector<string> nfpPoints, string model){
	//获取一个扩张测试用例
	Utility utility;
	for (string nfpPoint : nfpPoints){
		int iCase[30] = { 0 };
		int pp = 0;
		for (char c : nfpPoint){
			iCase[pp] = c - '0';
			pp++;
		}
		bool idt = utility.isDNFtermTrue(eterm, iCase, model);
		if (idt == true){
			return nfpPoint;
		}
	}
	return NULL;
}
//给指定项的词缺失
void termLof(string &term, const char * ch, vector<map<string, short>> &characters){
	int y = term.find(ch);
	if (y < 0)return;//特征情况如下
	map<string, short> character;
	character.insert(pair<string, short>(term, 0));//当前项为假

	if (ch[0] == '!'){//如果是!缺失
		term.erase(y, 2);
	}
	else{
		const char * t = term.c_str();
		if (y > 0 && t[y - 1] == '!'){//如果是!缺失
			term.erase(y - 1, 2);
		}
		else term.erase(y, 1);
	}
	character.insert(pair<string, short>(term, 1));//缺失词后为真
	character.insert(pair<string, short>("other", 0));//其他项为假
	characters.push_back(character);
}
int termLof(string &term, int pos, vector<map<string, short>> &characters){
	int y = pos;
	if (y < 0)return 0;
	if (pos >= term.size())return 0;
	const char * t = term.c_str();
	int ret = 0;
	map<string, short> character;
	character.insert(pair<string, short>(term, 0));//当前项为假
	if (t[y] == '!'){//如果是!缺失
		term.erase(y, 2);
		ret = 2;
	}
	else{
		if (y > 0 && t[y - 1] == '!'){//如果是!缺失
			term.erase(y - 1, 2);
			ret = 2;
		}
		else{
			term.erase(y, 1);
			ret = 1;
		}
	}
	character.insert(pair<string, short>(term, 1));//缺失词后为真
	character.insert(pair<string, short>("other", 0));//其他项为假
	characters.push_back(character);
	return ret;
}
//给指定项的次取反
void termLnf(string &term, const char * ch, vector<map<string, short>> &characters){
	int y = term.find(ch);
	if (y < 0)return;
	//特征情况如下
	map<string, short> character1;
	character1.insert(pair<string, short>(term, 1));//当前项为真
	character1.insert(pair<string, short>("other", 0));//其他项为假
	characters.push_back(character1);

	map<string, short> character2;
	character2.insert(pair<string, short>(term, 0));//当前项为假
	if (ch[0] == '!'){//如果是!取反
		term.erase(y, 1);
	}
	else{
		const char * t = term.c_str();
		if (y > 0 && t[y - 1] == '!'){//如果是!取反
			term.erase(y - 1, 1);
		}
		else{
			term.insert(y, "!");
		}
	}
	character2.insert(pair<string, short>(term, 1));//词取反后为真
	character2.insert(pair<string, short>("other", 0));//其他项为假
	characters.push_back(character2);
}
int termLnf(string &term, int pos, vector<map<string, short>> &characters){
	int y = pos;
	if (y < 0)return 0;
	if (pos >= term.size())return 0;
	const char * t = term.c_str();
	int ret = 0;
	//特征情况如下
	map<string, short> character1;
	character1.insert(pair<string, short>(term, 1));//当前项为真
	character1.insert(pair<string, short>("other", 0));//其他项为假
	characters.push_back(character1);

	map<string, short> character2;
	character2.insert(pair<string, short>(term, 0));//当前项为假
	if (t[y] == '!'){//如果是!取反
		term.erase(y, 1);
		ret = 2;
	}
	else{
		if (y > 0 && t[y - 1] == '!'){//如果是!取反
			term.erase(y - 1, 1);
			ret = 2;
		}
		else{
			term.insert(y, "!");
			ret = 1;
		}
	}
	character2.insert(pair<string, short>(term, 1));//词取反后为真
	character2.insert(pair<string, short>("other", 0));//其他项为假
	characters.push_back(character2);
	return ret;
}
//给指定项缺失
void expTof(vector<string>&terms, int pos, vector<map<string, short>> &characters){
	if (pos >= terms.size())return;
	//特征情况如下
	map<string, short> character;
	character.insert(pair<string, short>(terms.at(pos), 1));//当前项为真
	character.insert(pair<string, short>("other", 0));//其他项为假
	characters.push_back(character);
	terms.erase(terms.begin() + pos);
}

//给指定项取反
void expTnf(vector<string>&terms, int pos, vector<map<string, short>> &characters){
	if (pos >= terms.size())return;
	//特征情况如下
	map<string, short> character;
	character.insert(pair<string, short>("other", 0));//其他项为假
	characters.push_back(character);
	string term = terms.at(pos);
	term = "!(" + term + ")";
	terms[pos] = term;
}

//给指定两项合并
string termDorf(string term1, string term2, vector<map<string, short>> &characters){
	string term = term1 + term2;
	//特征情况如下
	map<string, short> character1;
	character1.insert(pair<string, short>(term1, 1));//当前项第一个为真
	character1.insert(pair<string, short>(term2, 0)); //第二个为假
	character1.insert(pair<string, short>("other", 0));//其他项为假
	characters.push_back(character1);
	map<string, short> character2;
	character2.insert(pair<string, short>(term1, 0));//当前项第二个为假
	character2.insert(pair<string, short>(term2, 1));//第一个为真
	character2.insert(pair<string, short>("other", 0));//其他项为假
	characters.push_back(character1);
	characters.push_back(character2);
	return term;
}
void expDorf(vector<string>&terms, int pos1, int pos2, vector<map<string, short>> &characters){
	if (pos1 >= terms.size() || pos2 >= terms.size())return;
	string term1 = terms.at(pos1);
	string term2 = terms.at(pos2);
	//特征情况如下
	map<string, short> character1;
	character1.insert(pair<string, short>(term1, 1));//当前项第一个为真
	character1.insert(pair<string, short>(term2, 0)); //第二个为假
	character1.insert(pair<string, short>("other", 0));//其他项为假
	characters.push_back(character1);
	map<string, short> character2;
	character2.insert(pair<string, short>(term1, 0));//当前项第二个为假
	character2.insert(pair<string, short>(term2, 1));//第一个为真
	character2.insert(pair<string, short>("other", 0));//其他项为假
	characters.push_back(character1);
	characters.push_back(character2);
	string term = term1 + term2;
	terms[pos1] = term;
	terms.erase(terms.begin() + pos2);
}

//给指定项拆分
vector<string> termCorf(string &term, int termPos, vector<map<string, short>> &characters){
	vector<string> v;
	if (termPos >= term.size() - 1)return v;
	int u = 0;
	if (term.at(termPos) == '!'){
		termPos++;
		u = 1;
	}
	string subterm1 = term.substr(0, termPos + 1);
	string subterm2 = term.substr(termPos + 1, term.size() - 1);

	//特征情况如下
	map<string, short> character1;
	character1.insert(pair<string, short>(subterm1, 1));//当前项第一个为真
	character1.insert(pair<string, short>(subterm2, 0)); //第二个为假
	character1.insert(pair<string, short>("all", 0));//所有项为假
	characters.push_back(character1);
	map<string, short> character2;
	character2.insert(pair<string, short>(subterm1, 0));//当前项第二个为假
	character2.insert(pair<string, short>(subterm2, 1));//第一个为真
	character2.insert(pair<string, short>("all", 0));//所有项为假
	characters.push_back(character1);
	characters.push_back(character2);
	vector<string>subTerms;
	subTerms.push_back(subterm1);
	subTerms.push_back(subterm2);
	return subTerms;
}
int expCorf(vector<string>&terms, int expPos, int termPos, vector<map<string, short>> &characters){
	if (expPos >= terms.size())return 0;
	string term = terms.at(expPos);
	if (termPos >= term.size() - 1)return 0;
	int u = 0;
	if (term.at(termPos) == '!'){
		termPos++;
		u = 1;
	}
	string subterm1 = term.substr(0, termPos + 1);
	string subterm2 = term.substr(termPos + 1, term.size() - 1);

	//特征情况如下
	map<string, short> character1;
	character1.insert(pair<string, short>(subterm1, 1));//当前项第一个为真
	character1.insert(pair<string, short>(subterm2, 0)); //第二个为假
	character1.insert(pair<string, short>("all", 0));//所有项为假
	characters.push_back(character1);
	map<string, short> character2;
	character2.insert(pair<string, short>(subterm1, 0));//当前项第二个为假
	character2.insert(pair<string, short>(subterm2, 1));//第一个为真
	character2.insert(pair<string, short>("all", 0));//所有项为假
	characters.push_back(character1);
	characters.push_back(character2);
	terms[expPos] = subterm1;
	terms.push_back(subterm2);
	return u;
}

//给指定几个项整体取反
vector<string> termEnf(vector<string>&terms, vector<int>poss, vector<map<string, short>> &characters){
	vector<string>terms1;
	for (int pos = poss.size() - 1; pos >= 0; pos--){
		if (poss[pos] >= terms.size())return terms1;
		terms1.push_back(terms.at(poss[pos]));
		terms.erase(terms.begin() + poss[pos]);
	}
	return terms1;
}

void expEnf(vector<string>&terms, vector<int>poss, vector<map<string, short>> &characters){
	vector<string>terms1;//特征情况如下
	map<string, short> character;
	character.insert(pair<string, short>("other", 0));//其他项为假
	characters.push_back(character);
	for (int pos = poss.size() - 1; pos >= 0; pos--){
		if (poss[pos] >= terms.size())return;
		terms1.push_back(terms.at(poss[pos]));
		terms.erase(terms.begin() + poss[pos]);
	}
	string exp = "!(" + terms2Exp(terms1) + ")";
	terms.push_back(exp);
}

