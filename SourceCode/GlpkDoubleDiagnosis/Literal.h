#pragma once
using namespace std;

class Literal
{
	private:
		char l;//��ǰ��ĸ
		bool isPositive; //ȡ������ȡ�� a/!a
	public:
		Literal();//���캯��
		Literal(char L,bool P);//���캯��
		char getChar() const;
		bool getPos() const;
		bool  Literal::operator==(const Literal& rc)const;
		bool  Literal::operator < (const Literal& n)const;
};


//nanjia:����һ����ϣ����������
class Literal_hash
{
public:
	int  operator()(const Literal & rc) const               //����hash
	{
		return rc.getChar() - 'A';
		//if (rc.getPos()==true) return rc.getChar()-'A';
		//else return -(rc.getChar() - 'A');
	}
};

