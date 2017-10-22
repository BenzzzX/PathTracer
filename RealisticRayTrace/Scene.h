#pragma once
#include "Shapes.h"
struct Scene
{
	std::vector<Shape*> shapes;
	Vec3 skyLight{ 0,0,0 };
	double airTranslucent = 0.998;
	int trace(Ray ray, HitRecord& rec) const
	{
		rec.t = INFINITY;
		Shape *hit = nullptr;
		int state = 0;
		for (auto &i : shapes)
		{
			HitRecord reci; int statei = 0;
			if ((statei = i->intersect(ray, reci)) != 0 && reci.t < rec.t)
			{
				rec = reci;
				hit = i;
				state = statei;
			}
		}
		return state;
	}
	void add(Shape* s) { shapes.push_back(s); }
};