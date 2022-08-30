#include "stdafx.h"
#include "DnfMutant.h"
#include "Utility.h"
#include <set>
#include <iostream>

#include <algorithm>
using namespace std;

DnfMutant::DnfMutant(void)
{
}


DnfMutant::~DnfMutant(void)
{
}

Mutants DnfMutant::getMutants(string exp, string faultType, vector<string> faultTerms){
	if (faultType == "LIF"){
		return generateLIFmutants(exp, faultTerms);
	}
	if (faultType == "LOF"){
		return generateLOFmutants(exp, faultTerms);
	}
	else if (faultType == "LNF"){
		return generateLNFmutants(exp, faultTerms);
	}
	else if (faultType == "LRF"){
		return generateLRFmutants(exp, faultTerms);
	}
	else if (faultType == "TOF"){
		return generateTOFmutants(exp, faultTerms);
	}
	else if (faultType == "TNF"){
		return generateTNFmutants(exp, faultTerms);
	}
	else if (faultType == "CORF"){
		return generateCORFmutants(exp, faultTerms);
	}
	else if (faultType == "DORF"){
		return generateDORFmutants(exp, faultTerms);
	}
	else if (faultType == "ENF"){//和term无关
		return generateENFmutants(exp);
	}
	else if (faultType == "SALL"){
		Mutants a = generateLIORFmutants(exp, faultTerms);
		a += generateTOFmutants(exp, faultTerms);//添加TOF变体
		a += generateLNFmutants(exp, faultTerms);//添加LNF变体
		a += generateCORFmutants(exp, faultTerms);//添加CORF变体
		a += generateDORFmutants(exp, faultTerms);//添加CORF变体
		a += generateTNFmutants(exp, faultTerms);//添加CORF变体
		a += generateENFmutants(exp);//添加CORF变体
		//a += generateDLIRFdoublemutants(exp);
		return a;
	}
	//单项双缺陷
	else if (faultType == "DLIRF"){//生成变体生成重复缺陷表达式
		return generateDLIRFdoublemutants(exp);
	}
	else if (faultType == "DLORF"){
		return generateDLORFdoublemutants(exp);
	}
	else if (faultType == "DLRRF"){
		return generateDLRRFdoublemutants(exp);
	}
	else if (faultType == "DLIIF"){
		return generateSingleTermLIFxLIFdoublemutants(exp);
	}
	else if (faultType == "DLOOF"){
		return generateSingleTermLOFxLOFdoublemutants(exp);
	}
	else if (faultType == "DLNNF"){
		return generateSingleTermLNFxLNFdoublemutants(exp);
	}
	else if (faultType == "DLINF"){
		return generateSingleTermLIFxLNFdoublemutants(exp);
	}
	else if (faultType == "DLONF"){
		return generateSingleTermLOFxLNFdoublemutants(exp);
	}
	else if (faultType == "DLNRF"){
		return generateSingleTermLNFxLRFdoublemutants(exp);
	}
	else if (faultType == "DLITNF"){
		return generateSingleTermLIFxTNFdoublemutants(exp);
	}
	else if (faultType == "DLRTNF"){
		return generateSingleTermLRFxTNFdoublemutants(exp);
	}
	else if (faultType == "DLOTNF"){
		return generateSingleTermLOFxTNFdoublemutants(exp);
	}
	else if (faultType == "DLNTNF"){
		return generateSingleTermLNFxTNFdoublemutants(exp);
	}
	else if (faultType == "DLICORF"){
		return generateSingleTermCORFxLIFmutants(exp);
	}
	else if (faultType == "DLOCORF"){
		return generateSingleTermCORFxLOFmutants(exp);
	}
	else if (faultType == "DLRCORF"){
		return generateSingleTermCORFxLRFmutants(exp);
	}
	else if (faultType == "DLNCORF"){
		return generateSingleTermCORFxLNFmutants(exp);
	}
	else if (faultType == "DLIDORF"){
		return generateSingleTermDORFxLIFmutants(exp);
	}
	else if (faultType == "DLODORF"){
		return generateSingleTermDORFxLOFmutants(exp);
	}
	else if (faultType == "DLNDORF"){
		return generateSingleTermDORFxLNFmutants(exp);
	}
	//双项双缺陷
	else if (faultType == "LIFxLIF")//双项双缺陷1：LIFxLIF
		return generateLIFxLIFdoublemutants(exp);
	else if (faultType == "LIFxLRF")//双项双缺陷2：LIFxLRF
		return generateLIFxLRFdoublemutants(exp);
	else if (faultType == "LRFxLRF")//双项双缺陷3：LRFxLRF
		return generateLRFxLRFdoublemutants(exp);
	else if (faultType == "DORFxLRF")//双项双缺陷4：DORFxLRF
		return generateDORFxLRFdoublemutants(exp);
	else if (faultType == "TOFxLRF"){//双项双缺陷5：TOFxLRF
		printf("\n-----------enter TOFxLRF-----------\n");
		return generateTOFxLRFdoublemutants(exp);
	}
	else if (faultType == "LIORF" || faultType == "LIROF" || faultType == "LOIRF" || faultType == "LOIRF" || faultType == "LRIOF" || faultType == "LROIF"){
		return generateLIORFmutants(exp, faultTerms);
	}
	else if (faultType == "DALL"){
		Mutants a = generateDLIRFdoublemutants(exp);//LIF x LRF --
		a += generateDLRRFdoublemutants(exp);//LRF x LRF --
		a += generateDLORFdoublemutants(exp);//LOF x LRF --

		a += generateSingleTermLIFxLIFdoublemutants(exp);//LIF x LIF
		a += generateSingleTermLOFxLOFdoublemutants(exp);//LOF x LOF
		a += generateSingleTermLNFxLNFdoublemutants(exp);//LNF x LNF
		a += generateSingleTermLIFxLNFdoublemutants(exp);//LIF x LNF
		//LIFxLOF等价于LRF
		a += generateSingleTermLOFxLNFdoublemutants(exp);//LOF x LNF
		a += generateSingleTermLNFxLRFdoublemutants(exp);//LNF x LRF

		//词+项(TNF)相关：
		a += generateSingleTermLIFxTNFdoublemutants(exp);//LIF x TNF
		a += generateSingleTermLRFxTNFdoublemutants(exp);//LRF x TNF 
		a += generateSingleTermLOFxTNFdoublemutants(exp);//LOF x TNF
		a += generateSingleTermLNFxTNFdoublemutants(exp);//LNF x TNF

		//CORF + 词 相关：
		a += generateSingleTermCORFxLIFmutants(exp);
		a += generateSingleTermCORFxLOFmutants(exp);
		a += generateSingleTermCORFxLRFmutants(exp);
		a += generateSingleTermCORFxLNFmutants(exp);

		//CORF+TNF

		//DORF + 词 相关：
		a += generateSingleTermDORFxLIFmutants(exp);
		a += generateSingleTermDORFxLOFmutants(exp);
		a += generateSingleTermDORFxLNFmutants(exp);

		//=== 双项双缺陷 ===
		a += generateLIFxLIFdoublemutants(exp);//LIF x LIF
		a += generateLIFxLRFdoublemutants(exp);//LIF x LRF
		a += generateLRFxLRFdoublemutants(exp);//LRF x LRF
		a += generateDORFxLRFdoublemutants(exp);//DORF x LRF
		a += generateTOFxLRFdoublemutants(exp);//TOFxLRF

		return a;
	}
	else if (faultType == "ALL"){
		Mutants a = generateLIORFmutants(exp, faultTerms);
		a += generateTOFmutants(exp, faultTerms);//添加TOF变体
		a += generateLNFmutants(exp, faultTerms);//添加LNF变体
		a += generateCORFmutants(exp, faultTerms);//添加CORF变体
		a += generateDORFmutants(exp, faultTerms);//添加CORF变体
		a += generateTNFmutants(exp, faultTerms);//添加CORF变体
		a += generateENFmutants(exp);//添加CORF变体
		a += generateDLIRFdoublemutants(exp);//LIF x LRF --
		a += generateDLRRFdoublemutants(exp);//LRF x LRF --
		a += generateDLORFdoublemutants(exp);//LOF x LRF --

		a += generateSingleTermLIFxLIFdoublemutants(exp);//LIF x LIF
		a += generateSingleTermLOFxLOFdoublemutants(exp);//LOF x LOF
		a += generateSingleTermLNFxLNFdoublemutants(exp);//LNF x LNF
		a += generateSingleTermLIFxLNFdoublemutants(exp);//LIF x LNF
		//LIFxLOF等价于LRF
		a += generateSingleTermLOFxLNFdoublemutants(exp);//LOF x LNF
		a += generateSingleTermLNFxLRFdoublemutants(exp);//LNF x LRF

		//词+项(TNF)相关：
		a += generateSingleTermLIFxTNFdoublemutants(exp);//LIF x TNF
		a += generateSingleTermLRFxTNFdoublemutants(exp);//LRF x TNF 
		a += generateSingleTermLOFxTNFdoublemutants(exp);//LOF x TNF
		a += generateSingleTermLNFxTNFdoublemutants(exp);//LNF x TNF

		//CORF + 词 相关：
		a += generateSingleTermCORFxLIFmutants(exp);
		a += generateSingleTermCORFxLOFmutants(exp);
		a += generateSingleTermCORFxLRFmutants(exp);
		a += generateSingleTermCORFxLNFmutants(exp);

		//CORF+TNF

		//DORF + 词 相关：
		a += generateSingleTermDORFxLIFmutants(exp);
		a += generateSingleTermDORFxLOFmutants(exp);
		a += generateSingleTermDORFxLNFmutants(exp);

		//=== 双项双缺陷 ===
		a += generateLIFxLIFdoublemutants(exp);//LIF x LIF
		a += generateLIFxLRFdoublemutants(exp);//LIF x LRF
		a += generateLRFxLRFdoublemutants(exp);//LRF x LRF
		a += generateDORFxLRFdoublemutants(exp);//DORF x LRF
		a += generateTOFxLRFdoublemutants(exp);//TOFxLRF

		return a;
	}
	return Mutants("");
}


Mutants DnfMutant::getMutants(string exp, string faultType, vector<string> oriTerms, vector<string> faultTerms, string model){
	
	/*
	if (faultType == "LIF"){
		return generateLIFmutants(exp, faultTerms);
	}
	if (faultType == "LOF"){
		return generateLOFmutants(exp, faultTerms);
	}
	else if (faultType == "LNF"){
		return generateLNFmutants(exp, faultTerms);
	}
	else if (faultType == "LRF"){
		return generateLRFmutants(exp, faultTerms);
	}
	else if (faultType == "TOF"){
		return generateTOFmutants(exp, faultTerms);
	}
	else if (faultType == "TNF"){
		return generateTNFmutants(exp, faultTerms);
	}
	else if (faultType == "CORF"){
		return generateCORFmutants(exp, faultTerms);
	}
	else if (faultType == "DORF"){
		return generateDORFmutants(exp, faultTerms);
	}
	else if (faultType == "ENF"){//和term无关
		return generateENFmutants(exp);
	}
	else if (faultType == "SALL"){
		Mutants a = generateLIORFmutants(exp, faultTerms);
		a += generateTOFmutants(exp, faultTerms);//添加TOF变体
		a += generateLNFmutants(exp, faultTerms);//添加LNF变体
		a += generateCORFmutants(exp, faultTerms);//添加CORF变体
		a += generateDORFmutants(exp, faultTerms);//添加CORF变体
		a += generateTNFmutants(exp, faultTerms);//添加CORF变体
		a += generateENFmutants(exp);//添加CORF变体
		//a += generateDLIRFdoublemutants(exp);
		return a;
	}
	//单项双缺陷
	else if (faultType == "DLIRF"){//生成变体生成重复缺陷表达式
		return generateDLIRFdoublemutants(exp);
	}
	else if (faultType == "DLORF"){
		return generateDLORFdoublemutants(exp);
	}
	else if (faultType == "DLRRF"){
		return generateDLRRFdoublemutants(exp);
	}
	else if (faultType == "DLIIF"){
		return generateSingleTermLIFxLIFdoublemutants(exp);
	}
	else if (faultType == "DLOOF"){
		return generateSingleTermLOFxLOFdoublemutants(exp);
	}
	else if (faultType == "DLNNF"){
		return generateSingleTermLNFxLNFdoublemutants(exp);
	}
	else if (faultType == "DLINF"){
		return generateSingleTermLIFxLNFdoublemutants(exp);
	}
	else if (faultType == "DLONF"){
		return generateSingleTermLOFxLNFdoublemutants(exp);
	}
	else if (faultType == "DLNRF"){
		return generateSingleTermLNFxLRFdoublemutants(exp);
	}
	else if (faultType == "DLITNF"){
		return generateSingleTermLIFxTNFdoublemutants(exp);
	}
	else if (faultType == "DLRTNF"){
		return generateSingleTermLRFxTNFdoublemutants(exp);
	}
	else if (faultType == "DLOTNF"){
		return generateSingleTermLOFxTNFdoublemutants(exp);
	}
	else if (faultType == "DLNTNF"){
		return generateSingleTermLNFxTNFdoublemutants(exp);
	}
	else if (faultType == "DLICORF"){
		return generateSingleTermCORFxLIFmutants(exp);
	}
	else if (faultType == "DLOCORF"){
		return generateSingleTermCORFxLOFmutants(exp);
	}
	else if (faultType == "DLRCORF"){
		return generateSingleTermCORFxLRFmutants(exp);
	}
	else if (faultType == "DLNCORF"){
		return generateSingleTermCORFxLNFmutants(exp);
	}
	else if (faultType == "DLIDORF"){
		return generateSingleTermDORFxLIFmutants(exp);
	}
	else if (faultType == "DLODORF"){
		return generateSingleTermDORFxLOFmutants(exp);
	}
	else if (faultType == "DLNDORF"){
		return generateSingleTermDORFxLNFmutants(exp);
	}

	//==========================================  双项双缺陷 =========================================
	else if (faultType == "LRFxLRF")//双项双缺陷3：LRFxLRF
		return generateLRFxLRFdoublemutants(exp);
	else if (faultType == "DORFxLRF")//双项双缺陷4：DORFxLRF
		return generateDORFxLRFdoublemutants(exp);
	else if (faultType == "TOFxLRF"){//双项双缺陷5：TOFxLRF
		return generateTOFxLRFdoublemutants(exp);
	}
	*/

	//x https://www.youtube.com/watch?v=xWLyUmsSRPk
	//=======================================================================================================================
	if (faultType == "LIFxLIF"){//双项双缺陷1：LIFxLIF
		return generateLIFxLIFdoublemutants(exp, oriTerms, faultTerms, model);
	}
	else if (faultType == "LIFxTOF"){//双项双缺陷：LIFxTOF
		return generateTOFxLIFdoublemutants(exp, oriTerms, faultTerms, model);
	}
	else if (faultType == "TOFxTOF"){//双项双缺陷：TOFxTOF
		return generateTOFxTOFdoublemutants(exp, oriTerms, faultTerms, model);
	}
	else if (faultType == "LOFxLOF"){//双项双缺陷：LOFxLOF
		return generateLOFxLOFdoublemutants(exp, oriTerms, faultTerms, model);
	}
	else if (faultType == "LOFxCORF"){//双项双缺陷：LOFxCORF
		return generateCORFxLOFCase1doublemutants(exp, oriTerms, faultTerms, model);
	}
	else if (faultType == "CORFxCORF"){//双项双缺陷：CORFxCORF
		return generateCORFxCORFCase1doublemutants(exp, oriTerms, faultTerms, model);
	}
	else if (faultType == "LIFxLRF"){//双项双缺陷：LIFxLRF
		return generateLIFxLRFdoublemutants(exp, oriTerms, faultTerms, model);
	}
	else if (faultType == "LIFxLNF"){//双项双缺陷：LIFxLNF
		return generateLNFxLIFdoublemutants(exp, oriTerms, faultTerms, model);
	}
	else if (faultType == "LIFxTNF"){//双项双缺陷：LIFxTNF
		return generateTNFxLIFTwoTermdoublemutants(exp, oriTerms, faultTerms, model);
	}
	else if (faultType == "TOFxLRF"){//双项双缺陷：TOFxLRF
		return generateTOFxLRFdoublemutants(exp, oriTerms, faultTerms, model);
	}
	else if (faultType == "TOFxLNF"){//双项双缺陷：TOFxLNF
		return generateTOFxLNFdoublemutants(exp, oriTerms, faultTerms, model);
	}
	else if (faultType == "TOFxTNF"){//双项双缺陷：TOFxTNF
		return generateTNFxTOFdoublemutants(exp, oriTerms, faultTerms, model);
	}

	else if (faultType == "LOFxLRF"){//双项双缺陷：LOFxLRF
		return generateLOFxLRFdoublemutants(exp, oriTerms, faultTerms, model);
	}
	else if (faultType == "LOFxLNF"){//双项双缺陷：LOFxLNF
		return generateLNFxLOFdoublemutants(exp, oriTerms, faultTerms, model);
	}
	else if (faultType == "LOFxTNF"){//双项双缺陷：LOFxTNF
		return generateTNFxLOFTwoTermdoublemutants(exp, oriTerms, faultTerms, model);
	}

	else if (faultType == "CORFxLIF"){//双项双缺陷：CORFxLIF
		return generateCORFxLIFCase1doublemutants(exp, oriTerms, faultTerms, model);
	}

	else if (faultType == "CORFxLRF"){//双项双缺陷：CORFxLRF
		return generateCORFxLRFCase1doublemutants(exp, oriTerms, faultTerms, model);
	}
	else if (faultType == "CORFxLNF"){//双项双缺陷：CORFxLNF
		return generateCORFxLNFCase1doublemutants(exp, oriTerms, faultTerms, model);
	}
	else if (faultType == "CORFxTNF"){//双项双缺陷：CORFxTNF
		return generateCORFxLNFCase1doublemutants(exp, oriTerms, faultTerms, model);
	}

	else if (faultType == "CORFxLOF"){//双项双缺陷：CORFxLOF
		return generateCORFxLOFCase1doublemutants(exp, oriTerms, faultTerms, model);
	}

	

	else if (faultType == "DORFxLIF"){//双项双缺陷：DORFxLIF
		return generateDORFxLIFCase1doublemutants(exp, oriTerms, faultTerms, model);
	}
	else if (faultType == "DORFxTOF"){//双项双缺陷：DORFxTOF
		return generateTOFxDORFdoublemutants(exp, oriTerms, faultTerms, model);
	}
	else if (faultType == "DORFxLOF"){//双项双缺陷：DORFxLOF
		return generateDORFxLOFCase1doublemutants(exp, oriTerms, faultTerms, model);
	}
	else if (faultType == "DORFxCORF"){//双项双缺陷：DORFxCORF
		return generateDORFxCORFCase1doublemutants(exp, oriTerms, faultTerms, model);
	}

	else if (faultType == "ENFxLIF"){//双项双缺陷：ENFxLIF
		return generateENFxLIFCase1doublemutants(exp, oriTerms, faultTerms, model);
	}
	else if (faultType == "ENFxTOF"){//双项双缺陷：ENFxTOF
		return generateENFxTOFCase1doublemutants(exp, oriTerms, faultTerms, model);
	}
	else if (faultType == "ENFxLOF"){//双项双缺陷：ENFxLOF
		return generateENFxLOFCase1doublemutants(exp, oriTerms, faultTerms, model);
	}
	else if (faultType == "ENFxCORF"){//双项双缺陷：ENFxCORF
		return generateENFxCORFCase1doublemutants(exp, oriTerms, faultTerms, model);
	}

	else if (faultType == "LIFxLOF"){//双项双缺陷：LIFxLOF
		return generateLOFxLIFdoublemutants(exp, oriTerms, faultTerms, model);
	}
	else if (faultType == "LIFxCORF"){//双项双缺陷：LIFxCORF
		return generateCORFxLIFCase1doublemutants(exp, oriTerms, faultTerms, model);
	}
	else if (faultType == "TOFxLOF"){//双项双缺陷：TOFxLOF
		return generateTOFxLOFdoublemutants(exp, oriTerms, faultTerms, model);
	}
	else if (faultType == "TOFxCORF"){//双项双缺陷：TOFxCORF
		return generateCORFxTOFdoublemutants(exp, oriTerms, faultTerms, model);
	}
	else if (faultType == "LIFxLRF"){//双项双缺陷：LIFxLRF
		return generateLIFxLRFdoublemutants(exp, oriTerms, faultTerms, model);
	}
	else if (faultType == "LIFxLNF"){//双项双缺陷：LIFxLNF
		return generateLNFxLIFdoublemutants(exp, oriTerms, faultTerms, model);
	}
	else if (faultType == "LIFxTNF"){//双项双缺陷：LIFxTNF
		return generateTNFxLIFTwoTermdoublemutants(exp, oriTerms, faultTerms, model);
	}
	else if (faultType == "TOFxLRF"){//双项双缺陷：TOFxLRF
		return generateTOFxLRFdoublemutants(exp, oriTerms, faultTerms, model);
	}
	else if (faultType == "TOFxLNF"){//双项双缺陷：TOFxLNF
		return generateTOFxLNFdoublemutants(exp, oriTerms, faultTerms, model);
	}
	else if (faultType == "TOFxTNF"){//双项双缺陷：TOFxTNF
		return generateTNFxTOFdoublemutants(exp, oriTerms, faultTerms, model);
	}
	else if (faultType == "LOFxLRF"){//双项双缺陷：LOFxLRF
		return generateLOFxLRFdoublemutants(exp, oriTerms, faultTerms, model);
	}
	else if (faultType == "LOFxLNF"){//双项双缺陷：LOFxLNF
		return generateLNFxLOFdoublemutants(exp, oriTerms, faultTerms, model);
	}
	else if (faultType == "LOFxTNF"){//双项双缺陷：LOFxTNF
		return generateTNFxLOFTwoTermdoublemutants(exp, oriTerms, faultTerms, model);
	}
	else if (faultType == "CORFxLRF"){//双项双缺陷：CORFxLRF
		return generateCORFxLRFCase1doublemutants(exp, oriTerms, faultTerms, model);
	}
	else if (faultType == "CORFxLNF"){//双项双缺陷：CORFxLNF
		return generateCORFxLNFCase1doublemutants(exp, oriTerms, faultTerms, model);
	}
	else if (faultType == "CORFxTNF"){//双项双缺陷：CORFxTNF
		return generateTNFxCORFCase1doublemutants(exp, oriTerms, faultTerms, model);
	}
	else if (faultType == "LRFxLRF"){//双项双缺陷：LRFxLRF
		return generateLRFxLRFdoublemutants(exp, oriTerms, faultTerms, model);
	}
	else if (faultType == "LRFxLNF"){//双项双缺陷：LRFxLNF
		return generateLNFxLRFdoublemutants(exp, oriTerms, faultTerms, model);
	}
	else if (faultType == "LRFxTNF"){//双项双缺陷：LRFxTNF
		return generateTNFxLRFTwoTermdoublemutants(exp, oriTerms, faultTerms, model);
	}
	else if (faultType == "LNFxLNF"){//双项双缺陷：LNFxLNF
		return generateLNFxLNFdoublemutants(exp, oriTerms, faultTerms, model);
	}
	else if (faultType == "LNFxTNF"){//双项双缺陷：LNFxTNF
		return generateTNFxLNFTwoTermdoublemutants(exp, oriTerms, faultTerms, model);
	}
	else if (faultType == "TNFxTNF"){//双项双缺陷：TNFxTNF
		return generateTNFxTNFdoublemutants(exp, oriTerms, faultTerms, model);
	}
	
	else if (faultType == "DORFxTOF"){//双项双缺陷：DORFxTOF
		return generateTOFxDORFdoublemutants(exp, oriTerms, faultTerms, model);
	}
	else if (faultType == "DORFxLOF"){//双项双缺陷：DORFxLOF
		return generateDORFxLOFCase1doublemutants(exp, oriTerms, faultTerms, model);
	}
	else if (faultType == "DORFxCORF"){//双项双缺陷：DORFxCORF
		return generateDORFxCORFCase1doublemutants(exp, oriTerms, faultTerms, model);
	}
	else if (faultType == "ENFxLIF"){//双项双缺陷：ENFxLIF
		return generateENFxLIFCase1doublemutants(exp, oriTerms, faultTerms, model);
	}
	else if (faultType == "ENFxTOF"){//双项双缺陷：ENFxTOF
		return generateENFxTOFCase1doublemutants(exp, oriTerms, faultTerms, model);
	}

	else if (faultType == "ENFxLOF"){//双项双缺陷：ENFxLOF
		return generateENFxLOFCase1doublemutants(exp, oriTerms, faultTerms, model);
	}
	else if (faultType == "ENFxCORF"){//双项双缺陷：ENFxCORF
		return generateENFxCORFCase1doublemutants(exp, oriTerms, faultTerms, model);
	}

	else if (faultType == "DORFxTNF"){//双项双缺陷：DORFxTNF
		return generateTNFxDORFCase1doublemutants(exp, oriTerms, faultTerms, model);
	}
	else if (faultType == "DORFxLRF"){//双项双缺陷：DORFxLRF
		return generateDORFxLRFCase1doublemutants(exp, oriTerms, faultTerms, model);
	}
	else if (faultType == "DORFxDORF"){//双项双缺陷：DORFxDORF
		return generateDORFxDORFCase1doublemutants(exp, oriTerms, faultTerms, model);
	}
	else if (faultType == "DORFxLNF"){//双项双缺陷：DORFxLNF
		return generateDORFxLNFCase1doublemutants(exp, oriTerms, faultTerms, model);
	}
	else if (faultType == "ENFxLRF"){//双项双缺陷：ENFxLRF
		return generateENFxLRFCase1doublemutants(exp, oriTerms, faultTerms, model);
	}
	else if (faultType == "ENFxLNF"){//双项双缺陷：ENFxLNF
		return generateENFxLNFCase1doublemutants(exp, oriTerms, faultTerms, model);
	}
	else if (faultType == "ENFxTNF"){//双项双缺陷：ENFxTNF
		return generateENFxTNFCase1doublemutants(exp, oriTerms, faultTerms, model);
	}
	else if (faultType == "ENFxDORF"){//双项双缺陷：ENFxDORF
		return generateENFxDORFCase1doublemutants(exp, oriTerms, faultTerms, model);
	}
	else if (faultType == "ENFxENF"){//双项双缺陷：ENFxENF
		return generateENFxDORFCase1doublemutants(exp, oriTerms, faultTerms, model);
	}


	

	//==========================================  双项双缺陷 -end ==========================================

	else if (faultType == "LIORF" || faultType == "LIROF" || faultType == "LOIRF" || faultType == "LOIRF" || faultType == "LRIOF" || faultType == "LROIF"){
		return generateLIORFmutants(exp, faultTerms);
	}
	else if (faultType == "DALL"){
		Mutants a = generateDLIRFdoublemutants(exp);//LIF x LRF --
		a += generateDLRRFdoublemutants(exp);//LRF x LRF --
		a += generateDLORFdoublemutants(exp);//LOF x LRF --

		a += generateSingleTermLIFxLIFdoublemutants(exp);//LIF x LIF
		a += generateSingleTermLOFxLOFdoublemutants(exp);//LOF x LOF
		a += generateSingleTermLNFxLNFdoublemutants(exp);//LNF x LNF
		a += generateSingleTermLIFxLNFdoublemutants(exp);//LIF x LNF
		//LIFxLOF等价于LRF
		a += generateSingleTermLOFxLNFdoublemutants(exp);//LOF x LNF
		a += generateSingleTermLNFxLRFdoublemutants(exp);//LNF x LRF

		//词+项(TNF)相关：
		a += generateSingleTermLIFxTNFdoublemutants(exp);//LIF x TNF
		a += generateSingleTermLRFxTNFdoublemutants(exp);//LRF x TNF 
		a += generateSingleTermLOFxTNFdoublemutants(exp);//LOF x TNF
		a += generateSingleTermLNFxTNFdoublemutants(exp);//LNF x TNF

		//CORF + 词 相关：
		a += generateSingleTermCORFxLIFmutants(exp);
		a += generateSingleTermCORFxLOFmutants(exp);
		a += generateSingleTermCORFxLRFmutants(exp);
		a += generateSingleTermCORFxLNFmutants(exp);

		//CORF+TNF

		//DORF + 词 相关：
		a += generateSingleTermDORFxLIFmutants(exp);
		a += generateSingleTermDORFxLOFmutants(exp);
		a += generateSingleTermDORFxLNFmutants(exp);

		//=== 双项双缺陷 ===
		a += generateLIFxLIFdoublemutants(exp);//LIF x LIF
		a += generateLIFxLRFdoublemutants(exp);//LIF x LRF
		a += generateLRFxLRFdoublemutants(exp);//LRF x LRF
		a += generateDORFxLRFdoublemutants(exp);//DORF x LRF
		a += generateTOFxLRFdoublemutants(exp);//TOFxLRF

		return a;
	}
	else if (faultType == "ALL"){
		Mutants a = generateLIORFmutants(exp, faultTerms);
		a += generateTOFmutants(exp, faultTerms);//添加TOF变体
		a += generateLNFmutants(exp, faultTerms);//添加LNF变体
		a += generateCORFmutants(exp, faultTerms);//添加CORF变体
		a += generateDORFmutants(exp, faultTerms);//添加CORF变体
		a += generateTNFmutants(exp, faultTerms);//添加CORF变体
		a += generateENFmutants(exp);//添加CORF变体
		a += generateDLIRFdoublemutants(exp);//LIF x LRF --
		a += generateDLRRFdoublemutants(exp);//LRF x LRF --
		a += generateDLORFdoublemutants(exp);//LOF x LRF --

		a += generateSingleTermLIFxLIFdoublemutants(exp);//LIF x LIF
		a += generateSingleTermLOFxLOFdoublemutants(exp);//LOF x LOF
		a += generateSingleTermLNFxLNFdoublemutants(exp);//LNF x LNF
		a += generateSingleTermLIFxLNFdoublemutants(exp);//LIF x LNF
		//LIFxLOF等价于LRF
		a += generateSingleTermLOFxLNFdoublemutants(exp);//LOF x LNF
		a += generateSingleTermLNFxLRFdoublemutants(exp);//LNF x LRF

		//词+项(TNF)相关：
		a += generateSingleTermLIFxTNFdoublemutants(exp);//LIF x TNF
		a += generateSingleTermLRFxTNFdoublemutants(exp);//LRF x TNF 
		a += generateSingleTermLOFxTNFdoublemutants(exp);//LOF x TNF
		a += generateSingleTermLNFxTNFdoublemutants(exp);//LNF x TNF

		//CORF + 词 相关：
		a += generateSingleTermCORFxLIFmutants(exp);
		a += generateSingleTermCORFxLOFmutants(exp);
		a += generateSingleTermCORFxLRFmutants(exp);
		a += generateSingleTermCORFxLNFmutants(exp);

		//CORF+TNF

		//DORF + 词 相关：
		a += generateSingleTermDORFxLIFmutants(exp);
		a += generateSingleTermDORFxLOFmutants(exp);
		a += generateSingleTermDORFxLNFmutants(exp);

		//=== 双项双缺陷 ===
		a += generateLIFxLIFdoublemutants(exp);//LIF x LIF
		a += generateLIFxLRFdoublemutants(exp);//LIF x LRF
		a += generateLRFxLRFdoublemutants(exp);//LRF x LRF
		a += generateDORFxLRFdoublemutants(exp);//DORF x LRF
		a += generateTOFxLRFdoublemutants(exp);//TOFxLRF

		return a;
	}
	return Mutants("");
}


//根据类型生成变体，用于检测变体；coded by ZHANG XIAOXIONG

Mutants DnfMutant::generateSingleTermDORFxLRFmutants(string exp){
	Utility uti;
	Mutants mutants(exp);
	vector<string> terms = uti.extractDNFstyle(exp);
	vector<string> faultterms;
	string model = uti.generateModel(exp);

	for (int i = 0; i < terms.size() - 1; i++){
	
		faultterms.push_back(terms[i]);
		faultterms.push_back(terms[i+1]);

		mutants += generateDORFxLRFCase2doublemutants(exp, terms, faultterms, model);
		faultterms.clear();
	}

	return mutants;
}

//此四种有待补充

Mutants DnfMutant::generateSingleTermENFxLIFmutants(string exp){
	Utility uti;
	Mutants mutants(exp);
	vector<string> terms = uti.extractDNFstyle(exp);
	vector<string> faultterms;
	string model = uti.generateModel(exp);

	for (int i = 0; i < terms.size() - 1; i++){

		faultterms.push_back(terms[i]);
		faultterms.push_back(terms[i + 1]);

		mutants += generateDORFxLRFCase2doublemutants(exp, terms, faultterms, model);
		faultterms.clear();
	}

	return mutants;
}

Mutants DnfMutant::generateSingleTermENFxLOFmutants(string exp){
	Utility uti;
	Mutants mutants(exp);
	vector<string> terms = uti.extractDNFstyle(exp);
	vector<string> faultterms;
	string model = uti.generateModel(exp);

	for (int i = 0; i < terms.size() - 1; i++){

		faultterms.push_back(terms[i]);
		faultterms.push_back(terms[i + 1]);

		mutants += generateDORFxLRFCase2doublemutants(exp, terms, faultterms, model);
		faultterms.clear();
	}

	return mutants;
}

Mutants DnfMutant::generateSingleTermENFxLRFmutants(string exp){
	Utility uti;
	Mutants mutants(exp);
	vector<string> terms = uti.extractDNFstyle(exp);
	vector<string> faultterms;
	string model = uti.generateModel(exp);

	for (int i = 0; i < terms.size() - 1; i++){

		faultterms.push_back(terms[i]);
		faultterms.push_back(terms[i + 1]);

		mutants += generateDORFxLRFCase2doublemutants(exp, terms, faultterms, model);
		faultterms.clear();
	}

	return mutants;
}

Mutants DnfMutant::generateSingleTermENFxLNFmutants(string exp){
	Utility uti;
	Mutants mutants(exp);
	vector<string> terms = uti.extractDNFstyle(exp);
	vector<string> faultterms;
	string model = uti.generateModel(exp);

	for (int i = 0; i < terms.size() - 1; i++){

		faultterms.push_back(terms[i]);
		faultterms.push_back(terms[i + 1]);

		mutants += generateDORFxLRFCase2doublemutants(exp, terms, faultterms, model);
		faultterms.clear();
	}

	return mutants;
}

Mutants DnfMutant::generateSingleTermTNFxDORFmutants(string exp){
	Utility uti;
	Mutants mutants(exp);
	vector<string> terms = uti.extractDNFstyle(exp);
	vector<string> faultterms;
	string model = uti.generateModel(exp);

	for (int i = 0; i < terms.size() - 1; i++){

		faultterms.push_back(terms[i]);
		faultterms.push_back(terms[i + 1]);

		mutants += generateTNFxDORFCase2doublemutants(exp, terms, faultterms, model);
		faultterms.clear();
	}

	return mutants;
	
}

Mutants DnfMutant::generateSingleTermTNFxCORFmutants(string exp){
	Utility uti;
	Mutants mutants(exp);
	vector<string> terms = uti.extractDNFstyle(exp);
	vector<string> faultterms;
	string model = uti.generateModel(exp);
	for (int i = 0; i < terms.size(); i++){
		faultterms.push_back(terms[i]);
		mutants += generateTNFxCORFCase2doublemutants(exp, terms, faultterms, model);
		faultterms.clear();
	}
	return mutants;
}

Mutants DnfMutant::generateSingleTermDORFxDORFmutants(string exp){
	Utility uti;
	Mutants mutants(exp);
	vector<string> terms = uti.extractDNFstyle(exp);
	vector<string> faultterms;
	string model = uti.generateModel(exp);
	if (terms.size()>=3)
		for (int i = 0; i < terms.size()-2; i++){
			faultterms.push_back(terms[i]);
			faultterms.push_back(terms[i+1]);
			faultterms.push_back(terms[i+2]);
			mutants += generateDORFxDORFCase2doublemutants(exp, terms, faultterms, model);
			faultterms.clear();
		}
	return mutants;
}

Mutants DnfMutant::generateSingleTermDORFxCORFmutants(string exp){
	Utility uti;
	Mutants mutants(exp);
	vector<string> terms = uti.extractDNFstyle(exp);
	vector<string> faultterms;
	string model = uti.generateModel(exp);
	if (terms.size() >= 2)
		for (int i = 0; i < terms.size() - 1; i++){
			faultterms.push_back(terms[i]);
			faultterms.push_back(terms[i + 1]);
			mutants += generateDORFxCORFCase2doublemutants(exp, terms, faultterms, model);
			faultterms.clear();
		}
	return mutants;
}

Mutants DnfMutant::generateSingleTermCORFxCORFmutants(string exp){
	Utility uti;
	Mutants mutants(exp);
	vector<string> terms = uti.extractDNFstyle(exp);
	vector<string> faultterms;
	string model = uti.generateModel(exp);
	for (int i = 0; i < terms.size(); i++){
		faultterms.push_back(terms[i]);
		mutants += generateCORFxCORFCase2doublemutants(exp, terms, faultterms, model);
		faultterms.clear();
	}
	return mutants;
}

Mutants DnfMutant::generateSingleTermCORFxTNFmutants(string exp){
	Utility uti;
	Mutants mutants(exp);
	vector<string> terms = uti.extractDNFstyle(exp);
	vector<string> faultterms;
	string model = uti.generateModel(exp);
	for (int i = 0; i < terms.size(); i++){
		faultterms.push_back(terms[i]);
		mutants += generateCORFxTNFdoublemutants(exp, terms, faultterms, model);
		faultterms.clear();
	}
	return mutants;
}

Mutants DnfMutant::generateSingleTermCORFxTOFmutants(string exp){
	Utility uti;
	Mutants mutants(exp);
	vector<string> terms = uti.extractDNFstyle(exp);
	vector<string> faultterms;
	string model = uti.generateModel(exp);
	for (int i = 0; i < terms.size(); i++){
		faultterms.push_back(terms[i]);
		mutants += generateCORFxTOFdoublemutants(exp, terms, faultterms, model);
		faultterms.clear();
	}
	return mutants;
}

//双项双缺陷

Mutants DnfMutant::generateDoubleTermLIFxLOFmutants(string exp){
	Utility uti;
	Mutants mutants(exp);
	vector<string> terms = uti.extractDNFstyle(exp);
	vector<string> faultterms = uti.extractDNFstyle(exp);
	string model = uti.generateModel(exp);
	mutants += generateLOFxLIFdoublemutants(exp, terms, faultterms, model);
	return mutants;
}

Mutants DnfMutant::generateDoubleTermLIFxLNFmutants(string exp){
	Utility uti;
	Mutants mutants(exp);
	vector<string> terms = uti.extractDNFstyle(exp);
	vector<string> faultterms = uti.extractDNFstyle(exp);
	string model = uti.generateModel(exp);
	mutants += generateLNFxLIFdoublemutants(exp, terms, faultterms, model);
	return mutants;
}

Mutants DnfMutant::generateDoubleTermLOFxLRFmutants(string exp){
	Utility uti;
	Mutants mutants(exp);
	vector<string> terms = uti.extractDNFstyle(exp);
	vector<string> faultterms = uti.extractDNFstyle(exp);
	string model = uti.generateModel(exp);
	mutants += generateLOFxLRFdoublemutants(exp, terms, faultterms, model);
	return mutants;
}

Mutants DnfMutant::generateDoubleTermLNFxLRFmutants(string exp){
	Utility uti;
	Mutants mutants(exp);
	vector<string> terms = uti.extractDNFstyle(exp);
	vector<string> faultterms = uti.extractDNFstyle(exp);
	string model = uti.generateModel(exp);
	mutants += generateLNFxLRFdoublemutants(exp, terms, faultterms, model);
	return mutants;
}

Mutants DnfMutant::generateDoubleTermLOFxLOFmutants(string exp){
	Utility uti;
	Mutants mutants(exp);
	vector<string> terms = uti.extractDNFstyle(exp);
	vector<string> faultterms = uti.extractDNFstyle(exp);
	string model = uti.generateModel(exp);
	mutants += generateLOFxLOFdoublemutants(exp, terms, faultterms, model);
	return mutants;
}

Mutants DnfMutant::generateDoubleTermLNFxLOFmutants(string exp){
	Utility uti;
	Mutants mutants(exp);
	vector<string> terms = uti.extractDNFstyle(exp);
	vector<string> faultterms = uti.extractDNFstyle(exp);
	string model = uti.generateModel(exp);
	mutants += generateLNFxLOFdoublemutants(exp, terms, faultterms, model);
	return mutants;
}

Mutants DnfMutant::generateDoubleTermLNFxLNFmutants(string exp){
	Utility uti;
	Mutants mutants(exp);
	vector<string> terms = uti.extractDNFstyle(exp);
	vector<string> faultterms = uti.extractDNFstyle(exp);
	string model = uti.generateModel(exp);
	mutants += generateLNFxLNFdoublemutants(exp, terms, faultterms, model);
	return mutants;
}


//涉及DORF和词的全变体，生成不全，有待补充

Mutants DnfMutant::generateDoubleTermDORFxLIFmutants(string exp){
	Utility uti;
	Mutants mutants(exp);
	vector<string> terms = uti.extractDNFstyle(exp);
	vector<string> faultterms;
	string model = uti.generateModel(exp);
	if (terms.size() >= 3)
		for (int i = 0; i < terms.size() - 2; i++){
			faultterms.push_back(terms[i]);
			faultterms.push_back(terms[i + 1]);
			faultterms.push_back(terms[i + 2]);
			mutants += generateDORFxLIFCase1doublemutants(exp, terms, faultterms, model);
			faultterms.clear();
		}
	return mutants;
}

Mutants DnfMutant::generateDoubleTermDORFxLOFmutants(string exp){
	Utility uti;
	Mutants mutants(exp);
	vector<string> terms = uti.extractDNFstyle(exp);
	vector<string> faultterms;
	string model = uti.generateModel(exp);
	if (terms.size() >= 3)
		for (int i = 0; i < terms.size() - 2; i++){
			faultterms.push_back(terms[i]);
			faultterms.push_back(terms[i + 1]);
			faultterms.push_back(terms[i + 2]);
			mutants += generateDORFxLOFCase1doublemutants(exp, terms, faultterms, model);
			faultterms.clear();
		}
	return mutants;
}

Mutants DnfMutant::generateDoubleTermDORFxLNFmutants(string exp){
	Utility uti;
	Mutants mutants(exp);
	vector<string> terms = uti.extractDNFstyle(exp);
	vector<string> faultterms;
	string model = uti.generateModel(exp);
	if (terms.size() >= 3)
		for (int i = 0; i < terms.size() - 2; i++){
			faultterms.push_back(terms[i]);
			faultterms.push_back(terms[i + 1]);
			faultterms.push_back(terms[i + 2]);
			mutants += generateDORFxLNFCase1doublemutants(exp, terms, faultterms, model);
			faultterms.clear();
		}
	return mutants;
}

Mutants DnfMutant::generateDoubleTermTOFxLIFmutants(string exp){
	Utility uti;
	Mutants mutants(exp);
	vector<string> terms = uti.extractDNFstyle(exp);
	vector<string> faultterms = uti.extractDNFstyle(exp);
	string model = uti.generateModel(exp);
	mutants += generateTOFxLIFdoublemutants(exp, terms, faultterms, model);	
	return mutants;
}

Mutants DnfMutant::generateDoubleTermTOFxLOFmutants(string exp){
	Utility uti;
	Mutants mutants(exp);
	vector<string> terms = uti.extractDNFstyle(exp);
	vector<string> faultterms = uti.extractDNFstyle(exp);
	string model = uti.generateModel(exp);
	mutants += generateTOFxLOFdoublemutants(exp, terms, faultterms, model);
	return mutants;
}

Mutants DnfMutant::generateDoubleTermTOFxLNFmutants(string exp){
	Utility uti;
	Mutants mutants(exp);
	vector<string> terms = uti.extractDNFstyle(exp);
	vector<string> faultterms = uti.extractDNFstyle(exp);
	string model = uti.generateModel(exp);
	mutants += generateTOFxLNFdoublemutants(exp, terms, faultterms, model);
	return mutants;
}

Mutants DnfMutant::generateDoubleTermCORFxLIFmutants(string exp){
	Utility uti;
	Mutants mutants(exp);
	vector<string> terms = uti.extractDNFstyle(exp);
	vector<string> faultterms = uti.extractDNFstyle(exp);
	string model = uti.generateModel(exp);
	mutants += generateCORFxLIFCase1doublemutants(exp, terms, faultterms, model);
	return mutants;
}

Mutants DnfMutant::generateDoubleTermCORFxLRFmutants(string exp){
	Utility uti;
	Mutants mutants(exp);
	vector<string> terms = uti.extractDNFstyle(exp);
	vector<string> faultterms = uti.extractDNFstyle(exp);
	string model = uti.generateModel(exp);
	mutants += generateCORFxLRFCase1doublemutants(exp, terms, faultterms, model);
	return mutants;
}

Mutants DnfMutant::generateDoubleTermCORFxLOFmutants(string exp){
	Utility uti;
	Mutants mutants(exp);
	vector<string> terms = uti.extractDNFstyle(exp);
	vector<string> faultterms = uti.extractDNFstyle(exp);
	string model = uti.generateModel(exp);
	mutants += generateCORFxLOFCase1doublemutants(exp, terms, faultterms, model);
	return mutants;
}

Mutants DnfMutant::generateDoubleTermTNFxLIFmutants(string exp){
	Utility uti;
	Mutants mutants(exp);
	vector<string> terms = uti.extractDNFstyle(exp);
	vector<string> faultterms = uti.extractDNFstyle(exp);
	string model = uti.generateModel(exp);
	mutants += generateTNFxLIFTwoTermdoublemutants(exp, terms, faultterms, model);
	return mutants;
}

Mutants DnfMutant::generateDoubleTermTNFxLRFmutants(string exp){
	Utility uti;
	Mutants mutants(exp);
	vector<string> terms = uti.extractDNFstyle(exp);
	vector<string> faultterms = uti.extractDNFstyle(exp);
	string model = uti.generateModel(exp);
	mutants += generateTNFxLRFTwoTermdoublemutants(exp, terms, faultterms, model);
	return mutants;
}

Mutants DnfMutant::generateDoubleTermTNFxLOFmutants(string exp){
	Utility uti;
	Mutants mutants(exp);
	vector<string> terms = uti.extractDNFstyle(exp);
	vector<string> faultterms = uti.extractDNFstyle(exp);
	string model = uti.generateModel(exp);
	mutants += generateTNFxLOFTwoTermdoublemutants(exp, terms, faultterms, model);
	return mutants;
}

Mutants DnfMutant::generateDoubleTermTNFxLNFmutants(string exp){
	Utility uti;
	Mutants mutants(exp);
	vector<string> terms = uti.extractDNFstyle(exp);
	vector<string> faultterms = uti.extractDNFstyle(exp);
	string model = uti.generateModel(exp);
	mutants += generateTNFxLNFTwoTermdoublemutants(exp, terms, faultterms, model);
	return mutants;
}

Mutants DnfMutant::generateDoubleTermCORFxLNFmutants(string exp){
	Utility uti;
	Mutants mutants(exp);
	vector<string> terms = uti.extractDNFstyle(exp);
	vector<string> faultterms = uti.extractDNFstyle(exp);
	string model = uti.generateModel(exp);
	mutants += generateCORFxLNFCase1doublemutants(exp, terms, faultterms, model);
	return mutants;
}

Mutants DnfMutant::generateDoubleTermENFxLIFmutants(string exp){
	Utility uti;
	Mutants mutants(exp);
	vector<string> terms = uti.extractDNFstyle(exp);
	vector<string> faultterms = uti.extractDNFstyle(exp);
	string model = uti.generateModel(exp);
	mutants += generateENFxLIFCase1doublemutants(exp, terms, faultterms, model);
	return mutants;
}

Mutants DnfMutant::generateDoubleTermENFxLRFmutants(string exp){
	Utility uti;
	Mutants mutants(exp);
	vector<string> terms = uti.extractDNFstyle(exp);
	vector<string> faultterms = uti.extractDNFstyle(exp);
	string model = uti.generateModel(exp);
	mutants += generateENFxLRFCase1doublemutants(exp, terms, faultterms, model);
	return mutants;
}

Mutants DnfMutant::generateDoubleTermENFxLOFmutants(string exp){
	Utility uti;
	Mutants mutants(exp);
	vector<string> terms = uti.extractDNFstyle(exp);
	vector<string> faultterms = uti.extractDNFstyle(exp);
	string model = uti.generateModel(exp);
	mutants += generateENFxLOFCase1doublemutants(exp, terms, faultterms, model);
	return mutants;
}
Mutants DnfMutant::generateDoubleTermENFxLNFmutants(string exp){
	Utility uti;
	Mutants mutants(exp);
	vector<string> terms = uti.extractDNFstyle(exp);
	vector<string> faultterms = uti.extractDNFstyle(exp);
	string model = uti.generateModel(exp);
	mutants += generateENFxLNFCase1doublemutants(exp, terms, faultterms, model);
	return mutants;
}

Mutants DnfMutant::generateDoubleTermTOFxTOFmutants(string exp){
	Utility uti;
	Mutants mutants(exp);
	vector<string> terms = uti.extractDNFstyle(exp);
	vector<string> faultterms;
	string model = uti.generateModel(exp);
	for (int i = 0; i < terms.size()-1; i++){
		for (int j = i + 1; j < terms.size(); j++){
			faultterms.push_back(terms[i]);
			faultterms.push_back(terms[j]);
			mutants += generateTOFxTOFdoublemutants(exp, terms, faultterms);
			faultterms.clear();
		}

	}
	return mutants;
}

Mutants DnfMutant::generateDoubleTermTOFxDORFmutants(string exp){
	Utility uti;
	Mutants mutants(exp);
	vector<string> terms = uti.extractDNFstyle(exp);
	vector<string> faultterms;
	string model = uti.generateModel(exp);
	for (int i = 0; i < terms.size() - 1; i++){
		for (int j = 0; j < terms.size()&&j!=i &&j!=(i+1); j++){
			faultterms.push_back(terms[i]);
			faultterms.push_back(terms[i+1]);
			faultterms.push_back(terms[j]);
			mutants += generateTOFxDORFdoublemutantsZXX(exp, terms, faultterms, model);
			faultterms.clear();
		}

	}
	return mutants;
}

Mutants DnfMutant::generateDoubleTermTOFxCORFmutants(string exp){
	Utility uti;
	Mutants mutants(exp);
	vector<string> terms = uti.extractDNFstyle(exp);
	vector<string> faultterms;
	string model = uti.generateModel(exp);
	for (int i = 0; i < terms.size() - 1; i++){
		for (int j = i+1; j < terms.size(); j++){
			faultterms.push_back(terms[i]);
			faultterms.push_back(terms[j]);
			mutants += generateTOFxCORFdoublemutants(exp, terms, faultterms, model);
			faultterms.clear();
		}

	}
	return mutants;
}

Mutants DnfMutant::generateDoubleTermTOFxTNFmutants(string exp){
	Utility uti;
	Mutants mutants(exp);
	vector<string> terms = uti.extractDNFstyle(exp);
	vector<string> faultterms = uti.extractDNFstyle(exp);
	string model = uti.generateModel(exp);
	mutants += generateTNFxTOFdoublemutants(exp, terms, faultterms, model);
	return mutants;
}

Mutants DnfMutant::generateDoubleTermTNFxDORFmutants(string exp){
	Utility uti;
	Mutants mutants(exp);
	vector<string> terms = uti.extractDNFstyle(exp);
	vector<string> faultterms;
	string model = uti.generateModel(exp);
	for (int i = 0; i < terms.size() - 1; i++){
		for (int j = 0; j < terms.size() && j != i &&j != (i + 1); j++){
			faultterms.push_back(terms[i]);
			faultterms.push_back(terms[i + 1]);
			faultterms.push_back(terms[j]);
			mutants += generateTNFxDORFCase1doublemutantsZXX(exp, terms, faultterms, model);
			faultterms.clear();
		}

	}
	return mutants;
}

Mutants DnfMutant::generateDoubleTermTNFxTNFmutants(string exp){
	Utility uti;
	Mutants mutants(exp);
	vector<string> terms = uti.extractDNFstyle(exp);
	vector<string> faultterms = uti.extractDNFstyle(exp);
	string model = uti.generateModel(exp);
	mutants += generateTNFxTNFdoublemutants(exp, terms, faultterms, model);
	return mutants;
}

Mutants DnfMutant::generateDoubleTermTNFxCORFmutants(string exp){
	Utility uti;
	Mutants mutants(exp);
	vector<string> terms = uti.extractDNFstyle(exp);
	vector<string> faultterms = uti.extractDNFstyle(exp);
	string model = uti.generateModel(exp);
	for (int i = 0; i < terms.size()-1; i++){
		for (int j = i; j < terms.size(); j++){
			faultterms.push_back(terms[i]);
			faultterms.push_back(terms[j]);
			mutants += generateTNFxCORFCase1doublemutants(exp, terms, faultterms, model);
			faultterms.clear();
		}
	}
	return mutants;
}

Mutants DnfMutant::generateDoubleTermDORFxDORFmutants(string exp){
	Utility uti;
	Mutants mutants(exp);
	vector<string> terms = uti.extractDNFstyle(exp);
	vector<string> faultterms;
	string model = uti.generateModel(exp);
	for (int i = 0; i < terms.size() - 1; i++){
		for (int j = i+2; j < terms.size()-1; j++){
			faultterms.push_back(terms[i]);
			faultterms.push_back(terms[i + 1]);
			faultterms.push_back(terms[j]);
			faultterms.push_back(terms[j+1]);
			mutants += generateDORFxDORFCase1doublemutants(exp, terms, faultterms, model);
			faultterms.clear();
		}

	}
	return mutants;
}

Mutants DnfMutant::generateDoubleTermDORFxCORFmutants(string exp){
	Utility uti;
	Mutants mutants(exp);
	vector<string> terms = uti.extractDNFstyle(exp);
	vector<string> faultterms;
	string model = uti.generateModel(exp);
	for (int i = 0; i < terms.size() - 1; i++){
		for (int j = 0; j < terms.size() && j != i &&j != (i + 1); j++){
			faultterms.push_back(terms[i]);
			faultterms.push_back(terms[i + 1]);
			faultterms.push_back(terms[j]);
			mutants += generateDORFxDORFCase1doublemutants(exp, terms, faultterms, model);
			faultterms.clear();
		}

	}
	return mutants;
}

Mutants DnfMutant::generateDoubleTermCORFxCORFmutants(string exp){
	Utility uti;
	Mutants mutants(exp);
	vector<string> terms = uti.extractDNFstyle(exp);
	vector<string> faultterms;
	string model = uti.generateModel(exp);
	for (int i = 0; i < terms.size() - 1; i++){
		for (int j = i + 1; j < terms.size(); j++){
			faultterms.push_back(terms[i]);
			faultterms.push_back(terms[j]);
			mutants += generateCORFxCORFCase1doublemutants(exp, terms, faultterms, model);
			faultterms.clear();
		}

	}
	return mutants;
}


















Mutants DnfMutant::getMutants(string exp){
	Mutants a = generateLIORFmutants(exp);
	a += generateTOFmutants(exp);//添加TOF变体
	a += generateLNFmutants(exp);//添加LNF变体
	a += generateCORFmutants(exp);//添加CORF变体
	a += generateDORFmutants(exp);//添加CORF变体
	a += generateTNFmutants(exp);//添加CORF变体
	a += generateENFmutants(exp);//添加CORF变体

	//所有同项双缺陷类型
	//只与词有关
	a += generateDLIRFdoublemutants(exp);//添加同项LIF x LRF
	a += generateDLORFdoublemutants(exp);//添加同项LOF x LRF
	a += generateDLRRFdoublemutants(exp);//添加同项LRF x LRF
	a += generateSingleTermLIFxLIFdoublemutants(exp);//添加同项LIF x LIF
	a += generateSingleTermLOFxLOFdoublemutants(exp);//添加同项LOF x LOF
	a += generateSingleTermLNFxLNFdoublemutants(exp);//添加同项LNF x LNF
	a += generateSingleTermLIFxLNFdoublemutants(exp);//添加同项LIF x LNF
	a += generateSingleTermLOFxLNFdoublemutants(exp);//添加同项LOF x LNF
	a += generateSingleTermLNFxLRFdoublemutants(exp);//添加同项LNF x LRF

	//与词或项有关
	a += generateSingleTermLIFxTNFdoublemutants(exp);
	a += generateSingleTermLRFxTNFdoublemutants(exp);
	a += generateSingleTermLOFxTNFdoublemutants(exp);
	a += generateSingleTermLNFxTNFdoublemutants(exp);
	a += generateSingleTermCORFxLIFmutants(exp);
	a += generateSingleTermCORFxLOFmutants(exp);
    a += generateSingleTermCORFxLRFmutants(exp);
	a += generateSingleTermCORFxLNFmutants(exp);
	a += generateSingleTermDORFxLIFmutants(exp);
    a += generateSingleTermDORFxLOFmutants(exp);
    a += generateSingleTermDORFxLNFmutants(exp);
	a += generateSingleTermDORFxLRFmutants(exp);

	//只与项相关

	a += generateSingleTermTNFxDORFmutants(exp);
	a += generateSingleTermTNFxCORFmutants(exp);
	a += generateSingleTermDORFxDORFmutants(exp);
	a += generateSingleTermDORFxCORFmutants(exp);
	a += generateSingleTermCORFxCORFmutants(exp);
	a += generateSingleTermCORFxTNFmutants(exp);
	a += generateSingleTermCORFxTOFmutants(exp);

	//所有双项双缺陷类型
	//只与词有关
	a += generateLIFxLIFdoublemutants(exp);//LIF x LIF
	a += generateLIFxLRFdoublemutants(exp);//LIF x LRF
	a += generateLRFxLRFdoublemutants(exp);//LRF x LRF
	a += generateDoubleTermLIFxLOFmutants(exp);
	a += generateDoubleTermLIFxLNFmutants(exp);
	a += generateDoubleTermLOFxLRFmutants(exp);
	a += generateDoubleTermLNFxLRFmutants(exp);
	a += generateDoubleTermLOFxLOFmutants(exp);
	a += generateDoubleTermLNFxLOFmutants(exp);
	a += generateDoubleTermLNFxLNFmutants(exp);

	//与词和项有关
	a += generateDoubleTermDORFxLIFmutants(exp);
	a += generateDORFxLRFdoublemutants(exp);//DORF x LRF
	a += generateDoubleTermDORFxLOFmutants(exp);
	a += generateDoubleTermDORFxLNFmutants(exp);

	a += generateDoubleTermTOFxLIFmutants(exp);
	a += generateTOFxLRFdoublemutants(exp);//TOFxLRF
	a += generateDoubleTermTOFxLNFmutants(exp);
	a += generateDoubleTermTOFxLOFmutants(exp);

	a += generateDoubleTermCORFxLIFmutants(exp);
	a += generateDoubleTermCORFxLRFmutants(exp);
	a += generateDoubleTermCORFxLNFmutants(exp);
	a += generateDoubleTermCORFxLOFmutants(exp);

	a += generateDoubleTermTNFxLIFmutants(exp);
	a += generateDoubleTermTNFxLRFmutants(exp);
	a += generateDoubleTermTNFxLOFmutants(exp);
	a += generateDoubleTermTNFxLNFmutants(exp);

	a += generateDoubleTermENFxLIFmutants(exp);
	a += generateDoubleTermENFxLRFmutants(exp);
	a += generateDoubleTermENFxLOFmutants(exp);
	a += generateDoubleTermENFxLNFmutants(exp);

	//只和项有关
	a += generateDoubleTermTOFxTOFmutants(exp);
	a += generateDoubleTermTOFxDORFmutants(exp);
	a += generateDoubleTermTOFxCORFmutants(exp);
	a += generateDoubleTermTOFxTNFmutants(exp);


	a += generateDoubleTermTNFxDORFmutants(exp);
	a += generateDoubleTermTNFxCORFmutants(exp);
	a += generateDoubleTermTNFxTNFmutants(exp);

	a += generateDoubleTermDORFxDORFmutants(exp);
	a += generateDoubleTermDORFxCORFmutants(exp);
	a += generateDoubleTermCORFxCORFmutants(exp);

	return a;
}






Mutants DnfMutant::getMutants(string exp, string faultType)
{
	if (faultType == "LIF"){
		return generateLIFmutants(exp);
	}
	if (faultType == "LOF"){
		return generateLOFmutants(exp);
	}
	else if (faultType == "LNF"){
		return generateLNFmutants(exp);
	}
	else if (faultType == "LRF"){
		return generateLRFmutants(exp);
	}
	else if (faultType == "TOF"){
		return generateTOFmutants(exp);
	}
	else if (faultType == "TNF"){
		return generateTNFmutants(exp);
	}
	else if (faultType == "CORF"){
		return generateCORFmutants(exp);
	}
	else if (faultType == "DORF"){
		return generateDORFmutants(exp);
	}
	else if (faultType == "ENF"){
		return generateENFmutants(exp);
	}
	else if (faultType == "SALL"){//SALL：所有单缺陷
		Mutants a = generateLIORFmutants(exp);
		a += generateTOFmutants(exp);//添加TOF变体
		a += generateLNFmutants(exp);//添加LNF变体
		a += generateCORFmutants(exp);//添加CORF变体
		a += generateDORFmutants(exp);//添加CORF变体
		a += generateTNFmutants(exp);//添加CORF变体
		a += generateENFmutants(exp);//添加CORF变体
		return a;
	}
	//单项双缺陷
	else if (faultType == "DLIRF"){//生成变体生成重复缺陷表达式
		return generateDLIRFdoublemutants(exp);
	}
	else if (faultType == "DLORF"){
		return generateDLORFdoublemutants(exp);
	}
	else if (faultType == "DLRRF"){
		return generateDLRRFdoublemutants(exp);
	}
	else if (faultType == "DLIIF"){
		return generateSingleTermLIFxLIFdoublemutants(exp);
	}
	else if (faultType == "DLOOF"){
		return generateSingleTermLOFxLOFdoublemutants(exp);
	}
	else if (faultType == "DLNNF"){
		return generateSingleTermLNFxLNFdoublemutants(exp);
	}
	else if (faultType == "DLINF"){
		return generateSingleTermLIFxLNFdoublemutants(exp);
	}
	else if (faultType == "DLONF"){
		return generateSingleTermLOFxLNFdoublemutants(exp);
	}
	else if (faultType == "DLNRF"){
		return generateSingleTermLNFxLRFdoublemutants(exp);
	}
	else if (faultType == "DLITNF"){
		return generateSingleTermLIFxTNFdoublemutants(exp);
	}
	else if (faultType == "DLRTNF"){
		return generateSingleTermLRFxTNFdoublemutants(exp);
	}
	else if (faultType == "DLOTNF"){
		return generateSingleTermLOFxTNFdoublemutants(exp);
	}
	else if (faultType == "DLNTNF"){
		return generateSingleTermLNFxTNFdoublemutants(exp);
	}
	else if (faultType == "DLICORF"){
		return generateSingleTermCORFxLIFmutants(exp);
	}
	else if (faultType == "DLOCORF"){
		return generateSingleTermCORFxLOFmutants(exp);
	}
	else if (faultType == "DLRCORF"){
		return generateSingleTermCORFxLRFmutants(exp);
	}
	else if (faultType == "DLNCORF"){
		return generateSingleTermCORFxLNFmutants(exp);
	}
	else if (faultType == "DLIDORF"){
		return generateSingleTermDORFxLIFmutants(exp);
	}
	else if (faultType == "DLODORF"){
		return generateSingleTermDORFxLOFmutants(exp);
	}
	else if (faultType == "DLNDORF"){
		return generateSingleTermDORFxLNFmutants(exp);
	}

	//双项双缺陷
	else if (faultType == "LIFxLIF")//双项双缺陷1：LIFxLIF 
		return generateLIFxLIFdoublemutants(exp);
	else if (faultType == "LIFxLRF")//双项双缺陷2：LIFxLRF
		return generateLIFxLRFdoublemutants(exp);
	else if (faultType == "LRFxLRF")//双项双缺陷3：LRFxLRF
		return generateLRFxLRFdoublemutants(exp);
	else if (faultType == "DORFxLRF")//双项双缺陷4：DORFxLRF
		return generateDORFxLRFdoublemutants(exp);
	else if (faultType == "TOFxLRF")//双项双缺陷5：TOFxLRF
		return generateTOFxLRFdoublemutants(exp);

	else if (faultType == "LIORF" || faultType == "LIROF" || faultType == "LOIRF" || faultType == "LOIRF" || faultType == "LRIOF" || faultType == "LROIF"){
		return generateLIORFmutants(exp);
	}
	if (faultType == "ALL"){
	
		Mutants mutants = generateLIORFmutants(exp);
		mutants += generateTOFmutants(exp);//添加TOF变体
		mutants += generateLNFmutants(exp);//添加LNF变体
		mutants += generateCORFmutants(exp);//添加CORF变体
		mutants += generateDORFmutants(exp);//添加CORF变体
		mutants += generateTNFmutants(exp);//添加CORF变体
		mutants += generateENFmutants(exp);//添加CORF变体
		////同时实现单双项双缺陷的变体 
		//mutants += generateDLIRFdoublemutants(exp);//LIF x LRF --
		//mutants += generateDLRRFdoublemutants(exp);//LRF x LRF --
		//mutants += generateDLORFdoublemutants(exp);//LOF x LRF --

		////实现三类双项双缺陷的变体
		//mutants += generateLIFxLIFdoublemutants(exp);//LIF x LIF
		//mutants += generateLIFxLRFdoublemutants(exp);//LIF x LRF
		//mutants += generateLRFxLRFdoublemutants(exp);//LRF x LRF

		////实现四类单项双缺陷的变体
		//mutants += generateSingleTermLIFxLIFdoublemutants(exp);//LIF x LIF
		//mutants += generateSingleTermLOFxLOFdoublemutants(exp);//LOF x LOF
		//mutants += generateSingleTermLNFxLNFdoublemutants(exp);//LNF x LNF
		//mutants += generateSingleTermLIFxLNFdoublemutants(exp);//LIF x LNF
		////LIFxLOF等价于LRF
		//mutants += generateSingleTermLOFxLNFdoublemutants(exp);//LOF x LNF
		//mutants += generateSingleTermLNFxLRFdoublemutants(exp);//LNF x LRF

		////词+项(TNF)相关：
		//mutants += generateSingleTermLIFxTNFdoublemutants(exp);//LIF x TNF
		//mutants += generateSingleTermLRFxTNFdoublemutants(exp);//LRF x TNF 
		//mutants += generateSingleTermLOFxTNFdoublemutants(exp);//LOF x TNF
		//mutants += generateSingleTermLNFxTNFdoublemutants(exp);//LNF x TNF

		////CORF + 词 相关：
		//mutants += generateSingleTermCORFxLIFmutants(exp);
		//mutants += generateSingleTermCORFxLOFmutants(exp);
		//mutants += generateSingleTermCORFxLRFmutants(exp);
		//mutants += generateSingleTermCORFxLNFmutants(exp);

		////CORF+TNF

		////DORF + 词 相关：
		//mutants += generateSingleTermDORFxLIFmutants(exp);
		//mutants += generateSingleTermDORFxLOFmutants(exp);
		//mutants += generateSingleTermDORFxLNFmutants(exp);

		////=== 双项双缺陷 ===
		//mutants += generateLIFxLIFdoublemutants(exp);//LIF x LIF
		//mutants += generateLIFxLRFdoublemutants(exp);//LIF x LRF
		//mutants += generateLRFxLRFdoublemutants(exp);//LRF x LRF
		//mutants += generateDORFxLRFdoublemutants(exp);//DORF x LRF
		//mutants += generateTOFxLRFdoublemutants(exp);//TOFxLRF




		////与词或项有关
		//mutants += generateSingleTermLIFxTNFdoublemutants(exp);
		//mutants += generateSingleTermLRFxTNFdoublemutants(exp);
		//mutants += generateSingleTermLOFxTNFdoublemutants(exp);
		//mutants += generateSingleTermLNFxTNFdoublemutants(exp);
		//mutants += generateSingleTermCORFxLIFmutants(exp);
		//mutants += generateSingleTermCORFxLOFmutants(exp);
		//mutants += generateSingleTermCORFxLRFmutants(exp);
		//mutants += generateSingleTermCORFxLNFmutants(exp);
		//mutants += generateSingleTermDORFxLIFmutants(exp);
		//mutants += generateSingleTermDORFxLOFmutants(exp);
		//mutants += generateSingleTermDORFxLNFmutants(exp);
		//mutants += generateSingleTermDORFxLRFmutants(exp);

		//////只与项相关

		//mutants += generateSingleTermTNFxDORFmutants(exp);
		//mutants += generateSingleTermTNFxCORFmutants(exp);
		//mutants += generateSingleTermDORFxDORFmutants(exp);
		//mutants += generateSingleTermDORFxCORFmutants(exp);
		//mutants += generateSingleTermCORFxCORFmutants(exp);
		//mutants += generateSingleTermCORFxTNFmutants(exp);
		//mutants += generateSingleTermCORFxTOFmutants(exp);

		//////所有双项双缺陷类型
		//////只与词有关
		//mutants += generateLIFxLIFdoublemutants(exp);//LIF x LIF
		//mutants += generateLIFxLRFdoublemutants(exp);//LIF x LRF
		//mutants += generateLRFxLRFdoublemutants(exp);//LRF x LRF
		//mutants += generateDoubleTermLIFxLOFmutants(exp);
		//mutants += generateDoubleTermLIFxLNFmutants(exp);
		//mutants += generateDoubleTermLOFxLRFmutants(exp);
		//mutants += generateDoubleTermLNFxLRFmutants(exp);
		//mutants += generateDoubleTermLOFxLOFmutants(exp);
		//mutants += generateDoubleTermLNFxLOFmutants(exp);
		//mutants += generateDoubleTermLNFxLNFmutants(exp);

		//////与词和项有关
		//mutants += generateDoubleTermDORFxLIFmutants(exp);
		//mutants += generateDORFxLRFdoublemutants(exp);//DORF x LRF
		//mutants += generateDoubleTermDORFxLOFmutants(exp);
		//mutants += generateDoubleTermDORFxLNFmutants(exp);

		//mutants += generateDoubleTermTOFxLIFmutants(exp);
		//mutants += generateTOFxLRFdoublemutants(exp);//TOFxLRF
		//mutants += generateDoubleTermTOFxLNFmutants(exp);
		//mutants += generateDoubleTermTOFxLOFmutants(exp);

		//mutants += generateDoubleTermCORFxLIFmutants(exp);
		//mutants += generateDoubleTermCORFxLRFmutants(exp);
		//mutants += generateDoubleTermCORFxLNFmutants(exp);
		//mutants += generateDoubleTermCORFxLOFmutants(exp);

		//mutants += generateDoubleTermTNFxLIFmutants(exp);
		//mutants += generateDoubleTermTNFxLRFmutants(exp);
		//mutants += generateDoubleTermTNFxLOFmutants(exp);
		//mutants += generateDoubleTermTNFxLNFmutants(exp);

		//mutants += generateDoubleTermENFxLIFmutants(exp);
		//mutants += generateDoubleTermENFxLRFmutants(exp);
		//mutants += generateDoubleTermENFxLOFmutants(exp);
		//mutants += generateDoubleTermENFxLNFmutants(exp);

		//////只和项有关
		////mutants += generateDoubleTermTOFxTOFmutants(exp);
		//mutants += generateDoubleTermTOFxDORFmutants(exp);
		//mutants += generateDoubleTermTOFxCORFmutants(exp);
		//mutants += generateDoubleTermTOFxTNFmutants(exp);


		//mutants += generateDoubleTermTNFxDORFmutants(exp);
		//mutants += generateDoubleTermTNFxCORFmutants(exp);
	 //   mutants += generateDoubleTermTNFxTNFmutants(exp);

		//mutants += generateDoubleTermDORFxDORFmutants(exp);
		//mutants += generateDoubleTermDORFxCORFmutants(exp);
		//mutants += generateDoubleTermCORFxCORFmutants(exp);

		return mutants;

	}
	return Mutants("");
}

//双项双缺陷1：LIFxLIF
Mutants DnfMutant::generateLIFxLIFdoublemutants(string exp){

	Mutants mutants(exp);
	bool vars[26];
	for (size_t i = 0; i < 26; i++)
	{
		vars[i] = false;
	}

	string oriExp(exp);
	Utility uti;
	vector<string> terms;
	vector<vector<string>> terms_literals;
	vector<string> oriterms;
	vector<string> faultterms;
	uti.mutantsPreproccess(oriExp, terms, terms_literals, vars);
	//开始生成变体
	for (size_t i = 0; i < terms.size() - 1; i++)
	{
		for (size_t j = i + 1; j < terms.size(); j++)//两层循环取两项
		{
			oriterms.clear();
			oriterms = { terms[i], terms[j] };
			for (size_t k = 0; k < 26; k++)
			{
				for (size_t l = 0; l < 26; l++)
				{
					wchar_t toReplaceWith1 = 'a' + k;
					wchar_t toReplaceWith2 = 'a' + l;
					if ((vars[k] && terms[i].find(toReplaceWith1) == string::npos) && (vars[l] && terms[j].find(toReplaceWith2) == string::npos))
					{
						string s11, s12, s21, s22;
						s11 += toReplaceWith1;
						s12 += '!';
						s12 += toReplaceWith1;
						s21 += toReplaceWith2;
						s22 += '!';
						s22 += toReplaceWith2;
						string new_term11 = (terms[i] + s11);
						string new_term12 = (terms[i] + s12);
						string new_term21 = (terms[j] + s21);
						string new_term22 = (terms[j] + s22);
						string m;
						string _mutant;
						//变体1
						_mutant = uti.replaceDoubleTerms(oriExp, i, j, new_term11, new_term21);
						m.assign(_mutant);
						faultterms = { new_term11, new_term21 };
						mutants.addMutant(m, "LIFxLIF", oriterms, faultterms);
						//变体2
						_mutant = uti.replaceDoubleTerms(oriExp, i, j, new_term12, new_term21);
						m.assign(_mutant);
						faultterms = { new_term12, new_term21 };
						mutants.addMutant(m, "LIFxLIF", oriterms, faultterms);
						faultterms.clear();
						//变体3
						_mutant = uti.replaceDoubleTerms(oriExp, i, j, new_term11, new_term22);
						m.assign(_mutant);
						faultterms = { new_term11, new_term22 };
						mutants.addMutant(m, "LIFxLIF", oriterms, faultterms);
						faultterms.clear();
						//变体4
						_mutant = uti.replaceDoubleTerms(oriExp, i, j, new_term12, new_term22);
						m.assign(_mutant);
						faultterms = { new_term12, new_term22 };
						mutants.addMutant(m, "LIFxLIF", oriterms, faultterms);
						faultterms.clear();
					}
				}
			}
		}
	}
	return mutants;

}

//双项双缺陷2：LIFxLRF
Mutants DnfMutant::generateLIFxLRFdoublemutants(string exp)
{
	Mutants mutants(exp);
	bool vars[26];
	for (size_t i = 0; i < 26; i++)
	{
		vars[i] = false;
	}

	string oriExp(exp);
	Utility uti;
	vector<string> terms;
	vector<vector<string>> terms_literals;
	vector<string> oriterms;
	vector<string> faultterms;
	uti.mutantsPreproccess(oriExp, terms, terms_literals, vars);
	//开始生成变体
	for (size_t i = 0; i < terms.size() - 1; i++)
	{
		for (size_t j = i + 1; j < terms.size(); j++)//两层循环取两项
		{
			oriterms.clear();
			oriterms = { terms[i], terms[j] };
			for (size_t k = 0; k < 26; k++)
			{
				for (size_t l = 0; l < 26; l++)
				{
					wchar_t toReplaceWith1 = 'a' + k;
					wchar_t toReplaceWith2 = 'a' + l;
					if ((vars[k] && terms[i].find(toReplaceWith1) == string::npos) && (vars[l] && terms[j].find(toReplaceWith2) == string::npos))
					{
						string m;
						string _mutant;
						string s11, s12, s21, s22;
						s11 += toReplaceWith1;
						s12 += '!';
						s12 += toReplaceWith1;
						s21 += toReplaceWith2;
						s22 += '!';
						s22 += toReplaceWith2;
						string new_term11 = (terms[i] + s11);
						string new_term12 = (terms[i] + s12);
						string new_term21 = (terms[j] + s21);
						string new_term22 = (terms[j] + s22);
						//第一项LRF，第二项LIF
						for (size_t p = 0; p < terms_literals[i].size(); p++)
						{
							string new_term13 = terms[i];

							int literal_pos = new_term13.find(terms_literals[i].at(p));
							new_term13.replace(new_term13.begin() + literal_pos, new_term13.begin() + literal_pos + terms_literals[i].at(p).size(), s11);
							//第二项LIF
							_mutant = uti.replaceDoubleTerms(oriExp, i, j, new_term13, new_term21);
							m.assign(_mutant);
							faultterms = { new_term13, new_term21 };
							mutants.addMutant(m, "LIFxLRF", oriterms, faultterms);
							faultterms.clear();
							//第二项LIF非
							_mutant = uti.replaceDoubleTerms(oriExp, i, j, new_term13, new_term22);
							m.assign(_mutant);
							faultterms = { new_term13, new_term22 };
							mutants.addMutant(m, "LIFxLRF", oriterms, faultterms);
							faultterms.clear();
						}
						//第一项LRF非，第二项LIF
						for (size_t p = 0; p < terms_literals[i].size(); p++)
						{
							string new_term14 = terms[i];
							int literal_pos = new_term14.find(terms_literals[i].at(p));
							new_term14.replace(new_term14.begin() + literal_pos, new_term14.begin() + literal_pos + terms_literals[i].at(p).size(), s12);
							//第二项LIF
							_mutant = uti.replaceDoubleTerms(oriExp, i, j, new_term14, new_term21);
							m.assign(_mutant);
							faultterms = { new_term14, new_term21 };
							mutants.addMutant(m, "LIFxLRF", oriterms, faultterms);
							faultterms.clear();
							//第二项LIF非
							_mutant = uti.replaceDoubleTerms(oriExp, i, j, new_term14, new_term22);
							m.assign(_mutant);
							faultterms = { new_term14, new_term22 };
							mutants.addMutant(m, "LIFxLRF", oriterms, faultterms);
							faultterms.clear();
						}
						//第一项LIF，第二项LRF
						for (size_t p = 0; p < terms_literals[j].size(); p++)
						{
							string new_term23 = terms[j];

							int literal_pos = new_term23.find(terms_literals[j].at(p));
							new_term23.replace(new_term23.begin() + literal_pos, new_term23.begin() + literal_pos + terms_literals[j].at(p).size(), s21);
							//第一项LIF
							_mutant = uti.replaceDoubleTerms(oriExp, i, j, new_term11, new_term23);
							m.assign(_mutant);
							faultterms = { new_term11, new_term23 };
							mutants.addMutant(m, "LIFxLRF", oriterms, faultterms);
							faultterms.clear();
							//第一项LIF非
							_mutant = uti.replaceDoubleTerms(oriExp, i, j, new_term12, new_term23);
							m.assign(_mutant);
							faultterms = { new_term12, new_term23 };
							mutants.addMutant(m, "LIFxLRF", oriterms, faultterms);
							faultterms.clear();
						}
						//第一项LIF，第二项LRF非
						for (size_t p = 0; p < terms_literals[j].size(); p++)
						{
							string new_term24 = terms[j];
							int literal_pos = new_term24.find(terms_literals[j].at(p));
							new_term24.replace(new_term24.begin() + literal_pos, new_term24.begin() + literal_pos + terms_literals[j].at(p).size(), s22);
							//第一项LIF
							_mutant = uti.replaceDoubleTerms(oriExp, i, j, new_term11, new_term24);
							m.assign(_mutant);
							faultterms = { new_term11, new_term24 };
							mutants.addMutant(m, "LIFxLRF", oriterms, faultterms);
							faultterms.clear();
							//第一项LIF非
							_mutant = uti.replaceDoubleTerms(oriExp, i, j, new_term12, new_term24);
							m.assign(_mutant);
							faultterms = { new_term12, new_term24 };
							mutants.addMutant(m, "LIFxLRF", oriterms, faultterms);
							faultterms.clear();
						}
					}
				}
			}
		}
	}
	return mutants;
}

//双项双缺陷3：LRFxLRF
Mutants DnfMutant::generateLRFxLRFdoublemutants(string exp)
{
	Mutants mutants(exp);
	bool vars[26];
	for (size_t i = 0; i < 26; i++)
	{
		vars[i] = false;
	}

	string oriExp(exp);
	Utility uti;
	vector<string> terms;
	vector<vector<string>> terms_literals;
	vector<string> oriterms;
	vector<string> faultterms;
	uti.mutantsPreproccess(oriExp, terms, terms_literals, vars);

	//开始生成变体
	size_t count = 0;

	for (size_t i = 0; i < terms.size() - 1; i++)
	{
		for (size_t j = i + 1; j < terms.size(); j++)
		{
			oriterms.clear();
			oriterms = { terms[i], terms[j] };
			vector<string> literals_in_this_term1 = uti.extractCNFstyle(terms[i]);
			vector<string> literals_in_this_term2 = uti.extractCNFstyle(terms[j]);
			for (size_t k = 0; k < 26; k++)
			{
				wchar_t toReplaceWith1 = 'a' + k;
				string s11, s12;
				s11 += toReplaceWith1;
				s12 += '!';
				s12 += toReplaceWith1;

				if (vars[k] && terms[i].find(toReplaceWith1) == string::npos)
				{

					for (size_t l = 0; l < literals_in_this_term1.size(); l++)
					{
						string new_term11 = terms[i];
						string new_term12 = terms[i];
						int literal_pos1 = new_term11.find(literals_in_this_term1[l]);
						new_term11.replace(new_term11.begin() + literal_pos1, new_term11.begin() + literal_pos1 + literals_in_this_term1[l].size(), s11);
						new_term12.replace(new_term12.begin() + literal_pos1, new_term12.begin() + literal_pos1 + literals_in_this_term1[l].size(), s12);

						for (size_t m = 0; m < 26; m++)
						{
							wchar_t toReplaceWith2 = 'a' + m;
							string s21, s22;
							s21 += toReplaceWith2;
							s22 += '!';
							s22 += toReplaceWith2;
							if (vars[m] && terms[j].find(toReplaceWith2) == string::npos)
							{
								for (size_t n = 0; n < literals_in_this_term2.size(); n++)
								{
									string new_term21 = terms[j];
									string new_term22 = terms[j];
									int literal_pos2 = new_term21.find(literals_in_this_term2[n]);
									new_term21.replace(new_term21.begin() + literal_pos2, new_term21.begin() + literal_pos2 + literals_in_this_term2[n].size(), s21);
									new_term22.replace(new_term22.begin() + literal_pos2, new_term22.begin() + literal_pos2 + literals_in_this_term2[n].size(), s22);

									string m;
									string _mutant;
									//变体1
									_mutant = uti.replaceDoubleTerms(oriExp, i, j, new_term11, new_term21);
									m.assign(_mutant);
									faultterms = { new_term11, new_term21 };
									mutants.addMutant(m, "LRFxLRF", oriterms, faultterms);
									faultterms.clear();
									//变体2
									_mutant = uti.replaceDoubleTerms(oriExp, i, j, new_term12, new_term21);
									m.assign(_mutant);
									faultterms = { new_term12, new_term21 };
									mutants.addMutant(m, "LRFxLRF", oriterms, faultterms);
									faultterms.clear();
									//变体3
									_mutant = uti.replaceDoubleTerms(oriExp, i, j, new_term11, new_term22);
									m.assign(_mutant);
									faultterms = { new_term11, new_term22 };
									mutants.addMutant(m, "LRFxLRF", oriterms, faultterms);
									faultterms.clear();
									//变体4
									_mutant = uti.replaceDoubleTerms(oriExp, i, j, new_term12, new_term22);
									m.assign(_mutant);
									faultterms = { new_term12, new_term22 };
									mutants.addMutant(m, "LRFxLRF", oriterms, faultterms);
									faultterms.clear();
									count += 4;
								}
							}
						}
					}
				}
			}
		}
	}
	return mutants;
}

//双项双缺陷4：DORFxLRF变体
Mutants DnfMutant::generateDORFxLRFdoublemutants(string exp)
{
	Mutants mutants(exp);
	bool vars[26];
	for (size_t i = 0; i < 26; i++)
	{
		vars[i] = false;
	}

	string oriExp(exp);
	Utility uti;
	vector<string> terms;
	vector<vector<string>> terms_literals;
	vector<string> oriterms;
	vector<string> faultterms;
	uti.mutantsPreproccess(oriExp, terms, terms_literals, vars);
	for (size_t i = 0; i < terms.size(); i++)//【遍历term】
	{
		int pos = oriExp.find(terms[i]);
		for (size_t j = 0; j < 26; j++)
		{
			wchar_t toReplaceWith = 'a' + j;
			string s1, s2;
			s1 += toReplaceWith;
			s2 += '!';
			s2 += toReplaceWith;

			if (vars[j] && terms[i].find(toReplaceWith) == string::npos)
			{
				for (size_t k = 0; k < terms_literals.at(i).size(); k++)//替换第k个词
				{
					string new_term1;
					string new_term2;

					for (size_t ll = 0; ll < terms_literals.at(i).size(); ll++)//拼接词组成新的项
					{
						if (ll == k)
						{
							new_term1 += s1;
							new_term2 += s2;
						}
						else
						{
							new_term1 += terms_literals.at(i).at(ll);
							new_term2 += terms_literals.at(i).at(ll);
						}
					}

					for (size_t ii = 0; ii < terms.size() - 1; ii++)//选择两项合并
					{
						if (ii == i || ii == i - 1)continue;
						string combine_term = terms[ii] + terms[ii + 1];
						oriterms.clear();
						oriterms = { terms[i], terms[ii], terms[ii + 1] };
						string _mutant1;
						string _mutant2;
						for (size_t t = 0; t < terms.size(); t++)
						{
							if (t == i)
							{
								_mutant1 += new_term1;
								_mutant2 += new_term2;
							}
							else if (t == ii)
							{
								_mutant1 += combine_term;
								_mutant2 += combine_term;
							}
							else if (t != ii + 1)
							{
								_mutant1 += terms[t];
								_mutant2 += terms[t];
							}
							if (t != terms.size() - 1 && t != ii)
							{
								_mutant1 += '+';
								_mutant2 += '+';
							}
						}
						string m1, m2;
						m1.assign(_mutant1);
						m2.assign(_mutant2);
						faultterms = { new_term1, combine_term };
						mutants.addMutant(m1, "DORFxLRF", oriterms, faultterms);
						faultterms.clear();
						faultterms = { new_term2, combine_term };
						mutants.addMutant(m2, "DORFxLRF", oriterms, faultterms);
						faultterms.clear();
					}
				}
			}
		}
	}
	return mutants;
}

//双项双缺陷5：TOFxLRF变体
Mutants DnfMutant::generateTOFxLRFdoublemutants(string exp)
{
	Mutants mutants(exp);
	bool vars[26];
	for (size_t i = 0; i < 26; i++)
	{
		vars[i] = false;
	}

	string oriExp(exp);
	Utility uti;
	vector<string> terms;
	vector<vector<string>> terms_literals;
	vector<string> oriterms;
	vector<string> faultterms;
	uti.mutantsPreproccess(oriExp, terms, terms_literals, vars);
	for (size_t i = 0; i < terms.size(); i++)
	{
		int pos = oriExp.find(terms[i]);
		for (size_t j = 0; j < 26; j++)
		{
			wchar_t toReplaceWith = 'a' + j;
			string s1, s2;
			s1 += toReplaceWith;
			s2 += '!';
			s2 += toReplaceWith;

			if (vars[j] && terms[i].find(toReplaceWith) == string::npos)
			{
				for (size_t k = 0; k < terms_literals.at(i).size(); k++)//替换第k个词
				{
					string new_term1;
					string new_term2;
					for (size_t ll = 0; ll < terms_literals.at(i).size(); ll++)//拼接词组成新的项
					{
						if (ll == k)
						{
							new_term1 += s1;
							new_term2 += s2;
						}
						else
						{
							new_term1 += terms_literals.at(i).at(ll);
							new_term2 += terms_literals.at(i).at(ll);
						}
					}

					for (size_t ii = 0; ii < terms.size(); ii++)//选择一项遗漏
					{
						if (ii == i)continue;

						oriterms.clear();
						oriterms = { terms[i], terms[ii] };

						string _mutant1;
						string _mutant2;
						for (size_t t = 0; t<terms.size(); t++)
						{
							if (t == i)
							{
								if (_mutant1.size()>0)
								{
									_mutant1 += '+';
									_mutant2 += '+';
								}
								_mutant1 += new_term1;
								_mutant2 += new_term2;
							}
							else if (t != ii)
							{
								if (_mutant1.size() > 0)
								{
									_mutant1 += '+';
									_mutant2 += '+';
								}
								_mutant1 += terms[t];
								_mutant2 += terms[t];

							}
						}
						string m1, m2;
						m1.assign(_mutant1);
						m2.assign(_mutant2);
						faultterms = { new_term1 };
						mutants.addMutant(m1, "TOFxLRF", oriterms, faultterms);
						faultterms.clear();
						faultterms = { new_term2 };
						mutants.addMutant(m2, "TOFxLRF", oriterms, faultterms);
						faultterms.clear();
					}
				}
			}
		}
	}
	return mutants;
}


//单项双缺陷：LOF x LRF
Mutants DnfMutant::generateDLORFdoublemutants(string exp)
{
	Mutants mutants(exp);
	bool vars[26];
	for (size_t i = 0; i < 26; i++)
	{
		vars[i] = false;
	}

	string oriExp(exp);
	Utility uti;
	vector<string> terms;
	vector<vector<string>> terms_literals;
	vector<string> oriterms;
	vector<string> faultterms;
	uti.mutantsPreproccess(oriExp, terms, terms_literals, vars);
	for (size_t i = 0; i < terms.size(); i++)//【遍历term】
	{
		oriterms.clear();
		oriterms = { terms[i] };
		int pos = oriExp.find(terms[i]);
		for (size_t j = 0; j < 26; j++)
		{
			char toReplaceWith = 'a' + j;
			string s1, s2;
			s1 += toReplaceWith;
			s2 += '!';
			s2 += toReplaceWith;

			if (vars[j] && terms[i].find(toReplaceWith) == string::npos)//不在当前term中的literal
			{
				//【遍历term的literal】
				for (size_t k = 0; k < terms_literals.at(i).size() - 1; k++)//替换第k个词
				{
					for (size_t l = k + 1; l < terms_literals.at(i).size(); l++)//遗漏第L个词
					{
						if (l == k)
							continue;

						string new_term1;
						string new_term2;
						for (size_t ll = 0; ll < terms_literals.at(i).size(); ll++)//拼接词组成新的项
						{
							if (ll == l)
								continue;

							if (ll == k)
							{
								new_term1 += s1;
								new_term2 += s2;
							}
							else
							{
								new_term1 += terms_literals.at(i).at(ll);
								new_term2 += terms_literals.at(i).at(ll);
							}
						}

						string _mutant1 = oriExp;
						_mutant1.replace(_mutant1.begin() + pos, _mutant1.begin() + pos + terms[i].size(), new_term1);
						string _mutant2 = oriExp;
						_mutant2.replace(_mutant2.begin() + pos, _mutant2.begin() + pos + terms[i].size(), new_term2);

						string m1, m2;
						m1.append(_mutant1);
						m2.append(_mutant2);
						faultterms = { new_term1 };
						mutants.addMutant(m1, "DLORF", oriterms, faultterms);
						faultterms.clear();
						faultterms = { new_term2 };
						mutants.addMutant(m2, "DLORF", oriterms, faultterms);
						faultterms.clear();
					}
				}
			}
		}
	}
	return mutants;
}

//单项双缺陷：LIF x LRF
Mutants DnfMutant::generateDLIRFdoublemutants(string exp)
{
	Mutants mutants(exp);
	bool vars[26];
	for (int i = 0; i < 26; i++)
	{
		vars[i] = false;
	}

	string oriExp(exp);//原表达式
	Utility uti;
	vector<string> terms;//项
	vector<vector<string>> terms_literals;//项中的词
	vector<string> oriterms;
	vector<string> faultterms;
	uti.mutantsPreproccess(oriExp, terms, terms_literals, vars);//对表达式进行预处理，给参数中的项赋值

	for (int i = 0; i < terms.size(); i++)//以term为单位，分别处理
	{
		oriterms.clear();
		oriterms = { terms[i] };
		vector<string> literals_in_this_term = uti.extractCNFstyle(terms[i]);
		int pos = oriExp.find(terms[i]);//当前项在表达式中的位置（从0开始计数）
		for (int j = 0; j < 26; j++)//遍历26个字母
		{
			char toReplaceWith = 'a' + j;
			string s11, s12;
			s11 += toReplaceWith;
			s12 += '!';
			s12 += toReplaceWith;

			if (vars[j] && terms[i].find(toReplaceWith) == string::npos)//表达式中存在 & 当前term中不存在 的词——vars[j]
			{
				for (int k = 0; k < literals_in_this_term.size(); k++)//遍历当前term中存在的词——literals_in_this_term[k]
				{
					string new_term11 = terms[i];
					string new_term12 = terms[i];
					int literal_pos = new_term11.find(literals_in_this_term[k]);//词在当前项中的位置
					new_term11.replace(new_term11.begin() + literal_pos, new_term11.begin() + literal_pos + literals_in_this_term[k].size(), s11);
					new_term12.replace(new_term12.begin() + literal_pos, new_term12.begin() + literal_pos + literals_in_this_term[k].size(), s12);

					for (int l = 0; l < 26; l++)
					{
						char toReplaceWith2 = 'a' + l;
						string s21, s22;
						s21 += toReplaceWith2;
						s22 += '!';
						s22 += toReplaceWith2;
						if (vars[l] && new_term11.find(toReplaceWith2) == string::npos && terms[i].find(toReplaceWith2) == string::npos)
						{
							string new_term21 = (new_term11 + s21);
							string new_term22 = (new_term11 + s22);
							string new_term23 = (new_term12 + s21);
							string new_term24 = (new_term12 + s22);

							string m1;
							string _mutant1 = oriExp;
							//变体1
							_mutant1.replace(_mutant1.begin() + pos, _mutant1.begin() + pos + terms[i].size(), new_term21);
							m1.append(_mutant1);
							faultterms = { new_term21 };
							mutants.addMutant(m1, "DLIRF", oriterms, faultterms);
							faultterms.clear();

							string m2;
							string _mutant2 = oriExp;
							//变体1
							_mutant2.replace(_mutant2.begin() + pos, _mutant2.begin() + pos + terms[i].size(), new_term22);
							m2.append(_mutant2);
							faultterms = { new_term22 };
							mutants.addMutant(m2, "DLIRF", oriterms, faultterms);
							faultterms.clear();

							string m3;
							string _mutant3 = oriExp;
							//变体1
							_mutant3.replace(_mutant3.begin() + pos, _mutant3.begin() + pos + terms[i].size(), new_term23);
							m3.append(_mutant3);
							faultterms = { new_term23 };
							mutants.addMutant(m3, "DLIRF", oriterms, faultterms);
							faultterms.clear();

							string m4;
							string _mutant4 = oriExp;
							//变体1
							_mutant4.replace(_mutant4.begin() + pos, _mutant4.begin() + pos + terms[i].size(), new_term24);
							m4.append(_mutant4);
							faultterms = { new_term24 };
							mutants.addMutant(m4, "DLIRF", oriterms, faultterms);
							faultterms.clear();
						}
					}
				}
			}
		}
	}
	return mutants;
}

//单项双缺陷：LRFxLRF
Mutants DnfMutant::generateDLRRFdoublemutants(string exp)
{
	Mutants mutants(exp);
	bool vars[26];
	for (size_t i = 0; i < 26; i++)
	{
		vars[i] = false;
	}

	string oriExp(exp);
	Utility uti;
	vector<string> terms;
	vector<vector<string>> terms_literals;
	vector<string> oriterms;
	vector<string> faultterms;
	uti.mutantsPreproccess(oriExp, terms, terms_literals, vars);

	for (size_t i = 0; i < terms.size(); i++)//【遍历term】
	{
		oriterms.clear();
		oriterms = {terms[i]};
		vector<string> literals_in_this_term = uti.extractCNFstyle(terms[i]);//获取当前项中的词
		int pos = oriExp.find(terms[i]);//项在表达式中开始的位置
		string new_term11 = terms[i];
		string new_term12 = terms[i];
		for (size_t j = 0; j < 26; j++)//【遍历不在当前项中的词literals】
		{
			char toReplaceWith = 'a' + j;
			string s11, s12;
			s11 += toReplaceWith;//不在当前项中的词literals
			s12 += '!';
			s12 += toReplaceWith;
			if (vars[j] && terms[i].find(toReplaceWith) == string::npos)//不在当前项中的词literals
			{
				for (size_t k = 0; k < literals_in_this_term.size(); k++)//【遍历当前term中的literal】
				{
					new_term11 = terms[i];//当前项
					new_term12 = terms[i];

					//第一次 LRF
					int literal_pos = new_term11.find(literals_in_this_term[k]);
					new_term11.replace(new_term11.begin() + literal_pos, new_term11.begin() + literal_pos + literals_in_this_term[k].size(), s11);
					new_term12.replace(new_term12.begin() + literal_pos, new_term12.begin() + literal_pos + literals_in_this_term[k].size(), s12);

					for (size_t l = j + 1; l < 26; l++)//遍历other term剩下的词literal
					{
						//int pos1 = oriExp.find(new_term11);
						char toReplaceWith2 = 'a' + l;
						string s21, s22;
						s21 += toReplaceWith2;
						s22 += '!';
						s22 += toReplaceWith2;
						if (vars[l] && new_term11.find(toReplaceWith2) == string::npos && terms[i].find(toReplaceWith2) == string::npos)
						{
							for (size_t m = k + 1; m < literals_in_this_term.size(); m++)
							{
								int literal_pos1 = new_term11.find(literals_in_this_term[m]);
								int literal_pos2 = new_term12.find(literals_in_this_term[m]);
								if (-1 != literal_pos1 && (literal_pos1 != literal_pos)
									&& (-1 != literal_pos2) && (literal_pos2 != literal_pos))
								{
									string new_term21, new_term22, new_term23, new_term24;
									new_term21 = new_term11;
									new_term22 = new_term11;
									new_term23 = new_term12;
									new_term24 = new_term12;
									new_term21.replace(new_term21.begin() + literal_pos1, new_term21.begin() + literal_pos1 + literals_in_this_term[m].size(), s21);
									new_term22.replace(new_term22.begin() + literal_pos1, new_term22.begin() + literal_pos1 + literals_in_this_term[m].size(), s22);
									new_term23.replace(new_term23.begin() + literal_pos2, new_term23.begin() + literal_pos2 + literals_in_this_term[m].size(), s21);
									new_term24.replace(new_term24.begin() + literal_pos2, new_term24.begin() + literal_pos2 + literals_in_this_term[m].size(), s22);
									string m1;
									string _mutant1 = oriExp;
									//变体1
									_mutant1.replace(_mutant1.begin() + pos, _mutant1.begin() + pos + terms[i].size(), new_term21);
									m1.append(_mutant1);
									faultterms = { new_term21 };
									mutants.addMutant(m1, "DLRRF", oriterms, faultterms);
									faultterms.clear();

									string m2;
									string _mutant2 = oriExp;
									//变体2
									_mutant2.replace(_mutant2.begin() + pos, _mutant2.begin() + pos + terms[i].size(), new_term22);
									m2.append(_mutant2);
									faultterms = { new_term22 };
									mutants.addMutant(m2, "DLRRF", oriterms, faultterms);
									faultterms.clear();

									string m3;
									string _mutant3 = oriExp;
									//变体3
									_mutant3.replace(_mutant3.begin() + pos, _mutant3.begin() + pos + terms[i].size(), new_term23);
									m3.append(_mutant3);
									faultterms = { new_term23 };
									mutants.addMutant(m3, "DLRRF", oriterms, faultterms);
									faultterms.clear();

									string m4;
									string _mutant4 = oriExp;
									//变体4
									_mutant4.replace(_mutant4.begin() + pos, _mutant4.begin() + pos + terms[i].size(), new_term24);
									m4.append(_mutant4);
									faultterms = { new_term24 };
									mutants.addMutant(m4, "DLRRF", oriterms, faultterms);
									faultterms.clear();
								}
							}
						}
					}
				}
			}
		}
	}
	return mutants;
}

//单项双缺陷：LIFxLIF 
Mutants DnfMutant::generateSingleTermLIFxLIFdoublemutants(string exp)
{
	Mutants mutants(exp);
	bool vars[26];
	for (int i = 0; i < 26; i++)
	{
		vars[i] = false;
	}

	string oriExp(exp);
	Utility uti;
	vector<string> terms;
	vector<vector<string>> terms_literals;
	vector<string> oriterms;
	vector<string> faultterms;
	uti.mutantsPreproccess(oriExp, terms, terms_literals, vars);

	for (int i = 0; i < terms.size(); i++)//【遍历term】
	{
		oriterms.clear();
		oriterms = {terms[i]};
		int pos = oriExp.find(terms[i]);//项在表达式中开始的位置
		string new_term11 = terms[i];
		string new_term12 = terms[i];

		for (int j = 0; j < 26; j++)//【遍历不在当前项中的词literals】
		{
			char toInsert1 = 'a' + j;
			if (vars[j] && terms[i].find(toInsert1) == string::npos)//不在当前项中的词literal
			{
				string s11, s12;
				s11 += toInsert1;//不在当前项中的词literals
				s12 += '!';
				s12 += toInsert1;

				new_term11 = terms[i];//当前项
				new_term12 = terms[i];

				//第一次LIF
				new_term11 += s11;
				new_term12 += s12;

				for (int k = j + 1; k < 26; k++)//遍历other term剩下的词literal
				{
					char toInsert2 = 'a' + k;
					if (vars[k] && terms[i].find(toInsert2) == string::npos)
					{
						string s21, s22;
						s21 += toInsert2;
						s22 += '!';
						s22 += toInsert2;

						string new_term21, new_term22, new_term23, new_term24;
						new_term21 = new_term11;
						new_term22 = new_term11;
						new_term23 = new_term12;
						new_term24 = new_term12;

						//第二次LIF
						new_term21 += s21;
						new_term22 += s22;
						new_term23 += s21;
						new_term24 += s22;

						string m1;
						string _mutant1 = oriExp;
						//变体1
						_mutant1.replace(_mutant1.begin() + pos, _mutant1.begin() + pos + terms[i].size(), new_term21);
						m1.append(_mutant1);
						faultterms = { new_term21 };
						mutants.addMutant(m1, "DLIIF", oriterms, faultterms);
						faultterms.clear();

						string m2;
						string _mutant2 = oriExp;
						//变体2
						_mutant2.replace(_mutant2.begin() + pos, _mutant2.begin() + pos + terms[i].size(), new_term22);
						m2.append(_mutant2);
						faultterms = { new_term22 };
						mutants.addMutant(m2, "DLIIF", oriterms, faultterms);
						faultterms.clear();

						string m3;
						string _mutant3 = oriExp;
						//变体3
						_mutant3.replace(_mutant3.begin() + pos, _mutant3.begin() + pos + terms[i].size(), new_term23);
						m3.append(_mutant3);
						faultterms = { new_term23 };
						mutants.addMutant(m3, "DLIIF", oriterms, faultterms);
						faultterms.clear();

						string m4;
						string _mutant4 = oriExp;
						//变体4
						_mutant4.replace(_mutant4.begin() + pos, _mutant4.begin() + pos + terms[i].size(), new_term24);
						m4.append(_mutant4);
						faultterms = { new_term24 };
						mutants.addMutant(m4, "DLIIF", oriterms, faultterms);
						faultterms.clear();
					}
				}
			}
		}
	}
	return mutants;
}


//单项双缺陷：LOFxLOF 
Mutants DnfMutant::generateSingleTermLOFxLOFdoublemutants(string exp)
{
	Mutants mutants(exp);
	bool vars[26];
	for (int i = 0; i < 26; i++)
	{
		vars[i] = false;
	}
	string oriExp(exp);
	Utility uti;
	vector<string> terms;
	vector<vector<string>> terms_literals;
	vector<string> oriterms;
	vector<string> faultterms;
	uti.mutantsPreproccess(oriExp, terms, terms_literals, vars);

	for (int i = 0; i < terms.size(); i++)//【遍历term】
	{
		oriterms.clear();
		oriterms = {terms[i]};
		int pos = oriExp.find(terms[i]);//第i个term的开始位置
		string term = terms[i];
		if (term.size() > 2)//项的长度大于2时，才可以遗漏
		{
			for (int j = 0; j < terms_literals.at(i).size(); j++)//遗漏第j个literal
			{
				//第一次：LOF
				string new_term1 = term;
				string s1 = terms_literals.at(i).at(j);

				int pos1 = new_term1.find(s1);
				new_term1.replace(pos1, s1.size(), "");

				for (int k = j + 1; k < terms_literals.at(i).size(); k++)//遗漏第k个词
				{
					//第二次：LOF
					string new_term2 = new_term1;
					string s2 = terms_literals.at(i).at(k);

					int pos2 = new_term1.find(s2);
					new_term2.replace(pos2, s2.size(), "");

					string _mutant1 = oriExp;
					_mutant1.replace(_mutant1.begin() + pos, _mutant1.begin() + pos + terms[i].size(), new_term2);
					faultterms = { new_term2 };
					mutants.addMutant(_mutant1, "DLOOF", oriterms, faultterms);
					faultterms.clear();
				}
			}
		}
	}
	return mutants;
}

//单项双缺陷：LIFxLNF （先LNF，再LIF）
Mutants DnfMutant::generateSingleTermLIFxLNFdoublemutants(string exp)
{
	Mutants mutants(exp);
	bool vars[26];
	for (int i = 0; i < 26; i++)
	{
		vars[i] = false;
	}
	string oriExp(exp);
	Utility uti;
	vector<string> terms;
	vector<vector<string>> terms_literals;
	vector<string> oriterms;
	vector<string> faultterms;
	uti.mutantsPreproccess(oriExp, terms, terms_literals, vars);

	for (int i = 0; i < terms.size(); i++)//【遍历term】
	{
		oriterms.clear();
		oriterms = { terms[i] };
		int pos = oriExp.find(terms[i]);//第i个term的开始位置
		string term = terms[i];

		for (int j = 0; j < terms_literals.at(i).size(); j++)//遍历literal
		{
			//第一步：LNF
			string temp_term1 = term;
			string s1 = terms_literals.at(i).at(j);//第j个literal
			string s2 = uti.getLiteralNegation(s1);//获取s1单词的Negation
			int pos1 = temp_term1.find(s1);
			temp_term1.replace(pos1, s1.size(), s2);

			//第二步：LIF
			for (int j = 0; j < 26; j++)
			{
				char toInsert = 'a' + j;
				if (vars[j] && terms[i].find(toInsert) == string::npos)//不在当前项中的词literal
				{
					string s3, s4;
					s3 += toInsert;
					s4 += '!';
					s4 += toInsert;

					string temp_term3 = temp_term1;
					string temp_term4 = temp_term1;
					temp_term3 += s3;
					temp_term4 += s4;

					string _mutant1 = oriExp;
					_mutant1.replace(_mutant1.begin() + pos, _mutant1.begin() + pos + terms[i].size(), temp_term3);
					faultterms = { temp_term3 };
					mutants.addMutant(_mutant1, "DLINF", oriterms, faultterms);
					faultterms.clear();

					string _mutant2 = oriExp;
					_mutant2.replace(_mutant2.begin() + pos, _mutant2.begin() + pos + terms[i].size(), temp_term4);
					faultterms = { temp_term4 };
					mutants.addMutant(_mutant2, "DLINF", oriterms, faultterms);
					faultterms.clear();
				}
			}
		}
	}
	return mutants;
}

//单项双缺陷：LOFxLNF （先LOF，再LNF，否则与单缺陷LNF等价） 
Mutants DnfMutant::generateSingleTermLOFxLNFdoublemutants(string exp)
{
	Mutants mutants(exp);
	bool vars[26];
	for (int i = 0; i < 26; i++)
	{
		vars[i] = false;
	}
	string oriExp(exp);
	Utility uti;
	vector<string> terms;
	vector<vector<string>> terms_literals;
	vector<string> oriterms;
	vector<string> faultterms;
	uti.mutantsPreproccess(oriExp, terms, terms_literals, vars);

	for (int i = 0; i < terms.size(); i++)//【遍历term】
	{
		oriterms.clear();
		oriterms = { terms[i] };
		int pos = oriExp.find(terms[i]);//第i个term的开始位置
		string term = terms[i];

		for (int j = 0; j < terms_literals.at(i).size(); j++)//遍历literal
		{
			//第一步：LOF
			string temp_term = term;
			string s1 = terms_literals.at(i).at(j);
			int pos1 = temp_term.find(s1);
			temp_term.replace(pos1, s1.size(), "");

			vector<string> terms_literal2 = terms_literals.at(i);
			terms_literal2.erase(terms_literal2.begin() + j);//删除第j个元素

			//第二步：LNF
			for (int k = 0; k < terms_literal2.size(); k++)
			{
				string temp_term2 = temp_term;
				string s2 = terms_literal2.at(k);//第k个literal
				string s3 = uti.getLiteralNegation(s2);//获取单词的Negation
				int pos2 = temp_term2.find(s2);
				temp_term2.replace(pos2, s3.size(), s3);

				string _mutant1 = oriExp;
				_mutant1.replace(_mutant1.begin() + pos, _mutant1.begin() + pos + terms[i].size(), temp_term2);
				faultterms = { temp_term2 };
				mutants.addMutant(_mutant1, "DLONF", oriterms, faultterms);
				faultterms.clear();
			}
		}
	}
	return mutants;
}

//单项双缺陷：LNFxLRF  
Mutants DnfMutant::generateSingleTermLNFxLRFdoublemutants(string exp)
{
	Mutants mutants(exp);
	bool vars[26];
	for (int i = 0; i < 26; i++)
	{
		vars[i] = false;
	}
	string oriExp(exp);
	Utility uti;
	vector<string> terms;
	vector<vector<string>> terms_literals;
	vector<string> oriterms;
	vector<string> faultterms;
	uti.mutantsPreproccess(oriExp, terms, terms_literals, vars);

	for (int i = 0; i < terms.size(); i++)//【遍历term】
	{
		oriterms.clear();
		oriterms = { terms[i] };
		int pos = oriExp.find(terms[i]);//第i个term的开始位置
		string term = terms[i];

		for (int j = 0; j < terms_literals.at(i).size(); j++)//遍历literal
		{
			//第一步：LNF
			string temp_term1 = term;
			string s1 = terms_literals.at(i).at(j);//第j个literal
			string s2 = uti.getLiteralNegation(s1);//获取单词的Negation
			int pos1 = temp_term1.find(s1);
			temp_term1.replace(pos1, s1.size(), s2);//temp_term1为 LNF 后的项

			//第二步：LRF
			for (int p = 0; p < terms_literals.at(i).size(); p++)//替换第p个literal（不能是上边LNF的那个词）
			{
				if (p == j)
					continue;
				for (int k = 0; k < 26; k++)
				{
					char toReplaceWith = 'a' + k;
					if (vars[k] && terms[i].find(toReplaceWith) == string::npos)//不在当前term中的literal
					{
						string s3, s4;
						s3 += toReplaceWith;
						s4 += '!';
						s4 += toReplaceWith;

						string temp_literals = terms_literals.at(i).at(p);//要被替换的单词
						int pos1 = temp_term1.find(temp_literals);

						string temp_term2 = temp_term1;
						string temp_term3 = temp_term1;

						temp_term2.replace(temp_term2.begin() + pos1, temp_term2.begin() + pos1 + temp_literals.size(), s3);
						temp_term3.replace(temp_term3.begin() + pos1, temp_term3.begin() + pos1 + temp_literals.size(), s4);

						string _mutant1 = oriExp;
						_mutant1.replace(_mutant1.begin() + pos, _mutant1.begin() + pos + terms[i].size(), temp_term2);
						faultterms = { temp_term2 };
						mutants.addMutant(_mutant1, "DLNRF", oriterms, faultterms);
						faultterms.clear();

						string _mutant2 = oriExp;
						_mutant2.replace(_mutant2.begin() + pos, _mutant2.begin() + pos + terms[i].size(), temp_term3);
						faultterms = { temp_term3 };
						mutants.addMutant(_mutant2, "DLNRF", oriterms, faultterms);
						faultterms.clear();
					}
				}
			}
		}
	}
	return mutants;
}

//单项双缺陷：LNFxLNF  
Mutants DnfMutant::generateSingleTermLNFxLNFdoublemutants(string exp)
{
	Mutants mutants(exp);
	bool vars[26];
	for (int i = 0; i < 26; i++)
	{
		vars[i] = false;
	}
	string oriExp(exp);
	Utility uti;
	vector<string> terms;
	vector<vector<string>> terms_literals;
	vector<string> oriterms;
	vector<string> faultterms;
	uti.mutantsPreproccess(oriExp, terms, terms_literals, vars);

	for (int i = 0; i < terms.size(); i++)//【遍历term】
	{
		oriterms.clear();
		oriterms = { terms[i] };
		int pos = oriExp.find(terms[i]);//第i个term的开始位置
		string term = terms[i];
		if (term.size() > 2)//项的长度大于2时，才可以LNF,否则会变为本身
		{
			for (int j = 0; j < terms_literals.at(i).size(); j++)//遍历literal
			{
				//第一步：LNF
				string temp_term1 = term;
				string s1 = terms_literals.at(i).at(j);//第j个literal
				string s2 = uti.getLiteralNegation(s1);//获取单词的Negation
				int pos1 = temp_term1.find(s1);
				temp_term1.replace(pos1, s1.size(), s2);//temp_term1为 LNF 后的项

				//第二步：LNF
				for (int p = j + 1; p < terms_literals.at(i).size(); p++)
				{
					string temp_term2 = temp_term1;
					string temp_term3 = temp_term1;

					string s3 = terms_literals.at(i).at(p);//第p个literal
					string s4 = uti.getLiteralNegation(s3);//获取单词的Negation
					int pos2 = temp_term2.find(s3);
					temp_term2.replace(pos2, s3.size(), s4);//temp_term1为 LNF 后的项

					temp_term3.replace(temp_term3.begin() + pos2, temp_term3.begin() + pos2 + s3.size(), s4);

					string _mutant1 = oriExp;
					_mutant1.replace(_mutant1.begin() + pos, _mutant1.begin() + pos + terms[i].size(), temp_term3);
					faultterms = { temp_term3 };
					mutants.addMutant(_mutant1, "DLNNF", oriterms, faultterms);
					faultterms.clear();
				}
			}
		}
	}
	return mutants;
}


//单项双缺陷：（词+项）LIF x TNF
Mutants DnfMutant::generateSingleTermLIFxTNFdoublemutants(string exp){//LIF x TNF
	Mutants mutants(exp);
	bool vars[26];
	for (int i = 0; i < 26; i++) {
		vars[i] = false;
	}

	string oriExp(exp);
	Utility uti;
	vector<string> terms;
	vector<vector<string>> terms_literals;
	vector<string> oriterms;
	vector<string> faultterms;
	uti.mutantsPreproccess(oriExp, terms, terms_literals, vars);

	for (int i = 0; i < terms.size(); i++)//【遍历term】
	{
		oriterms.clear();
		oriterms = { terms[i] };
		int pos = oriExp.find(terms[i]);//项在表达式中开始的位置
		string new_term11 = terms[i];
		string new_term12 = terms[i];

		for (int j = 0; j < 26; j++)//【遍历不在当前项中的词literals】
		{
			char toInsert1 = 'a' + j;
			if (vars[j] && terms[i].find(toInsert1) == string::npos)//不在当前项中的词literal
			{
				string s11, s12;
				s11 += toInsert1;//不在当前项中的词literals
				s12 += '!';
				s12 += toInsert1;

				new_term11 = terms[i];//当前项
				new_term12 = terms[i];

				//LIF
				new_term11 += s11;
				new_term12 += s12;

				//TNF
				string new_term21, new_term22;
				new_term21 = "!(" + new_term11 + ")";
				new_term22 = "!(" + new_term12 + ")";


				string m1;
				string _mutant1 = oriExp;
				//变体1
				_mutant1.replace(_mutant1.begin() + pos, _mutant1.begin() + pos + terms[i].size(), new_term21);
				m1.append(_mutant1);
				faultterms = { new_term21 };
				mutants.addMutant(m1, "LIFxTNF", oriterms, faultterms);
				faultterms.clear();

				string m2;
				string _mutant2 = oriExp;
				//变体2
				_mutant2.replace(_mutant2.begin() + pos, _mutant2.begin() + pos + terms[i].size(), new_term22);
				m2.append(_mutant2);
				faultterms = { new_term22 };
				mutants.addMutant(m2, "DLINF", oriterms, faultterms);
				faultterms.clear();

			}
		}
	}
	return mutants;
}

//单项双缺陷：（词+项）LRFxTNF
Mutants DnfMutant::generateSingleTermLRFxTNFdoublemutants(string exp)
{
	Mutants mutants(exp);
	bool vars[26];
	for (size_t i = 0; i < 26; i++)
	{
		vars[i] = false;
	}

	string oriExp(exp);
	Utility uti;
	vector<string> terms;
	vector<vector<string>> terms_literals;
	vector<string> oriterms;
	vector<string> faultterms;
	uti.mutantsPreproccess(oriExp, terms, terms_literals, vars);

	//遍历term
	for (size_t i = 0; i < terms.size(); i++)
	{
		oriterms.clear();
		oriterms = { terms[i] };
		vector<string> literals_in_this_term = uti.extractCNFstyle(terms[i]);//获取当前项中的词
		int pos = oriExp.find(terms[i]);//项在表达式中开始的位置
		string new_term11 = terms[i];
		string new_term12 = terms[i];

		for (size_t j = 0; j < 26; j++)//【遍历不在当前项中的词literals】
		{
			char toReplaceWith = 'a' + j;
			string s11, s12;
			s11 += toReplaceWith;//不在当前项中的词literals
			s12 += '!';
			s12 += toReplaceWith;
			if (vars[j] && terms[i].find(toReplaceWith) == string::npos)//不在当前项中的词literals
			{
				for (size_t k = 0; k < literals_in_this_term.size(); k++)//【遍历当前term中的literal】
				{
					new_term11 = terms[i];//当前项
					new_term12 = terms[i];

					//LRF
					int literal_pos = new_term11.find(literals_in_this_term[k]);
					new_term11.replace(new_term11.begin() + literal_pos, new_term11.begin() + literal_pos + literals_in_this_term[k].size(), s11);
					new_term12.replace(new_term12.begin() + literal_pos, new_term12.begin() + literal_pos + literals_in_this_term[k].size(), s12);

					//LNF
					string new_term21, new_term22;
					new_term21 = "!(" + new_term11 + ")";
					new_term22 = "!(" + new_term12 + ")";

					string m1;
					string _mutant1 = oriExp;
					//变体1
					_mutant1.replace(_mutant1.begin() + pos, _mutant1.begin() + pos + terms[i].size(), new_term21);
					m1.append(_mutant1);
					faultterms = { new_term21 };
					mutants.addMutant(m1, "DLRNF", oriterms, faultterms);
					faultterms.clear();

					string m2;
					string _mutant2 = oriExp;
					//变体2
					_mutant2.replace(_mutant2.begin() + pos, _mutant2.begin() + pos + terms[i].size(), new_term22);
					m2.append(_mutant2);
					faultterms = { new_term22 };
					mutants.addMutant(m2, "DLRNF", oriterms, faultterms);
					faultterms.clear();
				}
			}
		}
	}
	return mutants;
}


//单项双缺陷：（词+项）LOFxTNF
Mutants DnfMutant::generateSingleTermLOFxTNFdoublemutants(string exp)
{
	Mutants mutants(exp);
	bool vars[26];
	for (int i = 0; i < 26; i++)
	{
		vars[i] = false;
	}
	string oriExp(exp);
	Utility uti;
	vector<string> terms;
	vector<vector<string>> terms_literals;
	vector<string> oriterms;
	vector<string> faultterms;
	uti.mutantsPreproccess(oriExp, terms, terms_literals, vars);

	for (int i = 0; i < terms.size(); i++)//【遍历term】
	{
		oriterms.clear();
		oriterms = { terms[i] };
		int pos = oriExp.find(terms[i]);//第i个term的开始位置
		string term = terms[i];

		for (int j = 0; j < terms_literals.at(i).size(); j++)//遍历literal
		{
			//第一步：LOF
			string temp_term = term;
			string s1 = terms_literals.at(i).at(j);
			int pos1 = temp_term.find(s1);
			temp_term.replace(pos1, s1.size(), "");

			//TNF
			string temp_term2 = "!(" + temp_term + ")";
			string _mutant1 = oriExp;
			_mutant1.replace(_mutant1.begin() + pos, _mutant1.begin() + pos + terms[i].size(), temp_term2);
			faultterms = { temp_term2 };
			mutants.addMutant(_mutant1, "DLOTNF", oriterms, faultterms);
			faultterms.clear();
		}
	}
	return mutants;
}

//单项双缺陷：（词+项）LNFxTNF
Mutants DnfMutant::generateSingleTermLNFxTNFdoublemutants(string exp)
{
	Mutants mutants(exp);
	bool vars[26];
	for (int i = 0; i < 26; i++)
	{
		vars[i] = false;
	}
	string oriExp(exp);
	Utility uti;
	vector<string> terms;
	vector<vector<string>> terms_literals;
	vector<string> oriterms;
	vector<string> faultterms;
	uti.mutantsPreproccess(oriExp, terms, terms_literals, vars);

	for (int i = 0; i < terms.size(); i++)//【遍历term】
	{
		oriterms.clear();
		oriterms = { terms[i] };
		int pos = oriExp.find(terms[i]);//第i个term的开始位置
		string term = terms[i];

		for (int j = 0; j < terms_literals.at(i).size(); j++)//遍历literal
		{
			//LNF
			string temp_term1 = term;
			string s1 = terms_literals.at(i).at(j);//第j个literal
			string s2 = uti.getLiteralNegation(s1);//获取单词的Negation
			int pos1 = temp_term1.find(s1);
			temp_term1.replace(pos1, s1.size(), s2);//temp_term1为 LNF 后的项

			//TNF
			string temp_term2 = "!(" + temp_term1 + ")";
			string _mutant1 = oriExp;
			_mutant1.replace(_mutant1.begin() + pos, _mutant1.begin() + pos + terms[i].size(), temp_term2);
			faultterms = { temp_term2 };
			mutants.addMutant(_mutant1, "DLNTNF", oriterms, faultterms);
			faultterms.clear();
		}
	}
	return mutants;
}

//CORFxLIF
Mutants DnfMutant::generateSingleTermCORFxLIFmutants(string exp){
	Mutants mutants(exp);
	bool vars[26];
	for (int i = 0; i < 26; i++) {
		vars[i] = false;
	}

	string oriExp(exp);
	Utility uti;
	vector<string> terms;
	vector<vector<string>> terms_literals;
	vector<string> oriterms;
	vector<string> faultterms;
	uti.mutantsPreproccess(oriExp, terms, terms_literals, vars);

	//遍历term
	for (int i = 0; i < terms.size(); i++)
	{
		oriterms.clear();
		oriterms = { terms[i] };
		int pos = oriExp.find(terms[i]);//项在表达式中开始的位置
		string new_term11 = terms[i];
		string new_term12 = terms[i];

		for (int j = 0; j < 26; j++)//【遍历不在当前项中的词literals】
		{
			char toInsert1 = 'a' + j;
			if (vars[j] && terms[i].find(toInsert1) == string::npos)//不在当前项中的词literal
			{
				string s11, s12;
				s11 += toInsert1;//不在当前项中的词literals
				s12 += '!';
				s12 += toInsert1;

				new_term11 = terms[i];//当前项
				new_term12 = terms[i];

				//LIF
				new_term11 += s11;
				new_term12 += s12;

				//CORF
				vector<string> literals_in_this_term1 = uti.extractCNFstyle(new_term11);
				string newTerm1 = literals_in_this_term1[0];
				for (int j = 1; j < literals_in_this_term1.size(); j++)
				{
					newTerm1 = newTerm1 + "+" + literals_in_this_term1[j];
				}
				string _mutant1 = oriExp;
				_mutant1.replace(_mutant1.begin() + pos, _mutant1.begin() + pos + terms[i].size(), newTerm1);
				mutants.addMutant(_mutant1, "DCORLIF", oriterms, faultterms);

				vector<string> literals_in_this_term2 = uti.extractCNFstyle(new_term12);
				string newTerm2 = literals_in_this_term2[0];
				for (int j = 1; j < literals_in_this_term2.size(); j++)
				{
					newTerm2 = newTerm2 + "+" + literals_in_this_term2[j];
				}
				string _mutant2 = oriExp;
				_mutant2.replace(_mutant2.begin() + pos, _mutant2.begin() + pos + terms[i].size(), newTerm2);
				faultterms = { newTerm2 };
				mutants.addMutant(_mutant2, "DCORLIF", oriterms, faultterms);
				faultterms.clear();
			}
		}
	}
	return mutants;
}

//CORFxLOF
Mutants DnfMutant::generateSingleTermCORFxLOFmutants(string exp){
	Mutants mutants(exp);
	bool vars[26];
	for (int i = 0; i < 26; i++)
	{
		vars[i] = false;
	}
	string oriExp(exp);
	Utility uti;
	vector<string> terms;
	vector<vector<string>> terms_literals;
	vector<string> oriterms;
	vector<string> faultterms;
	uti.mutantsPreproccess(oriExp, terms, terms_literals, vars);

	for (int i = 0; i < terms.size(); i++)//【遍历term】
	{
		oriterms.clear();
		oriterms = { terms[i] };
		int pos = oriExp.find(terms[i]);//第i个term的开始位置
		string term = terms[i];

		for (int j = 0; j < terms_literals.at(i).size(); j++)//遍历literal
		{
			//第一步：LOF
			string temp_term = term;
			string s1 = terms_literals.at(i).at(j);
			int pos1 = temp_term.find(s1);
			temp_term.replace(pos1, s1.size(), "");

			//CORF
			if (temp_term == "")break;
			vector<string> literals_in_this_term1 = uti.extractCNFstyle(temp_term);
			if (literals_in_this_term1.size() == 0)break;
			string newTerm1 = literals_in_this_term1[0];
			for (int j = 1; j < literals_in_this_term1.size(); j++)
			{
				newTerm1 = newTerm1 + "+" + literals_in_this_term1[j];
			}
			string _mutant1 = oriExp;
			_mutant1.replace(_mutant1.begin() + pos, _mutant1.begin() + pos + terms[i].size(), newTerm1);
			faultterms = { newTerm1 };
			faultterms.clear();
		}
	}
	return mutants;
}

//CORFxLRF
Mutants DnfMutant::generateSingleTermCORFxLRFmutants(string exp){
	Mutants mutants(exp);
	bool vars[26];
	for (size_t i = 0; i < 26; i++)
	{
		vars[i] = false;
	}

	string oriExp(exp);
	Utility uti;
	vector<string> terms;
	vector<vector<string>> terms_literals;
	vector<string> oriterms;
	vector<string> faultterms;
	uti.mutantsPreproccess(oriExp, terms, terms_literals, vars);

	//遍历term
	for (size_t i = 0; i < terms.size(); i++)
	{
		oriterms.clear();
		oriterms = { terms[i] };
		vector<string> literals_in_this_term = uti.extractCNFstyle(terms[i]);//获取当前项中的词
		int pos = oriExp.find(terms[i]);//项在表达式中开始的位置
		string new_term11 = terms[i];
		string new_term12 = terms[i];

		for (size_t j = 0; j < 26; j++)//【遍历不在当前项中的词literals】
		{
			char toReplaceWith = 'a' + j;
			string s11, s12;
			s11 += toReplaceWith;//不在当前项中的词literals
			s12 += '!';
			s12 += toReplaceWith;
			if (vars[j] && terms[i].find(toReplaceWith) == string::npos)//不在当前项中的词literals
			{
				for (size_t k = 0; k < literals_in_this_term.size(); k++)//【遍历当前term中的literal】
				{
					new_term11 = terms[i];//当前项
					new_term12 = terms[i];

					//LRF
					int literal_pos = new_term11.find(literals_in_this_term[k]);
					new_term11.replace(new_term11.begin() + literal_pos, new_term11.begin() + literal_pos + literals_in_this_term[k].size(), s11);
					new_term12.replace(new_term12.begin() + literal_pos, new_term12.begin() + literal_pos + literals_in_this_term[k].size(), s12);

					//CORF
					vector<string> literals_in_this_term1 = uti.extractCNFstyle(new_term11);
					string newTerm1 = literals_in_this_term1[0];
					for (int j = 1; j < literals_in_this_term1.size(); j++)
					{
						newTerm1 = newTerm1 + "+" + literals_in_this_term1[j];
					}
					string _mutant1 = oriExp;
					_mutant1.replace(_mutant1.begin() + pos, _mutant1.begin() + pos + terms[i].size(), newTerm1);
					mutants.addMutant(_mutant1, "DCORLRF", oriterms, faultterms);
					faultterms.clear();

					vector<string> literals_in_this_term2 = uti.extractCNFstyle(new_term12);
					string newTerm2 = literals_in_this_term2[0];
					for (int j = 1; j < literals_in_this_term2.size(); j++)
					{
						newTerm2 = newTerm2 + "+" + literals_in_this_term2[j];
					}
					string _mutant2 = oriExp;
					_mutant2.replace(_mutant2.begin() + pos, _mutant2.begin() + pos + terms[i].size(), newTerm2);
					faultterms = { newTerm2 };
					mutants.addMutant(_mutant2, "DCORLRF", oriterms, faultterms);
					faultterms.clear();
				}
			}
		}
	}
	return mutants;
}

//CORFxLNF
Mutants DnfMutant::generateSingleTermCORFxLNFmutants(string exp)
{
	Mutants mutants(exp);
	bool vars[26];
	for (int i = 0; i < 26; i++)
	{
		vars[i] = false;
	}
	string oriExp(exp);
	Utility uti;
	vector<string> terms;
	vector<vector<string>> terms_literals;
	vector<string> oriterms;
	vector<string> faultterms;
	uti.mutantsPreproccess(oriExp, terms, terms_literals, vars);

	for (int i = 0; i < terms.size(); i++)//【遍历term】
	{
		oriterms.clear();
		oriterms = { terms[i] };
		int pos = oriExp.find(terms[i]);//第i个term的开始位置
		string term = terms[i];

		for (int j = 0; j < terms_literals.at(i).size(); j++)//遍历literal
		{
			//LNF
			string temp_term1 = term;
			string s1 = terms_literals.at(i).at(j);//第j个literal
			string s2 = uti.getLiteralNegation(s1);//获取单词的Negation
			int pos1 = temp_term1.find(s1);
			temp_term1.replace(pos1, s1.size(), s2);//temp_term1为 LNF 后的项

			//CORF
			vector<string> literals_in_this_term1 = uti.extractCNFstyle(temp_term1);
			string newTerm1 = literals_in_this_term1[0];
			for (int j = 1; j < literals_in_this_term1.size(); j++)
			{
				newTerm1 = newTerm1 + "+" + literals_in_this_term1[j];
			}
			string _mutant1 = oriExp;
			_mutant1.replace(_mutant1.begin() + pos, _mutant1.begin() + pos + terms[i].size(), newTerm1);
			faultterms = { newTerm1 };
			mutants.addMutant(_mutant1, "DCORLNF", oriterms, faultterms);
			faultterms.clear();

		}
	}
	return mutants;
}

//DORFxLIF ————先发生DORF，再发生LIF
Mutants DnfMutant::generateSingleTermDORFxLIFmutants(string exp)
{
	Mutants mutants(exp);
	string oriExp(exp);
	Utility uti;
	vector<string> terms = uti.extractDNFstyle(oriExp);

	vector<string> oriterms;
	vector<string> faultterms;
	for (int i = 1; i < terms.size(); i++)
	{
		oriterms.clear();
		oriterms = { terms[i] };
		//DORF
		int pos_current_term = oriExp.find(terms[i]);
		int pos_erase_operator_or = pos_current_term - 1;// erase "+ " at this position
		string new_exp = oriExp;
		new_exp.erase(new_exp.begin() + pos_erase_operator_or);

		bool vars[26];
		for (int j = 0; j < 26; j++) {
			vars[j] = false;
		}
		vector<string> terms1;
		vector<vector<string>> terms_literals;
		uti.mutantsPreproccess(new_exp, terms1, terms_literals, vars);

		string currentTerm = terms1[i - 1];

		int pos = new_exp.find(currentTerm);//项在表达式中开始的位置
		string new_term11 = currentTerm;
		string new_term12 = currentTerm;

		for (int k = 0; k < 26; k++)//遍历不在当前项中的词literals
		{
			char toInsert1 = 'a' + k;
			if (vars[k] && currentTerm.find(toInsert1) == string::npos)//不在当前项中的词literal
			{
				string s11, s12;
				s11 += toInsert1;//不在当前项中的词literals
				s12 += '!';
				s12 += toInsert1;

				new_term11 = currentTerm;//当前项
				new_term12 = currentTerm;
				//LIF
				new_term11 += s11;
				new_term12 += s12;

				string m1;
				string _mutant1 = new_exp;
				//变体1
				_mutant1.replace(_mutant1.begin() + pos, _mutant1.begin() + pos + currentTerm.size(), new_term11);
				m1.append(_mutant1);
				faultterms = { new_term11 };
				mutants.addMutant(m1, "DDORLIF", oriterms, faultterms);
				faultterms.clear();

				string m2;
				string _mutant2 = new_exp;
				//变体2
				_mutant2.replace(_mutant2.begin() + pos, _mutant2.begin() + pos + currentTerm.size(), new_term12);
				m2.append(_mutant2);
				faultterms = { new_term12 };
				mutants.addMutant(m2, "DDORLIF", oriterms, faultterms);
				faultterms.clear();

			}
		}
	}
	return mutants;
}

//DORFxLOF ————先发生DORF，再发生LOF
Mutants DnfMutant::generateSingleTermDORFxLOFmutants(string exp)
{
	Mutants mutants(exp);
	string oriExp(exp);
	Utility uti;
	vector<string> terms = uti.extractDNFstyle(oriExp);

	vector<string> oriterms;
	vector<string> faultterms;
	for (int i = 1; i < terms.size(); i++)
	{
		oriterms.clear();
		oriterms = { terms[i] };
		//DORF
		int pos_current_term = oriExp.find(terms[i]);
		int pos_erase_operator_or = pos_current_term - 1;// erase "+ " at this position
		string new_exp = oriExp;
		new_exp.erase(new_exp.begin() + pos_erase_operator_or);

		bool vars[26];
		for (int j = 0; j < 26; j++) {
			vars[j] = false;
		}
		vector<string> terms1;
		vector<vector<string>> terms_literals;
		uti.mutantsPreproccess(new_exp, terms1, terms_literals, vars);

		int pos = new_exp.find(terms1[i - 1]);//第i个term的开始位置

		for (int j = 0; j < terms_literals.at(i - 1).size(); j++)//遍历literal
		{
			//第一步：LOF
			string temp_term = terms1[i - 1];
			string s1 = terms_literals.at(i - 1).at(j);
			int pos1 = temp_term.find(s1);
			temp_term.replace(pos1, s1.size(), "");

			vector<string> terms_literal2 = terms_literals.at(i - 1);
			terms_literal2.erase(terms_literal2.begin() + j);//删除第j个元素
			string newTerm;
			for (int k = 0; k < terms_literal2.size(); k++){
				newTerm += terms_literal2[k];
			}

			string _mutant1 = new_exp;
			_mutant1.replace(_mutant1.begin() + pos, _mutant1.begin() + pos + terms1[i - 1].size(), newTerm);
			faultterms = { newTerm };
			mutants.addMutant(_mutant1, "DDORLOF", oriterms, faultterms);
			faultterms.clear();
		}
	}
	return mutants;
}

//DORFxLNF ————先发生DORF，再发生LNF
Mutants DnfMutant::generateSingleTermDORFxLNFmutants(string exp)
{
	Mutants mutants(exp);
	string oriExp(exp);
	Utility uti;
	vector<string> terms = uti.extractDNFstyle(oriExp);
	vector<string> oriterms;
	vector<string> faultterms;
	for (int i = 1; i < terms.size(); i++)
	{
		oriterms.clear();
		oriterms = { terms[i] };
		//DORF
		int pos_current_term = oriExp.find(terms[i]);
		int pos_erase_operator_or = pos_current_term - 1;// erase "+ " at this position
		string new_exp = oriExp;
		new_exp.erase(new_exp.begin() + pos_erase_operator_or);

		bool vars[26];
		for (int j = 0; j < 26; j++) {
			vars[j] = false;
		}
		vector<string> terms1;
		vector<vector<string>> terms_literals;
		uti.mutantsPreproccess(new_exp, terms1, terms_literals, vars);

		int pos = new_exp.find(terms1[i - 1]);//第i个term的开始位置

		for (int j = 0; j < terms_literals.at(i - 1).size(); j++)//遍历literal
		{
			//第一步：LOF
			string temp_term = terms1[i - 1];
			string s1 = terms_literals.at(i - 1).at(j);
			int pos1 = temp_term.find(s1);
			temp_term.replace(pos1, s1.size(), "");

			vector<string> terms_literal2 = terms_literals.at(i - 1);
			terms_literal2.erase(terms_literal2.begin() + j);//删除第j个元素
			string newTerm;
			for (int k = 0; k < terms_literal2.size(); k++){
				newTerm += terms_literal2[k];
			}

			string _mutant1 = new_exp;
			_mutant1.replace(_mutant1.begin() + pos, _mutant1.begin() + pos + terms1[i - 1].size(), newTerm);
			faultterms = { newTerm };
			mutants.addMutant(_mutant1, "DDORLNF", oriterms, faultterms);
			faultterms.clear();
		}
	}
	return mutants;
}




//======================== 单缺陷 ============================

Mutants DnfMutant::generateLIFmutants(string exp){


	Mutants mutants(exp);
	bool vars[26];
	for (size_t i = 0; i < 26; i++)
	{
		vars[i] = false;
	}

	string oriExp = exp;
	Utility uti;
	vector<string> terms = uti.extractDNFstyle(oriExp);
	vector<vector<string>> terms_literals;
	vector<string> oriterms;
	vector<string> faultterms;
	for (size_t i = 0; i < terms.size(); i++)
	{
		terms_literals.push_back(uti.extractCNFstyle(terms[i]));
	}
	for (size_t i = 0; i < terms_literals.size(); i++)
	{
		for (size_t j = 0; j < terms_literals[i].size(); j++)
		{
			string temp = terms_literals[i][j];
			if (temp.size() == 1)
			{
				vars[temp[0] - 'a'] = true;
			}
			else
			{
				vars[temp[1] - 'a'] = true;
			}
		}
	}
	for (int i = 0; i < terms.size(); i++)
	{
		oriterms.clear();
		oriterms = { terms[i] };
		int pos = oriExp.find(terms[i]);
		for (int j = 0; j < 26; j++)
		{
			char toReplaceWith = 'a' + j;
			string s1, s2;
			s1 += toReplaceWith;
			s2 += '!';
			s2 += toReplaceWith;
			if (vars[j] && terms[i].find(toReplaceWith) == string::npos)
			{
				//insert x
				string new_term = terms[i];
				new_term += s1;
				string _mutant = oriExp;
				_mutant.replace(_mutant.begin() + pos, _mutant.begin() + pos + terms[i].size(), new_term);
				faultterms = { new_term };
				mutants.addMutant(_mutant, "LIF", oriterms, faultterms);
				faultterms.clear();
				//insert !x 
				string new_term2 = terms[i];
				new_term2 += s2;
				string _mutant2 = oriExp;
				_mutant2.replace(_mutant2.begin() + pos, _mutant2.begin() + pos + terms[i].size(), new_term2);
				faultterms = { new_term2 };
				mutants.addMutant(_mutant2, "LIF", oriterms, faultterms);
				faultterms.clear();
			}
		}
	}
	return mutants;

}

Mutants DnfMutant::generateLOFmutants(string exp)
{
	Mutants mutants(exp);

	string oriExp = exp;
	Utility uti;
	vector<string> terms = uti.extractDNFstyle(oriExp);
	vector<string> oriterms;
	vector<string> faultterms;

	for (int i = 0; i < terms.size(); i++)
	{
		oriterms.clear();
		oriterms = { terms[i] };
		string term = terms[i];
		string pre;
		string back;
		if (i != 0)
			pre = oriExp.substr(0, oriExp.find(term));
		if (i != terms.size() - 1)
			back = oriExp.substr(oriExp.find(term) + term.length());

		vector<string> characters = uti.extractCNFstyle(term);
		if (characters.size()<2){
			if (pre.size()>0)
				pre.pop_back();
			else if (back.size() > 0)
				back = back.substr(1);
			else
				return mutants;

			string mut;
			mut.append(pre);
			mut.append(back);
			string m(mut.c_str());
			mutants.addMutant(m, "LOF", oriterms, faultterms);
		}
		else if (characters.size() == 2){
			for (int j = 0; j < 2; j++)
			{
				string mut;
				mut.append(pre);
				mut.append(characters[j]);
				mut.append(back);
				string m(mut.c_str());
				faultterms = { characters[j] };
				mutants.addMutant(m, "LOF", oriterms, faultterms);
				faultterms.clear();
			}
		}
		else
		{
			for (int j = 0; j < characters.size(); j++)
			{
				string mutTerm;
				for (int k = 0; k < characters.size(); k++)
				{
					if (k == j)
						continue;
					mutTerm.append(characters[k]);
				}

				string mut;
				mut.append(pre);
				mut.append(mutTerm);
				mut.append(back);
				string m(mut.c_str());
				faultterms = { mutTerm };
				mutants.addMutant(m, "LOF", oriterms, faultterms);
				faultterms.clear();
			}//for
		}//else
	}

	return mutants;
}

Mutants DnfMutant::generateLNFmutants(string exp){
	Mutants mutants(exp);

	string oriExp = exp;
	Utility uti;
	vector<string> terms = uti.extractDNFstyle(oriExp);
	vector<string> oriterms;
	vector<string> faultterms;

	for (int i = 0; i < terms.size(); i++)
	{
		oriterms.clear();
		oriterms = { terms[i] };
		string term = terms[i];
		string pre;
		string back;
		if (i != 0)
			pre = oriExp.substr(0, oriExp.find(term));
		if (i != terms.size() - 1)
			back = oriExp.substr(oriExp.find(term) + term.length());

		for (size_t i = 0; i < term.size(); i++)
		{
			char current_ch = term[i];
			if (current_ch >= 'a'&&current_ch <= 'z')
			{
				string _term = term;
				string mut;
				if (i >= 1 && term[i - 1] == '!')
				{
					_term.erase(_term.begin() + (i - 1));
					string t = _term;
					mut.append(pre);
					mut.append(t);
					mut.append(back);
					faultterms = { t };
				}
				else
				{
					_term.insert(i, "!");
					string t(_term.c_str());
					mut.append(pre);
					mut.append(t);
					mut.append(back);
					faultterms = { t };
				}
				string m(mut.c_str());
				mutants.addMutant(m, "LNF", oriterms, faultterms);
				faultterms.clear();
			}
		}
	}
	return mutants;

	return mutants;

}

Mutants DnfMutant::generateTOFmutants(string exp){

	// new term (null) <- old term 

	Mutants mutants(exp);

	string oriExp = exp;
	Utility uti;
	vector<string> terms = uti.extractDNFstyle(oriExp);
	vector<string> oriterms;
	vector<string> faultterms;
	if (terms.size() <= 1)
		return mutants;
	for (size_t i = 0; i < terms.size(); i++)
	{
		oriterms.clear();
		oriterms = { terms[i] };
		string new_exp = oriExp;
		size_t pos_current_term = oriExp.find(terms[i]);
		if (i != terms.size() - 1)
		{
			new_exp.erase(new_exp.begin() + pos_current_term, new_exp.begin() + pos_current_term + terms[i].size() + 1);
		}
		else
		{
			if (terms.size() >1)
			{
				new_exp.erase(new_exp.begin() + pos_current_term - 1, new_exp.begin() + pos_current_term + terms[i].size() + 1);
			}
		}

		string t(new_exp.c_str());
		mutants.addMutant(t, "TOF", oriterms, faultterms);
	}
	return mutants;

}

Mutants DnfMutant::generateTNFmutants(string exp){

	// new term (null) <- old term 

	Mutants mutants(exp);

	string oriExp = exp;
	Utility uti;
	vector<string> terms = uti.extractDNFstyle(oriExp);
	vector<string> oriterms;
	vector<string> faultterms;
	if (terms.size() <= 1)
		return mutants;
	for (size_t i = 0; i < terms.size(); i++)
	{
		oriterms.clear();
		oriterms = { terms[i] };
		string new_exp = oriExp;
		size_t pos_current_term = oriExp.find(terms[i]);
		string new_term = "!(" + terms[i] + ")";
		new_exp.replace(new_exp.begin() + pos_current_term, new_exp.begin() + pos_current_term + terms[i].size(), new_term);
		string t(new_exp.c_str());
		faultterms = { new_term };
		mutants.addMutant(t, "TNF", oriterms, faultterms);
		faultterms.clear();
	}
	return mutants;

}

Mutants DnfMutant::generateCORFmutants(string exp){
	// a DNF term departed into 2 terms 
	Mutants mutants(exp);

	string oriExp = exp;
	Utility uti;
	vector<string> terms = uti.extractDNFstyle(oriExp);
	vector<string> oriterms;
	vector<string> faultterms;

	for (size_t i = 0; i < terms.size(); i++)
	{
		oriterms.clear();
		oriterms = { terms[i] };
		// for each term ,departed at every literal
		size_t pos_current_term = oriExp.find(terms[i]);
		vector<string> _literals_in_this_term = uti.extractCNFstyle(terms[i]);
		for (size_t j = 1; j < _literals_in_this_term.size(); j++)
		{
			size_t pos_insert_operator_or = pos_current_term + terms[i].find(_literals_in_this_term[j]);
			string new_exp = oriExp;

			new_exp.insert(pos_insert_operator_or, "+");
			string t(new_exp.c_str());
			vector<string> tmpterms = uti.extractDNFstyle(t);
			faultterms = { tmpterms[i], tmpterms[i+1] };
			mutants.addMutant(t, "CORF", oriterms, faultterms);
			tmpterms.clear();
			faultterms.clear();
		}

	}
	return mutants;

}

Mutants DnfMutant::generateLRFmutants(string exp){

	Mutants mutants(exp);
	bool vars[26];
	for (size_t i = 0; i < 26; i++)
	{
		vars[i] = false;
	}
	string oriExp = exp;
	Utility uti;
	vector<string> terms = uti.extractDNFstyle(oriExp);
	vector<string> oriterms;
	vector<string> faultterms;

	vector<vector<string>> terms_literals;
	for (size_t i = 0; i < terms.size(); i++)
	{
		terms_literals.push_back(uti.extractCNFstyle(terms[i]));
	}

	for (size_t i = 0; i < terms_literals.size(); i++)
	{
		for (size_t j = 0; j < terms_literals[i].size(); j++)
		{
			string temp = terms_literals[i][j];
			if (temp.size() == 1)
			{
				vars[temp[0] - 'a'] = true;
			}
			else
			{
				vars[temp[1] - 'a'] = true;
			}
		}
	}
	for (size_t i = 0; i < terms.size(); i++)
	{
		oriterms.clear();
		oriterms = { terms[i] };
		vector<string> literals_in_this_term = uti.extractCNFstyle(terms[i]);
		int pos = oriExp.find(terms[i]);
		for (size_t j = 0; j < 26; j++)
		{
			char toReplaceWith = 'a' + j;
			string s1, s2;
			s1 += toReplaceWith;
			s2 += '!';
			s2 += toReplaceWith;

			if (vars[j] && terms[i].find(toReplaceWith) == string::npos)
			{
				for (size_t k = 0; k < literals_in_this_term.size(); k++)
				{
					string new_term = terms[i];

					int literal_pos = new_term.find(literals_in_this_term[k]);
					new_term.replace(new_term.begin() + literal_pos, new_term.begin() + literal_pos + literals_in_this_term[k].size(), s1);
					string _mutant = oriExp;
					_mutant.replace(_mutant.begin() + pos, _mutant.begin() + pos + terms[i].size(), new_term);

					string m(_mutant.c_str());
					faultterms = { new_term };
					mutants.addMutant(m, "LRF", oriterms, faultterms);
					faultterms.clear();
				}
				for (size_t k = 0; k < literals_in_this_term.size(); k++)
				{
					string new_term2 = terms[i];
					int literal_pos = new_term2.find(literals_in_this_term[k]);
					new_term2.replace(new_term2.begin() + literal_pos, new_term2.begin() + literal_pos + literals_in_this_term[k].size(), s2);
					string _mutant = oriExp;
					_mutant.replace(_mutant.begin() + pos, _mutant.begin() + pos + terms[i].size(), new_term2);

					string m(_mutant.c_str());
					faultterms = { new_term2 };
					mutants.addMutant(m, "LRF", oriterms, faultterms);
					faultterms.clear();

				}

			}
		}

	}
	return mutants;
}

Mutants DnfMutant::generateDORFmutants(string exp){
	// 2 DNF term combined into one term
	Mutants mutants(exp);

	string oriExp(exp);
	Utility uti;
	vector<string> terms = uti.extractDNFstyle(oriExp);
	vector<string> oriterms;
	vector<string> faultterms;

	for (size_t i = 1; i < terms.size(); i++)
	{
		oriterms.clear();
		oriterms = { terms[i] };
		size_t pos_current_term = oriExp.find(terms[i]);

		// erase "+ " at this position
		size_t pos_erase_operator_or = pos_current_term - 1;

		string new_exp = oriExp;
		if (pos_erase_operator_or<new_exp.size()) new_exp.erase(new_exp.begin() + pos_erase_operator_or);
		string t(new_exp.c_str());
		if (i + 1<terms.size())faultterms = { terms[i] + terms[i + 1] };
		mutants.addMutant(t, "DORF", oriterms, faultterms);
		faultterms.clear();
	}
	return mutants;
}

Mutants DnfMutant::generateENFmutants(string exp){
	Mutants mutants(exp);
	vector<string> oriterms;
	vector<string> faultterms;
	string new_exp = "!(" + exp + ")";
	mutants.addMutant(new_exp, "ENF", oriterms, faultterms);
	return mutants;
}

Mutants DnfMutant::generateLIORFmutants(string exp)
{
	Mutants mutants = generateLIFmutants(exp);
	mutants += generateLOFmutants(exp);
	mutants += generateLRFmutants(exp);

	return mutants;
}

//===
Mutants DnfMutant::generateENFmutants(string exp, vector<string> terms){
	Mutants mutants(exp);
	string type = "ENF";
	string oriExp(exp);
	Utility uti;
	vector<string> oriTerms = uti.extractDNFstyle(exp);
	string new_exp = terms[0];
	if (terms.size() == 0)type = "TNF";
	for (int i = 1; i < terms.size(); i++){
		new_exp = new_exp + "+" + terms[i];
	}
	new_exp = "!(" + new_exp + ")";
	for (int i = 0; i < oriTerms.size(); i++)
	{
		int isEnf = false;
		for (string term : terms){
			if (oriTerms[i] == term){
				isEnf = true;
				break;
			}
		}
		if (isEnf){
			continue;
		}
		else{
			new_exp = new_exp + "+" + oriTerms[i];
		}
	}
	mutants.addMutant(new_exp, type);
	return mutants;
}

Mutants DnfMutant::generateLIFmutants(string exp, vector<string> terms, string model)
{
	Mutants mutants(exp);
	bool vars[26];
	for (int i = 0; i < 26; i++)
	{
		vars[i] = false;
	}

	string oriExp = exp;
	Utility uti;
	vector<string> oriTerms = uti.extractDNFstyle(oriExp);

	vector<vector<string>> terms_literals;
	for (int j = 0; j < model.size(); j++)
	{
		string temp = { model[j] };
		vars[temp[0] - 'a'] = true;
	}

	for (int i = 0; i < terms.size(); i++)
	{
		int pos = oriExp.find(terms[i]);
		for (int j = 0; j < 26; j++)
		{
			char toReplaceWith = 'a' + j;
			string s1, s2;
			s1 += toReplaceWith;
			s2 += '!';
			s2 += toReplaceWith;
			if (vars[j] && terms[i].find(toReplaceWith) == string::npos)
			{
				//insert x
				string new_term = terms[i];
				new_term += s1;
				string _mutant = oriExp;
				_mutant.replace(_mutant.begin() + pos, _mutant.begin() + pos + terms[i].size(), new_term);
				mutants.addMutant(_mutant, "LIF");
				//insert !x 
				string new_term2 = terms[i];
				new_term2 += s2;
				string _mutant2 = oriExp;
				_mutant2.replace(_mutant2.begin() + pos, _mutant2.begin() + pos + terms[i].size(), new_term2);
				mutants.addMutant(_mutant2, "LIF");
			}
		}
	}
	return mutants;

}

Mutants DnfMutant::generateLRFmutants(string exp, vector<string> terms, string model){

	Mutants mutants(exp);
	bool vars[26];
	for (int i = 0; i < 26; i++)
	{
		vars[i] = false;
	}
	string oriExp = exp;
	Utility uti;
	vector<string> oriTerms = uti.extractDNFstyle(exp);

	for (int j = 0; j < model.size(); j++)
	{
		string temp = { model[j] };
		if (temp.size() == 1)
		{
			vars[temp[0] - 'a'] = true;
		}
		else
		{
			vars[temp[1] - 'a'] = true;
		}
	}
	for (int i = 0; i < terms.size(); i++)
	{
		vector<string> literals_in_this_term = uti.extractCNFstyle(terms[i]);
		int pos = oriExp.find(terms[i]);
		for (int j = 0; j < 26; j++)
		{
			char toReplaceWith = 'a' + j;
			string s1, s2;
			s1 += toReplaceWith;
			s2 += '!';
			s2 += toReplaceWith;

			if (vars[j] && terms[i].find(toReplaceWith) == string::npos)
			{
				for (int k = 0; k < literals_in_this_term.size(); k++)
				{
					string new_term = terms[i];

					int literal_pos = new_term.find(literals_in_this_term[k]);
					new_term.replace(new_term.begin() + literal_pos, new_term.begin() + literal_pos + literals_in_this_term[k].size(), s1);
					string _mutant = oriExp;
					_mutant.replace(_mutant.begin() + pos, _mutant.begin() + pos + terms[i].size(), new_term);

					string m(_mutant.c_str());
					mutants.addMutant(m, "LRF");
				}
				for (int k = 0; k < literals_in_this_term.size(); k++)
				{
					string new_term2 = terms[i];
					int literal_pos = new_term2.find(literals_in_this_term[k]);
					new_term2.replace(new_term2.begin() + literal_pos, new_term2.begin() + literal_pos + literals_in_this_term[k].size(), s2);
					string _mutant = oriExp;
					_mutant.replace(_mutant.begin() + pos, _mutant.begin() + pos + terms[i].size(), new_term2);

					string m(_mutant.c_str());
					mutants.addMutant(m, "LRF");

				}

			}
		}

	}
	return mutants;
}

Mutants DnfMutant::generateLIFmutants(string exp, vector<string> terms){


	Mutants mutants(exp);
	bool vars[26];
	for (size_t i = 0; i < 26; i++)
	{
		vars[i] = false;
	}

	string oriExp = exp;
	Utility uti;
	vector<string> oriTerms = uti.extractDNFstyle(oriExp);
	vector<string> oriterms;
	vector<string> faultterms;

	vector<vector<string>> terms_literals;
	for (size_t i = 0; i < oriTerms.size(); i++)
	{
		terms_literals.push_back(uti.extractCNFstyle(oriTerms[i]));
	}
	for (size_t i = 0; i < terms_literals.size(); i++)
	{
		for (size_t j = 0; j < terms_literals[i].size(); j++)
		{
			string temp = terms_literals[i][j];
			if (temp.size() == 1)
			{
				vars[temp[0] - 'a'] = true;
			}
			else
			{
				vars[temp[1] - 'a'] = true;
			}
		}
	}
	for (int i = 0; i < terms.size(); i++)
	{
		oriterms.clear();
		oriterms = { terms[i] };
		int pos = oriExp.find(terms[i]);
		for (int j = 0; j < 26; j++)
		{
			char toReplaceWith = 'a' + j;
			string s1, s2;
			s1 += toReplaceWith;
			s2 += '!';
			s2 += toReplaceWith;
			if (vars[j] && terms[i].find(toReplaceWith) == string::npos)
			{
				//insert x
				string new_term = terms[i];
				new_term += s1;
				string _mutant = oriExp;
				_mutant.replace(_mutant.begin() + pos, _mutant.begin() + pos + terms[i].size(), new_term);
				faultterms = { new_term };
				mutants.addMutant(_mutant, "LIF", oriterms, faultterms);
				faultterms.clear();
				//insert !x 
				string new_term2 = terms[i];
				new_term2 += s2;
				string _mutant2 = oriExp;
				_mutant2.replace(_mutant2.begin() + pos, _mutant2.begin() + pos + terms[i].size(), new_term2);
				faultterms = { new_term2 };
				mutants.addMutant(_mutant2, "LIF", oriterms, faultterms);
				faultterms.clear();
			}
		}
	}
	return mutants;

}

Mutants DnfMutant::generateLOFmutants(string exp, vector<string> terms)
{
	Mutants mutants(exp);

	string oriExp = exp;
	Utility uti;
	vector<string> oriterms;
	vector<string> faultterms;
	for (int i = 0; i < terms.size(); i++)
	{
		oriterms.clear();
		oriterms = { terms[i] };
		string term = terms[i];
		string pre;
		string back;

		/*cout << "term==" <<term<< endl;
		cout << "oriexp: " << oriExp << endl;
		cout << "index: " << oriExp.find(term) << endl;*/
		pre = oriExp.substr(0, oriExp.find(term));
		back = oriExp.substr(oriExp.find(term) + term.length());

		vector<string> characters = uti.extractCNFstyle(term);
		if (characters.size()<2){
			if (pre.size()>0)
				pre.pop_back();
			else if (back.size() > 0)
				back = back.substr(1);
			else
				return mutants;

			string mut;
			mut.append(pre);
			mut.append(back);
			string m(mut.c_str());
			mutants.addMutant(m, "LOF", oriterms, faultterms);
		}
		else if (characters.size() == 2){
			for (int j = 0; j < 2; j++)
			{
				string mut;
				mut.append(pre);
				mut.append(characters[j]);
				mut.append(back);
				string m(mut.c_str());
				faultterms = { characters[j] };
				mutants.addMutant(m, "LOF", oriterms, faultterms);
			}
		}
		else
		{
			for (int j = 0; j < characters.size(); j++)
			{
				string mutTerm;
				for (int k = 0; k < characters.size(); k++)
				{
					if (k == j)
						continue;
					mutTerm.append(characters[k]);
				}

				string mut;
				mut.append(pre);
				mut.append(mutTerm);
				mut.append(back);
				string m(mut.c_str());
				faultterms = { mutTerm };
				mutants.addMutant(m, "LOF", oriterms, faultterms);
				faultterms.clear();
			}//for
		}//else
	}

	return mutants;
}

Mutants DnfMutant::generateLNFmutants(string exp, vector<string> terms){
	Mutants mutants(exp);

	string oriExp = exp;
	Utility uti;
	vector<string> oriterms;
	vector<string> faultterms;

	for (int i = 0; i < terms.size(); i++)
	{
		oriterms.clear();
		oriterms = { terms[i] };
		string term = terms[i];
		string pre;
		string back;

		pre = oriExp.substr(0, oriExp.find(term));
		back = oriExp.substr(oriExp.find(term) + term.length());

		for (size_t i = 0; i < term.size(); i++)
		{
			char current_ch = term[i];
			if (current_ch >= 'a'&&current_ch <= 'z')
			{
				string _term = term;
				string mut;
				if (i >= 1 && term[i - 1] == '!')
				{
					_term.erase(_term.begin() + (i - 1));
					string t = _term;
					mut.append(pre);
					mut.append(t);
					mut.append(back);
					faultterms = { t };
				}
				else
				{
					_term.insert(i, "!");
					string t(_term.c_str());
					mut.append(pre);
					mut.append(t);
					mut.append(back);
					faultterms = { t };
				}
				string m(mut.c_str());
				mutants.addMutant(m, "LNF", oriterms, faultterms);
				faultterms.clear();
			}
		}
	}

	return mutants;

}

Mutants DnfMutant::generateTOFmutants(string exp, vector<string> terms){

	// new term (null) <- old term 

	Mutants mutants(exp);

	Utility uti;
	vector<string> oriTerms = uti.extractDNFstyle(exp);
	vector<string> oriterms;
	vector<string> faultterms;

	string oriExp = exp;
	if (oriTerms.size() <= 1)
		return mutants;
	for (size_t i = 0; i < terms.size(); i++)
	{
		oriterms.clear();
		oriterms = { terms[i] };
		string new_exp = oriExp;
		size_t pos_current_term = oriExp.find(terms[i]);
		if (oriExp.size() - pos_current_term != terms[i].size())
		{
			new_exp.erase(new_exp.begin() + pos_current_term, new_exp.begin() + pos_current_term + terms[i].size() + 1);
		}
		else
		{
			new_exp.erase(new_exp.begin() + pos_current_term - 1, new_exp.begin() + pos_current_term + terms[i].size() + 1);
		}

		string t(new_exp.c_str());
		mutants.addMutant(t, "TOF", oriterms, faultterms);
		faultterms.clear();
	}
	return mutants;

}

Mutants DnfMutant::generateTNFmutants(string exp, vector<string> terms){

	// new term (null) <- old term 

	Mutants mutants(exp);

	string oriExp = exp;

	Utility uti;
	vector<string> oriTerms = uti.extractDNFstyle(exp);
	vector<string> oriterms;
	vector<string> faultterms;

	if (oriTerms.size() <= 1)
		return mutants;
	for (size_t i = 0; i < terms.size(); i++)
	{
		oriterms.clear();
		oriterms = { terms[i] };
		string new_exp = oriExp;
		size_t pos_current_term = oriExp.find(terms[i]);
		string new_term = "!(" + terms[i] + ")";
		new_exp.replace(new_exp.begin() + pos_current_term, new_exp.begin() + pos_current_term + terms[i].size(), new_term);
		string t(new_exp.c_str());
		faultterms = { new_term };
		mutants.addMutant(t, "TNF", oriterms, faultterms);
		faultterms.clear();
	}
	return mutants;

}

Mutants DnfMutant::generateCORFmutants(string exp, vector<string> terms){
	// a DNF term departed into 2 terms 
	Mutants mutants(exp);

	string oriExp = exp;
	Utility uti;
	vector<string> oriterms;
	vector<string> faultterms;
	for (size_t i = 0; i < terms.size(); i++)
	{
		oriterms.clear();
		oriterms = { terms[i] };
		// for each term ,departed at every literal
		size_t pos_current_term = oriExp.find(terms[i]);
		vector<string> _literals_in_this_term = uti.extractCNFstyle(terms[i]);
		for (size_t j = 1; j < _literals_in_this_term.size(); j++)
		{
			size_t pos_insert_operator_or = pos_current_term + terms[i].find(_literals_in_this_term[j]);
			string new_exp = oriExp;

			new_exp.insert(pos_insert_operator_or, "+");
			string t(new_exp.c_str());
			vector<string> tmpterms = uti.extractDNFstyle(t);
			faultterms = { tmpterms[i], tmpterms[i + 1] };
			mutants.addMutant(t, "CORF", oriterms, faultterms);
			tmpterms.clear();
			faultterms.clear();
		}

	}
	return mutants;

}

Mutants DnfMutant::generateLRFmutants(string exp, vector<string> terms){

	Mutants mutants(exp);
	bool vars[26];
	for (size_t i = 0; i < 26; i++)
	{
		vars[i] = false;
	}
	string oriExp = exp;
	Utility uti;
	vector<string> oriTerms = uti.extractDNFstyle(exp);
	vector<string> oriterms;
	vector<string> faultterms;

	vector<vector<string>> terms_literals;
	for (size_t i = 0; i < terms.size(); i++)
	{
		terms_literals.push_back(uti.extractCNFstyle(terms[i]));
	}

	for (size_t i = 0; i < terms_literals.size(); i++)
	{
		for (size_t j = 0; j < terms_literals[i].size(); j++)
		{
			string temp = terms_literals[i][j];
			if (temp.size() == 1)
			{
				vars[temp[0] - 'a'] = true;
			}
			else
			{
				vars[temp[1] - 'a'] = true;
			}
		}
	}
	for (size_t i = 0; i < terms.size(); i++)
	{
		oriterms.clear();
		oriterms = { terms[i] };
		vector<string> literals_in_this_term = uti.extractCNFstyle(terms[i]);
		int pos = oriExp.find(terms[i]);
		for (size_t j = 0; j < 26; j++)
		{
			char toReplaceWith = 'a' + j;
			string s1, s2;
			s1 += toReplaceWith;
			s2 += '!';
			s2 += toReplaceWith;

			if (vars[j] && terms[i].find(toReplaceWith) == string::npos)
			{
				for (size_t k = 0; k < literals_in_this_term.size(); k++)
				{
					string new_term = terms[i];

					int literal_pos = new_term.find(literals_in_this_term[k]);
					new_term.replace(new_term.begin() + literal_pos, new_term.begin() + literal_pos + literals_in_this_term[k].size(), s1);
					string _mutant = oriExp;
					_mutant.replace(_mutant.begin() + pos, _mutant.begin() + pos + terms[i].size(), new_term);

					string m(_mutant.c_str());
					faultterms = { new_term };
					mutants.addMutant(m, "LRF", oriterms, faultterms);
					faultterms.clear();
				}
				for (size_t k = 0; k < literals_in_this_term.size(); k++)
				{
					string new_term2 = terms[i];
					int literal_pos = new_term2.find(literals_in_this_term[k]);
					new_term2.replace(new_term2.begin() + literal_pos, new_term2.begin() + literal_pos + literals_in_this_term[k].size(), s2);
					string _mutant = oriExp;
					_mutant.replace(_mutant.begin() + pos, _mutant.begin() + pos + terms[i].size(), new_term2);

					string m(_mutant.c_str());
					faultterms = { new_term2 };
					mutants.addMutant(m, "LRF", oriterms, faultterms);
					faultterms.clear();

				}

			}
		}

	}
	return mutants;
}

Mutants DnfMutant::generateDORFmutants(string exp, vector<string> terms){
	// 2 DNF term combined into one term
	Mutants mutants(exp);

	string oriExp(exp);
	Utility uti;
	vector<string> oriTerms = uti.extractDNFstyle(exp);
	vector<string> oriterms;
	vector<string> faultterms;

	for (size_t i = 1; i < terms.size(); i++)
	{
		oriterms.clear();
		oriterms = { terms [i]};
		size_t pos_current_term = oriExp.find(terms[i]);

		// erase "+ " at this position
		size_t pos_erase_operator_or = pos_current_term - 1;

		string new_exp = oriExp;
		new_exp.erase(new_exp.begin() + pos_erase_operator_or);
		string t(new_exp.c_str()); 
		if (i + 1<terms.size())faultterms = { terms[i] + terms[i + 1] };
		mutants.addMutant(t, "DORF", oriterms, faultterms);
		faultterms.clear();

	}
	return mutants;
}

Mutants DnfMutant::generateLIFmutants(string type, string exp, vector<string> terms, string model)
{
	Mutants mutants(exp);
	bool vars[26];
	for (int i = 0; i < 26; i++)
	{
		vars[i] = false;
	}

	string oriExp = exp;
	Utility uti;
	vector<string> oriTerms = uti.extractDNFstyle(oriExp);

	vector<vector<string>> terms_literals;
	for (int j = 0; j < model.size(); j++)
	{
		string temp = { model[j] };
		vars[temp[0] - 'a'] = true;
	}

	for (int i = 0; i < terms.size(); i++)
	{
		int pos = oriExp.find(terms[i]);
		for (int j = 0; j < 26; j++)
		{
			char toReplaceWith = 'a' + j;
			string s1, s2;
			s1 += toReplaceWith;
			s2 += '!';
			s2 += toReplaceWith;
			if (vars[j] && terms[i].find(toReplaceWith) == string::npos)
			{
				//insert x
				string new_term = terms[i];
				new_term += s1;
				string _mutant = oriExp;
				_mutant.replace(_mutant.begin() + pos, _mutant.begin() + pos + terms[i].size(), new_term);
				mutants.addMutant(_mutant, type + "xLIF");
				//insert !x 
				string new_term2 = terms[i];
				new_term2 += s2;
				string _mutant2 = oriExp;
				_mutant2.replace(_mutant2.begin() + pos, _mutant2.begin() + pos + terms[i].size(), new_term2);
				mutants.addMutant(_mutant2, type + "xLIF");
			}
		}
	}
	return mutants;

}

Mutants DnfMutant::generateLOFmutants(string type, string exp, vector<string> terms)
{
	Mutants mutants(exp);

	string oriExp = exp;
	Utility uti;
	for (int i = 0; i < terms.size(); i++)
	{
		string term = terms[i];
		string pre;
		string back;

		pre = oriExp.substr(0, oriExp.find(term));
		back = oriExp.substr(oriExp.find(term) + term.length());

		vector<string> characters = uti.extractCNFstyle(term);
		if (characters.size()<2){
			if (pre.size()>0)
				pre.pop_back();
			else if (back.size() > 0)
				back = back.substr(1);
			else
				return mutants;

			string mut;
			mut.append(pre);
			mut.append(back);
			string m(mut.c_str());
			mutants.addMutant(m, type + "xLOF");
		}
		else if (characters.size() == 2){
			for (int j = 0; j < 2; j++)
			{
				string mut;
				mut.append(pre);
				mut.append(characters[j]);
				mut.append(back);
				string m(mut.c_str());
				mutants.addMutant(m, type + "xLOF");
			}
		}
		else
		{
			for (int j = 0; j < characters.size(); j++)
			{
				string mutTerm;
				for (int k = 0; k < characters.size(); k++)
				{
					if (k == j)
						continue;
					mutTerm.append(characters[k]);
				}

				string mut;
				mut.append(pre);
				mut.append(mutTerm);
				mut.append(back);
				string m(mut.c_str());
				mutants.addMutant(m, "LOF");
			}//for
		}//else
	}

	return mutants;
}


Mutants DnfMutant::generateLNFmutants(string type, string exp, vector<string> terms){
	Mutants mutants(exp);

	string oriExp = exp;
	Utility uti;

	for (int i = 0; i < terms.size(); i++)
	{
		string term = terms[i];
		string pre;
		string back;

		pre = oriExp.substr(0, oriExp.find(term));
		back = oriExp.substr(oriExp.find(term) + term.length());

		for (int i = 0; i < term.size(); i++)
		{
			char current_ch = term[i];
			if (current_ch >= 'a'&&current_ch <= 'z')
			{
				string _term = term;
				string mut;
				if (i >= 1 && term[i - 1] == '!')
				{
					_term.erase(_term.begin() + (i - 1));
					string t = _term;
					mut.append(pre);
					mut.append(t);
					mut.append(back);
				}
				else
				{
					_term.insert(i, "!");
					string t(_term.c_str());
					mut.append(pre);
					mut.append(t);
					mut.append(back);
				}
				string m(mut.c_str());
				mutants.addMutant(m, type + "xLNF");
			}
		}
	}

	return mutants;

}


Mutants DnfMutant::generateTOFmutants(string type, string exp, vector<string> terms){

	// new term (null) <- old term 

	Mutants mutants(exp);

	Utility uti;
	vector<string> oriTerms = uti.extractDNFstyle(exp);

	string oriExp = exp;
	if (oriTerms.size() <= 1)
		return mutants;
	for (int i = 0; i < terms.size(); i++)
	{
		string new_exp = oriExp;
		int pos_current_term = oriExp.find(terms[i]);
		if (oriExp.size() - pos_current_term != terms[i].size())
		{
			new_exp.erase(new_exp.begin() + pos_current_term, new_exp.begin() + pos_current_term + terms[i].size() + 1);
		}
		else
		{
			new_exp.erase(new_exp.begin() + pos_current_term - 1, new_exp.begin() + pos_current_term + terms[i].size() + 1);
		}

		string t(new_exp.c_str());
		mutants.addMutant(t, type + "xTOF");
	}
	return mutants;

}


Mutants DnfMutant::generateTNFmutants(string type, string exp, vector<string> terms){

	// new term (null) <- old term 

	Mutants mutants(exp);

	string oriExp = exp;

	Utility uti;
	vector<string> oriTerms = uti.extractDNFstyle(exp);

	if (oriTerms.size() <= 1)
		return mutants;
	for (int i = 0; i < terms.size(); i++)
	{
		string new_exp = oriExp;
		int pos_current_term = oriExp.find(terms[i]);
		string new_term = "!(" + terms[i] + ")";
		new_exp.replace(new_exp.begin() + pos_current_term, new_exp.begin() + pos_current_term + terms[i].size(), new_term);
		string t(new_exp.c_str());
		mutants.addMutant(t, type + "xTNF");
	}
	return mutants;

}


Mutants DnfMutant::generateCORFmutants(string type, string exp, vector<string> terms){
	// a DNF term departed into 2 terms 
	Mutants mutants(exp);

	string oriExp = exp;
	Utility uti;
	for (int i = 0; i < terms.size(); i++)
	{
		// for each term ,departed at every literal
		int pos_current_term = oriExp.find(terms[i]);
		vector<string> _literals_in_this_term = uti.extractCNFstyle(terms[i]);
		for (int j = 1; j < _literals_in_this_term.size(); j++)
		{
			int pos_insert_operator_or = pos_current_term + terms[i].find(_literals_in_this_term[j]);
			string new_exp = oriExp;

			new_exp.insert(pos_insert_operator_or, "+");
			string t(new_exp.c_str());
			mutants.addMutant(t, type + "xCORF");
		}

	}
	return mutants;

}
Mutants DnfMutant::generateDORFmutants(string type, string exp, vector<string> terms){
	// 2 DNF term combined into one term
	Mutants mutants(exp);

	string oriExp(exp);
	Utility uti;
	vector<string> oriTerms = uti.extractDNFstyle(exp);
	if (terms.size() < 2){
		//无法发生DORF
		return mutants;
	}
	string new_exp = terms[0] + terms[1];

	for (int i = 0; i < oriTerms.size(); i++)
	{
		if (oriTerms[i] == terms[0] || oriTerms[i] == terms[1]){
			continue;
		}
		else{
			new_exp = new_exp + "+" + oriTerms[i];
		}
	}
	mutants.addMutant(new_exp, type + "xDORF");
	return mutants;
}
Mutants DnfMutant::generateLRFmutants(string type, string exp, vector<string> terms, string model){

	Mutants mutants(exp);
	bool vars[26];
	for (int i = 0; i < 26; i++)
	{
		vars[i] = false;
	}
	string oriExp = exp;
	Utility uti;
	vector<string> oriTerms = uti.extractDNFstyle(exp);

	for (int j = 0; j < model.size(); j++)
	{
		string temp = { model[j] };
		if (temp.size() == 1)
		{
			vars[temp[0] - 'a'] = true;
		}
		else
		{
			vars[temp[1] - 'a'] = true;
		}
	}
	for (int i = 0; i < terms.size(); i++)
	{
		vector<string> literals_in_this_term = uti.extractCNFstyle(terms[i]);
		int pos = oriExp.find(terms[i]);
		for (int j = 0; j < 26; j++)
		{
			char toReplaceWith = 'a' + j;
			string s1, s2;
			s1 += toReplaceWith;
			s2 += '!';
			s2 += toReplaceWith;

			if (vars[j] && terms[i].find(toReplaceWith) == string::npos)
			{
				for (int k = 0; k < literals_in_this_term.size(); k++)
				{
					string new_term = terms[i];

					int literal_pos = new_term.find(literals_in_this_term[k]);
					new_term.replace(new_term.begin() + literal_pos, new_term.begin() + literal_pos + literals_in_this_term[k].size(), s1);
					string _mutant = oriExp;
					_mutant.replace(_mutant.begin() + pos, _mutant.begin() + pos + terms[i].size(), new_term);

					string m(_mutant.c_str());
					mutants.addMutant(m, type + "xLRF");
				}
				for (int k = 0; k < literals_in_this_term.size(); k++)
				{
					string new_term2 = terms[i];
					int literal_pos = new_term2.find(literals_in_this_term[k]);
					new_term2.replace(new_term2.begin() + literal_pos, new_term2.begin() + literal_pos + literals_in_this_term[k].size(), s2);
					string _mutant = oriExp;
					_mutant.replace(_mutant.begin() + pos, _mutant.begin() + pos + terms[i].size(), new_term2);

					string m(_mutant.c_str());
					mutants.addMutant(m, type + "xLRF");

				}

			}
		}

	}
	return mutants;
}

Mutants DnfMutant::generateENFmutants(string type, string exp, vector<string> terms){
	Mutants mutants(exp);
	string type1 = type + "xENF";
	string oriExp(exp);
	Utility uti;
	vector<string> oriTerms = uti.extractDNFstyle(exp);
	string new_exp = terms[0];
	if (terms.size() == 0)type1 = type + "xTNF";
	for (int i = 1; i < terms.size(); i++){
		new_exp = new_exp + "+" + terms[i];
	}
	new_exp = "!(" + new_exp + ")";
	for (int i = 0; i < oriTerms.size(); i++)
	{
		int isEnf = false;
		for (string term : terms){
			if (oriTerms[i] == term){
				isEnf = true;
				break;
			}
		}
		if (isEnf){
			continue;
		}
		else{
			new_exp = new_exp + "+" + oriTerms[i];
		}
	}
	mutants.addMutant(new_exp, type1);
	return mutants;
}

Mutants DnfMutant::generateLIORFmutants(string exp, vector<string> terms)
{
	Mutants mutants = generateLIFmutants(exp, terms);
	mutants += generateLOFmutants(exp, terms);
	mutants += generateLRFmutants(exp, terms);

	return mutants;
}




struct info
{
	string term;
	int pos;
	bool operator < (const info& i) { return pos < i.pos; }
};

//LNFxLNF---(1)——不同项
Mutants DnfMutant::generateLNFxLNFdoublemutants(string exp, vector<string> oriTerms, vector<string> terms, string model){
	Mutants mutants(exp);
	Utility uti;

	for (int m = 0; m < terms.size() - 1; m++){
		for (int n = m + 1; n < terms.size(); n++){
			string term1 = terms[m];
			string term2 = terms[n];

			vector<string> literals_in_term_1 = uti.extractCNFstyle(term1);
			vector<string> literals_in_term_2 = uti.extractCNFstyle(term2);

			//第一项LNF，第二项LNF
			for (int i = 0; i < literals_in_term_1.size(); i++){
				//第一项LNF
				string new_term1 = term1;
				string s1 = literals_in_term_1.at(i);//第i个literal
				string s2 = uti.getLiteralNegation(s1);//获取单词的Negation
				int pos1 = new_term1.find(s1);
				new_term1.replace(pos1, s1.size(), s2);//new_term1为 LNF 后的项

				//第二项LNF
				for (int j = 0; j < literals_in_term_2.size(); j++)
				{
					string new_term2 = term2;
					string s3 = literals_in_term_2.at(j);//第j个literal
					string s4 = uti.getLiteralNegation(s3);//获取单词的Negation
					int pos2 = new_term2.find(s3);
					new_term2.replace(pos2, s3.size(), s4);//new_term2为 LNF 后的项

					//替换生成变体
					string mutant = uti.replaceDoubleTerms(exp, oriTerms, m, n, new_term1, new_term2);
					mutants.addMutant(mutant, "LNFxLNF");
				}
			}
		}
	}
	return mutants;
}

//LNFxLNF---(2)
Mutants DnfMutant::generateDLNNFdoublemutants(string exp, vector<string> oriTerms, vector<string> terms, string model){
	Mutants mutants(exp);
	Utility uti;

	for (int k = 0; k < terms.size(); k++){
		string term = terms[k];
		int pos = exp.find(term);

		vector<string> term_literals = uti.extractCNFstyle(term);

		for (int i = 0; i < term_literals.size() - 1; i++)
		{
			//LNF
			string new_term1 = term;
			string s1 = term_literals.at(i);//第i个literal
			string s2 = uti.getLiteralNegation(s1);//获取单词的Negation
			int pos1 = new_term1.find(s1);
			new_term1.replace(pos1, s1.size(), s2);//temp_term2为 LNF 后的项

			for (int j = i + 1; j < term_literals.size(); j++)
			{
				//LNF
				string new_term2 = new_term1;
				string s3 = term_literals.at(j);//第j个literal
				string s4 = uti.getLiteralNegation(s3);//获取单词的Negation
				int pos2 = new_term2.find(s3);
				new_term2.replace(pos2, s3.size(), s4);//new_term2 为 LNF 后的项

				string mutant = exp;
				mutant.replace(mutant.begin() + pos, mutant.begin() + pos + term.size(), new_term2);

				mutants.addMutant(mutant, "DLNNF");
			}
		}
	}
	return mutants;
}

//LNFxLOF---(3)
Mutants DnfMutant::generateLNFxLOFdoublemutants(string exp, vector<string> oriTerms, vector<string> terms, string model){
	Mutants mutants(exp);
	Utility uti;

	for (int m = 0; m < terms.size() - 1; m++){
		for (int n = m + 1; n < terms.size(); n++){
			string term1 = terms[m];
			string term2 = terms[n];

			vector<string> literals_in_term_1 = uti.extractCNFstyle(term1);
			vector<string> literals_in_term_2 = uti.extractCNFstyle(term2);

			//第一项LNF，第二项LOF
			for (int i = 0; i < literals_in_term_1.size(); i++){
				//第一项LNF
				string new_term1 = term1;
				string s1 = literals_in_term_1.at(i);//第i个literal
				string s2 = uti.getLiteralNegation(s1);//获取单词的Negation
				int pos1 = new_term1.find(s1);
				new_term1.replace(pos1, s1.size(), s2);//new_term1为 LNF 后的项

				//第二项LOF
				for (int j = 0; j < literals_in_term_2.size(); j++)
				{
					string new_term2 = term2;
					string s3 = literals_in_term_2.at(j);
					int pos2 = new_term2.find(s3);
					new_term2.replace(pos2, s3.size(), "");

					//替换生成变体
					string mutant = uti.replaceDoubleTerms(exp, oriTerms, m, n, new_term1, new_term2);
					mutants.addMutant(mutant, "LNFxLOF");
				}
			}

			//第二项LNF, 第一项LOF
			for (int i = 0; i < literals_in_term_2.size(); i++){
				//第二项LNF
				string new_term1 = term2;
				string s1 = literals_in_term_2.at(i);//第i个literal
				string s2 = uti.getLiteralNegation(s1);//获取单词的Negation
				int pos1 = new_term1.find(s1);
				new_term1.replace(pos1, s1.size(), s2);//new_term1为 LNF 后的项

				//第一项LOF
				for (int j = 0; j < literals_in_term_1.size(); j++)
				{
					string new_term2 = term1;
					string s3 = literals_in_term_1.at(j);
					int pos2 = new_term2.find(s3);
					new_term2.replace(pos2, s3.size(), "");

					//替换生成变体
					string mutant = uti.replaceDoubleTerms(exp, oriTerms, m, n, new_term1, new_term2);
					mutants.addMutant(mutant, "LNFxLOF");
				}
			}
		}
	}
	return mutants;
}

//LNFxLOF---(4)
Mutants DnfMutant::generateDLNOFdoublemutants(string exp, vector<string> oriTerms, vector<string> terms, string model){
	Mutants mutants(exp);
	Utility uti;

	for (int k = 0; k < terms.size(); k++) {
		string term = terms[k];
		int pos = exp.find(term);

		vector<string> literals_in_term = uti.extractCNFstyle(term);

		for (int i = 0; i < literals_in_term.size(); i++)
		{
			//LNF
			string new_term1 = term;
			string s1 = literals_in_term.at(i);//第i个literal
			string s2 = uti.getLiteralNegation(s1);//获取单词的Negation
			int pos1 = new_term1.find(s1);
			new_term1.replace(pos1, s1.size(), s2);//temp_term2为 LNF 后的项

			//LOF
			for (int j = 0; j < literals_in_term.size(); j++)
			{
				if (j == i){
					continue;
				}
				string new_term2 = new_term1;
				string s1 = literals_in_term.at(j);
				int pos1 = new_term2.find(s1);
				new_term2.replace(pos1, s1.size(), "");

				string mutant = exp;
				mutant.replace(mutant.begin() + pos, mutant.begin() + pos + term.size(), new_term2);
				mutants.addMutant(mutant, "DLNOF");
			}
		}
	}
	return mutants;
}

//LNFxLIF---(5)
Mutants DnfMutant::generateLNFxLIFdoublemutants(string exp, vector<string> oriTerms, vector<string> terms, string model){
	Mutants mutants(exp);
	Utility uti;

	for (int m = 0; m < terms.size() - 1; m++)
	{
		for (int n = m + 1; n < terms.size(); n++)
		{
			string term1 = terms[m];
			string term2 = terms[n];

			vector<string> literals_in_term_1 = uti.extractCNFstyle(term1);
			vector<string> literals_in_term_2 = uti.extractCNFstyle(term2);

			for (int k = 0; k < model.length(); k++)
			{
				wchar_t toReplaceWith1 = model[k];//插入的词

				//第一项LIF，第二项LNF
				if (term1.find(toReplaceWith1) == string::npos)
				{
					//第一项LIF
					string s11, s12;
					s11 += toReplaceWith1;
					s12 += '!';
					s12 += toReplaceWith1;
					string new_term11 = (term1 + s11);
					string new_term12 = (term1 + s12);

					//第二项LNF
					for (int j = 0; j < literals_in_term_2.size(); j++)
					{
						string new_term2 = term2;
						string s1 = literals_in_term_2.at(j);//第j个literal
						string s2 = uti.getLiteralNegation(s1);//获取单词的Negation
						int pos1 = new_term2.find(s1);
						new_term2.replace(pos1, s1.size(), s2);//temp_term2为 LNF 后的项

						//替换生成变体
						string mutant = uti.replaceDoubleTerms(exp, oriTerms, m, n, new_term11, new_term2);
						mutants.addMutant(mutant, "LNFxLIF");

						mutant = uti.replaceDoubleTerms(exp, oriTerms, m, n, new_term12, new_term2);
						mutants.addMutant(mutant, "LNFxLIF");
					}
				}

				//第二项LIF，第一项LNF
				if (term2.find(toReplaceWith1) == string::npos)
				{
					//第二项LIF
					string s11, s12;
					s11 += toReplaceWith1;
					s12 += '!';
					s12 += toReplaceWith1;
					string new_term11 = (term2 + s11);
					string new_term12 = (term2 + s12);

					//第一项LNF
					for (int j = 0; j < literals_in_term_1.size(); j++)
					{
						string new_term2 = term1;
						string s1 = literals_in_term_1.at(j);//第j个literal
						string s2 = uti.getLiteralNegation(s1);//获取单词的Negation
						int pos1 = new_term2.find(s1);
						new_term2.replace(pos1, s1.size(), s2);//temp_term2为 LNF 后的项

						//替换生成变体
						string mutant = uti.replaceDoubleTerms(exp, oriTerms, m, n, new_term2, new_term11);
						mutants.addMutant(mutant, "LNFxLIF");

						mutant = uti.replaceDoubleTerms(exp, oriTerms, m, n, new_term2, new_term12);
						mutants.addMutant(mutant, "LNFxLIF");
					}
				}
			}
		}
	}
	return mutants;
}

//LNFxLIF---(6)
Mutants DnfMutant::generateDLNIFdoublemutants(string exp, vector<string> oriTerms, vector<string> terms, string model){
	Mutants mutants(exp);
	Utility uti;

	for (int k = 0; k < terms.size(); k++){
		string term = terms[k];
		int pos = exp.find(term);
		vector<string> literals_in_term = uti.extractCNFstyle(term);

		for (int j = 0; j < literals_in_term.size(); j++)
		{
			//第一步：LNF
			string temp_term1 = term;
			string s1 = literals_in_term.at(j);//第j个literal
			string s2 = uti.getLiteralNegation(s1);//获取s1单词的Negation
			int pos1 = temp_term1.find(s1);
			temp_term1.replace(pos1, s1.size(), s2);

			//第二步：LIF
			for (int j = 0; j < model.length(); j++)
			{
				//char toInsert = 'a' + j;
				char toInsert = model[j];
				if (term.find(toInsert) == string::npos)//不在当前项中的词literal
				{
					string s3, s4;
					s3 += toInsert;
					s4 += '!';
					s4 += toInsert;

					string temp_term3 = temp_term1;
					string temp_term4 = temp_term1;
					temp_term3 += s3;
					temp_term4 += s4;

					string mutant = exp;
					mutant.replace(mutant.begin() + pos, mutant.begin() + pos + term.size(), temp_term3);
					mutants.addMutant(mutant, "DLNIF");

					mutant = exp;
					mutant.replace(mutant.begin() + pos, mutant.begin() + pos + term.size(), temp_term4);
					mutants.addMutant(mutant, "DLNIF");
				}
			}
		}
	}
	return mutants;
}

//LNFxLRF---(7)
Mutants DnfMutant::generateLNFxLRFdoublemutants(string exp, vector<string> oriTerms, vector<string> terms, string model){
	Mutants mutants(exp);
	Utility uti;

	for (int m = 0; m < terms.size() - 1; m++){
		for (int n = m + 1; n < terms.size(); n++){
			string term1 = terms[m];
			string term2 = terms[n];
			vector<string> literals_in_term_1 = uti.extractCNFstyle(term1);
			vector<string> literals_in_term_2 = uti.extractCNFstyle(term2);

			for (int k = 0; k < model.length(); k++)
			{
				wchar_t toReplaceWith1 = model[k];//替换的词
				if (term1.find(toReplaceWith1) == string::npos)
				{
					string s11, s12;
					s11 += toReplaceWith1;
					s12 += '!';
					s12 += toReplaceWith1;

					//第一项LRF，第二项LNF
					for (int p = 0; p < literals_in_term_1.size(); p++)
					{
						//第一项LRF
						string new_term11 = term1;
						string new_term12 = term1;
						int literal_pos = new_term11.find(literals_in_term_1.at(p));
						new_term11.replace(new_term11.begin() + literal_pos, new_term11.begin() + literal_pos + literals_in_term_1.at(p).size(), s11);
						new_term12.replace(new_term12.begin() + literal_pos, new_term12.begin() + literal_pos + literals_in_term_1.at(p).size(), s12);

						//第二项LNF
						for (int j = 0; j < literals_in_term_2.size(); j++)
						{
							string new_term2 = term2;
							string s1 = literals_in_term_2.at(j);//第j个literal
							string s2 = uti.getLiteralNegation(s1);//获取单词的Negation
							int pos1 = new_term2.find(s1);
							new_term2.replace(pos1, s1.size(), s2);//temp_term2为 LNF 后的项

							string mutant = uti.replaceDoubleTerms(exp, oriTerms, m, n, new_term11, new_term2);
							mutants.addMutant(mutant, "LNFxLRF");
							mutant = uti.replaceDoubleTerms(exp, oriTerms, m, n, new_term12, new_term2);
							mutants.addMutant(mutant, "LNFxLRF");
						}
					}
				}

				if (term2.find(toReplaceWith1) == string::npos)
				{
					string s11, s12;
					s11 += toReplaceWith1;
					s12 += '!';
					s12 += toReplaceWith1;
					//第二项LRF，第一项LNF
					for (int p = 0; p < literals_in_term_2.size(); p++)
					{
						//第二项LRF
						string new_term11 = term2;
						string new_term12 = term2;
						int literal_pos = new_term11.find(literals_in_term_2.at(p));
						new_term11.replace(new_term11.begin() + literal_pos, new_term11.begin() + literal_pos + literals_in_term_2.at(p).size(), s11);
						new_term12.replace(new_term12.begin() + literal_pos, new_term12.begin() + literal_pos + literals_in_term_2.at(p).size(), s12);

						//第一项LNF
						for (int j = 0; j < literals_in_term_1.size(); j++)
						{
							string new_term2 = term1;
							string s1 = literals_in_term_1.at(j);//第j个literal
							string s2 = uti.getLiteralNegation(s1);//获取单词的Negation
							int pos1 = new_term2.find(s1);
							new_term2.replace(pos1, s1.size(), s2);//temp_term2为 LNF 后的项

							string mutant = uti.replaceDoubleTerms(exp, oriTerms, m, n, new_term2, new_term11);
							mutants.addMutant(mutant, "LNFxLRF");
							mutant = uti.replaceDoubleTerms(exp, oriTerms, m, n, new_term2, new_term12);
							mutants.addMutant(mutant, "LNFxLRF");
						}
					}
				}
			}
		}
	}
	return mutants;
}

//LNFxLRF---(8)
Mutants DnfMutant::generateDLNRFdoublemutants(string exp, vector<string> oriTerms, vector<string> terms, string model){
	Mutants mutants(exp);
	Utility uti;

	for (int m = 0; m < terms.size(); m++){
		string term = terms[m];
		int pos = exp.find(term);

		vector<string> literals_in_term = uti.extractCNFstyle(term);
		for (int i = 0; i < literals_in_term.size(); i++)//遍历literal
		{
			//第一步：LNF
			string temp_term1 = term;
			string s1 = literals_in_term.at(i);//第i个literal
			string s2 = uti.getLiteralNegation(s1);//获取单词的Negation
			int pos1 = temp_term1.find(s1);
			temp_term1.replace(pos1, s1.size(), s2);//temp_term1为 LNF 后的项

			//第二步：LRF
			for (int j = 0; j < literals_in_term.size(); j++)//替换第j个literal（不能是上边LNF的那个词）
			{
				if (j == i)
					continue;
				for (int k = 0; k < model.length(); k++)
				{
					//char toReplaceWith = 'a' + k;
					char toReplaceWith = model[k];
					if (term.find(toReplaceWith) == string::npos)//不在当前term中的literal
					{
						string s3, s4;
						s3 += toReplaceWith;
						s4 += '!';
						s4 += toReplaceWith;

						string temp_literals = literals_in_term.at(j);//要被替换的单词
						int pos1 = temp_term1.find(temp_literals);

						string temp_term2 = temp_term1;
						string temp_term3 = temp_term1;

						temp_term2.replace(temp_term2.begin() + pos1, temp_term2.begin() + pos1 + temp_literals.size(), s3);
						temp_term3.replace(temp_term3.begin() + pos1, temp_term3.begin() + pos1 + temp_literals.size(), s4);

						string mutant = exp;
						mutant.replace(mutant.begin() + pos, mutant.begin() + pos + term.size(), temp_term2);
						mutants.addMutant(mutant, "DLNRF");

						mutant = exp;
						mutant.replace(mutant.begin() + pos, mutant.begin() + pos + term.size(), temp_term3);
						mutants.addMutant(mutant, "DLNRF");
					}
				}
			}
		}
	}
	return mutants;
}

//LOFxLOF---(9)
Mutants DnfMutant::generateLOFxLOFdoublemutants(string exp, vector<string> oriTerms, vector<string> terms, string model){
	Mutants mutants(exp);
	Utility uti;

	for (int m = 0; m < terms.size() - 1; m++){
		for (int n = m + 1; n < terms.size(); n++){
			string term1 = terms[m];
			string term2 = terms[n];

			vector<string> literals_in_term_1 = uti.extractCNFstyle(term1);
			vector<string> literals_in_term_2 = uti.extractCNFstyle(term2);

			for (int j = 0; j < literals_in_term_1.size(); j++)//遗漏第j个词
			{
				//第一个项：LOF
				string new_term1 = term1;
				string s1 = literals_in_term_1.at(j);
				int pos1 = new_term1.find(s1);
				new_term1.replace(pos1, s1.size(), "");

				for (int k = 0; k < literals_in_term_2.size(); k++)//遗漏第k个词
				{
					//第二个项：LOF
					string new_term2 = term2;
					string s2 = literals_in_term_2.at(k);
					int pos2 = new_term2.find(s2);
					new_term2.replace(pos2, s2.size(), "");

					string mutant = uti.replaceDoubleTerms(exp, oriTerms, m, n, new_term1, new_term2);
					mutants.addMutant(mutant, "LOFxLOF");
				}
			}
		}
	}
	return mutants;
}

//LOFxLOF---(10)
Mutants DnfMutant::generateDLOOFdoublemutants(string exp, vector<string> oriTerms, vector<string> terms, string model){
	Mutants mutants(exp);
	Utility uti;

	for (int m = 0; m < terms.size(); m++){
		string term = terms[m];
		if (term.length() > 2){
			int pos = exp.find(term);
			vector<string> literals_in_term = uti.extractCNFstyle(term);

			for (int j = 0; j < literals_in_term.size(); j++)//遗漏第j个词
			{
				//第一次：LOF
				string new_term1 = term;
				string s1 = literals_in_term.at(j);
				int pos1 = new_term1.find(s1);
				new_term1.replace(pos1, s1.size(), "");

				for (int k = j + 1; k < literals_in_term.size(); k++)//遗漏第k个词
				{
					//第二次：LOF
					string new_term2 = new_term1;
					string s2 = literals_in_term.at(k);
					int pos2 = new_term2.find(s2);
					new_term2.replace(pos2, s2.size(), "");

					string mutant = exp;
					mutant.replace(mutant.begin() + pos, mutant.begin() + pos + term.size(), new_term2);
					mutants.addMutant(mutant, "DLOOF");
				}
			}
		}
	}
	return mutants;
}

//LOFxLIF---(11)—— 不同项
Mutants DnfMutant::generateLOFxLIFdoublemutants(string exp, vector<string> oriTerms, vector<string> terms, string model){
	Mutants mutants(exp);
	Utility uti;
	for (int i = 0; i < terms.size() - 1; i++)
	{
		for (int j = i + 1; j < terms.size(); j++)
		{
			string term1 = terms[i];
			string term2 = terms[j];

			vector<string> literals_in_this_term1 = uti.extractCNFstyle(term1);
			vector<string> literals_in_this_term2 = uti.extractCNFstyle(term2);

			for (int k = 0; k < model.length(); k++)
			{
				wchar_t toReplaceWith1 = model[k];
				string s11, s12;
				s11 += toReplaceWith1;
				s12 += '!';
				s12 += toReplaceWith1;

				//第一个项LIF
				if (term1.find(toReplaceWith1) == string::npos)
				{
					string s11, s12;
					s11 += toReplaceWith1;
					s12 += '!';
					s12 += toReplaceWith1;

					string new_term11 = (term1 + s11);
					string new_term12 = (term1 + s12);

					//第二个项LOF
					for (int n = 0; n < literals_in_this_term2.size(); n++)
					{
						string new_term21 = term2;
						string s1 = literals_in_this_term2.at(n);
						int pos1 = term2.find(s1);
						new_term21.replace(pos1, s1.size(), "");

						string mutant = uti.replaceDoubleTerms(exp, oriTerms, i, j, new_term11, new_term21);
						mutants.addMutant(mutant, "LOFxLIF");

						mutant = uti.replaceDoubleTerms(exp, oriTerms, i, j, new_term12, new_term21);
						mutants.addMutant(mutant, "LOFxLIF");
					}
				}

				//第二个项LIF
				if (term2.find(toReplaceWith1) == string::npos)
				{
					string s11, s12;
					s11 += toReplaceWith1;
					s12 += '!';
					s12 += toReplaceWith1;

					string new_term11 = (term2 + s11);
					string new_term12 = (term2 + s12);

					//第一个项LOF
					for (int n = 0; n < literals_in_this_term1.size(); n++)
					{
						string new_term21 = term1;
						string s1 = literals_in_this_term1.at(n);
						int pos1 = term1.find(s1);
						new_term21.replace(pos1, s1.size(), "");

						string mutant = uti.replaceDoubleTerms(exp, oriTerms, i, j, new_term21, new_term11);
						mutants.addMutant(mutant, "LOFxLIF");

						mutant = uti.replaceDoubleTerms(exp, oriTerms, i, j, new_term21, new_term12);
						mutants.addMutant(mutant, "LOFxLIF");
					}
				}
			}
		}
	}
	return mutants;
}

//LOFxLRF---(12)—— 不同项
Mutants DnfMutant::generateLOFxLRFdoublemutants(string exp, vector<string> oriTerms, vector<string> terms, string model){
	Mutants mutants(exp);
	Utility uti;

	for (int i = 0; i < terms.size() - 1; i++){
		for (int j = i + 1; j < terms.size(); j++){
			string term1 = terms[i];
			string term2 = terms[j];

			vector<string> literals_in_this_term1 = uti.extractCNFstyle(term1);
			vector<string> literals_in_this_term2 = uti.extractCNFstyle(term2);

			for (int k = 0; k < model.length(); k++)
			{
				//wchar_t toReplaceWith1 = 'a' + k;
				wchar_t toReplaceWith1 = model[k];
				string s11, s12;
				s11 += toReplaceWith1;
				s12 += '!';
				s12 += toReplaceWith1;

				//第一个项LRF，第二个项LOF
				if (term1.find(toReplaceWith1) == string::npos)
				{
					for (int l = 0; l < literals_in_this_term1.size(); l++)
					{
						string new_term11 = term1;
						string new_term12 = term1;
						int literal_pos1 = new_term11.find(literals_in_this_term1[l]);
						new_term11.replace(new_term11.begin() + literal_pos1, new_term11.begin() + literal_pos1 + literals_in_this_term1[l].size(), s11);
						new_term12.replace(new_term12.begin() + literal_pos1, new_term12.begin() + literal_pos1 + literals_in_this_term1[l].size(), s12);

						//第二个项LOF
						for (int n = 0; n < literals_in_this_term2.size(); n++)
						{
							string new_term21 = term2;
							string s1 = literals_in_this_term2.at(n);
							int pos1 = new_term21.find(s1);
							new_term21.replace(pos1, s1.size(), "");

							string mutant = uti.replaceDoubleTerms(exp, oriTerms, i, j, new_term11, new_term21);
							mutants.addMutant(mutant, "LOFxLRF");

							mutant = uti.replaceDoubleTerms(exp, oriTerms, i, j, new_term12, new_term21);
							mutants.addMutant(mutant, "LOFxLRF");

						}
					}
				}

				//第二个项LRF，第一个项LOF
				if (term2.find(toReplaceWith1) == string::npos)
				{
					for (int l = 0; l < literals_in_this_term2.size(); l++)
					{
						string new_term11 = term2;
						string new_term12 = term2;
						int literal_pos1 = new_term11.find(literals_in_this_term2[l]);
						new_term11.replace(new_term11.begin() + literal_pos1, new_term11.begin() + literal_pos1 + literals_in_this_term2[l].size(), s11);
						new_term12.replace(new_term12.begin() + literal_pos1, new_term12.begin() + literal_pos1 + literals_in_this_term2[l].size(), s12);

						//第一个项LOF
						for (int n = 0; n < literals_in_this_term1.size(); n++)
						{
							string new_term21 = term1;
							string s1 = literals_in_this_term1.at(n);
							int pos1 = term1.find(s1);
							new_term21.replace(pos1, s1.size(), "");

							string mutant = uti.replaceDoubleTerms(exp, oriTerms, i, j, new_term21, new_term11);
							mutants.addMutant(mutant, "LOFxLRF");

							mutant = uti.replaceDoubleTerms(exp, oriTerms, i, j, new_term21, new_term12);
							mutants.addMutant(mutant, "LOFxLRF");

						}
					}
				}
			}
		}
	}
	return mutants;
}

//LOFxLRF ---(13)——同项
Mutants DnfMutant::generateDLORFdoublemutants(string exp, vector<string> oriTerms, vector<string> terms, string model)
{
	Mutants mutants(exp);
	Utility uti;

	for (int m = 0; m < terms.size(); m++){
		string term = terms[m];

		if (term.length() >= 2){
			int pos = exp.find(term);
			vector<string> literals_in_term = uti.extractCNFstyle(term);

			for (int i = 0; i < literals_in_term.size(); i++)
			{
				//第i个词LOF
				string new_term1 = term;
				string s1 = literals_in_term.at(i);
				int pos1 = new_term1.find(s1);
				new_term1.replace(pos1, s1.size(), "");

				//第j个词LRF
				for (int j = i + 1; j < literals_in_term.size(); j++)
				{
					for (int k = 0; k < model.length(); k++)
					{
						char toReplaceWith = model[k];
						string s1, s2;
						s1 += toReplaceWith;
						s2 += '!';
						s2 += toReplaceWith;

						if (term.find(toReplaceWith) == string::npos)//不在当前term中的literal
						{
							string new_term11 = new_term1;
							string new_term12 = new_term1;
							int literal_pos = new_term11.find(literals_in_term[j]);//词在当前项中的位置
							new_term11.replace(new_term11.begin() + literal_pos, new_term11.begin() + literal_pos + literals_in_term[j].size(), s1);
							new_term12.replace(new_term12.begin() + literal_pos, new_term12.begin() + literal_pos + literals_in_term[j].size(), s2);


							string mutant = exp;
							mutant.replace(mutant.begin() + pos, mutant.begin() + pos + term.size(), new_term11);
							mutants.addMutant(mutant, "DLORF");

							mutant = exp;
							mutant.replace(mutant.begin() + pos, mutant.begin() + pos + term.size(), new_term12);
							mutants.addMutant(mutant, "DLORF");
						}
					}
				}
			}
		}
	}
	return mutants;
}

//LIFxLIF——(14)---不同项
Mutants DnfMutant::generateLIFxLIFdoublemutants(string exp, vector<string> oriTerms, vector<string> terms, string model)
{
	Mutants mutants(exp);
	Utility uti;

	for (int m = 0; m < terms.size() - 1; m++){
		for (int n = m + 1; n < terms.size(); n++){
			string term1 = terms[m];
			string term2 = terms[n];

			string new_term11;
			string new_term12;
			string new_term21;
			string new_term22;

			for (int k = 0; k < model.length(); k++)
			{
				char toReplaceWith1 = model[k];
				if (term1.find(toReplaceWith1) == string::npos)
				{
					string s11, s12;
					s11 += toReplaceWith1;
					s12 += '!';
					s12 += toReplaceWith1;
					new_term11 = (term1 + s11);
					new_term12 = (term1 + s12);

					for (int l = 0; l < model.length(); l++)
					{
						char toReplaceWith2 = model[l];
						if (term2.find(toReplaceWith2) == string::npos)
						{
							string s21, s22;
							s21 += toReplaceWith2;
							s22 += '!';
							s22 += toReplaceWith2;

							new_term21 = (term2 + s21);
							new_term22 = (term2 + s22);

							string mutant = uti.replaceDoubleTerms(exp, oriTerms, m, n, new_term11, new_term21);
							mutants.addMutant(mutant, "LIFxLIF");

							mutant = uti.replaceDoubleTerms(exp, oriTerms, m, n, new_term11, new_term22);
							mutants.addMutant(mutant, "LIFxLIF");

							mutant = uti.replaceDoubleTerms(exp, oriTerms, m, n, new_term12, new_term21);
							mutants.addMutant(mutant, "LIFxLIF");

							mutant = uti.replaceDoubleTerms(exp, oriTerms, m, n, new_term12, new_term22);
							mutants.addMutant(mutant, "LIFxLIF");
						}
					}
				}
			}
		}
	}
	return mutants;

}

//LIFxLIF——(15)---同项
Mutants DnfMutant::generateDLIIFdoublemutants(string exp, vector<string> oriTerms, vector<string> terms, string model)
{
	Mutants mutants(exp);
	Utility uti;

	for (int m = 0; m < terms.size(); m++){
		string term = terms[m];
		int pos = exp.find(term);

		//遍历term
		string new_term11 = term;
		string new_term12 = term;

		//遍历不在当前项中的词literals
		for (int j = 0; j < model.length(); j++)
		{
			char toInsert1 = model.at(j);
			if (term.find(toInsert1) == string::npos)//不在当前项中的词literal
			{
				string s11, s12;
				s11 += toInsert1;//不在当前项中的词literals
				s12 += '!';
				s12 += toInsert1;

				//第一次LIF
				new_term11 += s11;
				new_term12 += s12;

				for (int k = j + 1; k < model.length(); k++)//遍历model剩下的词literal
				{
					char toInsert2 = model.at(k);
					if (new_term11.find(toInsert2) == string::npos)
					{
						string s21, s22;
						s21 += toInsert2;
						s22 += '!';
						s22 += toInsert2;

						string new_term21, new_term22, new_term23, new_term24;
						new_term21 = new_term11;
						new_term22 = new_term11;
						new_term23 = new_term12;
						new_term24 = new_term12;

						//第二次LIF
						new_term21 += s21;
						new_term22 += s22;
						new_term23 += s21;
						new_term24 += s22;

						//变体1
						string mutant = exp;
						mutant.replace(mutant.begin() + pos, mutant.begin() + pos + term.size(), new_term21);
						mutants.addMutant(mutant, "DLIIF");
						//变体2
						mutant = exp;
						mutant.replace(mutant.begin() + pos, mutant.begin() + pos + term.size(), new_term22);
						mutants.addMutant(mutant, "DLIIF");
						//变体3
						mutant = exp;
						mutant.replace(mutant.begin() + pos, mutant.begin() + pos + term.size(), new_term23);
						mutants.addMutant(mutant, "DLIIF");
						//变体4
						mutant = exp;
						mutant.replace(mutant.begin() + pos, mutant.begin() + pos + term.size(), new_term24);
						mutants.addMutant(mutant, "DLIIF");
					}
				}
			}
		}
	}
	return mutants;
}

//LIFxLRF---(16)---不同项
Mutants DnfMutant::generateLIFxLRFdoublemutants(string exp, vector<string> oriTerms, vector<string> terms, string model)
{
	Mutants mutants(exp);
	Utility uti;

	for (int m = 0; m < terms.size() - 1; m++){
		for (int n = m + 1; n < terms.size(); n++){
			string term1 = terms[m];
			string term2 = terms[n];

			vector<string> term_literal_1 = uti.extractCNFstyle(term1);
			vector<string> term_literal_2 = uti.extractCNFstyle(term2);
			//开始生成变体
			for (int k = 0; k < model.length(); k++)
			{
				for (int l = 0; l < model.length(); l++)
				{
					wchar_t toReplaceWith1 = model[k];
					wchar_t toReplaceWith2 = model[l];

					if (term1.find(toReplaceWith1) == string::npos && term2.find(toReplaceWith2) == string::npos)
					{
						string _mutant;
						string s11, s12, s21, s22;
						s11 += toReplaceWith1;
						s12 += '!';
						s12 += toReplaceWith1;
						s21 += toReplaceWith2;
						s22 += '!';
						s22 += toReplaceWith2;
						string new_term11 = (term1 + s11);
						string new_term12 = (term1 + s12);
						string new_term21 = (term2 + s21);
						string new_term22 = (term2 + s22);

						//第一项LRF，第二项LIF
						for (int p = 0; p < term_literal_1.size(); p++)
						{
							//第一项LRF
							string new_term31 = term1;
							int literal_pos = new_term31.find(term_literal_1.at(p));
							new_term31.replace(new_term31.begin() + literal_pos, new_term31.begin() + literal_pos + term_literal_1.at(p).size(), s11);

							//第二项LIF
							string new_term32 = new_term21;
							string mutant = uti.replaceDoubleTerms(exp, oriTerms, m, n, new_term31, new_term32);
							mutants.addMutant(mutant, "LIFxLRF");

							//第二项LIF非
							string new_term33 = new_term22;
							mutant = uti.replaceDoubleTerms(exp, oriTerms, m, n, new_term31, new_term33);
							mutants.addMutant(mutant, "LIFxLRF");
						}

						//第一项LRF非，第二项LIF
						for (int p = 0; p < term_literal_1.size(); p++)
						{
							//第一项LRF非
							string new_term41 = term1;
							int literal_pos = new_term41.find(term_literal_1.at(p));
							new_term41.replace(new_term41.begin() + literal_pos, new_term41.begin() + literal_pos + term_literal_1.at(p).size(), s12);

							//第二项LIF
							string new_term42 = new_term21;
							string mutant = uti.replaceDoubleTerms(exp, oriTerms, m, n, new_term41, new_term42);
							mutants.addMutant(mutant, "LIFxLRF");

							//第二项LIF非
							string new_term43 = new_term22;
							mutant = uti.replaceDoubleTerms(exp, oriTerms, m, n, new_term41, new_term43);
							mutants.addMutant(mutant, "LIFxLRF");
						}


						//第一项LIF，第二项LRF
						for (int p = 0; p < term_literal_2.size(); p++)
						{
							//第二项LRF
							string new_term51 = term2;
							int literal_pos = new_term51.find(term_literal_2.at(p));
							new_term51.replace(new_term51.begin() + literal_pos, new_term51.begin() + literal_pos + term_literal_2.at(p).size(), s21);
							//第一项LIF
							string new_term52 = new_term11;
							string mutant = uti.replaceDoubleTerms(exp, oriTerms, m, n, new_term52, new_term51);
							mutants.addMutant(mutant, "LIFxLRF");

							//第一项LIF非
							string new_term53 = new_term12;
							mutant = uti.replaceDoubleTerms(exp, oriTerms, m, n, new_term53, new_term51);
							mutants.addMutant(mutant, "LIFxLRF");
						}

						//第一项LIF，第二项LRF非
						for (int p = 0; p < term_literal_2.size(); p++)
						{
							//第二项LRF非
							string new_term61 = term2;
							int literal_pos = new_term61.find(term_literal_2.at(p));
							new_term61.replace(new_term61.begin() + literal_pos, new_term61.begin() + literal_pos + term_literal_2.at(p).size(), s22);
							//第一项LIF
							string new_term62 = new_term11;
							string mutant = uti.replaceDoubleTerms(exp, oriTerms, m, n, new_term62, new_term61);
							mutants.addMutant(mutant, "LIFxLRF");

							//第一项LIF非
							string new_term63 = new_term12;
							mutant = uti.replaceDoubleTerms(exp, oriTerms, m, n, new_term63, new_term61);
							mutants.addMutant(mutant, "LIFxLRF");
						}
					}
				}
			}
		}
	}
	return mutants;
}

//LIFxLRF---(17)---同项
Mutants DnfMutant::generateDLIRFdoublemutants(string exp, vector<string> oriTerms, vector<string> terms, string model){
	Mutants mutants(exp);
	Utility uti;

	for (int m = 0; m < terms.size(); m++){
		string term = terms[m];
		int pos = exp.find(term);

		vector<string> literals_in_this_term = uti.extractCNFstyle(term);
		//LRF
		for (int j = 0; j < model.length(); j++)
		{
			char toReplaceWith = model[j];
			string s11, s12;
			s11 += toReplaceWith;
			s12 += '!';
			s12 += toReplaceWith;

			if (term.find(toReplaceWith) == string::npos)
			{
				for (int k = 0; k < literals_in_this_term.size(); k++)//遍历当前term中存在的词
				{
					string new_term11 = term;
					string new_term12 = term;
					int literal_pos = new_term11.find(literals_in_this_term[k]);//词在当前项中的位置
					new_term11.replace(new_term11.begin() + literal_pos, new_term11.begin() + literal_pos + literals_in_this_term[k].size(), s11);
					new_term12.replace(new_term12.begin() + literal_pos, new_term12.begin() + literal_pos + literals_in_this_term[k].size(), s12);

					//LIF
					for (int l = 0; l < model.length(); l++)
					{
						char toReplaceWith2 = model[l];
						string s21, s22;
						s21 += toReplaceWith2;
						s22 += '!';
						s22 += toReplaceWith2;
						if (new_term11.find(toReplaceWith2) == string::npos && term.find(toReplaceWith2) == string::npos)
						{
							string new_term21 = (new_term11 + s21);
							string new_term22 = (new_term11 + s22);
							string new_term23 = (new_term12 + s21);
							string new_term24 = (new_term12 + s22);
							//变体
							string mutant = exp;
							mutant.replace(mutant.begin() + pos, mutant.begin() + pos + term.size(), new_term21);
							mutants.addMutant(mutant, "DLIRF");

							mutant = exp;
							mutant.replace(mutant.begin() + pos, mutant.begin() + pos + term.size(), new_term22);
							mutants.addMutant(mutant, "DLIRF");

							mutant = exp;
							mutant.replace(mutant.begin() + pos, mutant.begin() + pos + term.size(), new_term23);
							mutants.addMutant(mutant, "DLIRF");

							mutant = exp;
							mutant.replace(mutant.begin() + pos, mutant.begin() + pos + term.size(), new_term24);
							mutants.addMutant(mutant, "DLIRF");
						}
					}
				}
			}
		}
	}
	return mutants;
}

//LRFxLRF---(18)---不同项
Mutants DnfMutant::generateLRFxLRFdoublemutants(string exp, vector<string> oriTerms, vector<string> terms, string model){
	Mutants mutants(exp);
	Utility uti;

	for (int i = 0; i < terms.size() - 1; i++){
		for (int j = i + 1; j < terms.size(); j++){
			string term1 = terms[i];
			string term2 = terms[j];

			vector<string> literals_in_this_term1 = uti.extractCNFstyle(term1);
			vector<string> literals_in_this_term2 = uti.extractCNFstyle(term2);
			for (int k = 0; k < model.length(); k++)
			{
				wchar_t toReplaceWith1 = model[k];
				string s11, s12;
				s11 += toReplaceWith1;
				s12 += '!';
				s12 += toReplaceWith1;

				if (term1.find(toReplaceWith1) == string::npos)
				{
					for (int l = 0; l < literals_in_this_term1.size(); l++)
					{
						//第一个项LRF
						string new_term11 = term1;
						string new_term12 = term1;
						int literal_pos1 = new_term11.find(literals_in_this_term1[l]);
						new_term11.replace(new_term11.begin() + literal_pos1, new_term11.begin() + literal_pos1 + literals_in_this_term1[l].size(), s11);
						new_term12.replace(new_term12.begin() + literal_pos1, new_term12.begin() + literal_pos1 + literals_in_this_term1[l].size(), s12);

						//第二个项LRF
						for (int m = 0; m < model.length(); m++)
						{
							//wchar_t toReplaceWith2 = 'a' + m;
							wchar_t toReplaceWith2 = model[m];
							string s21, s22;
							s21 += toReplaceWith2;
							s22 += '!';
							s22 += toReplaceWith2;
							if (term2.find(toReplaceWith2) == string::npos)
							{
								for (int n = 0; n < literals_in_this_term2.size(); n++)
								{
									string new_term21 = term2;
									string new_term22 = term2;
									int literal_pos2 = new_term21.find(literals_in_this_term2[n]);
									new_term21.replace(new_term21.begin() + literal_pos2, new_term21.begin() + literal_pos2 + literals_in_this_term2[n].size(), s21);
									new_term22.replace(new_term22.begin() + literal_pos2, new_term22.begin() + literal_pos2 + literals_in_this_term2[n].size(), s22);

									string mutant = uti.replaceDoubleTerms(exp, oriTerms, i, j, new_term11, new_term21);
									mutants.addMutant(mutant, "LRFxLRF");
									mutant = uti.replaceDoubleTerms(exp, oriTerms, i, j, new_term12, new_term21);
									mutants.addMutant(mutant, "LRFxLRF");
									mutant = uti.replaceDoubleTerms(exp, oriTerms, i, j, new_term11, new_term22);
									mutants.addMutant(mutant, "LRFxLRF");
									mutant = uti.replaceDoubleTerms(exp, oriTerms, i, j, new_term12, new_term22);
									mutants.addMutant(mutant, "LRFxLRF");
								}
							}
						}
					}
				}
			}
		}
	}
	return mutants;
}

//LRFxLRF---(19)---同项
Mutants DnfMutant::generateDLRRFdoublemutants(string exp, vector<string> oriTerms, vector<string> terms, string model)
{
	Mutants mutants(exp);
	Utility uti;

	for (int n = 0; n < terms.size(); n++)
	{
		string term = terms[n];
		if (term.length() >= 2)
		{
			int pos = exp.find(term);

			vector<string> literals_in_this_term = uti.extractCNFstyle(term);//获取当前项中的词
			string new_term11 = term;
			string new_term12 = term;

			for (int j = 0; j < model.length(); j++)//遍历不在当前项中的词literals
			{
				char toReplaceWith = model[j];
				string s11, s12;
				s11 += toReplaceWith;//不在当前项中的词literals
				s12 += '!';
				s12 += toReplaceWith;
				if (term.find(toReplaceWith) == string::npos)//不在当前项中的词literals
				{
					for (int k = 0; k < literals_in_this_term.size(); k++)
					{
						new_term11 = term;//当前项
						new_term12 = term;

						//第一次 LRF
						int literal_pos = new_term11.find(literals_in_this_term[k]);
						new_term11.replace(new_term11.begin() + literal_pos, new_term11.begin() + literal_pos + literals_in_this_term[k].size(), s11);
						new_term12.replace(new_term12.begin() + literal_pos, new_term12.begin() + literal_pos + literals_in_this_term[k].size(), s12);

						for (int l = j + 1; l < model.length(); l++)//遍历剩下的词literal
						{
							char toReplaceWith2 = model[l];
							string s21, s22;
							s21 += toReplaceWith2;
							s22 += '!';
							s22 += toReplaceWith2;
							if (new_term11.find(toReplaceWith2) == string::npos && term.find(toReplaceWith2) == string::npos)
							{
								for (int m = k + 1; m < literals_in_this_term.size(); m++)
								{
									int literal_pos1 = new_term11.find(literals_in_this_term[m]);
									int literal_pos2 = new_term12.find(literals_in_this_term[m]);
									if (-1 != literal_pos1 && (literal_pos1 != literal_pos) && (-1 != literal_pos2) && (literal_pos2 != literal_pos))
									{
										string new_term21, new_term22, new_term23, new_term24;
										new_term21 = new_term11;
										new_term22 = new_term11;
										new_term23 = new_term12;
										new_term24 = new_term12;
										new_term21.replace(new_term21.begin() + literal_pos1, new_term21.begin() + literal_pos1 + literals_in_this_term[m].size(), s21);
										new_term22.replace(new_term22.begin() + literal_pos1, new_term22.begin() + literal_pos1 + literals_in_this_term[m].size(), s22);
										new_term23.replace(new_term23.begin() + literal_pos2, new_term23.begin() + literal_pos2 + literals_in_this_term[m].size(), s21);
										new_term24.replace(new_term24.begin() + literal_pos2, new_term24.begin() + literal_pos2 + literals_in_this_term[m].size(), s22);

										//变体1
										string mutant = exp;
										mutant.replace(mutant.begin() + pos, mutant.begin() + pos + term.size(), new_term21);
										mutants.addMutant(mutant, "LRFxLRF");
										//变体2
										mutant = exp;
										mutant.replace(mutant.begin() + pos, mutant.begin() + pos + term.size(), new_term22);
										mutants.addMutant(mutant, "LRFxLRF");
										//变体3
										mutant = exp;
										mutant.replace(mutant.begin() + pos, mutant.begin() + pos + term.size(), new_term23);
										mutants.addMutant(mutant, "LRFxLRF");
										//变体4
										mutant = exp;
										mutant.replace(mutant.begin() + pos, mutant.begin() + pos + term.size(), new_term24);
										mutants.addMutant(mutant, "LRFxLRF");
									}
								}
							}
						}
					}
				}
			}
		}
	}
	return mutants;
}



//求子表达式，且子表达式个数大于2
vector<string> DnfMutant::subTermOver2Exp(string exp, vector<string>terms, int start, int end){
	vector<string> subExpArr;
	//Utility uti;
	//vector<string> terms = uti.extractDNFstyle(exp);
	//a+b+c+d
	int n = end - start + 1;//表达式项的个数
	int sum = 2;//子表达式项的个数

	//cout << "n: " << n << endl;

	for (int i = 0; i < n - 1; i++){ //子表达式中项的个数有n-1种情况

		if (n == sum){
			string subExp;
			for (int j = start; j <= end - sum + 2; j++){
				if (subExp.length() == 0){
					subExp = terms[j];
				}
				else{
					subExp = subExp + "+" + terms[j];
				}
			}
			subExpArr.push_back(subExp);
			return subExpArr;
		}

		for (int j = start; j < end - sum + 1; j++){
			string subExp;
			for (int k = 0; k < sum; k++){
				if (subExp.length() == 0){
					subExp = terms[j + k];
				}
				else{
					subExp = subExp + "+" + terms[j + k];
				}
			}
			subExpArr.push_back(subExp);
		}
		sum++;
	}
	return subExpArr;
}

//2——项————27个无序、4个有序
//   无序
Mutants DnfMutant::generateENFxENFCase1doublemutants(string exp, vector<string> oriTerms, vector<string> terms, string model){ //ENFxENF---(1)
	Mutants mutants(exp);

	if (terms.size() >= 4){//至少4个项
		for (int i = 1; i < terms.size() - 2; i++){	//分成两部分：subExp1, subExp2;

			//	cout << "sub1: " << 0 << " - " << i << endl;
			vector<string> subExp1 = subTermOver2Exp(exp, terms, 0, i);
			//	cout << "subExp1.size: " << subExp1.size() << endl;

			//	cout << "sub2: " << i + 1 << " - " << terms.size() - 1 << endl;

			vector<string> subExp2 = subTermOver2Exp(exp, terms, i + 1, terms.size() - 1);
		//	cout << "subExp2.size: " << subExp2.size() << endl;


			for (int j = 0; j < subExp1.size(); j++){
				string new_exp1, new_exp2;
				new_exp1 = "!(" + subExp1[j] + ")";
				for (int k = 0; k < subExp2.size(); k++){
					new_exp2 = "!(" + subExp2[k] + ")";
					mutants.addMutant(new_exp1, new_exp2, "ENFxENFCase1");

					//cout << "new_exp1: " << new_exp1 << "   new_exp2: " << new_exp2 << endl;
				}
			}
		}
	}
	return mutants;
}

//ENFxTNF---(1)——  1 <= i1 <h1 < i2 < h2 <= m 
Mutants DnfMutant::generateENFxTNFCase1doublemutants(string subexp1, string subexp2, string faultType, string model){
	Mutants mutants(model);
	string new_exp1, new_exp2;
	new_exp1 = "!(" + subexp1 + ")";
	new_exp2 = "!(" + subexp2 + ")";
	mutants.addMutant(new_exp1, new_exp2, "ENFxTNFCase1");
	return mutants;
}

//ENFxTNF---(2)——  1 <= i1 <= i2 < h2 <= h1 <= m 
Mutants DnfMutant::generateENFxTNFCase2doublemutants(string exp, int i1, int h1, int i2, int h2, string faultType, string model){
	Mutants mutants(exp);
	Utility uti;
	vector<string> terms = uti.extractDNFstyle(exp);
	string new_exp1, new_exp2;

	int start1, end1;
	int start2, end2;

	for (int i = i1 - 1; i < h1; i++) {
		if (new_exp1.length() != 0){
			new_exp1 = new_exp1 + "+" + terms[i];
		}
		else{
			new_exp1 = terms[i];
		}
	}
	start1 = exp.find(new_exp1);//subexp1开始的位置
	end1 = start1 + new_exp1.length();//subexp1结束的位置

	for (int i = i2 - 1; i < h2; i++) {
		if (new_exp2.length() != 0){
			new_exp2 = new_exp2 + "+" + terms[i];
		}
		else{
			new_exp2 = terms[i];
		}
	}
	start2 = exp.find(new_exp2);//subexp2开始的位置
	end2 = start2 + new_exp2.length();//subexp2结束的位置

	new_exp2 = "!(" + new_exp2 + ")";
	exp.replace(exp.begin() + start2, exp.begin() + end2, new_exp2);//替换，原表达式的的长度增加了3

	exp.insert(start1, "!(");//长度又增加2
	end1 = end1 + 5;
	exp.insert(end1, ")");

	mutants.addMutant(exp, "ENFxTNFCase2");
	return mutants;
}


//ENFxTNF---(3)——连着的一部分发生ENF，剩下的一个项发生TNF
Mutants DnfMutant::generateENFxTNFCase1doublemutants(string exp, vector<string> oriTerms, vector<string> faultterms, string model)
{
	Mutants mutants(exp);
	Utility uti;
	if (faultterms.size() >= 3){
		vector<info> terms;
		for (int i = 0; i < faultterms.size(); i++){
			info t;
			t.term = faultterms[i];
			t.pos = uti.getTermPosition(oriTerms, faultterms[i]);//获取当前项在表达式中的位置
			terms.push_back(t);
		}
		sort(terms.begin(), terms.end());//排序

		//-------------------------------------------------------------------------
		bool flag = true;
		for (int i = 0; i < terms.size() - 2; i++){//是否连续
			if (terms[i + 1].pos - terms[i].pos != 1){
				flag = false;
				break;
			}
		}
		//前n-1个发生ENF，最后一个发生TNF
		if (flag){
			//ENF
			int start = 0;
			for (int i = 0; i < oriTerms.size(); i++){
				if (terms[0].term == oriTerms[i]){
					start = i;
					break;
				}
			}
			string enfExpression = terms[start].term;
			for (int i = start + 1; i < start + faultterms.size() - 1; i++){
				enfExpression = enfExpression + "+" + terms[i].term;
			}
			string new_enfExpression = "!(" + enfExpression + ")";

			string mutant = exp;
			int pos = exp.find(enfExpression);
			mutant.replace(mutant.begin() + pos, mutant.begin() + pos + enfExpression.length(), new_enfExpression);

			//TNF
			string corfTerm = terms[faultterms.size() - 1].term;
			int p2 = mutant.find(corfTerm);
			string new_term = "!(" + corfTerm + ")";
			mutant.replace(mutant.begin() + p2, mutant.begin() + p2 + corfTerm.length(), new_term);
			mutants.addMutant(mutant, "ENFxTNFCase1");
		}

		//------------------------------------------------------
		flag = true;
		for (int i = 1; i < faultterms.size() - 1; i++){//判断是否连续
			if (terms[i + 1].pos - terms[i].pos != 1){
				flag = false;
				return mutants;
			}
		}
		//第1个发生TNF，后边的发生ENF
		if (flag){
			//TNF
			string corfTerm = terms[0].term;
			string new_term = "!(" + corfTerm + ")";
			string mutant = exp;
			int p2 = mutant.find(corfTerm);
			mutant.replace(mutant.begin() + p2, mutant.begin() + p2 + corfTerm.length(), new_term);

			//ENF
			int start;
			for (int i = 0; i < oriTerms.size(); i++){
				if (terms[0].term == oriTerms[i]){
					start = i;
					break;
				}
			}
			string enfExpression = terms[start + 1].term;
			for (int i = start + 2; i < start + faultterms.size(); i++){
				enfExpression = enfExpression + "+" + terms[i].term;
			}
			string new_enfExpression = "!(" + enfExpression + ")";
			int pos = mutant.find(enfExpression);
			mutant.replace(mutant.begin() + pos, mutant.begin() + pos + enfExpression.length(), new_enfExpression);
			mutants.addMutant(mutant, "ENFxTOFCase1");
		}
	}
	return mutants;
}


//ENFxTOF---(5)——连着的一部分发生ENF，剩下的一个项发生TOF
Mutants DnfMutant::generateENFxTOFCase1doublemutants(string exp, vector<string> oriTerms, vector<string> faultterms, string model)
{
	Mutants mutants(exp);
	Utility uti;
	if (faultterms.size() >= 3){
		vector<info> terms;
		for (int i = 0; i < faultterms.size(); i++){
			info t;
			t.term = faultterms[i];
			t.pos = uti.getTermPosition(oriTerms, faultterms[i]);//获取当前项在表达式中的位置
			terms.push_back(t);
		}
		sort(terms.begin(), terms.end());//排序

		//-------------------------------------------------------------------------
		bool flag = true;
		for (int i = 0; i < terms.size() - 2; i++){//是否连续
			if (terms[i + 1].pos - terms[i].pos != 1){
				flag = false;
				break;
			}
		}
		//前n-1个发生ENF，最后一个发生TOF
		if (flag){
			//ENF
			int start = 0;
			for (int i = 0; i < oriTerms.size(); i++){
				if (terms[0].term == oriTerms[i]){
					start = i;
					break;
				}
			}
			string enfExpression = terms[start].term;
			for (int i = start + 1; i < start + faultterms.size() - 1; i++){
				enfExpression = enfExpression + "+" + terms[i].term;
			}
			string new_enfExpression = "!(" + enfExpression + ")";

			string mutant = exp;
			int pos = exp.find(enfExpression);
			mutant.replace(mutant.begin() + pos, mutant.begin() + pos + enfExpression.length(), new_enfExpression);

			//TOF
			string corfTerm = terms[faultterms.size() - 1].term;

			int p2 = mutant.find(corfTerm);
			mutant.replace(mutant.begin() + p2 - 1, mutant.begin() + p2 + corfTerm.length(), "");
			mutants.addMutant(mutant, "ENFxTOFCase1");
		}

		//------------------------------------------------------
		flag = true;
		for (int i = 1; i < faultterms.size() - 1; i++){//判断是否连续
			if (terms[i + 1].pos - terms[i].pos != 1){
				flag = false;
				return mutants;
			}
		}
		//第1个发生TOF，后边的发生ENF
		if (flag){
			//TOF
			string corfTerm = terms[0].term;

			string mutant = exp;
			int p2 = mutant.find(corfTerm);
			mutant.replace(mutant.begin() + p2, mutant.begin() + p2 + corfTerm.length() + 1, "");

			//ENF
			int start;
			for (int i = 0; i < oriTerms.size(); i++){
				if (terms[0].term == oriTerms[i]){
					start = i;
					break;
				}
			}
			string enfExpression = terms[start + 1].term;
			for (int i = start + 2; i < start + faultterms.size(); i++){
				enfExpression = enfExpression + "+" + terms[i].term;
			}
			string new_enfExpression = "!(" + enfExpression + ")";
			int pos = mutant.find(enfExpression);
			mutant.replace(mutant.begin() + pos, mutant.begin() + pos + enfExpression.length(), new_enfExpression);
			mutants.addMutant(mutant, "ENFxTOFCase1");
		}
	}
	return mutants;
}

//ENFxDORF---(7)——连着的一部分发生ENF，剩下的两个项如果连着的话发生DORF
Mutants DnfMutant::generateENFxDORFCase1doublemutants(string exp, vector<string> oriTerms, vector<string> faultterms, string model)
{
	Mutants mutants(exp);
	Utility uti;
	if (faultterms.size() >= 4){
		vector<info> terms;
		for (int i = 0; i < faultterms.size(); i++){
			info t;
			t.term = faultterms[i];
			t.pos = uti.getTermPosition(oriTerms, faultterms[i]);//获取当前项在表达式中的位置
			terms.push_back(t);
		}
		sort(terms.begin(), terms.end());//排序

		//-------------------------------------------------------------------------
		bool flag = true;
		for (int i = 0; i < terms.size() - 3; i++){//前n-2个项是否连续
			if (terms[i + 1].pos - terms[i].pos != 1){
				flag = false;
				break;
			}
		}
		if (terms[terms.size() - 1].pos - terms[terms.size() - 2].pos != 1){//后2个项是否连续
			flag = false;
		}

		//前n-2个发生ENF，最后两个发生DORF
		if (flag){
			//ENF
			int start = 0;
			for (int i = 0; i < oriTerms.size(); i++){
				if (terms[0].term == oriTerms[i]){
					start = i;
					break;
				}
			}
			string enfExpression = terms[start].term;
			for (int i = start + 1; i < start + faultterms.size() - 2; i++){
				enfExpression = enfExpression + "+" + terms[i].term;
			}
			string new_enfExpression = "!(" + enfExpression + ")";

			string mutant = exp;
			int pos = exp.find(enfExpression);
			mutant.replace(mutant.begin() + pos, mutant.begin() + pos + enfExpression.length(), new_enfExpression);

			//DORF
			string dorfTerm = terms[faultterms.size() - 2].term + "+" + terms[faultterms.size() - 1].term;
			int p2 = mutant.find(dorfTerm);
			string new_term = terms[faultterms.size() - 2].term + terms[faultterms.size() - 1].term;
			mutant.replace(mutant.begin() + p2, mutant.begin() + p2 + dorfTerm.length(), new_term);
			mutants.addMutant(mutant, "ENFxDORFCase1");
		}

		//------------------------------------------------------
		flag = true;
		if (terms[1].pos - terms[0].pos != 1){
			flag = false;
			return mutants;
		}
		for (int i = 2; i < faultterms.size() - 1; i++){//后n-2个项是否连续
			if (terms[i + 1].pos - terms[i].pos != 1){
				flag = false;
				return mutants;
			}
		}

		//前两个项发生DORF，后边的发生ENF
		if (flag){
			//DORF
			string dorfTerm = terms[0].term + "+" + terms[1].term;
			string new_term = terms[0].term + terms[1].term;
			string mutant = exp;
			int p2 = mutant.find(dorfTerm);
			mutant.replace(mutant.begin() + p2, mutant.begin() + p2 + dorfTerm.length(), new_term);

			//ENF
			int start;
			for (int i = 0; i < oriTerms.size(); i++){
				if (terms[0].term == oriTerms[i]){
					start = i;
					break;
				}
			}
			string enfExpression = terms[start + 2].term;
			for (int i = start + 3; i < start + faultterms.size(); i++){
				enfExpression = enfExpression + "+" + terms[i].term;
			}
			string new_enfExpression = "!(" + enfExpression + ")";
			int pos = mutant.find(enfExpression);
			mutant.replace(mutant.begin() + pos, mutant.begin() + pos + enfExpression.length(), new_enfExpression);
			mutants.addMutant(mutant, "ENFxDORFCase1");
		}
	}
	return mutants;
}


//ENFxCORF---(11)——连着的一部分发生ENF，剩下的一个项发生CORF
Mutants DnfMutant::generateENFxCORFCase1doublemutants(string exp, vector<string> oriTerms, vector<string> faultterms, string model)
{
	Mutants mutants(exp);
	Utility uti;
	if (faultterms.size() >= 3){
		vector<info> terms;
		for (int i = 0; i < faultterms.size(); i++){
			info t;
			t.term = faultterms[i];
			t.pos = uti.getTermPosition(oriTerms, faultterms[i]);//获取当前项在表达式中的位置
			terms.push_back(t);
		}
		sort(terms.begin(), terms.end());//排序

		bool flag = true;
		for (int i = 0; i < terms.size() - 2; i++){//是否连续
			if (terms[i + 1].pos - terms[i].pos != 1){
				flag = false;
				break;
			}
		}
		//前n-1个发生ENF，最后一个发生CORF
		if (flag){
			//ENF
			int start = 0;
			for (int i = 0; i < oriTerms.size(); i++){
				if (terms[0].term == oriTerms[i]){
					start = i;
					break;
				}
			}
			string enfExpression = terms[start].term;
			for (int i = start + 1; i < start + faultterms.size() - 1; i++){
				enfExpression = enfExpression + "+" + terms[i].term;
			}
			string new_enfExpression = "!(" + enfExpression + ")";

			string mutant = exp;
			int pos = exp.find(enfExpression);
			mutant.replace(mutant.begin() + pos, mutant.begin() + pos + enfExpression.length(), new_enfExpression);

			//CORF
			string corfTerm = terms[faultterms.size() - 1].term;
			vector<string> literals_in_this_term = uti.extractCNFstyle(corfTerm);
			for (int j = 1; j < literals_in_this_term.size(); j++)
			{
				string mutant2 = mutant;
				int pos_insert_operator_or = corfTerm.find(literals_in_this_term[j]);
				string new_term1 = corfTerm;
				new_term1.insert(pos_insert_operator_or, "+");

				int p2 = mutant2.find(corfTerm);
				mutant2.replace(mutant2.begin() + p2, mutant2.begin() + p2 + corfTerm.length(), new_term1);

				mutants.addMutant(mutant2, "ENFxCORFCase1");
			}

		}
		//------------------------------------------------------
		flag = true;
		for (int i = 1; i < faultterms.size() - 1; i++){//判断是否连续
			if (terms[i + 1].pos - terms[i].pos != 1){
				flag = false;
				return mutants;
			}
		}
		//第1个发生CORF，后边的发生ENF
		if (flag){
			//CORF
			string corfTerm = terms[0].term;
			vector<string> literals_in_this_term = uti.extractCNFstyle(corfTerm);
			for (int j = 1; j < literals_in_this_term.size(); j++)
			{
				string mutant = exp;
				int pos_insert_operator_or = corfTerm.find(literals_in_this_term[j]);
				string new_term1 = corfTerm;
				new_term1.insert(pos_insert_operator_or, "+");

				int p2 = mutant.find(corfTerm);
				mutant.replace(mutant.begin() + p2, mutant.begin() + p2 + corfTerm.length(), new_term1);

				//ENF
				int start;
				for (int i = 0; i < oriTerms.size(); i++){
					if (terms[0].term == oriTerms[i]){
						start = i;
						break;
					}
				}
				string enfExpression = terms[start + 1].term;
				for (int i = start + 2; i < start + faultterms.size(); i++){
					enfExpression = enfExpression + "+" + terms[i].term;
				}
				string new_enfExpression = "!(" + enfExpression + ")";
				int pos = mutant.find(enfExpression);
				mutant.replace(mutant.begin() + pos, mutant.begin() + pos + enfExpression.length(), new_enfExpression);
				mutants.addMutant(mutant, "ENFxCORFCase1");
			}
		}
	}
	return mutants;
}

//TNFxTNF---(13)
Mutants DnfMutant::generateTNFxTNFdoublemutants(string exp, vector<string> oriTerms, vector<string> terms, string model){
	Mutants mutants(exp);
	Utility uti;
	for (int m = 0; m < terms.size() - 1; m++)
	{
		for (int n = m + 1; n < terms.size(); n++)
		{
			string term1 = terms[m];
			string term2 = terms[n];

			string new_term1, new_term2;
			new_term1 = "!(" + term1 + ")";
			new_term2 = "!(" + term2 + ")";
			string mutant = uti.replaceDoubleTerms(exp, oriTerms, m, n, new_term1, new_term2);
			mutants.addMutant(mutant, "TNFxTNF");

		}
	}
	return mutants;
}

//TNFxTOF---(14)
Mutants DnfMutant::generateTNFxTOFdoublemutants(string exp, vector<string> oriTerms, vector<string> terms, string model){
	Mutants mutants(exp);
	Utility uti;

	for (int m = 0; m < terms.size() - 1; m++){
		for (int n = m + 1; n < terms.size(); n++){
			string term1 = terms[m];
			string term2 = terms[n];

			string new_term1, new_term2;
			new_term1 = "!(" + term1 + ")";
			string mutant = exp;
			int pos = mutant.find(term1);
			mutant.replace(mutant.begin() + pos, mutant.begin() + pos + term1.length(), new_term1);
			pos = mutant.find(term2);
			int termPosition = uti.getTermPosition(oriTerms, term2);
			if (termPosition == 1){
				mutant.replace(mutant.begin() + pos, mutant.begin() + pos + term2.length() + 1, "");
			}
			else{
				mutant.replace(mutant.begin() + pos - 1, mutant.begin() + pos + term2.length(), "");
			}
			mutants.addMutant(mutant, "TNFxTOF");

			//-------------------------------------------
			string mutant2 = exp;
			int pos2 = mutant2.find(term2);
			new_term2 = "!(" + term2 + ")";
			mutant2.replace(mutant2.begin() + pos2, mutant2.begin() + pos2 + term2.length(), new_term2);

			pos2 = mutant2.find(term1);
			int termPosition2 = uti.getTermPosition(oriTerms, term1);
			if (termPosition2 == 1){
				mutant2.replace(mutant2.begin() + pos2, mutant2.begin() + pos2 + term1.length() + 1, "");
			}
			else{
				mutant2.replace(mutant2.begin() + pos2 - 1, mutant2.begin() + pos2 + term1.length(), "");
			}
			mutants.addMutant(mutant2, "TNFxTOF");
		}
	}
	return mutants;
}



//TNFxDORF---(15) ——3个项，两个相邻的项发生DORF，剩下的项发生TNF.有两个版本的实现，第一个由ZHANG XIAOXIONG实现

Mutants DnfMutant::generateTNFxDORFCase1doublemutantsZXX(string exp, vector<string> oriTerms, vector<string> faultterms, string model){
	Mutants mutants(exp);
	Utility uti;
	if (faultterms.size() == 3){
		//term1,term2,term3在原表达式中项的位置
		info terms[3];
		for (int i = 0; i < faultterms.size(); i++){
			terms[i].term = faultterms[i];
		}

		for (int i = 0; i < faultterms.size(); i++){
			terms[i].pos = uti.getTermPosition(oriTerms, terms[i].term);
		}
		sort(terms, terms + 3);

		if (terms[1].pos - terms[0].pos == 1){
			//term1与term2相邻，发生DORF
			string new_term1 = terms[0].term + terms[1].term;//DORF
			string new_term2 = "!(" + terms[2].term + ")";//TNF

			//生成新的表达式
			string mutant = exp;
			int pos = exp.find(terms[0].term);
			mutant.replace(mutant.begin() + pos, mutant.begin() + pos + terms[0].term.length() + terms[1].term.length() + 1, new_term1);
			pos = mutant.rfind(terms[2].term);
			mutant.replace(mutant.begin() + pos, mutant.begin() + pos + terms[2].term.length(), new_term2);
			mutants.addMutant(mutant, "TNFxDORFCase1");

		}
	}
	return mutants;
}




Mutants DnfMutant::generateTNFxDORFCase1doublemutants(string exp, vector<string> oriTerms, vector<string> faultterms, string model){
	Mutants mutants(exp);
	Utility uti;
	if (faultterms.size() == 3){
		//term1,term2,term3在原表达式中项的位置
		info terms[3];
		for (int i = 0; i < faultterms.size(); i++){
			terms[i].term = faultterms[i];
		}

		for (int i = 0; i < faultterms.size(); i++){
			terms[i].pos = uti.getTermPosition(oriTerms, terms[i].term);
		}
		sort(terms, terms + 3);

		if (terms[1].pos - terms[0].pos == 1){
			//term1与term2相邻，发生DORF
			string new_term1 = terms[0].term + terms[1].term;//DORF
			string new_term2 = "!(" + terms[2].term + ")";//TNF

			//生成新的表达式
			string mutant = exp;
			int pos = exp.find(terms[0].term);
			mutant.replace(mutant.begin() + pos, mutant.begin() + pos + terms[0].term.length() + terms[1].term.length() + 1, new_term1);
			pos = mutant.rfind(terms[2].term);
			mutant.replace(mutant.begin() + pos, mutant.begin() + pos + terms[2].term.length(), new_term2);
			mutants.addMutant(mutant, "TNFxDORFCase1");

		}

		if (terms[2].pos - terms[1].pos == 1){//term2与term3相邻
			string new_term1 = "!(" + terms[0].term + ")";//TNF
			string new_term2 = terms[1].term + terms[2].term;//DORF

			//生成新的表达式
			string mutant = exp;
			int pos = exp.find(terms[0].term);
			mutant.replace(mutant.begin() + pos, mutant.begin() + pos + terms[0].term.length(), new_term1);

			pos = mutant.rfind(terms[1].term);
			mutant.replace(mutant.begin() + pos, mutant.begin() + pos + terms[1].term.length() + terms[2].term.length() + 1, new_term2);

			mutants.addMutant(mutant, "TNFxDORFCase1");
		}
	}
	return mutants;
}

//TNFxDORF---(16)——term1，term2中有一个取反，然后合并(term1与term2必须相邻)
Mutants DnfMutant::generateTNFxDORFCase2doublemutants(string exp, vector<string> oriTerms, vector<string> faultterms, string model){
	Mutants mutants(exp);
	Utility uti;

	if (faultterms.size() == 2){
		//term1,term2在原表达式中项的位置
		info terms[2];
		for (int i = 0; i < faultterms.size(); i++){
			terms[i].term = faultterms[i];
		}
		for (int i = 0; i < 2; i++){
			terms[i].pos = uti.getTermPosition(oriTerms, terms[i].term);
		}
		sort(terms, terms + 2);

		if (terms[1].pos - terms[0].pos == 1){
			//第1个项TNF，第2个项发生DORF
			string new_term1, new_term2;
			new_term1 = "!(" + terms[0].term + ")";
			new_term2 = new_term1 + terms[1].term;

			string mutant = exp;
			int pos = exp.find(terms[0].term);
			mutant.replace(mutant.begin() + pos, mutant.begin() + pos + terms[0].term.length() + terms[1].term.length() + 1, new_term2);
			mutants.addMutant(mutant, "TNFxDORFCase2");

			//第2个项TNF，第1个项发生DORF
			new_term1 = "!(" + terms[1].term + ")";
			new_term2 = terms[0].term + new_term1;

			mutant = exp;
			pos = exp.find(terms[0].term);
			mutant.replace(mutant.begin() + pos, mutant.begin() + pos + terms[0].term.length() + terms[1].term.length() + 1, new_term2);
			mutants.addMutant(mutant, "TNFxDORFCase2");
		}
	}
	return mutants;
}


//TNFxCORF---(17)——一个项取反，另一个项发生CORF
Mutants DnfMutant::generateTNFxCORFCase1doublemutants(string exp, vector<string> oriTerms, vector<string> faultterms, string model){
	Mutants mutants(exp);
	Utility uti;

	if (faultterms.size() == 2){
		string term1 = faultterms[0];
		string term2 = faultterms[1];

		string new_term1, new_term2;

		//第一个项TNF，第二个项CORF
		new_term1 = "!(" + term1 + ")";

		string mutant = exp;
		int pos = exp.find(term1);
		mutant.replace(mutant.begin() + pos, mutant.begin() + pos + term1.length(), new_term1);

		vector<string> literals_in_this_term = uti.extractCNFstyle(term2);
		for (int j = 1; j < literals_in_this_term.size(); j++)
		{
			int pos_insert_operator_or = term2.find(literals_in_this_term[j]);
			new_term2 = term2;
			new_term2.insert(pos_insert_operator_or, "+");

			pos = mutant.find(term2);
			mutant.replace(mutant.begin() + pos, mutant.begin() + pos + term2.length(), new_term2);
			mutants.addMutant(mutant, "TNFxCORFCase1");
		}

		//第二个项TNF，第一个项CORF
		new_term2 = "!(" + term2 + ")";
		mutant = exp;
		pos = exp.find(term2);
		mutant.replace(mutant.begin() + pos, mutant.begin() + pos + term2.length(), new_term2);

		literals_in_this_term = uti.extractCNFstyle(term1);
		for (int j = 1; j < literals_in_this_term.size(); j++)
		{
			int pos_insert_operator_or = term1.find(literals_in_this_term[j]);
			new_term1 = term1;
			new_term1.insert(pos_insert_operator_or, "+");

			pos = mutant.find(term1);
			mutant.replace(mutant.begin() + pos, mutant.begin() + pos + term1.length(), new_term1);
			mutants.addMutant(mutant, "TNFxCORFCase1");
		}
	}
	return mutants;
}

//TNFxCORF---(18)——同一个项：先发生CORF，再TNF
Mutants DnfMutant::generateTNFxCORFCase2doublemutants(string exp, vector<string> oriTerms, vector<string> terms, string model){
	Mutants mutants(exp);
	Utility uti;
	if (terms.size() == 1){
		string term = terms[0];
		string new_term1;
		string new_term2;
		Utility uti;
		vector<string> literals_in_this_term = uti.extractCNFstyle(term);
		for (int j = 1; j < literals_in_this_term.size(); j++)
		{
			//CORF
			int pos_insert_operator_or = term.find(literals_in_this_term[j]);
			new_term1 = term;
			new_term1.insert(pos_insert_operator_or, "+");
			//LNF
			new_term2 = "!(" + new_term1 + ")";

			string mutant = exp;
			int pos = mutant.find(term);
			mutant.replace(mutant.begin() + pos, mutant.begin() + pos + term.length(), new_term2);
			mutants.addMutant(mutant, "TNFxCORFCase2");

		}
	}
	return mutants;
}

//TOFxTOF---(19) 有两个版本，第一个版本由ZHANG XIAOXIONG实现
Mutants DnfMutant::generateTOFxTOFdoublemutants(string exp, vector<string> oriTerms, vector<string> terms){
	Mutants mutants(exp);
	Utility uti;
	string mutant="";
	if (terms.size() == 2){
		vector<string> temp;
		for (int i = 0; i < terms.size(); i++){
			temp.clear();
			for (int j = 0; j < oriTerms.size(); j++){
				if (terms[i] != oriTerms[j])
					temp.push_back(oriTerms[j]);
			}

			oriTerms = temp;
		}

		for (int i = 0; i < temp.size(); i++){
			if (i != temp.size()-1)
				mutant = mutant + temp[i] + "+";
			else
				mutant = mutant + temp[i];

		}

		mutants.addMutant(mutant, "TOFxTOF");
	}
	
	return mutants;

}

Mutants DnfMutant::generateTOFxTOFdoublemutants(string exp, vector<string> oriTerms, vector<string> terms, string model){
	Mutants mutants(exp);
	Utility uti;
	if (terms.size() == 2){
		string term1 = terms[0];
		string term2 = terms[1];
		int p1 = uti.getTermPosition(oriTerms, term1);
		int p2 = uti.getTermPosition(oriTerms, term2);


		string mutant = exp;
		int pos = mutant.find(term1);
		if (p1 == oriTerms.size()){
			mutant.replace(mutant.begin() + pos - 1, mutant.begin() + pos + term1.length(), "");//最后一个项，要特殊处理
		}
		else{
			mutant.replace(mutant.begin() + pos, mutant.begin() + pos + term1.length() + 1, "");
		}

		pos = mutant.find(term2);
		if (p2 == oriTerms.size() - 1){
			mutant.replace(mutant.begin() + pos - 1, mutant.begin() + pos + term2.length(), "");
		}
		else{
			mutant.replace(mutant.begin() + pos, mutant.begin() + pos + term2.length() + 1, "");
		}

		mutants.addMutant(mutant, "TOFxTOF");
	}
	return mutants;
}

//TOFxDORF---(20)——三个项，相邻的两个项发生DORF， 第3个项发生TOF.有两个版本的实现，第一个由ZHANG XIAOXIONG实现

Mutants DnfMutant::generateTOFxDORFdoublemutantsZXX(string exp, vector<string> oriTerms, vector<string> faultterms, string model){
	Mutants mutants(exp);
	Utility uti;

	if (faultterms.size() == 3){
		//term1,term2在原表达式中项的位置
		info terms[3];
		for (int i = 0; i < faultterms.size(); i++){
			terms[i].term = faultterms[i];
		}
		for (int i = 0; i < faultterms.size(); i++){
			terms[i].pos = uti.getTermPosition(oriTerms, terms[i].term);
		}
		sort(terms, terms + 3);

		if (terms[1].pos - terms[0].pos == 1)//term1与term2相邻
		{
			//term1与term2发生DORF
			string new_term1 = terms[0].term + terms[1].term;
			string mutant = exp;
			int pos = exp.find(terms[0].term);
			mutant.replace(mutant.begin() + pos, mutant.begin() + pos + terms[0].term.length() + terms[1].term.length() + 1, new_term1);
			//term3发生TOF
			int p = uti.getTermPosition(oriTerms, terms[2].term);
			int pos2 = mutant.find(terms[2].term);

			if (p == oriTerms.size()){
				mutant.replace(mutant.begin() + pos2 - 1, mutant.begin() + pos2 + terms[2].term.length(), "");
			}
			else{
				mutant.replace(mutant.begin() + pos2, mutant.begin() + pos2 + terms[2].term.length() + 1, "");
			}
			mutants.addMutant(mutant, "TOFxDORF");
		}
	}
	return mutants;
}



Mutants DnfMutant::generateTOFxDORFdoublemutants(string exp, vector<string> oriTerms, vector<string> faultterms, string model){
	Mutants mutants(exp);
	Utility uti;

	if (faultterms.size() == 3){
		//term1,term2在原表达式中项的位置
		info terms[3];
		for (int i = 0; i < faultterms.size(); i++){
			terms[i].term = faultterms[i];
		}
		for (int i = 0; i < faultterms.size(); i++){
			terms[i].pos = uti.getTermPosition(oriTerms, terms[i].term);
		}
		sort(terms, terms + 3);

		if (terms[1].pos - terms[0].pos == 1)//term1与term2相邻
		{
			//term1与term2发生DORF
			string new_term1 = terms[0].term + terms[1].term;
			string mutant = exp;
			int pos = exp.find(terms[0].term);
			mutant.replace(mutant.begin() + pos, mutant.begin() + pos + terms[0].term.length() + terms[1].term.length() + 1, new_term1);
			//term3发生TOF
			int p = uti.getTermPosition(oriTerms, terms[2].term);
			int pos2 = mutant.find(terms[2].term);

			if (p == oriTerms.size()){
				mutant.replace(mutant.begin() + pos2 - 1, mutant.begin() + pos2 + terms[2].term.length(), "");
			}
			else{
				mutant.replace(mutant.begin() + pos2, mutant.begin() + pos2 + terms[2].term.length() + 1, "");
			}
			mutants.addMutant(mutant, "TOFxDORF");
		}

		if (terms[2].pos - terms[1].pos == 1)//term3与term2相邻
		{
			//term3与term2发生DORF
			string new_term1 = terms[1].term + terms[2].term;
			string mutant = exp;
			int pos = exp.find(terms[1].term);
			mutant.replace(mutant.begin() + pos, mutant.begin() + pos + terms[1].term.length() + terms[2].term.length() + 1, new_term1);
			//term1发生TOF
			int p = uti.getTermPosition(oriTerms, terms[0].term);
			int pos2 = mutant.find(terms[0].term);
			if (p == oriTerms.size()){
				mutant.replace(mutant.begin() + pos2 - 1, mutant.begin() + pos2 + terms[0].term.length(), "");
			}
			else{
				mutant.replace(mutant.begin() + pos2, mutant.begin() + pos2 + terms[0].term.length() + 1, "");
			}
			mutants.addMutant(mutant, "TOFxDORF");
		}
	}
	return mutants;
}

//TOFxCORF---(21)——两个项，一个发生CORF，一个发生TOF
Mutants DnfMutant::generateTOFxCORFdoublemutants(string exp, vector<string> oriTerms, vector<string> terms, string model){
	Mutants mutants(exp);
	Utility uti;

	string term1 = terms[0];
	string term2 = terms[1];

	//第一个项TOF，第二个项CORF
	int termPos = uti.getTermPosition(oriTerms, term1);
	int p1 = exp.find(term1);
	string mutant = exp;
	if (termPos == oriTerms.size()){
		mutant.replace(mutant.begin() + p1 - 1, mutant.begin() + p1 + term1.length(), "");//最后一个项
	}
	else{
		mutant.replace(mutant.begin() + p1, mutant.begin() + p1 + term1.length() + 1, "");
	}

	vector<string> literals_in_this_term = uti.extractCNFstyle(term2);
	for (int j = 1; j < literals_in_this_term.size(); j++)
	{
		int pos_insert_operator_or = term2.find(literals_in_this_term[j]);
		string new_term1 = term2;
		new_term1.insert(pos_insert_operator_or, "+");

		int p2 = mutant.find(term2);
		mutant.replace(mutant.begin() + p2, mutant.begin() + p2 + term2.length(), new_term1);
		mutants.addMutant(mutant, "TOFxCORF");
	}


	//第二个项TOF，第一个项CORF
	termPos = uti.getTermPosition(oriTerms, term2);
	p1 = exp.find(term2);
	mutant = exp;
	if (termPos == oriTerms.size()){
		mutant.replace(mutant.begin() + p1 - 1, mutant.begin() + p1 + term2.length(), "");//最后一个项
	}
	else{
		mutant.replace(mutant.begin() + p1, mutant.begin() + p1 + term2.length() + 1, "");
	}

	literals_in_this_term = uti.extractCNFstyle(term1);
	for (int j = 1; j < literals_in_this_term.size(); j++)
	{
		int pos_insert_operator_or = term1.find(literals_in_this_term[j]);
		string new_term1 = term1;
		new_term1.insert(pos_insert_operator_or, "+");

		int p2 = mutant.find(term1);
		mutant.replace(mutant.begin() + p2, mutant.begin() + p2 + term2.length(), new_term1);
		mutants.addMutant(mutant, "TOFxCORF");
	}
	return mutants;
}

//DORFxDORF---(22)——四个项，1,2相邻，3,4相邻
Mutants DnfMutant::generateDORFxDORFCase1doublemutants(string exp, vector<string> oriTerms, vector<string> faultterms, string model){
	Mutants mutants(exp);
	Utility uti;
	if (faultterms.size() == 4){
		//term1,term2,term3,term4在原表达式中项的位置
		info terms[4];
		for (int i = 0; i < faultterms.size(); i++){
			terms[i].term = faultterms[i];
		}

		for (int i = 0; i < faultterms.size(); i++){
			terms[i].pos = uti.getTermPosition(oriTerms, terms[i].term);
		}
		sort(terms, terms + 4);

		if (terms[1].pos - terms[0].pos == 1 && terms[3].pos - terms[2].pos == 1){//term1与term2相邻，term3与term4相邻
			string new_term1 = terms[0].term + terms[1].term;//DORF

			string mutant = exp;
			int pos = exp.find(terms[0].term);
			mutant.replace(mutant.begin() + pos, mutant.begin() + pos + terms[0].term.length() + terms[1].term.length() + 1, new_term1);

			string new_term2 = terms[2].term + terms[3].term;//DORF
			pos = mutant.find(terms[2].term);
			mutant.replace(mutant.begin() + pos, mutant.begin() + pos + terms[2].term.length() + terms[3].term.length() + 1, new_term2);
			mutants.addMutant(mutant, "DORFxDORFCase1");
		}
	}
	return mutants;
}

//DORFxDORF---(23)——三个项，1,2,3相邻
Mutants DnfMutant::generateDORFxDORFCase2doublemutants(string exp, vector<string> oriTerms, vector<string> faultterms, string model){
	Mutants mutants(exp);
	Utility uti;
	if (faultterms.size() == 3){
		//term1,term2,term3在原表达式中项的位置
		info terms[3];
		for (int i = 0; i < faultterms.size(); i++){
			terms[i].term = faultterms[i];
		}

		for (int i = 0; i < faultterms.size(); i++){
			terms[i].pos = uti.getTermPosition(oriTerms, terms[i].term);
		}
		sort(terms, terms + 3);

		if (terms[1].pos - terms[0].pos == 1 && terms[2].pos - terms[1].pos == 1){//term1与term2相邻,term2与term1相邻
			string new_term1 = terms[0].term + terms[1].term + terms[2].term;//DORF
			string mutant = exp;
			int pos = exp.find(terms[0].term);
			mutant.replace(mutant.begin() + pos, mutant.begin() + pos + terms[0].term.length() + terms[1].term.length() + terms[2].term.length() + 2, new_term1);
			mutants.addMutant(mutant, "DORFxDORFCase2");
		}
	}
	return mutants;
}

//DORFxCORF---(24)——三个项，相邻的两个项发生DORF,另一个项发生CORF.又两个版本，第一个由ZHANG XIAOXIONG 实现
Mutants DnfMutant::generateDORFxCORFCase1doublemutantsZXX(string exp, vector<string> oriTerms, vector<string> faultterms, string model){
	Mutants mutants(exp);
	Utility uti;
	if (faultterms.size() == 3){
		//term1,term2,term3在原表达式中项的位置
		info terms[3];
		for (int i = 0; i < faultterms.size(); i++){
			terms[i].term = faultterms[i];
		}

		for (int i = 0; i < faultterms.size(); i++){
			terms[i].pos = uti.getTermPosition(oriTerms, terms[i].term);
		}
		sort(terms, terms + 3);

		if (terms[1].pos - terms[0].pos == 1){//term1与term2相邻
			//DORF
			string new_term1 = terms[0].term + terms[1].term;
			string mutant = exp;
			int pos = exp.find(terms[0].term);
			mutant.replace(mutant.begin() + pos, mutant.begin() + pos + terms[0].term.length() + terms[1].term.length() + 1, new_term1);

			//CORF
			vector<string> literals_in_this_term = uti.extractCNFstyle(terms[2].term);
			for (int j = 1; j < literals_in_this_term.size(); j++)
			{
				int pos_insert_operator_or = terms[2].term.find(literals_in_this_term[j]);
				string new_term2 = terms[2].term;
				new_term2.insert(pos_insert_operator_or, "+");
				pos = mutant.find(terms[2].term);
				mutant.replace(mutant.begin() + pos, mutant.begin() + pos + terms[2].term.length(), new_term2);
				mutants.addMutant(mutant, "DORFxCORF");
			}
		}
	}
	return mutants;
}


Mutants DnfMutant::generateDORFxCORFCase1doublemutants(string exp, vector<string> oriTerms, vector<string> faultterms, string model){
	Mutants mutants(exp);
	Utility uti;
	if (faultterms.size() == 3){
		//term1,term2,term3在原表达式中项的位置
		info terms[3];
		for (int i = 0; i < faultterms.size(); i++){
			terms[i].term = faultterms[i];
		}

		for (int i = 0; i < faultterms.size(); i++){
			terms[i].pos = uti.getTermPosition(oriTerms, terms[i].term);
		}
		sort(terms, terms + 3);

		if (terms[1].pos - terms[0].pos == 1){//term1与term2相邻
			//DORF
			string new_term1 = terms[0].term + terms[1].term;
			string mutant = exp;
			int pos = exp.find(terms[0].term);
			mutant.replace(mutant.begin() + pos, mutant.begin() + pos + terms[0].term.length() + terms[1].term.length() + 1, new_term1);

			//CORF
			vector<string> literals_in_this_term = uti.extractCNFstyle(terms[2].term);
			for (int j = 1; j < literals_in_this_term.size(); j++)
			{
				int pos_insert_operator_or = terms[2].term.find(literals_in_this_term[j]);
				string new_term2 = terms[2].term;
				new_term2.insert(pos_insert_operator_or, "+");
				pos = mutant.find(terms[2].term);
				mutant.replace(mutant.begin() + pos, mutant.begin() + pos + terms[2].term.length(), new_term2);
				mutants.addMutant(mutant, "DORFxCORFCase1");
			}
		}

		if (terms[2].pos - terms[1].pos == 1){//term2与term3相邻
			//DORF
			string new_term1 = terms[1].term + terms[2].term;
			string mutant = exp;
			int pos = exp.find(terms[1].term);
			mutant.replace(mutant.begin() + pos, mutant.begin() + pos + terms[1].term.length() + terms[2].term.length() + 1, new_term1);

			//CORF
			vector<string> literals_in_this_term = uti.extractCNFstyle(terms[0].term);
			for (int j = 1; j < literals_in_this_term.size(); j++)
			{
				int pos_insert_operator_or = terms[0].term.find(literals_in_this_term[j]);
				string new_term2 = terms[0].term;
				new_term2.insert(pos_insert_operator_or, "+");
				pos = mutant.find(terms[0].term);
				mutant.replace(mutant.begin() + pos, mutant.begin() + pos + terms[0].term.length(), new_term2);
				mutants.addMutant(mutant, "DORFxCORFCase1");
			}
		}
	}
	return mutants;
}

//DORFxCORF---(25)——两个项（必须相邻）, 一个项发生CORF,分开的后一半与另一个相邻的项发生DORF
Mutants DnfMutant::generateDORFxCORFCase2doublemutants(string exp, vector<string> oriTerms, vector<string> faultterms, string model){
	Mutants mutants(exp);
	Utility uti;
	if (faultterms.size() == 2){
		//term1,term2在原表达式中项的位置
		info terms[2];
		for (int i = 0; i < faultterms.size(); i++){
			terms[i].term = faultterms[i];
		}

		for (int i = 0; i < faultterms.size(); i++){
			terms[i].pos = uti.getTermPosition(oriTerms, terms[i].term);
		}
		sort(terms, terms + 2);

		if (terms[1].pos - terms[0].pos == 1)//term1与term2相邻
		{
			string new_term1, new_term2;
			//CORF
			vector<string> literals_in_this_term = uti.extractCNFstyle(terms[0].term);
			for (int j = 1; j < literals_in_this_term.size(); j++)
			{
				int pos_insert_operator_or = terms[0].term.find(literals_in_this_term[j]);
				new_term1 = terms[0].term.substr(0, pos_insert_operator_or);
				new_term2 = terms[0].term.substr(pos_insert_operator_or, terms[0].term.length());

				//DORF————与后一个项发生DORF
				new_term2 = new_term2 + terms[1].term;
				string new_term3 = new_term1 + "+" + new_term2;
				string mutant = exp;
				int pos = exp.find(terms[0].term);
				mutant.replace(mutant.begin() + pos, mutant.begin() + pos + terms[0].term.length() + terms[1].term.length() + 1, new_term3);
				mutants.addMutant(mutant, "DORFxCORFCase2");
			}
		}

		if (terms[1].pos - terms[0].pos == 1)//term1与term2相邻
		{
			string new_term1, new_term2;
			//CORF
			vector<string> literals_in_this_term = uti.extractCNFstyle(terms[1].term);
			for (int j = 1; j < literals_in_this_term.size(); j++)
			{
				int pos_insert_operator_or = terms[1].term.find(literals_in_this_term[j]);
				new_term1 = terms[1].term.substr(0, pos_insert_operator_or);
				new_term2 = terms[1].term.substr(pos_insert_operator_or, terms[1].term.length());

				//DORF————与前一个项发生DORF
				new_term1 = terms[0].term + new_term1;
				string new_term3 = new_term1 + "+" + new_term2;
				string mutant = exp;
				int pos = exp.find(terms[0].term);
				mutant.replace(mutant.begin() + pos, mutant.begin() + pos + terms[0].term.length() + terms[1].term.length() + 1, new_term3);
				mutants.addMutant(mutant, "DORFxCORFCase2");
			}
		}
	}
	return mutants;
}

//CORFxCORF---(26)——两个项
Mutants DnfMutant::generateCORFxCORFCase1doublemutants(string exp, vector<string> oriTerms, vector<string> terms, string model){
	Mutants mutants(exp);
	Utility uti;
	if (terms.size() == 2){
		string term1 = terms[0];
		string term2 = terms[1];
		string new_term1, new_term2;

		vector<string> literals_in_term_1 = uti.extractCNFstyle(term1);
		vector<string> literals_in_term_2 = uti.extractCNFstyle(term2);

		for (int j = 1; j < literals_in_term_1.size(); j++)
		{
			int pos_insert_operator_or_1 = term1.find(literals_in_term_1[j]);
			new_term1 = term1;
			new_term1.insert(pos_insert_operator_or_1, "+");

			string mutant = exp;
			int pos = mutant.find(term1);
			mutant.replace(mutant.begin() + pos, mutant.begin() + pos + term1.length(), new_term1);

			int pos2 = mutant.find(term2);
			for (int i = 1; i < literals_in_term_2.size(); i++)
			{
				int pos_insert_operator_or_2 = term2.find(literals_in_term_2[i]);
				new_term2 = term2;
				new_term2.insert(pos_insert_operator_or_2, "+");
				mutant.replace(mutant.begin() + pos2, mutant.begin() + pos2 + term2.length(), new_term2);
				mutants.addMutant(mutant, "CORFxCORFCase1");
			}
		}
	}
	return mutants;
}

//CORFxCORF---(27)——一个项，term分成3个项
Mutants DnfMutant::generateCORFxCORFCase2doublemutants(string exp, vector<string> oriTerms, vector<string> terms, string model){
	Mutants mutants(exp);
	Utility uti;
	if (terms.size() == 1){
		string term = terms[0];

		string new_term1, new_term2;
		//CORF
		vector<string> literals_in_this_term = uti.extractCNFstyle(term);
		for (int j = 1; j < literals_in_this_term.size() - 1; j++)
		{
			int pos_insert_operator_or = term.find(literals_in_this_term[j]);
			new_term1 = term.substr(0, pos_insert_operator_or);
			new_term2 = term.substr(pos_insert_operator_or, term.length());//CORF,拆分成两个项

			//CORF
			vector<string> literals_in_this_term_2 = uti.extractCNFstyle(new_term2);
			for (int i = 1; i < literals_in_this_term_2.size(); i++){
				int pos_insert_operator_or_2 = new_term2.find(literals_in_this_term_2[i]);
				string new_term3 = new_term2;
				new_term3.insert(pos_insert_operator_or_2, "+");
				string new_term4 = new_term1 + "+" + new_term3;
				string mutant = exp;
				int pos = exp.find(term);
				mutant.replace(mutant.begin() + pos, mutant.begin() + pos + term.length(), new_term4);
				mutants.addMutant(mutant, "CORFxCORFCase2");
			}
		}
	}
	return mutants;
}

//TOFxDORF---(53)——三个项（相邻），term2遗漏，term1和term3合并
Mutants DnfMutant::generateTOFxDORFCase2doublemutants(string exp, vector<string> oriTerms, vector<string> faultterms, string model){
	Mutants mutants(exp);
	Utility uti;
	if (faultterms.size() == 3){
		//term1,term2,term3在原表达式中项的位置
		info terms[3];
		for (int i = 0; i < faultterms.size(); i++){
			terms[i].term = faultterms[i];
		}

		for (int i = 0; i < faultterms.size(); i++){
			terms[i].pos = uti.getTermPosition(oriTerms, terms[i].term);
		}
		sort(terms, terms + 3);

		if (terms[1].pos - terms[0].pos == 1 && terms[2].pos - terms[1].pos == 1){//term1与term2相邻，term2与term3相邻

			string new_term1 = terms[0].term + terms[2].term;
			string mutant = exp;
			int pos = exp.find(terms[0].term);
			mutant.replace(mutant.begin() + pos, mutant.begin() + pos + terms[0].term.length() + terms[1].term.length() + terms[2].term.length() + 2, new_term1);
			mutants.addMutant(mutant, "TOFxDORFCase2");
		}
	}
	return mutants;
}

//CORFxTNF---(73)——一个项，先CORF，然后分裂后的其中一个项TNF
Mutants DnfMutant::generateCORFxTNFdoublemutants(string exp, vector<string> oriTerms, vector<string> terms, string model){
	Mutants mutants(exp);
	Utility uti;
	if (terms.size() == 1){
		string term = terms[0];

		string new_term1, new_term2;
		vector<string> literals_in_term = uti.extractCNFstyle(term);
		//CORF
		for (int j = 1; j < literals_in_term.size(); j++)
		{
			int pos_insert_operator_or = term.find(literals_in_term[j]);
			new_term1 = term.substr(0, pos_insert_operator_or);
			new_term2 = term.substr(pos_insert_operator_or, term.length());

			//TNF
			string new_term3 = "!(" + new_term1 + ")";

			string new_term4 = new_term3 + "+" + new_term2;
			string mutant = exp;
			int pos = exp.find(term);
			mutant.replace(mutant.begin() + pos, mutant.begin() + pos + term.length(), new_term4);
			mutants.addMutant(mutant, "CORFxTNF");

			new_term3 = "!(" + new_term2 + ")";
			new_term4 = new_term1 + "+" + new_term3;
			mutant = exp;
			pos = exp.find(term);
			mutant.replace(mutant.begin() + pos, mutant.begin() + pos + term.length(), new_term4);
			mutants.addMutant(mutant, "CORFxTNF");
		}
	}
	return mutants;
}

//CORFxTOF---(76)——一个项，先CORF，然后分裂后的其中一个项TOF
Mutants DnfMutant::generateCORFxTOFdoublemutants(string exp, vector<string> oriTerms, vector<string> terms, string model){
	Mutants mutants(exp);
	Utility uti;
	if (terms.size() == 1)
	{
		string term = terms[0];

		string new_term1, new_term2;
		vector<string> literals_in_term = uti.extractCNFstyle(term);
		//CORF
		for (int j = 1; j < literals_in_term.size(); j++)
		{
			int pos_insert_operator_or = term.find(literals_in_term[j]);
			new_term1 = term.substr(0, pos_insert_operator_or);
			new_term2 = term.substr(pos_insert_operator_or, term.length());

			string mutant = exp;
			int pos = exp.find(term);
			mutant.replace(mutant.begin() + pos, mutant.begin() + pos + term.length(), new_term1);
			mutants.addMutant(mutant, "CORFxTOF");

			mutant = exp;
			pos = exp.find(term);
			mutant.replace(mutant.begin() + pos, mutant.begin() + pos + term.length(), new_term2);
			mutants.addMutant(mutant, "CORFxTOF");
		}
	}
	return mutants;
}


//~~~~~~~~~~~ 词 + 项 ~~~~~~~~~~~ 
//ENFxLNF---(1)——连着的一部分发生ENF，剩下的一个项发生LNF
Mutants DnfMutant::generateENFxLNFCase1doublemutants(string exp, vector<string> oriTerms, vector<string> faultterms, string model)
{
	Mutants mutants(exp);
	Utility uti;
	if (faultterms.size() >= 3){
		vector<info> terms;
		for (int i = 0; i < faultterms.size(); i++){
			info t;
			t.term = faultterms[i];
			t.pos = uti.getTermPosition(oriTerms, faultterms[i]);//获取当前项在表达式中的位置
			terms.push_back(t);
		}
		sort(terms.begin(), terms.end());//排序

		bool flag = true;
		for (int i = 0; i < terms.size() - 2; i++){//是否连续
			if (terms[i + 1].pos - terms[i].pos != 1){
				flag = false;
				break;
			}
		}
		//前n-1个发生ENF，最后一个发生LNF
		if (flag){
			//ENF
			int start = 0;
			for (int i = 0; i < oriTerms.size(); i++){
				if (terms[0].term == oriTerms[i]){
					start = i;
					break;
				}
			}
			string enfExpression = terms[start].term;
			for (int i = start + 1; i < start + faultterms.size() - 1; i++){
				enfExpression = enfExpression + "+" + terms[i].term;
			}
			string new_enfExpression = "!(" + enfExpression + ")";

			string mutant = exp;
			int pos = exp.find(enfExpression);
			mutant.replace(mutant.begin() + pos, mutant.begin() + pos + enfExpression.length(), new_enfExpression);

			//LNF
			string lnfTerm = terms[faultterms.size() - 1].term;
			vector<string> literals_in_this_term = uti.extractCNFstyle(lnfTerm);
			int p2 = mutant.find(lnfTerm);

			for (int i = 0; i < literals_in_this_term.size(); i++){
				string mutant2 = mutant;
				string new_term = lnfTerm;
				string s1 = literals_in_this_term.at(i);//第i个literal
				string s2 = uti.getLiteralNegation(s1);//获取单词的Negation
				int pos1 = new_term.find(s1);
				new_term.replace(pos1, s1.size(), s2);//new_term为 LNF 后的项
				mutant2.replace(mutant2.begin() + p2, mutant2.begin() + p2 + lnfTerm.length(), new_term);
				mutants.addMutant(mutant2, "ENFxLNFCase1");
			}

		}
		//------------------------------------------------------
		flag = true;
		for (int i = 1; i < faultterms.size() - 1; i++){//判断是否连续
			if (terms[i + 1].pos - terms[i].pos != 1){
				flag = false;
				return mutants;
			}
		}
		//第1个发生LNF，后边的发生ENF
		if (flag){
			//LNF
			string lnfTerm = terms[0].term;
			vector<string> literals_in_this_term = uti.extractCNFstyle(lnfTerm);
			string mutant = exp;
			int p2 = mutant.find(lnfTerm);
			for (int j = 0; j < literals_in_this_term.size(); j++)
			{
				string mutant2 = mutant;
				string new_term = lnfTerm;
				string s1 = literals_in_this_term.at(j);//第j个literal
				string s2 = uti.getLiteralNegation(s1);//获取单词的Negation
				int pos1 = new_term.find(s1);
				new_term.replace(pos1, s1.size(), s2);//new_term为 LNF 后的项
				mutant2.replace(mutant2.begin() + p2, mutant2.begin() + p2 + lnfTerm.length(), new_term);

				//ENF
				int start;
				for (int i = 0; i < oriTerms.size(); i++){
					if (terms[0].term == oriTerms[i]){
						start = i;
						break;
					}
				}
				string enfExpression = terms[start + 1].term;
				for (int i = start + 2; i < start + faultterms.size(); i++){
					enfExpression = enfExpression + "+" + terms[i].term;
				}
				string new_enfExpression = "!(" + enfExpression + ")";
				int pos = mutant2.find(enfExpression);
				mutant2.replace(mutant2.begin() + pos, mutant2.begin() + pos + enfExpression.length(), new_enfExpression);
				mutants.addMutant(mutant2, "ENFxLNFCase1");
			}
		}
	}
	return mutants;
}

//ENFxLOF---(3)——连着的一部分发生ENF，剩下的一个项发生LOF
Mutants DnfMutant::generateENFxLOFCase1doublemutants(string exp, vector<string> oriTerms, vector<string> faultterms, string model)
{
	Mutants mutants(exp);
	Utility uti;
	if (faultterms.size() >= 3){
		vector<info> terms;
		for (int i = 0; i < faultterms.size(); i++){
			info t;
			t.term = faultterms[i];
			t.pos = uti.getTermPosition(oriTerms, faultterms[i]);//获取当前项在表达式中的位置
			terms.push_back(t);
		}
		sort(terms.begin(), terms.end());//排序

		bool flag = true;
		for (int i = 0; i < terms.size() - 2; i++){//是否连续
			if (terms[i + 1].pos - terms[i].pos != 1){
				flag = false;
				break;
			}
		}
		//前n-1个发生ENF，最后一个发生LOF
		if (flag){
			//ENF
			int start = 0;
			for (int i = 0; i < oriTerms.size(); i++){
				if (terms[0].term == oriTerms[i]){
					start = i;
					break;
				}
			}
			string enfExpression = terms[start].term;
			for (int i = start + 1; i < start + faultterms.size() - 1; i++){
				enfExpression = enfExpression + "+" + terms[i].term;
			}
			string new_enfExpression = "!(" + enfExpression + ")";

			string mutant = exp;
			int pos = exp.find(enfExpression);
			mutant.replace(mutant.begin() + pos, mutant.begin() + pos + enfExpression.length(), new_enfExpression);

			//LOF
			string lofTerm = terms[faultterms.size() - 1].term;
			vector<string> literals_in_this_term = uti.extractCNFstyle(lofTerm);
			int p2 = mutant.find(lofTerm);

			//第二个项LOF
			for (int n = 0; n < literals_in_this_term.size(); n++)
			{
				string mutant2 = mutant;
				string new_term = lofTerm;
				string s1 = literals_in_this_term.at(n);
				int pos1 = new_term.find(s1);
				new_term.replace(pos1, s1.size(), "");

				mutant2.replace(mutant2.begin() + p2, mutant2.begin() + p2 + lofTerm.length(), new_term);
				mutants.addMutant(mutant2, "ENFxLOFCase1");
			}

		}
		//------------------------------------------------------
		flag = true;
		for (int i = 1; i < faultterms.size() - 1; i++){//判断是否连续
			if (terms[i + 1].pos - terms[i].pos != 1){
				flag = false;
				return mutants;
			}
		}
		//第1个发生LOF，后边的发生ENF
		if (flag){
			//LOF
			string lofTerm = terms[0].term;
			vector<string> literals_in_this_term = uti.extractCNFstyle(lofTerm);
			string mutant = exp;
			int p2 = mutant.find(lofTerm);

			for (int n = 0; n < literals_in_this_term.size(); n++)
			{
				string mutant2 = mutant;
				string new_term = lofTerm;
				string s1 = literals_in_this_term.at(n);
				int pos1 = new_term.find(s1);
				new_term.replace(pos1, s1.size(), "");
				mutant2.replace(mutant2.begin() + p2, mutant2.begin() + p2 + lofTerm.length(), new_term);

				//ENF
				int start;
				for (int i = 0; i < oriTerms.size(); i++){
					if (terms[0].term == oriTerms[i]){
						start = i;
						break;
					}
				}
				string enfExpression = terms[start + 1].term;
				for (int i = start + 2; i < start + faultterms.size(); i++){
					enfExpression = enfExpression + "+" + terms[i].term;
				}
				string new_enfExpression = "!(" + enfExpression + ")";
				int pos = mutant2.find(enfExpression);
				mutant2.replace(mutant2.begin() + pos, mutant2.begin() + pos + enfExpression.length(), new_enfExpression);
				mutants.addMutant(mutant2, "ENFxLOFCase1");
			}
		}
	}
	return mutants;
}

//ENFxLIF---(5)——连着的一部分发生ENF，剩下的一个项发生LIF
Mutants DnfMutant::generateENFxLIFCase1doublemutants(string exp, vector<string> oriTerms, vector<string> faultterms, string model)
{
	Mutants mutants(exp);
	Utility uti;
	if (faultterms.size() >= 3){
		vector<info> terms;
		for (int i = 0; i < faultterms.size(); i++){
			info t;
			t.term = faultterms[i];
			t.pos = uti.getTermPosition(oriTerms, faultterms[i]);//获取当前项在表达式中的位置
			terms.push_back(t);
		}
		sort(terms.begin(), terms.end());//排序

		bool flag = true;
		for (int i = 0; i < terms.size() - 2; i++){//是否连续
			if (terms[i + 1].pos - terms[i].pos != 1){
				flag = false;
				break;
			}
		}
		//前n-1个发生ENF，最后一个发生LIF
		if (flag){
			//ENF
			int start = 0;
			for (int i = 0; i < oriTerms.size(); i++){
				if (terms[0].term == oriTerms[i]){
					start = i;
					break;
				}
			}
			string enfExpression = terms[start].term;
			for (int i = start + 1; i < start + faultterms.size() - 1; i++){
				enfExpression = enfExpression + "+" + terms[i].term;
			}
			string new_enfExpression = "!(" + enfExpression + ")";

			string mutant = exp;
			int pos = exp.find(enfExpression);
			mutant.replace(mutant.begin() + pos, mutant.begin() + pos + enfExpression.length(), new_enfExpression);

			//LIF
			string lifTerm = terms[faultterms.size() - 1].term;
			vector<string> literals_in_this_term = uti.extractCNFstyle(lifTerm);
			int p2 = mutant.find(lifTerm);

			//第二个项LIF
			for (int k = 0; k < model.length(); k++)
			{
				string mutant2 = mutant;
				char toReplaceWith1 = model[k];
				if (lifTerm.find(toReplaceWith1) == string::npos)
				{
					string s11, s12;
					s11 += toReplaceWith1;
					s12 += '!';
					s12 += toReplaceWith1;
					string new_term11 = (lifTerm + s11);
					string new_term12 = (lifTerm + s12);

					mutant2.replace(mutant2.begin() + p2, mutant2.begin() + p2 + lifTerm.length(), new_term11);
					mutants.addMutant(mutant2, "ENFxLIFCase1");

					mutant2 = mutant;
					mutant2.replace(mutant2.begin() + p2, mutant2.begin() + p2 + lifTerm.length(), new_term12);
					mutants.addMutant(mutant2, "ENFxLIFCase1");
				}
			}
		}
		//------------------------------------------------------
		flag = true;
		for (int i = 1; i < faultterms.size() - 1; i++){//判断是否连续
			if (terms[i + 1].pos - terms[i].pos != 1){
				flag = false;
				return mutants;
			}
		}
		//第1个发生LIF，后边的发生ENF
		if (flag){
			//LIF
			string lifTerm = terms[0].term;
			vector<string> literals_in_this_term = uti.extractCNFstyle(lifTerm);
			string mutant = exp;
			int p2 = mutant.find(lifTerm);

			for (int k = 0; k < model.length(); k++)
			{
				string mutant2 = mutant;
				string mutant3 = mutant;
				char toReplaceWith1 = model[k];
				if (lifTerm.find(toReplaceWith1) == string::npos)
				{
					string s11, s12;
					s11 += toReplaceWith1;
					s12 += '!';
					s12 += toReplaceWith1;
					string new_term11 = (lifTerm + s11);
					string new_term12 = (lifTerm + s12);

					mutant2.replace(mutant2.begin() + p2, mutant2.begin() + p2 + lifTerm.length(), new_term11);
					mutant3.replace(mutant3.begin() + p2, mutant3.begin() + p2 + lifTerm.length(), new_term12);

					//ENF
					int start;
					for (int i = 0; i < oriTerms.size(); i++){
						if (terms[0].term == oriTerms[i]){
							start = i;
							break;
						}
					}
					string enfExpression = terms[start + 1].term;
					for (int i = start + 2; i < start + faultterms.size(); i++){
						enfExpression = enfExpression + "+" + terms[i].term;
					}
					string new_enfExpression = "!(" + enfExpression + ")";
					int pos = mutant2.find(enfExpression);
					mutant2.replace(mutant2.begin() + pos, mutant2.begin() + pos + enfExpression.length(), new_enfExpression);
					mutants.addMutant(mutant2, "ENFxLIFCase1");

					mutant3.replace(mutant3.begin() + pos, mutant3.begin() + pos + enfExpression.length(), new_enfExpression);
					mutants.addMutant(mutant3, "ENFxLIFCase1");
				}
			}
		}

	}
	return mutants;
}

//ENFxLRF---(7)——连着的一部分发生ENF，剩下的一个项发生LRF
Mutants DnfMutant::generateENFxLRFCase1doublemutants(string exp, vector<string> oriTerms, vector<string> faultterms, string model)
{
	Mutants mutants(exp);
	Utility uti;
	if (faultterms.size() >= 3){
		vector<info> terms;
		for (int i = 0; i < faultterms.size(); i++){
			info t;
			t.term = faultterms[i];
			t.pos = uti.getTermPosition(oriTerms, faultterms[i]);//获取当前项在表达式中的位置
			terms.push_back(t);
		}
		sort(terms.begin(), terms.end());//排序

		bool flag = true;
		for (int i = 0; i < terms.size() - 2; i++){//是否连续
			if (terms[i + 1].pos - terms[i].pos != 1){
				flag = false;
				break;
			}
		}
		//前n-1个发生ENF，最后一个发生LRF
		if (flag){
			//ENF
			int start = 0;
			for (int i = 0; i < oriTerms.size(); i++){
				if (terms[0].term == oriTerms[i]){
					start = i;
					break;
				}
			}
			string enfExpression = terms[start].term;
			for (int i = start + 1; i < start + faultterms.size() - 1; i++){
				enfExpression = enfExpression + "+" + terms[i].term;
			}
			string new_enfExpression = "!(" + enfExpression + ")";

			string mutant = exp;
			int pos = exp.find(enfExpression);
			mutant.replace(mutant.begin() + pos, mutant.begin() + pos + enfExpression.length(), new_enfExpression);

			//LRF
			string lrfTerm = terms[faultterms.size() - 1].term;
			vector<string> literals_in_this_term = uti.extractCNFstyle(lrfTerm);
			int p2 = mutant.find(lrfTerm);

			for (int k = 0; k < model.length(); k++)
			{
				wchar_t toReplaceWith1 = model[k];//替换的词
				if (lrfTerm.find(toReplaceWith1) == string::npos)
				{
					string s11, s12;
					s11 += toReplaceWith1;
					s12 += '!';
					s12 += toReplaceWith1;

					for (int p = 0; p < literals_in_this_term.size(); p++)
					{
						string mutant2 = mutant;
						string mutant3 = mutant;
						string new_term11 = lrfTerm;
						string new_term12 = lrfTerm;
						int literal_pos = new_term11.find(literals_in_this_term.at(p));
						new_term11.replace(new_term11.begin() + literal_pos, new_term11.begin() + literal_pos + literals_in_this_term.at(p).size(), s11);
						new_term12.replace(new_term12.begin() + literal_pos, new_term12.begin() + literal_pos + literals_in_this_term.at(p).size(), s12);

						mutant2.replace(mutant2.begin() + p2, mutant2.begin() + p2 + lrfTerm.length(), new_term11);
						mutants.addMutant(mutant2, "ENFxLRFCase1");

						mutant3.replace(mutant3.begin() + p2, mutant3.begin() + p2 + lrfTerm.length(), new_term12);
						mutants.addMutant(mutant3, "ENFxLRFCase1");
					}
				}
			}
		}
		//------------------------------------------------------
		flag = true;
		for (int i = 1; i < faultterms.size() - 1; i++){//判断是否连续
			if (terms[i + 1].pos - terms[i].pos != 1){
				flag = false;
				return mutants;
			}
		}
		//第1个发生LRF，后边的发生ENF
		if (flag){
			//LRF
			string lrfTerm = terms[0].term;
			vector<string> literals_in_this_term = uti.extractCNFstyle(lrfTerm);
			string mutant = exp;
			int p2 = mutant.find(lrfTerm);

			for (int k = 0; k < model.length(); k++)
			{
				string mutant2 = mutant;
				string mutant3 = mutant;
				char toReplaceWith1 = model[k];
				if (lrfTerm.find(toReplaceWith1) == string::npos)
				{
					string s11, s12;
					s11 += toReplaceWith1;
					s12 += '!';
					s12 += toReplaceWith1;

					for (int p = 0; p < literals_in_this_term.size(); p++)
					{
						string mutant2 = mutant;
						string mutant3 = mutant;
						string new_term11 = lrfTerm;
						string new_term12 = lrfTerm;
						int literal_pos = new_term11.find(literals_in_this_term.at(p));
						new_term11.replace(new_term11.begin() + literal_pos, new_term11.begin() + literal_pos + literals_in_this_term.at(p).size(), s11);
						new_term12.replace(new_term12.begin() + literal_pos, new_term12.begin() + literal_pos + literals_in_this_term.at(p).size(), s12);

						mutant2.replace(mutant2.begin() + p2, mutant2.begin() + p2 + lrfTerm.length(), new_term11);
						mutant3.replace(mutant3.begin() + p2, mutant3.begin() + p2 + lrfTerm.length(), new_term12);

						//ENF
						int start;
						for (int i = 0; i < oriTerms.size(); i++){
							if (terms[0].term == oriTerms[i]){
								start = i;
								break;
							}
						}
						string enfExpression = terms[start + 1].term;
						for (int i = start + 2; i < start + faultterms.size(); i++){
							enfExpression = enfExpression + "+" + terms[i].term;
						}
						string new_enfExpression = "!(" + enfExpression + ")";

						int pos1 = mutant2.find(enfExpression);
						mutant2.replace(mutant2.begin() + pos1, mutant2.begin() + pos1 + enfExpression.length(), new_enfExpression);
						mutants.addMutant(mutant2, "ENFxLRFCase1");

						int pos2 = mutant3.find(enfExpression);
						mutant3.replace(mutant3.begin() + pos2, mutant3.begin() + pos2 + enfExpression.length(), new_enfExpression);
						mutants.addMutant(mutant3, "ENFxLRFCase1");
					}
				}
			}
		}

	}
	return mutants;
}



//TNFxLNF---(9)——两个项
Mutants DnfMutant::generateTNFxLNFTwoTermdoublemutants(string exp, vector<string> oriTerms, vector<string> terms, string model){
	Mutants mutants(exp);
	Utility uti;

	for (int m = 0; m < terms.size() - 1; m++){
		for (int n = m + 1; n < terms.size(); n++){
			string term1 = terms[m];
			string term2 = terms[n];

			//term1: TNF, term2: LNF
			string new_term1 = "!(" + term1 + ")";
			string mutant = exp;
			int pos = mutant.find(term1);
			mutant.replace(mutant.begin() + pos, mutant.begin() + pos + term1.length(), new_term1);

			vector<string> literals_in_term = uti.extractCNFstyle(term2);
			for (int i = 0; i < literals_in_term.size(); i++)
			{
				//LNF
				string mutant11 = mutant;
				string new_term2 = term2;
				string s1 = literals_in_term.at(i);//第i个literal
				string s2 = uti.getLiteralNegation(s1);//获取单词的Negation
				int pos1 = new_term2.find(s1);
				new_term2.replace(pos1, s1.size(), s2);//temp_term2为 LNF 后的项

				int pos2 = mutant11.find(term2);
				mutant11.replace(mutant11.begin() + pos2, mutant11.begin() + pos2 + term2.length(), new_term2);
				mutants.addMutant(mutant11, "TNFxLNFTwoTerm");
				cout << "mutant11:" << mutant11 << endl;
			}

			//--------------------------------------------------------------------
			//term2: TNF, term1: LNF
			string new_term4 = "!(" + term2 + ")";
			string mutant2 = exp;
			int pos3 = mutant2.find(term2);
			mutant2.replace(mutant2.begin() + pos3, mutant2.begin() + pos3 + term2.length(), new_term4);

			vector<string> literals_in_term_2 = uti.extractCNFstyle(term1);
			for (int i = 0; i < literals_in_term_2.size(); i++)
			{
				//LNF
				string mutant22 = mutant2;
				string new_term3 = term1;
				string s1 = literals_in_term_2.at(i);//第i个literal
				string s2 = uti.getLiteralNegation(s1);//获取单词的Negation
				int pos1 = new_term3.find(s1);
				new_term3.replace(pos1, s1.size(), s2);//temp_term3为 LNF 后的项

				int pos2 = mutant22.find(term1);
				mutant22.replace(mutant22.begin() + pos2, mutant22.begin() + pos2 + term1.length(), new_term3);
				mutants.addMutant(mutant22, "TNFxLNFTwoTerm");
			}
		}
	}
	return mutants;
}

//TNFxLNF---(10)——一个项
Mutants DnfMutant::generateTNFxLNFOneTermdoublemutants(string exp, vector<string> oriTerms, vector<string> terms, string model){
	Mutants mutants(exp);
	Utility uti;

	//先LNF，再TNF
	for (int m = 0; m < terms.size(); m++){
		string term = terms[m];
		int pos = exp.find(term);

		vector<string> literals_in_term = uti.extractCNFstyle(term);
		for (int i = 0; i < literals_in_term.size(); i++)
		{
			//LNF
			string new_term1 = term;
			string s1 = literals_in_term.at(i);//第i个literal
			string s2 = uti.getLiteralNegation(s1);//获取单词的Negation
			int pos1 = new_term1.find(s1);
			new_term1.replace(pos1, s1.size(), s2);//temp_term2为 LNF 后的项

			//TNF
			string new_term2 = "!(" + new_term1 + ")";

			string mutant = exp;
			mutant.replace(mutant.begin() + pos, mutant.begin() + pos + term.size(), new_term2);
			mutants.addMutant(mutant, "TNFxLNFOneTerm");
		}
	}
	return mutants;
}

//TNFxLOF---(11)——两个项
Mutants DnfMutant::generateTNFxLOFTwoTermdoublemutants(string exp, vector<string> oriTerms, vector<string> terms, string model){
	Mutants mutants(exp);
	Utility uti;

	for (int m = 0; m < terms.size() - 1; m++){
		for (int n = m + 1; n < terms.size(); n++){
			string term1 = terms[m];
			string term2 = terms[n];

			//第1项：TNF， 第2项：LOF
			//TNF
			string new_term1 = "!(" + term1 + ")";
			string mutant = exp;
			int pos = mutant.find(term1);
			mutant.replace(mutant.begin() + pos, mutant.begin() + pos + term1.length(), new_term1);

			//LOF
			vector<string> literals_in_term = uti.extractCNFstyle(term2);
			for (int i = 0; i < literals_in_term.size(); i++)
			{
				string mutant1 = mutant;
				string new_term2 = term2;
				string s1 = literals_in_term.at(i);
				int pos1 = new_term2.find(s1);
				new_term2.replace(pos1, s1.size(), "");

				int pos2 = mutant1.find(term2);
				mutant1.replace(mutant1.begin() + pos2, mutant1.begin() + pos2 + term2.length(), new_term2);
				mutants.addMutant(mutant1, "TNFxLOFTwoTerm");
			}

			//第2项：TNF， 第1项：LOF
			//TNF——第2项
			string new_term4 = "!(" + term2 + ")";
			string mutant2 = exp;
			int pos3 = mutant2.find(term2);
			mutant2.replace(mutant2.begin() + pos3, mutant2.begin() + pos3 + term2.length(), new_term4);


			//LOF——第1项
			vector<string> literals_in_term_2 = uti.extractCNFstyle(term1);
			for (int i = 0; i < literals_in_term_2.size(); i++)
			{
				string mutant3 = mutant2;
				string new_term3 = term1;
				string s1 = literals_in_term_2.at(i);
				int pos4 = new_term3.find(s1);
				new_term3.replace(pos4, s1.size(), "");

				int pos5 = mutant3.find(term1);
				mutant3.replace(mutant3.begin() + pos5, mutant3.begin() + pos5 + term1.length(), new_term3);
				mutants.addMutant(mutant3, "TNFxLOFTwoTerm");
			}
		}
	}
	return mutants;
}

//TNFxLOF---(12)——一个项——先LOF，再TNF
Mutants DnfMutant::generateTNFxLOFOneTermdoublemutants(string exp, vector<string> oriTerms, vector<string> terms, string model){
	Mutants mutants(exp);
	Utility uti;

	for (int m = 0; m < terms.size(); m++){
		string term = terms[m];
		int pos = exp.find(term);

		vector<string> literals_in_term = uti.extractCNFstyle(term);
		for (int i = 0; i < literals_in_term.size(); i++)
		{
			//LNF
			string new_term1 = term;
			string s1 = literals_in_term.at(i);
			int pos1 = new_term1.find(s1);
			new_term1.replace(pos1, s1.size(), "");

			//TOF
			string new_term2 = "!(" + new_term1 + ")";

			string mutant = exp;
			int pos = mutant.find(term);
			mutant.replace(mutant.begin() + pos, mutant.begin() + pos + term.length(), new_term2);
			mutants.addMutant(mutant, "TNFxLOFOneTerm");
		}
	}
	return mutants;
}

//TNFxLIF---(13)——两个项
Mutants DnfMutant::generateTNFxLIFTwoTermdoublemutants(string exp, vector<string> oriTerms, vector<string> terms, string model){
	Mutants mutants(exp);
	Utility uti;

	for (int m = 0; m < terms.size() - 1; m++){
		for (int n = m + 1; n < terms.size(); n++){
			string term1 = terms[m];
			string term2 = terms[n];

			//第1个项: TNF, 第2个项: LIF
			//TNF——term1
			string new_term1 = "!(" + term1 + ")";
			string mutant = exp;
			int pos = mutant.find(term1);
			mutant.replace(mutant.begin() + pos, mutant.begin() + pos + term1.length(), new_term1);

			//LIF——term2
			for (int i = 0; i < model.length(); i++)
			{
				string mutant1 = mutant;
				char toReplaceWith = model[i];
				string s21, s22;
				s21 += toReplaceWith;
				s22 += '!';
				s22 += toReplaceWith;
				if (term2.find(toReplaceWith) == string::npos)
				{
					string new_term11 = (term2 + s21);
					string new_term12 = (term2 + s22);

					int pos2 = mutant1.find(term2);
					string mutant2 = mutant1;
					mutant2.replace(mutant2.begin() + pos2, mutant2.begin() + pos2 + term2.length(), new_term11);
					mutants.addMutant(mutant2, "TNFxLIFTwoTerm");

					string mutant3 = mutant1;
					mutant3.replace(mutant3.begin() + pos2, mutant3.begin() + pos2 + term2.length(), new_term12);
					mutants.addMutant(mutant3, "TNFxLIFTwoTerm");

				}
			}

			//第2个项: TNF, 第1个项: LIF
			//TNF——term2
			string new_term2 = "!(" + term2 + ")";
			string mutant4 = exp;
			int pos3 = mutant4.find(term2);
			mutant4.replace(mutant4.begin() + pos3, mutant4.begin() + pos3 + term2.length(), new_term2);

			//LIF
			for (int i = 0; i < model.length(); i++)
			{
				string mutant5 = mutant4;
				char toReplaceWith = model[i];
				string s21, s22;
				s21 += toReplaceWith;
				s22 += '!';
				s22 += toReplaceWith;
				if (term1.find(toReplaceWith) == string::npos)
				{
					string new_term11 = (term1 + s21);
					string new_term12 = (term1 + s22);

					int pos2 = mutant5.find(term2);
					string mutant2 = mutant5;
					mutant2.replace(mutant2.begin() + pos2, mutant2.begin() + pos2 + term2.length(), new_term11);
					mutants.addMutant(mutant2, "TNFxLIFTwoTerm");

					string mutant3 = mutant5;
					mutant3.replace(mutant3.begin() + pos2, mutant3.begin() + pos2 + term2.length(), new_term12);
					mutants.addMutant(mutant3, "TNFxLIFTwoTerm");

				}
			}
		}
	}
	return mutants;
}

//TNFxLIF---(14)——一个项——先LIF，再TNF
Mutants DnfMutant::generateTNFxLIFOneTermdoublemutants(string exp, vector<string> oriTerms, vector<string> terms, string model){
	Mutants mutants(exp);
	Utility uti;

	//先LIF，再TNF
	for (int m = 0; m < terms.size(); m++){
		string term = terms[m];
		int pos = exp.find(term);

		//LIF
		for (int i = 0; i < model.length(); i++)
		{
			char toReplaceWith = model[i];
			string s21, s22;
			s21 += toReplaceWith;
			s22 += '!';
			s22 += toReplaceWith;
			if (term.find(toReplaceWith) == string::npos)
			{
				string new_term11 = (term + s21);
				string new_term12 = (term + s22);

				//TNF
				new_term11 = "!(" + new_term11 + ")";
				new_term12 = "!(" + new_term12 + ")";

				string mutant = exp;
				mutant.replace(mutant.begin() + pos, mutant.begin() + pos + term.length(), new_term11);
				mutants.addMutant(mutant, "TNFxLIFOneTerm");

				mutant = exp;
				mutant.replace(mutant.begin() + pos, mutant.begin() + pos + term.length(), new_term12);
				mutants.addMutant(mutant, "TNFxLIFOneTerm");

			}
		}
	}
	return mutants;
}

//TNFxLRF---(15)——两个项
Mutants DnfMutant::generateTNFxLRFTwoTermdoublemutants(string exp, vector<string> oriTerms, vector<string> terms, string model){
	Mutants mutants(exp);
	Utility uti;

	for (int m = 0; m < terms.size() - 1; m++){
		for (int n = m + 1; n < terms.size(); n++){
			string term1 = terms[m];
			string term2 = terms[n];

			vector<string> literals_in_term_1 = uti.extractCNFstyle(term1);
			vector<string> literals_in_term_2 = uti.extractCNFstyle(term2);

			//第1个项：TNF,  第2个项：LRF
			//TNF——term1
			string new_term1 = "!(" + term1 + ")";
			string mutant = exp;
			int pos = mutant.find(term1);
			mutant.replace(mutant.begin() + pos, mutant.begin() + pos + term1.length(), new_term1);

			//LRF——term2
			for (int i = 0; i < model.length(); i++)
			{
				string mutant1 = mutant;
				char toReplaceWith = model[i];
				if (term2.find(toReplaceWith) == string::npos)
				{
					string s11, s12;
					s11 += toReplaceWith;//不在当前项中的词literals
					s12 += '!';
					s12 += toReplaceWith;
					for (int k = 0; k < literals_in_term_2.size(); k++)
					{
						string new_term11 = term2;
						string new_term12 = term2;
						int literal_pos = new_term11.find(literals_in_term_2[k]);
						new_term11.replace(new_term11.begin() + literal_pos, new_term11.begin() + literal_pos + literals_in_term_2[k].size(), s11);
						new_term12.replace(new_term12.begin() + literal_pos, new_term12.begin() + literal_pos + literals_in_term_2[k].size(), s12);

						int pos2 = mutant1.find(term2);
						string mutant2 = mutant1;
						mutant2.replace(mutant2.begin() + pos2, mutant2.begin() + pos2 + term2.length(), new_term11);
						mutants.addMutant(mutant2, "TNFxLRFTwoTerm");


						mutant2 = mutant1;
						mutant2.replace(mutant2.begin() + pos2, mutant2.begin() + pos2 + term2.length(), new_term12);
						mutants.addMutant(mutant2, "TNFxLRFTwoTerm");
					}
				}
			}

			//第2个项：TNF,  第1个项：LRF
			//TNF——term2
			string new_term2 = "!(" + term2 + ")";
			mutant = exp;
			pos = mutant.find(term2);
			mutant.replace(mutant.begin() + pos, mutant.begin() + pos + term2.length(), new_term2);


			//LRF——term1
			for (int i = 0; i < model.length(); i++)
			{
				char toReplaceWith = model[i];
				if (term1.find(toReplaceWith) == string::npos)
				{
					string mutant1 = mutant;
					string s11, s12;
					s11 += toReplaceWith;//不在当前项中的词literals
					s12 += '!';
					s12 += toReplaceWith;
					for (int k = 0; k < literals_in_term_1.size(); k++)
					{
						string new_term11 = term1;
						string new_term12 = term1;
						int literal_pos = new_term11.find(literals_in_term_1[k]);
						new_term11.replace(new_term11.begin() + literal_pos, new_term11.begin() + literal_pos + literals_in_term_1[k].size(), s11);
						new_term12.replace(new_term12.begin() + literal_pos, new_term12.begin() + literal_pos + literals_in_term_1[k].size(), s12);

						int pos2 = mutant1.find(term1);
						string mutant2 = mutant1;
						mutant2.replace(mutant2.begin() + pos2, mutant2.begin() + pos2 + term1.length(), new_term11);
						mutants.addMutant(mutant2, "TNFxLRFTwoTerm");

						mutant2 = mutant1;
						mutant2.replace(mutant2.begin() + pos2, mutant2.begin() + pos2 + term1.length(), new_term12);
						mutants.addMutant(mutant2, "TNFxLRFTwoTerm");
					}
				}
			}
		}
	}
	return mutants;
}

//TNFxLRF---(16)——一个项——先LRF，再TNF
Mutants DnfMutant::generateTNFxLRFOneTermdoublemutants(string exp, vector<string> oriTerms, vector<string> terms, string model){
	Mutants mutants(exp);
	Utility uti;

	for (int m = 0; m < terms.size(); m++){
		string term = terms[m];
		int pos = exp.find(term);

		vector<string> literals_in_this_term = uti.extractCNFstyle(term);

		for (int j = 0; j < model.length(); j++)
		{
			char toReplaceWith = model[j];
			if (term.find(toReplaceWith) == string::npos)
			{
				string s11, s12;
				s11 += toReplaceWith;//不在当前项中的词literals
				s12 += '!';
				s12 += toReplaceWith;

				for (int k = 0; k < literals_in_this_term.size(); k++)
				{
					string new_term11 = term;
					string new_term12 = term;

					//LRF
					int literal_pos = new_term11.find(literals_in_this_term[k]);
					new_term11.replace(new_term11.begin() + literal_pos, new_term11.begin() + literal_pos + literals_in_this_term[k].size(), s11);
					new_term12.replace(new_term12.begin() + literal_pos, new_term12.begin() + literal_pos + literals_in_this_term[k].size(), s12);

					//TNF
					string new_term21, new_term22;
					new_term21 = "!(" + new_term11 + ")";
					new_term22 = "!(" + new_term12 + ")";

					string mutant = exp;
					mutant.replace(mutant.begin() + pos, mutant.begin() + pos + term.length(), new_term21);
					mutants.addMutant(mutant, "TNFxLRFOneTerm");

					mutant = exp;
					mutant.replace(mutant.begin() + pos, mutant.begin() + pos + term.length(), new_term22);
					mutants.addMutant(mutant, "TNFxLRFOneTerm");
				}
			}
		}
	}
	return mutants;
}

//TOFxLNF---(17)——两个项
Mutants DnfMutant::generateTOFxLNFdoublemutants(string exp, vector<string> oriTerms, vector<string> terms, string model){
	Mutants mutants(exp);
	Utility uti;

	for (int m = 0; m < terms.size() - 1; m++){
		for (int n = m + 1; n < terms.size(); n++){
			string term1 = terms[m];
			string term2 = terms[n];

			vector<string> literals_in_term_1 = uti.extractCNFstyle(term1);
			vector<string> literals_in_term_2 = uti.extractCNFstyle(term2);

			//第1个项LNF
			for (int i = 0; i < literals_in_term_1.size(); i++)
			{
				string new_term1 = term1;
				string s1 = literals_in_term_1.at(i);//第i个literal
				string s2 = uti.getLiteralNegation(s1);//获取单词的Negation
				int pos1 = new_term1.find(s1);
				new_term1.replace(pos1, s1.size(), s2);

				//TOF
				string mutant = exp;
				int p = uti.getTermPosition(oriTerms, term2);
				int pos2 = mutant.find(term2);
				if (p == oriTerms.size()){
					mutant.replace(mutant.begin() + pos2 - 1, mutant.begin() + pos2 + term2.length(), "");
				}
				else{
					mutant.replace(mutant.begin() + pos2, mutant.begin() + pos2 + term2.length() + 1, "");
				}

				int pos3 = mutant.find(term1);
				mutant.replace(mutant.begin() + pos3, mutant.begin() + pos3 + term1.length(), new_term1);
				mutants.addMutant(mutant, "TOFxLNF");

			}

			//第2个项LNF
			for (int i = 0; i < literals_in_term_2.size(); i++)
			{
				string new_term2 = term2;
				string s1 = literals_in_term_2.at(i);//第i个literal
				string s2 = uti.getLiteralNegation(s1);//获取单词的Negation
				int pos1 = new_term2.find(s1);
				new_term2.replace(pos1, s1.size(), s2);

				string mutant = exp;
				int p = uti.getTermPosition(oriTerms, term1);
				int pos2 = mutant.find(term1);
				if (p == oriTerms.size()){
					mutant.replace(mutant.begin() + pos2 - 1, mutant.begin() + pos2 + term1.length(), "");
				}
				else{
					mutant.replace(mutant.begin() + pos2, mutant.begin() + pos2 + term1.length() + 1, "");
				}

				int pos3 = mutant.find(term2);
				mutant.replace(mutant.begin() + pos3, mutant.begin() + pos3 + term2.length(), new_term2);
				mutants.addMutant(mutant, "TOFxLNF");
			}
		}
	}
	return mutants;
}

//TOFxLOF---(18)
Mutants DnfMutant::generateTOFxLOFdoublemutants(string exp, vector<string> oriTerms, vector<string> terms, string model){
	Mutants mutants(exp);
	Utility uti;

	for (int m = 0; m < terms.size() - 1; m++){
		for (int n = m + 1; n < terms.size(); n++){
			string term1 = terms[m];
			string term2 = terms[n];

			vector<string> literals_in_term_1 = uti.extractCNFstyle(term1);
			vector<string> literals_in_term_2 = uti.extractCNFstyle(term2);

			//第一个项LOF
			for (int i = 0; i < literals_in_term_1.size(); i++)
			{
				string new_term1 = term1;
				string s1 = literals_in_term_1.at(i);
				int pos1 = new_term1.find(s1);
				new_term1.replace(pos1, s1.size(), "");


				//TOF
				string mutant = exp;
				int p = uti.getTermPosition(oriTerms, term2);
				int pos2 = mutant.find(term2);
				if (p == oriTerms.size()){
					mutant.replace(mutant.begin() + pos2 - 1, mutant.begin() + pos2 + term2.length(), "");
				}
				else{
					mutant.replace(mutant.begin() + pos2, mutant.begin() + pos2 + term2.length() + 1, "");
				}

				int pos3 = mutant.find(term1);
				mutant.replace(mutant.begin() + pos3, mutant.begin() + pos3 + term1.length(), new_term1);
				mutants.addMutant(mutant, "TOFxLOF");
			}

			//第二个项LOF
			for (int i = 0; i < literals_in_term_2.size(); i++)
			{
				string new_term2 = term2;
				string s1 = literals_in_term_2.at(i);
				int pos1 = new_term2.find(s1);
				new_term2.replace(pos1, s1.size(), "");

				//TOF
				string mutant = exp;
				int p = uti.getTermPosition(oriTerms, term1);
				int pos2 = mutant.find(term1);
				if (p == oriTerms.size()){
					mutant.replace(mutant.begin() + pos2 - 1, mutant.begin() + pos2 + term1.length(), "");
				}
				else{
					mutant.replace(mutant.begin() + pos2, mutant.begin() + pos2 + term1.length() + 1, "");
				}

				int pos3 = mutant.find(term2);
				mutant.replace(mutant.begin() + pos3, mutant.begin() + pos3 + term2.length(), new_term2);
				mutants.addMutant(mutant, "TOFxLOF");
			}
		}
	}
	return mutants;
}

//TOFxLIF---(19)
Mutants DnfMutant::generateTOFxLIFdoublemutants(string exp, vector<string> oriTerms, vector<string> terms, string model){
	Mutants mutants(exp);
	Utility uti;

	for (int m = 0; m < terms.size() - 1; m++){
		for (int n = m + 1; n < terms.size(); n++){
			string term1 = terms[m];
			string term2 = terms[n];

			//第2个项TOF
			string mutant = exp;
			int p = uti.getTermPosition(oriTerms, term2);
			int pos2 = mutant.find(term2);
			if (p == oriTerms.size()){
				mutant.replace(mutant.begin() + pos2 - 1, mutant.begin() + pos2 + term2.length(), "");
			}
			else{
				mutant.replace(mutant.begin() + pos2, mutant.begin() + pos2 + term2.length() + 1, "");
			}

			//第1个项LIF
			for (int i = 0; i < model.length(); i++)
			{
				char toReplaceWith = model[i];
				string s21, s22;
				s21 += toReplaceWith;
				s22 += '!';
				s22 += toReplaceWith;
				if (term1.find(toReplaceWith) == string::npos)
				{
					string new_term11 = (term1 + s21);
					string new_term12 = (term1 + s22);

					string mutant11 = mutant;
					int pos3 = mutant11.find(term1);
					mutant11.replace(mutant11.begin() + pos3, mutant11.begin() + pos3 + term1.length(), new_term11);
					mutants.addMutant(mutant11, "TOFxLIF");

					string mutant12 = mutant;
					pos3 = mutant12.find(term1);
					mutant12.replace(mutant12.begin() + pos3, mutant12.begin() + pos3 + term1.length(), new_term12);
					mutants.addMutant(mutant12, "TOFxLIF");
				}
			}

			//--------------------------------------
			//第1个项TOF
			string mutant2 = exp;
			p = uti.getTermPosition(oriTerms, term1);
			pos2 = mutant2.find(term1);
			if (p == oriTerms.size()){
				mutant2.replace(mutant2.begin() + pos2 - 1, mutant2.begin() + pos2 + term1.length(), "");
			}
			else{
				mutant2.replace(mutant2.begin() + pos2, mutant2.begin() + pos2 + term1.length() + 1, "");
			}
			//第2个项LIF
			for (int i = 0; i < model.length(); i++)
			{
				char toReplaceWith = model[i];
				string s21, s22;
				s21 += toReplaceWith;
				s22 += '!';
				s22 += toReplaceWith;
				if (term2.find(toReplaceWith) == string::npos)
				{
					string new_term11 = (term2 + s21);
					string new_term12 = (term2 + s22);

					string mutant21 = mutant2;
					int pos3 = mutant21.find(term2);
					mutant21.replace(mutant21.begin() + pos3, mutant21.begin() + pos3 + term2.length(), new_term11);
					mutants.addMutant(mutant21, "TOFxLIF");

					string mutant22 = mutant2;
					pos3 = mutant22.find(term2);
					mutant22.replace(mutant22.begin() + pos3, mutant22.begin() + pos3 + term2.length(), new_term12);
					mutants.addMutant(mutant22, "TOFxLIF");
				}
			}
		}
	}
	return mutants;
}

//TOFxLRF---(20)
Mutants DnfMutant::generateTOFxLRFdoublemutants(string exp, vector<string> oriTerms, vector<string> terms, string model){
	Mutants mutants(exp);
	Utility uti;

	for (int m = 0; m < terms.size() - 1; m++){
		for (int n = m + 1; n < terms.size(); n++){
			string term1 = terms[m];
			string term2 = terms[n];

			vector<string> literals_in_term_1 = uti.extractCNFstyle(term1);
			vector<string> literals_in_term_2 = uti.extractCNFstyle(term2);

			//第2个项TOF
			string mutant = exp;
			int p = uti.getTermPosition(oriTerms, term2);
			int pos2 = mutant.find(term2);
			if (p == oriTerms.size()){
				mutant.replace(mutant.begin() + pos2 - 1, mutant.begin() + pos2 + term2.length(), "");
			}
			else{
				mutant.replace(mutant.begin() + pos2, mutant.begin() + pos2 + term2.length() + 1, "");
			}

			//第一个项LRF
			for (int i = 0; i < model.length(); i++)
			{
				char toReplaceWith = model[i];
				if (term1.find(toReplaceWith) == string::npos)
				{
					string s11, s12;
					s11 += toReplaceWith;//不在当前项中的词literals
					s12 += '!';
					s12 += toReplaceWith;
					for (int k = 0; k < literals_in_term_1.size(); k++)
					{
						string new_term11 = term1;
						string new_term12 = term1;
						int literal_pos = new_term11.find(literals_in_term_1[k]);
						new_term11.replace(new_term11.begin() + literal_pos, new_term11.begin() + literal_pos + literals_in_term_1[k].size(), s11);
						new_term12.replace(new_term12.begin() + literal_pos, new_term12.begin() + literal_pos + literals_in_term_1[k].size(), s12);

						string mutant11 = mutant;
						int pos3 = mutant11.find(term1);
						mutant11.replace(mutant11.begin() + pos3, mutant11.begin() + pos3 + term1.length(), new_term11);
						mutants.addMutant(mutant11, "TOFxLRF");

						string mutant12 = mutant;
						pos3 = mutant12.find(term1);
						mutant12.replace(mutant12.begin() + pos3, mutant12.begin() + pos3 + term1.length(), new_term12);
						mutants.addMutant(mutant12, "TOFxLRF");
					}
				}
			}


			//--------------------------------------
			//第1个项TOF
			string mutant2 = exp;
			p = uti.getTermPosition(oriTerms, term1);
			pos2 = mutant2.find(term1);
			if (p == oriTerms.size()){
				mutant2.replace(mutant2.begin() + pos2 - 1, mutant2.begin() + pos2 + term1.length(), "");
			}
			else{
				mutant2.replace(mutant2.begin() + pos2, mutant2.begin() + pos2 + term1.length() + 1, "");
			}

			//第二个项LRF
			for (int i = 0; i < model.length(); i++)
			{
				char toReplaceWith = model[i];
				if (term2.find(toReplaceWith) == string::npos)
				{
					string s11, s12;
					s11 += toReplaceWith;//不在当前项中的词literals
					s12 += '!';
					s12 += toReplaceWith;
					for (int k = 0; k < literals_in_term_2.size(); k++)
					{
						string new_term11 = term2;
						string new_term12 = term2;
						int literal_pos = new_term11.find(literals_in_term_2[k]);
						new_term11.replace(new_term11.begin() + literal_pos, new_term11.begin() + literal_pos + literals_in_term_2[k].size(), s11);
						new_term12.replace(new_term12.begin() + literal_pos, new_term12.begin() + literal_pos + literals_in_term_2[k].size(), s12);

						string mutant21 = mutant2;
						int pos3 = mutant21.find(term2);
						mutant21.replace(mutant21.begin() + pos3, mutant21.begin() + pos3 + term2.length(), new_term11);
						mutants.addMutant(mutant21, "TOFxLRF");

						string mutant22 = mutant2;
						pos3 = mutant22.find(term2);
						mutant22.replace(mutant22.begin() + pos3, mutant22.begin() + pos3 + term2.length(), new_term12);
						mutants.addMutant(mutant22, "TOFxLRF");
					}
				}
			}
		}
	}
	return mutants;
}


//DORFxLNF---(21)——3个项：相邻的两个发生DORF, 另一个发生LNF
Mutants DnfMutant::generateDORFxLNFCase1doublemutants(string exp, vector<string> oriTerms, vector<string> faultterms, string model){
	Mutants mutants(model);
	Utility uti;
	if (faultterms.size() == 3){
		//term1,term2,term3在原表达式中项的位置
		info terms[3];
		for (int i = 0; i < faultterms.size(); i++){
			terms[i].term = faultterms[i];
		}

		for (int i = 0; i < faultterms.size(); i++){
			terms[i].pos = uti.getTermPosition(oriTerms, terms[i].term);
		}
		sort(terms, terms + 3);

		if (terms[1].pos - terms[0].pos == 1){
			//DORF——term1与term2
			string new_term1 = terms[0].term + terms[1].term;
			string mutant = exp;
			int pos = mutant.find(terms[0].term);
			mutant.replace(mutant.begin() + pos, mutant.begin() + pos + terms[0].term.length() + terms[1].term.length() + 1, new_term1);

			//LNF——term3
			vector<string> literals_in_term = uti.extractCNFstyle(terms[2].term);
			for (int i = 0; i < literals_in_term.size(); i++)
			{
				string new_term2 = terms[2].term;
				string s1 = literals_in_term.at(i);//第i个literal
				string s2 = uti.getLiteralNegation(s1);//获取单词的Negation
				int pos1 = new_term2.find(s1);
				new_term2.replace(pos1, s1.size(), s2);

				string mutant1 = mutant;
				pos = mutant1.find(terms[2].term);
				mutant1.replace(mutant1.begin() + pos, mutant1.begin() + pos + terms[2].term.length(), new_term2);

				mutants.addMutant(mutant1, "DORFxLNFCase1");
			}
		}

		if (terms[2].pos - terms[1].pos == 1){
			//DORF——term2与term3
			string new_term1 = terms[1].term + terms[2].term;
			string mutant = exp;
			int pos = mutant.find(terms[1].term);
			mutant.replace(mutant.begin() + pos, mutant.begin() + pos + terms[1].term.length() + terms[2].term.length() + 1, new_term1);

			//LNF——term1
			vector<string> literals_in_term = uti.extractCNFstyle(terms[0].term);
			for (int i = 0; i < literals_in_term.size(); i++)
			{
				string new_term2 = terms[0].term;
				string s1 = literals_in_term.at(i);//第i个literal
				string s2 = uti.getLiteralNegation(s1);//获取单词的Negation
				int pos1 = new_term2.find(s1);
				new_term2.replace(pos1, s1.size(), s2);

				string mutant1 = mutant;
				pos = mutant1.find(terms[0].term);
				mutant1.replace(mutant1.begin() + pos, mutant1.begin() + pos + terms[0].term.length(), new_term2);

				mutants.addMutant(mutant1, "DORFxLNFCase1");
			}
		}

	}
	return mutants;
}

//DORFxLNF---(22)——两个项（相邻）：一个项LNF，然后再另一个项DORF
Mutants DnfMutant::generateDORFxLNFCase2doublemutants(string exp, vector<string> oriTerms, vector<string> faultterms, string model){
	Mutants mutants(exp);
	Utility uti;

	if (faultterms.size() == 2){
		//term1,term2 在原表达式中项的位置
		info terms[2];
		for (int i = 0; i < faultterms.size(); i++){
			terms[i].term = faultterms[i];
		}

		for (int i = 0; i < faultterms.size(); i++){
			terms[i].pos = uti.getTermPosition(oriTerms, terms[i].term);
		}
		sort(terms, terms + 2);

		if (terms[1].pos - terms[0].pos == 1){//term1与term2相邻

			string term1 = terms[0].term;
			string term2 = terms[1].term;

			vector<string> literals_in_term_1 = uti.extractCNFstyle(term1);
			vector<string> literals_in_term_2 = uti.extractCNFstyle(term2);

			//第1个项 LNF
			for (int i = 0; i < literals_in_term_1.size(); i++)
			{
				string new_term1 = term1;
				string s1 = literals_in_term_1.at(i);//第i个literal
				string s2 = uti.getLiteralNegation(s1);//获取单词的Negation
				int pos1 = new_term1.find(s1);
				new_term1.replace(pos1, s1.size(), s2);

				//第2个项 DORF
				string new_term2 = new_term1 + term2;

				string mutant = exp;
				int pos = mutant.find(term1);
				mutant.replace(mutant.begin() + pos, mutant.begin() + pos + term1.length() + term2.length() + 1, new_term2);
				mutants.addMutant(mutant, "DORFxLNFCase2");
			}

			//------------------------------------------------
			//第2个项LNF
			for (int i = 0; i < literals_in_term_2.size(); i++)
			{
				string new_term1 = term2;
				string s1 = literals_in_term_2.at(i);//第i个literal
				string s2 = uti.getLiteralNegation(s1);//获取单词的Negation
				int pos1 = new_term1.find(s1);
				new_term1.replace(pos1, s1.size(), s2);

				//第1个项LNF
				string new_term2 = term1 + new_term1;

				string mutant = exp;
				int pos = mutant.find(term1);
				mutant.replace(mutant.begin() + pos, mutant.begin() + pos + term1.length() + term2.length() + 1, new_term2);
				mutants.addMutant(mutant, "DORFxLNFCase2");
			}
		}
	}
	return mutants;
}

//DORFxLOF---(23)——3个项：相邻的两个发生DORF, 另一个发生LOF
Mutants DnfMutant::generateDORFxLOFCase1doublemutants(string exp, vector<string> oriTerms, vector<string> faultterms, string model){
	Mutants mutants(model);
	Utility uti;
	if (faultterms.size() == 3){
		//term1,term2,term3在原表达式中项的位置
		info terms[3];
		for (int i = 0; i < faultterms.size(); i++){
			terms[i].term = faultterms[i];
		}

		for (int i = 0; i < faultterms.size(); i++){
			terms[i].pos = uti.getTermPosition(oriTerms, terms[i].term);
		}
		sort(terms, terms + 3);

		string term1 = terms[0].term;
		string term2 = terms[1].term;
		string term3 = terms[2].term;

		//前2个项DORF，第3个项：LNF
		if (terms[1].pos - terms[0].pos == 1){
			//前两个项DORF
			string new_term1 = term1 + term2;
			string mutant = exp;
			int pos = mutant.find(term1);
			mutant.replace(mutant.begin() + pos, mutant.begin() + pos + term1.length() + term2.length() + 1, new_term1);

			//第三个项：LNF
			vector<string> literals_in_term = uti.extractCNFstyle(term3);
			for (int i = 0; i < literals_in_term.size(); i++)
			{
				string new_term2 = term3;
				string s1 = literals_in_term.at(i);
				int pos1 = new_term2.find(s1);
				new_term2.replace(pos1, s1.size(), "");

				string mutant1 = mutant;
				int pos2 = mutant1.find(term3);
				mutant1.replace(mutant1.begin() + pos2, mutant1.begin() + pos2 + term2.length(), new_term2);
				mutants.addMutant(mutant1, "DORFxLOFCase1");
			}
		}

		//---------------------------------------------------------------
		//第1个项：LNF， 后2个项DORF，
		if (terms[2].pos - terms[1].pos == 1){

			//后两个项DORF
			string new_term1 = term2 + term3;
			string mutant = exp;
			int pos = mutant.find(term2);
			mutant.replace(mutant.begin() + pos, mutant.begin() + pos + term2.length() + term3.length() + 1, new_term1);

			//第1个项：LNF
			vector<string> literals_in_term = uti.extractCNFstyle(term1);
			for (int i = 0; i < literals_in_term.size(); i++)
			{
				string new_term2 = term1;
				string s1 = literals_in_term.at(i);
				int pos1 = new_term2.find(s1);
				new_term2.replace(pos1, s1.size(), "");

				string mutant1 = mutant;
				int pos2 = mutant1.find(term1);
				mutant1.replace(mutant1.begin() + pos2, mutant1.begin() + pos2 + term1.length(), new_term2);
				mutants.addMutant(mutant1, "DORFxLOFCase1");
			}
		}
	}
	return mutants;
}

//DORFxLOF---(24)——两个项（相邻）：一个项LOF，然后再另一个项DORF
Mutants DnfMutant::generateDORFxLOFCase2doublemutants(string exp, vector<string> oriTerms, vector<string> faultterms, string model){
	Mutants mutants(exp);
	Utility uti;

	if (faultterms.size() == 2){
		//term1,term2 在原表达式中项的位置
		info terms[2];
		for (int i = 0; i < faultterms.size(); i++){
			terms[i].term = faultterms[i];
		}

		for (int i = 0; i < faultterms.size(); i++){
			terms[i].pos = uti.getTermPosition(oriTerms, terms[i].term);
		}
		sort(terms, terms + 2);


		if (terms[1].pos - terms[0].pos == 1){//term1与term2相邻
			string term1 = terms[0].term;
			string term2 = terms[1].term;

			vector<string> literals_in_term_1 = uti.extractCNFstyle(term1);
			vector<string> literals_in_term_2 = uti.extractCNFstyle(term2);

			//第1个项：LOF
			for (int i = 0; i < literals_in_term_1.size(); i++)
			{
				string new_term1 = term1;
				string s1 = literals_in_term_1.at(i);
				int pos1 = new_term1.find(s1);
				new_term1.replace(pos1, s1.size(), "");

				//第2个项：DORF
				string new_term2 = new_term1 + term2;

				string mutant = exp;
				int pos = mutant.find(term1);
				mutant.replace(mutant.begin() + pos, mutant.begin() + pos + term1.length() + term2.length() + 1, new_term2);
				mutants.addMutant(mutant, "DORFxLOFCase2");
			}

			//-----------------------------------------------------------------
			//第2个项：LOF
			for (int i = 0; i < literals_in_term_2.size(); i++)
			{
				string new_term2 = term2;
				string s1 = literals_in_term_2.at(i);
				int pos1 = new_term2.find(s1);
				new_term2.replace(pos1, s1.size(), "");

				//DORF
				string new_term3 = term1 + new_term2;

				string mutant = exp;
				int pos = mutant.find(term1);
				mutant.replace(mutant.begin() + pos, mutant.begin() + pos + term1.length() + term2.length() + 1, new_term3);
				mutants.addMutant(mutant, "DORFxLOFCase2");
			}
		}
	}
	return mutants;
}

//DORFxLIF---(25)——3个项：相邻的两个发生DORF, 另一个发生LIF
Mutants DnfMutant::generateDORFxLIFCase1doublemutants(string exp, vector<string> oriTerms, vector<string> faultterms, string model){
	
	Mutants mutants(model);
	Utility uti;
	if (faultterms.size() == 3){
		//term1,term2,term3在原表达式中项的位置
		info terms[3];
		for (int i = 0; i < faultterms.size(); i++){
			terms[i].term = faultterms[i];
		}

		for (int i = 0; i < faultterms.size(); i++){
			terms[i].pos = uti.getTermPosition(oriTerms, terms[i].term);
		}
		sort(terms, terms + 3);

		string term1 = terms[0].term;
		string term2 = terms[1].term;
		string term3 = terms[2].term;

		//前2个项DORF，第3个项：LIF
		if (terms[1].pos - terms[0].pos == 1){
			//前两个项DORF
			string new_term1 = term1 + term2;
			string mutant = exp;
			int pos = mutant.find(term1);
			mutant.replace(mutant.begin() + pos, mutant.begin() + pos + term1.length() + term2.length() + 1, new_term1);

			//第三个项LIF
			for (int i = 0; i < model.length(); i++)
			{
				char toReplaceWith = model[i];
				string s21, s22;
				s21 += toReplaceWith;
				s22 += '!';
				s22 += toReplaceWith;
				if (term3.find(toReplaceWith) == string::npos)
				{
					string new_term11 = (term3 + s21);
					string new_term12 = (term3 + s22);

					string mutant11 = mutant;
					int pos2 = mutant11.find(term3);
					mutant11.replace(mutant11.begin() + pos2, mutant11.begin() + pos2 + term3.length(), new_term11);
					mutants.addMutant(mutant11, "DORFxLIFCase1");

					string mutant12 = mutant;
					mutant12.replace(mutant12.begin() + pos2, mutant12.begin() + pos2 + term3.length(), new_term12);
					mutants.addMutant(mutant12, "DORFxLIFCase1");
				}
			}
		}

		//---------------------------------------------------------------
		//第1个项：LIF， 后2个项DORF，
		if (terms[2].pos - terms[1].pos == 1){

			//后两个项DORF
			string new_term1 = term2 + term3;
			string mutant = exp;
			int pos = mutant.find(term2);
			mutant.replace(mutant.begin() + pos, mutant.begin() + pos + term2.length() + term3.length() + 1, new_term1);

			//第1个项LIF
			for (int i = 0; i < model.length(); i++)
			{
				char toReplaceWith = model[i];
				string s21, s22;
				s21 += toReplaceWith;
				s22 += '!';
				s22 += toReplaceWith;
				if (term1.find(toReplaceWith) == string::npos)
				{
					string new_term11 = (term1 + s21);
					string new_term12 = (term1 + s22);

					string mutant11 = mutant;
					int pos2 = mutant11.find(term1);
					mutant11.replace(mutant11.begin() + pos2, mutant11.begin() + pos2 + term1.length(), new_term11);
					mutants.addMutant(mutant11, "DORFxLIFCase1");

					string mutant12 = mutant;
					mutant12.replace(mutant12.begin() + pos2, mutant12.begin() + pos2 + term1.length(), new_term12);
					mutants.addMutant(mutant12, "DORFxLIFCase1");
				}
			}
		}
	}
	return mutants;
}

//DORFxLIF---(26)——两个项（相邻）：一个项LIF，然后再另一个项DORF
Mutants DnfMutant::generateDORFxLIFCase2doublemutants(string exp, vector<string> oriTerms, vector<string> faultterms, string model){
	Mutants mutants(exp);
	Utility uti;

	if (faultterms.size() == 2){
		//term1,term2 在原表达式中项的位置
		info terms[2];
		for (int i = 0; i < faultterms.size(); i++){
			terms[i].term = faultterms[i];
		}

		for (int i = 0; i < faultterms.size(); i++){
			terms[i].pos = uti.getTermPosition(oriTerms, terms[i].term);
		}
		sort(terms, terms + 2);

		if (terms[1].pos - terms[0].pos == 1){//term1与term2相邻
			string term1 = terms[0].term;
			string term2 = terms[1].term;

			vector<string> literals_in_term_1 = uti.extractCNFstyle(term1);
			vector<string> literals_in_term_2 = uti.extractCNFstyle(term2);

			//第1个项：LIF
			for (int i = 0; i < model.length(); i++)
			{
				char toReplaceWith = model[i];
				string s21, s22;
				s21 += toReplaceWith;
				s22 += '!';
				s22 += toReplaceWith;
				if (term1.find(toReplaceWith) == string::npos)
				{
					string new_term11 = (term1 + s21);
					string new_term12 = (term1 + s22);

					//DORF
					string new_term21 = new_term11 + term2;
					string new_term22 = new_term12 + term2;

					string mutant11 = exp;
					int pos = mutant11.find(term1);
					mutant11.replace(mutant11.begin() + pos, mutant11.begin() + pos + term1.length() + term2.length() + 1, new_term21);
					mutants.addMutant(mutant11, "DORFxLIFCase2");

					string mutant12 = exp;
					mutant12.replace(mutant12.begin() + pos, mutant12.begin() + pos + term1.length() + term2.length() + 1, new_term22);
					mutants.addMutant(mutant12, "DORFxLIFCase2");
				}
			}

			//---------------------------------------------------------------------
			//第2个项：LIF
			for (int i = 0; i < model.length(); i++)
			{
				char toReplaceWith = model[i];
				string s21, s22;
				s21 += toReplaceWith;
				s22 += '!';
				s22 += toReplaceWith;
				if (term2.find(toReplaceWith) == string::npos)
				{
					string new_term11 = (term2 + s21);
					string new_term12 = (term2 + s22);

					//DORF
					string new_term21 = term1 + new_term11;
					string new_term22 = term1 + new_term12;

					string mutant21 = exp;
					int pos = mutant21.find(term1);
					mutant21.replace(mutant21.begin() + pos, mutant21.begin() + pos + term1.length() + term2.length() + 1, new_term21);
					mutants.addMutant(mutant21, "DORFxLIFCase2");

					string mutant22 = exp;
					mutant22.replace(mutant22.begin() + pos, mutant22.begin() + pos + term1.length() + term2.length() + 1, new_term22);
					mutants.addMutant(mutant22, "DORFxLIFCase2");
				}
			}
		}
	}
	return mutants;
}

//DORFxLRF---(27)——3个项：相邻的两个发生DORF, 另一个发生LRF
Mutants DnfMutant::generateDORFxLRFCase1doublemutants(string exp, vector<string> oriTerms, vector<string> faultterms, string model){
	Mutants mutants(model);
	Utility uti;
	if (faultterms.size() == 3){
		//term1,term2,term3在原表达式中项的位置
		info terms[3];
		for (int i = 0; i < faultterms.size(); i++){
			terms[i].term = faultterms[i];
		}

		for (int i = 0; i < faultterms.size(); i++){
			terms[i].pos = uti.getTermPosition(oriTerms, terms[i].term);
		}
		sort(terms, terms + 3);

		string term1 = terms[0].term;
		string term2 = terms[1].term;
		string term3 = terms[2].term;

		//前2个项DORF，第3个项：LRF
		if (terms[1].pos - terms[0].pos == 1)
		{
			//前两个项DORF
			string new_term1 = term1 + term2;
			string mutant = exp;
			int pos = mutant.find(term1);
			mutant.replace(mutant.begin() + pos, mutant.begin() + pos + term1.length() + term2.length() + 1, new_term1);

			//第3个项LRF
			vector<string> literals_in_term = uti.extractCNFstyle(term3);
			for (int i = 0; i < model.length(); i++)
			{
				char toReplaceWith = model[i];
				if (term3.find(toReplaceWith) == string::npos)
				{
					string s11, s12;
					s11 += toReplaceWith;//不在当前项中的词literals
					s12 += '!';
					s12 += toReplaceWith;
					for (int k = 0; k < literals_in_term.size(); k++)
					{
						string new_term11 = term3;
						string new_term12 = term3;
						int literal_pos = new_term11.find(literals_in_term[k]);
						new_term11.replace(new_term11.begin() + literal_pos, new_term11.begin() + literal_pos + literals_in_term[k].size(), s11);
						new_term12.replace(new_term12.begin() + literal_pos, new_term12.begin() + literal_pos + literals_in_term[k].size(), s12);

						string mutant11 = mutant;
						int pos2 = mutant11.find(term3);
						mutant11.replace(mutant11.begin() + pos2, mutant11.begin() + pos2 + term3.length(), new_term11);
						mutants.addMutant(mutant11, "DORFxLRFCase1");

						string mutant12 = mutant;
						mutant12.replace(mutant12.begin() + pos2, mutant12.begin() + pos2 + term3.length(), new_term12);
						mutants.addMutant(mutant12, "DORFxLRFCase1");
					}
				}
			}
		}

		//----------------------------------------------------
		//第1个项：LRF， 后2个项DORF，
		if (terms[2].pos - terms[1].pos == 1)
		{
			//后2个项DORF
			string new_term1 = term2 + term3;
			string mutant = exp;
			int pos = mutant.find(term2);
			mutant.replace(mutant.begin() + pos, mutant.begin() + pos + term2.length() + term3.length() + 1, new_term1);

			//第1个项LRF
			vector<string> literals_in_term = uti.extractCNFstyle(term1);
			for (int i = 0; i < model.length(); i++)
			{
				char toReplaceWith = model[i];
				if (term1.find(toReplaceWith) == string::npos)
				{
					string s11, s12;
					s11 += toReplaceWith;//不在当前项中的词literals
					s12 += '!';
					s12 += toReplaceWith;
					for (int k = 0; k < literals_in_term.size(); k++)
					{
						string new_term11 = term1;
						string new_term12 = term1;
						int literal_pos = new_term11.find(literals_in_term[k]);
						new_term11.replace(new_term11.begin() + literal_pos, new_term11.begin() + literal_pos + literals_in_term[k].size(), s11);
						new_term12.replace(new_term12.begin() + literal_pos, new_term12.begin() + literal_pos + literals_in_term[k].size(), s12);

						string mutant11 = mutant;
						int pos2 = mutant11.find(term1);
						mutant11.replace(mutant11.begin() + pos2, mutant11.begin() + pos2 + term1.length(), new_term11);
						mutants.addMutant(mutant11, "DORFxLRFCase1");

						string mutant12 = mutant;
						mutant12.replace(mutant12.begin() + pos2, mutant12.begin() + pos2 + term1.length(), new_term12);
						mutants.addMutant(mutant12, "DORFxLRFCase1");
					}
				}
			}
		}
	}
	return mutants;
}

//DORFxLRF---(28)——两个项（相邻）：一个项LRF，然后再另一个项DORF
Mutants DnfMutant::generateDORFxLRFCase2doublemutants(string exp, vector<string> oriTerms, vector<string> faultterms, string model)
{
	Mutants mutants(exp);
	Utility uti;

	if (faultterms.size() == 2){
		//term1,term2 在原表达式中项的位置
		info terms[2];
		for (int i = 0; i < faultterms.size(); i++){
			terms[i].term = faultterms[i];
		}

		for (int i = 0; i < faultterms.size(); i++){
			terms[i].pos = uti.getTermPosition(oriTerms, terms[i].term);
		}
		sort(terms, terms + 2);

		if (terms[1].pos - terms[0].pos == 1){//term1与term2相邻
			string term1 = terms[0].term;
			string term2 = terms[1].term;

			vector<string> literals_in_term_1 = uti.extractCNFstyle(term1);
			vector<string> literals_in_term_2 = uti.extractCNFstyle(term2);

			//第一个项：LRF
			for (int i = 0; i < model.length(); i++)
			{
				char toReplaceWith = model[i];
				if (term1.find(toReplaceWith) == string::npos)
				{
					string s11, s12;
					s11 += toReplaceWith;//不在当前项中的词literals
					s12 += '!';
					s12 += toReplaceWith;
					for (int k = 0; k < literals_in_term_1.size(); k++)
					{
						string new_term11 = term1;
						string new_term12 = term1;
						int literal_pos = new_term11.find(literals_in_term_1[k]);
						new_term11.replace(new_term11.begin() + literal_pos, new_term11.begin() + literal_pos + literals_in_term_1[k].size(), s11);
						new_term12.replace(new_term12.begin() + literal_pos, new_term12.begin() + literal_pos + literals_in_term_1[k].size(), s12);

						//第二个项：DORF
						string new_term21 = new_term11 + term2;
						string new_term22 = new_term12 + term2;

						string mutant11 = exp;
						int pos = mutant11.find(term1);
						mutant11.replace(mutant11.begin() + pos, mutant11.begin() + pos + term1.length() + term2.length() + 1, new_term21);
						mutants.addMutant(mutant11, "DORFxLRFCase2");

						string mutant12 = exp;
						mutant12.replace(mutant12.begin() + pos, mutant12.begin() + pos + term1.length() + term2.length() + 1, new_term22);
						mutants.addMutant(mutant12, "DORFxLRFCase2");
					}
				}
			}

			//-------------------------------------------------------------
			//第二个项：LRF
			for (int i = 0; i < model.length(); i++)
			{
				char toReplaceWith = model[i];
				if (term2.find(toReplaceWith) == string::npos)
				{
					string s11, s12;
					s11 += toReplaceWith;//不在当前项中的词literals
					s12 += '!';
					s12 += toReplaceWith;
					for (int k = 0; k < literals_in_term_2.size(); k++)
					{
						string new_term11 = term2;
						string new_term12 = term2;
						int literal_pos = new_term11.find(literals_in_term_2[k]);
						new_term11.replace(new_term11.begin() + literal_pos, new_term11.begin() + literal_pos + literals_in_term_2[k].size(), s11);
						new_term12.replace(new_term12.begin() + literal_pos, new_term12.begin() + literal_pos + literals_in_term_2[k].size(), s12);

						string new_term21 = term1 + new_term11;
						string new_term22 = term1 + new_term12;

						string mutant21 = exp;
						int pos = mutant21.find(term1);
						mutant21.replace(mutant21.begin() + pos, mutant21.begin() + pos + term1.length() + term2.length() + 1, new_term21);
						mutants.addMutant(mutant21, "DORFxLRFCase2");

						string mutant22 = exp;
						mutant22.replace(mutant22.begin() + pos, mutant22.begin() + pos + term1.length() + term2.length() + 1, new_term22);
						mutants.addMutant(mutant22, "DORFxLRFCase2");
					}
				}
			}
		}
	}
	return mutants;
}



//CORFxLNF---(29)——两个项：一个项发生CORF，另一个发生LNF
Mutants DnfMutant::generateCORFxLNFCase1doublemutants(string exp, vector<string> oriTerms, vector<string> terms, string model){
	Mutants mutants(exp);
	Utility uti;

	if (terms.size() >= 2)
	{
		for (int m = 0; m < terms.size() - 1; m++){
			for (int n = m + 1; n < terms.size(); n++){
				string term1 = terms[m];
				string term2 = terms[n];

				vector<string> literals_in_term_1 = uti.extractCNFstyle(term1);
				vector<string> literals_in_term_2 = uti.extractCNFstyle(term2);

				//term1发生CORF， term2发生LNF
				for (int j = 1; j < literals_in_term_1.size(); j++)
				{
					int pos_insert_operator_or_1 = term1.find(literals_in_term_1[j]);
					string new_term1 = term1;
					new_term1.insert(pos_insert_operator_or_1, "+");

					string mutant1 = exp;
					int pos = mutant1.find(term1);
					mutant1.replace(mutant1.begin() + pos, mutant1.begin() + pos + term1.length(), new_term1);

					for (int i = 0; i < literals_in_term_2.size(); i++)
					{
						string new_term2 = term2;
						string s1 = literals_in_term_2.at(i);//第i个literal
						string s2 = uti.getLiteralNegation(s1);//获取单词的Negation
						int pos1 = new_term2.find(s1);
						new_term2.replace(pos1, s1.size(), s2);

						string mutant11 = mutant1;
						int pos2 = mutant11.find(term2);
						mutant11.replace(mutant11.begin() + pos2, mutant11.begin() + pos2 + term2.length(), new_term2);
						mutants.addMutant(mutant11, "CORFxLNFCase1");
					}
				}

				//--------------------------------------------------------
				//term2发生CORF， term1发生LNF
				for (int j = 1; j < literals_in_term_2.size(); j++)
				{
					int pos_insert_operator_or_1 = term2.find(literals_in_term_2[j]);
					string new_term2 = term2;
					new_term2.insert(pos_insert_operator_or_1, "+");

					string mutant1 = exp;
					int pos = mutant1.find(term2);
					mutant1.replace(mutant1.begin() + pos, mutant1.begin() + pos + term2.length(), new_term2);

					//term1——LNF
					for (int i = 0; i < literals_in_term_1.size(); i++)
					{
						string new_term1 = term1;
						string s1 = literals_in_term_1.at(i);//第i个literal
						string s2 = uti.getLiteralNegation(s1);//获取单词的Negation
						int pos1 = new_term1.find(s1);
						new_term1.replace(pos1, s1.size(), s2);

						string mutant11 = mutant1;
						int pos2 = mutant11.find(term1);
						mutant11.replace(mutant11.begin() + pos2, mutant11.begin() + pos2 + term1.length(), new_term1);
						mutants.addMutant(mutant11, "CORFxLNFCase1");
					}
				}
			}
		}
	}
	return mutants;
}

//CORFxLNF---(30)——一个项：先CORF分成两个项，其中一个项再发生LNF
Mutants DnfMutant::generateCORFxLNFCase2doublemutants(string exp, vector<string> oriTerms, vector<string> terms, string model){
	Mutants mutants(exp);
	Utility uti;

	for (int m = 0; m < terms.size(); m++){
		string term = terms[m];
		int pos = exp.find(term);

		string new_term1, new_term2;
		vector<string> literals_in_term = uti.extractCNFstyle(term);
		for (int j = 1; j < literals_in_term.size(); j++)
		{
			int pos_insert_operator_or = term.find(literals_in_term[j]);
			new_term1 = term.substr(0, pos_insert_operator_or);
			new_term2 = term.substr(pos_insert_operator_or, term.length());

			//LNF————new_term1发生LNF
			vector<string> literals_in_term_1 = uti.extractCNFstyle(new_term1);
			for (int i = 0; i < literals_in_term_1.size(); i++)
			{
				string new_term3 = new_term1;
				string s1 = literals_in_term_1.at(i);//第i个literal
				string s2 = uti.getLiteralNegation(s1);//获取单词的Negation
				int pos1 = new_term3.find(s1);
				new_term3.replace(pos1, s1.size(), s2);

				string new_term4 = new_term3 + "+" + new_term2;

				string mutant = exp;
				mutant.replace(mutant.begin() + pos, mutant.begin() + pos + term.size(), new_term4);
				mutants.addMutant(mutant, "CORFxLNFCase2");
			}

			//LNF————new_term2发生LNF
			vector<string> literals_in_term_2 = uti.extractCNFstyle(new_term2);
			for (int i = 0; i < literals_in_term_2.size(); i++)
			{
				string new_term3 = new_term2;
				string s1 = literals_in_term_2.at(i);//第i个literal
				string s2 = uti.getLiteralNegation(s1);//获取单词的Negation
				int pos1 = new_term3.find(s1);
				new_term3.replace(pos1, s1.size(), s2);

				string new_term4 = new_term1 + "+" + new_term3;

				string mutant = exp;
				mutant.replace(mutant.begin() + pos, mutant.begin() + pos + term.size(), new_term4);
				mutants.addMutant(mutant, "CORFxLNFCase2");
			}
		}
	}
	return mutants;
}

//CORFxLOF---(31)——两个项：一个项发生CORF，另一个发生LOF
Mutants DnfMutant::generateCORFxLOFCase1doublemutants(string exp, vector<string> oriTerms, vector<string> terms, string model){
	Mutants mutants(exp);
	Utility uti;
	
	if (terms.size() >= 2)
	{
		for (int m = 0; m < terms.size() - 1; m++){
			for (int n = m + 1; n < terms.size(); n++){
				string term1 = terms[m];
				string term2 = terms[n];

				if (term1.length() >= 2 && term2.length() >= 2){
					vector<string> literals_in_term_1 = uti.extractCNFstyle(term1);
					vector<string> literals_in_term_2 = uti.extractCNFstyle(term2);

					//term1发生CORF， term2发生LOF
					for (int j = 1; j < literals_in_term_1.size(); j++)
					{
						int pos_insert_operator_or_1 = term1.find(literals_in_term_1[j]);
						string new_term1 = term1;
						new_term1.insert(pos_insert_operator_or_1, "+");

						string mutant1 = exp;
						int pos = mutant1.find(term1);
						mutant1.replace(mutant1.begin() + pos, mutant1.begin() + pos + term1.length(), new_term1);

						//第2项——LOF
						for (int i = 0; i < literals_in_term_2.size(); i++)
						{
							string new_term2 = term2;
							string s1 = literals_in_term_2.at(i);
							int pos1 = new_term2.find(s1);
							new_term2.replace(pos1, s1.size(), "");

							string mutant11 = mutant1;
							int pos2 = mutant11.find(term2);
							mutant11.replace(mutant11.begin() + pos2, mutant11.begin() + pos2 + term2.length(), new_term2);
							mutants.addMutant(mutant11, "CORFxLOFCase1");
						}
					}

					//-------------------------------------------------------------------
					//term2发生CORF， term1发生LOF
					for (int j = 1; j < literals_in_term_2.size(); j++)
					{
						int pos_insert_operator_or_1 = term2.find(literals_in_term_2[j]);
						string new_term2 = term2;
						new_term2.insert(pos_insert_operator_or_1, "+");

						string mutant1 = exp;
						int pos = mutant1.find(term2);
						mutant1.replace(mutant1.begin() + pos, mutant1.begin() + pos + term2.length(), new_term2);

						//第1项——LOF
						for (int i = 0; i < literals_in_term_1.size(); i++)
						{
							string new_term1 = term1;
							string s1 = literals_in_term_1.at(i);
							int pos1 = new_term1.find(s1);
							new_term1.replace(pos1, s1.size(), "");

							string mutant11 = mutant1;
							int pos2 = mutant11.find(term1);
							mutant11.replace(mutant11.begin() + pos2, mutant11.begin() + pos2 + term1.length(), new_term1);
							mutants.addMutant(mutant11, "CORFxLOFCase1");
						}
					}
				}
			}
		}
	}
	return mutants;
}

//CORFxLOF---(32)——一个项：先CORF分成两个项，其中一个项再发生LOF
Mutants DnfMutant::generateCORFxLOFCase2doublemutants(string exp, vector<string> oriTerms, vector<string> terms, string model){
	Mutants mutants(exp);
	Utility uti;

	for (int m = 0; m < terms.size(); m++){
		string term = terms[m];

		if (term.length() >= 2){
			int pos = exp.find(term);

			string new_term1, new_term2;
			vector<string> literals_in_term = uti.extractCNFstyle(term);
			for (int j = 1; j < literals_in_term.size(); j++)
			{
				int pos_insert_operator_or = term.find(literals_in_term[j]);
				new_term1 = term.substr(0, pos_insert_operator_or);
				new_term2 = term.substr(pos_insert_operator_or, term.length());

				//LOF————new_term1发生LOF
				vector<string> literals_in_term_1 = uti.extractCNFstyle(new_term1);
				for (int i = 0; i < literals_in_term_1.size(); i++)
				{
					string new_term3 = new_term1;
					string s1 = literals_in_term_1.at(i);
					int pos1 = new_term3.find(s1);
					new_term3.replace(pos1, s1.size(), "");

					string new_term4 = new_term3 + "+" + new_term2;

					string mutant = exp;
					mutant.replace(mutant.begin() + pos, mutant.begin() + pos + term.size(), new_term4);
					mutants.addMutant(mutant, "CORFxLOFCase2");
				}

				//LOF————new_term2s发生LOF
				vector<string> literals_in_term_2 = uti.extractCNFstyle(new_term2);
				for (int i = 0; i < literals_in_term_2.size(); i++)
				{
					string new_term3 = new_term2;
					string s1 = literals_in_term_2.at(i);//第i个literal
					int pos1 = new_term3.find(s1);
					new_term3.replace(pos1, s1.size(), "");

					string new_term4 = new_term1 + "+" + new_term3;

					string mutant = exp;
					mutant.replace(mutant.begin() + pos, mutant.begin() + pos + term.size(), new_term4);
					mutants.addMutant(mutant, "CORFxLOFCase2");
				}
			}
		}
	}
	return mutants;
}

//CORFxLIF---(33)——两个项：一个项发生CORF，另一个项发生LIF
Mutants DnfMutant::generateCORFxLIFCase1doublemutants(string exp, vector<string> oriTerms, vector<string> terms, string model){
	Mutants mutants(exp);
	Utility uti;
	if (terms.size() >= 2)
	{
		for (int m = 0; m < terms.size() - 1; m++){
			for (int n = m + 1; n < terms.size(); n++){
				string term1 = terms[m];
				string term2 = terms[n];

				vector<string> literals_in_term_1 = uti.extractCNFstyle(term1);
				vector<string> literals_in_term_2 = uti.extractCNFstyle(term2);

				//term1发生CORF， term2发生LIF
				if (term1.length() > 1){
					for (int j = 1; j < literals_in_term_1.size(); j++)
					{
						int pos_insert_operator_or_1 = term1.find(literals_in_term_1[j]);
						string new_term1 = term1;
						new_term1.insert(pos_insert_operator_or_1, "+");

						string mutant1 = exp;
						int pos = mutant1.find(term1);
						mutant1.replace(mutant1.begin() + pos, mutant1.begin() + pos + term1.length(), new_term1);

						//term2——LIF
						for (int i = 0; i < model.length(); i++)
						{
							char toReplaceWith = model[i];
							string s21, s22;
							s21 += toReplaceWith;
							s22 += '!';
							s22 += toReplaceWith;
							if (term2.find(toReplaceWith) == string::npos)
							{
								string new_term11 = (term2 + s21);
								string new_term12 = (term2 + s22);

								string mutant11 = mutant1;
								int pos21 = mutant11.find(term2);
								mutant11.replace(mutant11.begin() + pos21, mutant11.begin() + pos21 + term2.length(), new_term11);
								mutants.addMutant(mutant11, "CORFxLIFCase1");

								string mutant12 = mutant1;
								int pos22 = mutant12.find(term2);
								mutant12.replace(mutant12.begin() + pos22, mutant12.begin() + pos22 + term2.length(), new_term12);
								mutants.addMutant(mutant12, "CORFxLIFCase1");
							}
						}
					}
				}

				//----------------------------------------------------
				//term2发生CORF， term1发生LIF
				if (term2.length() > 1){
					for (int j = 1; j < literals_in_term_2.size(); j++)
					{
						int pos_insert_operator_or_1 = term2.find(literals_in_term_2[j]);
						string new_term2 = term2;
						new_term2.insert(pos_insert_operator_or_1, "+");

						string mutant1 = exp;
						int pos = mutant1.find(term2);
						mutant1.replace(mutant1.begin() + pos, mutant1.begin() + pos + term2.length(), new_term2);

						//term1——LIF
						for (int i = 0; i < model.length(); i++)
						{
							char toReplaceWith = model[i];
							string s21, s22;
							s21 += toReplaceWith;
							s22 += '!';
							s22 += toReplaceWith;
							if (term1.find(toReplaceWith) == string::npos)
							{
								string new_term11 = (term1 + s21);
								string new_term12 = (term1 + s22);

								string mutant11 = mutant1;
								int pos21 = mutant11.find(term1);
								mutant11.replace(mutant11.begin() + pos21, mutant11.begin() + pos21 + term1.length(), new_term11);
								mutants.addMutant(mutant11, "CORFxLIFCase1");

								string mutant12 = mutant1;
								mutant12.replace(mutant12.begin() + pos21, mutant12.begin() + pos21 + term1.length(), new_term12);
								mutants.addMutant(mutant12, "CORFxLIFCase1");
							}
						}
					}
				}
			}
		}
	}
	return mutants;
}

//CORFxLIF---(34)——一个项：先CORF分成两个项，其中一个项再发生LOF（插入的词，不能在原term中存在）
Mutants DnfMutant::generateCORFxLIFCase2doublemutants(string exp, vector<string> oriTerms, vector<string> terms, string model){
	Mutants mutants(exp);
	Utility uti;

	for (int m = 0; m < terms.size(); m++){
		string term = terms[m];
		int pos = exp.find(term);

		string new_term1, new_term2;
		vector<string> literals_in_term = uti.extractCNFstyle(term);
		for (int j = 1; j < literals_in_term.size(); j++)
		{
			int pos_insert_operator_or = term.find(literals_in_term[j]);
			new_term1 = term.substr(0, pos_insert_operator_or);
			new_term2 = term.substr(pos_insert_operator_or, term.length());

			//new_term1——LIF
			for (int i = 0; i < model.length(); i++)
			{
				char toReplaceWith = model[i];
				string s21, s22;
				s21 += toReplaceWith;
				s22 += '!';
				s22 += toReplaceWith;
				if (term.find(toReplaceWith) == string::npos)
				{
					string new_term11 = (new_term1 + s21);
					string new_term12 = (new_term1 + s22);

					string new_term31 = new_term11 + "+" + new_term2;
					string new_term32 = new_term12 + "+" + new_term2;

					string mutant = exp;
					mutant.replace(mutant.begin() + pos, mutant.begin() + pos + term.size(), new_term31);
					mutants.addMutant(mutant, "CORFxLIFCase2");

					mutant = exp;
					mutant.replace(mutant.begin() + pos, mutant.begin() + pos + term.size(), new_term32);
					mutants.addMutant(mutant, "CORFxLIFCase2");
				}
			}

			//new_term2——LIF
			for (int i = 0; i < model.length(); i++)
			{
				char toReplaceWith = model[i];
				string s21, s22;
				s21 += toReplaceWith;
				s22 += '!';
				s22 += toReplaceWith;
				if (term.find(toReplaceWith) == string::npos)
				{
					string new_term11 = (new_term2 + s21);
					string new_term12 = (new_term2 + s22);

					string new_term31 = new_term11 + "+" + new_term2;
					string new_term32 = new_term12 + "+" + new_term2;

					string mutant = exp;
					mutant.replace(mutant.begin() + pos, mutant.begin() + pos + term.size(), new_term31);
					mutants.addMutant(mutant, "CORFxLIFCase2");

					mutant = exp;
					mutant.replace(mutant.begin() + pos, mutant.begin() + pos + term.size(), new_term32);
					mutants.addMutant(mutant, "CORFxLIFCase2");
				}
			}
		}
	}
	return mutants;
}

//CORFxLRF---(35)——两个项：一个项发生CORF，另一个项发生LRF
Mutants DnfMutant::generateCORFxLRFCase1doublemutants(string exp, vector<string> oriTerms, vector<string> terms, string model){
	Mutants mutants(exp);
	Utility uti;

	if (terms.size() >= 2)
	{
		for (int m = 0; m < terms.size() - 1; m++){
			for (int n = m + 1; n < terms.size(); n++){
				string term1 = terms[m];
				string term2 = terms[n];

				vector<string> literals_in_term_1 = uti.extractCNFstyle(term1);
				vector<string> literals_in_term_2 = uti.extractCNFstyle(term2);

				//term1发生CORF， term2发生LRF
				if (term1.length() > 1 && term2.length() > 1)
				{

					for (int j = 1; j < literals_in_term_1.size(); j++)
					{
						int pos_insert_operator_or_1 = term1.find(literals_in_term_1[j]);
						string new_term1 = term1;
						new_term1.insert(pos_insert_operator_or_1, "+");

						string mutant1 = exp;
						int pos = mutant1.find(term1);
						mutant1.replace(mutant1.begin() + pos, mutant1.begin() + pos + term1.length(), new_term1);

						//term2——LRF
						for (int i = 0; i < model.length(); i++)
						{
							char toReplaceWith = model[i];
							if (term2.find(toReplaceWith) == string::npos)
							{
								string s11, s12;
								s11 += toReplaceWith;//不在当前项中的词literals
								s12 += '!';
								s12 += toReplaceWith;
								for (int k = 0; k < literals_in_term_2.size(); k++)
								{
									string new_term11 = term2;
									string new_term12 = term2;
									int literal_pos = new_term11.find(literals_in_term_2[k]);
									new_term11.replace(new_term11.begin() + literal_pos, new_term11.begin() + literal_pos + literals_in_term_2[k].size(), s11);
									new_term12.replace(new_term12.begin() + literal_pos, new_term12.begin() + literal_pos + literals_in_term_2[k].size(), s12);

									string mutant11 = mutant1;
									int pos21 = mutant11.find(term2);
									mutant11.replace(mutant11.begin() + pos21, mutant11.begin() + pos21 + term2.length(), new_term11);
									mutants.addMutant(mutant11, "CORFxLRFCase1");

									string mutant12 = mutant1;
									mutant12.replace(mutant12.begin() + pos21, mutant12.begin() + pos21 + term2.length(), new_term12);
									mutants.addMutant(mutant12, "CORFxLRFCase1");

								}
							}
						}
					}
					//--------------------------------------------------------
					//term2发生CORF， term1发生LRF
					for (int j = 1; j < literals_in_term_2.size(); j++)
					{
						int pos_insert_operator_or_1 = term2.find(literals_in_term_2[j]);
						string new_term2 = term2;
						new_term2.insert(pos_insert_operator_or_1, "+");

						string mutant1 = exp;
						int pos = mutant1.find(term2);
						mutant1.replace(mutant1.begin() + pos, mutant1.begin() + pos + term2.length(), new_term2);

						//term1——LRF
						for (int i = 0; i < model.length(); i++)
						{
							char toReplaceWith = model[i];
							if (term1.find(toReplaceWith) == string::npos)
							{
								string s11, s12;
								s11 += toReplaceWith;//不在当前项中的词literals
								s12 += '!';
								s12 += toReplaceWith;
								for (int k = 0; k < literals_in_term_1.size(); k++)
								{
									string new_term11 = term1;
									string new_term12 = term1;
									int literal_pos = new_term11.find(literals_in_term_1[k]);
									new_term11.replace(new_term11.begin() + literal_pos, new_term11.begin() + literal_pos + literals_in_term_1[k].size(), s11);
									new_term12.replace(new_term12.begin() + literal_pos, new_term12.begin() + literal_pos + literals_in_term_1[k].size(), s12);

									string mutant11 = mutant1;
									int pos21 = mutant11.find(term1);
									mutant11.replace(mutant11.begin() + pos21, mutant11.begin() + pos21 + term1.length(), new_term11);
									mutants.addMutant(mutant11, "CORFxLRFCase1");

									string mutant12 = mutant1;
									mutant12.replace(mutant12.begin() + pos21, mutant12.begin() + pos21 + term1.length(), new_term12);
									mutants.addMutant(mutant12, "CORFxLRFCase1");
								}
							}
						}
					}
				}
			}
		}
	}
	return mutants;
}

//CORFxLRF---(36)——一个项：先CORF分成两个项，其中一个项再发生LRF（替换的词，不能在原term中存在）
Mutants DnfMutant::generateCORFxLRFCase2doublemutants(string exp, vector<string> oriTerms, vector<string> terms, string model){
	Mutants mutants(exp);
	Utility uti;

	for (int m = 0; m < terms.size(); m++){
		string term = terms[m];
		int pos = exp.find(term);

		string new_term1, new_term2;
		vector<string> literals_in_term = uti.extractCNFstyle(term);
		for (int j = 1; j < literals_in_term.size(); j++)
		{
			int pos_insert_operator_or = term.find(literals_in_term[j]);
			new_term1 = term.substr(0, pos_insert_operator_or);
			new_term2 = term.substr(pos_insert_operator_or, term.length());

			//new_term1——LRF
			for (int i = 0; i < model.length(); i++)
			{
				char toReplaceWith = model[i];
				if (term.find(toReplaceWith) == string::npos)
				{
					string s11, s12;
					s11 += toReplaceWith;//不在当前项中的词literals
					s12 += '!';
					s12 += toReplaceWith;

					vector<string> literals_in_term_1 = uti.extractCNFstyle(new_term1);
					for (int k = 0; k < literals_in_term_1.size(); k++)
					{
						string new_term11 = new_term1;
						string new_term12 = new_term1;

						int literal_pos = new_term11.find(literals_in_term_1[k]);
						new_term11.replace(new_term11.begin() + literal_pos, new_term11.begin() + literal_pos + literals_in_term_1[k].size(), s11);
						new_term12.replace(new_term12.begin() + literal_pos, new_term12.begin() + literal_pos + literals_in_term_1[k].size(), s12);

						string new_term31 = new_term11 + "+" + new_term2;
						string new_term32 = new_term12 + "+" + new_term2;

						string mutant = exp;
						mutant.replace(mutant.begin() + pos, mutant.begin() + pos + term.size(), new_term31);
						mutants.addMutant(mutant, "CORFxLRFCase2");

						mutant = exp;
						mutant.replace(mutant.begin() + pos, mutant.begin() + pos + term.size(), new_term32);
						mutants.addMutant(mutant, "CORFxLRFCase2");
					}
				}
			}

			//new_term2——LRF
			for (int i = 0; i < model.length(); i++)
			{
				char toReplaceWith = model[i];
				if (term.find(toReplaceWith) == string::npos)
				{
					string s11, s12;
					s11 += toReplaceWith;//不在当前项中的词literals
					s12 += '!';
					s12 += toReplaceWith;

					vector<string> literals_in_term_2 = uti.extractCNFstyle(new_term2);
					for (int k = 0; k < literals_in_term_2.size(); k++)
					{
						string new_term11 = new_term2;
						string new_term12 = new_term2;

						int literal_pos = new_term2.find(literals_in_term_2[k]);
						new_term11.replace(new_term11.begin() + literal_pos, new_term11.begin() + literal_pos + literals_in_term_2[k].size(), s11);
						new_term12.replace(new_term12.begin() + literal_pos, new_term12.begin() + literal_pos + literals_in_term_2[k].size(), s12);


						string new_term31 = new_term11 + "+" + new_term2;
						string new_term32 = new_term12 + "+" + new_term2;

						string mutant = exp;
						mutant.replace(mutant.begin() + pos, mutant.begin() + pos + term.size(), new_term31);
						mutants.addMutant(mutant, "CORFxLRFCase2");

						mutant = exp;
						mutant.replace(mutant.begin() + pos, mutant.begin() + pos + term.size(), new_term32);
						mutants.addMutant(mutant, "CORFxLRFCase2");
					}
				}
			}
		}
	}
	return mutants;
}

//===有序===

//TNFxLIF---(52)——一个项：先发生TNF，再发生LIF
Mutants DnfMutant::generateTNFxLIFWithOrderdoublemutants(string exp, vector<string> oriTerms, vector<string> terms, string model){
	Mutants mutants(exp);
	Utility uti;

	for (int m = 0; m < terms.size(); m++){
		string term = terms[m];
		int pos = exp.find(term);

		string new_term1 = "!(" + term + ")";
		//LIF
		for (int i = 0; i < model.length(); i++)
		{
			char toReplaceWith = model[i];
			string s21, s22;
			s21 += toReplaceWith;
			s22 += '!';
			s22 += toReplaceWith;
			if (new_term1.find(toReplaceWith) == string::npos)
			{
				string new_term11 = (new_term1 + s21);
				string new_term12 = (new_term1 + s22);

				string mutant = exp;
				mutant.replace(mutant.begin() + pos, mutant.begin() + pos + term.size(), new_term11);
				mutants.addMutant(mutant, "TNFxLIFWithOrder");

				mutant = exp;
				mutant.replace(mutant.begin() + pos, mutant.begin() + pos + term.size(), new_term12);
				mutants.addMutant(mutant, "TNFxLIFWithOrder");
			}
		}
	}
	return mutants;
}

//CORFxLIFWithOrderCase1---(73)——一个项：先发生CORF，然后第二个项的第一个词被插入第一个项
Mutants DnfMutant::generateCORFxLIFWithOrderCase1doublemutants(string exp, vector<string> oriTerms, vector<string> terms, string model){
	Mutants mutants(exp);
	Utility uti;

	for (int m = 0; m < terms.size(); m++){
		string term = terms[m];
		int pos = exp.find(term);

		string new_term1, new_term2;
		vector<string> literals_in_term = uti.extractCNFstyle(term);
		for (int j = 1; j < literals_in_term.size(); j++)
		{
			int pos_insert_operator_or = term.find(literals_in_term[j]);
			new_term1 = term.substr(0, pos_insert_operator_or);
			new_term2 = term.substr(pos_insert_operator_or, term.length());

			//第二个项的第一个词被插入第一个项
			vector<string> literals_in_term_2 = uti.extractCNFstyle(new_term2);
			string s1 = literals_in_term_2.at(0);
			string new_term11 = (new_term1 + s1);

			string new_term3 = new_term11 + "+" + new_term2;

			string mutant = exp;
			mutant.replace(mutant.begin() + pos, mutant.begin() + pos + term.size(), new_term3);
			mutants.addMutant(mutant, "CORFxLIFWithOrderCase1");


			//LIF————new_term2的第一个词插入new_term1
			/*
			for (int i = 0; i < model.length(); i++)
			{
			char toReplaceWith = model[i];
			string s21, s22;
			s21 += toReplaceWith;
			s22 += '!';
			s22 += toReplaceWith;
			if (term.find(toReplaceWith) == string::npos)
			{
			string new_term11 = (new_term1 + s21);
			string new_term12 = (new_term1 + s22);
			mutants.addMutant(new_term11, new_term2, "CORFxLIFCase1");
			mutants.addMutant(new_term12, new_term2, "CORFxLIFCase1");
			}
			}
			*/
		}
	}
	return mutants;
}

//CORFxLIFWithOrderCase2---(74)——一个项：先发生CORF，然后第二个项的第一个词的非被插入第一个项________不用管是否存在？？？
Mutants DnfMutant::generateCORFxLIFWithOrderCase2doublemutants(string exp, vector<string> oriTerms, vector<string> terms, string model){
	Mutants mutants(exp);
	Utility uti;

	for (int m = 0; m < terms.size(); m++){
		string term = terms[m];
		int pos = exp.find(term);

		string new_term1, new_term2;
		vector<string> literals_in_term = uti.extractCNFstyle(term);
		for (int j = 1; j < literals_in_term.size(); j++)
		{
			int pos_insert_operator_or = term.find(literals_in_term[j]);
			new_term1 = term.substr(0, pos_insert_operator_or);
			new_term2 = term.substr(pos_insert_operator_or, term.length());

			//第二个项的第一个词的非被插入第一个项
			vector<string> literals_in_term_2 = uti.extractCNFstyle(new_term2);
			string s1 = literals_in_term_2.at(0);
			string s2 = uti.getLiteralNegation(s1);//获取单词的Negation
			string new_term11 = (new_term1 + s2);

			string new_term3 = new_term11 + "+" + new_term2;

			string mutant = exp;
			mutant.replace(mutant.begin() + pos, mutant.begin() + pos + term.size(), new_term3);
			mutants.addMutant(mutant, "CORFxLIFWithOrderCase2");
		}
	}
	return mutants;

}

//CORFxLRFWithOrderCase1---(77)——一个项：先发生CORF，然后第一个项的某个词被第二个项中的第一个词替换________不用管是否存在？？？
Mutants DnfMutant::generateCORFxLRFWithOrderCase1doublemutants(string exp, vector<string> oriTerms, vector<string> terms, string model){
	Mutants mutants(exp);
	Utility uti;

	for (int m = 0; m < terms.size(); m++){
		string term = terms[m];
		int pos = exp.find(term);

		string new_term1, new_term2;
		vector<string> literals_in_term = uti.extractCNFstyle(term);
		for (int j = 1; j < literals_in_term.size(); j++)
		{
			int pos_insert_operator_or = term.find(literals_in_term[j]);
			new_term1 = term.substr(0, pos_insert_operator_or);
			new_term2 = term.substr(pos_insert_operator_or, term.length());

			//new_term1——LRF
			vector<string> literals_in_term_1 = uti.extractCNFstyle(new_term1);
			vector<string> literals_in_term_2 = uti.extractCNFstyle(new_term2);
			for (int k = 0; k < literals_in_term_1.size(); k++)
			{
				string s11 = literals_in_term_2.at(0);
				string new_term11 = new_term1;
				int literal_pos = new_term11.find(literals_in_term_1[k]);
				new_term11.replace(new_term11.begin() + literal_pos, new_term11.begin() + literal_pos + literals_in_term_1[k].size(), s11);

				string new_term3 = new_term11 + "+" + new_term2;

				string mutant = exp;
				mutant.replace(mutant.begin() + pos, mutant.begin() + pos + term.size(), new_term3);
				mutants.addMutant(mutant, "CORFxLRFWithOrderCase1");
			}
		}
	}
	return mutants;
}

//CORFxLRFWithOrderCase2---(78)——一个项：先发生CORF，然后第一个项的某个词被第二个项中的第一个词的非替换
Mutants DnfMutant::generateCORFxLRFWithOrderCase2doublemutants(string exp, vector<string> oriTerms, vector<string> terms, string model){
	Mutants mutants(exp);
	Utility uti;

	for (int m = 0; m < terms.size(); m++){
		string term = terms[m];
		int pos = exp.find(term);

		string new_term1, new_term2;
		vector<string> literals_in_term = uti.extractCNFstyle(term);
		for (int j = 1; j < literals_in_term.size(); j++)
		{
			int pos_insert_operator_or = term.find(literals_in_term[j]);
			new_term1 = term.substr(0, pos_insert_operator_or);
			new_term2 = term.substr(pos_insert_operator_or, term.length());

			//LRF————new_term1发生LRF
			vector<string> literals_in_term_1 = uti.extractCNFstyle(new_term1);
			vector<string> literals_in_term_2 = uti.extractCNFstyle(new_term2);
			for (int k = 0; k < literals_in_term_1.size(); k++)
			{
				string s1 = literals_in_term_2.at(0);
				string s2 = uti.getLiteralNegation(s1);//获取单词的Negation
				string new_term11 = new_term1;
				int literal_pos = new_term11.find(literals_in_term_1[k]);
				new_term11.replace(new_term11.begin() + literal_pos, new_term11.begin() + literal_pos + literals_in_term_1[k].size(), s2);

				string new_term3 = new_term11 + "+" + new_term2;

				string mutant = exp;
				mutant.replace(mutant.begin() + pos, mutant.begin() + pos + term.size(), new_term3);
				mutants.addMutant(mutant, "CORFxLRFWithOrderCase2");
			}
		}
	}
	return mutants;
}

//2018.12.27
string getalllitter(string exp){
	string r;
	for (int i = 0; i < exp.size(); ++i){
		char c = exp[i];
		if (c == ')' || c == '(' || c == '!'){
			continue;
		}
		if (r.find(c) == string::npos){
			r.push_back(c);
		}
	}

	//insert sort
	for (int i = 1; i < r.size(); ++i){
		for (int j = i; j > 0; --j){
			if (r[j] < r[j - 1]){
				char c = r[j];
				r[j] = r[j - 1];
				r[j - 1] = c;
			}
			else{
				break;
			}
		}
	}
	return r;
}

string stand_string(vector<string> terms, string litters){
	string r;
	for (int i = 0; i < terms.size(); ++i){
		string term = terms[i];
		string newt;
		for (int j = 0; j < litters.size(); ++j){
			int pos = term.find(litters[j]);
			if (pos != -1){
				if (pos > 0 && term[pos - 1] == '!'){
					newt += '!';
					newt += litters[j];
				}
				else{
					newt += litters[j];
				}
			}
		}
		r += newt;
		r += '+';
	}
	string re = r.substr(0, r.size() - 1);
	return re;
}

bool issame(const string& exp, const string& faultexp){
	if (exp.size() != faultexp.size()){
		return false;
	}
	for (int i = 0; i < exp.size(); ++i){
		if (exp[i] != faultexp[i]){
			return false;
		}
	}
	return true;
}

vector<string> extractDNF(string exp){
	vector<string> r;
	int brackets = 0, termstart = 0;
	for (int i = 0; i < exp.size(); ++i){
		char ch = exp[i];
		if (ch == '('){
			brackets++;
		}
		else if (ch == ')'){
			brackets--;
		}
		else if (ch == '+'&&brackets == 0){
			string term = exp.substr(termstart, i - termstart);
			r.push_back(term);
			termstart = i + 1;
		}
	}
	string term = exp.substr(termstart);
	r.push_back(term);
	return r;
}

bool DnfMutant::checkDiffer3(string faultExp, string mutant){
	bool equalFlag;
	string fault_alllitter = getalllitter(faultExp);
	string mutant_alllitter = getalllitter(mutant);
	vector<string> mutantterms, faultterms;
	faultterms = extractDNF(faultExp);
	mutantterms = extractDNF(mutant);
	string new_fault = stand_string(faultterms, fault_alllitter);
	string new_mutant = stand_string(mutantterms, mutant_alllitter);
	/*cout << "new_fault: " << new_fault << endl;
	cout << "new_mutant: " << new_mutant << endl;*/
	equalFlag = issame(new_fault, new_mutant);
	return equalFlag;
}
///
bool DnfMutant::checkDiffer(
	string oriExp,
	string faultExp,//待测表达式
	string mutant,//待检测变体
	int termSeq,//发生缺陷项的位置
	int firstLiteralSeq,//第一次发生缺陷的是哪个词（字母顺序，若是项缺陷或lif则-1）
	int secondLiteralSeq,//第二次发生缺陷的是哪个词（字母顺序，若是项缺陷或lif则-1）
	string firstLiteralChanged,//第一次缺陷词的变化（若是项缺陷，则为term）
	string secondLiteralChanged,//第二次缺陷词的变化（若是项缺陷，则为term）
	string doubleFaultType,//此双缺陷的类型
	hash_set<string>& outMutant,//排除的变体格式
	vector<vector<string>> optiUniformitySet,//最优无效点
	vector<vector<string>> optiDifferSet,//最优有效点
	hash_map<string, HierarchyNode>& hierarchyMap,
	PRE_PROCESS_DATA& preData

	){
	Utility uti;
	bool equalFlag = true;
	//首先使用无效点判断
	for (int i = 0; i < optiUniformitySet.size(); i++){
		//printf("point: %s, faultExp: %s, mutant: %s\n", optiUniformitySet[i][0].c_str(), faultExp.c_str(), mutant.c_str());
		if (uti.checkTestDiffer(optiUniformitySet[i][0], faultExp, mutant)){
			//根据层次关系将一些变体排除  2018.9.29测试时间提升 测试类容：同项双缺陷层次关系变体筛除算法
			addOutMutant(oriExp, outMutant, optiUniformitySet[i][0], termSeq, firstLiteralSeq, secondLiteralSeq, firstLiteralChanged, secondLiteralChanged,doubleFaultType, hierarchyMap);
			equalFlag = false;
			break;
		}
	}
	//再使用有效点判断
	if (equalFlag){
		for (int i = 0; i < optiDifferSet.size(); i++){
			if (uti.checkTestDiffer(optiDifferSet[i][0], faultExp, mutant)){
				equalFlag = false;
				break;
			}
		}
	}
	//再用其他的点判断
	if (equalFlag){
		for (int i = 0; i < preData.up.size(); i++){
			if (uti.checkTestDiffer(preData.up[i].c_str(), faultExp, mutant)){
				equalFlag = false;
				return equalFlag;
			}
		}
		for (int i = 0; i < preData.op.size(); i++){
			if (uti.checkTestDiffer(preData.op[i].c_str(), faultExp, mutant)){
				equalFlag = false;
				return equalFlag;
			}
		}
		for (int i = 0; i < preData.opover2.size(); i++){
			if (uti.checkTestDiffer(preData.opover2[i].c_str(), faultExp, mutant)){
				equalFlag = false;
				return equalFlag;
			}
		}
		for (hash_set<string>::iterator iter = preData.fp1.begin(); iter != preData.fp1.end(); iter++){
			if (uti.checkTestDiffer((*iter), faultExp, mutant)){
				equalFlag = false;
				return equalFlag;
			}
		}
		for (hash_set<string>::iterator iter = preData.fp2.begin(); iter != preData.fp2.end(); iter++){
			if (uti.checkTestDiffer((*iter), faultExp, mutant)){
				equalFlag = false;
				return equalFlag;
			}
		}
	}
	if (equalFlag){
		string fault_alllitter = getalllitter(faultExp);
		string mutant_alllitter = getalllitter(mutant);
		vector<string> mutantterms, faultterms;
		faultterms = extractDNF(faultExp);
		mutantterms = extractDNF(mutant);
		string new_fault = stand_string(faultterms, fault_alllitter);
		string new_mutant = stand_string(mutantterms, mutant_alllitter);
		/*cout << "new_fault: " << new_fault << endl;
		cout << "new_mutant: " << new_mutant << endl;*/
		equalFlag = issame(new_fault, new_mutant);
	}
	return equalFlag;

}



bool DnfMutant::checkDiffer2(
	string oriExp,
	string faultExp,//待测表达式
	string mutant,//待检测变体
	vector<vector<string>> optiUniformitySet,//最优无效点
	vector<vector<string>> optiDifferSet,//最优有效点
	PRE_PROCESS_DATA& preData

	){
	Utility uti;
	bool equalFlag = true;
	//首先使用无效点判断
	for (int i = 0; i < optiUniformitySet.size(); i++){
		//printf("point: %s, faultExp: %s, mutant: %s\n", optiUniformitySet[i][0].c_str(), faultExp.c_str(), mutant.c_str());
		if (uti.checkTestDiffer(optiUniformitySet[i][0], faultExp, mutant)){
			equalFlag = false;
			break;
		}
	}
	//再使用有效点判断
	if (equalFlag){
		for (int i = 0; i < optiDifferSet.size(); i++){
			if (uti.checkTestDiffer(optiDifferSet[i][0], faultExp, mutant)){
				equalFlag = false;
				break;
			}
		}
	}
	//再用其他的点判断
	if (equalFlag){
		for (int i = 0; i < preData.up.size(); i++){
			if (uti.checkTestDiffer(preData.up[i].c_str(), faultExp, mutant)){
				equalFlag = false;
				return equalFlag;
			}
		}
		for (int i = 0; i < preData.op.size(); i++){
			if (uti.checkTestDiffer(preData.op[i].c_str(), faultExp, mutant)){
				equalFlag = false;
				return equalFlag;
			}
		}
		for (int i = 0; i < preData.opover2.size(); i++){
			if (uti.checkTestDiffer(preData.opover2[i].c_str(), faultExp, mutant)){
				equalFlag = false;
				return equalFlag;
			}
		}
		for (hash_set<string>::iterator iter = preData.fp1.begin(); iter != preData.fp1.end(); iter++){
			if (uti.checkTestDiffer((*iter), faultExp, mutant)){
				equalFlag = false;
				return equalFlag;
			}
		}
		for (hash_set<string>::iterator iter = preData.fp2.begin(); iter != preData.fp2.end(); iter++){
			if (uti.checkTestDiffer((*iter), faultExp, mutant)){
				equalFlag = false;
				return equalFlag;
			}
		}
	}
	if (equalFlag){
		string fault_alllitter = getalllitter(faultExp);
		string mutant_alllitter = getalllitter(mutant);
		vector<string> mutantterms, faultterms;
		faultterms = extractDNF(faultExp);
		mutantterms = extractDNF(mutant);
		string new_fault = stand_string(faultterms, fault_alllitter);
		string new_mutant = stand_string(mutantterms, mutant_alllitter);
		/*cout << "new_fault: " << new_fault << endl;
		cout << "new_mutant: " << new_mutant << endl;*/
		equalFlag = issame(new_fault, new_mutant);
	}
	
	return equalFlag;

}

bool DnfMutant::addOutMutant(string oriExp,
	hash_set<string>& outMutant,//排除的变体格式
	string op,//无效点
	int termSeq,//发生缺陷项的位置
	int firstLiteralSeq,//第一次发生缺陷的是哪个词（字母顺序，若是项缺陷则-1）
	int secondLiteralSeq,//第二次发生缺陷的是哪个词（字母顺序，若是项缺陷则-1）
	string firstLiteralChanged,//第一次缺陷词的变化（若是项缺陷，则为term）
	string secondLiteralChanged,//第二次缺陷词的变化（若是项缺陷，则为term）
	string doubleFaultType,//此双缺陷的类型
	hash_map<string, HierarchyNode>& hierarchyMap){
	//string firstFaultFeature,//第一次缺陷的特征   
	//string new_term,//发生缺陷后的项
	// faultType,//第一次发生缺陷的类型

	Utility uti;
	bool v1 = uti.evaluate(oriExp, op);
	int condition = 0;
	if (v1){
		//原表达式为1，无效点，则待测表达式为1，对变体而言又是有效点，所以是收缩点
		condition = 2;
	}
	else{
		condition = 1;
	}
	//获取层次关系下面需要排除的变体格式(遍历图)
	hash_map<string, HierarchyNode> nodeSet;
	uti.getBelowNodeByCondition(condition,doubleFaultType, hierarchyMap, nodeSet);
	HierarchyNode thisNode = hierarchyMap[doubleFaultType];
	hash_map<string, HierarchyNode> nodeSetDisorder;
	if (thisNode.getIsDisorder()){
		uti.getBelowNodeByCondition(condition, thisNode.getEqualNode()->getValue(), hierarchyMap, nodeSetDisorder);
	}
	for (hash_map<string,HierarchyNode>::iterator iter = nodeSet.begin(); iter != nodeSet.end(); iter++){
		//第一次缺陷后的表达

		//exp = oriExp;
		//exp = exp.replace(exp.begin() + pos, exp.begin() + pos + terms[i].size(), new_term11);
		//在第i个term上发生了lif缺陷，变成了new_term11
		//string fault = faultType;
		string spec;
		if ((*iter).second.getNeedliteral()){
			//缺陷项的位置+第一次缺陷词的位置+第一次缺陷词的变化+第二次缺陷词的位置+第二次缺陷词的变化（插入了什么，替换成了什么，lof为空）
			spec = std::to_string(termSeq) + to_string(firstLiteralSeq) + firstLiteralChanged + to_string(secondLiteralSeq) + secondLiteralChanged + (*iter).first;
			//spec = fault + std::to_string(termSeq) + new_term + to_string(literalSeq) + (*iter).first;
		}
		else{
			//缺陷项的位置+第一次缺陷词的位置+第一次缺陷词的变化
			spec = std::to_string(termSeq) + to_string(firstLiteralSeq) + firstLiteralChanged + (*iter).first;
			//spec = fault + std::to_string(termSeq) + new_term + (*iter).first;
		}
		//cout << "spec1: " << spec << endl;
		outMutant.insert(spec);
	}
	for (hash_map<string, HierarchyNode>::iterator iter = nodeSetDisorder.begin(); iter != nodeSetDisorder.end(); iter++){
		//第一次缺陷后的表达

		//exp = oriExp;
		//exp = exp.replace(exp.begin() + pos, exp.begin() + pos + terms[i].size(), new_term11);
		//在第i个term上发生了lif缺陷，变成了new_term11
		//string fault = faultType;
		string spec;
		if ((*iter).second.getNeedliteral()){
			//反过来
			//缺陷项的位置+第二次缺陷词的位置+第二次缺陷词的变化+第一次缺陷词的位置+第一次缺陷词的变化（插入了什么，替换成了什么，lof为空）
			spec = std::to_string(termSeq) + to_string(secondLiteralSeq) + secondLiteralChanged + to_string(firstLiteralSeq) + firstLiteralChanged + (*iter).first;
			//spec = fault + std::to_string(termSeq) + new_term + to_string(literalSeq) + (*iter).first;
		}
		else{
			//缺陷项的位置+第二次缺陷词的位置+第二次缺陷词的变化
			spec = std::to_string(termSeq) + to_string(secondLiteralSeq) + secondLiteralChanged + (*iter).first;
		}
		//cout << "spec2: " << spec << endl;
		outMutant.insert(spec);
	}
	return true;
}
//单缺陷诊断
Mutants DnfMutant::diagLIF(string exp, string faultexp, vector<int> locat){
	Mutants mutants(exp);
	bool vars[26];
	for (int i = 0; i < 26; i++)
	{
		vars[i] = false;
	}

	string oriExp(exp);
	Utility uti;
	vector<string> terms;
	vector<vector<string>> terms_literals;
	vector<string> oriterms;
	vector<string> faultterms;
	uti.mutantsPreproccess(oriExp, terms, terms_literals, vars);
	for (int t = 0; t < locat.size(); ++t){
		int i = locat.at(t);
		oriterms.clear();
		oriterms = { terms[i] };
		int pos = oriExp.find(terms[i]);//项在表达式中开始的位置
		string new_term11 = terms[i];
		string new_term12 = terms[i];
		for (int j = 0; j < 26; j++)//【遍历不在当前项中的词literals】
		{
			char toInsert1 = 'a' + j;
			if (vars[j] && terms[i].find(toInsert1) == string::npos)//不在当前项中的词literal
			{
				//cout << "ttt===" << toInsert1 << endl;
				string s11, s12;
				s11 += toInsert1;//不在当前项中的词literals
				s12 += '!';
				s12 += toInsert1;

				new_term11 = terms[i];//当前项
				new_term12 = terms[i];

				//LIF
				new_term11 += s11;
				new_term12 += s12;
				string m1;
				string _mutant1 = oriExp;
				//变体1
				_mutant1.replace(_mutant1.begin() + pos, _mutant1.begin() + pos + terms[i].size(), new_term11);
				m1.append(_mutant1);
				faultterms = { new_term11 };
				
				bool equalFlag = checkDiffer3(faultexp, _mutant1);
				if (equalFlag){
					mutants.addMutant(m1, "lif", oriterms, faultterms);
				}
				faultterms.clear();

				string m2;
				string _mutant2 = oriExp;
				//变体2
				_mutant2.replace(_mutant2.begin() + pos, _mutant2.begin() + pos + terms[i].size(), new_term12);
				m2.append(_mutant2);
				faultterms = { new_term12 };
				bool equalFlag1 = checkDiffer3(faultexp, _mutant2);
				if (equalFlag1){
					mutants.addMutant(m2, "lif", oriterms, faultterms);
				}
				faultterms.clear();
			}
		}
	}
	return mutants;
}

Mutants DnfMutant::diagLRF(string exp, string faultexp, vector<int> locat){
	Mutants mutants(exp);
	bool vars[26];
	for (int i = 0; i < 26; i++)
	{
		vars[i] = false;
	}

	string oriExp(exp);
	Utility uti;
	vector<string> terms;
	vector<vector<string>> terms_literals;
	vector<string> oriterms;
	vector<string> faultterms;
	uti.mutantsPreproccess(oriExp, terms, terms_literals, vars);
	for (int t = 0; t < locat.size(); ++t){
		int i = locat.at(t);
		oriterms.clear();
		oriterms = { terms[i] };
		int pos = oriExp.find(terms[i]);//项在表达式中开始的位置
		string new_term11 = terms[i];
		string new_term12 = terms[i];
		vector<string> literals_in_this_term = uti.extractCNFstyle(terms[i]);//获取当前项中的词

		for (size_t j = 0; j < 26; j++)//【遍历不在当前项中的词literals】
		{
			char toReplaceWith = 'a' + j;
			string s11, s12;
			s11 += toReplaceWith;//不在当前项中的词literals
			s12 += '!';
			s12 += toReplaceWith;
			if (vars[j] && terms[i].find(toReplaceWith) == string::npos)//不在当前项中的词literals
			{
				for (size_t k = 0; k < literals_in_this_term.size(); k++)//【遍历当前term中的literal】
				{
					new_term11 = terms[i];//当前项
					new_term12 = terms[i];

					//LRF
					int literal_pos = new_term11.find(literals_in_this_term[k]);
					new_term11.replace(new_term11.begin() + literal_pos, new_term11.begin() + literal_pos + literals_in_this_term[k].size(), s11);
					new_term12.replace(new_term12.begin() + literal_pos, new_term12.begin() + literal_pos + literals_in_this_term[k].size(), s12);
					
					string _mutant1 = oriExp;
					string m1;
					_mutant1.replace(_mutant1.begin() + pos, _mutant1.begin() + pos + terms[i].size(), new_term11);
					m1.append(_mutant1);
					faultterms = { new_term11 };

					bool equalFlag = checkDiffer3(faultexp, _mutant1);
					if (equalFlag){
						mutants.addMutant(m1, "lrf", oriterms, faultterms);
					}
					faultterms.clear();

					string m2;
					string _mutant2 = oriExp;
					//变体2
					_mutant2.replace(_mutant2.begin() + pos, _mutant2.begin() + pos + terms[i].size(), new_term12);
					m2.append(_mutant2);
					faultterms = { new_term12 };
					bool equalFlag1 = checkDiffer3(faultexp, _mutant2);
					if (equalFlag1){
						mutants.addMutant(m2, "lrf", oriterms, faultterms);
					}
					faultterms.clear();

				}
			}
		}
	}
	return mutants;
}

Mutants DnfMutant::diagLOF(string exp, string faultexp, vector<int> locat){
	Mutants mutants(exp);
	bool vars[26];
	for (int i = 0; i < 26; i++)
	{
		vars[i] = false;
	}

	string oriExp(exp);
	Utility uti;
	vector<string> terms;
	vector<vector<string>> terms_literals;
	vector<string> oriterms;
	vector<string> faultterms;
	uti.mutantsPreproccess(oriExp, terms, terms_literals, vars);
	for (int t = 0; t < locat.size(); ++t){
		int i = locat.at(t);
		oriterms.clear();
		oriterms = { terms[i] };
		int pos = oriExp.find(terms[i]);//项在表达式中开始的位置
		string new_term11 = terms[i];
		string new_term12 = terms[i];
		vector<string> literals_in_this_term = uti.extractCNFstyle(terms[i]);//获取当前项中的词

		for (size_t k = 0; k < literals_in_this_term.size(); k++)//【遍历当前term中的literal】
		{
			string new_term1;
			for (size_t ll = 0; ll < terms_literals.at(i).size(); ll++)//拼接词组成新的项
			{
				if (ll == k)
					continue;
				
				new_term1 += terms_literals.at(i).at(ll);
			}

			new_term11 = new_term1;//当前项
			//cout << "new_term11: " << new_term11 << endl;
			string _mutant1 = oriExp;
			string m1;
			_mutant1.replace(_mutant1.begin() + pos, _mutant1.begin() + pos + terms[i].size(), new_term11);
			//cout << "mutant: " << _mutant1 << endl;
			m1.append(_mutant1);
			faultterms = { new_term11 };

			bool equalFlag = checkDiffer3(faultexp, _mutant1);
			if (equalFlag){
				mutants.addMutant(m1, "lof", oriterms, faultterms);
			}
			faultterms.clear();
		}
	}
	return mutants;
}

Mutants DnfMutant::diagLNF(string exp, string faultexp, vector<int> locat){
	Mutants mutants(exp);
	bool vars[26];
	for (int i = 0; i < 26; i++)
	{
		vars[i] = false;
	}

	string oriExp(exp);
	Utility uti;
	vector<string> terms;
	vector<vector<string>> terms_literals;
	vector<string> oriterms;
	vector<string> faultterms;
	uti.mutantsPreproccess(oriExp, terms, terms_literals, vars);
	for (int t = 0; t < locat.size(); ++t){
		int i = locat.at(t);
		oriterms.clear();
		oriterms = { terms[i] };
		int pos = oriExp.find(terms[i]);//项在表达式中开始的位置
		string new_term11 = terms[i];
		string new_term12 = terms[i];
		vector<string> literals_in_this_term = uti.extractCNFstyle(terms[i]);//获取当前项中的词

		for (int j = 0; j < terms_literals.at(i).size(); j++)//遍历literal
		{
			//LNF
			new_term11 = terms[i];
			string s1 = terms_literals.at(i).at(j);//第j个literal
			string s2 = uti.getLiteralNegation(s1);//获取单词的Negation
			int pos1 = new_term11.find(s1);
			new_term11.replace(pos1, s1.size(), s2);

			string _mutant1 = oriExp;
			string m1;
			_mutant1.replace(_mutant1.begin() + pos, _mutant1.begin() + pos + terms[i].size(), new_term11);
			//cout << new_term11 << endl;
			//cout << _mutant1 << endl;
			m1.append(_mutant1);
			faultterms = { new_term11 };

			bool equalFlag = checkDiffer3(faultexp, _mutant1);
			if (equalFlag){
				mutants.addMutant(m1, "lnf", oriterms, faultterms);
			}
			faultterms.clear();
		}
	}
	return mutants;
}

Mutants DnfMutant::diagTOF(string exp, string faultexp, vector<int> locat){
	Mutants mutants(exp);
	bool vars[26];
	for (int i = 0; i < 26; i++)
	{
		vars[i] = false;
	}

	string oriExp(exp);
	Utility uti;
	vector<string> terms;
	vector<vector<string>> terms_literals;
	vector<string> oriterms;
	vector<string> faultterms;
	uti.mutantsPreproccess(oriExp, terms, terms_literals, vars);
	for (int t = 0; t < locat.size(); ++t){
		int i = locat.at(t);
		oriterms.clear();
		oriterms = { terms[i] };
		int pos = oriExp.find(terms[i]);//项在表达式中开始的位置
		string new_term11 = terms[i];
		string new_term12 = terms[i];
		
		//TOF
		vector<string> new_terms = uti.extractDNFstyle(oriExp);
		string _mutant1 = "";
		for (int j = 0; j < new_terms.size(); j++){
			if (j != i){
				_mutant1 = _mutant1 + "+" + new_terms[j];
			}
		}
		_mutant1 = _mutant1.substr(1);

		bool equalFlag = checkDiffer3(faultexp, _mutant1);
		if (equalFlag){
			mutants.addMutant(_mutant1, "tof", oriterms, faultterms);
		}
	}
	return mutants;
}

Mutants DnfMutant::diagTNF(string exp, string faultexp, vector<int> locat){
	Mutants mutants(exp);
	bool vars[26];
	for (int i = 0; i < 26; i++)
	{
		vars[i] = false;
	}

	string oriExp(exp);
	Utility uti;
	vector<string> terms;
	vector<vector<string>> terms_literals;
	vector<string> oriterms;
	vector<string> faultterms;
	uti.mutantsPreproccess(oriExp, terms, terms_literals, vars);
	for (int t = 0; t < locat.size(); ++t){
		int i = locat.at(t);
		oriterms.clear();
		oriterms = { terms[i] };
		int pos = oriExp.find(terms[i]);//项在表达式中开始的位置
		string new_term11 = terms[i];
		string new_term12 = terms[i];

		//TnF
		string _mutant1 = oriExp;
		_mutant1 = uti.reverseexp2(_mutant1, i, i);

		bool equalFlag = checkDiffer3(faultexp, _mutant1);
		if (equalFlag){
			mutants.addMutant(_mutant1, "tnf", oriterms, faultterms);
		}
	}
	return mutants;
}

Mutants DnfMutant::diagCORF(string exp, string faultexp, vector<int> locat){
	Mutants mutants(exp);
	bool vars[26];
	for (int i = 0; i < 26; i++)
	{
		vars[i] = false;
	}

	string oriExp(exp);
	Utility uti;
	vector<string> terms;
	vector<vector<string>> terms_literals;
	vector<string> oriterms;
	vector<string> faultterms;
	uti.mutantsPreproccess(oriExp, terms, terms_literals, vars);
	for (int t = 0; t < locat.size(); ++t){
		int i = locat.at(t);
		oriterms.clear();
		oriterms = { terms[i] };
		int pos = oriExp.find(terms[i]);//项在表达式中开始的位置
		string new_term11 = terms[i];
		string new_term12 = terms[i];

		//corf
		vector<string> new_terms = uti.extractDNFstyle(oriExp);
		vector<string> literals_in_this_term1 = uti.extractCNFstyle(new_terms[i]);
		string new_term = literals_in_this_term1[0];
		for (int j = 1; j < literals_in_this_term1.size(); j++){
			new_term = new_term + "+" + literals_in_this_term1[j];
		}
		string _mutant1 = "";
		for (int j = 0; j < new_terms.size(); j++){
			if (j != i){
				_mutant1 = _mutant1 + "+" + new_terms[j];
			}
			else{
				_mutant1 = _mutant1 + "+" + new_term;
			}
		}
		_mutant1 = _mutant1.substr(1);
		
		bool equalFlag = checkDiffer3(faultexp, _mutant1);
		if (equalFlag){
			mutants.addMutant(_mutant1, "corf", oriterms, faultterms);
		}
	}
	return mutants;
}

Mutants DnfMutant::diagDORF(string exp, string faultexp, vector<int> locat){
	Mutants mutants(exp);
	bool vars[26];
	for (int i = 0; i < 26; i++)
	{
		vars[i] = false;
	}

	string oriExp(exp);
	Utility uti;
	vector<string> terms;
	vector<vector<string>> terms_literals;
	vector<string> oriterms;
	vector<string> faultterms;
	uti.mutantsPreproccess(oriExp, terms, terms_literals, vars);
	for (int t = 0; t < locat.size(); ++t){
		int i = locat.at(t);
		oriterms.clear();
		oriterms = { terms[i] };
		int pos = oriExp.find(terms[i]);//项在表达式中开始的位置
		string new_term11 = terms[i];
		string new_term12 = terms[i];

		//dorf
		vector<string> new_terms = uti.extractDNFstyle(oriExp);
		string _mutant1 = "";
		for (int j = 0; j < new_terms.size(); j++){
			if (j != i){
				_mutant1 = _mutant1 + "+" + new_terms[j];
			}
			else if (j + 1 < new_terms.size()){
				_mutant1 = _mutant1 + "+" + new_terms[j] + new_terms[j + 1];
				j = j + 1;
			}
		}
		_mutant1 = _mutant1.substr(1);

		bool equalFlag = checkDiffer3(faultexp, _mutant1);
		if (equalFlag){
			mutants.addMutant(_mutant1, "dorf", oriterms, faultterms);
		}
	}
	return mutants;
}

Mutants DnfMutant::diagENF(string exp, string faultexp, vector<int> locat){
	Mutants mutants(exp);
	bool vars[26];
	for (int i = 0; i < 26; i++)
	{
		vars[i] = false;
	}

	string oriExp(exp);
	Utility uti;
	vector<string> terms;
	vector<vector<string>> terms_literals;
	vector<string> oriterms;
	vector<string> faultterms;
	uti.mutantsPreproccess(oriExp, terms, terms_literals, vars);
	for (int t = 0; t < locat.size(); ++t){
		int i = locat.at(t);
		oriterms.clear();
		oriterms = { terms[i] };
		int pos = oriExp.find(terms[i]);//项在表达式中开始的位置
		string new_term11 = terms[i];
		string new_term12 = terms[i];

		//enf
		int terms_num = terms.size();
		int cur = i;
		for (int k = 2; k <= terms_num; k++){
			int start = 0, end = cur;
			if (cur - k + 1 > 0){
				start = cur - k + 1;
			}
			if (terms_num - k < cur){
				end = terms_num - k;
			}
			for (int m = start; m <= end; m++){
				string _mutant1 = oriExp;
				_mutant1 = uti.reverseexp2(_mutant1, m, m + k - 1);
				bool equalFlag = checkDiffer3(faultexp, _mutant1);
				if (equalFlag){
					mutants.addMutant(_mutant1, "enf", oriterms, faultterms);
				}
			}
		}
	}
	return mutants;
}

//生成变体的同时check有效点和无效点
Mutants DnfMutant::diagnosisSingleTermLIFxLIFdoublemutants
(	string exp,
    string faultExp,//待测表达式
	hash_set<string>& outMutant,//排除的变体格式
	vector<vector<string>> optiUniformitySet,//最优无效点
	vector<vector<string>> optiDifferSet,//最优有效点
	hash_map<string,HierarchyNode>& hierarchyMap,
	vector<int> termsOccur,//可能发生缺陷的项
	PRE_PROCESS_DATA& preData
	)
{
	Mutants mutants(exp);
	bool vars[26];
	for (int i = 0; i < 26; i++)
	{
		vars[i] = false;
	}

	string oriExp(exp);
	Utility uti;
	vector<string> terms;
	vector<vector<string>> terms_literals;
	vector<string> oriterms;
	vector<string> faultterms;
	uti.mutantsPreproccess(oriExp, terms, terms_literals, vars);
	for (int t = 0; t < termsOccur.size();t++)
	//for (int i = 0; i < terms.size(); i++)//【遍历term】
	{
		int i = termsOccur.at(t);
		oriterms.clear();
		oriterms = { terms[i] };
		//cout << "====" << terms[i] << endl;
		int pos = oriExp.find(terms[i]);//项在表达式中开始的位置
		string new_term11 = terms[i];
		string new_term12 = terms[i];

		for (int j = 0; j < 26; j++)//【遍历不在当前项中的词literals】
		{
			char toInsert1 = 'a' + j;
			if (vars[j] && terms[i].find(toInsert1) == string::npos)//不在当前项中的词literal
			{
				//cout << "ttt===" << toInsert1 << endl;
				string s11, s12;
				s11 += toInsert1;//不在当前项中的词literals
				s12 += '!';
				s12 += toInsert1;

				new_term11 = terms[i];//当前项
				new_term12 = terms[i];

				//第一次LIF
				new_term11 += s11;
				new_term12 += s12;

				for (int k = j + 1; k < 26; k++)//遍历other term剩下的词literal
				{
					char toInsert2 = 'a' + k;
					if (vars[k] && terms[i].find(toInsert2) == string::npos)
					{
						string s21, s22;
						s21 += toInsert2;
						s22 += '!';
						s22 += toInsert2;

						string new_term21, new_term22, new_term23, new_term24;

						new_term21 = new_term11;
						new_term22 = new_term11;
						new_term23 = new_term12;
						new_term24 = new_term12;

						//第二次LIF
						new_term21 += s21;
						new_term22 += s22;
						new_term23 += s21;
						new_term24 += s22;

						bool equalFlag = true;
						//首次发生缺陷的第一种变体,对应的第二次缺陷有两种变体，检验是否应被排除
						if (new_term11.compare("out") != 0){
							string m1;
							string _mutant1 = oriExp;
							//变体1
							_mutant1.replace(_mutant1.begin() + pos, _mutant1.begin() + pos + terms[i].size(), new_term21);
							m1.append(_mutant1);
							faultterms = { new_term21 };
							/*cout << "m1: " << m1 << endl;
							cout << "new_term21:" << new_term21 << endl;*/
							//校验有效点和无效点，并记录可以排除的变体特征
							equalFlag = checkDiffer(exp, faultExp, m1, i, -1, -1, s11, s21, "liflif", outMutant, optiUniformitySet, optiDifferSet, hierarchyMap, preData);
							/*for (auto it : outMutant){
								cout << "it: " << it << endl;
							}*/
							if (equalFlag){
								mutants.addMutant(m1, "liflif", oriterms, faultterms);
							}
							faultterms.clear();

							string m2;
							string _mutant2 = oriExp;
							//变体2
							_mutant2.replace(_mutant2.begin() + pos, _mutant2.begin() + pos + terms[i].size(), new_term22);
							m2.append(_mutant2);
							faultterms = { new_term22 };
							equalFlag = checkDiffer(exp, faultExp, m2, i, -1, -1, s11, s22, "liflif", outMutant, optiUniformitySet, optiDifferSet, hierarchyMap, preData);
							if (equalFlag){
								mutants.addMutant(m2, "liflif", oriterms, faultterms);
							}
							faultterms.clear();
						}
						if (new_term12.compare("out") != 0){
							string m3;
							string _mutant3 = oriExp;
							//变体3
							_mutant3.replace(_mutant3.begin() + pos, _mutant3.begin() + pos + terms[i].size(), new_term23);
							m3.append(_mutant3);
							faultterms = { new_term23 };
							equalFlag = checkDiffer(exp, faultExp, m3, i, -1, -1, s12, s21, "liflif", outMutant, optiUniformitySet, optiDifferSet, hierarchyMap, preData);
							if (equalFlag){
								mutants.addMutant(m3, "liflif", oriterms, faultterms);
							}
							faultterms.clear();

							string m4;
							string _mutant4 = oriExp;
							//变体4
							_mutant4.replace(_mutant4.begin() + pos, _mutant4.begin() + pos + terms[i].size(), new_term24);
							m4.append(_mutant4);
							faultterms = { new_term24 };
							equalFlag = checkDiffer(exp, faultExp, m4, i, -1, -1, s12, s22, "liflif", outMutant, optiUniformitySet, optiDifferSet, hierarchyMap,preData);
							if (equalFlag){
								mutants.addMutant(m4, "liflif", oriterms, faultterms);
							}
							faultterms.clear();
						}
					}
				}
			}
		}
	}
	return mutants;
}

Mutants DnfMutant::diagnosisSingleTermLIFxLRFdoublemutants(
	string exp,
	string faultExp,//待测表达式
	hash_set<string>& outMutant,//排除的变体格式
	vector<vector<string>> optiUniformitySet,//最优无效点
	vector<vector<string>> optiDifferSet,//最优有效点
	hash_map<string, HierarchyNode>& hierarchyMap,
	vector<int> termsOccur,//可能发生缺陷的项
	PRE_PROCESS_DATA& preData
	)
{
	Mutants mutants(exp);
	bool vars[26];
	for (int i = 0; i < 26; i++)
	{
		vars[i] = false;
	}

	string oriExp(exp);//原表达式
	Utility uti;
	vector<string> terms;//项
	vector<vector<string>> terms_literals;//项中的词
	vector<string> oriterms;
	vector<string> faultterms;
	uti.mutantsPreproccess(oriExp, terms, terms_literals, vars);//对表达式进行预处理，给参数中的项赋值

	for (int t = 0; t < termsOccur.size(); t++)
		//for (int i = 0; i < terms.size(); i++)//【遍历term】
	{
		int i = termsOccur.at(t);
		oriterms.clear();
		oriterms = { terms[i] };
		vector<string> literals_in_this_term = uti.extractCNFstyle(terms[i]);
		int pos = oriExp.find(terms[i]);//当前项在表达式中的位置（从0开始计数）
		for (int j = 0; j < 26; j++)//遍历26个字母
		{
			char toInsert1 = 'a' + j;
			if (vars[j] && terms[i].find(toInsert1) == string::npos)//不在当前项中的词literal
			{
				string s11, s12;
				s11 += toInsert1;//不在当前项中的词literals
				s12 += '!';
				s12 += toInsert1;
				for (int k = 0; k < literals_in_this_term.size(); k++)//遍历当前term中存在的词——literals_in_this_term[k]
				{
					string new_term11 = terms[i];
					string new_term12 = terms[i];

					//第一次LIF
					new_term11 += s11;
					new_term12 += s12;

					

					//int literal_pos = new_term11.find(literals_in_this_term[k]);//词在当前项中的位置
					//new_term11.replace(new_term11.begin() + literal_pos, new_term11.begin() + literal_pos + literals_in_this_term[k].size(), s11);
					//new_term12.replace(new_term12.begin() + literal_pos, new_term12.begin() + literal_pos + literals_in_this_term[k].size(), s12);

					for (int l = 0; l < 26; l++)
					{
						char toReplaceWith2 = 'a' + l;
						string s21, s22;
						s21 += toReplaceWith2;
						s22 += '!';
						s22 += toReplaceWith2;
						if (vars[l] && new_term11.find(toReplaceWith2) == string::npos && terms[i].find(toReplaceWith2) == string::npos)
						{
							string new_term21 = new_term11;
							string new_term22 = new_term11;
							string new_term23 = new_term12;
							string new_term24 = new_term12;
							int literal_pos = new_term11.find(literals_in_this_term[k]);//词在当前项中的位置
							
							new_term21 = new_term21.replace(new_term21.begin() + literal_pos, new_term21.begin() + literal_pos + literals_in_this_term[k].size(), s21);;
							new_term22 = new_term22.replace(new_term22.begin() + literal_pos, new_term22.begin() + literal_pos + literals_in_this_term[k].size(), s22);;
							new_term23 = new_term23.replace(new_term23.begin() + literal_pos, new_term23.begin() + literal_pos + literals_in_this_term[k].size(), s21);;
							new_term24 = new_term24.replace(new_term24.begin() + literal_pos, new_term24.begin() + literal_pos + literals_in_this_term[k].size(), s22);
			
							
							bool equalFlag = true;
							if (outMutant.count(std::to_string(i) + std::to_string(-1) + s11 + std::to_string(literal_pos) + s21 + "liflrf") == 0){
								string m1;
								string _mutant1 = oriExp;
								//变体1
								_mutant1.replace(_mutant1.begin() + pos, _mutant1.begin() + pos + terms[i].size(), new_term21);
								m1.append(_mutant1);
								faultterms = { new_term21 };
								equalFlag = checkDiffer(exp, faultExp, m1, i, -1, literal_pos, s11, s21, "liflrf", outMutant, optiUniformitySet, optiDifferSet, hierarchyMap, preData);
								if (equalFlag){
									mutants.addMutant(m1, "liflrf", oriterms, faultterms);
								}
								faultterms.clear();
							}
							if (outMutant.count(std::to_string(i) + std::to_string(-1) + s11 + std::to_string(literal_pos) + s22 + "liflrf") == 0){
								string m2;
								string _mutant2 = oriExp;
								//变体2
								_mutant2.replace(_mutant2.begin() + pos, _mutant2.begin() + pos + terms[i].size(), new_term22);
								m2.append(_mutant2);
								faultterms = { new_term22 };
								equalFlag = checkDiffer(exp, faultExp, m2, i, -1, literal_pos, s11, s22, "liflrf", outMutant, optiUniformitySet, optiDifferSet, hierarchyMap , preData);
								if (equalFlag){
									mutants.addMutant(m2, "liflrf", oriterms, faultterms);
								}
								faultterms.clear();
							}
							if (outMutant.count(std::to_string(i) + std::to_string(-1) + s12 + std::to_string(literal_pos) + s21 + "liflrf") == 0){
								string m3;
								string _mutant3 = oriExp;
								//变体3
								_mutant3.replace(_mutant3.begin() + pos, _mutant3.begin() + pos + terms[i].size(), new_term23);
								m3.append(_mutant3);
								faultterms = { new_term23 };
								equalFlag = checkDiffer(exp, faultExp, m3, i, -1, literal_pos, s12, s21, "liflrf", outMutant, optiUniformitySet, optiDifferSet, hierarchyMap, preData);
								if (equalFlag){
									mutants.addMutant(m3, "liflrf", oriterms, faultterms);
								}
								faultterms.clear();
							}
							if (outMutant.count(std::to_string(i) + std::to_string(-1) + s12 + std::to_string(literal_pos) + s22 + "liflrf") == 0){
								string m4;
								string _mutant4 = oriExp;
								//变体4
								_mutant4.replace(_mutant4.begin() + pos, _mutant4.begin() + pos + terms[i].size(), new_term24);
								m4.append(_mutant4);
								faultterms = { new_term24 };
								equalFlag = checkDiffer(exp, faultExp, m4, i, -1, literal_pos, s12, s22, "liflrf", outMutant, optiUniformitySet, optiDifferSet, hierarchyMap, preData);

								if (equalFlag){
									mutants.addMutant(m4, "liflrf", oriterms, faultterms);
								}
								faultterms.clear();
							}
						}
					}
				}
			}
		}
	}
	return mutants;
}

Mutants DnfMutant::diagnosisSingleTermLIFxLNFdoublemutants(
	string exp,
	string faultExp,//待测表达式
	hash_set<string>& outMutant,//排除的变体格式
	vector<vector<string>> optiUniformitySet,//最优无效点
	vector<vector<string>> optiDifferSet,//最优有效点
	hash_map<string, HierarchyNode>& hierarchyMap,
	vector<int> termsOccur,//可能发生缺陷的项
	PRE_PROCESS_DATA& preData
	)
{
	int total = 0;
	Mutants mutants(exp);
	bool vars[26];
	for (int i = 0; i < 26; i++)
	{
		vars[i] = false;
	}

	string oriExp(exp);//原表达式
	Utility uti;
	vector<string> terms;//项
	vector<vector<string>> terms_literals;//项中的词
	vector<string> oriterms;
	vector<string> faultterms;
	uti.mutantsPreproccess(oriExp, terms, terms_literals, vars);//对表达式进行预处理，给参数中的项赋值

	for (int t = 0; t < termsOccur.size(); t++)
		//for (int i = 0; i < terms.size(); i++)//【遍历term】
	{
		int i = termsOccur.at(t);
		oriterms.clear();
		oriterms = { terms[i] };
		vector<string> literals_in_this_term = uti.extractCNFstyle(terms[i]);
		int pos = oriExp.find(terms[i]);//当前项在表达式中的位置（从0开始计数）
		for (int j = 0; j < 26; j++)//遍历26个字母
		{
			char toInsert1 = 'a' + j;
			if (vars[j] && terms[i].find(toInsert1) == string::npos)//不在当前项中的词literal
			{
				string s11, s12;
				s11 += toInsert1;//不在当前项中的词literals
				s12 += '!';
				s12 += toInsert1;
				for (int k = 0; k < literals_in_this_term.size(); k++)//遍历当前term中存在的词——literals_in_this_term[k]
				{
					string new_term11 = terms[i];
					string new_term12 = terms[i];

					//第一次LIF
					new_term11 += s11;
					new_term12 += s12;
					if (outMutant.count(std::to_string(i) + std::to_string(-1) + s11 + "liflnf") != 0){
						new_term11 = "out";
					}
					if (outMutant.count(std::to_string(i) + std::to_string(-1) + s12 + "liflnf") != 0){
						new_term12 = "out";
					}
					string s21, s22;
					s21 += terms_literals.at(i).at(k);//第k个literal
					s22 += uti.getLiteralNegation(s21);//获取s1单词的Negation

					bool equalFlag = true;
					if (new_term11.compare("out") != 0){
						if (outMutant.count(std::to_string(i) + std::to_string(-1) + s11 + std::to_string(k) + "liflnf") == 0){
							string new_term21 = new_term11;
							string s1 = terms_literals.at(i).at(k);//第j个literal
							string s2 = uti.getLiteralNegation(s1);//获取s1单词的Negation
							int pos1 = new_term21.find(s1);
							new_term21.replace(pos1, s1.size(), s2);
							string m1;
							string _mutant1 = oriExp;
							//变体1
							_mutant1.replace(_mutant1.begin() + pos, _mutant1.begin() + pos + terms[i].size(), new_term21);
							m1.append(_mutant1);
							faultterms = { new_term21 };
							total++;
							equalFlag = checkDiffer(exp, faultExp, m1, i, -1, k, s11, "", "liflnf", outMutant, optiUniformitySet, optiDifferSet, hierarchyMap, preData);
							if (equalFlag){
								mutants.addMutant(m1, "liflnf", oriterms, faultterms);
							}
							faultterms.clear();
						}
					}
					if (new_term12.compare("out") != 0){
						if (outMutant.count(std::to_string(i) + std::to_string(-1) + s11 + std::to_string(k) + "liflnf") == 0){
							string new_term22 = new_term12;
							string s1 = terms_literals.at(i).at(k);//第k个literal
							string s2 = uti.getLiteralNegation(s1);//获取s1单词的Negation
							int pos2 = new_term22.find(s1);
							new_term22.replace(pos2, s1.size(), s2);
							string m2;
							string _mutant2 = oriExp;
							//变体2
							_mutant2.replace(_mutant2.begin() + pos, _mutant2.begin() + pos + terms[i].size(), new_term22);
							m2.append(_mutant2);
							faultterms = { new_term22 };
							total++;
							equalFlag = checkDiffer(exp, faultExp, m2, i, -1, k, s12, "", "liflnf", outMutant, optiUniformitySet, optiDifferSet, hierarchyMap, preData);
							if (equalFlag){
								mutants.addMutant(m2, "liflnf", oriterms, faultterms);
							}
							faultterms.clear();
						}
					}
				}
			}
		}
	}	
	//printf("\nLRFLNF:%d\n",total);
	//mutants.addMutant(to_string(total), "total", oriterms, faultterms);
	return mutants;
}


Mutants DnfMutant::diagnosisSingleTermLIFxTNFdoublemutants(
	string exp,
	string faultExp,//待测表达式
	hash_set<string>& outMutant,//排除的变体格式
	vector<vector<string>> optiUniformitySet,//最优无效点
	vector<vector<string>> optiDifferSet,//最优有效点
	hash_map<string, HierarchyNode>& hierarchyMap,
	vector<int> termsOccur,//可能发生缺陷的项
	PRE_PROCESS_DATA& preData
	)
{
	int total = 0;
	Mutants mutants(exp);
	bool vars[26];
	for (int i = 0; i < 26; i++) {
		vars[i] = false;
	}

	string oriExp(exp);
	Utility uti;
	vector<string> terms;
	vector<vector<string>> terms_literals;
	vector<string> oriterms;
	vector<string> faultterms;
	uti.mutantsPreproccess(oriExp, terms, terms_literals, vars);

	for (int t = 0; t < termsOccur.size(); t++)
		//for (int i = 0; i < terms.size(); i++)//【遍历term】
	{
		int i = termsOccur.at(t);
		oriterms.clear();
		oriterms = { terms[i] };
		int pos = oriExp.find(terms[i]);//项在表达式中开始的位置
		string new_term11 = terms[i];
		string new_term12 = terms[i];

		for (int j = 0; j < 26; j++)//【遍历不在当前项中的词literals】
		{
			char toInsert1 = 'a' + j;
			if (vars[j] && terms[i].find(toInsert1) == string::npos)//不在当前项中的词literal
			{
				//cout <<t<<":"<< toInsert1 << endl;
				string s11, s12;
				s11 += toInsert1;//不在当前项中的词literals
				s12 += '!';
				s12 += toInsert1;

				new_term11 = terms[i];//当前项
				new_term12 = terms[i];

				//LIF
				new_term11 += s11;
				new_term12 += s12;
				//update:outMutant的判断改为不等号(9934行也是的） 
				//if (outMutant.count(std::to_string(i) + std::to_string(-1) + s11 +  "liftnf") == 0){
				if (outMutant.count(std::to_string(i) + std::to_string(-1) + s11 + "liftnf") != 0){
					//cout << std::to_string(i) + std::to_string(-1) + s11 + "liftnf" << endl;
					new_term11 = "out";
				}
				//if (outMutant.count(std::to_string(i) + std::to_string(-1) + s12 +  "liftnf") == 0){
				if (outMutant.count(std::to_string(i) + std::to_string(-1) + s12 + "liftnf") != 0){
					//cout << std::to_string(i) + std::to_string(-1) + s12 + "liftnf" << endl;
					new_term12 = "out";
				}

				//TNF
				string new_term21, new_term22;
				new_term21 = uti.reverseexp(new_term11);
				new_term22 = uti.reverseexp(new_term12);
				/*new_term21 = "!(" + new_term11 + ")";
				new_term22 = "!(" + new_term12 + ")";*/

				bool equalFlag = true;
				if (new_term11.compare("out") != 0){
					string m1;
					string _mutant1 = oriExp;
					//变体1
					_mutant1.replace(_mutant1.begin() + pos, _mutant1.begin() + pos + terms[i].size(), new_term21);
					m1.append(_mutant1);
					faultterms = { new_term21 };
					total++;
					equalFlag = checkDiffer(exp, faultExp, m1, i, -1, -1, s11, "t", "liftnf", outMutant, optiUniformitySet, optiDifferSet, hierarchyMap, preData);
					if (equalFlag){
						mutants.addMutant(m1, "liftnf", oriterms, faultterms);
					}
					faultterms.clear();
				}
				if (new_term12.compare("out") != 0){
					string m2;
					string _mutant2 = oriExp;
					//变体2
					_mutant2.replace(_mutant2.begin() + pos, _mutant2.begin() + pos + terms[i].size(), new_term22);
					m2.append(_mutant2);
					faultterms = { new_term22 };
					total++;
					equalFlag = checkDiffer(exp, faultExp, m2, i, -1, -1, s12, "t", "liftnf", outMutant, optiUniformitySet, optiDifferSet, hierarchyMap, preData);
					if (equalFlag){
						mutants.addMutant(m2, "liftnf", oriterms, faultterms);
					}
					faultterms.clear();
				}
			}
		}
	}
	//printf("\nLIFTNF:%d\n",total);
	//mutants.addMutant(to_string(total), "total", oriterms, faultterms);
	return mutants;

}

Mutants DnfMutant::diagnosisSingleTermLIFxCORFdoublemutants(
	string exp,
	string faultExp,//待测表达式
	hash_set<string>& outMutant,//排除的变体格式
	vector<vector<string>> optiUniformitySet,//最优无效点
	vector<vector<string>> optiDifferSet,//最优有效点
	hash_map<string, HierarchyNode>& hierarchyMap,
	vector<int> termsOccur,//可能发生缺陷的项
	PRE_PROCESS_DATA& preData
	)
{
	Mutants mutants(exp);
	bool vars[26];
	for (int i = 0; i < 26; i++) {
		vars[i] = false;
	}

	string oriExp(exp);
	Utility uti;
	vector<string> terms;
	vector<vector<string>> terms_literals;
	vector<string> oriterms;
	vector<string> faultterms;
	uti.mutantsPreproccess(oriExp, terms, terms_literals, vars);

	//遍历term
	for (int t = 0; t < termsOccur.size(); t++)
		//for (int i = 0; i < terms.size(); i++)//【遍历term】
	{
		int i = termsOccur.at(t);
		oriterms.clear();
		oriterms = { terms[i] };
		int pos = oriExp.find(terms[i]);//项在表达式中开始的位置
		string new_term11 = terms[i];
		string new_term12 = terms[i];

		for (int j = 0; j < 26; j++)//【遍历不在当前项中的词literals】
		{
			char toInsert1 = 'a' + j;
			if (vars[j] && terms[i].find(toInsert1) == string::npos)//不在当前项中的词literal
			{
				string s11, s12;
				s11 += toInsert1;//不在当前项中的词literals
				s12 += '!';
				s12 += toInsert1;

				new_term11 = terms[i];//当前项
				new_term12 = terms[i];

				//LIF
				new_term11 += s11;
				new_term12 += s12;
				if (outMutant.count(std::to_string(i) + std::to_string(-1) + s11 +  "lifcorf") == 0){
					new_term11 = "out";
				}
				if (outMutant.count(std::to_string(i) + std::to_string(-1) + s12 + "lifcorf") == 0){
					new_term12 = "out";
				}

				//CORF
				vector<string> literals_in_this_term1 = uti.extractCNFstyle(new_term11);
				string newTerm1 = literals_in_this_term1[0];
				for (int j = 1; j < literals_in_this_term1.size(); j++)
				{
					newTerm1 = newTerm1 + "+" + literals_in_this_term1[j];
				}
				bool equalFlag = true;
				if (new_term11.compare("out") != 0){
					string m1;
					string _mutant1 = oriExp;
					//变体1
					_mutant1.replace(_mutant1.begin() + pos, _mutant1.begin() + pos + terms[i].size(), newTerm1);
					m1.append(_mutant1);
					faultterms = { newTerm1 };
					equalFlag = checkDiffer(exp, faultExp, m1, i, -1, -1, s11, "t", "lifcorf", outMutant, optiUniformitySet, optiDifferSet, hierarchyMap, preData);
					if (equalFlag){
						mutants.addMutant(m1, "lifcorf", oriterms, faultterms);
					}
					faultterms.clear();
				}


				vector<string> literals_in_this_term2 = uti.extractCNFstyle(new_term12);
				string newTerm2 = literals_in_this_term2[0];
				for (int j = 1; j < literals_in_this_term2.size(); j++)
				{
					newTerm2 = newTerm2 + "+" + literals_in_this_term2[j];
				}
				if (new_term12.compare("out") != 0){
					string m2;
					string _mutant2 = oriExp;
					//变体1
					_mutant2.replace(_mutant2.begin() + pos, _mutant2.begin() + pos + terms[i].size(), newTerm2);
					m2.append(_mutant2);
					faultterms = { newTerm2 };
					equalFlag = checkDiffer(exp, faultExp, m2, i, -1, -1, s11, "t", "lifcorf", outMutant, optiUniformitySet, optiDifferSet, hierarchyMap, preData);
					if (equalFlag){
						mutants.addMutant(m2, "lifcorf", oriterms, faultterms);
					}
					faultterms.clear();
				}
			}
		}
	}
	return mutants;
}


Mutants DnfMutant::diagnosisSingleTermLRFxLRFdoublemutants(
	string exp,
	string faultExp,//待测表达式
	hash_set<string>& outMutant,//排除的变体格式
	vector<vector<string>> optiUniformitySet,//最优无效点
	vector<vector<string>> optiDifferSet,//最优有效点
	hash_map<string, HierarchyNode>& hierarchyMap,
	vector<int> termsOccur,//可能发生缺陷的项
	PRE_PROCESS_DATA& preData
	)
{
	Mutants mutants(exp);
	bool vars[26];
	for (size_t i = 0; i < 26; i++)
	{
		vars[i] = false;
	}

	string oriExp(exp);
	Utility uti;
	vector<string> terms;
	vector<vector<string>> terms_literals;
	vector<string> oriterms;
	vector<string> faultterms;
	uti.mutantsPreproccess(oriExp, terms, terms_literals, vars);

	for (int t = 0; t < termsOccur.size(); t++)
		//for (int i = 0; i < terms.size(); i++)//【遍历term】
	{
		int i = termsOccur.at(t);
		oriterms.clear();
		oriterms = { terms[i] };
		vector<string> literals_in_this_term = uti.extractCNFstyle(terms[i]);//获取当前项中的词
		int pos = oriExp.find(terms[i]);//项在表达式中开始的位置
		string new_term11 = terms[i];
		string new_term12 = terms[i];
		for (size_t j = 0; j < 26; j++)//【遍历不在当前项中的词literals】
		{
			char toReplaceWith = 'a' + j;
			string s11, s12;
			s11 += toReplaceWith;//不在当前项中的词literals
			s12 += '!';
			s12 += toReplaceWith;
			if (vars[j] && terms[i].find(toReplaceWith) == string::npos)//不在当前项中的词literals
			{
				for (size_t k = 0; k < literals_in_this_term.size(); k++)//【遍历当前term中的literal】
				{
					new_term11 = terms[i];//当前项
					new_term12 = terms[i];

					//第一次 LRF
					int literal_pos = new_term11.find(literals_in_this_term[k]);
					new_term11.replace(new_term11.begin() + literal_pos, new_term11.begin() + literal_pos + literals_in_this_term[k].size(), s11);
					new_term12.replace(new_term12.begin() + literal_pos, new_term12.begin() + literal_pos + literals_in_this_term[k].size(), s12);


					for (size_t l = j + 1; l < 26; l++)//遍历other term剩下的词literal
					{
						//int pos1 = oriExp.find(new_term11);
						char toReplaceWith2 = 'a' + l;
						string s21, s22;
						s21 += toReplaceWith2;
						s22 += '!';
						s22 += toReplaceWith2;
						if (vars[l] && new_term11.find(toReplaceWith2) == string::npos && terms[i].find(toReplaceWith2) == string::npos)
						{
							for (size_t m = k + 1; m < literals_in_this_term.size(); m++)
							{
								int literal_pos1 = new_term11.find(literals_in_this_term[m]);
								int literal_pos2 = new_term12.find(literals_in_this_term[m]);
								if (-1 != literal_pos1 && (literal_pos1 != literal_pos)
									&& (-1 != literal_pos2) && (literal_pos2 != literal_pos))
								{
									string new_term21, new_term22, new_term23, new_term24;
									new_term21 = new_term11;
									new_term22 = new_term11;
									new_term23 = new_term12;
									new_term24 = new_term12;

									new_term21.replace(new_term21.begin() + literal_pos1, new_term21.begin() + literal_pos1 + literals_in_this_term[m].size(), s21);
									new_term22.replace(new_term22.begin() + literal_pos1, new_term22.begin() + literal_pos1 + literals_in_this_term[m].size(), s22);
									new_term23.replace(new_term23.begin() + literal_pos2, new_term23.begin() + literal_pos2 + literals_in_this_term[m].size(), s21);
									new_term24.replace(new_term24.begin() + literal_pos2, new_term24.begin() + literal_pos2 + literals_in_this_term[m].size(), s22);
									bool equalFlag = true;
									if (outMutant.count(std::to_string(i) + std::to_string(literal_pos) + s11 + std::to_string(literal_pos1)+s21 + "lrflrf") == 0){
										//如果变体已被排除

											string m1;
											string _mutant1 = oriExp;
											//变体1
											_mutant1.replace(_mutant1.begin() + pos, _mutant1.begin() + pos + terms[i].size(), new_term21);
											m1.append(_mutant1);
											faultterms = { new_term21 };
											equalFlag = checkDiffer(exp, faultExp, m1, i, literal_pos, literal_pos1, s11, s21, "lrflrf", outMutant, optiUniformitySet, optiDifferSet, hierarchyMap, preData);
											if (equalFlag){
												mutants.addMutant(m1, "lrflrf", oriterms, faultterms);
											}
											faultterms.clear();
										}
									if (outMutant.count(std::to_string(i) + std::to_string(literal_pos) + s11 + std::to_string(literal_pos1) + s22 + "lrflrf") == 0){
										string m2;
										string _mutant2 = oriExp;
										//变体2
										_mutant2.replace(_mutant2.begin() + pos, _mutant2.begin() + pos + terms[i].size(), new_term22);
										m2.append(_mutant2);
										faultterms = { new_term22 };
										equalFlag = checkDiffer(exp, faultExp, m2, i, literal_pos, literal_pos1, s11, s22, "lrflrf", outMutant, optiUniformitySet, optiDifferSet, hierarchyMap, preData);
										if (equalFlag){
											mutants.addMutant(m2, "lrflrf", oriterms, faultterms);
										}
										faultterms.clear();
									}
									if (outMutant.count(std::to_string(i) + std::to_string(literal_pos) + s12 + std::to_string(literal_pos1) + s21 + "lrflrf") == 0)
									{
										string m3;
										string _mutant3 = oriExp;
										//变体3
										_mutant3.replace(_mutant3.begin() + pos, _mutant3.begin() + pos + terms[i].size(), new_term23);
										m3.append(_mutant3);
										faultterms = { new_term23 };
										equalFlag = checkDiffer(exp, faultExp, m3, i, literal_pos, literal_pos1, s12, s21, "lrflrf", outMutant, optiUniformitySet, optiDifferSet, hierarchyMap, preData);
										if (equalFlag){
											mutants.addMutant(m3, "lrflrf", oriterms, faultterms);
										}
										faultterms.clear();
									}
									if (outMutant.count(std::to_string(i) + std::to_string(literal_pos) + s12 + std::to_string(literal_pos1) + s22 + "lrflrf") == 0)
									{
										string m4;
										string _mutant4 = oriExp;
										//变体4
										_mutant4.replace(_mutant4.begin() + pos, _mutant4.begin() + pos + terms[i].size(), new_term24);
										m4.append(_mutant4);
										faultterms = { new_term24 };
										equalFlag = checkDiffer(exp, faultExp, m4, i, literal_pos, literal_pos1, s12, s22, "lrflrf", outMutant, optiUniformitySet, optiDifferSet, hierarchyMap, preData);
										if (equalFlag){
											mutants.addMutant(m4, "lrflrf", oriterms, faultterms);
										}
										faultterms.clear();
									}
								}
							}
						} 
					}
				}
			}
		}
	}
	return mutants;
}

Mutants DnfMutant::diagnosisSingleTermLRFxLOFdoublemutants(
	string exp,
	string faultExp,//待测表达式
	hash_set<string>& outMutant,//排除的变体格式
	vector<vector<string>> optiUniformitySet,//最优无效点
	vector<vector<string>> optiDifferSet,//最优有效点
	hash_map<string, HierarchyNode>& hierarchyMap,
	vector<int> termsOccur,//可能发生缺陷的项
	PRE_PROCESS_DATA& preData
	)
{
	Mutants mutants(exp);
	bool vars[26];
	for (size_t i = 0; i < 26; i++)
	{
		vars[i] = false;
	}

	string oriExp(exp);
	Utility uti;
	vector<string> terms;
	vector<vector<string>> terms_literals;
	vector<string> oriterms;
	vector<string> faultterms;
	uti.mutantsPreproccess(oriExp, terms, terms_literals, vars);
	for (int t = 0; t < termsOccur.size(); t++)
		//for (int i = 0; i < terms.size(); i++)//【遍历term】
	{
		int i = termsOccur.at(t);
		oriterms.clear();
		oriterms = { terms[i] };
		int pos = oriExp.find(terms[i]);
		for (size_t j = 0; j < 26; j++)
		{
			char toReplaceWith = 'a' + j;
			string s1, s2;
			s1 += toReplaceWith;
			s2 += '!';
			s2 += toReplaceWith;

			if (vars[j] && terms[i].find(toReplaceWith) == string::npos)//不在当前term中的literal
			{
				//【遍历term的literal】
				for (size_t k = 0; k < terms_literals.at(i).size() - 1; k++)//替换第k个词
				{
					string new_term11 = terms[i];
					string new_term12 = terms[i];
					int literal_pos = new_term11.find(terms_literals[i][k]);
					new_term11.replace(new_term11.begin() + literal_pos, new_term11.begin() + literal_pos + terms_literals[k].size(), s1);
					new_term12.replace(new_term12.begin() + literal_pos, new_term12.begin() + literal_pos + terms_literals[k].size(), s2);

					for (size_t l = k + 1; l < terms_literals.at(i).size(); l++)//遗漏第L个词
					{
						if (l == k)
							continue;

						string new_term1;
						string new_term2;
						for (size_t ll = 0; ll < terms_literals.at(i).size(); ll++)//拼接词组成新的项
						{
							if (ll == l)
								continue;

							if (ll == k)
							{
								new_term1 += s1;
								new_term2 += s2;
							}
							else
							{
								new_term1 += terms_literals.at(i).at(ll);
								new_term2 += terms_literals.at(i).at(ll);
							}
						}

						//if (outMutant.count("lrf" + to_string(i) + new_term11 + to_string(l) + "lrflof") != 0){
						//	new_term11 = "out";
						//}
						//if (outMutant.count("lrf" + to_string(i) + new_term12 + to_string(l) + "lrflof") != 0){
						//	new_term12 = "out";
						//}

						string _mutant1 = oriExp;
						_mutant1.replace(_mutant1.begin() + pos, _mutant1.begin() + pos + terms[i].size(), new_term1);
						string _mutant2 = oriExp;
						_mutant2.replace(_mutant2.begin() + pos, _mutant2.begin() + pos + terms[i].size(), new_term2);

						string m1, m2;
						m1.append(_mutant1);
						m2.append(_mutant2);
						bool equalFlag = true;
						if (outMutant.count(std::to_string(i) + std::to_string(literal_pos) + s1 + std::to_string(l) + "lrflof") == 0){
							faultterms = { new_term1 };
							equalFlag = checkDiffer(exp, faultExp, m1, i, literal_pos, l, s1, "", "lrflof", outMutant, optiUniformitySet, optiDifferSet, hierarchyMap, preData);
							if (equalFlag){
								mutants.addMutant(m1, "lrflof", oriterms, faultterms);
							}
							faultterms.clear();
						}
						if (outMutant.count(std::to_string(i) + std::to_string(literal_pos) + s2 + std::to_string(l) + "lrflof") == 0){
							faultterms = { new_term2 };
							equalFlag = checkDiffer(exp, faultExp, m2, i, literal_pos, l, s2, "", "lrflof", outMutant, optiUniformitySet, optiDifferSet, hierarchyMap, preData);
							if (equalFlag){
								mutants.addMutant(m2, "lrflof", oriterms, faultterms);
							}
							faultterms.clear();
						}
					}
				}
			}
		}
	}
	return mutants;
}

Mutants DnfMutant::diagnosisSingleTermLRFxLNFdoublemutants(
	string exp,
	string faultExp,//待测表达式
	hash_set<string>& outMutant,//排除的变体格式
	vector<vector<string>> optiUniformitySet,//最优无效点
	vector<vector<string>> optiDifferSet,//最优有效点
	hash_map<string, HierarchyNode>& hierarchyMap,
	vector<int> termsOccur,//可能发生缺陷的项
	PRE_PROCESS_DATA& preData
	)
{
	Mutants mutants(exp);
	bool vars[26];
	for (int i = 0; i < 26; i++)
	{
		vars[i] = false;
	}
	string oriExp(exp);
	Utility uti;
	vector<string> terms;
	vector<vector<string>> terms_literals;
	vector<string> oriterms;
	vector<string> faultterms;
	uti.mutantsPreproccess(oriExp, terms, terms_literals, vars);

	for (int t = 0; t < termsOccur.size(); t++)
		//for (int i = 0; i < terms.size(); i++)//【遍历term】
	{
		int i = termsOccur.at(t);
		oriterms.clear();
		oriterms = { terms[i] };
		int pos = oriExp.find(terms[i]);//第i个term的开始位置
		string term = terms[i];

		for (int j = 0; j < terms_literals.at(i).size(); j++)//遍历literal
		{
			for (int k = 0; k < 26; k++)
			{
				char toReplaceWith = 'a' + k;
				if (vars[k] && terms[i].find(toReplaceWith) == string::npos)//不在当前term中的literal
				{
					string s3, s4;
					s3 += toReplaceWith;
					s4 += '!';
					s4 += toReplaceWith;
					string temp_term1 = term;

					string temp_literals = terms_literals.at(i).at(j);//要被替换的单词
					int pos1 = temp_term1.find(temp_literals);

					string new_term11 = temp_term1;
					string new_term12 = temp_term1;

					new_term11.replace(new_term11.begin() + pos1, new_term11.begin() + pos1 + temp_literals.size(), s3);
					new_term12.replace(new_term12.begin() + pos1, new_term12.begin() + pos1 + temp_literals.size(), s4);

					//if (outMutant.count("lrf" + i + new_term11 + "lrflnf") != 0){
					//	new_term11 = "out";
					//}
					//if (outMutant.count("lrf" + i + new_term12 + "lrflnf") != 0){
					//	new_term12 = "out";
					//}

			//第二步：LNF
					for (int p = 0; p < terms_literals.at(i).size(); p++)//取反（不能是上边LRF的那个词）
					{
						if (p == j)
							continue;
						string new_term21 = new_term11;
						string new_term22 = new_term12;
						string s1 = terms_literals.at(i).at(p);//第p个literal
						string s2 = uti.getLiteralNegation(s1);//获取单词的Negation
						int pos1 = new_term11.find(s1);
						new_term21.replace(pos1, s1.size(), s2);
						int pos2 = new_term12.find(s1);
						new_term22.replace(pos2, s1.size(), s2);

						bool equalFlag = true;
						if (outMutant.count(std::to_string(i) + std::to_string(pos) + s1 + std::to_string(p) + "lrflnf") == 0){
							string m1;
							string _mutant1 = oriExp;
							_mutant1.replace(_mutant1.begin() + pos, _mutant1.begin() + pos + terms[i].size(), new_term21);
							m1.append(_mutant1);
							faultterms = { new_term21 };
							equalFlag = checkDiffer(exp, faultExp, m1, i, pos1, p, s1, "", "lrflnf", outMutant, optiUniformitySet, optiDifferSet, hierarchyMap, preData);
							if (equalFlag){
								mutants.addMutant(m1, "lrflnf", oriterms, faultterms);
							}
							faultterms.clear();
						}
						if (outMutant.count(std::to_string(i) + std::to_string(pos) + s2 + std::to_string(p) + "lrflnf") == 0){
							string m2;
							string _mutant2 = oriExp;
							_mutant2.replace(_mutant2.begin() + pos, _mutant2.begin() + pos + terms[i].size(), new_term22);
							m2.append(_mutant2);
							faultterms = { new_term22 };
							equalFlag = checkDiffer(exp, faultExp, m2, i, pos1, p, s2, "", "lrflnf", outMutant, optiUniformitySet, optiDifferSet, hierarchyMap, preData);
							if (equalFlag){
								mutants.addMutant(m2, "lrflnf", oriterms, faultterms);
							}
							faultterms.clear();
						}
					}
				}
			}
		}
	}
	return mutants;
}

	Mutants DnfMutant::diagnosisSingleTermLRFxCORFdoublemutants(
		string exp,
		string faultExp,//待测表达式
		hash_set<string>& outMutant,//排除的变体格式
		vector<vector<string>> optiUniformitySet,//最优无效点
		vector<vector<string>> optiDifferSet,//最优有效点
		hash_map<string, HierarchyNode>& hierarchyMap,
		vector<int> termsOccur,//可能发生缺陷的项
		PRE_PROCESS_DATA& preData
		)
	{
		Mutants mutants(exp);
		bool vars[26];
		for (size_t i = 0; i < 26; i++)
		{
			vars[i] = false;
		}

		string oriExp(exp);
		Utility uti;
		vector<string> terms;
		vector<vector<string>> terms_literals;
		vector<string> oriterms;
		vector<string> faultterms;
		uti.mutantsPreproccess(oriExp, terms, terms_literals, vars);

		//遍历term
		for (int t = 0; t < termsOccur.size(); t++)
			//for (int i = 0; i < terms.size(); i++)//【遍历term】
		{
			int i = termsOccur.at(t);
			oriterms.clear();
			oriterms = { terms[i] };
			vector<string> literals_in_this_term = uti.extractCNFstyle(terms[i]);//获取当前项中的词
			int pos = oriExp.find(terms[i]);//项在表达式中开始的位置
			string new_term11 = terms[i];
			string new_term12 = terms[i];

			for (size_t j = 0; j < 26; j++)//【遍历不在当前项中的词literals】
			{
				char toReplaceWith = 'a' + j;
				string s11, s12;
				s11 += toReplaceWith;//不在当前项中的词literals
				s12 += '!';
				s12 += toReplaceWith;
				if (vars[j] && terms[i].find(toReplaceWith) == string::npos)//不在当前项中的词literals
				{
					for (size_t k = 0; k < literals_in_this_term.size(); k++)//【遍历当前term中的literal】
					{
						new_term11 = terms[i];//当前项
						new_term12 = terms[i];

						//LRF
						int literal_pos = new_term11.find(literals_in_this_term[k]);
						new_term11.replace(new_term11.begin() + literal_pos, new_term11.begin() + literal_pos + literals_in_this_term[k].size(), s11);
						new_term12.replace(new_term12.begin() + literal_pos, new_term12.begin() + literal_pos + literals_in_this_term[k].size(), s12);
						/*cout << "10530: new11==" << new_term11 << endl;
						cout << "10531: new12==" << new_term12 << endl;*/

						//2018.10.19 注释掉的，原因是错误的把一些变体给筛掉了
						/*if (outMutant.count(std::to_string(i) + std::to_string(pos) + s11 + "lrfcorf") != 0){
							new_term11 = "out";
						}
						if (outMutant.count(std::to_string(i) + std::to_string(pos) + s11 + "lrfcorf") != 0){
							new_term12 = "out";
						}*/
						//CORF
						vector<string> literals_in_this_term1 = uti.extractCNFstyle(new_term11);
						string newTerm1 = literals_in_this_term1[0];
						for (int j = 1; j < literals_in_this_term1.size(); j++)
						{
							newTerm1 = newTerm1 + "+" + literals_in_this_term1[j];
						}
						bool equalFlag = true;
						string _mutant1 = oriExp;
						//_mutant1.replace(_mutant1.begin() + pos, _mutant1.begin() + pos + terms[i].size(), newTerm1);
						if (new_term11.compare("out") != 0){
							string m1;
							string _mutant1 = oriExp;
							_mutant1.replace(_mutant1.begin() + pos, _mutant1.begin() + pos + terms[i].size(), newTerm1);
							m1.append(_mutant1);
							faultterms = { newTerm1 };
							equalFlag = checkDiffer(exp, faultExp, m1, i, pos, -1, s11, "t", "lrfcorf", outMutant, optiUniformitySet, optiDifferSet, hierarchyMap, preData);
							if (equalFlag){
								mutants.addMutant(m1, "lrfcorf", oriterms, faultterms);
							}
							faultterms.clear();
						}

						vector<string> literals_in_this_term2 = uti.extractCNFstyle(new_term12);
						string newTerm2 = literals_in_this_term2[0];
						for (int j = 1; j < literals_in_this_term2.size(); j++)
						{
							newTerm2 = newTerm2 + "+" + literals_in_this_term2[j];
						}
						string _mutant2 = oriExp;
						if (new_term11.compare("out") != 0){
							string m2;
							string _mutant2 = oriExp;
							_mutant2.replace(_mutant2.begin() + pos, _mutant2.begin() + pos + terms[i].size(), newTerm2);
							m2.append(_mutant2);
							faultterms = { newTerm2 };
							equalFlag = checkDiffer(exp, faultExp, m2, i, pos, -1, s12, "t", "lrfcorf", outMutant, optiUniformitySet, optiDifferSet, hierarchyMap, preData);
							if (equalFlag){
								mutants.addMutant(m2, "lrfcorf", oriterms, faultterms);
							}
							faultterms.clear();
						}
					}
				}
			}
		}
		return mutants;
	}


	Mutants DnfMutant::diagnosisSingleTermLRFxTNFdoublemutants(
		string exp,
		string faultExp,//待测表达式
		hash_set<string>& outMutant,//排除的变体格式
		vector<vector<string>> optiUniformitySet,//最优无效点
		vector<vector<string>> optiDifferSet,//最优有效点
		hash_map<string, HierarchyNode>& hierarchyMap,
		vector<int> termsOccur,//可能发生缺陷的项
		PRE_PROCESS_DATA& preData
		)
	{
		Mutants mutants(exp);
		bool vars[26];
		for (size_t i = 0; i < 26; i++)
		{
			vars[i] = false;
		}

		string oriExp(exp);
		Utility uti;
		vector<string> terms;
		vector<vector<string>> terms_literals;
		vector<string> oriterms;
		vector<string> faultterms;
		uti.mutantsPreproccess(oriExp, terms, terms_literals, vars);

		//遍历term
		for (int t = 0; t < termsOccur.size(); t++)
			//for (int i = 0; i < terms.size(); i++)//【遍历term】
		{
			int i = termsOccur.at(t);
			oriterms.clear();
			oriterms = { terms[i] };
			vector<string> literals_in_this_term = uti.extractCNFstyle(terms[i]);//获取当前项中的词
			int pos = oriExp.find(terms[i]);//项在表达式中开始的位置
			string new_term11 = terms[i];
			string new_term12 = terms[i];

			for (size_t j = 0; j < 26; j++)//【遍历不在当前项中的词literals】
			{
				char toReplaceWith = 'a' + j;
				string s11, s12;
				s11 += toReplaceWith;//不在当前项中的词literals
				s12 += '!';
				s12 += toReplaceWith;
				if (vars[j] && terms[i].find(toReplaceWith) == string::npos)//不在当前项中的词literals
				{
					for (size_t k = 0; k < literals_in_this_term.size(); k++)//【遍历当前term中的literal】
					{
						new_term11 = terms[i];//当前项
						new_term12 = terms[i];

						//LRF
						int literal_pos = new_term11.find(literals_in_this_term[k]);
						new_term11.replace(new_term11.begin() + literal_pos, new_term11.begin() + literal_pos + literals_in_this_term[k].size(), s11);
						new_term12.replace(new_term12.begin() + literal_pos, new_term12.begin() + literal_pos + literals_in_this_term[k].size(), s12);

						if (outMutant.count(std::to_string(i) + std::to_string(pos) + s11 + "lrftnf") != 0){
							new_term11 = "out";
						}
						if (outMutant.count(std::to_string(i) + std::to_string(pos) + s12 + "lrftnf") != 0){
							new_term12 = "out";
						}

						//TNF
						string new_term21, new_term22;
						new_term21 = uti.reverseexp(new_term11);
						new_term22 = uti.reverseexp(new_term12);

						bool equalFlag = true;
						//变体1
						if (new_term11.compare("out") != 0){
							string m1;
							string _mutant1 = oriExp;
							_mutant1.replace(_mutant1.begin() + pos, _mutant1.begin() + pos + terms[i].size(), new_term21);
							m1.append(_mutant1);
							faultterms = { new_term21 };
							equalFlag = checkDiffer(exp, faultExp, m1, i, pos, -1, s11, "t", "lrftnf", outMutant, optiUniformitySet, optiDifferSet, hierarchyMap, preData);
							if (equalFlag){
								mutants.addMutant(m1, "lrftnf", oriterms, faultterms);
							}
							faultterms.clear();
						}
						//变体2
						if (new_term11.compare("out") != 0){
							string m2;
							string _mutant2 = oriExp;
							_mutant2.replace(_mutant2.begin() + pos, _mutant2.begin() + pos + terms[i].size(), new_term22);
							m2.append(_mutant2);
							faultterms = { new_term22 };
							equalFlag = checkDiffer(exp, faultExp, m2, i, pos, -1, s12, "t", "lrftnf", outMutant, optiUniformitySet, optiDifferSet, hierarchyMap, preData);
							if (equalFlag){
								mutants.addMutant(m2, "lrftnf", oriterms, faultterms);
							}
							faultterms.clear();
						}
					}
				}
			}
		}
		return mutants;
	}


	Mutants DnfMutant::diagnosisSingleTermDORFxLIFdoublemutants(
		string exp,
		string faultExp,//待测表达式
		hash_set<string>& outMutant,//排除的变体格式
		vector<vector<string>> optiUniformitySet,//最优无效点
		vector<vector<string>> optiDifferSet,//最优有效点
		hash_map<string, HierarchyNode>& hierarchyMap,
		vector<int> termsOccur,//可能发生缺陷的项
		PRE_PROCESS_DATA& preData
		)
	{
		Mutants mutants(exp);
		string oriExp(exp);
		Utility uti;
		vector<string> terms = uti.extractDNFstyle(oriExp);

		vector<string> oriterms;
		vector<string> faultterms;
		for (int t = 0; t < termsOccur.size(); t++)
			//for (int i = 0; i < terms.size(); i++)//【遍历term】
		{	
			int c = t;
			printf("%d", t);
			int i = termsOccur.at(t);
			if (i == 0){
				i++;
			}
			oriterms.clear();
			oriterms = { terms[i] };
			//DORF
			int pos_current_term = oriExp.find(terms[i]);
			printf("%d", pos_current_term);
			int pos_erase_operator_or = pos_current_term - 1;// erase "+ " at this position
			printf("%d", pos_erase_operator_or);
			string new_exp = oriExp;
			new_exp.erase(new_exp.begin() + pos_erase_operator_or);

				if (outMutant.count(std::to_string(i) + std::to_string(-1) + "t" + "dorflif") != 0){
					continue;
				}
				bool vars[26];
				for (int j = 0; j < 26; j++) {
					vars[j] = false;
				}
				vector<string> terms1;
				vector<vector<string>> terms_literals;
				uti.mutantsPreproccess(new_exp, terms1, terms_literals, vars);

				string currentTerm = terms1[i - 1];

				int pos = new_exp.find(currentTerm);//项在表达式中开始的位置
				string new_term11 = currentTerm;
				string new_term12 = currentTerm;

				for (int k = 0; k < 26; k++)//遍历不在当前项中的词literals
				{
					char toInsert1 = 'a' + k;
					if (vars[k] && currentTerm.find(toInsert1) == string::npos)//不在当前项中的词literal
					{
						string s11, s12;
						s11 += toInsert1;//不在当前项中的词literals
						s12 += '!';
						s12 += toInsert1;

						new_term11 = currentTerm;//当前项
						new_term12 = currentTerm;
						//LIF
						new_term11 += s11;
						new_term12 += s12;

						string m1;
						string _mutant1 = new_exp;
						bool equalFlag = true;
						//变体1
						_mutant1.replace(_mutant1.begin() + pos, _mutant1.begin() + pos + currentTerm.size(), new_term11);
						m1.append(_mutant1);
						faultterms = { new_term11 };
						equalFlag = checkDiffer(exp, faultExp, m1, i, -1, -1, "t", s11, "dorflif", outMutant, optiUniformitySet, optiDifferSet, hierarchyMap, preData);
						if (equalFlag){
							mutants.addMutant(m1, "dorflif", oriterms, faultterms);
						}

						string m2;
						string _mutant2 = new_exp;
						//变体2
						_mutant2.replace(_mutant2.begin() + pos, _mutant2.begin() + pos + currentTerm.size(), new_term12);
						m2.append(_mutant2);
						faultterms = { new_term12 };
						equalFlag = checkDiffer(exp, faultExp, m2, i, -1, -1, "t", s12, "dorflif", outMutant, optiUniformitySet, optiDifferSet, hierarchyMap, preData);
						if (equalFlag){
							mutants.addMutant(m2, "dorflif", oriterms, faultterms);
						}

					}
				}
			}
			return mutants;
		}



	//DORFxLOF ————先发生DORF，再发生LOF
	Mutants DnfMutant::diagnosisSingleTermDORFxLOFdoublemutants(
		string exp,
		string faultExp,//待测表达式
		hash_set<string>& outMutant,//排除的变体格式
		vector<vector<string>> optiUniformitySet,//最优无效点
		vector<vector<string>> optiDifferSet,//最优有效点
		hash_map<string, HierarchyNode>& hierarchyMap,
		vector<int> termsOccur,//可能发生缺陷的项
		PRE_PROCESS_DATA& preData
		)
	{
		Mutants mutants(exp);
		string oriExp(exp);
		Utility uti;
		vector<string> terms = uti.extractDNFstyle(oriExp);

		vector<string> oriterms;
		vector<string> faultterms;
		for (int t = 0; t < termsOccur.size(); t++)
			//for (int i = 0; i < terms.size(); i++)//【遍历term】
		{
			int i = termsOccur.at(t);
			if (i == 0){
				i++;
			}
			oriterms.clear();
			oriterms = { terms[i] };
			//DORF
			int pos_current_term = oriExp.find(terms[i]);
			int pos_erase_operator_or = pos_current_term - 1;// erase "+ " at this position
			string new_exp = oriExp;
			new_exp.erase(new_exp.begin() + pos_erase_operator_or);
			if (outMutant.count(std::to_string(i) + std::to_string(-1) + "t" + "dorflof") == 0){
				continue;
			}
			bool vars[26];
			for (int j = 0; j < 26; j++) {
				vars[j] = false;
			}
			vector<string> terms1;
			vector<vector<string>> terms_literals;
			uti.mutantsPreproccess(new_exp, terms1, terms_literals, vars);

			int pos = new_exp.find(terms1[i - 1]);//第i个term的开始位置
			string new_term11 = terms1[i - 1];

			for (int j = 0; j < terms_literals.at(i - 1).size(); j++)//遍历literal
			{
				//第一步：LOF
				string temp_term = terms1[i - 1];
				string s1 = terms_literals.at(i - 1).at(j);
				int pos1 = temp_term.find(s1);
				temp_term.replace(pos1, s1.size(), "");

				vector<string> terms_literal2 = terms_literals.at(i - 1);
				terms_literal2.erase(terms_literal2.begin() + j);//删除第j个元素
				string newTerm;
				for (int k = 0; k < terms_literal2.size(); k++){
					newTerm += terms_literal2[k];
				}

				string _mutant1 = new_exp;
				_mutant1.replace(_mutant1.begin() + pos, _mutant1.begin() + pos + terms1[i - 1].size(), newTerm);
				faultterms = { newTerm };
				bool equalFlag = true;
				equalFlag = checkDiffer(exp, faultExp, _mutant1, i, -1, -1, "t", "", "dorflof", outMutant, optiUniformitySet, optiDifferSet, hierarchyMap, preData);
				if (equalFlag){
					mutants.addMutant(_mutant1, "dorflof", oriterms, faultterms);
				}
				faultterms.clear();
			}
		}
		return mutants;
	}

	//DORFxLNF ————先发生DORF，再发生LNF
	Mutants DnfMutant::diagnosisSingleTermDORFxLNFdoublemutants(
		string exp,
		string faultExp,//待测表达式
		hash_set<string>& outMutant,//排除的变体格式
		vector<vector<string>> optiUniformitySet,//最优无效点
		vector<vector<string>> optiDifferSet,//最优有效点
		hash_map<string, HierarchyNode>& hierarchyMap,
		vector<int> termsOccur,//可能发生缺陷的项
		PRE_PROCESS_DATA& preData
		)
	{
		Mutants mutants(exp);
		string oriExp(exp);
		Utility uti;
		vector<string> terms = uti.extractDNFstyle(oriExp);
		vector<string> oriterms;
		vector<string> faultterms;
		for (int t = 0; t < termsOccur.size(); t++)
			//for (int i = 0; i < terms.size(); i++)//【遍历term】
		{
			int i = termsOccur.at(t);
			if (i == 0){
				i++;
			}
			oriterms.clear();
			oriterms = { terms[i] };
			//DORF
			int pos_current_term = oriExp.find(terms[i]);
			int pos_erase_operator_or = pos_current_term - 1;// erase "+ " at this position
			string new_exp = oriExp;
			new_exp.erase(new_exp.begin() + pos_erase_operator_or);
			//if (outMutant.count("dorf" + to_string(i) + "dorflnf") != 0){
			//	continue;
			//}
			bool vars[26];
			for (int j = 0; j < 26; j++) {
				vars[j] = false;
			}
			vector<string> terms1;
			vector<vector<string>> terms_literals;
			uti.mutantsPreproccess(new_exp, terms1, terms_literals, vars);

			int pos = new_exp.find(terms1[i - 1]);//第i个term的开始位置
			string currentTerm  = terms1[i - 1];


			for (int j = 0; j < terms_literals.at(i - 1).size(); j++)//遍历literal
			{
				//第一步：LNF
				if (outMutant.count(std::to_string(i) + std::to_string(-1) + "t" + std::to_string(j) + "" + "dorflnf") != 0){
					continue;
				}
				string temp_term = terms1[i - 1];
				string s1 = terms_literals.at(i - 1).at(j);
			//	int pos1 = temp_term.find(s1);
			//  temp_term.replace(pos1, s1.size(), "");

			//	string s3 = term_literals.at(j);//第j个literal
				string s2 = uti.getLiteralNegation(s1);//获取单词的Negation
				int pos2 = temp_term.find(s1);
				string newTerm = temp_term;
				std::cout << endl;
				/*std::cout << "pos2: " << pos2 << "s1.size: " << s1.size() << std::endl;
				std::cout << "newTerm: " << newTerm<< std::endl;*/
				newTerm.replace(pos2, s1.size(), s2);//new_term 为 LNF 后的项
				/*std::cout << "newTerm2: " << newTerm << std::endl;
				std::cout << "zhe li you yichang?" << std::endl;*/
				string _mutant1 = new_exp;
				_mutant1.replace(_mutant1.begin() + pos, _mutant1.begin() + pos + terms1[i - 1].size(), newTerm);
				faultterms = { newTerm };
				bool equalFlag = true;
				equalFlag = checkDiffer(exp, faultExp, _mutant1, i, -1, j, "t", "", "dorflnf", outMutant, optiUniformitySet, optiDifferSet, hierarchyMap, preData);
				if (equalFlag){
					mutants.addMutant(_mutant1, "dorflnf", oriterms, faultterms);
				}
				faultterms.clear();
			}
		}
		return mutants;
	}


	Mutants DnfMutant::diagnosisSingleTermLOFxLOFdoublemutants(
		string exp,
		string faultExp,//待测表达式
		hash_set<string>& outMutant,//排除的变体格式
		vector<vector<string>> optiUniformitySet,//最优无效点
		vector<vector<string>> optiDifferSet,//最优有效点
		hash_map<string, HierarchyNode>& hierarchyMap,
		vector<int> termsOccur,//可能发生缺陷的项
		PRE_PROCESS_DATA& preData
		)
	{
		Mutants mutants(exp);
		bool vars[26];
		for (int i = 0; i < 26; i++)
		{
			vars[i] = false;
		}
		string oriExp(exp);
		Utility uti;
		vector<string> terms;
		vector<vector<string>> terms_literals;
		vector<string> oriterms;
		vector<string> faultterms;
		uti.mutantsPreproccess(oriExp, terms, terms_literals, vars);

		for (int t = 0; t < termsOccur.size(); t++)
			//for (int i = 0; i < terms.size(); i++)//【遍历term】
		{
			int i = termsOccur.at(t);
			oriterms.clear();
			oriterms = { terms[i] };
			int pos = oriExp.find(terms[i]);//第i个term的开始位置
			string term = terms[i];
			if (term.size() > 2)//项的长度大于2时，才可以遗漏
			{
				for (int j = 0; j < terms_literals.at(i).size(); j++)//遗漏第j个literal
				{
					//第一次：LOF
					string new_term1 = term;
					string s1 = terms_literals.at(i).at(j);

					int pos1 = new_term1.find(s1);
					new_term1.replace(pos1, s1.size(), "");

					for (int k = j + 1; k < terms_literals.at(i).size(); k++)//遗漏第k个词
					{
						if (outMutant.count(std::to_string(i) + std::to_string(j) + "" + std::to_string(k) + "" + "loflof") != 0){
							continue;
						}
						//第二次：LOF
						string new_term2 = new_term1;
						string s2 = terms_literals.at(i).at(k);

						int pos2 = new_term1.find(s2);
						new_term2.replace(pos2, s2.size(), "");

						string _mutant1 = oriExp;
						_mutant1.replace(_mutant1.begin() + pos, _mutant1.begin() + pos + terms[i].size(), new_term2);
						faultterms = { new_term2 };
						bool equalFlag = true;
						equalFlag = checkDiffer(exp, faultExp, _mutant1, i, j, k, "", "", "loflof", outMutant, optiUniformitySet, optiDifferSet, hierarchyMap, preData);
						if (equalFlag){
							mutants.addMutant(_mutant1, "loflof", oriterms, faultterms);
						}
						faultterms.clear();
					}
				}
			}
		}
		return mutants;
	}

	Mutants DnfMutant::diagnosisSingleTermLOFxLNFdoublemutants(
		string exp,
		string faultExp,//待测表达式
		hash_set<string>& outMutant,//排除的变体格式
		vector<vector<string>> optiUniformitySet,//最优无效点
		vector<vector<string>> optiDifferSet,//最优有效点
		hash_map<string, HierarchyNode>& hierarchyMap,
		vector<int> termsOccur,//可能发生缺陷的项
		PRE_PROCESS_DATA& preData
		)
	{
		Mutants mutants(exp);
		bool vars[26];
		for (int i = 0; i < 26; i++)
		{
			vars[i] = false;
		}
		string oriExp(exp);
		Utility uti;
		vector<string> terms;
		vector<vector<string>> terms_literals;
		vector<string> oriterms;
		vector<string> faultterms;
		uti.mutantsPreproccess(oriExp, terms, terms_literals, vars);

		for (int t = 0; t < termsOccur.size(); t++)
			//for (int i = 0; i < terms.size(); i++)//【遍历term】
		{
			int i = termsOccur.at(t);
			oriterms.clear();
			oriterms = { terms[i] };
			int pos = oriExp.find(terms[i]);//第i个term的开始位置
			string term = terms[i];

			for (int j = 0; j < terms_literals.at(i).size(); j++)//遍历literal
			{
				//第一步：LOF
				string temp_term = term;
				string s1 = terms_literals.at(i).at(j);
				int pos1 = temp_term.find(s1);
				temp_term.replace(pos1, s1.size(), "");

				vector<string> terms_literal2 = terms_literals.at(i);
				terms_literal2.erase(terms_literal2.begin() + j);//删除第j个元素

				//第二步：LNF
				for (int k = 0; k < terms_literal2.size(); k++)
				{
					if (outMutant.count(std::to_string(i) + std::to_string(j) + "" + std::to_string(k) + "" + "loflnf") != 0){
						continue;
					}
					string temp_term2 = temp_term;
					string s2 = terms_literal2.at(k);//第k个literal
					string s3 = uti.getLiteralNegation(s2);//获取单词的Negation
					int pos2 = temp_term2.find(s2);
					temp_term2.replace(pos2, s3.size(), s3);

					string _mutant1 = oriExp;
					_mutant1.replace(_mutant1.begin() + pos, _mutant1.begin() + pos + terms[i].size(), temp_term2);
					faultterms = { temp_term };
					bool equalFlag = true;
					equalFlag = checkDiffer(exp, faultExp, _mutant1, i, j, k, "", "", "loflnf", outMutant, optiUniformitySet, optiDifferSet, hierarchyMap, preData);
					if (equalFlag){
						mutants.addMutant(_mutant1, "loflnf", oriterms, faultterms);
					}
					faultterms.clear();
				}
			}
		}
		return mutants;
	}

	Mutants DnfMutant::diagnosisSingleTermLOFxTNFdoublemutants(
		string exp,
		string faultExp,//待测表达式
		hash_set<string>& outMutant,//排除的变体格式
		vector<vector<string>> optiUniformitySet,//最优无效点
		vector<vector<string>> optiDifferSet,//最优有效点
		hash_map<string, HierarchyNode>& hierarchyMap,
		vector<int> termsOccur,//可能发生缺陷的项
		PRE_PROCESS_DATA& preData
		)
	{
		Mutants mutants(exp);
		bool vars[26];
		for (int i = 0; i < 26; i++)
		{
			vars[i] = false;
		}
		string oriExp(exp);
		Utility uti;
		vector<string> terms;
		vector<vector<string>> terms_literals;
		vector<string> oriterms;
		vector<string> faultterms;
		uti.mutantsPreproccess(oriExp, terms, terms_literals, vars);

		for (int t = 0; t < termsOccur.size(); t++)
			//for (int i = 0; i < terms.size(); i++)//【遍历term】
		{
			int i = termsOccur.at(t);
			oriterms.clear();
			oriterms = { terms[i] };
			int pos = oriExp.find(terms[i]);//第i个term的开始位置
			string term = terms[i];

			for (int j = 0; j < terms_literals.at(i).size(); j++)//遍历literal
			{
				
				//第一步：LOF
				string temp_term = term;
				string s1 = terms_literals.at(i).at(j);
				int pos1 = temp_term.find(s1);
				temp_term.replace(pos1, s1.size(), "");

				if (outMutant.count(std::to_string(i) + std::to_string(j) + "" + "loftnf") != 0){
					continue;
				}
				//TNF
				string temp_term2 = uti.reverseexp(temp_term);
				//string temp_term2 = "!(" + temp_term + ")";
				string _mutant1 = oriExp;
				_mutant1.replace(_mutant1.begin() + pos, _mutant1.begin() + pos + terms[i].size(), temp_term2);
				faultterms = { temp_term2 };
				bool equalFlag = true;
				equalFlag = checkDiffer(exp, faultExp, _mutant1, i, j, -1, "", "t", "loftnf", outMutant, optiUniformitySet, optiDifferSet, hierarchyMap, preData);
				if (equalFlag){
					mutants.addMutant(_mutant1, "loftnf", oriterms, faultterms);
				}
				faultterms.clear();
			}
		}
		return mutants;
	}


	Mutants DnfMutant::diagnosisSingleTermLOFxCORFdoublemutants(
		string exp,
		string faultExp,//待测表达式
		hash_set<string>& outMutant,//排除的变体格式
		vector<vector<string>> optiUniformitySet,//最优无效点
		vector<vector<string>> optiDifferSet,//最优有效点
		hash_map<string, HierarchyNode>& hierarchyMap,
		vector<int> termsOccur,//可能发生缺陷的项
		PRE_PROCESS_DATA& preData
		){
		Mutants mutants(exp);
		bool vars[26];
		for (int i = 0; i < 26; i++)
		{
			vars[i] = false;
		}
		string oriExp(exp);
		Utility uti;
		vector<string> terms;
		vector<vector<string>> terms_literals;
		vector<string> oriterms;
		vector<string> faultterms;
		uti.mutantsPreproccess(oriExp, terms, terms_literals, vars);

		for (int t = 0; t < termsOccur.size(); t++)
			//for (int i = 0; i < terms.size(); i++)//【遍历term】
		{
			int i = termsOccur.at(t);
			oriterms.clear();
			oriterms = { terms[i] };
			int pos = oriExp.find(terms[i]);//第i个term的开始位置
			string term = terms[i];

			for (int j = 0; j < terms_literals.at(i).size(); j++)//遍历literal
			{
				//第一步：LOF
				string temp_term = term;
				string s1 = terms_literals.at(i).at(j);
				int pos1 = temp_term.find(s1);
				temp_term.replace(pos1, s1.size(), "");
				if (outMutant.count(std::to_string(i) + std::to_string(j) + "" + "lofcorf") != 0){
					continue;
				}
				//CORF
				if (temp_term == "")break;
				vector<string> literals_in_this_term1 = uti.extractCNFstyle(temp_term);
				if (literals_in_this_term1.size() == 0)break;
				string newTerm1 = literals_in_this_term1[0];
				for (int j = 1; j < literals_in_this_term1.size(); j++)
				{
					newTerm1 = newTerm1 + "+" + literals_in_this_term1[j];
				}
				string _mutant1 = oriExp;
				_mutant1.replace(_mutant1.begin() + pos, _mutant1.begin() + pos + terms[i].size(), newTerm1);
				faultterms = { newTerm1 };
				bool equalFlag = true;
				equalFlag = checkDiffer(exp, faultExp, _mutant1, i, j, -1, "", "t", "lofcorf", outMutant, optiUniformitySet, optiDifferSet, hierarchyMap, preData);
				if (equalFlag){
					mutants.addMutant(_mutant1, "lofcorf", oriterms, faultterms);
				}
				faultterms.clear();
			}
		}
		return mutants;
	}


	Mutants DnfMutant::diagnosisSingleTermLNFxLNFdoublemutants(
		string exp,
		string faultExp,//待测表达式
		hash_set<string>& outMutant,//排除的变体格式
		vector<vector<string>> optiUniformitySet,//最优无效点
		vector<vector<string>> optiDifferSet,//最优有效点
		hash_map<string, HierarchyNode>& hierarchyMap,
		vector<int> termsOccur,//可能发生缺陷的项
		PRE_PROCESS_DATA& preData
		)
	{
		Mutants mutants(exp);
		bool vars[26];
		for (int i = 0; i < 26; i++)
		{
			vars[i] = false;
		}
		string oriExp(exp);
		Utility uti;
		vector<string> terms;
		vector<vector<string>> terms_literals;
		vector<string> oriterms;
		vector<string> faultterms;
		uti.mutantsPreproccess(oriExp, terms, terms_literals, vars);

		for (int t = 0; t < termsOccur.size(); t++)
			//for (int i = 0; i < terms.size(); i++)//【遍历term】
		{
			int k = termsOccur.at(t);
			oriterms.clear();
			oriterms = { terms[k] };
			string term = terms[k];
			int pos = exp.find(term);

			vector<string> term_literals = uti.extractCNFstyle(term);

			for (int i = 0; i < term_literals.size() - 1; i++)
			{
				//LNF
				string new_term1 = term;
				string s1 = term_literals.at(i);//第i个literal
				string s2 = uti.getLiteralNegation(s1);//获取单词的Negation
				int pos1 = new_term1.find(s1);
				new_term1.replace(pos1, s1.size(), s2);//temp_term2为 LNF 后的项
				for (int j = i + 1; j < term_literals.size(); j++)
				{
					if (outMutant.count(std::to_string(k) + std::to_string(i) + "" + std::to_string(j)+""+"lnflnf") != 0){
						continue;
					}
					//LNF
					string new_term2 = new_term1;
					string s3 = term_literals.at(j);//第j个literal
					string s4 = uti.getLiteralNegation(s3);//获取单词的Negation
					int pos2 = new_term2.find(s3);
					new_term2.replace(pos2, s3.size(), s4);//new_term2 为 LNF 后的项

					string mutant = exp;
					mutant.replace(mutant.begin() + pos, mutant.begin() + pos + term.size(), new_term2);

					bool equalFlag = true;
					equalFlag = checkDiffer(exp, faultExp, mutant, k, i, j, "", "", "lnflnf", outMutant, optiUniformitySet, optiDifferSet, hierarchyMap, preData);
					if (equalFlag){
						faultterms = { new_term1 };
						mutants.addMutant(mutant, "lnflnf", oriterms, faultterms);
					}
				}
			}
		}
		return mutants;
	}

	Mutants DnfMutant::diagnosisSingleTermLNFxTNFdoublemutants(
		string exp,
		string faultExp,//待测表达式
		hash_set<string>& outMutant,//排除的变体格式
		vector<vector<string>> optiUniformitySet,//最优无效点
		vector<vector<string>> optiDifferSet,//最优有效点
		hash_map<string, HierarchyNode>& hierarchyMap,
		vector<int> termsOccur,//可能发生缺陷的项
		PRE_PROCESS_DATA& preData
		)
	{
		Mutants mutants(exp);
		bool vars[26];
		for (int i = 0; i < 26; i++)
		{
			vars[i] = false;
		}
		string oriExp(exp);
		Utility uti;
		vector<string> terms;
		vector<vector<string>> terms_literals;
		vector<string> oriterms;
		vector<string> faultterms;
		uti.mutantsPreproccess(oriExp, terms, terms_literals, vars);

		for (int t = 0; t < termsOccur.size(); t++)
			//for (int i = 0; i < terms.size(); i++)//【遍历term】
		{
			int i = termsOccur.at(t);
			oriterms.clear();
			oriterms = { terms[i] };
			int pos = oriExp.find(terms[i]);//第i个term的开始位置
			string term = terms[i];

			for (int j = 0; j < terms_literals.at(i).size(); j++)//遍历literal
			{
				//LNF
				string temp_term1 = term;
				string s1 = terms_literals.at(i).at(j);//第j个literal
				string s2 = uti.getLiteralNegation(s1);//获取单词的Negation
				int pos1 = temp_term1.find(s1);
				temp_term1.replace(pos1, s1.size(), s2);//temp_term1为 LNF 后的项

				if (outMutant.count(std::to_string(i) + std::to_string(j) + "" + "lnftnf") != 0){
					continue;
				}

				//TNF
				string temp_term2 = "!(" + temp_term1 + ")";
				string _mutant1 = oriExp;
				_mutant1.replace(_mutant1.begin() + pos, _mutant1.begin() + pos + terms[i].size(), temp_term2);
				faultterms = { temp_term2 };
				bool equalFlag = true;

				equalFlag = checkDiffer(exp, faultExp, _mutant1, i, j, -1, "", "t", "lnftnf", outMutant, optiUniformitySet, optiDifferSet, hierarchyMap, preData);
				if (equalFlag){
					mutants.addMutant(_mutant1, "lnftnf", oriterms, faultterms);
				}
				faultterms.clear();
			}
		}
		return mutants;
	}

	Mutants DnfMutant::diagnosisSingleTermLNFxCORFdoublemutants(
		string exp,
		string faultExp,//待测表达式
		hash_set<string>& outMutant,//排除的变体格式
		vector<vector<string>> optiUniformitySet,//最优无效点
		vector<vector<string>> optiDifferSet,//最优有效点
		hash_map<string, HierarchyNode>& hierarchyMap,
		vector<int> termsOccur,//可能发生缺陷的项
		PRE_PROCESS_DATA& preData
		)
	{
		Mutants mutants(exp);
		bool vars[26];
		for (int i = 0; i < 26; i++)
		{
			vars[i] = false;
		}
		string oriExp(exp);
		Utility uti;
		vector<string> terms;
		vector<vector<string>> terms_literals;
		vector<string> oriterms;
		vector<string> faultterms;
		uti.mutantsPreproccess(oriExp, terms, terms_literals, vars);

		for (int t = 0; t < termsOccur.size(); t++)
			//for (int i = 0; i < terms.size(); i++)//【遍历term】
		{
			int i = termsOccur.at(t);
			oriterms.clear();
			oriterms = { terms[i] };
			int pos = oriExp.find(terms[i]);//第i个term的开始位置
			string term = terms[i];

			for (int j = 0; j < terms_literals.at(i).size(); j++)//遍历literal
			{
				//LNF
				string temp_term1 = term;
				string s1 = terms_literals.at(i).at(j);//第j个literal
				string s2 = uti.getLiteralNegation(s1);//获取单词的Negation
				int pos1 = temp_term1.find(s1);
				temp_term1.replace(pos1, s1.size(), s2);//temp_term1为 LNF 后的项
				if (outMutant.count(std::to_string(i) + std::to_string(j) + "" + "lnfcorf") != 0){
					continue;
				}
				vector<string> literals_in_this_term1 = uti.extractCNFstyle(temp_term1);
				string temp_term2 = literals_in_this_term1[0];
				for (int j = 1; j < literals_in_this_term1.size(); j++)
				{
					temp_term2 = temp_term2 + "+" + literals_in_this_term1[j];
				}
				bool equalFlag = true;
				string _mutant1 = oriExp;
				_mutant1.replace(_mutant1.begin() + pos, _mutant1.begin() + pos + terms[i].size(), temp_term2);
				faultterms = { temp_term2 };
				equalFlag = checkDiffer(exp, faultExp, _mutant1, i, j, -1, "", "t", "lnfcorf", outMutant, optiUniformitySet, optiDifferSet, hierarchyMap, preData);
				if (equalFlag){
					mutants.addMutant(_mutant1, "lnfcorf", oriterms, faultterms);
				}
				faultterms.clear();
			}
		}
		return mutants;
	}


	//2018.12.1
	Mutants DnfMutant::diagnosisSingleTermLRFxDORFdoublemutants(
		string exp,
		string faultExp,//待测表达式
		hash_set<string>& outMutant,//排除的变体格式
		vector<vector<string>> optiUniformitySet,//最优无效点
		vector<vector<string>> optiDifferSet,//最优有效点
		hash_map<string, HierarchyNode>& hierarchyMap,
		vector<int> termsOccur,//可能发生缺陷的项
		PRE_PROCESS_DATA& preData
		)
	{
		//cout << "DnfMutant(11504) here" << endl;
		Mutants mutants(exp);
		bool vars[26];
		for (size_t i = 0; i < 26; i++)
		{
			vars[i] = false;
		}

		string oriExp(exp);
		Utility uti;
		vector<string> terms;
		vector<vector<string>> terms_literals;
		vector<string> oriterms;
		vector<string> faultterms;
		uti.mutantsPreproccess(oriExp, terms, terms_literals, vars);

		//遍历term
		for (int t = 0; t < termsOccur.size(); t++)
			//for (int i = 0; i < terms.size(); i++)//【遍历term】
		{
			int i = termsOccur.at(t);
			oriterms.clear();
			oriterms = { terms[i] };
			vector<string> literals_in_this_term = uti.extractCNFstyle(terms[i]);//获取当前项中的词
			int pos = oriExp.find(terms[i]);//项在表达式中开始的位置
			string new_term11 = terms[i];
			string new_term12 = terms[i];

			for (size_t j = 0; j < 26; j++)//【遍历不在当前项中的词literals】
			{
				char toReplaceWith = 'a' + j;
				string s11, s12;
				s11 += toReplaceWith;//不在当前项中的词literals
				s12 += '!';
				s12 += toReplaceWith;
				if (vars[j] && terms[i].find(toReplaceWith) == string::npos)//不在当前项中的词literals
				{
					for (size_t k = 0; k < literals_in_this_term.size(); k++)//【遍历当前term中的literal】
					{
						new_term11 = terms[i];//当前项
						new_term12 = terms[i];

						//LRF
						int literal_pos = new_term11.find(literals_in_this_term[k]);
						new_term11.replace(new_term11.begin() + literal_pos, new_term11.begin() + literal_pos + literals_in_this_term[k].size(), s11);
						new_term12.replace(new_term12.begin() + literal_pos, new_term12.begin() + literal_pos + literals_in_this_term[k].size(), s12);

						if (outMutant.count(std::to_string(i) + std::to_string(pos) + s11 + "lrftnf") != 0){
							new_term11 = "out";
						}
						if (outMutant.count(std::to_string(i) + std::to_string(pos) + s12 + "lrftnf") != 0){
							new_term12 = "out";
						}
						/*cout << "new_term11: " << new_term11 << endl;
						cout << "new_term12: " << new_term12 << endl;*/

						//DORF
						string preterm = terms[i], postterm=terms[i];
						int flag1 = 0, flag2 = 0;
						if (i > 0){
							preterm = terms[i - 1];
							flag1 = 1;
						}
						if (i + 1 < terms.size()){
							postterm = terms[i + 1];
							flag2 = 1;
						}
						/*cout << "preterm: " << preterm << endl;
						cout << "postterm: " << postterm << endl;*/
						string new_term21, new_term22,new_term23,new_term24;
						if (flag1){
							new_term21 = preterm + new_term11;
							new_term22 = preterm + new_term12;
			
							string mut1 = oriExp, mut2=oriExp;
							int pos_1 = mut1.find(terms[i - 1]);
							int pos_2 = oriExp.size();
							if (i + 1 < terms.size()){
								pos_2 = mut1.find(terms[i + 1]) - 1;
							}
							mut1.replace(mut1.begin() + pos_1, mut1.begin() + pos_2, new_term21);
							mut2.replace(mut2.begin() + pos_1, mut2.begin() + pos_2, new_term22);
							/*cout << "DnfMutant.cpp(11582) mut1: " << mut1 << endl;
							cout << "DnfMutant.cpp(11582) mut2: " << mut2 << endl;*/
							faultterms = { new_term21 };
							if (checkDiffer2(exp, faultExp, mut1, optiUniformitySet, optiDifferSet, preData)){
								mutants.addMutant(mut1, "lrfdorf", oriterms, faultterms);
								return mutants;
							}
							faultterms.clear();

							faultterms = { new_term22 };
							if (checkDiffer2(exp, faultExp, mut2, optiUniformitySet, optiDifferSet, preData)){
								mutants.addMutant(mut2, "lrfdorf", oriterms, faultterms);
								return mutants;
							}
							 
						}

						if (flag2){
							
							new_term23 = new_term11 + postterm;
							new_term24 = new_term12 + postterm;
							/*cout << "11574 new_term23:" << new_term23 << endl;
							cout << "11574 new_term24:" << new_term24 << endl;*/
							string mut1 = oriExp, mut2 = oriExp;
							int pos_1 = mut1.find(terms[i]);
							int pos_2 = oriExp.size();
							if (i + 2 < terms.size()){
								pos_2 = mut1.find(terms[i + 2]) - 1;
							}
							mut1.replace(mut1.begin() + pos_1, mut1.begin() + pos_2, new_term23);
							mut2.replace(mut2.begin() + pos_1, mut2.begin() + pos_2, new_term24);
							/*cout << "DnfMutant.cpp(11610) mut1: " << mut1 << endl;
							cout << "DnfMutant.cpp(11611) mut2: " << mut2 << endl;*/
							faultterms = { new_term23 };
							if (checkDiffer2(exp, faultExp, mut1, optiUniformitySet, optiDifferSet, preData)){
								mutants.addMutant(mut1, "lrfdorf", oriterms, faultterms);
								return mutants;
							}
							faultterms.clear();

							faultterms = { new_term24 };
							if (checkDiffer2(exp, faultExp, mut2, optiUniformitySet, optiDifferSet, preData)){
								mutants.addMutant(mut2, "lrfdorf", oriterms, faultterms);
								return mutants;
							}

						}
						
					}
				}
			}
		}
		return mutants;
	}

	Mutants DnfMutant::diagnosisSingleTermDORFxDORFdoublemutants(
		string exp,
		string faultExp,//待测表达式
		hash_set<string>& outMutant,//排除的变体格式
		vector<vector<string>> optiUniformitySet,//最优无效点
		vector<vector<string>> optiDifferSet,//最优有效点
		hash_map<string, HierarchyNode>& hierarchyMap,
		vector<int> termsOccur,//可能发生缺陷的项
		PRE_PROCESS_DATA& preData
		)
	{
		//cout << "DnfMutant(11504) here" << endl;
		Mutants mutants(exp);
		bool vars[26];
		for (size_t i = 0; i < 26; i++)
		{
			vars[i] = false;
		}
		string oriExp(exp);
		Utility uti;
		vector<string> terms;
		vector<vector<string>> terms_literals;
		vector<string> oriterms;
		vector<string> faultterms;
		uti.mutantsPreproccess(oriExp, terms, terms_literals, vars);

		//遍历term
		for (int t = 0; t < termsOccur.size(); t++)
			//for (int i = 0; i < terms.size(); i++)//【遍历term】
		{
			int i = termsOccur.at(t);
			oriterms.clear();
			oriterms = { terms[i] };
			int pos = oriExp.find(terms[i]);//项在表达式中开始的位置
			string new_term11 = terms[i];
			string new_term12 = terms[i];

			//first dorf
			string preterm_1 = terms[i], postterm_1 = terms[i];
			int f1 = 0, f2 = 0;
			if (i - 1 >= 0){
				preterm_1 = terms[i - 1];
				f1 = 1;
			}
			if (i + 1 < terms.size()){
				postterm_1 = terms[i + 1];
				f2 = 1;
			}

			if (f1){
				new_term11 = preterm_1 + new_term11;
				int f3 = 0, f4 = 0;
				string preterm_2, postterm_2;
				string new_term21, new_term22;
				//second dorf
				if (i - 2 >= 0){
					preterm_2 = terms[i - 2];
					f3 = 1;
				}
				if (i + 1 < terms.size()){
					postterm_2 = terms[i + 1];
					f4 = 1;
				}
				if (f3){
					new_term21 = preterm_2 + new_term11;
					string mut1 = oriExp;
					int pos_1 = mut1.find(terms[i - 2]);
					int pos_2 = oriExp.size();
					if (i + 1 < terms.size()){
						pos_2 = mut1.find(terms[i + 1]) - 1;
					}
					mut1.replace(mut1.begin() + pos_1, mut1.begin() + pos_2, new_term21);

					/*cout << "DnfMutant.cpp(11582) mut1: " << mut1 << endl;
					cout << "DnfMutant.cpp(11582) mut2: " << mut2 << endl;*/
					faultterms = { new_term21 };
					if (checkDiffer2(exp, faultExp, mut1, optiUniformitySet, optiDifferSet, preData)){
						mutants.addMutant(mut1, "dorfdorf", oriterms, faultterms);
						return mutants;
					}
				}
				if (f4){
					new_term22 = new_term11 + postterm_2;
					string mut1 = oriExp;
					int pos_1 = mut1.find(terms[i - 1]);
					int pos_2 = oriExp.size();
					if (i + 2 < terms.size()){
						pos_2 = mut1.find(terms[i + 2]) - 1;
					}
					mut1.replace(mut1.begin() + pos_1, mut1.begin() + pos_2, new_term22);

					/*cout << "DnfMutant.cpp(11582) mut1: " << mut1 << endl;
					cout << "DnfMutant.cpp(11582) mut2: " << mut2 << endl;*/
					faultterms = { new_term22 };
					if (checkDiffer2(exp, faultExp, mut1, optiUniformitySet, optiDifferSet, preData)){
						mutants.addMutant(mut1, "dorfdorf", oriterms, faultterms);
						return mutants;
					}
				}
			}

			if (f2){
				new_term12 = new_term12 + postterm_1;
				int f3 = 0, f4 = 0;
				string preterm_2, postterm_2;
				string new_term21, new_term22;
				//second dorf
				if (i - 1 >= 0){
					preterm_2 = terms[i - 1];
					f3 = 1;
				}
				if (i + 2 < terms.size()){
					postterm_2 = terms[i + 2];
					f4 = 1;
				}
				if (f3){
					new_term21 = preterm_2 + new_term12;
					string mut1 = oriExp;
					int pos_1 = mut1.find(terms[i - 1]);
					int pos_2 = oriExp.size();
					if (i + 2 < terms.size()){
						pos_2 = mut1.find(terms[i + 2]) - 1;
					}
					mut1.replace(mut1.begin() + pos_1, mut1.begin() + pos_2, new_term21);

					/*cout << "DnfMutant.cpp(11582) mut1: " << mut1 << endl;
					cout << "DnfMutant.cpp(11582) mut2: " << mut2 << endl;*/
					faultterms = { new_term21 };
					if (checkDiffer2(exp, faultExp, mut1, optiUniformitySet, optiDifferSet, preData)){
						mutants.addMutant(mut1, "dorfdorf", oriterms, faultterms);
						return mutants;
					}
				}
				if (f4){
					new_term22 = new_term12 + postterm_2;
					string mut1 = oriExp;
					int pos_1 = mut1.find(terms[i]);
					int pos_2 = oriExp.size();
					if (i + 3 < terms.size()){
						pos_2 = mut1.find(terms[i + 3]) - 1;
					}
					mut1.replace(mut1.begin() + pos_1, mut1.begin() + pos_2, new_term22);

					/*cout << "DnfMutant.cpp(11582) mut1: " << mut1 << endl;
					cout << "DnfMutant.cpp(11582) mut2: " << mut2 << endl;*/
					faultterms = { new_term22 };
					if (checkDiffer2(exp, faultExp, mut1, optiUniformitySet, optiDifferSet, preData)){
						mutants.addMutant(mut1, "dorfdorf", oriterms, faultterms);
						return mutants;
					}
				}
			}
		}

		
		return mutants;
	}

	Mutants DnfMutant::diagnosisSingleTermDORFxTNFdoublemutants(
		string exp,
		string faultExp,//待测表达式
		hash_set<string>& outMutant,//排除的变体格式
		vector<vector<string>> optiUniformitySet,//最优无效点
		vector<vector<string>> optiDifferSet,//最优有效点
		hash_map<string, HierarchyNode>& hierarchyMap,
		vector<int> termsOccur,//可能发生缺陷的项
		PRE_PROCESS_DATA& preData
		)
	{
		//cout << "DnfMutant(11504) here" << endl;
		Mutants mutants(exp);
		bool vars[26];
		for (size_t i = 0; i < 26; i++)
		{
			vars[i] = false;
		}
		string oriExp(exp);
		Utility uti;
		vector<string> terms;
		vector<vector<string>> terms_literals;
		vector<string> oriterms;
		vector<string> faultterms;
		uti.mutantsPreproccess(oriExp, terms, terms_literals, vars);

		//遍历term
		for (int t = 0; t < termsOccur.size(); t++)
			//for (int i = 0; i < terms.size(); i++)//【遍历term】
		{
			int i = termsOccur.at(t);
			oriterms.clear();
			oriterms = { terms[i] };
			int pos = oriExp.find(terms[i]);//项在表达式中开始的位置
			string new_term11 = terms[i];
			string new_term12 = terms[i];

			//first dorf
			string preterm_1 = terms[i], postterm_1 = terms[i];
			int f1 = 0, f2 = 0;
			if (i - 1 >= 0){
				preterm_1 = terms[i - 1];
				f1 = 1;
			}
			if (i + 1 < terms.size()){
				postterm_1 = terms[i + 1];
				f2 = 1;
			}

			if (f1){
				new_term11 = preterm_1 + new_term11;
				string mut1 = oriExp;
				int pos_1 = mut1.find(terms[i - 1]);
				int pos_2 = oriExp.size();
				if (i + 1 < terms.size()){
					pos_2 = mut1.find(terms[i + 1]) - 1;
				}
				
				string new_term21;
				//tnf
				new_term21 = uti.reverseexp(new_term11);
				//cout << "new_term21: " << new_term21 << endl;
				mut1.replace(mut1.begin() + pos_1, mut1.begin() + pos_2, new_term21);
				faultterms = { new_term21 };
				if (checkDiffer2(exp, faultExp, mut1, optiUniformitySet, optiDifferSet, preData)){
					mutants.addMutant(mut1, "dorftnf", oriterms, faultterms);
					return mutants;
				}
				
			}

			if (f2){
				new_term12 = new_term12 + postterm_1;
				string mut1 = oriExp;
				int pos_1 = mut1.find(terms[i]);
				int pos_2 = oriExp.size();
				if (i + 2 < terms.size()){
					pos_2 = mut1.find(terms[i + 2]) - 1;
				}

				string new_term22;
				//tnf
				new_term22 = "!(" + new_term12 + ")";
				//cout << "new_term22: " << new_term22 << endl;
				mut1.replace(mut1.begin() + pos_1, mut1.begin() + pos_2, new_term22);

				/*cout << "DnfMutant.cpp(11582) mut1: " << mut1 << endl;
				cout << "DnfMutant.cpp(11582) mut2: " << mut2 << endl;*/
				faultterms = { new_term22 };
				if (checkDiffer2(exp, faultExp, mut1, optiUniformitySet, optiDifferSet, preData)){
					mutants.addMutant(mut1, "dorftnf", oriterms, faultterms);
					return mutants;
				}

			}
		}
		return mutants;
	}

	Mutants DnfMutant::diagnosisSingleTermTNFxDORFdoublemutants(
		string exp,
		string faultExp,//待测表达式
		hash_set<string>& outMutant,//排除的变体格式
		vector<vector<string>> optiUniformitySet,//最优无效点
		vector<vector<string>> optiDifferSet,//最优有效点
		hash_map<string, HierarchyNode>& hierarchyMap,
		vector<int> termsOccur,//可能发生缺陷的项
		PRE_PROCESS_DATA& preData
		)
	{
		//cout << "DnfMutant(11504) here" << endl;
		Mutants mutants(exp);
		bool vars[26];
		for (size_t i = 0; i < 26; i++)
		{
			vars[i] = false;
		}
		string oriExp(exp);
		Utility uti;
		vector<string> terms;
		vector<vector<string>> terms_literals;
		vector<string> oriterms;
		vector<string> faultterms;
		uti.mutantsPreproccess(oriExp, terms, terms_literals, vars);

		//遍历term
		for (int t = 0; t < termsOccur.size(); t++)
			//for (int i = 0; i < terms.size(); i++)//【遍历term】
		{
			int i = termsOccur.at(t);
			//cout << "i: " << i << endl;
			oriterms.clear();
			oriterms = { terms[i] };
			int pos = oriExp.find(terms[i]);//项在表达式中开始的位置
			//TNF
			string new_term1 = terms[i];
			string new_term11 = uti.reverseexp(new_term1);

			//dorf
			string preterm_1 = terms[i], postterm_1 = terms[i];
			int f1 = 0, f2 = 0;
			if (i - 1 >= 0){
				preterm_1 = terms[i - 1];
				f1 = 1;
			}
			if (i + 1 < terms.size()){
				postterm_1 = terms[i + 1];
				f2 = 1;
			}

			if (f1){
				string new_term21;
				new_term21 = preterm_1 + new_term11;
				string mut1 = oriExp;
				int pos_1 = mut1.find(terms[i - 1]);
				int pos_2 = oriExp.size();
				if (i + 1 < terms.size()){
					pos_2 = mut1.find(terms[i + 1]) - 1;
				}

				//cout << "new_term21: " << new_term21 << endl;
				mut1.replace(mut1.begin() + pos_1, mut1.begin() + pos_2, new_term21);
				faultterms = { new_term21 };
				if (checkDiffer2(exp, faultExp, mut1, optiUniformitySet, optiDifferSet, preData)){
					mutants.addMutant(mut1, "tnfdorf", oriterms, faultterms);
					return mutants;
				}

			}

			if (f2){
				string new_term22;
				new_term22 = new_term11 + postterm_1;
				string mut1 = oriExp;
				int pos_1 = mut1.find(terms[i]);
				int pos_2 = oriExp.size();
				if (i + 2 < terms.size()){
					pos_2 = mut1.find(terms[i + 2]) - 1;
				}

			
				//cout << "new_term22: " << new_term22 << endl;
				mut1.replace(mut1.begin() + pos_1, mut1.begin() + pos_2, new_term22);

				/*cout << "DnfMutant.cpp(11582) mut1: " << mut1 << endl;
				cout << "DnfMutant.cpp(11582) mut2: " << mut2 << endl;*/
				faultterms = { new_term22 };
				if (checkDiffer2(exp, faultExp, mut1, optiUniformitySet, optiDifferSet, preData)){
					mutants.addMutant(mut1, "tnfdorf", oriterms, faultterms);
					return mutants;
				}

			}
		}
		return mutants;
	}


	Mutants DnfMutant::diagnosisSingleTermCORFxLIFdoublemutants(
		string exp,
		string faultExp,//待测表达式
		hash_set<string>& outMutant,//排除的变体格式
		vector<vector<string>> optiUniformitySet,//最优无效点
		vector<vector<string>> optiDifferSet,//最优有效点
		hash_map<string, HierarchyNode>& hierarchyMap,
		vector<int> termsOccur,//可能发生缺陷的项
		PRE_PROCESS_DATA& preData
		)
	{
		Mutants mutants(exp);
		bool vars[26];
		for (int i = 0; i < 26; i++) {
			vars[i] = false;
		}

		string oriExp(exp);
		Utility uti;
		vector<string> terms;
		vector<vector<string>> terms_literals;
		vector<string> oriterms;
		vector<string> faultterms;
		uti.mutantsPreproccess(oriExp, terms, terms_literals, vars);


		//遍历term
		for (int t = 0; t < termsOccur.size(); t++)
			//for (int i = 0; i < terms.size(); i++)//【遍历term】
		{
			int i = termsOccur.at(t);
			oriterms.clear();
			oriterms = { terms[i] };
			int pos = oriExp.find(terms[i]);//项在表达式中开始的位置
			string new_term11 = terms[i];
			string new_term12 = terms[i];

			//first corf
			vector<string> literals_in_this_term1 = uti.extractCNFstyle(new_term11);
			string newTerm1 = literals_in_this_term1[0];
			for (int j = 1; j < literals_in_this_term1.size(); j++)
			{
				newTerm1 = newTerm1 + "+" + literals_in_this_term1[j];
			}

			//second lif
			for (int j = 0; j < 26; j++)//【遍历不在当前项中的词literals】
			{
				char toInsert1 = 'a' + j;
				if (vars[j] && terms[i].find(toInsert1) == string::npos)//不在当前项中的词literal
				{
					string s11, s12;
					s11 += toInsert1;//不在当前项中的词literals
					s12 += '!';
					s12 += toInsert1;


					vector<string> sub_newTerm1 = uti.extractDNFstyle(newTerm1);
					for (int k = 0; k < sub_newTerm1.size(); ++k){
						string mut1 = oriExp;
						string new_term21 = sub_newTerm1[k] + s11;
						//string new_term22 = sub_newTerm1[k] + s12;
						string newTerm2;
						if (k == 0){
							newTerm2 = new_term21;
						}
						else{
							newTerm2 = sub_newTerm1[0];
						}
						for (int m = 1; m < sub_newTerm1.size(); ++m){
							if (m == k){
								newTerm2 = newTerm2 + "+" + new_term21;
							}
							else{
								newTerm2 = newTerm2 + "+" + sub_newTerm1[m];
							}
						}
						mut1.replace(mut1.begin() + pos, mut1.begin() + pos + terms[i].size(), newTerm2);
						faultterms = { newTerm2 };
						if (checkDiffer2(exp, faultExp, mut1, optiUniformitySet, optiDifferSet, preData)){
							mutants.addMutant(mut1, "corflif", oriterms, faultterms);
							return mutants;
						}
					}

					for (int k = 0; k < sub_newTerm1.size(); ++k){
						string mut1 = oriExp;
						string new_term21 = sub_newTerm1[k] + s12;

						string newTerm2;
						if (k == 0){
							newTerm2 = new_term21;
						}
						else{
							newTerm2 = sub_newTerm1[0];
						}
						for (int m = 1; m < sub_newTerm1.size(); ++m){
							if (m == k){
								newTerm2 = newTerm2 + "+" + new_term21;
							}
							else{
								newTerm2 = newTerm2 + "+" + sub_newTerm1[m];
							}
						}
						mut1.replace(mut1.begin() + pos, mut1.begin() + pos + terms[i].size(), newTerm2);
						faultterms = { newTerm2 };
						if (checkDiffer2(exp, faultExp, mut1, optiUniformitySet, optiDifferSet, preData)){
							mutants.addMutant(mut1, "corflif", oriterms, faultterms);
							return mutants;
						}
					}
				}
			}
		}
		return mutants;
	}

	Mutants DnfMutant::diagnosisSingleTermCORFxLRFdoublemutants(
		string exp,
		string faultExp,//待测表达式
		hash_set<string>& outMutant,//排除的变体格式
		vector<vector<string>> optiUniformitySet,//最优无效点
		vector<vector<string>> optiDifferSet,//最优有效点
		hash_map<string, HierarchyNode>& hierarchyMap,
		vector<int> termsOccur,//可能发生缺陷的项
		PRE_PROCESS_DATA& preData
		)
	{
		Mutants mutants(exp);
		bool vars[26];
		for (int i = 0; i < 26; i++) {
			vars[i] = false;
		}

		string oriExp(exp);
		Utility uti;
		vector<string> terms;
		vector<vector<string>> terms_literals;
		vector<string> oriterms;
		vector<string> faultterms;
		uti.mutantsPreproccess(oriExp, terms, terms_literals, vars);


		//遍历term
		for (int t = 0; t < termsOccur.size(); t++)
			//for (int i = 0; i < terms.size(); i++)//【遍历term】
		{
			int i = termsOccur.at(t);
			oriterms.clear();
			oriterms = { terms[i] };
			int pos = oriExp.find(terms[i]);//项在表达式中开始的位置
			string new_term11 = terms[i];
			string new_term12 = terms[i];

			//first corf
			vector<string> literals_in_this_term1 = uti.extractCNFstyle(new_term11);
			string newTerm1 = literals_in_this_term1[0];
			for (int j = 1; j < literals_in_this_term1.size(); j++)
			{
				newTerm1 = newTerm1 + "+" + literals_in_this_term1[j];
			}

			//second lrf
			for (int j = 0; j < 26; j++)//【遍历不在当前项中的词literals】
			{
				char toInsert1 = 'a' + j;
				if (vars[j] && terms[i].find(toInsert1) == string::npos)//不在当前项中的词literal
				{
					string s11, s12;
					s11 += toInsert1;//不在当前项中的词literals
					s12 += '!';
					s12 += toInsert1;


					vector<string> sub_newTerm1 = uti.extractDNFstyle(newTerm1);
					for (int k = 0; k < sub_newTerm1.size(); ++k){
						string mut1 = oriExp;
						string new_term21 = s11;
						//string new_term22 = sub_newTerm1[k] + s12;
						string newTerm2;
						if (k == 0){
							newTerm2 = new_term21;
						}
						else{
							newTerm2 = sub_newTerm1[0];
						}
						for (int m = 1; m < sub_newTerm1.size(); ++m){
							if (m == k){
								newTerm2 = newTerm2 + "+" + new_term21;
							}
							else{
								newTerm2 = newTerm2 + "+" + sub_newTerm1[m];
							}
						}
						mut1.replace(mut1.begin() + pos, mut1.begin() + pos + terms[i].size(), newTerm2);
						faultterms = { newTerm2 };
						if (checkDiffer2(exp, faultExp, mut1, optiUniformitySet, optiDifferSet, preData)){
							mutants.addMutant(mut1, "corflif", oriterms, faultterms);
							return mutants;
						}
					}

					for (int k = 0; k < sub_newTerm1.size(); ++k){
						string mut1 = oriExp;
						string new_term21 = s12;

						string newTerm2;
						if (k == 0){
							newTerm2 = new_term21;
						}
						else{
							newTerm2 = sub_newTerm1[0];
						}
						for (int m = 1; m < sub_newTerm1.size(); ++m){
							if (m == k){
								newTerm2 = newTerm2 + "+" + new_term21;
							}
							else{
								newTerm2 = newTerm2 + "+" + sub_newTerm1[m];
							}
						}
						mut1.replace(mut1.begin() + pos, mut1.begin() + pos + terms[i].size(), newTerm2);
						faultterms = { newTerm2 };
						if (checkDiffer2(exp, faultExp, mut1, optiUniformitySet, optiDifferSet, preData)){
							mutants.addMutant(mut1, "corflif", oriterms, faultterms);
							return mutants;
						}
					}
				}
			}
		}
		return mutants;
	}

	Mutants DnfMutant::diagnosisSingleTermCORFxTOFdoublemutants(
		string exp,
		string faultExp,//待测表达式
		hash_set<string>& outMutant,//排除的变体格式
		vector<vector<string>> optiUniformitySet,//最优无效点
		vector<vector<string>> optiDifferSet,//最优有效点
		hash_map<string, HierarchyNode>& hierarchyMap,
		vector<int> termsOccur,//可能发生缺陷的项
		PRE_PROCESS_DATA& preData
		)
	{
		Mutants mutants(exp);
		bool vars[26];
		for (int i = 0; i < 26; i++) {
			vars[i] = false;
		}

		string oriExp(exp);
		Utility uti;
		vector<string> terms;
		vector<vector<string>> terms_literals;
		vector<string> oriterms;
		vector<string> faultterms;
		uti.mutantsPreproccess(oriExp, terms, terms_literals, vars);


		//遍历term
		for (int t = 0; t < termsOccur.size(); t++)
			//for (int i = 0; i < terms.size(); i++)//【遍历term】
		{
			int i = termsOccur.at(t);
			oriterms.clear();
			oriterms = { terms[i] };
			int pos = oriExp.find(terms[i]);//项在表达式中开始的位置
			string new_term11 = terms[i];
			string new_term12 = terms[i];

			//first corf and second tof
			vector<string> literals_in_this_term1 = uti.extractCNFstyle(new_term11);
			//string newTerm1 = literals_in_this_term1[0];
			for (int k = 0; k < literals_in_this_term1.size(); ++k){
				string newTerm1 = literals_in_this_term1[0];
				/*if (k == 0){
					newTerm1 = "";
				}*/
				for (int j = 1; j < literals_in_this_term1.size(); j++)
				{
					if (j != k){
						newTerm1 = newTerm1 + "+" + literals_in_this_term1[j];
					}
				}
				if (k == 0 && newTerm1.size()>=3){
					newTerm1 = newTerm1.substr(2);
				}
				//cout << "newterm1: " << newTerm1 << endl;
				string mut1 = oriExp;
				mut1.replace(mut1.begin() + pos, mut1.begin() + pos + terms[i].size(), newTerm1);
				faultterms = { newTerm1 };
				if (checkDiffer2(exp, faultExp, mut1, optiUniformitySet, optiDifferSet, preData)){
					mutants.addMutant(mut1, "corftof", oriterms, faultterms);
					return mutants;
				}
			}
			
		}
		return mutants;
	}

	Mutants DnfMutant::diagnosisSingleTermCORFxDORFdoublemutants(
		string exp,
		string faultExp,//待测表达式
		hash_set<string>& outMutant,//排除的变体格式
		vector<vector<string>> optiUniformitySet,//最优无效点
		vector<vector<string>> optiDifferSet,//最优有效点
		hash_map<string, HierarchyNode>& hierarchyMap,
		vector<int> termsOccur,//可能发生缺陷的项
		PRE_PROCESS_DATA& preData
		)
	{
		Mutants mutants(exp);
		bool vars[26];
		for (int i = 0; i < 26; i++) {
			vars[i] = false;
		}

		string oriExp(exp);
		Utility uti;
		vector<string> terms;
		vector<vector<string>> terms_literals;
		vector<string> oriterms;
		vector<string> faultterms;
		uti.mutantsPreproccess(oriExp, terms, terms_literals, vars);


		//遍历term
		for (int t = 0; t < termsOccur.size(); t++)
			//for (int i = 0; i < terms.size(); i++)//【遍历term】
		{
			int i = termsOccur.at(t);
			if (terms[i].size()>1){
				oriterms.clear();
				oriterms = { terms[i] };
				int pos = oriExp.find(terms[i]);//项在表达式中开始的位置
				string new_term11 = terms[i];
				string new_term12 = terms[i];

				string preterm, postterm;
				int flag1 = 0, flag2 = 0;
				int pos2, pos3;

				if (i > 0){
					flag1 = 1;
					preterm = terms[i - 1];
					pos2 = oriExp.find(terms[i - 1]);
				}
				if (i < terms.size() - 1){
					flag2 = 1;
					postterm = terms[i + 1];
					pos3 = oriExp.find(terms[i + 1]) + terms[i + 1].size();
				}
				//first corf and second dorf
				vector<string> literals_in_this_term1 = uti.extractCNFstyle(new_term11);
				//string newTerm1 = literals_in_this_term1[0];
				if (flag1){
					string newTerm1 = literals_in_this_term1[0];
					newTerm1 = preterm + newTerm1;
					for (int j = 1; j < literals_in_this_term1.size(); j++)
					{
						newTerm1 = newTerm1 + "+" + literals_in_this_term1[j];
					}
					//cout << "(flag1)newterm1: " << newTerm1 << endl;
					string mut1 = oriExp;
					mut1.replace(mut1.begin() + pos2, mut1.begin() + pos + terms[i].size(), newTerm1);
					//cout << "mut1: " << mut1 << endl;
					faultterms = { newTerm1 };
					if (checkDiffer2(exp, faultExp, mut1, optiUniformitySet, optiDifferSet, preData)){
						mutants.addMutant(mut1, "corfdorf", oriterms, faultterms);
						return mutants;
					}
				}
				if (flag2){
					string newTerm1 = literals_in_this_term1[0];
					for (int j = 1; j < literals_in_this_term1.size(); j++)
					{
						if (j == literals_in_this_term1.size() - 1){
							newTerm1 = newTerm1 + "+" + literals_in_this_term1[j] + postterm;
						}
						else{
							newTerm1 = newTerm1 + "+" + literals_in_this_term1[j];
						}
					}
					//cout << "(flag2)newterm1: " << newTerm1 << endl;
					string mut1 = oriExp;
					mut1.replace(mut1.begin() + pos, mut1.begin() + pos3, newTerm1);
					//cout << "mut1: " << mut1 << endl;
					faultterms = { newTerm1 };
					if (checkDiffer2(exp, faultExp, mut1, optiUniformitySet, optiDifferSet, preData)){
						mutants.addMutant(mut1, "corfdorf", oriterms, faultterms);
						return mutants;
					}
				}

				for (int k = 0; k < literals_in_this_term1.size() - 1; ++k){
					string newTerm1 = literals_in_this_term1[0];
					if (k == 0){
						newTerm1 = newTerm1 + literals_in_this_term1[k + 1];
						for (int j = 2; j < literals_in_this_term1.size(); j++)
						{
							newTerm1 = newTerm1 + "+" + literals_in_this_term1[j];
						}
						//cout << "newterm1: " << newTerm1 << endl;
						string mut1 = oriExp;
						mut1.replace(mut1.begin() + pos, mut1.begin() + pos + terms[i].size(), newTerm1);
						//cout << "mut1: " << mut1 << endl;
						faultterms = { newTerm1 };
						if (checkDiffer2(exp, faultExp, mut1, optiUniformitySet, optiDifferSet, preData)){
							mutants.addMutant(mut1, "corfdorf", oriterms, faultterms);
							return mutants;
						}
					}
					if (k != 0){
						for (int j = 1; j < literals_in_this_term1.size(); j++)
						{
							if (j == k){
								newTerm1 = newTerm1 + "+" + literals_in_this_term1[j] + literals_in_this_term1[j + 1];
								j = j + 1;
							}
							else{
								newTerm1 = newTerm1 + "+" + literals_in_this_term1[j];
							}

						}
						//cout << "newterm1: " << newTerm1 << endl;
						string mut1 = oriExp;
						mut1.replace(mut1.begin() + pos, mut1.begin() + pos + terms[i].size(), newTerm1);
						//cout << "mut1: " << mut1 << endl;
						faultterms = { newTerm1 };
						if (checkDiffer2(exp, faultExp, mut1, optiUniformitySet, optiDifferSet, preData)){
							mutants.addMutant(mut1, "corfdorf", oriterms, faultterms);
							return mutants;
						}
					}

				}
			}

		}
		return mutants;
	}

	Mutants DnfMutant::diagnosisSingleTermCORFxTNFdoublemutants(
		string exp,
		string faultExp,//待测表达式
		hash_set<string>& outMutant,//排除的变体格式
		vector<vector<string>> optiUniformitySet,//最优无效点
		vector<vector<string>> optiDifferSet,//最优有效点
		hash_map<string, HierarchyNode>& hierarchyMap,
		vector<int> termsOccur,//可能发生缺陷的项
		PRE_PROCESS_DATA& preData
		)
	{
		Mutants mutants(exp);
		bool vars[26];
		for (int i = 0; i < 26; i++) {
			vars[i] = false;
		}

		string oriExp(exp);
		Utility uti;
		vector<string> terms;
		vector<vector<string>> terms_literals;
		vector<string> oriterms;
		vector<string> faultterms;
		uti.mutantsPreproccess(oriExp, terms, terms_literals, vars);


		//遍历term
		for (int t = 0; t < termsOccur.size(); t++)
			//for (int i = 0; i < terms.size(); i++)//【遍历term】
		{
			int i = termsOccur.at(t);
			oriterms.clear();
			oriterms = { terms[i] };
			int pos = oriExp.find(terms[i]);//项在表达式中开始的位置
			string new_term11 = terms[i];
			string new_term12 = terms[i];

			//first corf and second tNf
			vector<string> literals_in_this_term1 = uti.extractCNFstyle(new_term11);
			
			//
			for (int k = 0; k < literals_in_this_term1.size(); ++k){
				string newTerm1;
				
				for (int j = 0; j < literals_in_this_term1.size(); j++)
				{
					if (j != k){
						newTerm1 = newTerm1 + "+" + literals_in_this_term1[j];
					}
					else{
						string str;
						if (literals_in_this_term1[j].size()>1){
							str = literals_in_this_term1[j][1];
						}
						else{
							str = str + "!" + literals_in_this_term1[j];
						}
						newTerm1 = newTerm1 + "+" + str;
					}
				}
				newTerm1 = newTerm1.substr(1);
				//cout << "newterm1: " << newTerm1 << endl;
				string mut1 = oriExp;
				mut1.replace(mut1.begin() + pos, mut1.begin() + pos + terms[i].size(), newTerm1);
				faultterms = { newTerm1 };
				if (checkDiffer2(exp, faultExp, mut1, optiUniformitySet, optiDifferSet, preData)){
					mutants.addMutant(mut1, "corftnf", oriterms, faultterms);
					return mutants;
				}
			}

		}
		return mutants;
	}

	Mutants DnfMutant::diagnosisSingleTermTNFxCORFdoublemutants(
		string exp,
		string faultExp,//待测表达式
		hash_set<string>& outMutant,//排除的变体格式
		vector<vector<string>> optiUniformitySet,//最优无效点
		vector<vector<string>> optiDifferSet,//最优有效点
		hash_map<string, HierarchyNode>& hierarchyMap,
		vector<int> termsOccur,//可能发生缺陷的项
		PRE_PROCESS_DATA& preData
		)
	{
		Mutants mutants(exp);
		bool vars[26];
		for (int i = 0; i < 26; i++) {
			vars[i] = false;
		}

		string oriExp(exp);
		Utility uti;
		vector<string> terms;
		vector<vector<string>> terms_literals;
		vector<string> oriterms;
		vector<string> faultterms;
		uti.mutantsPreproccess(oriExp, terms, terms_literals, vars);
		/*string ss = "c!d";
		cout <<"ss: "<< uti.reverseexp(ss) << endl;*/


		//遍历term
		for (int t = 0; t < termsOccur.size(); t++)
			//for (int i = 0; i < terms.size(); i++)//【遍历term】
		{
			int i = termsOccur.at(t);
			oriterms.clear();
			oriterms = { terms[i] };
			int pos = oriExp.find(terms[i]);//项在表达式中开始的位置
			string new_term11 = terms[i];
			string new_term12 = terms[i];

			//first tnf 
			new_term11 = uti.reverseexp(new_term11);
			vector<string> literals_in_this_term1 = uti.extractCNFstyle(new_term11);
			string newTerm1 = literals_in_this_term1[0];
			//corf
			for (int k = 1; k < literals_in_this_term1.size(); ++k){
				newTerm1 = newTerm1 + "+" + literals_in_this_term1[k];
			}

			//cout << "newterm1: " << newTerm1 << endl;
			string mut1 = oriExp;
			mut1.replace(mut1.begin() + pos, mut1.begin() + pos + terms[i].size(), newTerm1);
			faultterms = { newTerm1 };
			if (checkDiffer2(exp, faultExp, mut1, optiUniformitySet, optiDifferSet, preData)){
				mutants.addMutant(mut1, "tnfcorf", oriterms, faultterms);
				return mutants;
			}	
		}
		return mutants;
	}

	Mutants DnfMutant::diagnosisSingleTermENFxLIFdoublemutants(
		string exp,
		string faultExp,//待测表达式
		hash_set<string>& outMutant,//排除的变体格式
		vector<vector<string>> optiUniformitySet,//最优无效点
		vector<vector<string>> optiDifferSet,//最优有效点
		hash_map<string, HierarchyNode>& hierarchyMap,
		vector<int> termsOccur,//可能发生缺陷的项
		PRE_PROCESS_DATA& preData
		)
	{
		Mutants mutants(exp);
		bool vars[26];
		for (int i = 0; i < 26; i++) {
			vars[i] = false;
		}

		string oriExp(exp);
		Utility uti;
		vector<string> terms;
		vector<vector<string>> terms_literals;
		vector<string> oriterms;
		vector<string> faultterms;
		uti.mutantsPreproccess(oriExp, terms, terms_literals, vars);


		//遍历term
		for (int t = 0; t < termsOccur.size(); t++)
			//for (int i = 0; i < terms.size(); i++)//【遍历term】
		{
			int i = termsOccur.at(t);
			oriterms.clear();
			oriterms = { terms[i] };
			int pos = oriExp.find(terms[i]);//项在表达式中开始的位置
			string new_term1 = terms[i];

			//lif
			for (int j = 0; j < 26; j++)//【遍历不在当前项中的词literals】
		    {
				char toInsert1 = 'a' + j;
				if (vars[j] && terms[i].find(toInsert1) == string::npos)//不在当前项中的词literal
				{
					string s11, s12;
					s11 += toInsert1;//不在当前项中的词literals
					s12 += '!';
					s12 += toInsert1;

					string new_term11 = new_term1 + s11;
					string new_term12 = new_term1 + s12;

					string mutant_0 = oriExp, mutant_1 = oriExp;
					//after lif
					mutant_0.replace(mutant_0.begin() + pos, mutant_0.begin() + pos + terms[i].size(), new_term11);
					mutant_1.replace(mutant_1.begin() + pos, mutant_1.begin() + pos + terms[i].size(), new_term12);

					//enf
					int terms_num = terms.size();
					int cur = i;
					for (int k = 2; k <= terms_num; k++){
						int start = 0, end = cur;
						if (cur - k + 1 > 0){
							start = cur - k + 1;
						}
						if (terms_num - k < cur){
							end = terms_num - k;
						}
						for (int m = start; m <= end; m++){
							string mut_1 = mutant_0, mut_2 = mutant_1;
							mut_1 = uti.reverseexp2(mut_1, m, m + k-1);
							//cout << "m: "<<m<<" mut_1: " << mut_1 << endl;
							faultterms = { mut_1 };
							if (checkDiffer2(exp, faultExp, mut_1, optiUniformitySet, optiDifferSet, preData)){
								mutants.addMutant(mut_1, "enflif", oriterms, faultterms);
								return mutants;
							}

							mut_2 = uti.reverseexp2(mut_2, m, m + k - 1);
							//cout << "m: " << m << " mut_1: " << mut_1 << endl;
							faultterms = { mut_2 };
							if (checkDiffer2(exp, faultExp, mut_2, optiUniformitySet, optiDifferSet, preData)){
								mutants.addMutant(mut_2, "enflif", oriterms, faultterms);
								return mutants;
							}
						}
					}
				}
			}
		}
		return mutants;
	}


	Mutants DnfMutant::diagnosisSingleTermENFxLRFdoublemutants(
		string exp,
		string faultExp,//待测表达式
		hash_set<string>& outMutant,//排除的变体格式
		vector<vector<string>> optiUniformitySet,//最优无效点
		vector<vector<string>> optiDifferSet,//最优有效点
		hash_map<string, HierarchyNode>& hierarchyMap,
		vector<int> termsOccur,//可能发生缺陷的项
		PRE_PROCESS_DATA& preData
		)
	{
		Mutants mutants(exp);
		bool vars[26];
		for (int i = 0; i < 26; i++) {
			vars[i] = false;
		}

		string oriExp(exp);
		Utility uti;
		vector<string> terms;
		vector<vector<string>> terms_literals;
		vector<string> oriterms;
		vector<string> faultterms;
		uti.mutantsPreproccess(oriExp, terms, terms_literals, vars);


		//遍历term
		for (int t = 0; t < termsOccur.size(); t++)
			//for (int i = 0; i < terms.size(); i++)//【遍历term】
		{
			int i = termsOccur.at(t);
			oriterms.clear();
			oriterms = { terms[i] };
			int pos = oriExp.find(terms[i]);//项在表达式中开始的位置
			string new_term1 = terms[i];

			vector<string> literals_in_this_term = uti.extractCNFstyle(terms[i]);//获取当前项中的词

			//lrf
			for (int j = 0; j < 26; j++)//【遍历不在当前项中的词literals】
			{
				char toInsert1 = 'a' + j;
				if (vars[j] && terms[i].find(toInsert1) == string::npos)//不在当前项中的词literal
				{
					string s11, s12;
					s11 += toInsert1;//不在当前项中的词literals
					s12 += '!';
					s12 += toInsert1;
					//cout <<"i= "<< i << " lite:  " << toInsert1 << endl;

					for (size_t p = 0; p < literals_in_this_term.size(); p++){
						string new_term11 = terms[i];//当前项
						string new_term12 = terms[i];

						//LRF
						int literal_pos = new_term11.find(literals_in_this_term[p]);
						new_term11.replace(new_term11.begin() + literal_pos, new_term11.begin() + literal_pos + literals_in_this_term[p].size(), s11);
						new_term12.replace(new_term12.begin() + literal_pos, new_term12.begin() + literal_pos + literals_in_this_term[p].size(), s12);

						string mutant_0 = oriExp, mutant_1 = oriExp;
						//cout <<pos<<"|||"<< mutant_0 << "---------" << mutant_1 << endl;
						//after lrf
						mutant_0.replace(mutant_0.begin() + pos, mutant_0.begin() + pos + terms[i].size(), new_term11);
						mutant_1.replace(mutant_1.begin() + pos, mutant_1.begin() + pos + terms[i].size(), new_term12);
						//cout << i << "|||" << s11 << "||" << literal_pos << "|||" << new_term11 << "||||" << new_term12 << endl;
						//enf
						int terms_num = terms.size();
						int cur = i;
						for (int k = 2; k <= terms_num; k++){
							int start = 0, end = cur;
							if (cur - k + 1 > 0){
								start = cur - k + 1;
							}
							if (terms_num - k < cur){
								end = terms_num - k;
							}
							for (int m = start; m <= end; m++){
								string mut_1 = mutant_0, mut_2 = mutant_1;
								mut_1 = uti.reverseexp2(mut_1, m, m + k - 1);
								//cout << "m: "<<m<<" mut_1: " << mut_1 << endl;
								faultterms = { mut_1 };
								if (checkDiffer2(exp, faultExp, mut_1, optiUniformitySet, optiDifferSet, preData)){
									mutants.addMutant(mut_1, "enflrf", oriterms, faultterms);
									return mutants;
								}

								mut_2 = uti.reverseexp2(mut_2, m, m + k - 1);
								//cout << "m: " << m << " mut_1: " << mut_1 << endl;
								faultterms = { mut_2 };
								if (checkDiffer2(exp, faultExp, mut_2, optiUniformitySet, optiDifferSet, preData)){
									mutants.addMutant(mut_2, "enflrf", oriterms, faultterms);
									return mutants;
								}
							}
						}

					}
				}
			}
		}
		return mutants;
	}

		Mutants DnfMutant::diagnosisSingleTermENFxLOFdoublemutants(
			string exp,
			string faultExp,//待测表达式
			hash_set<string>& outMutant,//排除的变体格式
			vector<vector<string>> optiUniformitySet,//最优无效点
			vector<vector<string>> optiDifferSet,//最优有效点
			hash_map<string, HierarchyNode>& hierarchyMap,
			vector<int> termsOccur,//可能发生缺陷的项
			PRE_PROCESS_DATA& preData
			)
		{
			Mutants mutants(exp);
			bool vars[26];
			for (int i = 0; i < 26; i++) {
				vars[i] = false;
			}

			string oriExp(exp);
			Utility uti;
			vector<string> terms;
			vector<vector<string>> terms_literals;
			vector<string> oriterms;
			vector<string> faultterms;
			uti.mutantsPreproccess(oriExp, terms, terms_literals, vars);


			//遍历term
			for (int t = 0; t < termsOccur.size(); t++)
				//for (int i = 0; i < terms.size(); i++)//【遍历term】
			{
				int i = termsOccur.at(t);
				oriterms.clear();
				oriterms = { terms[i] };
				int pos = oriExp.find(terms[i]);//项在表达式中开始的位置
				string new_term1 = terms[i];

				for (int j = 0; j < terms_literals.at(i).size(); j++){
					//lof
					string temp_term = new_term1;
					string s1 = terms_literals.at(i).at(j);
					int pos1 = temp_term.find(s1);
					temp_term.replace(pos1, s1.size(), "");
					
					string mutant_0 = oriExp;
					mutant_0.replace(mutant_0.begin() + pos, mutant_0.begin() + pos + terms[i].size(), temp_term);
					//enf
					int terms_num = terms.size();
					int cur = i;
					for (int k = 2; k <= terms_num; k++){
						int start = 0, end = cur;
						if (cur - k + 1 > 0){
							start = cur - k + 1;
						}
						if (terms_num - k < cur){
							end = terms_num - k;
						}
						for (int m = start; m <= end; m++){
							string mut_1 = mutant_0;
							mut_1 = uti.reverseexp2(mut_1, m, m + k - 1);
							//cout << "m: "<<m<<" mut_1: " << mut_1 << endl;
							faultterms = { mut_1 };
							if (checkDiffer2(exp, faultExp, mut_1, optiUniformitySet, optiDifferSet, preData)){
								mutants.addMutant(mut_1, "enflof", oriterms, faultterms);
								return mutants;
							}
						}
					}
				}

			}
			return mutants;
		}

		Mutants DnfMutant::diagnosisSingleTermENFxLNFdoublemutants(
			string exp,
			string faultExp,//待测表达式
			hash_set<string>& outMutant,//排除的变体格式
			vector<vector<string>> optiUniformitySet,//最优无效点
			vector<vector<string>> optiDifferSet,//最优有效点
			hash_map<string, HierarchyNode>& hierarchyMap,
			vector<int> termsOccur,//可能发生缺陷的项
			PRE_PROCESS_DATA& preData
			)
		{
			Mutants mutants(exp);
			bool vars[26];
			for (int i = 0; i < 26; i++) {
				vars[i] = false;
			}

			string oriExp(exp);
			Utility uti;
			vector<string> terms;
			vector<vector<string>> terms_literals;
			vector<string> oriterms;
			vector<string> faultterms;
			uti.mutantsPreproccess(oriExp, terms, terms_literals, vars);


			//遍历term
			for (int t = 0; t < termsOccur.size(); t++)
				//for (int i = 0; i < terms.size(); i++)//【遍历term】
			{
				int i = termsOccur.at(t);
				oriterms.clear();
				oriterms = { terms[i] };
				int pos = oriExp.find(terms[i]);//项在表达式中开始的位置
				string new_term1 = terms[i];

				for (int j = 0; j < terms_literals.at(i).size(); j++){
					//lnf
					string temp_term = new_term1;
					string s1 = terms_literals.at(i).at(j);//第j个literal
					string s2 = uti.getLiteralNegation(s1);//获取单词的Negation
					int pos1 = temp_term.find(s1);
					temp_term.replace(pos1, s1.size(), s2);//temp_term1为 LNF 后的项

					string mutant_0 = oriExp;
					mutant_0.replace(mutant_0.begin() + pos, mutant_0.begin() + pos + terms[i].size(), temp_term);
					//enf
					int terms_num = terms.size();
					int cur = i;
					for (int k = 2; k <= terms_num; k++){
						int start = 0, end = cur;
						if (cur - k + 1 > 0){
							start = cur - k + 1;
						}
						if (terms_num - k < cur){
							end = terms_num - k;
						}
						for (int m = start; m <= end; m++){
							string mut_1 = mutant_0;
							mut_1 = uti.reverseexp2(mut_1, m, m + k - 1);
							//cout << "m: "<<m<<" mut_1: " << mut_1 << endl;
							faultterms = { mut_1 };
							if (checkDiffer2(exp, faultExp, mut_1, optiUniformitySet, optiDifferSet, preData)){
								mutants.addMutant(mut_1, "enflnf", oriterms, faultterms);
								return mutants;
							}
						}
					}
				}

			}
			return mutants;
		}

		Mutants DnfMutant::diagnosisSingleTermENFxTOFdoublemutants(
			string exp,
			string faultExp,//待测表达式
			hash_set<string>& outMutant,//排除的变体格式
			vector<vector<string>> optiUniformitySet,//最优无效点
			vector<vector<string>> optiDifferSet,//最优有效点
			hash_map<string, HierarchyNode>& hierarchyMap,
			vector<int> termsOccur,//可能发生缺陷的项
			PRE_PROCESS_DATA& preData
			)
		{
			Mutants mutants(exp);
			bool vars[26];
			for (int i = 0; i < 26; i++) {
				vars[i] = false;
			}

			string oriExp(exp);
			Utility uti;
			vector<string> terms;
			vector<vector<string>> terms_literals;
			vector<string> oriterms;
			vector<string> faultterms;
			uti.mutantsPreproccess(oriExp, terms, terms_literals, vars);


			//遍历term
			for (int t = 0; t < termsOccur.size(); t++)
				//for (int i = 0; i < terms.size(); i++)//【遍历term】
			{
				int i = termsOccur.at(t);
				oriterms.clear();
				oriterms = { terms[i] };
				int pos = oriExp.find(terms[i]);//项在表达式中开始的位置
				string new_term1 = terms[i];

				//enf
				int terms_num = terms.size();
				int cur = i;
				for (int k = 2; k <= terms_num; k++){
					int start = 0, end = cur;
					if (cur - k + 1 > 0){
						start = cur - k + 1;
					}
					if (terms_num - k < cur){
						end = terms_num - k;
					}
					for (int m = start; m <= end; m++){
						string mut_1 = oriExp;
						mut_1 = uti.reverseexp2(mut_1, m, m + k - 1);

						//TOF
						vector<string> new_terms = uti.extractDNFstyle(mut_1);
						string mut_2="";
						for (int j = 0; j < new_terms.size(); j++){
							if (j != i){
								mut_2 = mut_2 + "+" + new_terms[j];
							}
						}
						mut_2 = mut_2.substr(1);
						//cout << "mut_2: " << mut_2 << endl;
						//cout << "m: "<<m<<" mut_1: " << mut_1 << endl;
						faultterms = { mut_2 };
						if (checkDiffer2(exp, faultExp, mut_2, optiUniformitySet, optiDifferSet, preData)){
							mutants.addMutant(mut_2, "enftof", oriterms, faultterms);
							return mutants;
						}
					}
				}
			}
			return mutants;
		}
	

		Mutants DnfMutant::diagnosisSingleTermENFxTNFdoublemutants(
			string exp,
			string faultExp,//待测表达式
			hash_set<string>& outMutant,//排除的变体格式
			vector<vector<string>> optiUniformitySet,//最优无效点
			vector<vector<string>> optiDifferSet,//最优有效点
			hash_map<string, HierarchyNode>& hierarchyMap,
			vector<int> termsOccur,//可能发生缺陷的项
			PRE_PROCESS_DATA& preData
			)
		{
			Mutants mutants(exp);
			bool vars[26];
			for (int i = 0; i < 26; i++) {
				vars[i] = false;
			}

			string oriExp(exp);
			Utility uti;
			vector<string> terms;
			vector<vector<string>> terms_literals;
			vector<string> oriterms;
			vector<string> faultterms;
			uti.mutantsPreproccess(oriExp, terms, terms_literals, vars);


			//遍历term
			for (int t = 0; t < termsOccur.size(); t++)
				//for (int i = 0; i < terms.size(); i++)//【遍历term】
			{
				int i = termsOccur.at(t);
				oriterms.clear();
				oriterms = { terms[i] };
				int pos = oriExp.find(terms[i]);//项在表达式中开始的位置
				string new_term1 = terms[i];

				//enf
				int terms_num = terms.size();
				int cur = i;
				for (int k = 2; k <= terms_num; k++){
					int start = 0, end = cur;
					if (cur - k + 1 > 0){
						start = cur - k + 1;
					}
					if (terms_num - k < cur){
						end = terms_num - k;
					}
					for (int m = start; m <= end; m++){
						string mut_1 = oriExp;
						mut_1 = uti.reverseexp2(mut_1, m, m + k - 1);

						//TnF
						string mut_2 = uti.reverseexp2(mut_1, i, i);
						//cout << "mut_2: " << mut_2 << endl;
						//cout << "m: "<<m<<" mut_1: " << mut_1 << endl;
						faultterms = { mut_2 };
						if (checkDiffer2(exp, faultExp, mut_2, optiUniformitySet, optiDifferSet, preData)){
							mutants.addMutant(mut_2, "enftof", oriterms, faultterms);
							return mutants;
						}
					}
				}
			}
			return mutants;
		}

		Mutants DnfMutant::diagnosisSingleTermENFxCORFdoublemutants(
			string exp,
			string faultExp,//待测表达式
			hash_set<string>& outMutant,//排除的变体格式
			vector<vector<string>> optiUniformitySet,//最优无效点
			vector<vector<string>> optiDifferSet,//最优有效点
			hash_map<string, HierarchyNode>& hierarchyMap,
			vector<int> termsOccur,//可能发生缺陷的项
			PRE_PROCESS_DATA& preData
			)
		{
			Mutants mutants(exp);
			bool vars[26];
			for (int i = 0; i < 26; i++) {
				vars[i] = false;
			}

			string oriExp(exp);
			Utility uti;
			vector<string> terms;
			vector<vector<string>> terms_literals;
			vector<string> oriterms;
			vector<string> faultterms;
			uti.mutantsPreproccess(oriExp, terms, terms_literals, vars);


			//遍历term
			for (int t = 0; t < termsOccur.size(); t++)
				//for (int i = 0; i < terms.size(); i++)//【遍历term】
			{
				int i = termsOccur.at(t);
				oriterms.clear();
				oriterms = { terms[i] };
				int pos = oriExp.find(terms[i]);//项在表达式中开始的位置
				string new_term1 = terms[i];

				//enf
				int terms_num = terms.size();
				int cur = i;
				for (int k = 2; k <= terms_num; k++){
					int start = 0, end = cur;
					if (cur - k + 1 > 0){
						start = cur - k + 1;
					}
					if (terms_num - k < cur){
						end = terms_num - k;
					}
					for (int m = start; m <= end; m++){
						string mut_1 = oriExp;
						mut_1 = uti.reverseexp2(mut_1, m, m + k - 1);

						//CORF
						vector<string> new_terms = uti.extractDNFstyle(mut_1);
						vector<string> literals_in_this_term1 = uti.extractCNFstyle(new_terms[i]);
						string new_term = literals_in_this_term1[0];
						for (int j = 1; j < literals_in_this_term1.size(); j++){
							new_term = new_term + "+" + literals_in_this_term1[j];
						}
						string mut_2 = "";
						for (int j = 0; j < new_terms.size(); j++){
							if (j != i){
								mut_2 = mut_2 + "+" + new_terms[j];
							}
							else{
								mut_2 = mut_2 + "+" + new_term;
							}
						}
						mut_2 = mut_2.substr(1);
						//cout << "mut_2: " << mut_2 << endl;
						//cout << "m: "<<m<<" mut_1: " << mut_1 << endl;
						faultterms = { mut_2 };
						if (checkDiffer2(exp, faultExp, mut_2, optiUniformitySet, optiDifferSet, preData)){
							mutants.addMutant(mut_2, "enfcorf", oriterms, faultterms);
							return mutants;
						}
					}
				}
			}
			return mutants;
		}

		Mutants DnfMutant::diagnosisSingleTermENFxDORFdoublemutants(
			string exp,
			string faultExp,//待测表达式
			hash_set<string>& outMutant,//排除的变体格式
			vector<vector<string>> optiUniformitySet,//最优无效点
			vector<vector<string>> optiDifferSet,//最优有效点
			hash_map<string, HierarchyNode>& hierarchyMap,
			vector<int> termsOccur,//可能发生缺陷的项
			PRE_PROCESS_DATA& preData
			)
		{
			Mutants mutants(exp);
			bool vars[26];
			for (int i = 0; i < 26; i++) {
				vars[i] = false;
			}

			string oriExp(exp);
			Utility uti;
			vector<string> terms;
			vector<vector<string>> terms_literals;
			vector<string> oriterms;
			vector<string> faultterms;
			uti.mutantsPreproccess(oriExp, terms, terms_literals, vars);


			//遍历term
			for (int t = 0; t < termsOccur.size(); t++)
				//for (int i = 0; i < terms.size(); i++)//【遍历term】
			{
				int i = termsOccur.at(t);
				oriterms.clear();
				oriterms = { terms[i] };
				int pos = oriExp.find(terms[i]);//项在表达式中开始的位置
				string new_term1 = terms[i];

				//enf
				int terms_num = terms.size();
				int cur = i;
				for (int k = 2; k <= terms_num; k++){
					int start = 0, end = cur;
					if (cur - k + 1 > 0){
						start = cur - k + 1;
					}
					if (terms_num - k < cur){
						end = terms_num - k;
					}
					for (int m = start; m <= end; m++){
						string mut_1 = oriExp;
						mut_1 = uti.reverseexp2(mut_1, m, m + k - 1);

						//dORF
						vector<string> new_terms = uti.extractDNFstyle(mut_1);
						string mut_2 = "";
						for (int j = 0; j < new_terms.size(); j++){
							if (j != i){
								mut_2 = mut_2 + "+" + new_terms[j];
							}
							else if (j + 1 < new_terms.size()){
								mut_2 = mut_2 + "+" + new_terms[j] + new_terms[j + 1];
								j = j + 1;
							}
						}
						mut_2 = mut_2.substr(1);
						//cout << "mut_2: " << mut_2 << endl;
						//cout << "m: "<<m<<" mut_1: " << mut_1 << endl;
						faultterms = { mut_2 };
						if (checkDiffer2(exp, faultExp, mut_2, optiUniformitySet, optiDifferSet, preData)){
							mutants.addMutant(mut_2, "enfdorf", oriterms, faultterms);
							return mutants;
						}
					}
				}
			}
			return mutants;
		}

		Mutants DnfMutant::diagnosisSingleTermENFxENFdoublemutants(
			string exp,
			string faultExp,//待测表达式
			hash_set<string>& outMutant,//排除的变体格式
			vector<vector<string>> optiUniformitySet,//最优无效点
			vector<vector<string>> optiDifferSet,//最优有效点
			hash_map<string, HierarchyNode>& hierarchyMap,
			vector<int> termsOccur,//可能发生缺陷的项
			PRE_PROCESS_DATA& preData
			)
		{
			Mutants mutants(exp);
			bool vars[26];
			for (int i = 0; i < 26; i++) {
				vars[i] = false;
			}

			string oriExp(exp);
			Utility uti;
			vector<string> terms;
			vector<vector<string>> terms_literals;
			vector<string> oriterms;
			vector<string> faultterms;
			uti.mutantsPreproccess(oriExp, terms, terms_literals, vars);


			//遍历term
			for (int t = 0; t < termsOccur.size(); t++)
				//for (int i = 0; i < terms.size(); i++)//【遍历term】
			{
				int i = termsOccur.at(t);
				oriterms.clear();
				oriterms = { terms[i] };
				int pos = oriExp.find(terms[i]);//项在表达式中开始的位置
				string new_term1 = terms[i];

				//enf
				int terms_num = terms.size();
				int cur = i;
				for (int k = 2; k <= terms_num; k++){
					int start = 0, end = cur;
					if (cur - k + 1 > 0){
						start = cur - k + 1;
					}
					if (terms_num - k < cur){
						end = terms_num - k;
					}
					for (int m = start; m <= end; m++){
						string mut_1 = oriExp;
						mut_1 = uti.reverseexp2(mut_1, m, m + k - 1);

						//ENF
						for (int j = 2; j <= terms_num; j++){
							int start = 0, end = cur;
							if (cur - j + 1 > 0){
								start = cur - j + 1;
							}
							if (terms_num - j < cur){
								end = terms_num - j;
							}
							for (int c = start; c <= end; c++){
								string mut_2 = mut_1;
								mut_2 = uti.reverseexp2(mut_2, c, c + j - 1);
								//cout <<"cur: "<<cur<< " |mut_1: "<<mut_1<<" |mut_2: " << mut_2 << endl;
								faultterms = { mut_2 };
								if (checkDiffer2(exp, faultExp, mut_2, optiUniformitySet, optiDifferSet, preData)){
									mutants.addMutant(mut_2, "enfenf", oriterms, faultterms);
									return mutants;
								}
							}
							
						}
					}
				}
			}
			return mutants;
		}

		Mutants DnfMutant::diagnosisSingleTermTNFxLIFdoublemutants(
			string exp,
			string faultExp,//待测表达式
			hash_set<string>& outMutant,//排除的变体格式
			vector<vector<string>> optiUniformitySet,//最优无效点
			vector<vector<string>> optiDifferSet,//最优有效点
			hash_map<string, HierarchyNode>& hierarchyMap,
			vector<int> termsOccur,//可能发生缺陷的项
			PRE_PROCESS_DATA& preData
			)
		{
			Mutants mutants(exp);
			bool vars[26];
			for (int i = 0; i < 26; i++) {
				vars[i] = false;
			}

			string oriExp(exp);
			Utility uti;
			vector<string> terms;
			vector<vector<string>> terms_literals;
			vector<string> oriterms;
			vector<string> faultterms;
			uti.mutantsPreproccess(oriExp, terms, terms_literals, vars);
			/*string ss = "c!d";
			cout <<"ss: "<< uti.reverseexp(ss) << endl;*/


			//遍历term
			for (int t = 0; t < termsOccur.size(); t++)
				//for (int i = 0; i < terms.size(); i++)//【遍历term】
			{
				int i = termsOccur.at(t);
				oriterms.clear();
				oriterms = { terms[i] };
				int pos = oriExp.find(terms[i]);//项在表达式中开始的位置
				string new_term11 = terms[i];
				string new_term12 = terms[i];

				//first tnf 
				new_term11 = uti.reverseexp(new_term11);
				//lif
				for (int j = 0; j < 26; j++)//【遍历不在当前项中的词literals】
				{
					char toInsert1 = 'a' + j;
					if (vars[j] && terms[i].find(toInsert1) == string::npos)//不在当前项中的词literal
					{
						string s11, s12;
						s11 += toInsert1;//不在当前项中的词literals
						s12 += '!';
						s12 += toInsert1;

						string new_term21 = new_term11 + s11;
						string new_term22 = new_term11 + s12;

						string mutant_0 = oriExp, mutant_1 = oriExp;
						//after lif
						mutant_0.replace(mutant_0.begin() + pos, mutant_0.begin() + pos + terms[i].size(), new_term21);
						//cout << "newterm1: " << newTerm1 << endl;
						faultterms = { new_term21 };
						if (checkDiffer2(exp, faultExp, mutant_0, optiUniformitySet, optiDifferSet, preData)){
							mutants.addMutant(mutant_0, "tnflif", oriterms, faultterms);
							return mutants;
						}
						//
						mutant_1.replace(mutant_1.begin() + pos, mutant_1.begin() + pos + terms[i].size(), new_term22);
						faultterms = { new_term22 };
						if (checkDiffer2(exp, faultExp, mutant_1, optiUniformitySet, optiDifferSet, preData)){
							mutants.addMutant(mutant_1, "tnflif", oriterms, faultterms);
							return mutants;
						}
					}
				}
			}
			return mutants;
		}

		Mutants DnfMutant::diagnosisSingleTermTNFxLRFdoublemutants(
			string exp,
			string faultExp,//待测表达式
			hash_set<string>& outMutant,//排除的变体格式
			vector<vector<string>> optiUniformitySet,//最优无效点
			vector<vector<string>> optiDifferSet,//最优有效点
			hash_map<string, HierarchyNode>& hierarchyMap,
			vector<int> termsOccur,//可能发生缺陷的项
			PRE_PROCESS_DATA& preData
			)
		{
			Mutants mutants(exp);
			bool vars[26];
			for (int i = 0; i < 26; i++) {
				vars[i] = false;
			}

			string oriExp(exp);
			Utility uti;
			vector<string> terms;
			vector<vector<string>> terms_literals;
			vector<string> oriterms;
			vector<string> faultterms;
			uti.mutantsPreproccess(oriExp, terms, terms_literals, vars);
			/*string ss = "c!d";
			cout <<"ss: "<< uti.reverseexp(ss) << endl;*/


			//遍历term
			for (int t = 0; t < termsOccur.size(); t++)
				//for (int i = 0; i < terms.size(); i++)//【遍历term】
			{
				int i = termsOccur.at(t);
				oriterms.clear();
				oriterms = { terms[i] };
				int pos = oriExp.find(terms[i]);//项在表达式中开始的位置
				string new_term11 = terms[i];
				string new_term12 = terms[i];

				//first tnf 
				new_term11 = uti.reverseexp(new_term11);
				//lrf
				vector<string> literals_in_this_term = uti.extractCNFstyle(new_term11);//获取当前项中的词
				for (int j = 0; j < 26; j++)//【遍历不在当前项中的词literals】
				{
					char toInsert1 = 'a' + j;
					if (vars[j] && terms[i].find(toInsert1) == string::npos)//不在当前项中的词literal
					{
						string s11, s12;
						s11 += toInsert1;//不在当前项中的词literals
						s12 += '!';
						s12 += toInsert1;

						for (size_t p = 0; p < literals_in_this_term.size(); p++){
							string new_term21 = new_term11;//当前项
							string new_term22 = new_term11;

							//LRF
							int literal_pos = new_term21.find(literals_in_this_term[p]);
							new_term21.replace(new_term21.begin() + literal_pos, new_term21.begin() + literal_pos + literals_in_this_term[p].size(), s11);
							new_term22.replace(new_term22.begin() + literal_pos, new_term22.begin() + literal_pos + literals_in_this_term[p].size(), s12);

							string mutant_0 = oriExp, mutant_1 = oriExp;
							//cout <<pos<<"|||"<< mutant_0 << "---------" << mutant_1 << endl;
							//after lrf
							mutant_0.replace(mutant_0.begin() + pos, mutant_0.begin() + pos + terms[i].size(), new_term21);
							faultterms = { new_term21 };
							if (checkDiffer2(exp, faultExp, mutant_0, optiUniformitySet, optiDifferSet, preData)){
								mutants.addMutant(mutant_0, "tnflrf", oriterms, faultterms);
								return mutants;
							}
							//
							mutant_1.replace(mutant_1.begin() + pos, mutant_1.begin() + pos + terms[i].size(), new_term22);
							faultterms = { new_term22 };
							if (checkDiffer2(exp, faultExp, mutant_1, optiUniformitySet, optiDifferSet, preData)){
								mutants.addMutant(mutant_1, "tnflrf", oriterms, faultterms);
								return mutants;
							}
						}
					}
				}
			}
			return mutants;
		}

		Mutants DnfMutant::diagnosisSingleTermTNFxLNFdoublemutants(
			string exp,
			string faultExp,//待测表达式
			hash_set<string>& outMutant,//排除的变体格式
			vector<vector<string>> optiUniformitySet,//最优无效点
			vector<vector<string>> optiDifferSet,//最优有效点
			hash_map<string, HierarchyNode>& hierarchyMap,
			vector<int> termsOccur,//可能发生缺陷的项
			PRE_PROCESS_DATA& preData
			)
		{
			Mutants mutants(exp);
			bool vars[26];
			for (int i = 0; i < 26; i++) {
				vars[i] = false;
			}

			string oriExp(exp);
			Utility uti;
			vector<string> terms;
			vector<vector<string>> terms_literals;
			vector<string> oriterms;
			vector<string> faultterms;
			uti.mutantsPreproccess(oriExp, terms, terms_literals, vars);
			/*string ss = "c!d";
			cout <<"ss: "<< uti.reverseexp(ss) << endl;*/


			//遍历term
			for (int t = 0; t < termsOccur.size(); t++)
				//for (int i = 0; i < terms.size(); i++)//【遍历term】
			{
				int i = termsOccur.at(t);
				oriterms.clear();
				oriterms = { terms[i] };
				int pos = oriExp.find(terms[i]);//项在表达式中开始的位置
				string new_term11 = terms[i];
				string new_term12 = terms[i];

				//first tnf 
				new_term11 = uti.reverseexp(new_term11);

				//lnf
				vector<string> literals_in_this_term = uti.extractCNFstyle(new_term11);//获取当前项中的词
				for (size_t p = 0; p < literals_in_this_term.size(); p++){
					string new_term21 = new_term11;//当前项

					//LnF
					string s1 = literals_in_this_term[p];
					int literal_pos = new_term21.find(literals_in_this_term[p]);
					string s2 = uti.getLiteralNegation(s1);//获取s1单词的Negation
					new_term21.replace(new_term21.begin() + literal_pos, new_term21.begin() + literal_pos + literals_in_this_term[p].size(), s2);
					string mutant_0 = oriExp;
					//cout <<pos<<"|||"<< mutant_0 << "---------" << mutant_1 << endl;
					//after lnf
					mutant_0.replace(mutant_0.begin() + pos, mutant_0.begin() + pos + terms[i].size(), new_term21);
					faultterms = { new_term21 };
					if (checkDiffer2(exp, faultExp, mutant_0, optiUniformitySet, optiDifferSet, preData)){
						mutants.addMutant(mutant_0, "tnflnf", oriterms, faultterms);
						return mutants;
					}
				}
			}
			return mutants;
		}

		//nanjia
		string DnfMutant::generateLRFmutant_inPos(string exp, NFP nfp, Literal replace_literal)
		{
			string oriExp = exp;
			Utility uti;
		
			string term = nfp.term;
			int pos = oriExp.find(term);  //项在原函数中的位置
			//生成替换词格式
			char toReplaceWith = replace_literal.getChar();   //替换词
			string s1;
			if (replace_literal.getPos() == false) s1 += '!';
			s1 += toReplaceWith;

			//生成被替换词格式
			char beReplaceWith = nfp.getKey().getChar();
			string literal_in_this_term;
			if (nfp.getKey().getPos() == false) literal_in_this_term += '!';
			literal_in_this_term += beReplaceWith;

			string new_term = term;  //被替换项
			int literal_pos = new_term.find(literal_in_this_term);
			new_term.replace(new_term.begin() + literal_pos, new_term.begin() + literal_pos + literal_in_this_term.size(), s1);
			string _mutant = oriExp;
			_mutant.replace(_mutant.begin() + pos, _mutant.begin() + pos + term.size(), new_term);
			return _mutant;
		}

		string DnfMutant::generateLRFmutant_inPos_UTP(string exp, UTP utp, Literal replace_literal,Literal be_replaced_literal)
		{
			string oriExp = exp;
			Utility uti;

			string term = utp.term;
			int pos = oriExp.find(term);  //项在原函数中的位置
			//生成替换词格式
			char toReplaceWith = replace_literal.getChar();   //替换词
			string s1;
			if (replace_literal.getPos() == false) s1 += '!';
			s1 += toReplaceWith;

			//生成被替换词格式
			char beReplaceWith = be_replaced_literal.getChar();
			string literal_in_this_term;
			if (be_replaced_literal.getPos() == false) literal_in_this_term += '!';
			literal_in_this_term += beReplaceWith;

			string new_term = term;  //被替换项
			int literal_pos = new_term.find(literal_in_this_term);
			new_term.replace(new_term.begin() + literal_pos, new_term.begin() + literal_pos + literal_in_this_term.size(), s1);
			string _mutant = oriExp;
			_mutant.replace(_mutant.begin() + pos, _mutant.begin() + pos + term.size(), new_term);
			return _mutant;
		}

		string DnfMutant::generateLIFmutant_inPos_UTP(string exp, UTP utp, Literal replace_literal)
		{
			string oriExp = exp;
			Utility uti;

			string term = utp.term;
			int pos = oriExp.find(term);  //项在原函数中的位置
			//生成替换词格式
			char toReplaceWith = replace_literal.getChar();   //替换词
			string s1;
			if (replace_literal.getPos() == false) s1 += '!';
			s1 += toReplaceWith;

			//生成被替换词格式
			/*char beReplaceWith = be_replaced_literal.getChar();
			string literal_in_this_term;
			if (be_replaced_literal.getPos() == false) literal_in_this_term += '!';
			literal_in_this_term += beReplaceWith;*/

			string new_term = term;  //被替换项
			//int literal_pos = new_term.find(literal_in_this_term);
			new_term.append(s1);
			string _mutant = oriExp;
			_mutant.replace(_mutant.begin() + pos, _mutant.begin() + pos + term.size(), new_term);
			return _mutant;
		}

		string DnfMutant::generateLNFmutant_inPos(string exp, UTP utp, Literal insert_literal)
		{
			string mutant = exp;
			//cout << "generateLNFmutant" << endl;
			string fault_term = utp.term;
			char l = insert_literal.getChar();
			string fault_liter;
			bool ispos = insert_literal.getPos();
			string replace_liter;
			if (ispos){
				replace_liter += '!';
				replace_liter += l;
				fault_liter += l;
			}
			else{
				replace_liter += l;
				fault_liter += '!';
				fault_liter += l;
			}

			string replace_term = fault_term;
			int pos = replace_term.find(fault_liter);
			replace_term.replace(replace_term.begin() + pos, replace_term.begin() + pos + fault_liter.size(), replace_liter);
			int posi = mutant.find(fault_term);
			mutant.replace(mutant.begin() + posi, mutant.begin() + posi + fault_term.size(), replace_term);
			return mutant;
		}

		string DnfMutant::generateLOFmutant(string exp, NFP nfp){

			//cout << "generateLOFmutant" << endl;
			Utility uti;
			string fault_term = nfp.term;
			char omit_literal = nfp.getKey().getChar();
			string omitL;
			int omitLen = 1;
			if (nfp.getKey().getPos() == false){
				omitL += '!';
				omitLen = 2;
			}
			omitL += omit_literal;
			string newTerm = fault_term;
			int omitLPos = newTerm.find(omitL);
			newTerm.replace(newTerm.begin() + omitLPos, newTerm.begin() + omitLPos + omitLen, "");
			string _mutant = exp;
			int termPos = _mutant.find(fault_term);
			_mutant.replace(_mutant.begin() + termPos, _mutant.begin() + termPos + fault_term.size(), newTerm);
			return _mutant;

		}
		string DnfMutant::generateCORFmutants(string exp, NFP nfp, Literal literal){
			//cout << "generateCORFmutant" << endl;
			//literal 在项的开头+放后面
			//literal 不在项的开头 + 放后面			
			string _mutant = exp;
			string fault_term = nfp.term;
			char fault_liter = literal.getChar();
			string fault_literal;
			if (literal.getPos() == false) fault_literal += '!';
			fault_literal += fault_liter;
			int pos = fault_term.find(fault_literal);
			string new_term = fault_term;
			string new_liter;
			//cout << "pos:::::" << pos << endl;
			if (pos == 0){
				new_liter = fault_literal + "+";
			}
			else{
				new_liter = "+" + fault_literal;
			}
			new_term.replace(new_term.begin() + pos, new_term.begin() + pos + fault_literal.size(), new_liter);
			int termPos = exp.find(fault_term);
			_mutant.replace(_mutant.begin() + termPos, _mutant.begin() + termPos + fault_term.size(), new_term);
			return _mutant;
		}

		string DnfMutant::generateDORFmutants(string exp, string term, string utpterm){

			//cout << "generateDORFmutant" << endl;
			//cout << "term is " << term << " utpterm is " << utpterm << endl;
			//结果为term和utpterm的合并
			string mutant = exp;
			int pos1 = exp.find(term);
			int pos2 = exp.find(utpterm);
			//cout << "pos1 is " << pos1 << " pos2 is " << pos2 << endl;
			int startpos;
			string newTerm;
			if (pos1 < pos2){
				startpos = pos1;
				newTerm = term + utpterm;
			}
			else{
				startpos = pos2;
				newTerm = utpterm + term;
			}
			mutant.replace(mutant.begin() + startpos, mutant.begin() + startpos + term.length() + utpterm.length() + 1, newTerm);
			return mutant;

		}
		string DnfMutant::generateTOFmutants(string exp, string term){
			//cout << "generateTOFmutant" << endl;
			string mutant = exp;
			int pos = exp.find(term);
			if (pos == 0){
				//to be omitted term is the first,delete '+' after the term 
				mutant.replace(mutant.begin() + pos, mutant.begin() + pos + term.length() + 1, "");
			}
			else{
				mutant.replace(mutant.begin() + pos - 1, mutant.begin() + pos + term.length(), "");
			}
			return mutant;

		}
		string DnfMutant::generateENFmutant(string exp){
			string mutant = "!(";
			mutant += exp;
			mutant += ")";
			return mutant;
		}
		vector<string> DnfMutant::generateTNFmutant(string exp, UTP utp){
			vector<string> res;
			string fault_term = utp.term;
			int pos = exp.find(fault_term);
			// !a 生成 a 和 !(!a)
			if (fault_term.at(0) == '!' && fault_term.size() == 2){
				string neg = "";
				neg = fault_term.at(1);
				string mu1 = exp;
				mu1.replace(mu1.begin() + pos, mu1.begin() + pos + fault_term.size(), neg);
				res.push_back(mu1);
			}
			// a 生成 !a 和 !(a)
			else if (fault_term.size() == 1){
				string neg = "!" + fault_term;
				string mu1 = exp;
				mu1.replace(mu1.begin() + pos, mu1.begin() + pos + fault_term.size(), neg);
				res.push_back(mu1);
			}
			//生成 !( term )的一般情况
			string negative_term = "";
			negative_term = "!(" + fault_term + ")";
			string mutant = exp;
			res.push_back(mutant.replace(mutant.begin() + pos, mutant.begin() + pos + fault_term.size(), negative_term));
			return res;
		}