#pragma once
using namespace std;

class Literal
{
	private:
		char l;//当前字母
		bool isPositive; //取正还是取反 a/!a
	public:
		Literal();//构造函数
		Literal(char L,bool P);//构造函数
		char getChar() const;
		bool getPos() const;
		bool  Literal::operator==(const Literal& rc)const;
		bool  Literal::operator < (const Literal& n)const;
};


//nanjia:加了一个哈希函数定义类
class Literal_hash
{
public:
	int  operator()(const Literal & rc) const               //重载hash
	{
		return rc.getChar() - 'A';
		//if (rc.getPos()==true) return rc.getChar()-'A';
		//else return -(rc.getChar() - 'A');
	}
};

