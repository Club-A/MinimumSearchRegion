#include "StdAfx.h"
#include "TOFGenerateCellsForMutants.h"


using namespace std;
//运行TOF算法
string TOFGenerateCellsForMutants(
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

	commonFuncs.getDimensionValue(dimensionValuesStr, vOdd, dimensionValues, biggestDimensionValueAllowed);
	/*printf("%s,%d,dimensionValues Print:\n", __FILE__, __LINE__);
	for (int j = 0; j < dimensionValues.size(); j++)
	{
		for (int k = 0; k<dimensionValues[j].size(); k++)
			printf("%d ", dimensionValues[j][k]);
		printf("\n");
	}
	printf("%s,%d,end dimensionValues Print\r\n", __FILE__, __LINE__);*/
	vector<vector<int>> tempDimensionValues = dimensionValues;
	int dimensionNum = dimensionValues.size();
	vector<CCell*> TPCell;
	//取所有UTP
	for(int i=0; i<dimensionNum; i++)
	{
		vector<int> TPDimensionValue;
		vector<vector<int>> tempTPDimensionValue;
		int ithDimensionNum = dimensionValues.at(i).size();
		for(int j=0; j<ithDimensionNum; j++)
		{
			TPDimensionValue.push_back(dimensionValues.at(i).at(j));
		}
		/*printf("\n%s,%d,TPDimensionValue Print:\n", __FILE__, __LINE__);
		for (int j = 0; j < TPDimensionValue.size(); j++)
		{
			printf("%d ", TPDimensionValue[j]);
		}
		printf("\n%s,%d,end TPDimensionValue Print\r\n", __FILE__, __LINE__);*/
		commonFuncs.getTempDimensionValue(tempDimensionValues, i, TPDimensionValue, tempTPDimensionValue);
		/*printf("%s,%d,tempTPDimensionValue Print:\n", __FILE__, __LINE__);
		for (int j = 0; j < tempTPDimensionValue.size(); j++)
		{
			for (int k = 0; k<tempTPDimensionValue[j].size(); k++)
				printf("%d ", tempTPDimensionValue[j][k]);
			printf("\n");
		}
		printf("%s,%d,end tempTPDimensionValue Print\r\n", __FILE__, __LINE__);*/
		tempDimensionValues = tempTPDimensionValue;//下一次循环使用吗？
	}
	commonFuncs.ListAllCells(0, tempDimensionValues, TPCell);
	commonFuncs.kickOutCells(TPCell, allOTPs);
	/*printf("%s,%d,TPCell Print:\n", __FILE__, __LINE__);
	for (int j = 0; j < TPCell.size(); j++)
	{
		printf("%d ", TPCell[j]->getTModelIndex());
	}
	printf("\n%s,%d,end TPCell Print\r\n", __FILE__, __LINE__);*/
	matrixCells.push_back(TPCell);
	/*printf("%s,%d,matrixCells Print:\n", __FILE__, __LINE__);
	for (int j = 0; j < matrixCells.size(); j++)
	{
		for (int k = 0; k<matrixCells[j].size(); k++)
			printf("%d ", matrixCells[j][k]->getTModelIndex());
		printf("\n");
	}
	printf("%s,%d,end matrixCells Print\r\n", __FILE__, __LINE__);*/
	vector<vector<int>> remained_dims_values;
	vector<int> temp_dim_all_values4, temp_dim_all_values2;
	// dimension value : 1 2 3 4
	for (int __k = 0; __k < 4; __k++)
	{
		temp_dim_all_values4.push_back(__k + 1);
	}
	/*printf("%s,%d,temp_dim_all_values4 Print:\n", __FILE__, __LINE__);
	for (int j = 0; j < temp_dim_all_values4.size(); j++)
	{
		printf("%d ", temp_dim_all_values4[j]);
	}
	printf("\n%s,%d,end temp_dim_all_values4 Print\r\n", __FILE__, __LINE__);*/
	// dimension value:  1 2 ,necessary when vOdd == true
	temp_dim_all_values2.push_back(1);
	temp_dim_all_values2.push_back(2);
	/*printf("%s,%d,temp_dim_all_values2 Print:\n", __FILE__, __LINE__);
	for (int j = 0; j < temp_dim_all_values2.size(); j++)
	{
		printf("%d ", temp_dim_all_values2[j]);
	}
	printf("\n%s,%d,end temp_dim_all_values2 Print\r\n", __FILE__, __LINE__);*/
	for (int __i = 0; __i < dimensionNum; __i++)
	{
		if (__i == dimensionNum - 1 && vOdd)remained_dims_values.push_back(temp_dim_all_values2);
		else
		{
			remained_dims_values.push_back(temp_dim_all_values4);
		}
	}

	/*printf("%s,%d,remained_dims_values Print:\n", __FILE__, __LINE__);
	for (int j = 0; j < remained_dims_values.size(); j++)
	{
		for (int k = 0; k<remained_dims_values[j].size(); k++)
			printf("%d ", remained_dims_values[j][k]);
		printf("\n");
	}
	printf("%s,%d,end remained_dims_values Print\r\n", __FILE__, __LINE__);*/

	mutantsDimensionValue.push_back(remained_dims_values);
	corresponding_tests.push_back(TPCell);

	//显示系数矩阵
	return "";
}