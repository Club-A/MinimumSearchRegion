#include "stdafx.h"
#include "public.h"
//将可能的F节点摘出
void getNeedFValues(vector<vector<int>> DimensionValues, vector<vector<int>> &FValues)
{
	//方法，只需要将DimensionValues中没有的数字赋值给FValues即可
	for (int i = 0; i < DimensionValues.size(); i++){
		vector<int> tmpF;
		if (DimensionValues[i].size() == 4){
			for (int j = 0; j < 4; j++){
				tmpF.push_back(DimensionValues[i][j]);
			}
		}
		else{
			for (int j = 0, k = 0, l = 1; j < 4 && l <= 4; l++){
				if (k >= DimensionValues[i].size()){
					tmpF.push_back(l);
					j++;
				}
				else if (l != DimensionValues[i][k]){
					tmpF.push_back(l);
					j++;
				}
				else{
					k++;
				}
			}
		}
		FValues.push_back(tmpF);
	}
}
void removeAllZeroCols(vector<vector<int>> coefficientMatrix, vector<int> tModelIndexAppeared, vector<vector<int>>& finalCoefficientMatrix, vector<int>& finalTModelIndex)
{
	for (int i = 0; i<coefficientMatrix.size(); i++)
	{
		vector<int> coefficient;
		finalCoefficientMatrix.push_back(coefficient);
	}

	for (int i = 0; i<tModelIndexAppeared.size(); i++)
	{
		//查看该列的每一行是不是都为0
		int j = 0;
		int lineNum = coefficientMatrix.size();
		for (; j<lineNum; j++)
		{
			if (coefficientMatrix.at(j).at(i) == 1)
				break;
		}
		if (j<lineNum)//该列不为全0，加到finalMatrix中
		{
			for (int m = 0; m<lineNum; m++)
				finalCoefficientMatrix.at(m).push_back(coefficientMatrix.at(m).at(i));

			finalTModelIndex.push_back(tModelIndexAppeared.at(i));
		}
	}
}

vector<string> genMutants(vector<vector<vector<int>>>& mutantsDimensionValue, bool vOdd, string model)
{
	std::vector<string> mutants;
	//这里反推的原理是什么？
	// for every mutant，因为一个term变化可对应多个变体
	for (int i = 0; i < mutantsDimensionValue.size(); i++)
	{
		string s_mutant;
		// for each dim
		for (int j = 0; j < mutantsDimensionValue[i].size(); j++)
		{

			int x = (0 + 2 * j), y = (1 + 2 * j);
			unsigned current_dim_values_count = mutantsDimensionValue[i][j].size();
			int t1 = mutantsDimensionValue[i][j][0];
			int t2 = mutantsDimensionValue[i][j][0];
			if (current_dim_values_count == 2)
				t2 = mutantsDimensionValue[i][j][1];
			int magic_int = (t1 + t2)*(t1 - t2);

			// the last single-literal dim
			if (j == mutantsDimensionValue[i].size() - 1 && vOdd)
			{
				switch (current_dim_values_count)
				{
				case 1:
					switch (mutantsDimensionValue[i][j][0])
					{
					case 1:
						//0
						s_mutant += '!';
						s_mutant += model[x];
						break;
					case 2:
						//1
						s_mutant += model[x];
						break;
					default:
						break;
					}
					break;
				case 2:
					break;
				default:
					break;
				}
				break;
			}
			switch (current_dim_values_count)
			{
			case 1:
				switch (mutantsDimensionValue[i][j][0])
				{
				case 1:
					//00
					s_mutant += '!';
					s_mutant += model[x];
					s_mutant += '!';
					s_mutant += model[y];
					break;
				case 2:
					//01
					s_mutant += '!';
					s_mutant += model[x];
					s_mutant += model[y];
					break;
				case 3:
					//11
					s_mutant += model[x];
					s_mutant += model[y];
					break;
				case 4:
					//10
					s_mutant += model[x];
					s_mutant += '!';
					s_mutant += model[y];
					break;
				default:
					break;
				}
				break;
			case 2:

				if (magic_int < 0)
				{
					magic_int = 0 - magic_int;
				}
				switch (magic_int)
				{
				case 3:
					//1 2
					s_mutant += '!';
					s_mutant += model[x];
					break;
				case 5:
					//2 3
					s_mutant += model[y];
					break;
				case 7:
					//3 4
					s_mutant += model[x];
					break;
				case 15:
					//4 1
					s_mutant += '!';
					s_mutant += model[y];
					break;
				default:
					break;
				}
				break;
			case 4:
				break;
			default:
				break;
			}

		}

		mutants.push_back(s_mutant);
	}

	return mutants;
}

