#pragma once
#include <string>
#include <vector>
using namespace std;

class HierarchyNode
{
public:
	HierarchyNode(string);
	HierarchyNode();
	//~HierarchyNode(void);

	void setValue(string v){ value = v; };
	string getValue()const{ return value; };

	void setNextCondition(vector<int> v){ nextConditon = v; };
	vector<int> getNextCondition(){ return nextConditon; };

	void setNeedliteral(bool v){ needliteral = v; };
	int getNeedliteral()const{ return needliteral; };

	void setIsOut(bool v){ isOut = v; };
	int getIsOut()const{ return isOut; };

	void setIsDisorder(bool v){ isDisorder = v; };
	int getIsDisorder()const{ return isDisorder; };

	void setNext(vector<HierarchyNode *> v){ next = v; };
	vector<HierarchyNode *> getNext(){ return next; };

	void setEqualNode(HierarchyNode * v){ equalNode = v; };
	HierarchyNode* getEqualNode(){ return equalNode; };

	bool operator==(const HierarchyNode & my) const{
		return (value == my.value);
	}

	bool operator<(const HierarchyNode & my) const{
		return (value.compare(my.value));
	}







private:
	string value;
	vector<HierarchyNode*> next;
	vector<int> nextConditon;//1扩张2收缩
	HierarchyNode* equalNode;
	bool isOut;
	bool needliteral;//排除时是否需要精确到词；
	bool isDisorder;//是否是无序双缺陷

};

struct hash_HierarchyNode{
	size_t operator()(const class HierarchyNode & A)const{
		return  std::hash<string>()(A.getValue());
		//return A.getvalue();
	}
};

struct equal_HierarchyNode{
	bool operator()(const class HierarchyNode & a1, const class HierarchyNode & a2)const{
		return  a1.getValue().compare(a2.getValue())==0;
	}
};

