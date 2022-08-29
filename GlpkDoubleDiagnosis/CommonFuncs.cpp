#include "StdAfx.h"
#include "CommonFuncs.h"
#include<iostream>


ResourceGuard* CCommonFuncs:: rgd = nullptr ;
CCommonFuncs::CCommonFuncs(void)
{
	if(CCommonFuncs::rgd == nullptr)
	{
		CCommonFuncs::rgd = new ResourceGuard();
	}
}

CCommonFuncs::~CCommonFuncs(void)
{
}

//实现n取1
void CCommonFuncs::chooseOne(vector<int> src, vector<int> &choosed)
{
	for(int i=0; i<src.size(); i++)
	{
		choosed.push_back(src.at(i));
	}
}

//递归列举出从src中取x个元素的所有可能，通过调用chooseOne函数实现，即C5取2相当于C5取1再C4取1
void CCommonFuncs::chooseX(vector<int> src, int x, vector<vector<int>> &allChoosed)
{
	if(x>src.size())
		return;
	if(x==1)
	{
		vector<int> choosed;
		chooseOne(src,choosed);
		for(int i=0; i<choosed.size(); i++)
		{
			vector<int> v;
			v.push_back(choosed.at(i));
			allChoosed.push_back(v);
		}
	}
	else
	{
		vector<vector<int>> allChoosedTemp;
		chooseX(src,x-1,allChoosedTemp);

		vector<vector<int>> allRemainedTemp;
		getRemained(src,allChoosedTemp,allRemainedTemp);

		for(int i=0; i<allRemainedTemp.size(); i++)
		{
			vector<int> choosed;
			chooseOne(allRemainedTemp.at(i),choosed);

			for(int j=0; j<choosed.size(); j++)
			{
				vector<int> tempV;

				//先复制再push
				allChoosed.push_back(allChoosedTemp.at(i));
				allChoosed.at(allChoosed.end()-1-allChoosed.begin()).push_back(choosed.at(j));

				/*for(int k=0; k<allChoosedTemp.at(i).size(); k++)
				{
				tempV.push_back(allChoosedTemp.at(i).at(k);
				}
				tempV.push_back(choosed.at(j);

				allChoosed.push_back(tempV);*/
			}
		}

		//去重，因为是顺序无关的，所以123和213或231等都是一样的
		for(int i=0; i<allChoosed.size(); i++)
		{
			int j=i+1;
			while(j<allChoosed.size())
			{	
				int k=0;
				int temp = allChoosed.at(j).size();//第j行的元素个数
				while(k<temp)
				{
					if(find(allChoosed.at(i).begin(),allChoosed.at(i).end(),allChoosed.at(j).at(k))==allChoosed.at(i).end())
						break;
					k++;
				}
				if(k==temp)//第j行中的元素在第i行中都有，即重复了
					allChoosed.erase(allChoosed.begin()+j);
				else
					j++;
			}
		}
	}
}

//获取src中被取剩下的元素
void CCommonFuncs::getRemained(vector<int> src, vector<vector<int>> allChoosed, vector<vector<int>> &allRemained)
{
	for(int i=0; i<allChoosed.size(); i++)
	{
		vector<int> remained;
		for(int j=0; j<src.size(); j++)
		{
			if(find(allChoosed.at(i).begin(),allChoosed.at(i).end(),src.at(j))!=allChoosed.at(i).end())
				continue;
			remained.push_back(src.at(j));
		}
		allRemained.push_back(remained);
	}
}

//根据choosed中的列，选取matrix中的子矩阵，矩阵列编号从0开始
void CCommonFuncs::getSubMatrix(vector<vector<int>> matrix, vector<int> choosed, vector<vector<int>> &subMatrix)
{
	if(choosed.size()==0)
		return;
	//先加载choosed中的第一列
	int index = choosed.at(0);
	for(int i=0; i<matrix.size(); i++)
	{
		vector<int> row;
		row.push_back(matrix.at(i).at(index));
		subMatrix.push_back(row);
	}

	for(int i=1; i<choosed.size(); i++)
	{
		index = choosed.at(i);
		for(int j=0; j<matrix.size(); j++)
		{
			subMatrix.at(j).push_back(matrix.at(j).at(index));
		}
	}
}

