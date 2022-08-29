#include "CommonFuncs.h"

string TOFGenerateCells(CCommonFuncs commonFuncs, string dimensionValuesStr, bool vOdd, vector<vector<CCell*>>& matrixCells, vector<int> allOTPs);
string LOFGenerateCells(CCommonFuncs commonFuncs, string dimensionValuesStr, bool vOdd, vector<vector<CCell*>>& matrixCells, vector<int> allOTPs, vector<int> allUTPs);
string LRFrunWithParameter(CCommonFuncs commonFuncs, int param, vector<int>&literalNum, string dimensionValuesStr, bool vOdd, vector<vector<CCell*>>& matrixCells, vector<int> allOTPs, vector<int> allUTPs);
string LRFGenerateCells(
	CCommonFuncs commonFuncs,
	string dimensionValuesStr,
	bool vOdd,
	vector<vector<CCell*>>& matrixCells,
	vector<int> allOTPs,
	vector<int> allUTPs
	);
string LIORFrunWithParameter(CCommonFuncs commonFuncs, bool separateNeed, vector<int>&literalNum, vector<vector<CCell*>>& lifCells, vector<vector<CCell*>>& lofCells, vector<vector<CCell*>>& lrfCells, string dimensionValuesStr, bool vOdd, vector<vector<CCell*>>& matrixCells, vector<int> allOTPs, vector<int> allUTPs, bool isSingleLitral);
string LIORFGenerateCells(CCommonFuncs commonFuncs, string dimensionValuesStr, bool vOdd, vector<int> allOTPs, vector<int> allUTPs, bool isSingleLitral, vector<vector<CCell*>>& matrixCells);
string LIORFGenerateCells(CCommonFuncs commonFuncs, string dimensionValuesStr, bool vOdd, vector<int> allOTPs, vector<int> allUTPs, bool isSingleLitral, vector<vector<CCell*>>& matrixCells,
	vector<vector<CCell*>> &lifCells,
	vector<vector<CCell*>> &lofCells,
	vector<vector<CCell*>> &lrfCells
	);