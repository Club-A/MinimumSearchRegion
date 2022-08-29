#include "stdafx.h"
#include "BDD.h"
#include "Utility.h"

int BDD::_instanceCount=0;//记录实例的数量

BDD::BDD(string expression)
	:m_expression(expression)
	,m_BDD(biddy_null)
{
	if(0==_instanceCount){//Biddy should init only one time
		Biddy_About();
		Biddy_Init();
	}
	_instanceCount++;
	generateBDD();

}

BDD::BDD(BDD& bdd)
{
	_instanceCount++;
	*this=bdd;
}

BDD::~BDD(void)
{
	_instanceCount--;

	if (0==_instanceCount)
	{
		Biddy_Exit();
	}
	
}

int findPosition(string model, TCHAR c)
{
	for (int i = 0; i < model.length(); i++)
	{
		if(c==model[i])
			return i;
	}

	return -1;
}

Biddy_Edge generateBiddy(string expression,string model,std::vector<Biddy_Edge> variables)
{
	Utility uti;
	Biddy_Edge dnfEdge=biddy_null;
	std::vector<string> dnfTerms=uti.extractDNFstyle(expression);
	for (int i = 0; i < dnfTerms.size(); i++)
	{
		Biddy_Edge cnfEdge=biddy_null;
		std::vector<string> cnfTerms=uti.extractCNFstyle(dnfTerms[i]);
		for (int j = 0; j < cnfTerms.size(); j++)
		{
			Biddy_Edge edge;
			if ('('==cnfTerms[j][0])
			{
				edge=generateBiddy(cnfTerms[j].substr(1,cnfTerms[j].length()-2),model,variables);
			}
			else if ('!'==cnfTerms[j][0]&&'('==cnfTerms[j][1])
			{
				edge=Biddy_NOT(generateBiddy(cnfTerms[j].substr(2,cnfTerms[j].length()-3),model,variables));
			}
			else if ('!'==cnfTerms[j][0])
			{
				edge=Biddy_NOT(variables[findPosition(model,cnfTerms[j][1])]);
			}else
			{
				edge=variables[findPosition(model,cnfTerms[j][0])];
			}

			if (biddy_null==cnfEdge)
				cnfEdge=edge;
			else
				cnfEdge=Biddy_ITE(cnfEdge,edge,biddy_zero);
		}

		if (biddy_null==dnfEdge)
			dnfEdge=cnfEdge;
		else
			dnfEdge=Biddy_ITE(dnfEdge,biddy_one,cnfEdge);
	}

	return dnfEdge;
}

int BDD::generateBDD()
{	
	string exp = m_expression;
	Utility uti;
	string variables=uti.generateModel(exp);
	m_variables = variables;

	m_variableNodes.clear();
	for (int i = 0; i < variables.size(); i++)
	{
		char v[2]={m_variables[i],'\0'};
		Biddy_Edge edge=Biddy_FoaTerminal(v);
		m_variableNodes.push_back(edge);
	}

	m_BDD=generateBiddy(exp,variables,m_variableNodes);

	return 0;
}

std::vector<int> testStrtoIntArray(string testStr)
{
	std::vector<int> result;

	for (int i = 0; i < testStr.size(); i++)
	{
		TCHAR c=testStr[i];
		if('0'==c)
			result.push_back(0);
		else
			result.push_back(1);
	}

	return result;
}

//invalid return value -1 represents that cannot evaluate with such testcase
int BDD::evaluate(string testStr)
{
	std::vector<int> testIntArr=testStrtoIntArray(testStr);
	return evaluate(testIntArr);
}

//invalid return value -1 represents that cannot evaluate with such testcase
int BDD::evaluate(int testInt)
{
	int arrayLength=m_variableNodes.size();
	std::vector<int> testIntArr;

	unsigned int base=1;
	for(int i=0;i<arrayLength&&i<sizeof(int)*8;i++)
	{
		int bit=base<<i;
		if((testInt&bit)!=0)
			testIntArr.push_back(1);
		else
			testIntArr.push_back(0);
	}

	return evaluate(testIntArr);
}

