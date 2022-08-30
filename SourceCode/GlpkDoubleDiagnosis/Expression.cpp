#include "stdafx.h"
#include "Expression.h"
#include "BDD.h"
#include "time.h"


bool checkEqualment(string str1, string str2)
{
	BDD string1(str1);
	BDD string2(str2);
	return string1.isEquivalent(string2);
}


//求IDNF的FP
bool generateFP(string idnf, vector<string>& fp)
{
	if (idnf.size() == 0)
		return false;
	string exp = idnf;

	Utility utility;
	string model = utility.generateModel(exp);
	int count;
	for (int i = 0; i < (int)pow(2, model.length()); i++) {//2^词 
		string s = utility.intTostring(i, model.length());
		bool f = utility.evaluateDNF(exp, s, model);
		if (!f) {
			fp.push_back(s);
		}
	}
	return true;
}

void generateTPandFP(string exp,vector<Point>& TPs,list<Point>& FPs, bool withBDD)
{
	Utility utility;
	string model=utility.generateModel(exp);
	
	if (withBDD)
	{
		string expression(exp.c_str());
		BDD bdd(expression);
		for(int i=0;i<pow(2,model.length());i++)
		{
			string value=utility.intTostring(i,model.length());
			Point point(value);
			if(bdd.evaluate(i)){
				TPs.push_back(point);
			}else{
				FPs.push_back(point);
			}
		}
	}else
	{
		for(int i=0;i<pow(2,model.length());i++)
		{
			string value=utility.intTostring(i,model.length());
			Point point(value);
			if(utility.evaluate(exp,i)){
				TPs.push_back(point);
			}else{
				FPs.push_back(point);
			}
		}
	}
}
void generateFPandTP(string exp,vector<Point>& FPs,list<Point>& TPs, bool withBDD)
{
	Utility utility;
	string model=utility.generateModel(exp);
	if (withBDD)
	{
		string expression(exp.c_str());
		BDD bdd(expression);
		for(int i=0;i<pow(2,model.length());i++)
		{
			string value=utility.intTostring(i,model.length());
			Point point(value);
			if(bdd.evaluate(i)){
				TPs.push_back(point);
			}else{
				FPs.push_back(point);
			}
		}
	}else
	{
		for(int i=0;i<pow(2,model.length());i++)
		{
			string value=utility.intTostring(i,model.length());
			Point point(value);
			if(utility.evaluate(exp,i)){
				TPs.push_back(point);
			}else{
				FPs.push_back(point);
			}
		}
	}
	
}

bool isSpecifiedFieldInPoints(int n,vector<int>&p, string value, vector<Point>&Ps)
{
	for(vector<Point>::iterator itr=Ps.begin();itr!=Ps.end();itr++)
	{
		bool isIn=true;
		Point ps=*itr;
		string psValue=ps.getValue();
		for(int i=0;i<n;i++)
		{
			if(value[p[i]]!=psValue[p[i]]){
				isIn=false;
				break;
			}
		}//for
		if(isIn)
			return true;
	}//for
	return false;
}

bool getNumbers(int n, vector<int>& result, Point fPortP, vector<Point>&Ps)
{
	string value=fPortP.getValue();
	int len=value.length();
	if(n<1||n>len)
		return false;

	vector<int> p;
	for(int i=0;i<n;i++)
	{
		p.push_back(i);
	}
	while(true)
	{
		for(;p[n-1]<len;p[n-1]++)
		{
			//do here
			if(!isSpecifiedFieldInPoints(n,p,value,Ps)){
				for(int j=0;j<n;j++){
					result.push_back(p[j]);
				}
				return true;
			}
		}//for

		if(n==1&&p[0]==len)
			break;
		else if(p[0]==len-n)
			break;
		
		for(int k=n-2;k>=0;k--)
		{
			if(p[k]<len-(n-k)){
				p[k]++;
				for(int j=k+1;j<n;j++){
					p[j]=p[j-1]+1;
				}
				break;
			}
		}//for
	}//while
	return false;
}

