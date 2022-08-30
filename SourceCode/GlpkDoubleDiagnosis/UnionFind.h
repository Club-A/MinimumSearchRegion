#ifndef _UNION_FIND_H
#define _UNION_FIND_H
#define _UNION_FIND_SIZE 10000
#include <cassert>

struct UnionFindSet
{
	int* _union_par;
	int* _union_rank;;
	UnionFindSet(int size){
		assert(size>0);
		_union_par = new int[size];
		_union_rank= new int[size];

		for (int i = 0; i < size; i++)
		{
			_union_par[i] = i;
			_union_rank[i] = 0;
		} 
	}
	~UnionFindSet(){
		delete[] _union_par;
		delete[] _union_rank;
	}


	int find(int x)
	{
		if(_union_par[x]==x) return _union_par[x];
		else
		{
			return _union_par[x] = find(_union_par[x]);
		}

	}
	void unite(int x,int y)
	{
		x= find(x);

		y = find(y);

		if(x==y)return;
		if(_union_rank[x] < _union_rank[y]){
			_union_par[x] = y;
		}
		else
		{
			_union_par[y] = x;
			if(_union_rank[x] == _union_rank[y]) _union_rank[x]++;
		}

	}
	bool same(int x,int y)
	{
		return find(x) == find(y);
	}
};

#endif // _UNION_FIND_H
