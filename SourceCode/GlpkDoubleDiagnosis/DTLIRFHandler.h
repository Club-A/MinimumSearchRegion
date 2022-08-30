#pragma once
#include "CommonFuncs.h"
using namespace std;

class CDTLIRFHandler
{
public:
	CDTLIRFHandler(void);
	~CDTLIRFHandler(void);

	CCommonFuncs m_commonFuncs;

	//运行LIFxLRF算法
	string run(string dimensionValuesStr1, string dimensionValuesStr2,
		bool vOdd, 
		vector<vector<CCell*>>& matrixCells, 
		vector<int> allOTPs,vector<int> allUTPs
		,bool &hasEqualLRF );
	string run2(string dimensionValuesStr1, string dimensionValuesStr2,
		bool vOdd,
		vector<vector<CCell*>>& matrixCells,
		vector<int> allOTPs, vector<int> allUTPs
		, bool &hasEqualLRF);
	//显示系数矩阵
	string getShowStr(vector<vector<CCell*>> matrixCells);
};

