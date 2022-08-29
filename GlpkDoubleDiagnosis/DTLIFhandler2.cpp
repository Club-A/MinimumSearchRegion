#include "stdafx.h"
#include "DTLIFhandler.h"

//处理两项的double fault lifxlif 最新
string DTLIFhandler::run2(string dimensionValuesStr1, string dimensionValuesStr2, vector<vector<CCell*>>& matrixCells, vector<int> allOTPs, vector<int> all3OTPs)
{
	vector<vector<int>> dimensionValues1;
	vector<int> biggestDimensionValueAllowed1;
	//获取各维值
	m_commonFuncs.getDimensionValue(dimensionValuesStr1,false,dimensionValues1,biggestDimensionValueAllowed1);

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
					vector<vector<int>> dimensionValues2;
					vector<int> biggestDimensionValueAllowed2;
					//获取各维值
					m_commonFuncs.getDimensionValue(dimensionValuesStr2,false,dimensionValues2,biggestDimensionValueAllowed2);

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
									shrinkCells.push_back(shrinkCell);
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
									shrinkCells.push_back(shrinkCell);
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
					//此时为无效收缩，需要考虑第二项的收缩
					validshrink = false;
					vector<vector<int>> dimensionValues2;
					vector<int> biggestDimensionValueAllowed2;
					//获取各维值
					m_commonFuncs.getDimensionValue(dimensionValuesStr2,false,dimensionValues2,biggestDimensionValueAllowed2);

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
									shrinkCells.push_back(shrinkCell);
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
									shrinkCells.push_back(shrinkCell);
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
	



	//显示系数矩阵
	//showStr = getShowStr(matrixCells);
	//清空内存
	/*for (int i = 0; i < shrinkCells.size(); i++)
	{
		m_commonFuncs.clearCellList(shrinkCells.at(i));

	}*/
	shrinkCells.clear();
	vector<vector<CCell*>>().swap(shrinkCells);

	return "";

}

