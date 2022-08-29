#pragma once
#include "stdafx.h"
#include "LIFGenerateCellsForMutants.h"
using namespace std;
string LIFGenerateCellsForMutants(
	CCommonFuncs commonFuncs,
	string dimensionValuesStr,
	bool vOdd,
	vector<vector<CCell*>>& matrixCells,
	vector<vector<vector<int>>>& mutantsDimensionValue,
	vector<vector<CCell*>>& corresponding_tests,
	vector<int> allOTPs)
{
	vector<vector<int>> dimensionValues;
	vector<int> biggestDimensionValueAllowed;
	//获取各维值
	commonFuncs.getDimensionValue(dimensionValuesStr, vOdd, dimensionValues, biggestDimensionValueAllowed);
	int dimensionNum = dimensionValues.size();
	vector<vector<CCell*>> shrinkCells;
	//对每一维进行缩小，并取缩小部分的点
	for (int i = 0; i<dimensionNum; i++)
	{
		vector<CCell*> shrinkCell;
		vector<vector<int>> tempShrinkDimensionValue;
		vector<int> shrinkDimensionValue;
		int ithDimensionNum = dimensionValues.at(i).size();
		switch (ithDimensionNum)
		{
		case 4: //此种情况缩减为2，有1,2；2,3；3,4和4,1四种情况
			for (int m = 1; m<5; m++)
			{
				tempShrinkDimensionValue.clear();
				shrinkCell.clear();
				shrinkDimensionValue.clear();
				shrinkDimensionValue.push_back(commonFuncs.boundValue(m, 4));
				shrinkDimensionValue.push_back(commonFuncs.boundValue(m + 1, 4));
				//tempShrinkDimensionValue  : 蕴含当前变体的缺陷格集合
				commonFuncs.getTempDimensionValue(dimensionValues, i, shrinkDimensionValue, tempShrinkDimensionValue);
				commonFuncs.ListAllCells(0, tempShrinkDimensionValue, shrinkCell);
				commonFuncs.kickOutCells(shrinkCell, allOTPs);
				if (shrinkCell.size()>0){
					shrinkCells.push_back(shrinkCell);
				}
				// 当前的mutant,(用一个项来表示，这个项各个维的取值存在mutantsDimensionValue中)
				shrinkDimensionValue.clear();
				shrinkDimensionValue.push_back(commonFuncs.boundValue(m + 2, 4));
				shrinkDimensionValue.push_back(commonFuncs.boundValue(m + 3, 4));
				vector<vector<int>> tempMutantDimensionValue;
				commonFuncs.getTempDimensionValue(dimensionValues, i, shrinkDimensionValue, tempMutantDimensionValue);
				mutantsDimensionValue.push_back(tempMutantDimensionValue);

				//把当前mutant对应的测试用例集收集到corresponding_tests中
				corresponding_tests.push_back(shrinkCell);
			}
			break;
		case 2: //此种情况缩减为1
			for (int m = 0; m<2; m++)
			{
				tempShrinkDimensionValue.clear();
				shrinkCell.clear();
				shrinkDimensionValue.clear();
				shrinkDimensionValue.push_back(commonFuncs.boundValue(dimensionValues.at(i).at(m), 4));
				commonFuncs.getTempDimensionValue(dimensionValues, i, shrinkDimensionValue, tempShrinkDimensionValue);
				commonFuncs.ListAllCells(0, tempShrinkDimensionValue, shrinkCell);
				commonFuncs.kickOutCells(shrinkCell, allOTPs);//Shrink的点不能是OTP
				if (shrinkCell.size()>0){
					shrinkCells.push_back(shrinkCell);
				}

				// 当前的mutant,(用一个项来表示，这个项各个维的取值存在mutantsDimensionValue中)
				shrinkDimensionValue.clear();
				shrinkDimensionValue.push_back(commonFuncs.boundValue(dimensionValues.at(i).at((m + 1) % 2), 4));
				vector<vector<int>> tempMutantDimensionValue;
				commonFuncs.getTempDimensionValue(dimensionValues, i, shrinkDimensionValue, tempMutantDimensionValue);
				mutantsDimensionValue.push_back(tempMutantDimensionValue);

				//把当前mutant对应的测试用例集收集到corresponding_tests中
				corresponding_tests.push_back(shrinkCell);
			}
			break;
		case 1://此种情况不能再进行缩减
			break;
		}
	}

	//已经获取所有的shrink cells，将所有可能的shrink组成一个矩阵，一行表示一种shrink
	for (int i = 0; i<shrinkCells.size(); i++)
	{
		vector<CCell*> matrixRow;
		for (int j = 0; j<shrinkCells.at(i).size(); j++)
			matrixRow.push_back(shrinkCells.at(i).at(j));
		matrixCells.push_back(matrixRow);
	}

	return "";
}