//根据choosed中的列，选取matrix中的子矩阵，矩阵列编号从0开始
void CCommonFuncs::getSubMatrix(vector<int*> matrix, vector<int> choosed, vector<vector<int>> &subMatrix)
{
	if(choosed.size()==0)
		return;
	//先加载choosed中的第一列
	int index = choosed.at(0);
	for(int i=0; i<matrix.size(); i++)
	{
		vector<int> row;
		row.push_back(matrix.at(i)[index]);
		subMatrix.push_back(row);
	}

	for(int i=1; i<choosed.size(); i++)
	{
		index = choosed.at(i);
		for(int j=0; j<matrix.size(); j++)
		{
			subMatrix.at(j).push_back(matrix.at(j)[index]);
		}
	}

}

//将row转换为string
string CCommonFuncs::RowTostring(int*row, int num)
{
	string rowStr="";
	for(int i=0; i<num; i++)
	{
		rowStr.append("%d ", row[i]);
	}
	return rowStr;
}

//将字符串分割
vector<string> CCommonFuncs::split(string str, string pattern)
{
	std::string::size_type pos;
	std::vector<std::string> result;
	str += pattern;//扩展字符串以方便操作  
	int size = str.size();

	for (int i = 0; i<size; i++)
	{
		pos = str.find(pattern, i);
		if (pos<size)
		{
			std::string s = str.substr(i, pos - i);
			result.push_back(s);
			i = pos + pattern.size() - 1;
		}
	}
	return result;
}

//递归求DimensionValue中表示的所有cell
void CCommonFuncs::ListAllCells(int k, vector<vector<int>> DimensionValue, vector<CCell*>& cellList)
{
	int dimension = DimensionValue.size();
	CCell* pCell;
	if(k==dimension-1)
	{
		for(int i=0; i<DimensionValue.at(k).size(); i++)
		{
			pCell = new CCell(dimension);
			CCommonFuncs::rgd->addResource(pCell);
			pCell->value[k] = DimensionValue.at(k).at(i);
			cellList.push_back(pCell);
		}
	}
	else
	{
		ListAllCells(k+1,DimensionValue,cellList);
		if(DimensionValue.at(k).size()>0)
		{
			for(int i=0; i<cellList.size(); i++)
			{
				cellList.at(i)->value[k] = DimensionValue.at(k).at(0);
			}
		}

		int cellNum = cellList.size();
		//cout << "--------------cellsize---------: " << cellList.size() << endl;
		//cout << "--------------cellnum---------: " << cellNum << endl;
		//复制已有cell
		for(int j=0; j<cellNum; j++)
		{
			for(int i=1; i<DimensionValue.at(k).size(); i++)
			{
				pCell = new CCell(dimension);
				CCommonFuncs::rgd->addResource(pCell);
				pCell->Copy(cellList.at(j));
				pCell->value[k] = DimensionValue.at(k).at(i);
				cellList.push_back(pCell);
			}
		}
	}
}

//获取每一维的可能取值，以及tModel中每一维的最大可能值（一般情况下每一维的最大可能值为4，如果变量个数为奇数vOdd=true则最后一维的最大可能值为2）
void CCommonFuncs::getDimensionValue(string dimensionValuesStr, bool vOdd, vector<vector<int>>& dimensionValues, vector<int>& biggestDimensionValueAllowed)
{
	if(dimensionValuesStr.size()==0)
		return;
	//printf("enter getDimensionValue\n");
	vector<string> dimensions = split(dimensionValuesStr,"\n");
	int dimensionNum = dimensions.size();

	for(int i=0; i<dimensionNum; i++)
	{
		vector<int> dimension;
		vector<string> subStrs = split(dimensions[i]," ");
		for(int j=0; j<subStrs.size(); j++)
		{
			int temp = atoi(subStrs.at(j).c_str());
			if(temp>4 || temp<1)
			{
				//MessageBox("每维的可能取值是1到4，您的输入有错误！请重输！");
				if(temp != 0)dimensionValues.clear();
				return;
			}
			//printf("%d ", temp);
			dimension.push_back(temp);
		}
		//printf("\n");
		dimensionValues.push_back(dimension);

		if(vOdd && i==dimensionNum-1)
			biggestDimensionValueAllowed.push_back(2);
		else
			biggestDimensionValueAllowed.push_back(4);
	}
	//printf("out getDimensionValue\n");
}

//将value的值限定在tModel的取值范围内，即1,2,3,4中的一个(1和4是连在一起的)
int CCommonFuncs::boundValue(int value, int biggestValue)
{
	while(value<0)
		value+=100;

	//biggestValue只能是2或4
	int result = value%biggestValue;
	if(result==0)
		return biggestValue;
	else
		return result;
}

