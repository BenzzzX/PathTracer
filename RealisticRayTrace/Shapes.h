#pragma once

#include "Structs.h"
#include "Materials.h"

struct HitRecord
{
	Vec3 normal;
	double t;
	Material::Attribute matAttr;
};

class Shape
{
public:
	Material *mat;
	Shape()
	{
		static Material defaultMaterial;
		mat = &defaultMaterial;
	}
	virtual int intersect(Ray r, HitRecord &h) const = 0;
	virtual BoundBox getBound() const = 0;
};

class Surface : public Shape
{
	Vec3 point, normal;
public:
	Surface(Vec3 a, Vec3 b) :point(a), normal(b) {}
	int intersect(Ray r, HitRecord &h) const override;
	BoundBox getBound() const override;
};

class Triangle : public Shape
{
	Vec3 points[3];
	double us[3], vs[3];
public:
	Triangle(Vec3 a, Vec3 b, Vec3 c) :points{ a,b,c }, us{ 0,0,1 }, vs{ 0,1,0 } {}
	Triangle(
		Vec3 a, double ua, double va,
		Vec3 b, double ub, double vb,
		Vec3 c, double uc, double vc) :points{ a,b,c }, us{ ua,ub,uc }, vs{ va,vb,vc } {}
	int intersect(Ray r, HitRecord &h) const override;
	BoundBox getBound() const override;
};

class Sphere : public Shape
{
	Transform trans;
	double radius;
public:
	Sphere(const Transform& t, double r) :trans(t), radius(r) {}
	int intersect(Ray r, HitRecord &h) const override;
	BoundBox getBound() const override;
};
