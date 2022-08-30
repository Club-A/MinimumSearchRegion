#include "stdafx.h"
#include <string>
#include <vector>
#include <map>
#include <stdio.h>

using namespace std;

string notOfExp(string exp, string model);
string generateModel(string exp);
vector<string> extractDNFstyle(string exp);
vector<string> extractCNFstyle(string exp);
string terms2Exp(vector<string> terms);
void termLif(string &term, const char *ch, vector<map<string, short>> &characters);
string getOneLifCase(string sterm, vector<string> upPoints, string model);
void termLrf(string &term, const char * ch, const char *newch, vector<map<string, short>> &characters);
int termLrf(string &term, int pos, const char *newch, vector<map<string, short>> &characters);
string getOneLrfECase(string eterm, vector<string> nfpPoints, string model);
void termLof(string &term, const char * ch, vector<map<string, short>> &characters);
int termLof(string &term, int pos, vector<map<string, short>> &characters);
void termLnf(string &term, const char * ch, vector<map<string, short>> &characters);
int termLnf(string &term, int pos, vector<map<string, short>> &characters);
void expTof(vector<string>&terms, int pos, vector<map<string, short>> &characters);
void expTnf(vector<string>&terms, int pos, vector<map<string, short>> &characters);
string termDorf(string term1, string term2, vector<map<string, short>> &characters);
void expDorf(vector<string>&terms, int pos1, int pos2, vector<map<string, short>> &characters);
vector<string> termCorf(string &term, int termPos, vector<map<string, short>> &characters);
int expCorf(vector<string>&terms, int expPos, int termPos, vector<map<string, short>> &characters);
vector<string> termEnf(vector<string>&terms, vector<int>pos, vector<map<string, short>> &characters);
void expEnf(vector<string>&terms, vector<int>poss, vector<map<string, short>> &characters);