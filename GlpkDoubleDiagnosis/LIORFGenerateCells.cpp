#pragma once
#include "stdafx.h"
#include "LIORFGenerateCells.h"
#include "time.h"
using namespace std;
//运行TOF算法
string TOFGenerateCells(CCommonFuncs commonFuncs, string dimensionValuesStr, bool vOdd, vector<vector<CCell*>>& matrixCells, vector<int> allOTPs)
{
	string showStr = "";
	vector<vector<int>> dimensionValues;
	vector<int> biggestDimensionValueAllowed;

	commonFuncs.getDimensionValue(dimensionValuesStr, vOdd, dimensionValues, biggestDimensionValueAllowed);

	vector<vector<int>> tempDimensionValues = dimensionValues;
	int dimensionNum = dimensionValues.size();
	vector<CCell*> TPCell;
	//取所有UTP
	for (int i = 0; i<dimensionNum; i++)
	{
		vector<int> TPDimensionValue;
		vector<vector<int>> tempTPDimensionValue;
		int ithDimensionNum = dimensionValues.at(i).size();
		for (int j = 0; j<ithDimensionNum; j++)
		{
			TPDimensionValue.push_back(dimensionValues.at(i).at(j));
		}
		commonFuncs.getTempDimensionValue(tempDimensionValues, i, TPDimensionValue, tempTPDimensionValue);
		tempDimensionValues = tempTPDimensionValue;
	}
	commonFuncs.ListAllCells(0, tempDimensionValues, TPCell);
	commonFuncs.kickOutCells(TPCell, allOTPs);

	matrixCells.push_back(TPCell);

	//显示系数矩阵
	//showStr = getShowStr(matrixCells);

	return showStr;
}

//运行LOF算法
string LOFGenerateCells(CCommonFuncs commonFuncs, string dimensionValuesStr, bool vOdd, vector<vector<CCell*>>& matrixCells, vector<int> allOTPs, vector<int> allUTPs)
{
	string showStr = "";
	vector<vector<int>> dimensionValues;
	vector<int> biggestDimensionValueAllowed;

	commonFuncs.getDimensionValue(dimensionValuesStr, vOdd, dimensionValues, biggestDimensionValueAllowed);
	int totalLOFliteralsNum = 0;
	int dimensionNum = dimensionValues.size();
	vector<vector<CCell*>> expandCells;
	//对每一维进行放大一倍，并取放大部分的点
	for (int i = 0; i<dimensionNum; i++)
	{
		vector<CCell*> expandCell;
		vector<vector<int>> tempExpandDimensionValue;
		vector<int> expandDimensionValue;
		int biggestValue = biggestDimensionValueAllowed.at(i);//该维允许的最大值（2或4）
		int ithDimensionNum = dimensionValues.at(i).size();
		switch (ithDimensionNum)
		{
		case 4: //此种情况不能进行扩充
			break;
		case 2: //此种情况扩充为4
			if (biggestValue == 4)//如果等于2则不能扩充
			{
				totalLOFliteralsNum += 1;
				int num1 = dimensionValues.at(i).at(0);
				int num2 = dimensionValues.at(i).at(1);
				if ((num1 == 1 && num2 == 4) || (num1 == 4 && num2 == 1))
				{
					expandDimensionValue.push_back(2);
					expandDimensionValue.push_back(3);
				}
				else
				{
					int minNum = num1<num2 ? num1 : num2;
					expandDimensionValue.push_back(commonFuncs.boundValue(minNum + 2, biggestValue));
					expandDimensionValue.push_back(commonFuncs.boundValue(minNum + 3, biggestValue));
				}
				commonFuncs.getTempDimensionValue(dimensionValues, i, expandDimensionValue, tempExpandDimensionValue);
				commonFuncs.ListAllCells(0, tempExpandDimensionValue, expandCell);
				commonFuncs.kickOutCells(expandCell, allOTPs, allUTPs);//expand的点不能是TP
				if (expandCell.size()>0)
					expandCells.push_back(expandCell);
			}
			break;
		case 1://这种情况下，有两种扩充方式，比如原来为1，扩充部分可以是4或2
			int num = dimensionValues.at(i).at(0);
			bool noPlus1 = false, noMinus1 = false;
			if (biggestValue == 2)//这种情况只能往一边扩充，num的值非1即2
			{
				if (num == 1)
					noMinus1 = true;
				else
					noPlus1 = true;
			}

			if (!noPlus1)
			{
				expandDimensionValue.push_back(commonFuncs.boundValue(num + 1, biggestValue));
				commonFuncs.getTempDimensionValue(dimensionValues, i, expandDimensionValue, tempExpandDimensionValue);
				commonFuncs.ListAllCells(0, tempExpandDimensionValue, expandCell);
				commonFuncs.kickOutCells(expandCell, allOTPs, allUTPs);//expand的点不能是TP
				if (expandCell.size() > 0)
				{
					totalLOFliteralsNum += 1;
					expandCells.push_back(expandCell);
				}
			}

			if (!noMinus1)
			{
				expandDimensionValue.clear();
				tempExpandDimensionValue.clear();
				expandCell.clear();
				expandDimensionValue.push_back(commonFuncs.boundValue(num - 1, biggestValue));
				commonFuncs.getTempDimensionValue(dimensionValues, i, expandDimensionValue, tempExpandDimensionValue);
				commonFuncs.ListAllCells(0, tempExpandDimensionValue, expandCell);
				commonFuncs.kickOutCells(expandCell, allOTPs, allUTPs);//expand的点不能是TP
				if (expandCell.size() > 0)
				{
					expandCells.push_back(expandCell);
					totalLOFliteralsNum += 1;
				}
			}
			break;
		}
	}

	//已经获取所有的expand cells，将所有可能的扩充组成一个矩阵，一行表示一种扩充
	for (int i = 0; i<expandCells.size(); i++)
	{
		vector<CCell*> matrixRow;
		for (int j = 0; j<expandCells.at(i).size(); j++)
			matrixRow.push_back(expandCells.at(i).at(j));
		matrixCells.push_back(matrixRow);
	}

	//显示有效总的Expand数
	string temp;
	temp.append("%d", totalLOFliteralsNum);
	showStr.append(temp);
	return showStr;
}