//将dimensionValue中的第i维变为ithTempDimensionValue，得到的新值保存到 tempDimensionValue中
void CCommonFuncs::getTempDimensionValue(vector<vector<int>> dimensionValue, int i, vector<int> ithTempDimensionValue, 
	vector<vector<int>>& tempDimensionValue)
{
	int dimensionNum = dimensionValue.size();
	for(int m=0; m<dimensionNum; m++)
	{
		if(m!=i)
			tempDimensionValue.push_back(dimensionValue.at(m));
		else
			tempDimensionValue.push_back(ithTempDimensionValue);
	}
}

//获取matrixCells中出现的所有tModelIndex
void CCommonFuncs::getAlltModelIndexAppearedInMatrix(vector<vector<CCell*>> matrixCells, vector<int>& tModelIndexAppeared)
{
	for(int i=0; i<matrixCells.size(); i++)
	{
		for(int j=0; j<matrixCells.at(i).size(); j++)
		{
			int tModelIndex = matrixCells.at(i).at(j)->getTModelIndex();
			vector<int>::iterator iter = find(tModelIndexAppeared.begin(),tModelIndexAppeared.end(),tModelIndex);
			if(iter==tModelIndexAppeared.end())
				tModelIndexAppeared.push_back(tModelIndex);
		}
	}
}

//根据所有的列（即tModelIndexAppeared）,将matrixCells转换为系数矩阵
void CCommonFuncs::getCoefficientMatrix(vector<int> tModelIndexAppeared, vector<vector<CCell*>> matrixCells, vector<int*>& coefficientMatrix)
{
	for(int i=0; i<matrixCells.size(); i++)
	{
		int *coefficients = new int[tModelIndexAppeared.size()];
		CCommonFuncs::rgd->addResource(coefficients);
		for(int j=0; j<tModelIndexAppeared.size(); j++)//初始化
			coefficients[j]=0;
		for(int j=0; j<matrixCells.at(i).size(); j++)//将对应的系数值改为1
		{
			int tModelIndex = matrixCells.at(i).at(j)->getTModelIndex();
			vector<int>::iterator iter = find(tModelIndexAppeared.begin(),tModelIndexAppeared.end(),tModelIndex);
			int index = iter - tModelIndexAppeared.begin();
			coefficients[index]=1;
		}
		coefficientMatrix.push_back(coefficients);
	}
}

//根据所有的列（即tModelIndexAppeared）,将matrixCells转换为系数矩阵
void CCommonFuncs::getCoefficientMatrix(vector<int> tModelIndexAppeared, vector<vector<CCell*>> matrixCells, vector<vector<int>>& coefficientMatrix)
{
	for(int i=0; i<matrixCells.size(); i++)
	{
		vector<int> coefficients;
		for(int j=0; j<tModelIndexAppeared.size(); j++)//初始化
			coefficients.push_back(0);
		for(int j=0; j<matrixCells[i].size(); j++)//将对应的系数值改为1
		{
			int tModelIndex = matrixCells[i][j]->getTModelIndex();
			vector<int>::iterator iter = find(tModelIndexAppeared.begin(), tModelIndexAppeared.end(), tModelIndex);
			int index = iter - tModelIndexAppeared.begin();
			coefficients[index] = 1;
		}
		coefficientMatrix.push_back(coefficients);
	}
}

//将一维值的01表示法转换为1,2,3,4的表示法，如11用3表示 将每一行（即每一维）的值 转化 例如 "01 11" ->"2 3"
void CCommonFuncs::transDimensionLineFrom01totModelIndex(string ZeroOneLine, string& tModelIndexLine)
{
	vector<string> subStrs = split(ZeroOneLine," ");
	tModelIndexLine = "";
	int num = subStrs.size();
	for(int i=0; i<num; i++)
	{
		string tModelIndex;
		if(subStrs.at(i)=="00")
		{
			tModelIndex = "1";
		}
		else if(subStrs.at(i)=="01")
		{
			tModelIndex = "2";
		}
		else if(subStrs.at(i)=="11")
		{
			tModelIndex = "3";
		}
		else if(subStrs.at(i)=="10")
		{
			tModelIndex = "4";
		}
		else if(subStrs.at(i)=="0")
		{
			tModelIndex = "1";
		}
		else if(subStrs.at(i)=="1")
		{
			tModelIndex = "2";
		}

		tModelIndexLine.append(tModelIndex);
		if(i!=num-1)
			tModelIndexLine.append(" ");
	}
}
void CCommonFuncs::transDimensionLineFrom01totModelIndex(string ZeroOneLine, vector<int>& dimensionTModelIndex)
{
	vector<string> subStrs = split(ZeroOneLine," ");
	dimensionTModelIndex.clear();
	int num = subStrs.size();
	for(int i=0; i<num; i++)
	{
		int tModelIndex=0;
		if(subStrs.at(i)=="00")
		{
			tModelIndex = 1;
		}
		else if(subStrs.at(i)=="01")
		{
			tModelIndex = 2;
		}
		else if(subStrs.at(i)=="11")
		{
			tModelIndex = 3;
		}
		else if(subStrs.at(i)=="10")
		{
			tModelIndex = 4;
		}
		else if(subStrs.at(i)=="0")
		{
			tModelIndex = 1;
		}
		else if(subStrs.at(i)=="1")
		{
			tModelIndex = 2;
		}

		dimensionTModelIndex.push_back(tModelIndex);
	}
}