vector<int> generateTermField(Point fPortP,vector<Point>&Ps)
{
	vector<int> result;

	int len=fPortP.getValue().length();
	int i = 0;
	for(i=1;i<=len;i++)
	{
		if (getNumbers(i, result, fPortP, Ps))
			break;// return result;
	}

	return result;
}

void handleUniqueCoverPoints(vector<int>&p, string value, list<Point>&UCPs)
{
	int n=p.size();
	for(list<Point>::iterator itr=UCPs.begin();itr!=UCPs.end();)
	{
		bool isIn=true;
		Point tp=*itr;
		string tpValue=tp.getValue();
		for(int i=0;i<n;i++)
		{
			if(value[p[i]]!=tpValue[p[i]]){
				isIn=false;
				break;
			}
		}//for
		if(isIn){
			list<Point>::iterator it=itr;
			itr++;
			UCPs.erase(it);
		}else
			itr++;
	}//for
}
void handleNotCoverPoints(vector<int>&p, string value,int termNum, list<Point>&NCPs,list<Point>&UCPs)
{
	int n=p.size();

	for(list<Point>::iterator itr=NCPs.begin();itr!=NCPs.end();)
	{
		bool isIn=true;
		Point tp=*itr;
		string tpValue = tp.getValue();
		for(int i=0;i<n;i++)
		{
			if(value[p[i]]!=tpValue[p[i]]){
				isIn=false;
				break;
			}
		}//for
		if(isIn){
			itr->setCoverTerm(termNum);
			UCPs.push_back(*itr);
			list<Point>::iterator it=itr;
			itr++;
			NCPs.erase(it);
		}else
			itr++;
	}//for
}

string  generateIDNForICNF(string expression, bool DNF1CNF0,bool withBDD)
{
	if(expression.size()==0)
		return "";
	string exp = expression;
	vector<Point> TrueOrFalsePs;//表达式值为0
	list<Point> notCoverPs;//表达式值为1
	list<Point> uniqueCoverPs;
	clock_t time1 = clock();
	if(DNF1CNF0)
		generateFPandTP(exp,TrueOrFalsePs,notCoverPs,withBDD);
	else
		generateTPandFP(exp,TrueOrFalsePs,notCoverPs,withBDD);
	
	list<Point>::iterator itor;
	itor = notCoverPs.begin();

	if(TrueOrFalsePs.empty())
		return  "";
	else if(notCoverPs.empty())
		return  "";

	vector<Point> DNFtermTPs;
	vector<vector<int>> DNFtermFields;
	int termNum = 0;

	while(!notCoverPs.empty())
	{
		Point tp = notCoverPs.front();
		vector<int> field = generateTermField(tp, TrueOrFalsePs);
		DNFtermTPs.push_back(tp);
		DNFtermFields.push_back(field);
		handleUniqueCoverPoints(field, tp.getValue(), uniqueCoverPs);
		handleNotCoverPoints(field, tp.getValue(), termNum++, notCoverPs, uniqueCoverPs);
	}
	vector<int>selectedTerms;
	for(list<Point>::iterator itr=uniqueCoverPs.begin();itr!=uniqueCoverPs.end();itr++)
	{
		Point p=*itr;
		int num=p.getCoverTerm();
		bool hasyet=false;
		for(vector<int>::iterator it=selectedTerms.begin();it!=selectedTerms.end();it++){
			if(num==*it){
				hasyet=true;
				break;
			}
		}
		if(!hasyet)
			selectedTerms.push_back(num);
	}
	string INF;
	Utility uti;
	string model=uti.generateModel(exp);
	for(vector<int>::iterator itr=selectedTerms.begin();itr!=selectedTerms.end();itr++)
	{
		string term;

		int termNum=*itr;
		Point point=DNFtermTPs[termNum];
		string value=point.getValue();
		vector<int>field=DNFtermFields[termNum];
		if (DNF1CNF0)
		{
			for(vector<int>::iterator it=field.begin();it!=field.end();it++)
			{
				if(value[*it]=='1'){
					string s;
					s.push_back(model[*it]);
					term.append(s);
				}
				else{
					string s;
					s.push_back('!');
					s.push_back(model[*it]);
					term.append(s);
				}
			}
			term.push_back('+');
		}else
		{
			for(vector<int>::iterator it=field.begin();it!=field.end();it++)
			{
				if(value[*it]=='0'){
					string s;
					s.push_back(model[*it]);
					s.push_back('+');
					term.append(s);
				}
				else{
					string s;
					s.push_back('!');
					s.push_back(model[*it]);
					s.push_back('+');
					term.append(s);
				}
			}
			if(term.back()=='+')
				term.pop_back();
			if(term.length()>2)
				term="("+term+")";
		}//else
		
		INF.append(term);
	}
	if (DNF1CNF0)
	{
		if(INF.back()=='+')
			INF.pop_back();
		INF=uti.sortDNF(INF);
	}else
	{
		INF=uti.sortCNF(INF);
	}
	return INF;
}