//运行LRF算法  param为1，只返回validShrink部分  param为2，只返回expand部分 其他值两个都返回
string LRFrunWithParameter(CCommonFuncs commonFuncs, int param, vector<int>&literalNum, string dimensionValuesStr, bool vOdd, vector<vector<CCell*>>& matrixCells, vector<int> allOTPs, vector<int> allUTPs)
{
	string showStr = "";
	vector<vector<int>> dimensionValues;
	vector<int> biggestDimensionValueAllowed;
	int allShrinksCount = 0;
	int validShrinksCount = 0;


	commonFuncs.getDimensionValue(dimensionValuesStr, vOdd, dimensionValues, biggestDimensionValueAllowed);

	int dimensionNum = dimensionValues.size();
	//针对每一维，变为原来的1/2，再选一维扩大为原来的两倍

	for (int i = 0; i<dimensionNum; i++)
	{
		//分别获取剩下的一半和去掉的一半
		vector<vector<int>> remainedDimensionValues;
		vector<vector<int>> removedDimensionValues;
		vector<int> remainedTemp;
		vector<int> removedTemp;

		int ithDimensionValueNum = dimensionValues.at(i).size();
		//列举这一维可能出现的所有情况并记录下来
		switch (ithDimensionValueNum)
		{
		case 4://这一维是1,2,3,4的情况
			for (int m = 1; m<5; m++)
			{
				remainedTemp.clear();
				removedTemp.clear();
				remainedTemp.push_back(commonFuncs.boundValue(m, 4));
				remainedTemp.push_back(commonFuncs.boundValue(m + 1, 4));
				removedTemp.push_back(commonFuncs.boundValue(m + 2, 4));
				removedTemp.push_back(commonFuncs.boundValue(m + 3, 4));
				remainedDimensionValues.push_back(remainedTemp);
				removedDimensionValues.push_back(removedTemp);
			}
			break;
		case 2://这一维是1,2或2,3或3,4或4,1的情况
			remainedTemp.clear();
			removedTemp.clear();
			remainedTemp.push_back(dimensionValues.at(i).at(0));
			removedTemp.push_back(dimensionValues.at(i).at(1));
			remainedDimensionValues.push_back(remainedTemp);
			removedDimensionValues.push_back(removedTemp);

			remainedTemp.clear();
			removedTemp.clear();
			remainedTemp.push_back(dimensionValues.at(i).at(1));
			removedTemp.push_back(dimensionValues.at(i).at(0));
			remainedDimensionValues.push_back(remainedTemp);
			removedDimensionValues.push_back(removedTemp);
			break;
		case 1://这一维只有一个值，不能进行缩减
			break;
		}

		//针对每一种可能出现的情况，将余下的扩充为两倍并获取约束（去掉的一半点加扩充的点）
		for (int j = 0; j<removedDimensionValues.size(); j++)
		{
			//获取去掉的那一半的点，针对这一种去法有多种扩充
			vector<CCell*> removedCells;
			vector<vector<int>> tempDimensionValue;

			//获取被去掉的一半的点
			commonFuncs.getTempDimensionValue(dimensionValues, i, removedDimensionValues.at(j), tempDimensionValue);
			commonFuncs.ListAllCells(0, tempDimensionValue, removedCells);
			commonFuncs.kickOutCells(removedCells, allOTPs);//去掉的一半中不能选择OTP
			allShrinksCount++;
			/*
			*
			*去掉OTP之后进行判断：
			*1)如果removedCells中还有点，则说明此shrink是一个有效shrink，不再找扩充的点。
			*2)如果removedCells为空集，则需要继续找扩充的点
			*/

			//有剩余点
			if (removedCells.size() > 0)
			{
				//一行
				vector<CCell*> matrixRow;
				for (int m = 0; m<removedCells.size(); m++)
				{
					matrixRow.push_back(removedCells.at(m));
				}
				if (param != 2)
					matrixCells.push_back(matrixRow);
				validShrinksCount++;
				continue;
			}

			//获取剩下的一半的dimension值
			vector<vector<int>> tempRemainedDimensionValue;
			commonFuncs.getTempDimensionValue(dimensionValues, i, remainedDimensionValues.at(j), tempRemainedDimensionValue);

			//针对这种去法，列出所有可能的扩充
			vector<vector<CCell*>> expandCells;
			int literalN = 0;//
			for (int k = 0; k<dimensionNum; k++)//单独判断k==i的情况
			{
				vector<CCell*> expandCell;
				vector<vector<int>> tempExpandDimensionValue;
				vector<int> expandDimensionValue;
				int biggestValue = biggestDimensionValueAllowed.at(k);//该维允许的最大值（2或4）
				int kthRemainedDimensionValueNum = tempRemainedDimensionValue.at(k).size();
				switch (kthRemainedDimensionValueNum)
				{
				case 4://这种情况下这一维不能再扩充了
					break;
				case 2://这种情况下扩充的是剩下的两个，比如原来为1,2，扩充部分就为3,4
					if (k != i&&biggestValue == 4)//1.k==i时expand之后和shrink之前是一样的;2.如果该维的最大取值只到2则也不能进行expand
					{
						int num1 = tempRemainedDimensionValue.at(k).at(0);
						int num2 = tempRemainedDimensionValue.at(k).at(1);
						if ((num1 == 1 && num2 == 4) || (num1 == 4 && num2 == 1))
						{
							expandDimensionValue.push_back(2);
							expandDimensionValue.push_back(3);
						}
						else
						{
							int minNum = num1<num2 ? num1 : num2;
							expandDimensionValue.push_back(commonFuncs.boundValue(minNum + 2, biggestValue));
							expandDimensionValue.push_back(commonFuncs.boundValue(minNum + 3, biggestValue));
						}
						commonFuncs.getTempDimensionValue(tempRemainedDimensionValue, k, expandDimensionValue, tempExpandDimensionValue);
						commonFuncs.ListAllCells(0, tempExpandDimensionValue, expandCell);
						commonFuncs.kickOutCells(expandCell, allOTPs, allUTPs);//expand的点不能是TP
						if (removedCells.size() + expandCell.size()>0){
							expandCells.push_back(expandCell);
							literalNum.push_back(literalN);
						}
						literalN++;//
					}
					break;
				case 1://这种情况下，有两种扩充方式，比如原来为1，扩充部分可以是4或2，
					//但是得考虑k==i时，expand之后可能和shrink之前的一样
					int num = tempRemainedDimensionValue.at(k).at(0);
					bool noPlus1 = false;
					bool noMinus1 = false;
					if (k == i)
					{

						if (biggestValue == 2)//这个时候不能进行expand
						{
							noPlus1 = true;
							noMinus1 = true;
						}
						else
						{
							int anotherNum = removedDimensionValues.at(j).at(0);
							if (commonFuncs.boundValue(num + 1, biggestValue) == anotherNum)
								noPlus1 = true;
							if (commonFuncs.boundValue(num - 1, biggestValue) == anotherNum)
								noMinus1 = true;
						}
					}
					else
					{
						if (biggestValue == 2)//这个时候只能单方向expand
						{
							if (num == 0)//如果该维最大值是2，那么num只可能是0或1
								noMinus1 = true;
							else
								noPlus1 = true;
						}
					}
					if (!noPlus1)
					{
						expandDimensionValue.clear();
						tempExpandDimensionValue.clear();
						expandCell.clear();
						expandDimensionValue.push_back(commonFuncs.boundValue(num + 1, biggestValue));
						commonFuncs.getTempDimensionValue(tempRemainedDimensionValue, k, expandDimensionValue, tempExpandDimensionValue);
						commonFuncs.ListAllCells(0, tempExpandDimensionValue, expandCell);
						commonFuncs.kickOutCells(expandCell, allOTPs, allUTPs);//expand的点不能是TP
						if (removedCells.size() + expandCell.size()>0){
							expandCells.push_back(expandCell);
							literalNum.push_back(literalN);
						}
						literalN++;//
					}
					if (!noMinus1)
					{
						expandDimensionValue.clear();
						tempExpandDimensionValue.clear();
						expandCell.clear();
						expandDimensionValue.push_back(commonFuncs.boundValue(num - 1, biggestValue));
						commonFuncs.getTempDimensionValue(tempRemainedDimensionValue, k, expandDimensionValue, tempExpandDimensionValue);
						commonFuncs.ListAllCells(0, tempExpandDimensionValue, expandCell);
						commonFuncs.kickOutCells(expandCell, allOTPs, allUTPs);//expand的点不能是TP
						if (removedCells.size() + expandCell.size()>0){
							expandCells.push_back(expandCell);
							literalNum.push_back(literalN);
						}
						literalN++;//
					}
					break;
				}

			}
			//已经获取所有的expand cells，将去掉的一半和对应的扩充的一半合并组成矩阵中的一行
			for (int m = 0; m<expandCells.size(); m++)
			{
				vector<CCell*> matrixRow;
				for (int a = 0; a<removedCells.size(); a++)
					matrixRow.push_back(removedCells.at(a));
				for (int b = 0; b<expandCells.at(m).size(); b++)
					matrixRow.push_back(expandCells.at(m).at(b));
				if (param != 1)
					matrixCells.push_back(matrixRow);
			}
		}
	}

	//显示有效Shrink数和总的Shrink数
	string temp;
	temp.append("%d", allShrinksCount);
	showStr.append(temp + "\t");
	temp.append("%d", validShrinksCount);
	showStr.append(temp);
	return showStr;
}

