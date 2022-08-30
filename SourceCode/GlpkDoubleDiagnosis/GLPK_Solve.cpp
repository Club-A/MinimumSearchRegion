#include "stdafx.h"
#include "GLPK_Solve.h"
#include "MatrixPartition.h"
#include "UnionFind.h"
#include <fstream>
#include <ctime>
#include <sstream>
#include <algorithm>
#include <numeric>
#define MAX_COEFF_COUNT 1500*1500
#define MAX_PROB_DIFFICULTY 50
#define GAP_LIMIT 5
#define MAX_PROB_DIFFICULTY_FACTOR 5
//#define DUMP_GEN_TC_TIME
#define NEED_SMP_PRE
//#define DUMP_PROB_DIFFICULTY_DETERMINATION_PROCESS

int ia[MAX_COEFF_COUNT] = { 0 }, ja[MAX_COEFF_COUNT] = { 0 };
double ar[MAX_COEFF_COUNT] = { 0 };

typedef struct{
	//GLP_ON : hard problem is it ;GLP_OFF: not hard
	int isHardPro;
	//max gap between upper_bound and lower_bound 
	int gap_limit;
	//GLP_ON: at least a feasible solution found ,or else GLP_OFF ,no solution found within time limits
	int isFeasibleMIPSolutionFound;

} cb_param;

bool isSameMatrix(std::vector<std::vector<int>>& m1 ,std::vector<std::vector<int>>& m2) {

	if(m1.size() !=m2.size()) return false;
	if(m1[0].size() !=m2[0].size()) return false;
	for (size_t i = 0; i < m1.size(); i++)
	{
		for (size_t j = 0; j < m1[i].size(); j++)
		{
			if(m1[i][j] !=m2[i][j])
				return false;
		}
	}
	return true;
}

