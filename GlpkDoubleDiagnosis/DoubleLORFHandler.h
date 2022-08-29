#pragma once
#include "CommonFuncs.h"
using namespace std;
class CDoubleLORFHandler
{
public:
	CDoubleLORFHandler(void);
	~CDoubleLORFHandler(void);

	CCommonFuncs m_commonFuncs;

	string run(
		string dimensionValuesStr, bool vOdd,
		vector<vector<CCell*>>& matrixCells,
		vector<int> allOTPs, vector<int> allUTPs
		);

	string run2(
		string dimensionValuesStr, bool vOdd,
		vector<vector<CCell*>>& matrixCells,
		vector<int> allOTPs, vector<int> allUTPs
		);

	string runWithParameter(
		int param, vector<int>&literalNum, 
		string dimensionValuesStr, bool vOdd, 
		vector<vector<CCell*>>& matrixCells, 
		vector<int> allOTPs, vector<int> allUTPs
		);

	string runWithParameter2(
		int param, vector<int>&literalNum,
		string dimensionValuesStr, bool vOdd,
		vector<vector<CCell*>>& matrixCells,
		vector<int> allOTPs, vector<int> allUTPs
		);
};



