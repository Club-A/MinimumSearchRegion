#pragma once
#include<vector>
using namespace std;

class MatrixPartition
{
public:
	MatrixPartition(void);
	~MatrixPartition(void);

	bool isRowsColumnCross(int row1, int row2, const vector<vector<int>>& matrix);
	vector<vector<int>> findOutCrossRows(const vector<vector<int>>& matrix);
	vector<vector<int>> partitionMatrixWithRows(int blockNum, vector<vector<int>> rows);
	void generateSubMatrix(const vector<vector<int>>&matrix, vector<int> colIndex, vector<int> rows, vector<vector<int>>&subMatrix, vector<int>&subColIndex);
	void getSubMatrixs(vector<vector<int>>&matrix, vector<int>colIndex, vector<vector<int>>&rows, vector<vector<vector<int>>>&subMatrixs, vector<vector<int>>&subColIndexs);
	void genSubMatrixs(vector<vector<int>>&matrix,vector<vector<vector<int>>>&subMatrixs, vector<vector<int>>&partitionRows,vector<vector<int>>&subColIndexs);

};