bool generateOFPandUFP(string icnf, vector<string>& ofp, vector<string>& ufp)
{
	if(icnf.size()==0)
		return false;
	string exp = icnf;

	Utility utility;
	string model=utility.generateModel(exp);
	vector<string> terms=utility.extractCNFstyle(exp);

	int count;
	for (int i = 0; i < (int)pow(2, model.length()); i++) {
		count = -1;
		for (int j = 0; j < terms.size(); j++) {
			if (utility.isCNFtermFalse(terms[j], i, model)) {
				count++;
			}
		}
		if (count == 0) {
			string s=utility.intTostring(i,model.length());
			ufp.push_back(s);
		}else if(count>0){
			string s=utility.intTostring(i,model.length());
			ofp.push_back(s);
		}
	}

	return true;
}

bool generateOTPandUTPandFP(string idnf, vector<string>& otp, vector<string>& op2, vector<string>& utp, vector<string>& fp)
{
	if (idnf.size() == 0)
		return false;
	string exp = idnf;

	Utility utility;
	string model = utility.generateModel(exp);
	vector<string> terms = utility.extractDNFstyle(exp);

	int count;
	for (int i = 0; i < (int)pow(2, model.length()); i++) {
		count = -1;
		for (int j = 0; j < terms.size(); j++) {
			if (utility.isDNFtermTrue(terms[j], i, model)) {
				count++;
			}
		}
		if (count == 0) {
			string s = utility.intTostring(i, model.length());
			utp.push_back(s);
		}
		else if (count>0){
			string s = utility.intTostring(i, model.length());
			otp.push_back(s);
			if (count == 1){
				op2.push_back(s);
			}
		}
		else{
			string s = utility.intTostring(i, model.length());
			fp.push_back(s);
		}
	}

	return true;
}
bool generateOTPandUTP(string idnf, vector<string>& otp, vector<string>& utp)
{
	if(idnf.size()==0)
		return false;
	string exp = idnf;

	Utility utility;
	string model=utility.generateModel(exp);
	vector<string> terms=utility.extractDNFstyle(exp);

	int count;
	for (int i = 0; i < (int)pow(2, model.length()); i++) {
		count = -1;
		for (int j = 0; j < terms.size(); j++) {
			if (utility.isDNFtermTrue(terms[j], i, model)) {
				count++;
			}
		}
		if (count == 0) {
			string s=utility.intTostring(i,model.length());
			utp.push_back(s);
		}else if(count>0){
			string s=utility.intTostring(i,model.length());
			otp.push_back(s);
		}
	}

	return true;
}
//生成两项otp
bool generateOTPover2(string idnf, vector<string>& otpover2)
{
	if(idnf.size()==0)
		return false;
	string exp = idnf;

	Utility utility;
	string model=utility.generateModel(exp);
	vector<string> terms=utility.extractDNFstyle(exp);

	int count;
	for (int i = 0; i < (int)pow(2, model.length()); i++) {
		count = -1;
		for (int j = 0; j < terms.size(); j++) {
			if (utility.isDNFtermTrue(terms[j], i, model)) {
				count++;
			}
		}
		if (count > 1) {
			string s=utility.intTostring(i,model.length());
			otpover2.push_back(s);
		}
	}

	return true;
}