string LRFGenerateCells(
	CCommonFuncs commonFuncs,
	string dimensionValuesStr,
	bool vOdd,
	vector<vector<CCell*>>& matrixCells,
	vector<int> allOTPs,
	vector<int> allUTPs
	)
{
	vector<int>literalNum;
	return LRFrunWithParameter(commonFuncs, 0, literalNum, dimensionValuesStr, vOdd, matrixCells, allOTPs, allUTPs);
}


string LIORFrunWithParameter(CCommonFuncs commonFuncs, bool separateNeed, vector<int>&literalNum, vector<vector<CCell*>>& lifCells, vector<vector<CCell*>>& lofCells, vector<vector<CCell*>>& lrfCells, string dimensionValuesStr, bool vOdd, vector<vector<CCell*>>& matrixCells, vector<int> allOTPs, vector<int> allUTPs, bool isSingleLitral)
{
	string showStr = "";
	vector<vector<int>> dimensionValues;//dimensionValuesStr的数据结构版 第一个vector是维度 第二个是该维度上的值
	vector<int> biggestDimensionValueAllowed;
	bool needLOF = true; //只要有一个invalid shrink对应的扩展可以检测所有的LOF 则不需要单独计算

	int totalLOFliteralsNum = 0;
	int maxValidLOFliteralsNum = 0;

	commonFuncs.getDimensionValue(dimensionValuesStr, vOdd, dimensionValues, biggestDimensionValueAllowed);

	bool needTOF = true;
	int dimensionNum = dimensionValues.size();
	//针对每一维，变为原来的1/2，再选一维扩大为原来的两倍

	for (int i = 0; i<dimensionNum; i++)
	{
		//分别获取剩下的一半和去掉的一半
		vector<vector<int>> remainedDimensionValues;//第一个vector的数目等于shrink的数目，第二个vector存放每一个shrink涉及的值(维度确定[1,4])
		vector<vector<int>> removedDimensionValues;//同上
		vector<int> remainedTemp;
		vector<int> removedTemp;

		int ithDimensionValueNum = dimensionValues.at(i).size();//在该维度真值的宽度
		//列举这一维可能出现的所有情况并记录下来 
		switch (ithDimensionValueNum)
		{
		case 4://这一维是1,2,3,4的情况
			for (int m = 1; m<5; m++)
			{
				remainedTemp.clear();
				removedTemp.clear();
				remainedTemp.push_back(commonFuncs.boundValue(m, 4));
				remainedTemp.push_back(commonFuncs.boundValue(m + 1, 4));
				removedTemp.push_back(commonFuncs.boundValue(m + 2, 4));
				removedTemp.push_back(commonFuncs.boundValue(m + 3, 4));
				remainedDimensionValues.push_back(remainedTemp);
				removedDimensionValues.push_back(removedTemp);
			}
			break;
		case 2://这一维是1,2或2,3或3,4或4,1的情况
			remainedTemp.clear();
			removedTemp.clear();
			remainedTemp.push_back(dimensionValues.at(i).at(0));
			removedTemp.push_back(dimensionValues.at(i).at(1));
			remainedDimensionValues.push_back(remainedTemp);
			removedDimensionValues.push_back(removedTemp);

			remainedTemp.clear();
			removedTemp.clear();
			remainedTemp.push_back(dimensionValues.at(i).at(1));
			removedTemp.push_back(dimensionValues.at(i).at(0));
			remainedDimensionValues.push_back(remainedTemp);
			removedDimensionValues.push_back(removedTemp);
			break;
		case 1://这一维只有一个值，不能进行缩减
			break;
		}

		//针对每一种shrink，将**余下**的扩充为两倍并获取约束（去掉的一半点加扩充的点）
		for (int j = 0; j<removedDimensionValues.size(); j++)
		{
			//获取去掉的那一半的点，针对这一种去法有多种扩充
			vector<CCell*> removedCells;
			vector<vector<int>> tempDimensionValue;

			//获取被去掉的一半的点,传入该项的各维度值，目前所在的维度，本次缩减的值，最后tempDimensionValue既是输入也是输出
			commonFuncs.getTempDimensionValue(dimensionValues, i, removedDimensionValues.at(j), tempDimensionValue);
			commonFuncs.ListAllCells(0, tempDimensionValue, removedCells);//tempDimensionValue是一个多维度形式的值
			commonFuncs.kickOutCells(removedCells, allOTPs);//去掉的一半中不能选择OTP

			/*
			*
			*去掉OTP之后进行判断：
			*1)如果removedCells中还有点，则说明此shrink是一个有效shrink，不再找扩充的点。
			*2)如果removedCells为空集，则需要继续找扩充的点
			*/

			//有剩余点:第一种情况
			if (removedCells.size() > 0)
			{
				//一行
				vector<CCell*> matrixRow;
				for (int m = 0; m<removedCells.size(); m++)
				{
					matrixRow.push_back(removedCells.at(m));
				}
				matrixCells.push_back(matrixRow);
				needTOF = false;
				if (separateNeed)
					lifCells.push_back(matrixRow);//lif
				continue;
			}
			//needLOF = false;
			//无剩余点:第二种情况
			//获取剩下的一半的dimension值
			vector<vector<int>> tempRemainedDimensionValue;
			commonFuncs.getTempDimensionValue(dimensionValues, i, remainedDimensionValues.at(j), tempRemainedDimensionValue);

			//针对这种去法，列出所有可能的扩充
			vector<vector<CCell*>> expandCells;
			int literalN = 0;//
			//如果所有维度的扩充都找到了有效的点，则说明该shrink对应的扩充点保证了该项的LOF不需要再单独测试
			bool needLOFInThisShrink = false;
			int validLOFliteralsNumThisShrink = 0;
			int totalLOFliteralsNumThisShrink = 0;
			for (int k = 0; k<dimensionNum; k++)//单独判断k==i的情况
			{
				vector<CCell*> expandCell;
				vector<vector<int>> tempExpandDimensionValue;
				vector<int> expandDimensionValue;
				int biggestValue = biggestDimensionValueAllowed.at(k);//该维允许的最大值（2或4）
				int kthRemainedDimensionValueNum = tempRemainedDimensionValue.at(k).size();
				switch (kthRemainedDimensionValueNum)
				{
				case 4://这种情况下这一维不能再扩充了
					break;
				case 2://这种情况下扩充的是剩下的两个，比如原来为1,2，扩充部分就为3,4
					if (k != i&&biggestValue == 4)//1.k==i时expand之后和shrink之前是一样的;2.如果该维的最大取值只到2则也不能进行expand
					{
						totalLOFliteralsNumThisShrink += 1;
						int num1 = tempRemainedDimensionValue.at(k).at(0);
						int num2 = tempRemainedDimensionValue.at(k).at(1);
						if ((num1 == 1 && num2 == 4) || (num1 == 4 && num2 == 1))
						{
							expandDimensionValue.push_back(2);
							expandDimensionValue.push_back(3);
						}
						else
						{
							int minNum = num1<num2 ? num1 : num2;
							expandDimensionValue.push_back(commonFuncs.boundValue(minNum + 2, biggestValue));
							expandDimensionValue.push_back(commonFuncs.boundValue(minNum + 3, biggestValue));
						}
						commonFuncs.getTempDimensionValue(tempRemainedDimensionValue, k, expandDimensionValue, tempExpandDimensionValue);
						commonFuncs.ListAllCells(0, tempExpandDimensionValue, expandCell);
						commonFuncs.kickOutCells(expandCell, allOTPs, allUTPs);//expand的点不能是TP
						if (expandCell.size()>0)
						{
							validLOFliteralsNumThisShrink += 1;
							expandCells.push_back(expandCell);
							literalNum.push_back(literalN);//
						}
						else
							needLOFInThisShrink = true;//该单个literal的LOF不能通过此shrink测到(因为2所以必需是单个)
						literalN++;
					}
					break;
				case 1://这种情况下，有两种扩充方式，比如原来为1，扩充部分可以是4或2，
					//但是得考虑k==i时，expand之后可能和shrink之前的一样
					int num = tempRemainedDimensionValue.at(k).at(0);
					bool noPlus1 = false;
					bool noMinus1 = false;
					if (k == i)
					{

						if (biggestValue == 2)//这个时候不能进行expand
						{
							noPlus1 = true;
							noMinus1 = true;
						}
						else
						{
							int anotherNum = removedDimensionValues.at(j).at(0);
							if (commonFuncs.boundValue(num + 1, biggestValue) == anotherNum)
								noPlus1 = true;
							if (commonFuncs.boundValue(num - 1, biggestValue) == anotherNum)
								noMinus1 = true;
						}
					}
					else
					{
						if (biggestValue == 2)//这个时候只能单方向expand
						{
							if (num == 0)//如果该维最大值是2，那么num只可能是0或1
								noMinus1 = true;
							else
								noPlus1 = true;
						}
					}
					if (!noPlus1)
					{
						totalLOFliteralsNumThisShrink += 1;
						expandDimensionValue.clear();
						tempExpandDimensionValue.clear();
						expandCell.clear();
						expandDimensionValue.push_back(commonFuncs.boundValue(num + 1, biggestValue));
						commonFuncs.getTempDimensionValue(tempRemainedDimensionValue, k, expandDimensionValue, tempExpandDimensionValue);
						commonFuncs.ListAllCells(0, tempExpandDimensionValue, expandCell);
						commonFuncs.kickOutCells(expandCell, allOTPs, allUTPs);//expand的点不能是TP
						if (expandCell.size()>0)
						{
							validLOFliteralsNumThisShrink += 1;
							expandCells.push_back(expandCell);
							literalNum.push_back(literalN);//
						}
						else
							needLOFInThisShrink = true;//该两个literal中的一个literal的LOF不能通过此shrink检测到
						literalN++;
					}
					if (!noMinus1)
					{
						totalLOFliteralsNumThisShrink += 1;
						expandDimensionValue.clear();
						tempExpandDimensionValue.clear();
						expandCell.clear();
						expandDimensionValue.push_back(commonFuncs.boundValue(num - 1, biggestValue));
						commonFuncs.getTempDimensionValue(tempRemainedDimensionValue, k, expandDimensionValue, tempExpandDimensionValue);
						commonFuncs.ListAllCells(0, tempExpandDimensionValue, expandCell);
						commonFuncs.kickOutCells(expandCell, allOTPs, allUTPs);//expand的点不能是TP
						if (expandCell.size()>0)
						{
							validLOFliteralsNumThisShrink += 1;
							expandCells.push_back(expandCell);
							literalNum.push_back(literalN);
						}
						else
							needLOFInThisShrink = true;//该两个literal中的一个literal的LOF不能通过此shrink检测到
						literalN++;
					}
					break;
				}

			}

			//old:在任意一个维度的shrink如果是invalid 则不需要进行LOF 因为expand的点肯定能检测到LOF
			//new:在任意一个维度的扩充都找到了可用的点，则说明该shrink可以检测该项的LOF
			if (!needLOFInThisShrink)
				needLOF = false;
			if (validLOFliteralsNumThisShrink > maxValidLOFliteralsNum)
				maxValidLOFliteralsNum = validLOFliteralsNumThisShrink;
			if (totalLOFliteralsNum == 0)
				totalLOFliteralsNum = totalLOFliteralsNumThisShrink;
			//已经获取所有的expand cells，将去掉的一半和对应的扩充的一半合并组成矩阵中的一行(上面改进之后这里应该只有扩展的点)
			for (int m = 0; m<expandCells.size(); m++)
			{
				vector<CCell*> matrixRow;
				for (int a = 0; a<removedCells.size(); a++)
					matrixRow.push_back(removedCells.at(a));
				for (int b = 0; b<expandCells.at(m).size(); b++)
					matrixRow.push_back(expandCells.at(m).at(b));
				matrixCells.push_back(matrixRow);
				if (separateNeed)
					lrfCells.push_back(matrixRow);//lrf
			}
		}
	}

	//需要单独计算LOF 
	if (!isSingleLitral && needLOF)
	{
		LOFGenerateCells(commonFuncs, dimensionValuesStr, vOdd, matrixCells, allOTPs, allUTPs);
		if (separateNeed)
			LOFGenerateCells(commonFuncs, dimensionValuesStr, vOdd, lofCells, allOTPs, allUTPs);//lof
	}

	if (needTOF)
	{//如果词只变成一项，则转变成TOF
		TOFGenerateCells(commonFuncs, dimensionValuesStr, vOdd, matrixCells, allOTPs);

	}
	if (isSingleLitral)
	{
		totalLOFliteralsNum = 0;
		maxValidLOFliteralsNum = 0;
	}
	//显示有效Expand数和总的Expand数
	string temp;
	temp.append("%d", totalLOFliteralsNum);
	showStr.append(temp + "\t");
	temp.append("%d", maxValidLOFliteralsNum);
	showStr.append(temp);
	return showStr;
}

