#include "Structs.h"

#define Each(i, j) \
	for (int i = 0; i < 4; i++) \
		for (int j = 0; j < 4; j++)
#define Op_Body(op) Matrix4x4 ret; Each(i, j) ret.v[i][j] = op; return ret;

Matrix4x4 operator+(const Matrix4x4& l, const Matrix4x4& r)
{
	Op_Body(l.v[i][j] + r.v[i][j])
}

Matrix4x4 operator-(const Matrix4x4& l, const Matrix4x4& r)
{
	Op_Body(l.v[i][j] - r.v[i][j])
}

Matrix4x4 operator*(const Matrix4x4& l, const Matrix4x4& r)
{
	Matrix4x4 ret;
	Each(i, j)
		for (int k = 0; k < 4; k++)
			ret.v[i][j] += l.v[i][k] * r.v[k][j];
	return ret;
}

Matrix4x4 operator*(const Matrix4x4& l, double r)
{
	Op_Body(l.v[i][j] * r)
}

Matrix4x4 operator*(double l, const Matrix4x4& r)
{
	return r * l;
}

Matrix4x4::Matrix4x4(const double l[4][4])
{
	memcpy(v, l, sizeof(v));
}

Matrix4x4::Matrix4x4(const Matrix4x4& l)
{
	memcpy(v, l.v, sizeof(v));
}

Matrix4x4::Matrix4x4()
{
	memset(v, 0, sizeof(v));
}

Vec3 Matrix4x4::transform(const Vec3& r)
{
	Vec3 ret; double t;
	ret.x = r.x * v[0][0] + r.y * v[0][1] + r.z * v[0][2] + v[0][3];
	ret.y = r.x * v[1][0] + r.y * v[1][1] + r.z * v[1][2] + v[1][3];
	ret.z = r.x * v[2][0] + r.y * v[2][1] + r.z * v[2][2] + v[2][3];
	t = r.x * v[3][0] + r.y * v[3][1] + r.z * v[3][2] + v[3][3];
	return ret /= t;
}

Matrix4x4 operator/(const Matrix4x4& l, double r)
{
	Op_Body(l.v[i][j] / r)
}

#undef  Op_Body
#define Op_Body(op) Each(i, j) v[i][j] = op; return *this;

Matrix4x4& Matrix4x4::operator=(const Matrix4x4& r)
{
	memcpy(v, r.v, sizeof(v));
	return *this;
}

Matrix4x4& Matrix4x4::operator+=(const Matrix4x4& r)
{
	Op_Body(v[i][j] + r.v[i][j])
}

Matrix4x4& Matrix4x4::operator-=(const Matrix4x4& r)
{
	Op_Body(v[i][j] - r.v[i][j])
}

Matrix4x4& Matrix4x4::operator/=(double r)
{
	Op_Body(v[i][j] / r)
}

Matrix4x4& Matrix4x4::operator*=(double r)
{
	Op_Body(v[i][j] * r)
}

Matrix4x4& Matrix4x4::operator*=(const Matrix4x4& r)
{
	Matrix4x4 t(*this);
	Each(i, j)
		for (int k = 0; i < 4; k++)
			v[i][j] += t.v[i][k] + r.v[k][j];
	return *this;
}



double Matrix4x4::determinant() const
{
	double ret = 0.f; double t[9]; int k = 0; double inv = 1.f;
	for (int j = 0; j < 4; j++)
	{
		k = 0;
		Each(m, n) if (m != 0 && n != j) t[k++] = v[m][n];
		ret += inv * t[0] * t[4] * t[8] + t[3] * t[7] * t[2] + t[6] * t[1] * t[5] -
			(t[6] * t[4] * t[2] + t[3] * t[1] * t[8] + t[0] * t[7] * t[5]);
		inv *= -1.f;
	}
	return ret;
}

Matrix4x4 Matrix4x4::getInverse() const
{
	Matrix4x4 ret; double t[9]; int k = 0; double inv = 1.f;
	double det = determinant();
	Each(i, j)
	{
		k = 0;
		Each(m, n) if (m != i&&n != j) t[k++] = v[m][n];
		ret.v[i][j] = (inv * t[0] * t[4] * t[8] + t[3] * t[7] * t[2] + t[6] * t[1] * t[5] -
			(t[6] * t[4] * t[2] + t[3] * t[1] * t[8] + t[0] * t[7] * t[5])) / det;
		inv *= -1.f;
	}
	return ret;
}

