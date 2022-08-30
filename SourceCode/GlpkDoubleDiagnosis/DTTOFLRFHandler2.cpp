#include "stdafx.h"
#include "DTTOFLRFHandler.h"


//运行DLRF算法  
string CDTTOFLRFHandler::run2(string dimensionValuesStr1, string dimensionValuesStr2, bool vOdd, vector<vector<CCell*>>& matrixCells, vector<int> allOTPs, vector<int> allUTPs)
{
	vector<vector<int>> dimensionValues1;
	vector<int> biggestDimensionValueAllowed1;


	m_commonFuncs.getDimensionValue(dimensionValuesStr1,vOdd,dimensionValues1,biggestDimensionValueAllowed1);

	int dimensionNum1 = dimensionValues1.size();
	//针对每一维，变为原来的1/2，再选一维扩大为原来的两倍

	for(int i=0; i<dimensionNum1; i++)
	{
		//分别获取剩下的一半和去掉的一半
		vector<vector<int>> remainedDimensionValues1;
		vector<vector<int>> removedDimensionValues1;
		vector<int> remainedTemp1;
		vector<int> removedTemp1;

		int ithDimensionValueNum1 = dimensionValues1.at(i).size();
		//列举这一维可能出现的所有情况并记录下来
		switch(ithDimensionValueNum1)
		{
		case 4://这一维是1,2,3,4的情况
			for(int m=1; m<5; m++)
			{
				remainedTemp1.clear();
				removedTemp1.clear();
				remainedTemp1.push_back(m_commonFuncs.boundValue(m,4));
				remainedTemp1.push_back(m_commonFuncs.boundValue(m+1,4));
				removedTemp1.push_back(m_commonFuncs.boundValue(m+2,4));
				removedTemp1.push_back(m_commonFuncs.boundValue(m+3,4));
				remainedDimensionValues1.push_back(remainedTemp1);
				removedDimensionValues1.push_back(removedTemp1);
			}
			break;
		case 2://这一维是1,2或2,3或3,4或4,1的情况
			remainedTemp1.clear();
			removedTemp1.clear();
			remainedTemp1.push_back(dimensionValues1.at(i).at(0));
			removedTemp1.push_back(dimensionValues1.at(i).at(1));
			remainedDimensionValues1.push_back(remainedTemp1);
			removedDimensionValues1.push_back(removedTemp1);

			remainedTemp1.clear();
			removedTemp1.clear();
			remainedTemp1.push_back(dimensionValues1.at(i).at(1));
			removedTemp1.push_back(dimensionValues1.at(i).at(0));
			remainedDimensionValues1.push_back(remainedTemp1);
			removedDimensionValues1.push_back(removedTemp1);
			break;
		case 1://这一维只有一个值，不能进行缩减
			break;
		}

		//针对每一种可能出现的情况，将余下的扩充为两倍并获取约束（去掉的一半点加扩充的点）
		for(int j=0; j<removedDimensionValues1.size(); j++)
		{
			//获取去掉的那一半的点，针对这一种去法有多种扩充
			vector<CCell*> removedCells1;
			vector<vector<int>> tempDimensionValue1;

			//获取被去掉的一半的点
			m_commonFuncs.getTempDimensionValue(dimensionValues1,i,removedDimensionValues1.at(j),tempDimensionValue1);
			m_commonFuncs.ListAllCells(0,tempDimensionValue1,removedCells1);
			m_commonFuncs.kickOutCells(removedCells1,allOTPs);//去掉的一半中不能选择OTP
			/*
			*
			*去掉OTP之后进行判断：
			*1)如果removedCells中还有点，则说明此shrink是一个有效shrink，不再判断扩张和第二项的LIF。
			*2)如果removedCells为空集，则需要继续找扩充的点以及第二项的LIF
			*/

			//有剩余点
			if(removedCells1.size() > 0)
			{
				//m_commonFuncs.clearCellList(removedCells1);
				continue;
			}
			m_commonFuncs.clearCellList(removedCells1);
			//获取剩下的一半的dimension值
			vector<vector<int>> tempRemainedDimensionValue1;
			m_commonFuncs.getTempDimensionValue(dimensionValues1,i,remainedDimensionValues1.at(j),tempRemainedDimensionValue1);

			//针对这种去法，列出所有可能的扩充
			vector<vector<CCell*>> expandCells1;
			for(int k=0; k<dimensionNum1; k++)//单独判断k==i的情况
			{	
				vector<CCell*> expandCell1;
				vector<CCell*> expandUTPCell;
				vector<vector<int>> tempExpandDimensionValue1;
				vector<int> expandDimensionValue1;
				int biggestValue1 = biggestDimensionValueAllowed1.at(k);//该维允许的最大值（2或4）
				int kthRemainedDimensionValueNum1 = tempRemainedDimensionValue1.at(k).size();
				switch(kthRemainedDimensionValueNum1)
				{
				case 4://这种情况下这一维不能再扩充了
					break;
				case 2://这种情况下扩充的是剩下的两个，比如原来为1,2，扩充部分就为3,4
					if(k!=i&&biggestValue1==4)//1.k==i时expand之后和shrink之前是一样的;2.如果该维的最大取值只到2则也不能进行expand
					{
						int num1 = tempRemainedDimensionValue1.at(k).at(0);
						int num2 = tempRemainedDimensionValue1.at(k).at(1);
						if((num1==1&&num2==4) || (num1==4&&num2==1))
						{
							expandDimensionValue1.push_back(2);
							expandDimensionValue1.push_back(3);
						}
						else
						{
							int minNum = num1<num2? num1:num2;
							expandDimensionValue1.push_back(m_commonFuncs.boundValue(minNum+2,biggestValue1));
							expandDimensionValue1.push_back(m_commonFuncs.boundValue(minNum+3,biggestValue1));
						}
						m_commonFuncs.getTempDimensionValue(tempRemainedDimensionValue1,k,expandDimensionValue1,tempExpandDimensionValue1);
						m_commonFuncs.ListAllCells(0,tempExpandDimensionValue1,expandCell1);
						m_commonFuncs.keepCells(expandUTPCell ,expandCell1,allUTPs);
						m_commonFuncs.kickOutCells(expandCell1,allOTPs,allUTPs);//expand的点不能是TP
						if(expandCell1.size()>0){
							m_commonFuncs.clearCellList(expandCell1);
						}
						else{
							if (expandUTPCell.size()>0)
							{
								vector<vector<int>> dimensionValues2;
								vector<int> biggestDimensionValueAllowed2;
								//获取各维值
								m_commonFuncs.getDimensionValue(dimensionValuesStr2,vOdd,dimensionValues2,biggestDimensionValueAllowed2);

								int dimensionNum2 = dimensionValues2.size();
								vector<vector<CCell*>> shrinkCells2;
								//对每一维进行缩小，并取缩小部分的点
								for(int kk=0; kk<dimensionNum2; kk++)
								{
									vector<CCell*> shrinkCell2;
									vector<vector<int>> tempShrinkDimensionValue2;
									vector<int> shrinkDimensionValue2;
									int ithDimensionNum2 = dimensionValues2.at(kk).size();
									switch(ithDimensionNum2)
									{
									case 4: //此种情况缩减为2，有1,2；2,3；3,4和4,1四种情况
										for(int m=1; m<5; m++)
										{
											tempShrinkDimensionValue2.clear();
											shrinkCell2.clear();
											shrinkDimensionValue2.clear();
											shrinkDimensionValue2.push_back(m_commonFuncs.boundValue(m,4));
											shrinkDimensionValue2.push_back(m_commonFuncs.boundValue(m+1,4));
											m_commonFuncs.getTempDimensionValue(dimensionValues2,kk,shrinkDimensionValue2,tempShrinkDimensionValue2);
											m_commonFuncs.ListAllCells(0,tempShrinkDimensionValue2,shrinkCell2);
											m_commonFuncs.kickOutCells(shrinkCell2,allOTPs);
											m_commonFuncs.kickOutCommonCells(shrinkCell2,expandUTPCell);//Shrink的点不能是第一项expand的点
											//m_commonFuncs.clearCellList(expandUTPCell);
											if(shrinkCell2.size()>0)
											{
												matrixCells.push_back(shrinkCell2);
											}
											else
											{
												m_commonFuncs.clearCellList(shrinkCell2);
											}
										}
										break;
									case 2: //此种情况缩减为1
										for(int m=0; m<2; m++)
										{
											tempShrinkDimensionValue2.clear();
											shrinkCell2.clear();
											shrinkDimensionValue2.clear();
											shrinkDimensionValue2.push_back(m_commonFuncs.boundValue(dimensionValues2.at(kk).at(m),4));
											m_commonFuncs.getTempDimensionValue(dimensionValues2,kk,shrinkDimensionValue2,tempShrinkDimensionValue2);
											m_commonFuncs.ListAllCells(0,tempShrinkDimensionValue2,shrinkCell2);
											m_commonFuncs.kickOutCells(shrinkCell2,allOTPs);//Shrink的点不能是OTP
											m_commonFuncs.kickOutCommonCells(shrinkCell2,expandUTPCell);//Shrink的点不能是第一项expand的点
											//m_commonFuncs.clearCellList(expandUTPCell);
											if(shrinkCell2.size()>0)
											{
												matrixCells.push_back(shrinkCell2);
											}
											else
											{
												m_commonFuncs.clearCellList(shrinkCell2);
											}
										}
										break;
									case 1://此种情况不能再进行缩减
										break;
									}
								}
							}
						}
					}
					m_commonFuncs.clearCellList(expandUTPCell);
					break;
				case 1://这种情况下，有两种扩充方式，比如原来为1，扩充部分可以是4或2，
					//但是得考虑k==i时，expand之后可能和shrink之前的一样
					int num = tempRemainedDimensionValue1.at(k).at(0);
					bool noPlus1 = false;
					bool noMinus1 = false;
					if(k==i)
					{

						if(biggestValue1==2)//这个时候不能进行expand
						{
							noPlus1=true;
							noMinus1=true;
						}
						else
						{
							int anotherNum = removedDimensionValues1.at(j).at(0);
							if(m_commonFuncs.boundValue(num+1,biggestValue1)==anotherNum)
								noPlus1 = true;
							if(m_commonFuncs.boundValue(num-1,biggestValue1)==anotherNum)
								noMinus1 = true;
						}
					}
					else 
					{
						if(biggestValue1==2)//这个时候只能单方向expand
						{
							if(num==0)//如果该维最大值是2，那么num只可能是0或1
								noMinus1=true;
							else
								noPlus1=true;
						}
					}
					if(!noPlus1)
					{
						expandDimensionValue1.clear();
						tempExpandDimensionValue1.clear();
						expandCell1.clear();
						expandDimensionValue1.push_back(m_commonFuncs.boundValue(num+1,biggestValue1));
						m_commonFuncs.getTempDimensionValue(tempRemainedDimensionValue1,k,expandDimensionValue1,tempExpandDimensionValue1);
						m_commonFuncs.ListAllCells(0,tempExpandDimensionValue1,expandCell1);
						m_commonFuncs.keepCells(expandUTPCell ,expandCell1,allUTPs);
						m_commonFuncs.kickOutCells(expandCell1,allOTPs,allUTPs);//expand的点不能是TP
						if(expandCell1.size()>0){
							m_commonFuncs.clearCellList(expandCell1);
						}
						else{
							if (expandUTPCell.size()>0)
							{
								vector<vector<int>> dimensionValues2;
								vector<int> biggestDimensionValueAllowed2;
								//获取各维值
								m_commonFuncs.getDimensionValue(dimensionValuesStr2,false,dimensionValues2,biggestDimensionValueAllowed2);

								int dimensionNum2 = dimensionValues2.size();
								vector<vector<CCell*>> shrinkCells2;
								//对每一维进行缩小，并取缩小部分的点
								for(int kk=0; kk<dimensionNum2; kk++)
								{
									vector<CCell*> shrinkCell2;
									vector<vector<int>> tempShrinkDimensionValue2;
									vector<int> shrinkDimensionValue2;
									int ithDimensionNum2 = dimensionValues2.at(kk).size();
									switch(ithDimensionNum2)
									{
									case 4: //此种情况缩减为2，有1,2；2,3；3,4和4,1四种情况
										for(int m=1; m<5; m++)
										{
											tempShrinkDimensionValue2.clear();
											shrinkCell2.clear();
											shrinkDimensionValue2.clear();
											shrinkDimensionValue2.push_back(m_commonFuncs.boundValue(m,4));
											shrinkDimensionValue2.push_back(m_commonFuncs.boundValue(m+1,4));
											m_commonFuncs.getTempDimensionValue(dimensionValues2,kk,shrinkDimensionValue2,tempShrinkDimensionValue2);
											m_commonFuncs.ListAllCells(0,tempShrinkDimensionValue2,shrinkCell2);
											m_commonFuncs.kickOutCells(shrinkCell2,allOTPs);
											m_commonFuncs.kickOutCommonCells(shrinkCell2,expandUTPCell);//Shrink的点不能是第一项expand的点
											//m_commonFuncs.clearCellList(expandUTPCell);
											if(shrinkCell2.size()>0)
											{
												matrixCells.push_back(shrinkCell2);
											}
											else
											{
												m_commonFuncs.clearCellList(shrinkCell2);
											}
										}
										break;
									case 2: //此种情况缩减为1
										for(int m=0; m<2; m++)
										{
											tempShrinkDimensionValue2.clear();
											shrinkCell2.clear();
											shrinkDimensionValue2.clear();
											shrinkDimensionValue2.push_back(m_commonFuncs.boundValue(dimensionValues2.at(kk).at(m),4));
											m_commonFuncs.getTempDimensionValue(dimensionValues2,kk,shrinkDimensionValue2,tempShrinkDimensionValue2);
											m_commonFuncs.ListAllCells(0,tempShrinkDimensionValue2,shrinkCell2);
											m_commonFuncs.kickOutCells(shrinkCell2,allOTPs);//Shrink的点不能是OTP
											m_commonFuncs.kickOutCommonCells(shrinkCell2,expandUTPCell);//Shrink的点不能是第一项expand的点
											//m_commonFuncs.clearCellList(expandUTPCell);
											if(shrinkCell2.size()>0)
											{
												matrixCells.push_back(shrinkCell2);
											}
											else
											{
												m_commonFuncs.clearCellList(shrinkCell2);
											}
										}
										break;
									case 1://此种情况不能再进行缩减
										break;
									}
								}
							}
						}
					}
					if(!noMinus1)
					{
						expandDimensionValue1.clear();
						tempExpandDimensionValue1.clear();
						expandCell1.clear();
						expandDimensionValue1.push_back(m_commonFuncs.boundValue(num-1,biggestValue1));
						m_commonFuncs.getTempDimensionValue(tempRemainedDimensionValue1,k,expandDimensionValue1,tempExpandDimensionValue1);
						m_commonFuncs.ListAllCells(0,tempExpandDimensionValue1,expandCell1);
						m_commonFuncs.keepCells(expandUTPCell ,expandCell1,allUTPs);
						m_commonFuncs.kickOutCells(expandCell1,allOTPs,allUTPs);//expand的点不能是TP
						if(expandCell1.size()>0){
							m_commonFuncs.clearCellList(expandCell1);
						}
						else{
							if (expandUTPCell.size()>0)
							{
								vector<vector<int>> dimensionValues2;
								vector<int> biggestDimensionValueAllowed2;
								//获取各维值
								m_commonFuncs.getDimensionValue(dimensionValuesStr2,false,dimensionValues2,biggestDimensionValueAllowed2);

								int dimensionNum2 = dimensionValues2.size();
								vector<vector<CCell*>> shrinkCells2;
								//对每一维进行缩小，并取缩小部分的点
								for(int kk=0; kk<dimensionNum2; kk++)
								{
									vector<CCell*> shrinkCell2;
									vector<vector<int>> tempShrinkDimensionValue2;
									vector<int> shrinkDimensionValue2;
									int ithDimensionNum2 = dimensionValues2.at(kk).size();
									switch(ithDimensionNum2)
									{
									case 4: //此种情况缩减为2，有1,2；2,3；3,4和4,1四种情况
										for(int m=1; m<5; m++)
										{
											tempShrinkDimensionValue2.clear();
											shrinkCell2.clear();
											shrinkDimensionValue2.clear();
											shrinkDimensionValue2.push_back(m_commonFuncs.boundValue(m,4));
											shrinkDimensionValue2.push_back(m_commonFuncs.boundValue(m+1,4));
											m_commonFuncs.getTempDimensionValue(dimensionValues2,kk,shrinkDimensionValue2,tempShrinkDimensionValue2);
											m_commonFuncs.ListAllCells(0,tempShrinkDimensionValue2,shrinkCell2);
											m_commonFuncs.kickOutCells(shrinkCell2,allOTPs);
											m_commonFuncs.kickOutCommonCells(shrinkCell2,expandUTPCell);//Shrink的点不能是第一项expand的点
											//m_commonFuncs.clearCellList(expandUTPCell);
											if(shrinkCell2.size()>0)
											{
												matrixCells.push_back(shrinkCell2);
											}
											else
											{
												m_commonFuncs.clearCellList(shrinkCell2);
											}
										}
										break;
									case 2: //此种情况缩减为1
										for(int m=0; m<2; m++)
										{
											tempShrinkDimensionValue2.clear();
											shrinkCell2.clear();
											shrinkDimensionValue2.clear();
											shrinkDimensionValue2.push_back(m_commonFuncs.boundValue(dimensionValues2.at(kk).at(m),4));
											m_commonFuncs.getTempDimensionValue(dimensionValues2,kk,shrinkDimensionValue2,tempShrinkDimensionValue2);
											m_commonFuncs.ListAllCells(0,tempShrinkDimensionValue2,shrinkCell2);
											m_commonFuncs.kickOutCells(shrinkCell2,allOTPs);//Shrink的点不能是OTP
											m_commonFuncs.kickOutCommonCells(shrinkCell2,expandUTPCell);//Shrink的点不能是第一项expand的点
											//m_commonFuncs.clearCellList(expandUTPCell);
											if(shrinkCell2.size()>0)
											{
												matrixCells.push_back(shrinkCell2);
											}
											else
											{
												m_commonFuncs.clearCellList(shrinkCell2);
											}
										}
										break;
									case 1://此种情况不能再进行缩减
										break;
									}
								}
							}
						}
					}
					m_commonFuncs.clearCellList(expandUTPCell);
					break;
				}
			}
		}
	}

	return "";
}
