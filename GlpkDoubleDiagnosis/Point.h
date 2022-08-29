#pragma once
#include <string>
using namespace std;

class Point
{
public:
	Point(string);
	~Point(void);
	
	void setValue(string v){value=v;};
	string getValue()const{return value;};
	void setCoverTerm(int n){coverTerm=n;};
	int getCoverTerm()const{return coverTerm;};

private:
	string value; 
	int coverTerm;
};

