#include "StdAfx.h"
#include "DoubleLIRFHandler.h"


//运行LRF算法
string CDoubleLIRFHandler::run2(
	string dimensionValuesStr, bool vOdd,
	vector<vector<CCell*>>& matrixCells,
	vector<int> allOTPs, vector<int> allUTPs
	)
{
	vector<int>literalNum;
	return runWithParameter2(0,literalNum, dimensionValuesStr,vOdd, matrixCells, allOTPs,  allUTPs);
}
//运行LIF+LRF算法
string CDoubleLIRFHandler::runWithParameter2(
	int param, vector<int>&literalNum, 
	string dimensionValuesStr, bool vOdd, 
	vector<vector<CCell*>>& matrixCells, 
	vector<int> allOTPs, vector<int> allUTPs
	)
{
	int i=0;
	int	j=0;
	int k=0;
	int l=0;
	int n=0;
	int flag=0;

	vector<vector<int>> dimensionValues;
	vector<int> biggestDimensionValueAllowed;
	int allShrinksCount = 0;
	int validShrinksCount = 0;

	m_commonFuncs.getDimensionValue(dimensionValuesStr,vOdd,dimensionValues,biggestDimensionValueAllowed);

	int dimensionNum = dimensionValues.size();

	//针对每一维，变为原来的1/2，再选一维变为原来的1/2
	for(i=0; i<dimensionNum; i++)
	{
		//分别获取剩下的一半和去掉的一半
		vector<vector<int>> remainedDimensionValues;
		vector<vector<int>> removedDimensionValues;
		vector<int> remainedTemp;
		vector<int> removedTemp;

		int ithDimensionValueNum = dimensionValues.at(i).size();
		//列举这一维可能出现的所有情况并记录下来
		switch(ithDimensionValueNum)
		{
		case 4://这一维是1,2,3,4的情况
			for(int m=1; m<5; m++)
			{
				remainedTemp.clear();
				removedTemp.clear();
				remainedTemp.push_back(m_commonFuncs.boundValue(m,4));
				remainedTemp.push_back(m_commonFuncs.boundValue(m+1,4));
				removedTemp.push_back(m_commonFuncs.boundValue(m+2,4));
				removedTemp.push_back(m_commonFuncs.boundValue(m+3,4));
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

		//针对每一种可能出现的情况，将余下的再次缩减
		for(j=0; j<removedDimensionValues.size(); j++)
		{
			//获取去掉的那一半的点，针对这一种去法有多种缩减
			vector<CCell*> removedCells;
			vector<vector<int>> tempDimensionValue;
			//获取被去掉的一半的点
			m_commonFuncs.getTempDimensionValue(dimensionValues,i,removedDimensionValues.at(j),tempDimensionValue);
			m_commonFuncs.ListAllCells(0,tempDimensionValue,removedCells);
			m_commonFuncs.kickOutCells(removedCells,allOTPs);//去掉的一半中不能选择OTP
			/*
			*
			*去掉OTP之后进行判断：
			*1)如果removedCells中还有点，则说明此shrink是一个有效shrink，不再找扩充的点。
			*2)如果removedCells为空集，则需要继续找扩充的点
			*/
			//有剩余点
			if(removedCells.size() > 0)
			{//在LIORF的测试用例中检测
				//一行
				/*vector<CCell*> matrixRow;
				for(int m=0; m<removedCells.size(); m++)
				{
					matrixRow.push_back(removedCells.at(m));
				}
				matrixCells.push_back(matrixRow);*/
				continue;
			}

			//获取剩下的一半的dimension值
			vector<vector<int>> tempRemainedDimensionValue;
			m_commonFuncs.getTempDimensionValue(dimensionValues,i,remainedDimensionValues.at(j),tempRemainedDimensionValue);

			for(k=0; k<dimensionNum; k++)
			{
				vector<vector<int>> remainedDimensionValues1;
				vector<vector<int>> removedDimensionValues1;
				vector<int> remainedTemp1;
				vector<int> removedTemp1;

				int kthRemainedDimensionValueNum = tempRemainedDimensionValue.at(k).size();
				//列举这一维可能出现的所有情况并记录下来
				switch(kthRemainedDimensionValueNum)
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
					remainedTemp1.push_back(tempRemainedDimensionValue.at(k).at(0));
					removedTemp1.push_back(tempRemainedDimensionValue.at(k).at(1));
					remainedDimensionValues1.push_back(remainedTemp1);
					removedDimensionValues1.push_back(removedTemp1);

					remainedTemp1.clear();
					removedTemp1.clear();
					remainedTemp1.push_back(tempRemainedDimensionValue.at(k).at(1));
					removedTemp1.push_back(tempRemainedDimensionValue.at(k).at(0));
					remainedDimensionValues1.push_back(remainedTemp1);
					removedDimensionValues1.push_back(removedTemp1);
					break;
				case 1://这一维只有一个值，不能进行缩减
					break;
				}

				for(l=0; l<removedDimensionValues1.size(); l++)
				{
					//获取去掉的那一半的点，针对这一种去法有多种扩充
					vector<CCell*> removedCells1;
					vector<vector<int>> tempDimensionValue1;

					//获取被去掉的一半的点
					m_commonFuncs.getTempDimensionValue(tempRemainedDimensionValue,k,removedDimensionValues1.at(l),tempDimensionValue1);
					m_commonFuncs.ListAllCells(0,tempDimensionValue1,removedCells1);
					m_commonFuncs.kickOutCells(removedCells1,allOTPs);//去掉的一半中不能选择OTP

					/*
					*
					*去掉OTP之后进行判断：
					*1)如果removedCells中还有点，则说明此shrink是一个有效shrink，不再找扩充的点。
					*2)如果removedCells为空集，则需要继续找扩充的点
					*/
					//有剩余点
					if(removedCells1.size() > 0)
					{
						//一行
						vector<CCell*> matrixRow;
						for(int m=0; m<removedCells1.size(); m++)
						{
							matrixRow.push_back(removedCells1.at(m));
						}
						matrixCells.push_back(matrixRow);
						continue;
					}

					//获取剩下的一半的dimension值
					vector<vector<int>> tempRemainedDimensionValue1;
					m_commonFuncs.getTempDimensionValue(tempRemainedDimensionValue,k,remainedDimensionValues1.at(l),tempRemainedDimensionValue1);
					//针对这种去法，列出所有可能的扩充
					vector<vector<CCell*>> expandCells;
					int literalN=0;
					for(n=0; n<dimensionNum; n++)
					{
						if(i==2&&j==2&&k==2&&l==1&&n==2){
							flag=1;

						}
						vector<CCell*> expandCell;
						vector<vector<int>> tempExpandDimensionValue;
						vector<int> expandDimensionValue;
						int biggestValue = biggestDimensionValueAllowed.at(n);//该维允许的最大值（2或4）
						int kthRemainedDimensionValueNum1 = tempRemainedDimensionValue1.at(n).size();
						switch(kthRemainedDimensionValueNum1)
						{
						case 4://这种情况下这一维不能再扩充了
							break;
						case 2://这种情况下扩充的是剩下的两个，比如原来为1,2，扩充部分就为3,4
							if((n!=k&&biggestValue==4)/*||(n!=i&&biggestValue==4)*/)//1.n==k时expand之后和shrink之前是一样的;2.如果该维的最大取值只到2则也不能进行expand
							{
								int num1 = tempRemainedDimensionValue1.at(n).at(0);
								int num2 = tempRemainedDimensionValue1.at(n).at(1);
								if((num1==1&&num2==4) || (num1==4&&num2==1))
								{
									expandDimensionValue.push_back(2);
									expandDimensionValue.push_back(3);
								}
								else
								{
									int minNum = num1<num2? num1:num2;
									expandDimensionValue.push_back(m_commonFuncs.boundValue(minNum+2,biggestValue));
									expandDimensionValue.push_back(m_commonFuncs.boundValue(minNum+3,biggestValue));
								}
								m_commonFuncs.getTempDimensionValue(tempRemainedDimensionValue1,n,expandDimensionValue,tempExpandDimensionValue);
								m_commonFuncs.ListAllCells(0,tempExpandDimensionValue,expandCell);
								m_commonFuncs.kickOutCells(expandCell,allOTPs,allUTPs);//expand的点不能是TP
								if(expandCell.size()>0){
									expandCells.push_back(expandCell);
									literalNum.push_back(literalN);
								}
								literalN++;//
							}
							break;
						case 1://这种情况下，有两种扩充方式，比如原来为1，扩充部分可以是4或2，
							//但是得考虑n==i时，expand之后可能和shrink之前的一样
							int num = tempRemainedDimensionValue1.at(n).at(0);
							bool noPlus1 = false;
							bool noMinus1 = false;
							if(n==k)
							{

								if(biggestValue==2)//这个时候不能进行expand
								{
									noPlus1=true;
									noMinus1=true;
								}
								else
								{
									int anotherNum = removedDimensionValues1.at(l).at(0);
									if(m_commonFuncs.boundValue(num+1,biggestValue)==anotherNum)
										noPlus1 = true;
									if(m_commonFuncs.boundValue(num-1,biggestValue)==anotherNum)
										noMinus1 = true;
								}
							}
							else 
							{
								if(biggestValue==2)//这个时候只能单方向expand
								{
									if(num==0)//如果该维最大值是2，那么num只可能是0或1
										noMinus1=true;
									else
										noPlus1=true;
								}
							}
							if(!noPlus1)
							{
								expandDimensionValue.clear();
								tempExpandDimensionValue.clear();
								expandCell.clear();
								expandDimensionValue.push_back(m_commonFuncs.boundValue(num+1,biggestValue));
								m_commonFuncs.getTempDimensionValue(tempRemainedDimensionValue1,n,expandDimensionValue,tempExpandDimensionValue);
								m_commonFuncs.ListAllCells(0,tempExpandDimensionValue,expandCell);
								m_commonFuncs.kickOutCells(expandCell,allOTPs,allUTPs);//expand的点不能是TP
								if(expandCell.size()>0){
									expandCells.push_back(expandCell);
									literalNum.push_back(literalN);
								}
								literalN++;//
							}
							if(!noMinus1)
							{
								expandDimensionValue.clear();
								tempExpandDimensionValue.clear();
								expandCell.clear();
								expandDimensionValue.push_back(m_commonFuncs.boundValue(num-1,biggestValue));
								m_commonFuncs.getTempDimensionValue(tempRemainedDimensionValue1,n,expandDimensionValue,tempExpandDimensionValue);
								m_commonFuncs.ListAllCells(0,tempExpandDimensionValue,expandCell);
								m_commonFuncs.kickOutCells(expandCell,allOTPs,allUTPs);//expand的点不能是TP
								if(expandCell.size()>0){
									expandCells.push_back(expandCell);
									literalNum.push_back(literalN);
								}
								literalN++;//
							}
							break;
						}
					}
					//已经获取所有的expand cells，将去掉的一半和对应的扩充的一半合并组成矩阵中的一行
					for(int m=0; m<expandCells.size(); m++)
					{
						vector<CCell*> matrixRow;
						for(int a=0; a<removedCells.size(); a++)
							matrixRow.push_back(removedCells.at(a));
						for(int b=0; b<expandCells.at(m).size(); b++)
							matrixRow.push_back(expandCells.at(m).at(b));
						if (param!=1)
							matrixCells.push_back(matrixRow);
					}
				}
			}
		}
	}
	return "";
}

