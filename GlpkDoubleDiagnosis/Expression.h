#pragma once
#include "Utility.h"
#include "Point.h"
#include <Vector>
#include <list>
using namespace std;

typedef struct tagPRE_PROCESS_DATA{

	int literalCount;
	int termCount;
	std::vector<string> op;
	std::vector<string> op2;
	std::vector<string> up;
	std::vector<string> opover2;
	std::vector<string> fp;
	std::hash_set<string> fp1;
	std::hash_set<string> fp2;
	std::vector<std::vector<string>> termsDimension;

}PRE_PROCESS_DATA;

bool checkEqualment(string str1, string str2);
bool generateFP(string idnf, vector<string>& fp);
string  generateIDNForICNF(string expression, bool DNF1CNF0,bool withBDD=true);
// withBDD 
void generateTPandFP(string exp,vector<Point>& TPs,list<Point>& FPs, bool withBDD=true);
void generateFPandTP(string exp,vector<Point>& FPs,list<Point>& TPs, bool withBDD=true);
bool generateOTPandUTPandFP(string idnf, vector<string>& otp, vector<string>& op2, vector<string>& utp, vector<string>&fp);

//Tmodel data
bool generateLitNumTermNum(string icnf, int& literalNum, int& termNum, bool DNF1CNF0);
bool generateOFPandUFP(string icnf, vector<string>& ofp, vector<string>& ufp);
bool generateOTPandUTP(string idnf, vector<string>& otp, vector<string>& utp);
bool generateOTPover2(string idnf, vector<string>& otpof2);

string generateCNFtermDimension(string model, string cnfTerm);
string generateDNFtermDimension(string model, string dnfTerm);

string generateCNFdata(string icnf);
std::vector<string> genCNFtermDimensionVector(string model, string cnfTerm);

string generateDNFdata(string idnf);
std::vector<string> genDNFtermDimensionVector(string model, string dnfTerm);

bool getPreProcessData(string expression, PRE_PROCESS_DATA& preData, bool DNF1CNF0);