bool generateLitNumTermNum(string expression, int& literalNum, int& termNum, bool DNF1CNF0)
{
	if(expression.size()==0)
		return false;
	string exp = expression;

	Utility utility;
	string model=utility.generateModel(exp);
	vector<string> terms;
	if (DNF1CNF0)
		terms=utility.extractDNFstyle(exp);
	else
		terms=utility.extractCNFstyle(exp);

	literalNum=model.length();
	termNum=terms.size();

	return true;
}

string generateCNFtermDimension(string model, string cnfTerm)
{
	if(cnfTerm.size()==0||model.size()==0)
		return "";
	string cnfT = cnfTerm;
	string chars = model;
	string dimensions;

	//term=term.replaceAll("\\(|\\)|\\+","");
	string term;
	for(int i=0;i<cnfT.length();i++)
	{
		wchar_t ch=cnfT[i];
		if(ch=='('||ch==')'||ch=='+')
			continue;
		term.push_back(ch);
	}
		
	int total = chars.length();
	for (int i = 0; i < total; i = i + 2) {

		char ichar = chars[i];
		int idx = term.find(ichar); // position of current literal in term
										
		if (i + 1 < total) {

			char jchar = chars[i + 1];
			int jdx = term.find(jchar); // position of next literal in
											// term

			if (idx > -1) {
				if (jdx > -1) {
					// all in
					if (idx > 0 && term[idx - 1] == '!') { // whether  a negation

						if (jdx > 0 && term[jdx - 1]== '!') {
							dimensions.append("11\n");
						} else {
							dimensions.append("10\n");
						}
					} else {

						if (jdx > 0 && term[jdx - 1] == '!') {
							dimensions.append("01\n");
						} else {
							dimensions.append("00\n");
						}
					}

				} else {
					// only ichar in
					if (idx > 0 && term[idx - 1] == '!') { // whether a negation
															
						dimensions.append("11 10\n");
					} else {
						dimensions.append("00 01\n");
					}
				}

			} else {

				if (jdx > -1) {
					// only jchar in
					if (jdx > 0 && term[jdx - 1]== '!') { // whether negation
																	
						dimensions.append("01 11\n");
					} else {
						dimensions.append("00 10\n");
					}

				} else {
					// no one in
					dimensions.append("00 01 11 10\n");
				}
			}

		} else {
			if (idx > -1) {
				if (idx == 0) {
					dimensions.append("0\n");
				} else {
					if (term[idx - 1] == '!') { // whether a negation
						dimensions.append("1\n");
					} else {
						dimensions.append("0\n");
					}
				}
			} else {
				// no one in
				dimensions.append("0 1\n");
			}
		}
	}

	return dimensions;
}

std::vector<string> genCNFtermDimensionVector(string model, string cnfTerm)
{
	std::vector<string> result;

	if(cnfTerm.size()==0||model.size()==0)
		return result;

	string cnfT = cnfTerm;
	string chars = model;

	//term=term.replaceAll("\\(|\\)|\\+","");
	string term;
	for(int i=0;i<cnfT.length();i++)
	{
		wchar_t ch=cnfT[i];
		if(ch=='('||ch==')'||ch=='+')
			continue;
		term.push_back(ch);
	}
		
	int total = chars.length();
	for (int i = 0; i < total; i = i + 2) {

		char ichar = chars[i];
		int idx = term.find(ichar); // position of current literal in term
										
		if (i + 1 < total) {

			char jchar = chars[i + 1];
			int jdx = term.find(jchar); // position of next literal in
											// term

			if (idx > -1) {
				if (jdx > -1) {
					// all in
					if (idx > 0 && term[idx - 1] == '!') { // whether  a negation

						if (jdx > 0 && term[jdx - 1]== '!') {
							result.push_back("11");
						} else {
							result.push_back("10");
						}
					} else {

						if (jdx > 0 && term[jdx - 1] == '!') {
							result.push_back("01");
						} else {
							result.push_back("00");
						}
					}

				} else {
					// only ichar in
					if (idx > 0 && term[idx - 1] == '!') { // whether a negation
															
						result.push_back("11 10");
					} else {
						result.push_back("00 01");
					}
				}

			} else {

				if (jdx > -1) {
					// only jchar in
					if (jdx > 0 && term[jdx - 1]== '!') { // whether negation
																	
						result.push_back("01 11");
					} else {
						result.push_back("00 10");
					}

				} else {
					// no one in
					result.push_back("00 01 11 10");
				}
			}

		} else {
			if (idx > -1) {
				if (idx == 0) {
					result.push_back("0");
				} else {
					if (term[idx - 1] == '!') { // whether a negation
						result.push_back("1");
					} else {
						result.push_back("0");
					}
				}
			} else {
				// no one in
				result.push_back("0 1");
			}
		}
	}

	return result;
}

