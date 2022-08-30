#pragma once
#include "StdAfx.h"
#include "LIFGenerateCellsForMutants.h"
//运行LRF算法  param为1，只返回validShrink部分  param为2，只返回expand部分 其他值两个都返回
string runWithParameter(
	CCommonFuncs commonFuncs,
	int param,
	vector<int>&literalNum,
	string dimensionValuesStr,
	bool vOdd,
	vector<vector<CCell*>>& matrixCells,
	vector<vector<vector<int>>>& mutantsDimensionValue,
	vector<vector<CCell*>>& corresponding_tests,
	vector<int> allOTPs,
	vector<int> allUTPs
	)
{
	vector<vector<int>> dimensionValues;
	vector<int> biggestDimensionValueAllowed;
	int allShrinksCount = 0;
	int validShrinksCount = 0;

	// if mutantsNeed ,how to identify a equivalent LIF mutant 
	// Then if yes, generate a TIF_LRF_mutants 
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
			// Let XY represent the i-th dimension
			/*   to remove      will remain   meaning(insert a ...)
			*    1 , 2          3 , 4          insert  X
			*    2 , 3          1 , 4          insert  !Y
			*    3 , 4          1 , 2		   insert  !X
			*    4 , 1          2 , 3		   insert  Y
			*/
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
			bool hasEquivalentLifOccurred = false;
			/*
			*
			*去掉OTP之后进行判断：
			*1)如果removedCells中还有点，则说明此shrink是一个有效shrink，不再找扩充的点。
			*2)如果removedCells为空集，则需要继续找扩充的点
			*/

			if (removedCells.size() > 0){//有效收缩，无需记录
				hasEquivalentLifOccurred = true;
				//但对应扩张点需要记录约束方程
			}
			else{//无效收缩，需要记录
				hasEquivalentLifOccurred = false;
			}

			if (!hasEquivalentLifOccurred){
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
							/*Let XY represent the k-th dimension
							*    init          expanded        meaning(omit a ...)
							*    1 , 2          3 , 4          omit  X
							*    2 , 3          1 , 4          omit  !Y
							*    3 , 4          1 , 2		   omit  !X
							*    4 , 1          2 , 3		   omit  Y
							*/

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
							literalN++;

							vector<CCell*> matrixRow;
							// 当前的mutant,(用一个项来表示，这个项各个维的取值存在mutantsDimensionValue中)							
							vector<vector<int>> RemainedDimensionValue;
							RemainedDimensionValue.insert(RemainedDimensionValue.begin(), tempRemainedDimensionValue.begin(), tempRemainedDimensionValue.end());
							RemainedDimensionValue[k].insert(RemainedDimensionValue[k].begin(), expandDimensionValue.begin(), expandDimensionValue.end());
							mutantsDimensionValue.push_back(RemainedDimensionValue);
							//把当前mutant对应的测试用例集收集到corresponding_tests中
							for (int num = 0; num<expandCell.size(); num++)
							{
								matrixRow.push_back(expandCell[num]);
							}
							for (int num = 0; num<removedCells.size(); num++)
							{
								matrixRow.push_back(removedCells[num]);
							}
							corresponding_tests.push_back(matrixRow);
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

							vector<CCell*> matrixRow;
							// 当前的mutant,(用一个项来表示，这个项各个维的取值存在mutantsDimensionValue中)							
							vector<vector<int>> RemainedDimensionValue;
							RemainedDimensionValue.insert(RemainedDimensionValue.begin(), tempRemainedDimensionValue.begin(), tempRemainedDimensionValue.end());
							RemainedDimensionValue[k].insert(RemainedDimensionValue[k].begin(), expandDimensionValue.begin(), expandDimensionValue.end());
							mutantsDimensionValue.push_back(RemainedDimensionValue);
							//把当前mutant对应的测试用例集收集到corresponding_tests中
							//corresponding_tests.push_back(expandCell);
							for (int num = 0; num<expandCell.size(); num++)
							{
								matrixRow.push_back(expandCell[num]);
							}
							for (int num = 0; num<removedCells.size(); num++)
							{
								matrixRow.push_back(removedCells[num]);
							}
							corresponding_tests.push_back(matrixRow);

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

							vector<CCell*> matrixRow;
							// 当前的mutant,(用一个项来表示，这个项各个维的取值存在mutantsDimensionValue中)							
							vector<vector<int>> RemainedDimensionValue;
							RemainedDimensionValue.insert(RemainedDimensionValue.begin(), tempRemainedDimensionValue.begin(), tempRemainedDimensionValue.end());
							RemainedDimensionValue[k].insert(RemainedDimensionValue[k].begin(), expandDimensionValue.begin(), expandDimensionValue.end());
							mutantsDimensionValue.push_back(RemainedDimensionValue);
							//把当前mutant对应的测试用例集收集到corresponding_tests中
							//corresponding_tests.push_back(expandCell);
							for (int num = 0; num<expandCell.size(); num++)
							{
								matrixRow.push_back(expandCell[num]);
							}
							for (int num = 0; num<removedCells.size(); num++)
							{
								matrixRow.push_back(removedCells[num]);
							}
							corresponding_tests.push_back(matrixRow);
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
	}

	return "";
}
//运行LRF算法
string LRFGenerateCellsForMutants(
	CCommonFuncs commonFuncs,
	string dimensionValuesStr, 
	bool vOdd,
	vector<vector<CCell*>>& matrixCells,
	vector<vector<vector<int>>>& mutantsDimensionValue,
	vector<vector<CCell*>>& corresponding_tests,
	vector<int> allOTPs, 
	vector<int> allUTPs
	)
{
	vector<int>literalNum;
	return runWithParameter(commonFuncs,0, literalNum, dimensionValuesStr, vOdd, matrixCells,
	 mutantsDimensionValue,
	 corresponding_tests, allOTPs, allUTPs);
}

