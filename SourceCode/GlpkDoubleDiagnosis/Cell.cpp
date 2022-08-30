#include "StdAfx.h"
#include "Cell.h"


CCell::CCell(void)
{
}

CCell::~CCell(void)
{
	delete[] this->value;
}

CCell::CCell(int dimension)
{
	this->value = new int [dimension];
	this->dimension = dimension;
}

void CCell::Copy(CCell* pCell)
{
	for(int i=0; i<pCell->dimension; i++)
		this->value[i] = pCell->value[i];
}

//获得cell在tModel中的坐标如311
int CCell::getTModelIndex()
{
	int result=0;
	for(int i=dimension-1; i>=0; i--)
	{
		result += value[i]*pow(10.0,dimension-1-i);
	}

	return result;
}

bool CCell::compare(CCell* pCell)
{
	if(this->dimension != pCell->dimension)
		return false;

	for(int i=0; i<this->dimension; i++)
	{
		if(this->value[i]!=pCell->value[i])
			return false;
	}

	return true;
}
