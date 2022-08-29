#include "stdafx.h"
#include "Utility.h"
#include "FaultDetection.h"
#include "CommonFuncs.h"
#include "BDD.h"
#include "GLPK_Solve.h"
#include "MatrixPartition.h"
#include<algorithm>
#include "public.h"

#include<iostream>

FaultDetection::FaultDetection(void)
{
}


FaultDetection::~FaultDetection(void)
{
}

string FaultDetection::checkExprwithSet(
	string exprOri/*原表达式*/,
	string expr/*变体*/,
	vector<string> testset/*测试用例*/,
	vector<vector<int>> case_mutant/*注意，这个不是约束矩阵，是其置转的系数位置标号，这样其一行就对应一个格覆盖*/,
	vector<vector<int>>finalCoefficientMatrix/*变体对应的约束矩阵方程*/,
	vector<int>finalTModelIndex/*所有测试用例对应的格覆盖*/,
	vector<string>& mutants_DP/*变体索引，找到对应的变体所使用的索引*/,
	bool vOdd)
{
	GLPK_Solve glpk_Solve;
	CCommonFuncs commonFuncs;

	//1.缺陷生成测试用例
	vector<vector<int>> CoefficientMatrix(finalCoefficientMatrix);
	vector<int> TModelIndex(finalTModelIndex);/*拷贝所有测试用例对应的格覆盖*/
	//2.检测待测表达式
	vector<int> validTCsol;
	vector<int> invalidTCsol;
	//用测试用例检测待测表达式，如果都有效，则没有发生缺陷
	if (checkTestSet(testset/*测试用例*/, exprOri/*原表达式*/, expr/*变体*/, 
		validTCsol/*不一致节点集合，为有效测试点*/, invalidTCsol/*一致节点集合，为无效测试点*/, vOdd))
	{//返回真，说明全是无效测试点
		return "no fault";
	}
	//用测试用例检测待测表达式，存在无效测试用例，则存在缺陷
	else
	{//返回假，说明有有效测试点
		vector<int> p_mutants;
		vector<int> u_mutants;
		sort(invalidTCsol.begin(),invalidTCsol.end());
		vector<vector<int>> CoefficientMatrix_mid;
		vector<int> invalid_index;
		//vector<int> valid_index;
		//printf("enter FaultDetection While\n");
		while(true)
		{
			invalid_index.clear();
			/*printf("TModelIndex：\n");
			for (int j = 0; j < TModelIndex.size(); j++)
			{
				printf("%d ", TModelIndex[j]);
			}
			printf("\nend TModelIndex\r\n");
			printf("invalidTCsol：\n");
			for (int j = 0; j < invalidTCsol.size(); j++)
			{
				printf("%d ", invalidTCsol[j]);
			}
			printf("\nend invalidTCsol\r\n");
			printf("case_mutant：\n");
			for (int j = 0; j < case_mutant.size(); j++)
			{
				for (int l = 0; l < case_mutant[j].size(); l++)
				{
					printf("%d ", case_mutant[j][l]);
				}
				printf("\n");
			}
			printf("\nend case_mutant\r\n");*/
			//求交集
			for(int i=0;i<invalidTCsol.size();)
			{
				for(int j=0;j<TModelIndex.size()&&i<invalidTCsol.size();j++)
				{
					if(invalidTCsol[i]==TModelIndex[j])
					{//在集合中找到不符合等值条件的测试用例节点
						invalid_index.push_back(j);//在所有测试用例中找到对应格覆盖之后记录索引
						if(p_mutants.empty())
						{
							p_mutants=case_mutant[j];//获取拥有该格覆盖的所有系数矩阵的标号
						}
						else
						{
							vector<int> t_mutants2;
							set_intersection(/*求交集*/
								p_mutants.begin(),/*现有集合起点*/
								p_mutants.end(),/*现有集合终点*/
								case_mutant[j].begin(),/*变体对应的约束矩阵系数起点*/
								case_mutant[j].end(),/*变体对应的约束矩阵系数终点*/
								inserter(
									t_mutants2, 
									t_mutants2.end()
								)/*封装的std插入集合函数，第一个参数为集合空间，第二个参数为插入位置*/
							);//封装的std求集合交集函数
							p_mutants.swap(t_mutants2);/*将所求到集合与集合交换*/
							vector<int>().swap(t_mutants2);/*清空临时集合*/
						}
						if(p_mutants.size()==1)
							break;//如果集合只有一条记录则退出，这是为什么呢？因为这就意味着找到了唯一的变体约束矩阵
						i++;
					}
				}
				if(p_mutants.size()==1)
					break;
			}//所谓求交集，就是包含所有待测格覆盖的约束矩阵，这是因为置转了
			printf("p_mutants1：\n");//p_mutants就是方程的标号
			for (int j = 0; j < p_mutants.size(); j++)
			{
				printf("%d ", p_mutants[j]);
			}
			printf("\nend p_mutants1\r\n");
			printf("求交集结束\n");
			//求并集
			/*printf("validTCsol：\n");
			for (int j = 0; j < validTCsol.size(); j++)
			{
				printf("%d ", validTCsol[j]);
			}
			printf("\nend validTCsol\r\n");*/
			for(int i=0;i<validTCsol.size()&&p_mutants.size()>1;)
			{
				for(int j=0;j<TModelIndex.size()&&i<validTCsol.size();j++)
				{
					if(validTCsol[i]==TModelIndex[j])
					{
						//valid_index.push_back(j);
						vector<int> t_mutants;
						set_difference(//求差集
							p_mutants.begin(),
							p_mutants.end(),
							case_mutant[j].begin(),
							case_mutant[j].end(),
							inserter(
								t_mutants, 
								t_mutants.end()
							)
						);
						p_mutants.swap(t_mutants);
						vector<int>().swap(t_mutants); 
						if(p_mutants.size()==1)
							break;
						i++;
					}
				}
				if(p_mutants.size()==1)
					break;
			}//所谓求差集，其实就是说，如果求交集得到的结果不唯一，则要把那些包含"一致节点"的约束矩阵剔除
			printf("p_mutants2：\n");
			for (int j = 0; j < p_mutants.size(); j++)
			{
				printf("%d ", p_mutants[j]);
			}
			printf("\nend p_mutants2\r\n");
			printf("求并集结束\n");
			//由此可以推断上述代码的逻辑
			//首先获取待测点（也就是两个表达式不一致的点）和非待测点（也就是两个表达式一致的点）
			//将所有约束矩阵包含所有不一致的方程归并到集合A（使用的是置转法），如果A中包含了一致的点，则要剔除
			//A+(S-B)就是可能需要判断的集合点，下次迭代的时候就使用此集合作为全集S进行相同的处理
			if(p_mutants.size()==1)
				break;

			//以下代码就是说，如果还是得不到唯一的约束方程，则如下操作，并迭代
			//生成矩阵
			CoefficientMatrix_mid.clear(); 
			removeAllOneCols(CoefficientMatrix,p_mutants);
			for(int i=0;i<p_mutants.size();i++)
			{//p_mutants现在就成了待排除方程的索引标号
				bool zero=true;
				for(int j=0;j<CoefficientMatrix[p_mutants[i]].size();j++)
				{//CoefficientMatrix就是约束矩阵，所以CoefficientMatrix[p_mutants[i]]就是待测约束矩阵
					vector<int>::iterator it = find( invalid_index.begin(), invalid_index.end(), j); //查找约束矩阵中各个节点是否有已经校验过的点
					if(it!=invalid_index.end())
					{//如果相等，则表示没有查到，不相等，则表示查到了，说明矩阵中有已经校验过的点，修改约束矩阵中该节点的值为0，用于标示剔除了该节点（格覆盖）
						CoefficientMatrix[p_mutants[i]][j]=0;
					}
					if(CoefficientMatrix[p_mutants[i]][j]!=0)
					{//没有查到或者不是校验过的点
						zero=false;
						/*bool same=true;
						for(int ii=0;ii<p_mutants.size();ii++)
						{
							if(CoefficientMatrix[ii][j]==0)
							{
								same=false;
							}
						}*///无用代码
					}

				}
				if(!zero)
				{
					CoefficientMatrix_mid.push_back(CoefficientMatrix[p_mutants[i]]);//将新得到的约束方程插入到一个临时空间中
				}
			}
			printf("生成矩阵结束\n"); 
			if(CoefficientMatrix_mid.empty())
				break;
			//2.2.生成测试用例
			vector<vector<int>> CoefficientMatrix_temp;
			vector<int> index_temp;
			removeAllZeroCols(CoefficientMatrix_mid, TModelIndex, CoefficientMatrix_temp, index_temp);
			CoefficientMatrix_mid.clear();
			removeZeroRow(CoefficientMatrix_temp, CoefficientMatrix_mid);
			if(CoefficientMatrix_mid.empty())
				break;
			if(CoefficientMatrix_mid.size()==p_mutants.size()&&index_temp.size()==1)
				break;
			else
			{
				printf("求解结束，并没有得到唯一结果\n");
				//testset = testCaseObj.genTestCaseWithGLPK(CoefficientMatrix_mid, index_temp);
				//代替上面屏蔽的一句
				vector<int> colSelected;
				glpk_Solve.run(colSelected, &CoefficientMatrix_mid);//重新解这个临时的约束矩阵
				//将得到的格覆盖转换成测试用例
				//printf("tModelIndexAppeared colSelected Print:\n");
				testset.clear();
				for (int j = 0; j < colSelected.size(); j++)
				{
					char tmp[40];
					sprintf(tmp, "%d", index_temp[colSelected[j]]);
					string inde = "";
					inde.append(tmp);
					string point01Str = commonFuncs.transTModelIndexStrTo01Str(inde, vOdd);
					testset.push_back(point01Str);
					//printf("%s\n", point01Str.c_str());
				}

				printf("\n");
				//代替上面屏蔽的一句结束
				//2.3.检测待测表达式
				validTCsol.clear();
				invalidTCsol.clear();
				checkTestSet(testset,exprOri,expr,validTCsol,invalidTCsol,vOdd);//继续迭代
			}
			printf("生成测试用例结束\n");
		}
		//生成剩下的一行对应的变体
		if (p_mutants.size() == 0){//如果一个都没有，则直接返回一个错误结果即可
			return mutants_DP[0];
		}
		int mutant_index=p_mutants[0];
		printf("%s %d mutant_index:%d\n", __FILE__, __LINE__, mutant_index);
		printf("得到的结果[%d]:%s\n", mutant_index, mutants_DP[mutant_index].c_str());
		return mutants_DP[mutant_index];
	}
}
//这个检查的逻辑是什么呢？
bool FaultDetection::checkTestSet(
	vector<string> testset/*生成的测试用例集合，如果测试用例全部通过，则表示没有问题*/, 
	string exprOri/*原表达式*/, 
	string expr/*变体*/, 
	vector<int>& validTCsol, /*结果不一致的节点，为有效测试点*/
	vector<int>& invalidTCsol/*结果一致的节点，为无效测试点*/, 
	bool vOdd)
{
	Utility uti;
	CCommonFuncs CCommonFuncs;

	string oriexp = exprOri;
	string faultexp = expr;
	printf("checkTestSet testset：\n");
	for (int i = 0; i < testset.size();i++){
		printf("testset[%d]:%s\n",i,testset[i].c_str());
	}
	printf("checkTestSet testset end\n");

	printf("exprOri:%s\n", exprOri.c_str());
	printf("expr:%s\n", expr.c_str());
	for(int i=0;i<testset.size();i++)
	{
		string testcase = testset[i];
		printf("testcase:%s\n", testcase.c_str());
		printf("oriexp testcase evaluate:%d\n", uti.evaluate(oriexp, testcase));//测试用例在此表达式中是否为真
		printf("oriexp faultexp testcase evaluateMutant:%d\n", uti.evaluateMutant(oriexp, faultexp, testcase));//测试用例在此表达式中是否为真
		bool v=(uti.evaluate(oriexp,testcase))^(uti.evaluateMutant(oriexp,faultexp,testcase));//两个值相同则为假，不同则为真,不过这两个资源好浪费时间呀，明明有更优的算法
		printf("v:%d\n", v);
		if(v){
			invalidTCsol.push_back(CCommonFuncs.getTModelIndexOf01Str(testset[i],vOdd));
		}
		else
		{
			validTCsol.push_back(CCommonFuncs.getTModelIndexOf01Str(testset[i],vOdd));
		}
	}
	if(invalidTCsol.size()==0)
		return true;
	else
		return false;
}


