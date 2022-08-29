#pragma once
#include"math.h"
class CCell
{
public:
	CCell(void);
	~CCell(void);
	CCell(int dimension);
	void Copy(CCell* pCell);
	int dimension;
	int* value;//dimension个整数都是有意义的
	//获得cell在tModel中的坐标如311
	int getTModelIndex();
	bool compare(CCell* pCell);
	
};