//invalid return value -1 represents that cannot evaluate with such testcase
int BDD::evaluate(std::vector<int> testIntArr)
{
	if(testIntArr.size()!=m_variableNodes.size()||m_variableNodes.size()<1)
		return -1;

	Biddy_Edge result=Biddy_Restrict(m_BDD,Biddy_GetVariable(m_variableNodes[0]),testIntArr[0]);
	for (int i = 1; i < testIntArr.size(); i++)
	{
		result=Biddy_Restrict(result,Biddy_GetVariable(m_variableNodes[i]),testIntArr[i]);
	}

	if(Biddy_isEqv(result,biddy_one))
		return 1;
	else if(Biddy_isEqv(result,biddy_zero))
		return 0;

	return -1;
}

//invalid return value -1 represents that cannot evaluate with such testcase
int BDD::evaluateAsMutant(string testStr, string oriExpVars)
{
	std::vector<int> testIntArr=testStrtoIntArray(testStr);
	return evaluateAsMutant(testIntArr, oriExpVars);
}

//invalid return value -1 represents that cannot evaluate with such testcase
int BDD::evaluateAsMutant(int testInt, string oriExpVars)
{
	int arrayLength=oriExpVars.size();
	std::vector<int> testIntArr;

	unsigned int base=1;
	for(int i=0;i<arrayLength&&i<sizeof(int)*8;i++)
	{
		int bit=base<<i;
		if((testInt&bit)!=0)
			testIntArr.push_back(1);
		else
			testIntArr.push_back(0);
	}

	return evaluateAsMutant(testIntArr, oriExpVars);
}

//invalid return value -1 represents that cannot evaluate with such testcase
int BDD::evaluateAsMutant(std::vector<int> testIntArr, string oriExpVars)
{
	if(m_variables.size()==oriExpVars.size())
		return evaluate(testIntArr);

	std::vector<int> testIntArrMutant;
	int pMutantVariables=0;
	for (int i = 0; i < oriExpVars.size(); i++)
	{
		TCHAR oriChar=oriExpVars[i];
		TCHAR mutChar=m_variables[pMutantVariables];
		if(oriChar==mutChar){
			testIntArrMutant.push_back(testIntArr[i]);
			pMutantVariables++;
		}
	}

	return evaluate(testIntArrMutant);
}

bool BDD::isEquivalent(BDD& bdd)
{
	if (Biddy_isEqv(m_BDD,bdd.m_BDD))
		return true;

	return false;
}

string  BDD::getPrintString()
{
	string result;


	
	return result;
}





#ifdef DEBUG_BDD
//*************//unit test

BDD_UnitTest::BDD_UnitTest()
{
}

BDD_UnitTest::~BDD_UnitTest()
{
}

void BDD_UnitTest::run()
{
	//testEvaluate();
	//testEvaluateMutant();
	testIsEquivalent();
}

void BDD_UnitTest::testEvaluate()
{
	BDD bdd(_T("ab+cd"));
	for (int i = 0; i < pow(2,4); i++)
	{
		Utility uti;
		string ts=uti.intTostring(i,4);
		string s(ts.c_str());
		if(bdd.evaluate(i))
			s.AppendFormat(_T(" true\n"));
		else
			s.AppendFormat(_T(" false\n"));

		OutputDebugString(s);
	}
}

