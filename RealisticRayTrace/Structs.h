#pragma once
#include <cmath>
#include <string>
#define M_PI 3.14159265358979323846
struct Vec3
{
	double x, y, z;
	Vec3();
	Vec3(double f);
	Vec3(double a, double b, double c);
	Vec3(const Vec3& r);
	Vec3 operator - () const;
	double length() const;
	Vec3& operator = (const Vec3& r);
	Vec3& operator += (const Vec3& r);
	Vec3& operator -= (const Vec3& r);
	Vec3& operator *= (double r);
	Vec3& operator /= (double r);
	Vec3& operator *= (const Vec3& r);
	double &operator [] (int i);
	friend inline Vec3 operator + (const Vec3& l, const Vec3& r) { return Vec3(l.x + r.x, l.y + r.y, l.z + r.z); }
	friend inline Vec3 operator - (const Vec3& l, const Vec3& r) { return Vec3(l.x - r.x, l.y - r.y, l.z - r.z); }
	friend inline Vec3 operator * (const Vec3& l,double r) { return Vec3(l.x*r, l.y*r, l.z*r); }
	friend inline Vec3 operator * (double l, const Vec3& r) { return r*l; }
	friend inline Vec3 operator / (const Vec3& l, double r) { return Vec3(l.x/r, l.y/r, l.z/r); }
	friend inline Vec3 operator * (const Vec3& l, const Vec3& r) { return Vec3(l.x*r.x, l.y*r.y, l.z*r.z); }
	friend inline Vec3 cross(const Vec3& l, const Vec3& r) {
		return Vec3(l.y*r.z - l.z*r.y,
			l.z*r.x - l.x*r.z,
			l.x*r.y - l.y*r.x);
	}
	friend inline Vec3 pow(const Vec3& l, double r) { return Vec3(pow(l.x, r), pow(l.y, r), pow(l.z, r)); }
	friend inline double dot(const Vec3& l, const Vec3& r) { return l.x*r.x + l.y*r.y + l.z*r.z; }
	friend inline double tripleProduct(const Vec3& a, const Vec3& b, const Vec3& c) { return dot(cross(a, b), c); }
	friend inline Vec3 normalize(const Vec3& l) { double len = l.length(); return Vec3(l.x / len, l.y / len, l.z / len); }
	
};

struct Transform
{
	Vec3 location, rotation;
	Transform() :location(0), rotation(0) {};
	Transform(Vec3 l, Vec3 r) :location(l), rotation(r) {}
};

struct Bias
{
public:
	Vec3 u, w, v;
	static inline Bias buildFromU(const Vec3& u);
	static inline Bias buildFromV(const Vec3& v);
	static Bias buildFromW(const Vec3& w);
	static Bias buildFromUV(const Vec3& u, const Vec3& v);
	static Bias buildFromUW(const Vec3& u, const Vec3& w);
	static Bias buildFromVW(const Vec3& v, const Vec3& w);
	static Bias buildFromWV(const Vec3& w, const Vec3& v);
	static Bias buildFromWU(const Vec3& w, const Vec3& u);
	static Bias buildFromVU(const Vec3& v, const Vec3& u);
	static Bias buildFromRotator(const Vec3& r); //use matrix to calculate
};



struct Matrix4x4
{
	double v[4][4];
	Matrix4x4();
	Matrix4x4(const Matrix4x4& l);
	Matrix4x4(const double l[4][4]);
	friend Matrix4x4 operator + (const Matrix4x4& l, const Matrix4x4& r);
	friend Matrix4x4 operator - (const Matrix4x4& l, const Matrix4x4& r);
	friend Matrix4x4 operator * (const Matrix4x4& l, const Matrix4x4& r);
	friend Matrix4x4 operator * (const Matrix4x4& l, double r);
	friend Matrix4x4 operator * (double l, const Matrix4x4& r);
	Vec3 transform(const Vec3& r);
	friend Matrix4x4 operator / (const Matrix4x4& l, double r);
	Matrix4x4& operator = (const Matrix4x4& r);
	Matrix4x4& operator += (const Matrix4x4& r);
	Matrix4x4& operator -= (const Matrix4x4& r);
	Matrix4x4& operator *= (const Matrix4x4& r);
	Matrix4x4& operator *= (double r);
	Matrix4x4& operator /= (double r);
	double determinant() const;
	Matrix4x4 getInverse() const;
	Matrix4x4& invert();
	Matrix4x4& transpose();
	Matrix4x4 getTranspose() const;
	static Matrix4x4 indentify();
	static Matrix4x4 translate(const Vec3& r);
	static Matrix4x4 rotate(const Vec3& r);
	static Matrix4x4 fromTransform(const Transform& t);
	static Matrix4x4 rotateX(double angle);
	static Matrix4x4 rotateY(double angle);
	static Matrix4x4 rotateZ(double angle);
	static Matrix4x4 rotateAxis(Vec3 axis, double angle);
	static Matrix4x4 scale(Vec3 r);
	static Matrix4x4 view(const Vec3& eye, const Vec3& forward, const Vec3& up);
};



struct Ray
{
	Vec3 origin, direction;
	Ray(Vec3 o, Vec3 d) :origin(o), direction(d) {}
	Vec3 f(double t) { return origin + direction*t; }
};

struct BoundBox
{
	Vec3 pmin, pmax;
};
