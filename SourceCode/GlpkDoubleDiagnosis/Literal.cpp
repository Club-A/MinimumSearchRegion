#include "stdafx.h"
#include "Literal.h"

Literal::Literal(void)
{

}
Literal::Literal(char L, bool P) :l(L), isPositive(P)
{

}

//nanjia��ԭ���ĺ��������һ��const
char Literal::getChar() const{
	return this->l;
}
bool Literal::getPos() const{
	return this->isPositive;
}


//nanjia������������غ���
bool  Literal::operator==(const Literal& rc) const//����=
{
	return l == rc.l;// && isPositive == rc.isPositive;
}
bool  Literal::operator < (const Literal& n)const        //����<
{
	return l < n.l;
}