void BDD_UnitTest::testEvaluateMutant()
{
	BDD bdd(_T("ab+cd"));
	BDD bddMut(_T("adb"));
	/*for (int i = 0; i < pow(2,4); i++)
	{
		Utility uti;
		string ts=uti.intTostring(i,4);
		string s(ts.c_str());
		if(bdd.evaluate(i))
			s.AppendFormat(_T("bdd true\n"));
		else
			s.AppendFormat(_T("bdd false\n"));

		if(bddMut.evaluateAsMutant(i,bdd.getVariables()))
			s.AppendFormat(_T("bddmut true\n"));
		else
			s.AppendFormat(_T("bddmut false\n"));

		OutputDebugString(s);
	}*/

	string s;
	if(bddMut.evaluateAsMutant(_T("1101"),bdd.getVariables()))
		s.AppendFormat(_T("bddmut true\n"));
	else
		s.AppendFormat(_T("bddmut false\n"));

	OutputDebugString(s);
}

void BDD_UnitTest::testIsEquivalent()
{
	string	exp3(_T("～a～bc～g～i～k～m+～a～bcg～h～l～m+～a～bc～g～hi～m+～a～bcgi～l～m+～a～bcgi～k～m+～a～bc～h～k～m+～ab～c～g～i～k+a～b～c～g～i～k+～a～bc～i～kf+～ab～c～g～hi+～ab～cg～h～l+a～b～c～g～hi+a～b～cg～h～l+～a～bc～hif+～ab～cgi～k+～ab～cgi～l+a～b～cgi～k+a～b～cgi～l+a～b～c～h～k+～ab～c～h～k+a～b～cgf+～ab～cgf+～a～bcgf+a～b～c～d+a～b～c～e"));
	TCHAR con=exp3[0];
	for (int i = 0; i < exp3.GetLength(); i++)
	{
		TCHAR ch=exp3[i];
		if(' '==ch){
			string s;
			s.Format(_T("wrong at %d"),i);
			OutputDebugString(s);
		}
		if(con==ch)
			exp3.SetAt(i,'!');
	}
	OutputDebugString(exp3);
	OutputDebugString(_T("\n"));

	//origin
	BDD bdd1(_T("(a(!d+!c+de!(!fgh!i+!ghi)!(!fglk+!g!ik))+!(!fgh!i+!ghi)!(!fglk+!g!ik)(b+c!m+f))(a!b!c+!ab!c+!a!bc)"));
	//minimal mumcut
	BDD bdd2(_T("!a!bc!g!i!k!m+!a!bcg!h!l!m+!a!bc!g!hi!m+!a!bcgi!l!m+!a!bcgi!k!m+!a!bc!h!k!m+!ab!c!g!i!k+a!b!c!g!i!k+!a!bc!i!kf+!ab!c!g!hi+!ab!cg!h!l+a!b!c!g!hi+a!b!cg!h!l+!a!bc!hif+!ab!cgi!k!ab!cgi!l+a!b!cgi!k+a!b!cgi!l+a!b!c!h!k+!ab!c!h!k+a!b!cgf+!ab!cgf+!a!bcgf+a!b!c!d+a!b!c!e"));
	//minimal mumcut
	BDD bdd3(exp3);
	//tmodel
	BDD bdd4(_T("a!b!c+!ab!c!h!k+!a!bc!h!k!m+!ab!c!g!i!k+!a!bc!g!i!k!m+!ab!cfg+!a!bcfg+!ab!cgi!k+!a!bcgi!k!m+!ab!cg!h!l+!a!bcg!h!l!m+!ab!c!g!hi+!a!bc!g!hi!m+!ab!cgi!l+!a!bcgi!l!m+!a!bcf!i!k+!a!bcf!hi"));
	if(bdd1.isEquivalent(bdd2))
		OutputDebugString(_T("1 2 yes\n"));
	else
		OutputDebugString(_T("1 2 no\n"));

	if(bdd1.isEquivalent(bdd3))
		OutputDebugString(_T("1 3 yes\n"));
	else
		OutputDebugString(_T("1 3 no\n"));

	if(bdd1.isEquivalent(bdd4))
		OutputDebugString(_T("1 4 yes\n"));
	else
		OutputDebugString(_T("1 4 no\n"));
}


#endif
