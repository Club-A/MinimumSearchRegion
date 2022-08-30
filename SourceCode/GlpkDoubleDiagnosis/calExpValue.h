#include "stdafx.h"
#include <string>
#include <vector>
#include <stdio.h>

using namespace std;
bool isDNFtermTrue(const char * term, const char *  testData, string model);
bool evaluateDNF(string exp, const char * testData, string model);
bool evaluate(string exp, const char * testData, string model);
bool checkCharacter(vector<string> uterms, vector<string> oterms, vector<map<string, short>> characters, const char * testData, string model);
string findTestCaseFromCharacter(vector<string> uterms, vector<string> oterms, vector<map<string, short>> &characters, string model);