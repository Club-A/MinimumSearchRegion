#include "stdafx.h"
#include "MatrixPartition.h"
#include <list>
#include <cassert>

MatrixPartition::MatrixPartition(void)
{
}


MatrixPartition::~MatrixPartition(void)
{
}


bool MatrixPartition::isRowsColumnCross(int row1, int row2, const vector<vector<int>>& matrix)
{
	if(matrix.size()<row1||matrix.size()<row2)
		return false;
#ifdef DEBUG
	assert(matrix[row1].size()==matrix[row2].size());  
#endif // DEBUG

	int size_row = matrix[row1].size();
	for(int i=0;i < size_row;i++)
	{
		
		// more fast check : 
		if(matrix[row1][i]&matrix[row2][i]) return true;
	}

	return false;
}

/*
 * Input ：原始约束矩阵
 * Output: 每个分块后的行标号
 * 
 *
 */

vector<vector<int>>  MatrixPartition::findOutCrossRows(const vector<vector<int>>& matrix)
{
	list<vector<int>> crossRows;

	for(int i=0;i<matrix.size();i++)
	{
		vector<list<vector<int>>::iterator> intersectRows;
		for(list<vector<int>>::iterator it=crossRows.begin();it!=crossRows.end();it++)
		{
			vector<int> rows=*it;
			bool isCross=false;
			for(int j=0;j<rows.size();j++)
			{
				if(isRowsColumnCross(i,rows[j],matrix)){
					isCross=true;
					break;
				}
			}//for
			if(isCross)
				intersectRows.push_back(it);
		}//for
		if(intersectRows.size()==0){
			vector<int> row(1,i);
			crossRows.push_back(row);
		}else if(intersectRows.size()==1){
			list<vector<int>>::iterator it=intersectRows.back();
			it->push_back(i);
		}else{
			vector<int> row(*intersectRows[0]);
			crossRows.erase(intersectRows[0]);
			for(int j=1;j<intersectRows.size();j++)
			{
				vector<int> a=*intersectRows[j];
				for(int k=0;k<a.size();k++)
					row.push_back(a[k]);
				crossRows.erase(intersectRows[j]);
			}//for
			row.push_back(i);
			crossRows.push_back(row);
		}//else
	}//for

	vector<vector<int>>result;
	for(list<vector<int>>::iterator it=crossRows.begin();it!=crossRows.end();it++)
		result.push_back(*it);

	void insertionSort(vector<vector<int>>& rows);
	insertionSort(result);
	return result;
}
void insertionSort(vector<vector<int>>& rows)
{
	for(int i=0;i<rows.size();i++)
	{
		for(int j=i;j>0;j--)
		{
			if(rows[j].size()<=rows[j-1].size())
				break;
			vector<int> t=rows[j];
			rows[j]=rows[j-1];
			rows[j-1]=t;			
		}
	}
}
void joinVector(vector<int>&a, const vector<int>&b)
{
	if(b.size()==0)
		return ;
	for(vector<int>::const_iterator it=b.cbegin();it!=b.cend();it++)
	{
		a.push_back(*it);
	}
}
vector<vector<int>> MatrixPartition::partitionMatrixWithRows(int blockNum, vector<vector<int>> rows)
{
	vector<vector<int>> result;
	if((blockNum<1)||(blockNum>rows.size()))
		return result;
	if(blockNum==rows.size())
		return rows;

	insertionSort(rows);

	int p=0; 
	for(;p<blockNum;p++)
	{
		result.push_back(rows[p]);
	}

	while (true)
	{
		vector<vector<int>>::reverse_iterator it=result.rbegin();
		joinVector(*it,rows[p++]);
		if(p>=rows.size()){
			break;
		}
		insertionSort(result);
	}//while

	return result;
}

void MatrixPartition::generateSubMatrix(const vector<vector<int>>&matrix, vector<int> colIndex, vector<int> rows, vector<vector<int>>&subMatrix, vector<int>&subColIndex)
{
	subMatrix.clear();
	for(int i=0;i<rows.size();i++)
	{
		vector<int> row;
		subMatrix.push_back(row);
	}

	int colNum=matrix[0].size();

	for(int i=0;i<colNum;i++)
	{
		bool colAllZero=true;
		for(int j=0;j<rows.size();j++)
		{
			if(matrix[rows[j]][i]!=0){
				colAllZero=false;
				break;
			}
		}//for
		if(!colAllZero){
			for(int j=0;j<rows.size();j++)
				subMatrix[j].push_back(matrix[rows[j]][i]);
			subColIndex.push_back(colIndex[i]);
		}
	}//for
}

void MatrixPartition::getSubMatrixs(vector<vector<int>>&matrix, vector<int>colIndex, vector<vector<int>>&rows, vector<vector<vector<int>>>&subMatrixs, vector<vector<int>>&subColIndexs)
{
	subMatrixs.clear();
	subColIndexs.clear();
	for(int i=0;i<rows.size();i++)
	{
		vector<vector<int>>subMatrix;
		vector<int>subColIndex;
		generateSubMatrix(matrix,colIndex,rows[i],subMatrix,subColIndex);
		subMatrixs.push_back(subMatrix);
		subColIndexs.push_back(subColIndex);
	}
}

void MatrixPartition::genSubMatrixs(vector<vector<int>>&matrix,vector<vector<vector<int>>>&subMatrixs,vector<vector<int>>&partitionRows,vector<vector<int>>&subColIndexs)
{
	subMatrixs.clear();
	subColIndexs.clear();

	vector<vector<int>>crossRows1=findOutCrossRows(matrix);

	int block_count1=crossRows1.size();
	//记录分块的行数
	partitionRows=partitionMatrixWithRows(block_count1,crossRows1);

	int colNum1=matrix.back().size();
	vector<int>colIndex1(colNum1,-1);
	for(int i=0;i<colNum1;i++)
		colIndex1[i] = i;

	//根据行数进行分块
	getSubMatrixs(matrix,colIndex1,partitionRows,subMatrixs,subColIndexs);

}