void getUnequalMutants(
	vector<string> muts,
	vector<vector<CCell*>> corresponding_tests,
	vector<string> terms,
	int current_term_index,
	vector<string>& mutants)
{
	/*printf("\n%s,%d,terms Print:\n", __FILE__, __LINE__);
	for (int j = 0; j < terms.size(); j++)
	{
		printf("%s ", terms[j].c_str());
	}
	printf("\n%s,%d,end terms Print\r\n", __FILE__, __LINE__);*/
	for (int i = 0; i < muts.size(); i++)
	{
		if (corresponding_tests[i].size()>0){
			string mutant_DP = "";
			for (int j = 0; j < terms.size(); j++)
			{
				if (j != current_term_index)
				{
					mutant_DP.append(terms[j]);
					if (j != terms.size() - 1)
					{
						mutant_DP.append("+");
					}

				}
				else
				{
					if (muts[i].length() != 0)
					{
						mutant_DP.append(muts[i]);
						if (j != terms.size() - 1)
						{
							mutant_DP.append("+");
						}
					}
					else
					{
						mutant_DP = mutant_DP.substr(0, mutant_DP.length() - 1);
						if (j != terms.size() - 1 && j != 0)
						{
							mutant_DP.append("+");
						}
					}

				}
			}
			//printf("\n%s,%d,mutant_DP %s\r\n", __FILE__, __LINE__, mutant_DP.c_str());
			mutants.push_back(mutant_DP);
		}
	}
}

void get_mutant_constrains
(vector<vector<vector<int>>>& mutantsPresentedByDimensionValue,
bool vOdd,
vector<vector<CCell*>> corresponding_tests,
string expression,
int current_term_index,
vector<string>& mutants
)
{//这里为什么使用反推的方式获取变体呢，可以有其他的方式呀？
	Utility utility;
	string oriExp = expression;
	vector<string> terms = utility.extractDNFstyle(oriExp);
	string model = utility.generateModel(oriExp);
	vector<string> muts = genMutants(mutantsPresentedByDimensionValue, vOdd, model);
	/*printf("muts.size:%d\r\n", muts.size());
	for (int l = 0; l < muts.size(); l++){
		printf("muts[%d]:%s\n", l, muts[l].c_str());
	}*/
	getUnequalMutants(muts, corresponding_tests, terms, current_term_index, mutants);
	/*printf("mutants.size:%d\r\n", mutants.size());
	for (int l = 0; l < mutants.size(); l++){
		printf("mutants[%d]:%s\n", l, mutants[l].c_str());
	}*/
}


bool genDP_Matrix(vector<vector<int>>& finalCoefficientMatrix, vector<int>& finalTModelIndex, vector<vector<int>> &DP_Matrix)
{
	for (int k = 0; k <finalCoefficientMatrix[0].size(); k++)
	{
		vector<int> tmp_rowMatrix;
		for (int kk = 0; kk<finalCoefficientMatrix.size(); kk++)
		{
			tmp_rowMatrix.push_back(finalCoefficientMatrix[kk][k]);
		}
		DP_Matrix.push_back(tmp_rowMatrix);
	}
	if (DP_Matrix.size() == finalTModelIndex.size())
		return true;
	else
		return false;
}
bool genCase_Mutants(vector<vector<int>> &case_mutant, vector<int> finalTModelIndex, vector<vector<int>> DP_Matrix)
{
	for (int k = 0; k<DP_Matrix.size(); k++)
	{
		vector<int> tmp_rowMatrix;
		for (int kk = 0; kk<DP_Matrix[k].size(); kk++)
		{
			if (DP_Matrix[k][kk] == 1)
			{
				tmp_rowMatrix.push_back(kk);
			}
		}
		case_mutant.push_back(tmp_rowMatrix);
	}
	if (case_mutant.size() == finalTModelIndex.size())
		return true;
	else
		return false;
}