string LIORFGenerateCells(CCommonFuncs commonFuncs, string dimensionValuesStr, bool vOdd, vector<int> allOTPs, vector<int> allUTPs, bool isSingleLitral, vector<vector<CCell*>>& matrixCells)
{
	vector<int>literalNum;
	vector<vector<CCell*>> lifCells;
	vector<vector<CCell*>> lofCells;
	vector<vector<CCell*>> lrfCells;
	string ret = LIORFrunWithParameter(commonFuncs, false, literalNum, lifCells, lofCells, lrfCells, dimensionValuesStr, vOdd, matrixCells, allOTPs, allUTPs, isSingleLitral);
	return ret;
}

string LIORFGenerateCells(CCommonFuncs commonFuncs, string dimensionValuesStr, bool vOdd, vector<int> allOTPs, vector<int> allUTPs, bool isSingleLitral, vector<vector<CCell*>>& matrixCells,
vector<vector<CCell*>> &lifCells,
vector<vector<CCell*>> &lofCells,
vector<vector<CCell*>> &lrfCells
)
{
	vector<int>literalNum;
	string ret = LIORFrunWithParameter(commonFuncs, true, literalNum, lifCells, lofCells, lrfCells, dimensionValuesStr, vOdd, matrixCells, allOTPs, allUTPs, isSingleLitral);
	return ret;
}