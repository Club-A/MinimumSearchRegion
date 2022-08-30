#include "CommonFuncs.h"
#include "Utility.h"
void getNeedFValues(vector<vector<int>> DimensionValues, vector<vector<int>> &FValues);
void removeAllZeroCols(vector<vector<int>> coefficientMatrix, vector<int> tModelIndexAppeared, vector<vector<int>>& finalCoefficientMatrix, vector<int>& finalTModelIndex);
vector<string> genMutants(vector<vector<vector<int>>>& mutantsDimensionValue, bool vOdd, string model);
void getUnequalMutants(
	vector<string> muts,
	vector<vector<CCell*>> corresponding_tests,
	vector<string> terms,
	int current_term_index,
	vector<string>& mutants);
void get_mutant_constrains
(vector<vector<vector<int>>>& mutantsPresentedByDimensionValue,
bool vOdd,
vector<vector<CCell*>> corresponding_tests,
string expression,
int current_term_index,
vector<string>& mutants
);
bool genDP_Matrix(vector<vector<int>>& finalCoefficientMatrix, vector<int>& finalTModelIndex, vector<vector<int>> &DP_Matrix);
bool genCase_Mutants(vector<vector<int>> &case_mutant, vector<int> finalTModelIndex, vector<vector<int>> DP_Matrix);
