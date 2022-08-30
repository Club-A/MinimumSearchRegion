#pragma once
#include "CommonFuncs.h"
using namespace std;
class CLRFHandler
{
public:
	CLRFHandler(void);
	~CLRFHandler(void);

	CCommonFuncs m_commonFuncs;

	//‘À––LRFÀ„∑®
	string run(string dimensionValuesStr, bool vOdd, vector<vector<CCell*>>& matrixCells, bool mutantsNeed,
	vector<vector<vector<int>>>& mutantsDimensionValue,
	vector<vector<CCell*>>& corresponding_tests, vector<int> allOTPs, vector<int> allUTPs);
	string run(
		string dimensionValuesStr,
	bool vOdd,
	vector<vector<CCell*>>& matrixCells,
	vector<int> allOTPs, 
	vector<int> allUTPs
	);

	string runWithParameter(int param, vector<int>&literalNum, string dimensionValuesStr, bool vOdd, vector<vector<CCell*>>& matrixCells, vector<int> allOTPs, vector<int> allUTPs);
	string runWithParameter(int param, vector<int>&literalNum, string dimensionValuesStr, bool vOdd, vector<vector<CCell*>>& matrixCells, bool mutantsNeed,
	vector<vector<vector<int>>>& mutantsDimensionValue,
	vector<vector<CCell*>>& corresponding_tests, vector<int> allOTPs, vector<int> allUTPs);
};

