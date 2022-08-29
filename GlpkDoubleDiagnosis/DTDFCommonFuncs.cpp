#include "stdafx.h"
#include "DTDFCommonFuncs.h"


DTDFCommonFuncs::DTDFCommonFuncs(void)
{
}


DTDFCommonFuncs::~DTDFCommonFuncs(void)
{
}

//处理两项的double fault lifxlif 最新
string DTDFCommonFuncs::generateMatrixofOTP(string dimensionValuesStr1, string dimensionValuesStr2,vector<vector<CCell*>>& matrixCells, vector<int> allOTPs, vector<int> all3OTPs,bool &hasequal, bool vOdd)
{
	vector<vector<int>> dimensionValues1;
	vector<int> biggestDimensionValueAllowed1;
	//获取各维值
	m_commonFuncs.getDimensionValue(dimensionValuesStr1,vOdd,dimensionValues1,biggestDimensionValueAllowed1);

	int dimensionNum1 = dimensionValues1.size();
	vector<vector<CCell*>> shrinkCells;
	//对每一维进行缩小，并取缩小部分的点
	for(int i=0; i<dimensionNum1; i++)
	{
		vector<CCell*> shrinkCell; //收缩的格
		vector<CCell*> shrinkOtpCell1;//第一项收缩的otp
		vector<vector<int>> tempShrinkDimensionValue;
		vector<int> shrinkDimensionValue1;//第一项在维度上的收缩
		bool validshrink = false; //判断这一维度上的收缩是否有效
		int ithDimensionNum1 = dimensionValues1.at(i).size();
		switch(ithDimensionNum1)
		{
		case 4: //此种情况缩减为2，有1,2；2,3；3,4和4,1四种情况
			for(int m=1; m<5; m++)
			{
				tempShrinkDimensionValue.clear();
				shrinkCell.clear();
				shrinkOtpCell1.clear();
				shrinkDimensionValue1.clear();
				shrinkDimensionValue1.push_back(m_commonFuncs.boundValue(m,4));
				shrinkDimensionValue1.push_back(m_commonFuncs.boundValue(m+1,4));
				m_commonFuncs.getTempDimensionValue(dimensionValues1,i,shrinkDimensionValue1,tempShrinkDimensionValue);
				m_commonFuncs.ListAllCells(0,tempShrinkDimensionValue,shrinkCell);
				m_commonFuncs.getShrinkOtps(shrinkCell,shrinkOtpCell1,allOTPs);
				m_commonFuncs.kickOutCells(shrinkCell,allOTPs);
				if(shrinkCell.size()>0)
				{
					//shrinkCells.push_back(shrinkCell);
					validshrink = true;
					//m_commonFuncs.clearCellList(shrinkCell);
					m_commonFuncs.clearCellList(shrinkOtpCell1);

				}
				else
				{
					//此时为无效收缩，需要考虑第二项的收缩
					validshrink = false;
					hasequal = true;
					vector<vector<int>> dimensionValues2;
					vector<int> biggestDimensionValueAllowed2;
					//获取各维值
					m_commonFuncs.getDimensionValue(dimensionValuesStr2,vOdd,dimensionValues2,biggestDimensionValueAllowed2);

					int dimensionNum2 = dimensionValues2.size();
					//开始枚举第二项各个维度上的收缩
					for (int j = 0; j <dimensionNum2; j++)
					{
						vector<CCell*> shrinkOtpCell2;//第二项收缩的otp
						vector<CCell*> OtpCellsof2Terms;//两项共同的收缩otp
						vector<int> shrinkDimensionValue2;//第二项在维度上的收缩
						int ithDimensionNum2 = dimensionValues2.at(j).size();
						switch(ithDimensionNum2)
						{
						case 4: //此种情况缩减为2，有1,2；2,3；3,4和4,1四种情况
							for(int m=1; m<5; m++)
							{
								tempShrinkDimensionValue.clear();
								shrinkCell.clear();
								shrinkOtpCell2.clear();
								shrinkDimensionValue2.clear();
								shrinkDimensionValue2.push_back(m_commonFuncs.boundValue(m,4));
								shrinkDimensionValue2.push_back(m_commonFuncs.boundValue(m+1,4));
								m_commonFuncs.getTempDimensionValue(dimensionValues2,j,shrinkDimensionValue2,tempShrinkDimensionValue);
								m_commonFuncs.ListAllCells(0,tempShrinkDimensionValue,shrinkCell);
								m_commonFuncs.getShrinkOtps(shrinkCell,shrinkOtpCell2,allOTPs);
								m_commonFuncs.kickOutCells(shrinkCell,allOTPs);
								if(shrinkCell.size()>0)
								{
									//shrinkCells.push_back(shrinkCell);
									//m_commonFuncs.clearCellList(shrinkCell);
									m_commonFuncs.clearCellList(shrinkOtpCell2);
								}
								else
								{
									OtpCellsof2Terms.clear();
									m_commonFuncs.getOverlapCells(shrinkOtpCell1,shrinkOtpCell2,OtpCellsof2Terms);
									m_commonFuncs.kickOutCells(OtpCellsof2Terms,all3OTPs);
									if (!OtpCellsof2Terms.empty())
									{
										shrinkCells.push_back(OtpCellsof2Terms);
										m_commonFuncs.clearCellList(shrinkCell);
										m_commonFuncs.clearCellList(shrinkOtpCell2);
										//m_commonFuncs.clearCellList(OtpCellsof2Terms);
									}
									else
									{
										m_commonFuncs.clearCellList(shrinkCell);
										m_commonFuncs.clearCellList(shrinkOtpCell2);
										m_commonFuncs.clearCellList(OtpCellsof2Terms);
									}
								}
							}
							break;
						case 2: //此种情况缩减为1
							for(int m=0; m<2; m++)
							{
								tempShrinkDimensionValue.clear();
								shrinkCell.clear();
								shrinkOtpCell2.clear();
								shrinkDimensionValue2.clear();
								shrinkDimensionValue2.push_back(m_commonFuncs.boundValue(dimensionValues2.at(j).at(m),4));
								m_commonFuncs.getTempDimensionValue(dimensionValues2,j,shrinkDimensionValue2,tempShrinkDimensionValue);
								m_commonFuncs.ListAllCells(0,tempShrinkDimensionValue,shrinkCell);
								m_commonFuncs.getShrinkOtps(shrinkCell,shrinkOtpCell2,allOTPs);
								m_commonFuncs.kickOutCells(shrinkCell,allOTPs);//Shrink的点不能是OTP
								if(shrinkCell.size()>0)
								{
									//shrinkCells.push_back(shrinkCell);
									//m_commonFuncs.clearCellList(shrinkCell);
									m_commonFuncs.clearCellList(shrinkOtpCell2);
								}
								else
								{
									OtpCellsof2Terms.clear();
									m_commonFuncs.getOverlapCells(shrinkOtpCell1,shrinkOtpCell2,OtpCellsof2Terms);
									m_commonFuncs.kickOutCells(OtpCellsof2Terms,all3OTPs);
									if (!OtpCellsof2Terms.empty())
									{
										shrinkCells.push_back(OtpCellsof2Terms);
										m_commonFuncs.clearCellList(shrinkCell);
										m_commonFuncs.clearCellList(shrinkOtpCell2);
										//m_commonFuncs.clearCellList(OtpCellsof2Terms);
									}
									else
									{
										m_commonFuncs.clearCellList(shrinkCell);
										m_commonFuncs.clearCellList(shrinkOtpCell2);
										m_commonFuncs.clearCellList(OtpCellsof2Terms);
									}
								}
							}
							break;
						case 1://此种情况不能再进行缩减
							break;
						}			
					}//第二项收缩结束

				}

			}
			break;
		case 2: //此种情况缩减为1
			for(int m=0; m<2; m++)
			{
				tempShrinkDimensionValue.clear();
				shrinkCell.clear();
				shrinkOtpCell1.clear();
				shrinkDimensionValue1.clear();
				shrinkDimensionValue1.push_back(m_commonFuncs.boundValue(dimensionValues1.at(i).at(m),4));
				m_commonFuncs.getTempDimensionValue(dimensionValues1,i,shrinkDimensionValue1,tempShrinkDimensionValue);
				m_commonFuncs.ListAllCells(0,tempShrinkDimensionValue,shrinkCell);
				m_commonFuncs.getShrinkOtps(shrinkCell,shrinkOtpCell1,allOTPs);
				m_commonFuncs.kickOutCells(shrinkCell,allOTPs);//Shrink的点不能是OTP
				if(shrinkCell.size()>0)
				{
					//shrinkCells.push_back(shrinkCell);
					validshrink = true;
					//m_commonFuncs.clearCellList(shrinkCell);
					m_commonFuncs.clearCellList(shrinkOtpCell1);
				}
				else
				{
					validshrink = false;
					hasequal = true;
					//此时为无效收缩，需要考虑第二项的收缩
					vector<vector<int>> dimensionValues2;
					vector<int> biggestDimensionValueAllowed2;
					//获取各维值
					m_commonFuncs.getDimensionValue(dimensionValuesStr2,vOdd,dimensionValues2,biggestDimensionValueAllowed2);

					int dimensionNum2 = dimensionValues2.size();
					//开始枚举第二项各个维度上的收缩
					for (int j = 0; j <dimensionNum2; j++)
					{
						vector<CCell*> shrinkOtpCell2;//第二项收缩的otp
						vector<CCell*> OtpCellsof2Terms;//两项共同的收缩otp
						vector<int> shrinkDimensionValue2;//第二项在维度上的收缩
						int ithDimensionNum2 = dimensionValues2.at(j).size();
						switch(ithDimensionNum2)
						{
						case 4: //此种情况缩减为2，有1,2；2,3；3,4和4,1四种情况
							for(int m=1; m<5; m++)
							{
								tempShrinkDimensionValue.clear();
								shrinkCell.clear();
								shrinkOtpCell2.clear();
								shrinkDimensionValue2.clear();
								shrinkDimensionValue2.push_back(m_commonFuncs.boundValue(m,4));
								shrinkDimensionValue2.push_back(m_commonFuncs.boundValue(m+1,4));
								m_commonFuncs.getTempDimensionValue(dimensionValues2,j,shrinkDimensionValue2,tempShrinkDimensionValue);
								m_commonFuncs.ListAllCells(0,tempShrinkDimensionValue,shrinkCell);
								m_commonFuncs.getShrinkOtps(shrinkCell,shrinkOtpCell2,allOTPs);
								m_commonFuncs.kickOutCells(shrinkCell,allOTPs);
								if(shrinkCell.size()>0)
								{
									//shrinkCells.push_back(shrinkCell);
									//m_commonFuncs.clearCellList(shrinkCell);
									m_commonFuncs.clearCellList(shrinkOtpCell2);
								}
								else
								{
									OtpCellsof2Terms.clear();
									m_commonFuncs.getOverlapCells(shrinkOtpCell1,shrinkOtpCell2,OtpCellsof2Terms);
									m_commonFuncs.kickOutCells(OtpCellsof2Terms,all3OTPs);
									if (!OtpCellsof2Terms.empty())
									{
										shrinkCells.push_back(OtpCellsof2Terms);
										m_commonFuncs.clearCellList(shrinkCell);
										m_commonFuncs.clearCellList(shrinkOtpCell2);
										//m_commonFuncs.clearCellList(OtpCellsof2Terms);
									}
									else
									{
										m_commonFuncs.clearCellList(shrinkCell);
										m_commonFuncs.clearCellList(shrinkOtpCell2);
										m_commonFuncs.clearCellList(OtpCellsof2Terms);
									}
								}
							}
							break;
						case 2: //此种情况缩减为1
							for(int m=0; m<2; m++)
							{
								tempShrinkDimensionValue.clear();
								vector<vector<int>>().swap(tempShrinkDimensionValue);
								shrinkCell.clear();
								vector<CCell*>().swap(shrinkCell);
								shrinkOtpCell2.clear();
								vector<CCell*>().swap(shrinkOtpCell2);
								shrinkDimensionValue2.clear();
								vector<int>().swap(shrinkDimensionValue2);
								shrinkDimensionValue2.push_back(m_commonFuncs.boundValue(dimensionValues2.at(j).at(m),4));
								m_commonFuncs.getTempDimensionValue(dimensionValues2,j,shrinkDimensionValue2,tempShrinkDimensionValue);
								m_commonFuncs.ListAllCells(0,tempShrinkDimensionValue,shrinkCell);
								m_commonFuncs.getShrinkOtps(shrinkCell,shrinkOtpCell2,allOTPs);
								m_commonFuncs.kickOutCells(shrinkCell,allOTPs);//Shrink的点不能是OTP
								if(shrinkCell.size()>0)
								{
									//shrinkCells.push_back(shrinkCell);
									//m_commonFuncs.clearCellList(shrinkCell);
									m_commonFuncs.clearCellList(shrinkOtpCell2);
								}
								else
								{
									OtpCellsof2Terms.clear();
									m_commonFuncs.getOverlapCells(shrinkOtpCell1,shrinkOtpCell2,OtpCellsof2Terms);
									m_commonFuncs.kickOutCells(OtpCellsof2Terms,all3OTPs);
									if (!OtpCellsof2Terms.empty())
									{
										shrinkCells.push_back(OtpCellsof2Terms);
										m_commonFuncs.clearCellList(shrinkCell);
										m_commonFuncs.clearCellList(shrinkOtpCell2);
										//m_commonFuncs.clearCellList(OtpCellsof2Terms);
									}
									else
									{
										m_commonFuncs.clearCellList(shrinkCell);
										m_commonFuncs.clearCellList(shrinkOtpCell2);
										m_commonFuncs.clearCellList(OtpCellsof2Terms);
									}
								}
							}
							break;
						case 1://此种情况不能再进行缩减
							break;
						}
						//清空内存
					}//第二项收缩结束
				}
				
			}
			break;
		case 1://此种情况不能再进行缩减
			break;
		}
		//清空内存
	}//第一项枚举结束

	//已经获取所有的shrink cells，将所有可能的shrink组成一个矩阵，一行表示一种shrink
	for(int i=0; i<shrinkCells.size(); i++)
	{
		vector<CCell*> matrixRow;
		for(int j=0; j<shrinkCells.at(i).size(); j++)
			matrixRow.push_back(shrinkCells.at(i).at(j));
		matrixCells.push_back(matrixRow);
	}
	shrinkCells.clear();
	vector<vector<CCell*>>().swap(shrinkCells);

	return "";

}