//
void FaultDetection::checkTestSet1(
	vector<string> testset/*生成的测试用例集合，如果测试用例全部通过，则表示没有问题*/,
	string exprOri/*原表达式*/,
	string expr/*变体*/,
	vector<string>& validTCsol, /*结果不一致的节点，为有效测试点*/
	vector<string>& invalidTCsol/*结果一致的节点，为无效测试点*/)
{
	Utility uti;
	CCommonFuncs CCommonFuncs;

	string oriexp = exprOri;
	string faultexp = expr;
	
	for (int i = 0; i<testset.size(); i++)
	{
		string testcase = testset[i];

		bool v = ((uti.evaluate(oriexp, testcase)) == (uti.evaluateMutant(oriexp, faultexp, testcase)))?false:true;//两个值相同则为假，不同则为真,不过这两个资源好浪费时间呀，明明有更优的算法
		
		if (v == false){
			invalidTCsol.push_back(testcase);
		}
		else
		{
			validTCsol.push_back(testcase);
		}
	}

}





int FaultDetection::updateMatrix
	(
	vector<vector<int>>& CoefficientMatrix,vector<int>& TModelIndex, 
	vector<int> validTCsol,vector<int> invalidTCsol,vector<int>& constrain_index,vector<bool>& mutantsremained)
{
	vector<vector<int>> temp_CoefficientMatrix;
	vector<vector<int>> old_CoefficientMatrix(CoefficientMatrix);
	vector<int> temp_TModelIndex;
	vector<bool> temp_mutantsremained;
	vector<int> temp_constrain_index;
	vector<bool> allzero_cow;
	for(int i=0;i<CoefficientMatrix.size()&&CoefficientMatrix.size()>0;i++)
	{
		bool allzero = true;
		for(int j=0;j<CoefficientMatrix[i].size();j++)
		{
			if(CoefficientMatrix[i][j]==1)
			{
				//判断这个点是不是valid的点
				vector<int>::iterator valid = find( validTCsol.begin(), validTCsol.end(), TModelIndex[j]); 	
				//是有效的点，不会发生这个缺陷
				if(valid!=validTCsol.end())
				{
					mutantsremained[constrain_index[i]]=false;//表示不会发生这个缺陷
					break;
				}
				//判断这个点是不是无效的点
				vector<int>::iterator invalid = find( invalidTCsol.begin(), invalidTCsol.end(), TModelIndex[j]); 
				//是无效的点，把系数改为0
				if (invalid!=invalidTCsol.end( ))
				{
					if(mutantsremained[constrain_index[i]])
						mutantsremained[constrain_index[i]]=true;//表示可能发生这个缺陷
					for(int k=0;k<CoefficientMatrix.size();k++)
					{
						if(old_CoefficientMatrix[k][j]==0)
						{
							mutantsremained[constrain_index[k]]=false;
						}
					}
					CoefficientMatrix[i][j]=0;
				}
			}
			else
			{
				//判断这个点是不是无效的点
				vector<int>::iterator invalid = find( invalidTCsol.begin(), invalidTCsol.end(), TModelIndex[j]); 
				//是无效的点，把系数改为0
				if (invalid!=invalidTCsol.end( ))
				{
					mutantsremained[constrain_index[i]]=false;//表示不可能发生这个缺陷
				}
			}
		}
		for(int j=0;j<CoefficientMatrix[i].size();j++)
		{
			if(CoefficientMatrix[i][j]==1)
			{
				allzero=false;
			}
		}
		allzero_cow.push_back(allzero);
	}
	for(int i=0;i<CoefficientMatrix.size();i++)
	{
		if(mutantsremained[constrain_index[i]]/*&&!allzero_cow[i]*/)
		{
			temp_CoefficientMatrix.push_back(CoefficientMatrix[i]);
			temp_constrain_index.push_back(constrain_index[i]);
		}
	}

	constrain_index.swap(temp_constrain_index);
	temp_TModelIndex = TModelIndex;
	CoefficientMatrix.clear();
	TModelIndex.clear();
	for(int i=0; i<temp_CoefficientMatrix.size(); i++)
	{
		vector<int> coefficient;
		CoefficientMatrix.push_back(coefficient);
	}

	for(int i=0; i<temp_TModelIndex.size(); i++)
	{
		//查看该列的每一行是不是都为0
		int j=0;
		int lineNum = temp_CoefficientMatrix.size();
		for(; j<lineNum; j++)
		{
			if(temp_CoefficientMatrix.at(j).at(i)==1)
				break;
		}
		if(j<lineNum)//该列不为全0，加到finalMatrix中
		{
			for(int m=0; m<lineNum; m++)
				CoefficientMatrix.at(m).push_back(temp_CoefficientMatrix.at(m).at(i));

			TModelIndex.push_back(temp_TModelIndex.at(i));
		}
	}
	int mutantcount = 0;
	for (int i = 0; i < mutantsremained.size(); i++)
	{
		if(mutantsremained[i])
		{
			mutantcount+=1;
		}
	}
	return mutantcount;
}
bool FaultDetection::checkEqualment(string str1, string str2)
{
	BDD string1(str1);
	BDD string2(str2);

	return string1.isEquivalent(string2);
}

