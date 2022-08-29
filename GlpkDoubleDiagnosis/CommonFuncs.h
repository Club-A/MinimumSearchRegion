#pragma once
/* 2012.3 By 何建杉
* 该类用于存放一些共用的方法
*/
#include <vector>
#include <algorithm>
#include "Cell.h"
#include <set>
using namespace std;

class
	ResourceGuard
{
public :
	void addResource(CCell* ccell_pointer){
		resources_cells.insert(ccell_pointer);

	}
	void addResource(int* int_array){
		resources_int_arrays.insert(int_array);

	}
	~ResourceGuard()
	{
		for ( std::set<CCell*>::iterator it = resources_cells.begin(); it!= resources_cells.end();it++)
		{
			delete (*it);
		}

		for ( std::set<int*>::iterator it = resources_int_arrays.begin(); it!= resources_int_arrays.end();it++)
		{
			delete[] (*it);
		}
	
	}
private:
	std::set<CCell*> resources_cells;
	std::set<int*> resources_int_arrays;
   
};
class CCommonFuncs
{
public:
	CCommonFuncs(void);
	~CCommonFuncs(void);
	//实现n取1
	void chooseOne(vector<int> src, vector<int> &choosed);
	//递归列举出从src中取x个元素的所有可能，通过调用chooseOne函数实现，即C5取2相当于C5取1再C4取1
	void chooseX(vector<int> src, int x, vector<vector<int>> &allChoosed);
	//获取src中被取剩下的元素
	void getRemained(vector<int> src, vector<vector<int>> allChoosed, vector<vector<int>> &allRemained);
	//根据choosed中的列，选取matrix中的子矩阵，矩阵列编号从0开始
	void getSubMatrix(vector<vector<int>> matrix, vector<int> choosed, vector<vector<int>> &subMatrix);
	//将row转换为string
	string RowTostring(int*row, int num);
	//用split分割str
	vector<string> split(string str, string split);

	//递归求DimensionValue中表示的所有cell
	void ListAllCells(int k, vector<vector<int>> DimensionValue, vector<CCell*>& cellList);

	//获取每一维的可能取值，以及tModel中每一维的最大可能值（一般情况下每一维的最大可能值为4，如果变量个数为奇数vOdd=true则最后一维的最大可能值为2）
	void getDimensionValue(string dimensionValuesStr, bool vOdd, vector<vector<int>>& dimensionValues, 
		vector<int>& biggestDimensionValueAllowed);

	//将value的值限定在tModel的取值范围内，即1,2,3,4中的一个(1和4是连在一起的)
	int boundValue(int value, int biggestValue);

	//将dimensionValue中的第i维变为ithTempDimensionValue，得到的新值保存到 tempDimensionValue中
	void getTempDimensionValue(vector<vector<int>> dimensionValue, int i, vector<int> ithTempDimensionValue, 
		vector<vector<int>>& tempDimensionValue);

	//获取matrixCells中出现的所有tModelIndex
	void getAlltModelIndexAppearedInMatrix(vector<vector<CCell*>> matrixCells, 
		vector<int>& tModelIndexAppeared);

	//根据所有的列（即tModelIndexAppeared）,将matrixCells转换为系数矩阵
	void getCoefficientMatrix(vector<int> tModelIndexAppeared, vector<vector<CCell*>> matrixCells, 
		vector<int*>& coefficientMatrix);

	//根据所有的列（即tModelIndexAppeared）,将matrixCells转换为系数矩阵
	void getCoefficientMatrix(vector<int> tModelIndexAppeared, vector<vector<CCell*>> matrixCells, 
		vector<vector<int>>& coefficientMatrix);

	//将一维值的01表示法转换为1,2,3,4的表示法，如11用3表示
	void transDimensionLineFrom01totModelIndex(string ZeroOneLine, string& tModelIndexLine);
	void transDimensionLineFrom01totModelIndex(string ZeroOneLine, vector<int>& tModelIndexLine);

	//将点的01表示转换为1,2,3,4表示，如点110101表示成322
	int getTModelIndexOf01Str(string ZeroOneStr, bool vOdd);

	//根据choosed中的列，选取matrix中的子矩阵，矩阵列编号从0开始
	void getSubMatrix(vector<int*> matrix, vector<int> choosed, vector<vector<int>> &subMatrix);

	//根据tModelIndexs将cellList中的点去掉
	void kickOutCells(vector<CCell*>& cellList, vector<int> tModelIndexs);
	//根据tModelIndexs将cellList2中的点保存到celllist1中
	void keepCells(vector<CCell*>& cellList1 ,vector<CCell*> cellList2,vector<int> tModelIndexs);

	//根据tModelIndexs将cellList1中的点保存到cellList2中
	void getShrinkOtps(vector<CCell*>& cellList1, vector<CCell*>& cellList2, vector<int> tModelIndexs);
	//取celllist1与cellList2公共的点，保存到cellList3中
	void getOverlapCells(vector<CCell*>& cellList1, vector<CCell*>& cellList2,vector<CCell*>& cellList3);
	//从celllist1删掉与cellList2重复的点
	void kickOutCommonCells(vector<CCell*>& cellList1, vector<CCell*> cellList2);

	//根据tModelIndexs1和tModelIndexs2将cellList中的点去掉
	void kickOutCells(vector<CCell*>& cellList, vector<int> tModelIndexs1, vector<int> tModelIndexs2);

	//将1,2,3,4表示的串转换为01串
	string transTModelIndexStrTo01Str(string tModelIndexStr, bool vOdd);

	//将串表示的矩阵转换为vector表示的矩阵
	void transStrMatrix2VectorMatrix(string strMatrix, vector<vector<int>>& vectorMatrix);

	//将vector表示的矩阵转换为string形式
	void transVectorMatrix2StrMatrix(vector<vector<int>> vectorMatrix, string &strMatrix);

	//判断cell是否在cellList中
	bool isCellInCellList(CCell* pCell, vector<CCell*> cellList);
	//清空celllist
	bool clearCellList(vector<CCell*> &cellList);
	
	static ResourceGuard* rgd ;
};