//comparator: row1 :0101 , row2 :1101 then  row1 appears ahead of row2.
bool  Compare(const std::vector<int>& x,const std::vector<int>& y){	
	if( x.size() < y.size() ) return true;	
	if( x.size() > y.size() ) return false;
	int pos = 0;
	while (x[pos]==y[pos])pos++;		 
	return y[pos] == 1;
}
//
//
//
//
//
std::vector<int> GLPK_Solve::lp_solve(std::vector<std::vector<int>>& matrix ,bool isPartitionNeed)
{
	if(!isPartitionNeed) 
	{

		return //lp_solve_use_trftif(matrix);
			lp_solve(matrix);	  
	}
	int variables_count = matrix.at(0).size();
	
	std::vector<int> result(variables_count, 0);

	if (matrix.size() == 0){
		return result;
	}

	MatrixPartition mp;
	vector<vector<int>>crossRows = mp.findOutCrossRows(matrix);

	int block_count=crossRows.size();
	vector<vector<int>>partitionRows = mp.partitionMatrixWithRows(block_count, crossRows);

	int colNum=matrix.back().size();
	vector<int>colIndex(colNum,-1);
	for(int i=0;i<colNum;i++)
		colIndex[i] = i;
	//矩阵分块
	vector<vector<vector<int>>>subMatrixs;
	vector<vector<int>>subColIndexs;
	mp.getSubMatrixs(matrix,colIndex,partitionRows,subMatrixs,subColIndexs);

	UnionFindSet* ufs = new UnionFindSet(subMatrixs.size());
	for (size_t _i = 0; _i < subMatrixs.size(); _i++)
	{
		for (size_t _j = _i+1; _j < subMatrixs.size(); _j++)
		{
			if(ufs->same(_i,_j))
			{

			}
			else if(isSameMatrix(subMatrixs[_i],subMatrixs[_j]))
			{
				ufs->unite(_i,_j);			
			}
		}
	}
	vector<vector<int>> solutions(subMatrixs.size());

	int subProblems_count = subMatrixs.size();

	for (size_t i = 0; i < subProblems_count; i++)
	{

		// solve the subproblem transfer the solution 
		if (subMatrixs[i][0].size() ==1 || subMatrixs[i].size() == 1 )
		{
			result.at(subColIndexs[i][0]) = 1; 
		}
		else
		{
			if(solutions[ufs->find(i)].size() == 0)
			{
				/* time duration:  solve MIP */
				vector<int> solution4subproblem =
					lp_solve(subMatrixs[i]);
				//lp_solve_use_trftif(subMatrixs[i]);

				solutions[ufs->find(i)] = solution4subproblem;			

				for (size_t j = 0; j < solution4subproblem.size(); j++)
				{

					result.at(subColIndexs[i][j]) = solution4subproblem[j];
				} 

			}
			else
			{
				for (size_t j = 0; j < solutions[ufs->find(i)].size(); j++)
				{

					result.at(subColIndexs[i][j]) = solutions[ufs->find(i)][j];

				} 

			}
		}
	}
	return result;

}
std::vector<int> GLPK_Solve::lp_solve_lo(std::vector<std::vector<int>>& matrix, bool isPartitionNeed)
{
	if (!isPartitionNeed)
	{

		return //lp_solve_use_trftif(matrix);
			lp_solve(matrix);
	}
	int variables_count = matrix.at(0).size();

	std::vector<int> result(variables_count, 0);

	if (matrix.size() == 0){
		return result;
	}

	MatrixPartition mp;
	vector<vector<int>>crossRows = mp.findOutCrossRows(matrix);

	int block_count = crossRows.size();
	vector<vector<int>>partitionRows = mp.partitionMatrixWithRows(block_count, crossRows);

	int colNum = matrix.back().size();
	vector<int>colIndex(colNum, -1);
	for (int i = 0; i<colNum; i++)
		colIndex[i] = i;
	//矩阵分块
	vector<vector<vector<int>>>subMatrixs;
	vector<vector<int>>subColIndexs;
	mp.getSubMatrixs(matrix, colIndex, partitionRows, subMatrixs, subColIndexs);

	UnionFindSet* ufs = new UnionFindSet(subMatrixs.size());
	for (size_t _i = 0; _i < subMatrixs.size(); _i++)
	{
		for (size_t _j = _i + 1; _j < subMatrixs.size(); _j++)
		{
			if (ufs->same(_i, _j))
			{

			}
			else if (isSameMatrix(subMatrixs[_i], subMatrixs[_j]))
			{
				ufs->unite(_i, _j);
			}
		}
	}
	vector<vector<int>> solutions(subMatrixs.size());

	int subProblems_count = subMatrixs.size();

	for (size_t i = 0; i < subProblems_count; i++)
	{

		// solve the subproblem transfer the solution 
		if (subMatrixs[i][0].size() == 1 || subMatrixs[i].size() == 1)
		{
			result.at(subColIndexs[i][0]) = 1;
		}
		else
		{
			if (solutions[ufs->find(i)].size() == 0)
			{
				/* time duration:  solve MIP */
				vector<int> solution4subproblem =
					lp_solve_lo(subMatrixs[i]);
				//lp_solve_use_trftif(subMatrixs[i]);

				solutions[ufs->find(i)] = solution4subproblem;

				for (size_t j = 0; j < solution4subproblem.size(); j++)
				{

					result.at(subColIndexs[i][j]) = solution4subproblem[j];
				}

			}
			else
			{
				for (size_t j = 0; j < solutions[ufs->find(i)].size(); j++)
				{

					result.at(subColIndexs[i][j]) = solutions[ufs->find(i)][j];

				}

			}
		}
	}
	return result;

}
//
//直接求解
//
std::vector<int> GLPK_Solve::lp_solve(std::vector<std::vector<int>>& para){

	std::vector<int> result;
	if (para.size() == 0){
		return result;
	}
	int variables_count = para.at(0).size();
	int constraints_count = para.size();
	std::string default_constr_name_prefix = "C";
	std::string default_variable_name_prefix = "X";
	std::vector<std::string> cons_names;
	std::vector<std::string> var_names;
	for (size_t i = 0; i < constraints_count; i++)
	{
		std::ostringstream o;
		o << (i + 1);
		cons_names.push_back(default_constr_name_prefix + o.str());
	}
	for (size_t i = 0; i < variables_count; i++)
	{
		std::ostringstream o;
		o << (i + 1);
		var_names.push_back(default_variable_name_prefix + o.str());
	}
	glp_prob *mip = glp_create_prob();
	glp_set_prob_name(mip, "MinTestCases");
	glp_set_obj_dir(mip, GLP_MIN);

	//set the constraints: names and bounds
	glp_add_rows(mip, constraints_count);

	for (size_t i = 0; i < constraints_count; i++)
	{

		glp_set_row_name(mip, i + 1, cons_names.at(i).c_str());
		glp_set_row_bnds(mip, i + 1, GLP_DB, 1.0, variables_count);
	}

	// the object 
	glp_add_cols(mip, variables_count);

	for (size_t i = 0; i < variables_count; i++)
	{
		glp_set_col_name(mip, i + 1, var_names.at(i).c_str());
		glp_set_col_kind(mip, i + 1, GLP_BV);
		glp_set_obj_coef(mip, i + 1, 1.0);
	}

	//set the constraints matrix 
	int coef_count_1 = 0;
	std::vector<std::pair<int, int>> coefs_pos;
	for (size_t i = 0; i < constraints_count; i++)
	{
		for (size_t j = 0; j < variables_count; j++)
		{
			if (para.at(i).at(j) == 1)
			{
				coef_count_1++;
				coefs_pos.push_back(std::make_pair(i + 1, j + 1));
			}
		}
	}

	/*int* ia = new int[1 + coef_count_1];
	int* ja = new int[1 + coef_count_1];*/
	/*memset(ia,0,MAX_COEFF_COUNT*sizeof(int));
	memset(ja,0,MAX_COEFF_COUNT*sizeof(int));
	memset(ar,0,MAX_COEFF_COUNT*sizeof(double));*/
	//double* ar = new double[1 + coef_count_1];


	for (size_t i = 1; i <= coef_count_1; i++)
	{
		ar[i] = 1;
		ia[i] = coefs_pos.at(i - 1).first;
		ja[i] = coefs_pos.at(i - 1).second;
	}
	glp_load_matrix(mip, coef_count_1, ia, ja, ar);

	for (size_t i = 1; i <= coef_count_1; i++)
	{
		ar[i] = 0;
		ia[i] = 0;
		ja[i] = 0;
	}
	// Check if it is a hard problem
	// default
	int isHardPro = GLP_OFF;	

#ifdef NEED_SMP_PRE
	glp_prob *ip = mip;
	glp_smcp parm_smp;
	glp_init_smcp(&parm_smp);
	parm_smp.presolve = GLP_ON;
#endif // NEED_SMP_PRE

#ifdef NEED_SMP_PRE
	glp_simplex(ip, &parm_smp);  //此处为什么会运行时间长？是和链接有关系吗？

#endif // NEED_SMP_PRE

#ifdef NEED_SMP_PRE
	double obj_smp = glp_get_obj_val(ip);

	int count_none_zero_vars = 0;
	int count_integer_vars = 0;
	int count_integer_0_vars=0;
	int count_integer_1_vars=0;
	vector<int> temp_result;
	for (size_t i = 1; i <= variables_count; i++)
	{
		double xi_value = glp_get_col_prim(ip, i);


		// chaeck if the current dimension value is interger or not (i.e. 0 or 1)
		if (fabs(xi_value) <= 1e-20 || fabs(xi_value - 1) <= 1e-20)
		{
			count_integer_vars++;
			temp_result.push_back(int(xi_value));
		}
		if (fabs(xi_value) <= 1e-20)
		{
			count_integer_0_vars++;
		}
		if (fabs(xi_value - 1) <= 1e-20)
		{
			count_integer_1_vars++;
		}
	}

	count_none_zero_vars = variables_count - count_integer_vars;
	int max_prob_difficulty = MAX_PROB_DIFFICULTY;
	int max_prob_difficulty_factor = MAX_PROB_DIFFICULTY_FACTOR;
	if ((count_none_zero_vars - (int)obj_smp) >= max_prob_difficulty  
		|| int(count_none_zero_vars / obj_smp) >= max_prob_difficulty_factor)
		isHardPro = GLP_ON;


	if (count_integer_vars == variables_count)
	{
		return temp_result;
	}
#endif // NEED_SMP_PRE
	cb_param cbp;
	cbp.isHardPro = isHardPro;
	cbp.gap_limit = GAP_LIMIT;
	cbp.isFeasibleMIPSolutionFound = GLP_OFF;

	//GLP_ON : no need of a mip solution  and  MagicSolution will be the final solution;  GLP_OFF: use mip solution still.
	int isMagicSolutionRequired = GLP_ON;

	//if use the solution of mip,it doesn't matter whether it is an approximation result or not
	// or else use the TRF-TIF solution instead
	//cbp.isHardPro = GLP_ON;
	if (cbp.isHardPro == GLP_ON && isMagicSolutionRequired == GLP_ON)
	{
		result =lp_solve_use_trftif(para); 
	}
	else
	{
		int time_limit = 60000*100000;//10 secs
		glp_iocp parm;
		glp_init_iocp(&parm);
		parm.msg_lev = GLP_MSG_OFF;
		parm.presolve = GLP_ON;
		parm.gmi_cuts = GLP_ON;
		parm.mir_cuts = GLP_ON;
		parm.cov_cuts = GLP_ON;
		parm.clq_cuts = GLP_ON;
		parm.tm_lim = time_limit;

		int err = glp_intopt(mip, &parm); 

#ifndef CHECK_SOLUTION_FOUND

		for (size_t i = 1; i <= variables_count; i++)
		{
			result.push_back(int(glp_mip_col_val(mip, i)));
		}

#endif // !CHECK_SOLUTION_FOUND


	}

	glp_delete_prob(mip);
	return result;
}
std::vector<int> GLPK_Solve::lp_solve_lo(std::vector<std::vector<int>>& para){

	std::vector<int> result;
	if (para.size() == 0){
		return result;
	}
	int variables_count = para.at(0).size();
	int constraints_count = para.size();
	std::string default_constr_name_prefix = "C";
	std::string default_variable_name_prefix = "X";
	std::vector<std::string> cons_names;
	std::vector<std::string> var_names;
	for (size_t i = 0; i < constraints_count; i++)
	{
		std::ostringstream o;
		o << (i + 1);
		cons_names.push_back(default_constr_name_prefix + o.str());
	}
	for (size_t i = 0; i < variables_count; i++)
	{
		std::ostringstream o;
		o << (i + 1);
		var_names.push_back(default_variable_name_prefix + o.str());
	}
	glp_prob *mip = glp_create_prob();
	glp_set_prob_name(mip, "MaxMutants");
	glp_set_obj_dir(mip, GLP_MAX);

	//set the constraints: names and bounds
	glp_add_rows(mip, constraints_count);

	for (size_t i = 0; i < constraints_count; i++)
	{

		glp_set_row_name(mip, i + 1, cons_names.at(i).c_str());
		glp_set_row_bnds(mip, i + 1, GLP_UP, 0.0, 1.0);
	}

	// the object 
	glp_add_cols(mip, variables_count);

	for (size_t i = 0; i < variables_count; i++)
	{
		glp_set_col_name(mip, i + 1, var_names.at(i).c_str());
		glp_set_col_kind(mip, i + 1, GLP_BV);
		glp_set_obj_coef(mip, i + 1, 1.0);
	}

	//set the constraints matrix 
	int coef_count_1 = 0;
	std::vector<std::pair<int, int>> coefs_pos;
	for (size_t i = 0; i < constraints_count; i++)
	{
		for (size_t j = 0; j < variables_count; j++)
		{
			if (para.at(i).at(j) == 1)
			{
				coef_count_1++;
				coefs_pos.push_back(std::make_pair(i + 1, j + 1));
			}
		}
	}

	/*int* ia = new int[1 + coef_count_1];
	int* ja = new int[1 + coef_count_1];*/
	/*memset(ia,0,MAX_COEFF_COUNT*sizeof(int));
	memset(ja,0,MAX_COEFF_COUNT*sizeof(int));
	memset(ar,0,MAX_COEFF_COUNT*sizeof(double));*/
	//double* ar = new double[1 + coef_count_1];


	for (size_t i = 1; i <= coef_count_1; i++)
	{
		ar[i] = 1;
		ia[i] = coefs_pos.at(i - 1).first;
		ja[i] = coefs_pos.at(i - 1).second;
	}
	glp_load_matrix(mip, coef_count_1, ia, ja, ar);

	for (size_t i = 1; i <= coef_count_1; i++)
	{
		ar[i] = 0;
		ia[i] = 0;
		ja[i] = 0;
	}
	// Check if it is a hard problem
	// default
	int isHardPro = GLP_OFF;

#ifdef NEED_SMP_PRE
	glp_prob *ip = mip;
	glp_smcp parm_smp;
	glp_init_smcp(&parm_smp);
	parm_smp.presolve = GLP_ON;
#endif // NEED_SMP_PRE

#ifdef NEED_SMP_PRE
	glp_simplex(ip, &parm_smp);  //此处为什么会运行时间长？是和链接有关系吗？

#endif // NEED_SMP_PRE

#ifdef NEED_SMP_PRE
	double obj_smp = glp_get_obj_val(ip);

	int count_none_zero_vars = 0;
	int count_integer_vars = 0;
	int count_integer_0_vars = 0;
	int count_integer_1_vars = 0;
	vector<int> temp_result;
	for (size_t i = 1; i <= variables_count; i++)
	{
		double xi_value = glp_get_col_prim(ip, i);


		// chaeck if the current dimension value is interger or not (i.e. 0 or 1)
		if (fabs(xi_value) <= 1e-20 || fabs(xi_value - 1) <= 1e-20)
		{
			count_integer_vars++;
			temp_result.push_back(int(xi_value));
		}
		if (fabs(xi_value) <= 1e-20)
		{
			count_integer_0_vars++;
		}
		if (fabs(xi_value - 1) <= 1e-20)
		{
			count_integer_1_vars++;
		}
	}

	count_none_zero_vars = variables_count - count_integer_vars;
	int max_prob_difficulty = MAX_PROB_DIFFICULTY;
	int max_prob_difficulty_factor = MAX_PROB_DIFFICULTY_FACTOR;
	if ((count_none_zero_vars - (int)obj_smp) >= max_prob_difficulty
		|| int(count_none_zero_vars / obj_smp) >= max_prob_difficulty_factor)
		isHardPro = GLP_ON;


	if (count_integer_vars == variables_count)
	{
		return temp_result;
	}
#endif // NEED_SMP_PRE
	cb_param cbp;
	cbp.isHardPro = isHardPro;
	cbp.gap_limit = GAP_LIMIT;
	cbp.isFeasibleMIPSolutionFound = GLP_OFF;

	//GLP_ON : no need of a mip solution  and  MagicSolution will be the final solution;  GLP_OFF: use mip solution still.
	int isMagicSolutionRequired = GLP_ON;

	//if use the solution of mip,it doesn't matter whether it is an approximation result or not
	// or else use the TRF-TIF solution instead
	//cbp.isHardPro = GLP_ON;
	/*if (cbp.isHardPro == GLP_ON && isMagicSolutionRequired == GLP_ON)
	{
		result = lp_solve_use_trftif(para);
	}
	else*/
	{
		int time_limit = 60000 * 100000;//10 secs
		glp_iocp parm;
		glp_init_iocp(&parm);
		parm.msg_lev = GLP_MSG_OFF;
		parm.presolve = GLP_ON;
		parm.gmi_cuts = GLP_ON;
		parm.mir_cuts = GLP_ON;
		parm.cov_cuts = GLP_ON;
		parm.clq_cuts = GLP_ON;
		parm.tm_lim = time_limit;

		int err = glp_intopt(mip, &parm);

#ifndef CHECK_SOLUTION_FOUND

		for (size_t i = 1; i <= variables_count; i++)
		{
			result.push_back(int(glp_mip_col_val(mip, i)));
		}

#endif // !CHECK_SOLUTION_FOUND


	}

	glp_delete_prob(mip);
	return result;
}
std::vector<int> GLPK_Solve::lp_solve_use_trftif(std::vector<std::vector<int>> para)
{
	int variables_count = para.at(0).size();
	int constraints_count = para.size();
	std::vector<int> result(variables_count, 0);
	std::vector<int> mark(constraints_count, 0);
	std::vector<int> cols_1_occurences(variables_count, 0);
	//caculate cols_1_occurences
	for (size_t i = 0; i < constraints_count; i++)
	{
		for (size_t j = 0; j < variables_count; j++)
		{
			if (para[i][j] == 1)cols_1_occurences[j]++;
		}
	}
	int number_of_marked_rows = 0;
	//while there's a row  Ru: mark[Ru]==0
	while (number_of_marked_rows < constraints_count)
	{
		bool has_rows_mark_update = false;
		// select a col : Cx , with most 1-occurrence and result[Cx] ==0 ,then set result[Cx] =1
		int max_1_occurrence_index = -1;
		int max_number_1_occurrence = 0;
		for (size_t i = 0; i < variables_count; i++)
		{
			if (result[i] == 0 && cols_1_occurences[i] >max_number_1_occurrence)
			{
				max_1_occurrence_index = i;
				max_number_1_occurrence = cols_1_occurences[i];
			}
		}


		if (max_1_occurrence_index == -1 || max_number_1_occurrence == 0) break;
		result[max_1_occurrence_index] = 1;

		// mark any mark[row] ,once  para[Ry , Cx] ==1 
		// update cols_1_occurences : cols_1_occurences[d-th] decreases by 1 when Ry's d-th dimension value is 1

		for (size_t i = 0; i < constraints_count; i++)
		{
			if (mark[i] == 0 && para[i][max_1_occurrence_index] == 1)
			{
				mark[i] = 1;
				has_rows_mark_update = true;
				number_of_marked_rows++;
				for (size_t j = 0; j < variables_count; j++)
				{
					if (para[i][j] == 1) cols_1_occurences[j]--;
				}
			}
		}
		if (!has_rows_mark_update) break;
	}
	return result;
}
//*****************************************************************************************
//被调用的求解函数
//matrixPointer 带求解的矩阵
//colSelected 
//*****************************************************************************************
string GLPK_Solve::run(std::vector<int>&colSelected, std::vector<std::vector<int>>*matrixPointer)
{
	//std::vector<int> col_result((*matrixPointer)[0].size(),1);
	std::vector<int> col_result = lp_solve(*matrixPointer, true);
	int colNum = col_result.size();
	for (size_t i = 0; i<colNum; i++)
	{
		if (col_result[i] == 1)
			colSelected.push_back(i);
	}

	string result="ok";
	return result;
}
string GLPK_Solve::run_lo(std::vector<int>&colSelected, std::vector<std::vector<int>>*matrixPointer)
{
	//std::vector<int> col_result((*matrixPointer)[0].size(),1);
	std::vector<int> col_result = lp_solve_lo(*matrixPointer,true);
	int colNum = col_result.size();
	for (size_t i = 0; i<colNum; i++)
	{
		if (col_result[i] == 1)
			colSelected.push_back(i);
	}

	string result = "ok";
	return result;
}