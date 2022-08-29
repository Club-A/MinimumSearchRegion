#include "stdafx.h"
#include "Literal.h"

Literal::Literal(void)
{

}
Literal::Literal(char L, bool P) :l(L), isPositive(P)
{

}

//nanjia：原本的函数后加了一个const
char Literal::getChar() const{
	return this->l;
}
bool Literal::getPos() const{
	return this->isPositive;
}


//nanjia：词类加了重载函数
bool  Literal::operator==(const Literal& rc) const//重载=
{
	return l == rc.l;// && isPositive == rc.isPositive;
}
bool  Literal::operator < (const Literal& n)const        //重载<
{
	return l < n.l;
}


