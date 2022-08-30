/*
这个类封装了检测缺陷的接口
*/
#pragma once
#include <vector>
using namespace std;
class FaultDetection
{
public:
	FaultDetection(void);
	~FaultDetection(void);
	//输入:testset测试用例集，原表达式exprOri，待测表达式expr，有效的testcase对应的列validTCsol，无效的testcase对应的列invalidTCsol
	//返回:true,待测表达式没有发生缺陷，false，待测表达式发生缺陷
	bool checkTestSet(vector<string> testset,string exprOri,string expr,vector<int>& validTCsol,vector<int>& invalidTCsol,bool vOdd);
	// coded by ZHANG XIAOXIONG
	void checkTestSet1(vector<string> testset, string exprOri, string expr, vector<string>& validTCsol, vector<string>& invalidTCsol);

	//根据测试用例测待测表达式的结果更新矩阵
	//输入:CoefficientMatrix约束矩阵，TModelIndex矩阵中的索引，有效的测试用例validTCsol，无效的测试用例invalidTCsol
	//返回：CoefficientMatrix约束矩阵剩下的行数
	int updateMatrix
		(
		vector<vector<int>>& CoefficientMatrix,vector<int>& TModelIndex, 
		vector<int> validTCsol,vector<int> invalidTCsol,vector<int>& constrain_index,vector<bool>& mutantsremained);
	//检测两个表达式是否等价
	bool checkEqualment(string str1, string str2);
	//去掉矩阵中全都是0的竖列
	int removeZeroRow(vector<vector<int>> old_CoefficientMatrix,vector<vector<int>>& new_CoefficientMatrix);

	//使用求交集的方式检测变体
	string checkExprwithSet(string exprOri, string expr, vector<string> testset, vector<vector<int>> case_mutant, vector<vector<int>>finalCoefficientMatrix, vector<int>finalTModelIndex, vector<string>& mutants_DP, bool vOdd);

	bool checkTestCase(int testcase_Index, string exprOri, string expr, bool vOdd);
private:
	//原表达式
	string exprOri;
	//待测表达式
	string expr;

};
void removeAllOneCols(vector<vector<int>>& coefficientMatrix,vector<int> row_index);

