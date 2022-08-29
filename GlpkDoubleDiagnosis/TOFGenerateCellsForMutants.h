#pragma once
#include <vector>
#include "CommonFuncs.h"
string TOFGenerateCellsForMutants(
	CCommonFuncs commonFuncs,
	string dimensionValuesStr,
	bool vOdd, 
	vector<vector<CCell*>>& matrixCells, 
	vector<vector<vector<int>>>& mutantsDimensionValue,
	vector<vector<CCell*>>& corresponding_tests, 
	vector<int> allOTPs);