std::vector<string> genDNFtermDimensionVector(string model, string dnfTerm)
{
	std::vector<string> result;

	if(dnfTerm.size()==0||model.size()==0)
		return result;
	string dnfT = dnfTerm;
	string chars = model;
	string term = dnfTerm;
	
	int total = chars.length();
	for (int i = 0; i < total; i = i + 2) {

		char ichar = chars[i];
		int idx = term.find(ichar); // position of current literal in
										// term

		if (i + 1 < total) {

			char jchar = chars[i + 1];
			int jdx = term.find(jchar); // position of next literal in
											// term

			if (idx > -1) {
				if (jdx > -1) {
					// all in
					if (idx > 0 && term[idx - 1] == '!') { // whether
																	// a
																	// negation

						if (jdx > 0 && term[jdx - 1] == '!') {
							result.push_back("00");
						} else {
							result.push_back("01");
						}
					} else {

						if (jdx > 0 && term[jdx - 1] == '!') {
							result.push_back("10");
						} else {
							result.push_back("11");
						}
					}

				} else {
					// only ichar in
					if (idx > 0 && term[idx - 1] == '!') { // whether
																	// a
																	// negation
						result.push_back("00 01");
					} else {
						result.push_back("11 10");
					}
				}

			} else {

				if (jdx > -1) {
					// only jchar in
					if (jdx > 0 && term[jdx - 1] == '!') { // whether
																	// a
																	// negation
						result.push_back("00 10");
					} else {
						result.push_back("01 11");
					}

				} else {
					// no one in
					result.push_back("00 01 11 10");
				}
			}

		} else {
			if (idx > -1) {
				if (idx == 0) {
					result.push_back("1");
				} else {
					if (term[idx - 1] == '!') { // whether a negation
						result.push_back("0");
					} else {
						result.push_back("1");
					}
				}
			} else {
				// no one in
				result.push_back("0 1");
			}
		}
	}

	return result;
}

string generateDNFtermDimension(string model, string dnfTerm)
{
	if(dnfTerm.size()==0||model.size()==0)
		return "";
	string dnfT = dnfTerm;
	string chars = model;
	string dimensions;
	string term = dnfTerm;
	
	int total = chars.length();
	for (int i = 0; i < total; i = i + 2) {

		char ichar = chars[i];
		int idx = term.find(ichar); // position of current literal in
										// term

		if (i + 1 < total) {

			char jchar = chars[i + 1];
			int jdx = term.find(jchar); // position of next literal in
											// term

			if (idx > -1) {
				if (jdx > -1) {
					// all in
					if (idx > 0 && term[idx - 1] == '!') { // whether
																	// a
																	// negation

						if (jdx > 0 && term[jdx - 1] == '!') {
							dimensions.append("00\n");
						} else {
							dimensions.append("01\n");
						}
					} else {

						if (jdx > 0 && term[jdx - 1] == '!') {
							dimensions.append("10\n");
						} else {
							dimensions.append("11\n");
						}
					}

				} else {
					// only ichar in
					if (idx > 0 && term[idx - 1] == '!') { // whether
																	// a
																	// negation
						dimensions.append("00 01\n");
					} else {
						dimensions.append("11 10\n");
					}
				}

			} else {

				if (jdx > -1) {
					// only jchar in
					if (jdx > 0 && term[jdx - 1] == '!') { // whether
																	// a
																	// negation
						dimensions.append("00 10\n");
					} else {
						dimensions.append("01 11\n");
					}

				} else {
					// no one in
					dimensions.append("00 01 11 10\n");
				}
			}

		} else {
			if (idx > -1) {
				if (idx == 0) {
					dimensions.append("1\n");
				} else {
					if (term[idx - 1] == '!') { // whether a negation
						dimensions.append("0\n");
					} else {
						dimensions.append("1\n");
					}
				}
			} else {
				// no one in
				dimensions.append("0 1\n");
			}
		}
	}

	string result(dimensions.c_str());
	return result;
}

