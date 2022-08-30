#pragma once
#include "CommonFuncs.h"
using namespace std;
class DTLIFhandler
{
public:
	DTLIFhandler(void);
	~DTLIFhandler(void);

	CCommonFuncs m_commonFuncs;

	// 运行LIFxLIF算法生成约束矩阵
	string run(string dimensionValuesStr, vector<vector<CCell*>>& matrixCells, vector<int> allOTPs);
	string run(string dimensionValuesStr, vector<vector<CCell*>>& matrixCells, bool mutantsNeed, vector<vector<vector<int>>>& mutantsDimensionValue, vector<vector<CCell*>>& corresponding_tests, vector<int> allOTPs);
	string run(string dimensionValuesStr1, string dimensionValuesStr2, vector<vector<CCell*>>& matrixCells, vector<int> allOTPs, vector<int> all3OTPs);
	string run2(string dimensionValuesStr1, string dimensionValuesStr2, vector<vector<CCell*>>& matrixCells, vector<int> allOTPs, vector<int> all3OTPs);

	string run(string dimensionValuesStr1, string dimensionValuesStr2,
		vector<vector<CCell*>>& matrixCells,bool mutantsNeed, 
		vector<vector<vector<int>>>& mutantsDimensionValue,vector<vector<CCell*>>& corresponding_tests, 
		vector<int> allOTPs, vector<int> all3OTPs);

};

