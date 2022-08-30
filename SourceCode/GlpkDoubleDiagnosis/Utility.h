#pragma once
/* 2013.8 By 邓力维
* 该类用于存放一些共用的方法
*/
#include <string>
#include <vector>
#include <sstream>
#include <map>
#include <hash_map>
#include <hash_set>
#include "HierarchyNode.h"
using namespace std;

class Utility
{
public:
	Utility(void);
	~Utility(void);

	void initOneFault(vector<string> &onetype);
	string getLiteralNegation(string literal);
	string sortTerm(string term);
	vector<string> uniqueTermVector(vector<string> termVector, string term);
	bool evaluate(string exp,int testData[],string model);
	bool evaluate(string exp,int testNum);
	bool evaluate(string exp, int testNum, string model);
	bool evaluate(string exp,string testString);
	bool evaluateMutant(string oriExp,string mutant,string testString);
	bool evaluateDNF(string exp,int testData[],string model);
	bool evaluateDNF(string exp,int testNum, string model);
	bool evaluateDNF(string exp,string testString,string model);
	vector<string> extractCNFstyle(string exp);
	vector<string> extractDNFstyle(string exp);
	int getTermNum(vector<string> terms, string t);  //获取项在表达式中的序号
	string reverseexp(string exp); //将exp的每一项都取反
	string reverseexp2(string exp,int start,int end); //将exp的from start to end terms取反
	vector<int> countDNFstyle(string exp);//统计可能发生缺陷的项的集合
	vector<int> mayterms(string exp, string fault, string& may_type); //计算可能发生缺陷的项
	string generateModel(string exp);
	void intToIntArray(int intNum,int intArray[],int arrayLength);
	string intTostring(int intNum,int length);
	bool isCNFtermFalse(string term,int testData,string model);
	bool isCNFtermFalse(string term,int testData[],string model);
	bool isDNFtermTrue(string term,int testData,string model);
	bool isDNFtermTrue(string term,int testData[],string model);
	string sortVariables(string variables);
	string sortDNF(string dnf);
	string sortCNF(string cnf);
	bool isTwoPointsAdjacent(int p1[],int p2[],int len);
	bool isTwoPointsAdjacent(int p1,int p2,int len);
	vector<vector<int>> getConnectedRegion(string exp, bool trueOrFalsePoint);
	vector<int> getTermsOpCount(string exp,bool DNF1CNF0);
	bool isExpressionsEquivalent(string exp1, string exp2);
	bool Utility::checkTestDiffer(string test, string exprOri/*原表达式*/, string expr/*变体*/);
	bool initHierarchyRelation(hash_map<string, HierarchyNode>* hierarchyNodeMap, vector<HierarchyNode>* hierarchyEntry);

	//对表达式进行预处理，将项才分到terms中，vars记录字母是否出现
	void mutantsPreproccess(string exp, vector<string> &terms, vector<vector<string>> &literals_in_terms, bool (&vars)[26]);
	//替换表达式中的两项,exp为原表达式，a,b为原表达式中需要被替换的term的下标
	string replaceDoubleTerms(string exp, int a,int b,string term1,string term2);
	string replaceDoubleTerms(string exp, vector<string> terms, int a, int b, string term1, string term2);

	//在当前term中不存在，但是在其他term中存在的词
	vector<string> getLiteralsInOtherTerms(string term, bool(&vars)[26]);


	int getTermPosition(vector<string> terms, string term);

	bool getBelowNodeByCondition(int condition, string faultType, hash_map<string, HierarchyNode>& hierarchyMap, hash_map<string, HierarchyNode>& nodeSet);

	bool traversal(hash_map<string, HierarchyNode>& nodeSet, HierarchyNode* node, int condition);

	void stringTointArray(string str, int intarr[], int len);
};