//将点的01表示转换为1,2,3,4表示，如点110101表示成322 00-1 01-2 11-3 10-4
int CCommonFuncs::getTModelIndexOf01Str(string ZeroOneStr, bool vOdd)
{
	int tModelIndex=0;

	//计算维度数
	int strLength = ZeroOneStr.size();
	int dimensionNum = strLength/2;
	string tempStr;//用于后续的计算
	if(vOdd)
		tempStr = ZeroOneStr.substr(0,strLength-1);
	else
		tempStr = ZeroOneStr;

	//逐维进行转换，从最后一维开始
	string subStr;
	int tempStrLength = tempStr.size();
	for(int i=0; i<dimensionNum; i++)
	{
		subStr = tempStr.substr(tempStrLength-2-i*2,2);
		if(subStr == "00")
			tModelIndex += 1*pow(10.0,i);
		else if(subStr == "01")
			tModelIndex += 2*pow(10.0,i);
		else if(subStr == "11")
			tModelIndex += 3*pow(10.0,i);
		else if(subStr == "10")
			tModelIndex += 4*pow(10.0,i);
	}

	//如果为奇数则补上最后一维
	if(vOdd)
	{
		tModelIndex *= 10;
		subStr = ZeroOneStr.substr(strLength-1,1);
		if(subStr == "0")
			tModelIndex+=1;
		else
			tModelIndex+=2;
	}

	return tModelIndex;
}

//根据tModelIndexs将cellList中的点去掉
void CCommonFuncs::kickOutCells(vector<CCell*>& cellList, vector<int> tModelIndexs)
{
	for(int i=0; i<cellList.size(); i++)
	{
		int tModelIndex = cellList.at(i)->getTModelIndex();
		if(find(tModelIndexs.begin(),tModelIndexs.end(),tModelIndex)!=tModelIndexs.end())//如果在tModelIndexs中则去除
		{
			cellList.erase(cellList.begin()+i);
			i--;
		}
	}
}
//根据tModelIndexs将cellList2中的点保存到celllist1中
void CCommonFuncs::keepCells(vector<CCell*>& cellList1 ,vector<CCell*> cellList2,vector<int> tModelIndexs)
{
	for(int i=0; i<cellList2.size(); i++)
	{
		int tModelIndex = cellList2.at(i)->getTModelIndex();
		if(find(tModelIndexs.begin(),tModelIndexs.end(),tModelIndex)!=tModelIndexs.end())//如果在tModelIndexs中则去除
		{
			cellList1.push_back(cellList2.at(i));
		}
	}
}

//根据tModelIndexs将cellList1中的点保存到cellList2中
void CCommonFuncs::getShrinkOtps(vector<CCell*>& cellList1, vector<CCell*>& cellList2, vector<int> tModelIndexs)
{
	for(int i=0; i<cellList1.size(); i++)
	{
		int tModelIndex = cellList1.at(i)->getTModelIndex();
		if(find(tModelIndexs.begin(),tModelIndexs.end(),tModelIndex)!=tModelIndexs.end())//如果在tModelIndexs中则加入cellList2
		{
			cellList2.push_back(cellList1.at(i));
		}
	}

}