int FaultDetection::removeZeroRow(vector<vector<int>> old_CoefficientMatrix,vector<vector<int>>& new_CoefficientMatrix)
{
	
	for(int i=0; i<old_CoefficientMatrix.size();i++)
	{
		bool allzero = true;
		for (int j= 0; j < old_CoefficientMatrix[i].size(); j++)
		{
			if(old_CoefficientMatrix[i][j]==1)
			{
				allzero=false;
				break;
			}
		}
		if(!allzero)
		{
			new_CoefficientMatrix.push_back(old_CoefficientMatrix[i]);
		}
	}
	return new_CoefficientMatrix.size();
}

void removeAllOneCols(vector<vector<int>>& coefficientMatrix,vector<int> row_index)
{
	vector<int> allOneCol;
	bool have_valid_col=false;
	for(int i=0; i<coefficientMatrix[0].size(); i++)
	{
		//查看该列的每一行是不是都为0
		int j=0;
		int lineNum = row_index.size();
		int oneCount=0;
		for(; j<lineNum; j++)
		{
			if(coefficientMatrix.at(row_index[j]).at(i)==1)
				oneCount++;
		}
		if(oneCount==lineNum)//该列全为1
		{
			allOneCol.push_back(i);
		}
		else
		{
			if(oneCount>0)
			{
				have_valid_col=true;
			}
		}
	}

	int col=have_valid_col?0:1;
	for(;col<allOneCol.size();col++)
	{
		for(int m=0;m<row_index.size();m++)
		{
			coefficientMatrix[row_index[m]][allOneCol[col]]=0;
		}
	}
}

bool FaultDetection::checkTestCase(int testcase_Index, string exprOri, string expr, bool vOdd)
{
	Utility uti;

	string oriexp = exprOri;
	string faultexp = expr;
	string testcasetemp;
	testcasetemp.append("%d",testcase_Index); 

	string testcase = testcasetemp;

	bool result=(uti.evaluate(oriexp,testcase))^(uti.evaluateMutant(oriexp,faultexp,testcase));

	return result;
}