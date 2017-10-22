#pragma once
#include <iostream>

struct BVHnode
{
	struct BoundBox box;
	class Shape *shape;
	union
	{
		BVHnode *lchild, *rchild;
		int lchildi, rchildi;
	};
};


class BVH
{
	BVHnode *root;
public:
	void build(std::vector<class Shape *> shapes);
};