Matrix4x4& Matrix4x4::invert()
{
	return *this = getInverse();
}

Matrix4x4& Matrix4x4::transpose()
{
	Each(i, j) std::swap(v[i][j], v[j][i]); return *this;
}
#undef Op_Body
#undef Each
Matrix4x4 Matrix4x4::getTranspose() const
{
	return Matrix4x4(*this).transpose();
}

Matrix4x4 Matrix4x4::indentify()
{

	double t[4][4] = {
		{1.f, 0.f ,0.f , 0.f},
		{0.f, 1.f ,0.f , 0.f},
		{0.f, 0.f ,1.f , 0.f},
		{0.f, 0.f ,0.f , 1.f} };
	return Matrix4x4(t);
}

Matrix4x4 Matrix4x4::translate(const Vec3& r)
{
	double t[4][4] = {
		{ 1.f, 0.f ,0.f , r.x },
		{ 0.f, 1.f ,0.f , r.y },
		{ 0.f, 0.f ,1.f , r.z },
		{ 0.f, 0.f ,0.f , 1.f } };
	return Matrix4x4(t);
}

Matrix4x4 Matrix4x4::rotate(const Vec3& r)
{
	return rotateX(r.x) * rotateY(r.y) * rotateZ(r.z);
}

Matrix4x4 Matrix4x4::fromTransform(const Transform& t)
{
	return translate(t.location)*rotate(t.rotation);
}

Matrix4x4 Matrix4x4::rotateX(double angle)
{
	double cosine = cos(angle), sine = sin(angle);
	double t[4][4] = {
		{ 1.f,	0.f ,	0.f ,	0.f },
		{ 0.f,	cosine ,-sine ,	0.f },
		{ 0.f,	sine ,	cosine ,0.f },
		{ 0.f,	0.f ,	0.f ,	1.f } };
	return Matrix4x4(t);
}

Matrix4x4 Matrix4x4::rotateY(double angle)
{
	double cosine = cos(angle), sine = sin(angle);
	double t[4][4] = {
		{ cosine,	0.f ,	-sine , 0.f },
		{ 0.f,		1.f ,	0.f ,	0.f },
		{ sine,		0.f ,	cosine, 0.f },
		{ 0.f,		0.f ,	0.f ,	1.f } };
	return Matrix4x4(t);
}

Matrix4x4 Matrix4x4::rotateZ(double angle)
{
	double cosine = cos(angle), sine = sin(angle);
	double t[4][4] = {
		{ cosine,	-sine ,	0.f , 0.f },
		{ sine,		cosine, 0.f , 0.f },
		{ 0.f,		0.f ,	1.f , 0.f },
		{ 0.f,		0.f ,	0.f , 1.f } };
	return Matrix4x4(t);
}

Matrix4x4 Matrix4x4::rotateAxis(Vec3 axis, double angle)
{
	axis = normalize(axis);
	double x = axis.x, y = axis.y, z = axis.z;
	double cosine = cos(angle), sine = sin(angle), t = 1 - cosine;
	double r[4][4] = {
		{ t*x*x + cosine, t*x*y - sine*y ,t*x*z + sine*y , 0.f },
		{ t*x*y + sine*z, t*y*y + cosine ,t*y*z - sine*x , 0.f },
		{ t*x*z - sine*y, t*y*z + sine*x ,t*z*z + cosine , 0.f },
		{ 0.f,			  0.f ,		      0.f ,			   1.f } };
	return Matrix4x4(r);
}

Matrix4x4 Matrix4x4::scale(Vec3 r)
{
	double t[4][4] = {
		{ r.x, 0.f ,0.f , 0.f },
		{ 0.f, r.y ,0.f , 0.f },
		{ 0.f, 0.f ,r.z , 0.f },
		{ 0.f, 0.f ,0.f , 1.f } };
	return Matrix4x4(t);
}

Matrix4x4 Matrix4x4::view(const Vec3& eye, const Vec3& forward, const Vec3& up)
{
	Bias b = Bias::buildFromVW(up, forward);
	double rot[4][4] = {
		{ b.u.x, b.v.x ,b.w.x , 0.f },
		{ b.u.y, b.v.y ,b.w.y , 0.f },
		{ b.u.z, b.v.z ,b.w.z , 0.f },
		{ 0.f,	 0.f ,  0.f ,   1.f } };
	double tra[4][4] = {
		{ 1.f, 0.f ,0.f , eye.x },
		{ 0.f, 1.f ,0.f , eye.y },
		{ 0.f, 0.f ,1.f , eye.z },
		{ 0.f, 0.f ,0.f , 1.f } };
	return Matrix4x4(tra) * Matrix4x4(rot);
}

