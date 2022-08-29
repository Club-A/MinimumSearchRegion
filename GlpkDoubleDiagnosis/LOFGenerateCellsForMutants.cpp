#include "StdAfx.h"
#include "LOFGenerateCellsForMutants.h"

string LOFGenerateCellsForMutants(
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
	vector<vector<int>> dimensionValues;
	vector<int> biggestDimensionValueAllowed;

	commonFuncs.getDimensionValue(dimensionValuesStr, vOdd, dimensionValues, biggestDimensionValueAllowed);
	int totalLOFliteralsNum = 0;
	int dimensionNum = dimensionValues.size();
	vector<vector<CCell*>> expandCells;
	//对每一维进行放大一倍，并取放大部分的点
	for(int i=0; i<dimensionNum; i++)
	{
		vector<CCell*> expandCell;
		vector<vector<int>> tempExpandDimensionValue;
		vector<int> expandDimensionValue;
		int biggestValue = biggestDimensionValueAllowed.at(i);//该维允许的最大值（2或4）
		int ithDimensionNum = dimensionValues.at(i).size();
		switch(ithDimensionNum)
		{
		case 4: //此种情况不能进行扩充
			break;
		case 2: //此种情况扩充为4
			if(biggestValue==4)//如果等于2则不能扩充
			{
				totalLOFliteralsNum += 1;
				int num1 = dimensionValues.at(i).at(0);
				int num2 = dimensionValues.at(i).at(1);
				if((num1==1&&num2==4) || (num1==4&&num2==1))
				{
					expandDimensionValue.push_back(2);
					expandDimensionValue.push_back(3);
				}
				else
				{
					int minNum = num1<num2? num1:num2;
					expandDimensionValue.push_back(commonFuncs.boundValue(minNum + 2, biggestValue));
					expandDimensionValue.push_back(commonFuncs.boundValue(minNum + 3, biggestValue));
				}
				commonFuncs.getTempDimensionValue(dimensionValues, i, expandDimensionValue, tempExpandDimensionValue);
				/*printf("---------------tempShrinkDimensionValue start--------------\n");
				for (int z = 0; z<tempExpandDimensionValue.size(); z++){
					for (int y = 0; y < tempExpandDimensionValue[z].size(); y++){
						printf("%d ", tempExpandDimensionValue[z][y]);
					}
					printf("\n");
				}
				printf("\n");
				printf("---------------tempShrinkDimensionValue end--------------\n");*/
				commonFuncs.ListAllCells(0, tempExpandDimensionValue, expandCell);
				commonFuncs.kickOutCells(expandCell, allOTPs, allUTPs);//expand的点不能是TP
				if(expandCell.size()>0)
					expandCells.push_back(expandCell);

				// 当前的mutant,(用一个项来表示，这个项各个维的取值存在mutantsDimensionValue中)							
				vector<vector<int>> RemainedDimensionValue;
				RemainedDimensionValue.insert(RemainedDimensionValue.begin(),dimensionValues.begin(),dimensionValues.end());
				RemainedDimensionValue[i].insert(RemainedDimensionValue[i].begin(),expandDimensionValue.begin(),expandDimensionValue.end());
				mutantsDimensionValue.push_back(RemainedDimensionValue);
				//把当前mutant对应的测试用例集收集到corresponding_tests中
				corresponding_tests.push_back(expandCell);
			}
			break;
		case 1://这种情况下，有两种扩充方式，比如原来为1，扩充部分可以是4或2
			int num = dimensionValues.at(i).at(0);
			bool noPlus1=false, noMinus1=false;
			if(biggestValue==2)//这种情况只能往一边扩充，num的值非1即2
			{
				if(num==1)
					noMinus1 = true;
				else
					noPlus1 = true;
			}
			
			if(!noPlus1)
			{
				expandDimensionValue.push_back(commonFuncs.boundValue(num + 1, biggestValue));
				commonFuncs.getTempDimensionValue(dimensionValues, i, expandDimensionValue, tempExpandDimensionValue);
				/*printf("---------------tempShrinkDimensionValue start--------------\n");
				for (int z = 0; z<tempExpandDimensionValue.size(); z++){
					for (int y = 0; y < tempExpandDimensionValue[z].size(); y++){
						printf("%d ", tempExpandDimensionValue[z][y]);
					}
					printf("\n");
				}
				printf("\n");
				printf("---------------tempShrinkDimensionValue end--------------\n");*/
				commonFuncs.ListAllCells(0, tempExpandDimensionValue, expandCell);
				commonFuncs.kickOutCells(expandCell, allOTPs, allUTPs);//expand的点不能是TP
				if(expandCell.size() > 0 )
				{
					totalLOFliteralsNum += 1;
					expandCells.push_back(expandCell);
				}

				// 当前的mutant,(用一个项来表示，这个项各个维的取值存在mutantsDimensionValue中)							
				vector<vector<int>> RemainedDimensionValue;
				RemainedDimensionValue.insert(RemainedDimensionValue.begin(), dimensionValues.begin(), dimensionValues.end());
				RemainedDimensionValue[i].insert(RemainedDimensionValue[i].begin(), expandDimensionValue.begin(), expandDimensionValue.end());
				mutantsDimensionValue.push_back(RemainedDimensionValue);
				//把当前mutant对应的测试用例集收集到corresponding_tests中
				corresponding_tests.push_back(expandCell);
			}

			if(!noMinus1)
			{
				expandDimensionValue.clear();
				tempExpandDimensionValue.clear();
				expandCell.clear();
				expandDimensionValue.push_back(commonFuncs.boundValue(num - 1, biggestValue));
				commonFuncs.getTempDimensionValue(dimensionValues, i, expandDimensionValue, tempExpandDimensionValue);
				/*printf("---------------tempShrinkDimensionValue start--------------\n");
				for (int z = 0; z<tempExpandDimensionValue.size(); z++){
					for (int y = 0; y < tempExpandDimensionValue[z].size(); y++){
						printf("%d ", tempExpandDimensionValue[z][y]);
					}
					printf("\n");
				}
				printf("\n");
				printf("---------------tempShrinkDimensionValue end--------------\n");*/
				commonFuncs.ListAllCells(0, tempExpandDimensionValue, expandCell);
				commonFuncs.kickOutCells(expandCell, allOTPs, allUTPs);//expand的点不能是TP
				if(expandCell.size() > 0)
				{
					expandCells.push_back(expandCell);
					totalLOFliteralsNum += 1;
				}

				// 当前的mutant,(用一个项来表示，这个项各个维的取值存在mutantsDimensionValue中)							
				vector<vector<int>> RemainedDimensionValue;
				RemainedDimensionValue.insert(RemainedDimensionValue.begin(), dimensionValues.begin(), dimensionValues.end());
				RemainedDimensionValue[i].insert(RemainedDimensionValue[i].begin(), expandDimensionValue.begin(), expandDimensionValue.end());
				mutantsDimensionValue.push_back(RemainedDimensionValue);
				//把当前mutant对应的测试用例集收集到corresponding_tests中
				corresponding_tests.push_back(expandCell);
			}
			break;
		}
	}

	//已经获取所有的expand cells，将所有可能的扩充组成一个矩阵，一行表示一种扩充
	for(int i=0; i<expandCells.size(); i++)
	{
		vector<CCell*> matrixRow;
		for(int j=0; j<expandCells.at(i).size(); j++)
			matrixRow.push_back(expandCells.at(i).at(j));
		matrixCells.push_back(matrixRow);
	}

	return "";
}