//处理两项的double fault lofxlof
string DTDFCommonFuncs::generateMatrixofRFP(string dimensionValuesStr1, vector<vector<CCell*>>& matrixCells, vector<int> allUTPs, vector<int> allOTPs, bool &hasequal,bool vOdd)
{
	vector<vector<int>> dimensionValues;
	vector<int> biggestDimensionValueAllowed;

	m_commonFuncs.getDimensionValue(dimensionValuesStr1, vOdd, dimensionValues, biggestDimensionValueAllowed);
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
					expandDimensionValue.push_back(m_commonFuncs.boundValue(minNum + 2, biggestValue));
					expandDimensionValue.push_back(m_commonFuncs.boundValue(minNum + 3, biggestValue));
				}
				m_commonFuncs.getTempDimensionValue(dimensionValues, i, expandDimensionValue, tempExpandDimensionValue);
				m_commonFuncs.ListAllCells(0, tempExpandDimensionValue, expandCell);
				m_commonFuncs.kickOutCells(expandCell, allOTPs, allUTPs);//expand的点不能是TP
				if (expandCell.size()>0){//有效扩张，不用记录了
					/*printf("[%d]expandCell\n", __LINE__);
					for (int j = 0; j < expandCell.size(); j++){
						printf("[%d]", expandCell[j]->getTModelIndex());
					}
					printf("\n");*/
					expandCell.clear();
				}
				{//看二次扩张
					hasequal = true;
					// 当前的mutant,(用一个项来表示，这个项各个维的取值存在mutantsDimensionValue中)							
					vector<vector<int>> RemainedDimensionValues;
					RemainedDimensionValues.insert(RemainedDimensionValues.begin(), dimensionValues.begin(), dimensionValues.end());
					RemainedDimensionValues[i].insert(RemainedDimensionValues[i].begin(), expandDimensionValue.begin(), expandDimensionValue.end());

					int dimensionNum2 = RemainedDimensionValues.size();
					//开始枚举变化后各个维度上的扩张
					for (int j = 0; j <dimensionNum2; j++)
					{
						vector<CCell*> expandCell2;
						vector<vector<int>> tempExpandDimensionValue2;
						vector<int> expandDimensionValue2;
						int biggestValue2 = 0;
						if (vOdd && j == dimensionNum2 - 1)
							biggestValue2 = 2;
						else
							biggestValue2 = 4;
						int ithDimensionNum2 = RemainedDimensionValues.at(j).size();
						switch (ithDimensionNum2)
						{
						case 4: //此种情况不能进行扩充
							break;
						case 2: //此种情况扩充为4
							if (biggestValue2 == 4)//如果等于2则不能扩充
							{
								int num1 = RemainedDimensionValues.at(j).at(0);
								int num2 = RemainedDimensionValues.at(j).at(1);
								if ((num1 == 1 && num2 == 4) || (num1 == 4 && num2 == 1))
								{
									expandDimensionValue2.push_back(2);
									expandDimensionValue2.push_back(3);
								}
								else
								{
									int minNum = num1<num2 ? num1 : num2;
									expandDimensionValue2.push_back(m_commonFuncs.boundValue(minNum + 2, biggestValue2));
									expandDimensionValue2.push_back(m_commonFuncs.boundValue(minNum + 3, biggestValue2));
								}
								m_commonFuncs.getTempDimensionValue(RemainedDimensionValues, i, expandDimensionValue2, tempExpandDimensionValue2);
								m_commonFuncs.ListAllCells(0, tempExpandDimensionValue2, expandCell2);
								m_commonFuncs.kickOutCells(expandCell2, allOTPs, allUTPs);//expand的点不能是TP
								if (expandCell2.size()>0){
									expandCells.push_back(expandCell2);//记录扩张
								}
							}
							break;
						case 1://这种情况下，有两种扩充方式，比如原来为1，扩充部分可以是4或2
							int num = RemainedDimensionValues.at(j).at(0);
							bool noPlus1 = false, noMinus1 = false;
							if (biggestValue2 == 2)//这种情况只能往一边扩充，num的值非1即2
							{
								if (num == 1)
									noMinus1 = true;
								else
									noPlus1 = true;
							}

							if (!noPlus1)
							{
								expandDimensionValue2.push_back(m_commonFuncs.boundValue(num + 1, biggestValue2));
								m_commonFuncs.getTempDimensionValue(RemainedDimensionValues, i, expandDimensionValue2, tempExpandDimensionValue2);
								m_commonFuncs.ListAllCells(0, tempExpandDimensionValue2, expandCell2);
								m_commonFuncs.kickOutCells(expandCell2, allOTPs, allUTPs);//expand的点不能是TP
								if (expandCell2.size() > 0)
								{
									expandCells.push_back(expandCell2);
								}
							}

							if (!noMinus1)
							{
								expandDimensionValue2.clear();
								tempExpandDimensionValue2.clear();
								expandCell2.clear();
								expandDimensionValue2.push_back(m_commonFuncs.boundValue(num - 1, biggestValue2));
								m_commonFuncs.getTempDimensionValue(RemainedDimensionValues, i, expandDimensionValue2, tempExpandDimensionValue2);
								m_commonFuncs.ListAllCells(0, tempExpandDimensionValue2, expandCell2);
								m_commonFuncs.kickOutCells(expandCell2, allOTPs, allUTPs);//expand的点不能是TP
								if (expandCell2.size() > 0)
								{
									expandCells.push_back(expandCell2);
								}
							}
							break;
						}
						expandCell2.clear();
						tempExpandDimensionValue2.clear();
						expandDimensionValue2.clear();
					}//第二次扩张结束
				}
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
				expandDimensionValue.push_back(m_commonFuncs.boundValue(num + 1, biggestValue));
				m_commonFuncs.getTempDimensionValue(dimensionValues, i, expandDimensionValue, tempExpandDimensionValue);
				m_commonFuncs.ListAllCells(0, tempExpandDimensionValue, expandCell);
				m_commonFuncs.kickOutCells(expandCell, allOTPs, allUTPs);//expand的点不能是TP
				if (expandCell.size() > 0)
				{
					/*printf("[%d]expandCell\n", __LINE__);
					for (int j = 0; j < expandCell.size(); j++){
						printf("[%d]", expandCell[j]->getTModelIndex());
					}
					printf("\n");*/
					expandCell.clear();
				}
				{//看二次扩张
					hasequal = true;
					// 当前的mutant,(用一个项来表示，这个项各个维的取值存在mutantsDimensionValue中)							
					vector<vector<int>> RemainedDimensionValues;
					RemainedDimensionValues.insert(RemainedDimensionValues.begin(), dimensionValues.begin(), dimensionValues.end());
					RemainedDimensionValues[i].insert(RemainedDimensionValues[i].begin(), expandDimensionValue.begin(), expandDimensionValue.end());

					int dimensionNum2 = RemainedDimensionValues.size();
					//开始枚举变化后各个维度上的扩张
					for (int j = 0; j <dimensionNum2; j++)
					{
						vector<CCell*> expandCell2;
						vector<vector<int>> tempExpandDimensionValue2;
						vector<int> expandDimensionValue2;
						int biggestValue2 = 0;
						if (vOdd && j == dimensionNum2 - 1)
							biggestValue2 = 2;
						else
							biggestValue2 = 4;
						int ithDimensionNum2 = RemainedDimensionValues.at(j).size();
						switch (ithDimensionNum2)
						{
						case 4: //此种情况不能进行扩充
							break;
						case 2: //此种情况扩充为4
							if (biggestValue2 == 4)//如果等于2则不能扩充
							{
								int num1 = RemainedDimensionValues.at(j).at(0);
								int num2 = RemainedDimensionValues.at(j).at(1);
								if ((num1 == 1 && num2 == 4) || (num1 == 4 && num2 == 1))
								{
									expandDimensionValue2.push_back(2);
									expandDimensionValue2.push_back(3);
								}
								else
								{
									int minNum = num1<num2 ? num1 : num2;
									expandDimensionValue2.push_back(m_commonFuncs.boundValue(minNum + 2, biggestValue2));
									expandDimensionValue2.push_back(m_commonFuncs.boundValue(minNum + 3, biggestValue2));
								}
								m_commonFuncs.getTempDimensionValue(RemainedDimensionValues, j, expandDimensionValue2, tempExpandDimensionValue2);
								m_commonFuncs.ListAllCells(0, tempExpandDimensionValue2, expandCell2);
								m_commonFuncs.kickOutCells(expandCell2, allOTPs, allUTPs);//expand的点不能是TP
								if (expandCell2.size()>0)
									expandCells.push_back(expandCell2);//记录扩张
							}
							break;
						case 1://这种情况下，有两种扩充方式，比如原来为1，扩充部分可以是4或2
							int num = RemainedDimensionValues.at(j).at(0);
							bool noPlus1 = false, noMinus1 = false;
							if (biggestValue2 == 2)//这种情况只能往一边扩充，num的值非1即2
							{
								if (num == 1)
									noMinus1 = true;
								else
									noPlus1 = true;
							}

							if (!noPlus1)
							{
								expandDimensionValue2.push_back(m_commonFuncs.boundValue(num + 1, biggestValue2));
								m_commonFuncs.getTempDimensionValue(RemainedDimensionValues, j, expandDimensionValue2, tempExpandDimensionValue2);
								m_commonFuncs.ListAllCells(0, tempExpandDimensionValue2, expandCell2);
								m_commonFuncs.kickOutCells(expandCell2, allOTPs, allUTPs);//expand的点不能是TP
								if (expandCell2.size() > 0)
								{
									expandCells.push_back(expandCell2);
								}
							}

							if (!noMinus1)
							{
								expandDimensionValue2.clear();
								tempExpandDimensionValue2.clear();
								expandCell2.clear();
								expandDimensionValue2.push_back(m_commonFuncs.boundValue(num - 1, biggestValue2));
								m_commonFuncs.getTempDimensionValue(RemainedDimensionValues, j, expandDimensionValue2, tempExpandDimensionValue2);
								m_commonFuncs.ListAllCells(0, tempExpandDimensionValue2, expandCell2);
								m_commonFuncs.kickOutCells(expandCell2, allOTPs, allUTPs);//expand的点不能是TP
								if (expandCell2.size() > 0)
								{
									expandCells.push_back(expandCell2);
								}
							}
							break;
						}
						expandCell2.clear();
						tempExpandDimensionValue2.clear();
						expandDimensionValue2.clear();
					}//第二次扩张结束
				}
			}

			if (!noMinus1)
			{
				expandDimensionValue.clear();
				tempExpandDimensionValue.clear();
				expandCell.clear();
				expandDimensionValue.push_back(m_commonFuncs.boundValue(num - 1, biggestValue));
				m_commonFuncs.getTempDimensionValue(dimensionValues, i, expandDimensionValue, tempExpandDimensionValue);
				m_commonFuncs.ListAllCells(0, tempExpandDimensionValue, expandCell);
				m_commonFuncs.kickOutCells(expandCell, allOTPs, allUTPs);//expand的点不能是TP
				if (expandCell.size() > 0)
				{
					/*printf("[%d]expandCell\n", __LINE__);
					for (int j = 0; j < expandCell.size();j++){
						printf("[%d]", expandCell[j]->getTModelIndex());
					}
					printf("\n");*/
					expandCell.clear();
				}
				{//看二次扩张
					hasequal = true;
					// 当前的mutant,(用一个项来表示，这个项各个维的取值存在mutantsDimensionValue中)							
					vector<vector<int>> RemainedDimensionValues;
					RemainedDimensionValues.insert(RemainedDimensionValues.begin(), dimensionValues.begin(), dimensionValues.end());
					RemainedDimensionValues[i].insert(RemainedDimensionValues[i].begin(), expandDimensionValue.begin(), expandDimensionValue.end());
					int dimensionNum2 = RemainedDimensionValues.size();
					//开始枚举变化后各个维度上的扩张
					for (int j = 0; j <dimensionNum2; j++)
					{
						vector<CCell*> expandCell2;
						vector<vector<int>> tempExpandDimensionValue2;
						vector<int> expandDimensionValue2;
						int biggestValue2 = 0;
						if (vOdd && j == dimensionNum2 - 1)
							biggestValue2 = 2;
						else
							biggestValue2 = 4;
						int ithDimensionNum2 = RemainedDimensionValues.at(j).size();
						switch (ithDimensionNum2)
						{
						case 4: //此种情况不能进行扩充
							break;
						case 2: //此种情况扩充为4
							if (biggestValue2 == 4)//如果等于2则不能扩充
							{
								int num1 = RemainedDimensionValues.at(j).at(0);
								int num2 = RemainedDimensionValues.at(j).at(1);
								if ((num1 == 1 && num2 == 4) || (num1 == 4 && num2 == 1))
								{
									expandDimensionValue2.push_back(2);
									expandDimensionValue2.push_back(3);
								}
								else
								{
									int minNum = num1<num2 ? num1 : num2;
									expandDimensionValue2.push_back(m_commonFuncs.boundValue(minNum + 2, biggestValue2));
									expandDimensionValue2.push_back(m_commonFuncs.boundValue(minNum + 3, biggestValue2));
								}
								m_commonFuncs.getTempDimensionValue(RemainedDimensionValues, j, expandDimensionValue2, tempExpandDimensionValue2);
								m_commonFuncs.ListAllCells(0, tempExpandDimensionValue2, expandCell2);
								m_commonFuncs.kickOutCells(expandCell2, allOTPs, allUTPs);//expand的点不能是TP
								if (expandCell2.size()>0){
									expandCells.push_back(expandCell2);//记录扩张
								}
							}
							break;
						case 1://这种情况下，有两种扩充方式，比如原来为1，扩充部分可以是4或2
							int num = RemainedDimensionValues.at(j).at(0);
							bool noPlus1 = false, noMinus1 = false;
							if (biggestValue2 == 2)//这种情况只能往一边扩充，num的值非1即2
							{
								if (num == 1)
									noMinus1 = true;
								else
									noPlus1 = true;
							}

							if (!noPlus1)
							{
								expandDimensionValue2.push_back(m_commonFuncs.boundValue(num + 1, biggestValue2));
								m_commonFuncs.getTempDimensionValue(RemainedDimensionValues, j, expandDimensionValue2, tempExpandDimensionValue2);
								m_commonFuncs.ListAllCells(0, tempExpandDimensionValue2, expandCell2);
								m_commonFuncs.kickOutCells(expandCell2, allOTPs, allUTPs);//expand的点不能是TP
								if (expandCell2.size() > 0)
								{
									expandCells.push_back(expandCell2);
								}
							}

							if (!noMinus1)
							{
								expandDimensionValue2.clear();
								tempExpandDimensionValue2.clear();
								expandCell2.clear();
								expandDimensionValue2.push_back(m_commonFuncs.boundValue(num - 1, biggestValue2));
								m_commonFuncs.getTempDimensionValue(RemainedDimensionValues, j, expandDimensionValue2, tempExpandDimensionValue2);
								m_commonFuncs.ListAllCells(0, tempExpandDimensionValue2, expandCell2);
								m_commonFuncs.kickOutCells(expandCell2, allOTPs, allUTPs);//expand的点不能是TP
								if (expandCell2.size() > 0)
								{
									expandCells.push_back(expandCell2);
								}
							}
							break;
						}
						expandCell2.clear();
						tempExpandDimensionValue2.clear();
						expandDimensionValue2.clear();
					}//第二次扩张结束
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
	expandCells.clear();
	vector<vector<CCell*>>().swap(expandCells);
	return "";

}