Bias buildFromOneBias(const Vec3& bias, int which)
{
	Vec3 n(1.f, 0.f, 0.f), m(0.f, 1.f, 0.f);
	Bias r;
	Vec3 *v[3] = { &r.u,&r.v,&r.w };
	Vec3 &a = *v[which], &b = *v[(which + 1) % 3], &c = *v[(which + 2) % 3];
	a = normalize(bias); b = cross(n, a);
	if (b.length() < 0.01f) b = cross(m, a);
	c = cross(a, b);
	return r;
}

Bias buildFromTwoBias(const Vec3 b1, const Vec3& b2, int w1, int w2)
{
	int w3 = (w1 + 1) % 3;
	if (w3 == w2) w3 = (w3 + 1) % 3;
	Bias r;
	Vec3 *v[3] = { &r.u,&r.v,&r.w };
	Vec3 &a = *v[w1], &b = *v[w2], &c = *v[w3];
	a = normalize(b1);
	c = normalize(cross(b1, b2));
	b = cross(a, c);
	return r;
}

Bias Bias::buildFromU(const Vec3& u)
{
	return buildFromOneBias(u, 0);
}

Bias Bias::buildFromV(const Vec3& v)
{
	return buildFromOneBias(v, 1);
}

Bias Bias::buildFromW(const Vec3& w)
{
	return buildFromOneBias(w, 2);
}

Bias Bias::buildFromUV(const Vec3& u, const Vec3& v)
{
	return buildFromTwoBias(u, v, 0, 1);
}

Bias Bias::buildFromUW(const Vec3& u, const Vec3& w)
{
	return buildFromTwoBias(u, w, 0, 2);
}

Bias Bias::buildFromVW(const Vec3& v, const Vec3& w)
{
	return buildFromTwoBias(v, w, 1, 2);
}

Bias Bias::buildFromWV(const Vec3& w, const Vec3& v)
{
	return buildFromTwoBias(w, v, 2, 1);
}

Bias Bias::buildFromWU(const Vec3& w, const Vec3& u)
{
	return buildFromTwoBias(w, u, 2, 0);
}

Bias Bias::buildFromVU(const Vec3& v, const Vec3& u)
{
	return buildFromTwoBias(v, u, 1, 0);
}

Bias Bias::buildFromRotator(const Vec3& r)
{
	Matrix4x4 m = Matrix4x4::rotate(r);
	Bias b;
	b.u = normalize(Vec3{ m.v[0][0], m.v[1][0], m.v[2][0] });
	b.v = normalize(Vec3{ m.v[0][1],  m.v[1][1], m.v[2][1] });
	b.w = normalize(Vec3{ m.v[0][2], m.v[1][2],  m.v[2][2] });
	return b;
}

double & Vec3::operator[](int i)
{
	switch (i)
	{
	case 0:return x;
	case 1:return y;
	case 2:return z;
	}
	return x;
}

Vec3& Vec3::operator*=(const Vec3& r)
{
	x *= r.x; y *= r.y; z *= r.z; return *this;
}

Vec3& Vec3::operator*=(double r)
{
	x *= r; y *= r; z *= r; return *this;
}

Vec3& Vec3::operator/=(double r)
{
	x /= r; y /= r; z /= r; return *this;
}

Vec3& Vec3::operator-=(const Vec3& r)
{
	x -= r.x; y -= r.y; z -= r.z; return *this;
}

Vec3& Vec3::operator+=(const Vec3& r)
{
	x += r.x; y += r.y; z += r.z; return *this;
}

Vec3& Vec3::operator=(const Vec3& r)
{
	x = r.x; y = r.y; z = r.z; return *this;
}

Vec3::Vec3(const Vec3& r) : Vec3(r.x, r.y, r.z)
{

}

Vec3::Vec3(double a, double b, double c) : x(a), y(b), z(c)
{

}

Vec3::Vec3(double f) : Vec3(f, f, f)
{

}

Vec3::Vec3() : Vec3(0, 0, 0)
{

}

double Vec3::length() const
{
	return sqrt(x*x + y*y + z*z);
}

Vec3 Vec3::operator-() const
{
	return Vec3(-x, -y, -z);
}
