#pragma once
#include <vector>
#include <string>
#include <sstream>
#include "glpk.h"
#include <algorithm>

using namespace std;

class GLPK_Solve
{
public:
	 std::vector<int> lp_solve(std::vector<std::vector<int>>& para);
	 std::vector<int> lp_solve_lo(std::vector<std::vector<int>>& para);
	 std::vector<int> lp_solve(std::vector<std::vector<int>>& para, bool isPartitionNeed);
	 std::vector<int> lp_solve_lo(std::vector<std::vector<int>>& para, bool isPartitionNeed);
	 string run(std::vector<int>&colSelected, std::vector<std::vector<int>>*matrixPointer);
	 string run_lo(std::vector<int>&colSelected, std::vector<std::vector<int>>*matrixPointer);
	 std::vector<int> lp_solve_use_trftif(std::vector<std::vector<int>> para);
};