//根据tModelIndexs1和tModelIndexs2将cellList中的点去掉
void CCommonFuncs::kickOutCells(vector<CCell*>& cellList, vector<int> tModelIndexs1, vector<int> tModelIndexs2)
{
	for(int i=0; i<cellList.size(); i++)
	{
		int tModelIndex = cellList.at(i)->getTModelIndex();
		if(find(tModelIndexs1.begin(),tModelIndexs1.end(),tModelIndex)!=tModelIndexs1.end()||
			find(tModelIndexs2.begin(),tModelIndexs2.end(),tModelIndex)!=tModelIndexs2.end())//如果在tModelIndexs1或tModelIndexs2中则去除
		{
			cellList.erase(cellList.begin()+i);
			i--;
		}
	}
}
//取celllist1与cellList2公共的点，保存到cellList3中
void CCommonFuncs::getOverlapCells(vector<CCell*>& cellList1, vector<CCell*>& cellList2,vector<CCell*>& cellList3)
{
	for (int i = 0; i < cellList1.size(); i++)
	{
		for (int j = 0; j  < cellList2.size(); j ++)
		{
			if(cellList1.at(i)->compare(cellList2.at(j)))
			{
				cellList3.push_back(cellList1.at(i));
				break;
			}	
		}
	}
}
//从celllist1删掉与cellList2重复的点
void CCommonFuncs::kickOutCommonCells(vector<CCell*>& cellList1, vector<CCell*> cellList2)
{
	for (int i = 0; i < cellList1.size(); i++)
	{
		for (int j = 0; j  < cellList2.size(); j ++)
		{
			if(cellList1.at(i)->compare(cellList2.at(j)))
			{
				cellList1.erase(cellList1.begin()+i);
				i--;
				break;
			}
			
		}

	}
}

//将1,2,3,4表示的串转换为01串
string CCommonFuncs::transTModelIndexStrTo01Str(string tModelIndexStr, bool vOdd)
{
	string result = "";
	int len = tModelIndexStr.size();
	if(vOdd)
	{
		for(int i=0; i<len-1; i++)
		{
			if(tModelIndexStr.at(i)=='1')
				result.append("00");
			else if(tModelIndexStr.at(i)=='2')
				result.append("01");
			else if(tModelIndexStr.at(i)=='3')
				result.append("11");
			else if(tModelIndexStr.at(i)=='4')
				result.append("10");
		}

		//变量个数为奇数时，最后一维的情况
		int i=len-1;
		if(tModelIndexStr.at(i)=='1')
			result.append("0");
		else if(tModelIndexStr.at(i)=='2')
			result.append("1");
	}
	else
	{
		for(int i=0; i<len; i++)
		{
			if(tModelIndexStr.at(i)=='1')
				result.append("00");
			else if(tModelIndexStr.at(i)=='2')
				result.append("01");
			else if(tModelIndexStr.at(i)=='3')
				result.append("11");
			else if(tModelIndexStr.at(i)=='4')
				result.append("10");
		}
	}
	return result;
}

void CCommonFuncs::transStrMatrix2VectorMatrix(string strMatrix, vector<vector<int>>& vectorMatrix)
{
	vector<string> lines = split(strMatrix,"\n");
	int lineNum = lines.size();
	for(int i=0; i<lineNum; i++)
	{
		vector<int> lineVector;
		vectorMatrix.push_back(lineVector);
	}

	for(int i=0; i<lineNum; i++)
	{
		vector<string> cols = split(lines.at(i)," ");
		for(int j=0; j<cols.size(); j++)
		{
			vectorMatrix.at(i).push_back(atoi(cols.at(j).c_str()));
		}
	}
}

//将vector表示的矩阵转换为string形式
void CCommonFuncs::transVectorMatrix2StrMatrix(vector<vector<int>> vectorMatrix, string &strMatrix)
{
	strMatrix="";
	int lineNum = vectorMatrix.size();
	if(lineNum==0)
		return;
	int colNum = vectorMatrix.at(0).size();
	for(int i=0; i<lineNum; i++)
	{
		for(int j=0; j<colNum; j++)
		{
			string tempStr;
			tempStr.append("%d",vectorMatrix.at(i).at(j));
			strMatrix.append(tempStr);
			if(j==colNum-1)
				strMatrix.append("\r\n");
			else
				strMatrix.append(" ");
		}
	}
}

//判断cell是否在cellList中
bool CCommonFuncs::isCellInCellList(CCell* pCell, vector<CCell*> cellList)
{
	int num = cellList.size();
	for(int i=0; i<num; i++)
	{
		if(pCell->compare(cellList.at(i)))
			return true;
	}
	return false;
}

//清空celllist
bool CCommonFuncs::clearCellList(vector<CCell*> &cellList)
{
	for (vector<CCell*>::iterator it = cellList.begin(); it != cellList.end(); it ++) 
	{
		if (NULL != *it&&(*it)->dimension>0) 
		{
			delete *it; 
			*it = NULL;
		}
	}
	cellList.clear();
	vector<CCell*>().swap(cellList);
	return true;
}