string generateCNFdata(string icnf)
{
	string result;

	if(icnf.size()==0)
		return result;

	int literalNum;
	int termNum;
	if(!generateLitNumTermNum(icnf,literalNum,termNum,false))
		return result;

	vector<string> ofp;
	vector<string> ufp;
	if(!generateOFPandUFP(icnf,ofp,ufp))
		return result;

	string exp = icnf;
	Utility uti;
	vector<string> terms=uti.extractCNFstyle(exp);
	string wsModel=uti.generateModel(exp);
	string model;
	model = wsModel;
	string dimensions;//
	for(int i=0;i<terms.size();i++)
	{
		dimensions.append(generateCNFtermDimension(model, terms[i]));
	}

	result.append("%d %d\n",literalNum,termNum);
	for(int i=0;i<ofp.size();i++)
	{
		result.append(ofp[i]);
	}
	result.append("\n");
	for(int i=0;i<ufp.size();i++)
	{
		result.append(ufp[i]);
	}
	result.append("\n");
	result.append(dimensions);

	return result;
}

string generateDNFdata(string idnf)
{
	string result;

	if(idnf.size()==0)
		return result;

	string exp = idnf;

	int literalNum;
	int termNum;
	Utility utility;
	string modelws=utility.generateModel(exp);
	vector<string> terms=utility.extractDNFstyle(exp);
	literalNum=modelws.length();
	termNum=terms.size();

	vector<string> otp;
	vector<string> utp;
	if(!generateOTPandUTP(idnf,otp,utp))
		return result;

	Utility uti;
	string model = modelws;
	string dimensions;//
	for(int i=0;i<terms.size();i++)
	{
		dimensions.append(generateDNFtermDimension(model, terms[i]));
	}

	result.append("%d %d", literalNum, termNum);
	for(int i=0;i<otp.size();i++)
	{
		result.append(otp[i]);
	}
	result.append("\n");
	for(int i=0;i<utp.size();i++)
	{
		result = utp[i];
	}
	result.append("\n");
	result.append(dimensions);

	return result;
}

bool getPreProcessData(string expression, PRE_PROCESS_DATA& preData, bool DNF1CNF0)
{
	if (0 == expression.size())
		return false;

	string exp = expression;

	int literalNum;
	int termNum;
	Utility utility;
	string modelws = utility.generateModel(exp);
	vector<string> terms;
	if (DNF1CNF0)
		terms = utility.extractDNFstyle(exp);
	else
		terms = utility.extractCNFstyle(exp);

	literalNum = modelws.length();
	termNum = terms.size();

	vector<string> op;
	vector<string> op2;
	vector<string> up;
	vector<string> opover2;
	vector<string> fp;
	if (DNF1CNF0){
		if (!generateOTPandUTPandFP(expression, op, op2, up, fp))
			return false;
		//生成两项以上的otps
		if (!generateOTPover2(expression, opover2))
			return false;
	}
	else
	{
		if (!generateOFPandUFP(expression, op, up))
			return false;
	}

	std::vector<std::vector<string>> termsDimension;
	string model = modelws;
	for (int i = 0; i<terms.size(); i++)
	{
		string term = terms[i];
		std::vector<string> dimension;
		if (DNF1CNF0)
			dimension = genDNFtermDimensionVector(model, term);
		else
			dimension = genCNFtermDimensionVector(model, term);

		termsDimension.push_back(dimension);
	}

	preData.literalCount = literalNum;
	preData.termCount = termNum;
	preData.op2 = op2;
	preData.op = op;
	preData.up = up;
	preData.fp = fp;
	preData.opover2 = opover2;
	preData.termsDimension = termsDimension;

	return true;
}


