#include "Shapes.h"
#define _USE_MATH_DEFINES
#include <math.h>
#include "BVH.h"

int Surface::intersect(Ray r, HitRecord &h) const
{
	Vec3 l = point - r.origin;
	double cosi = dot(normal, r.direction);
	if (cosi == 0) return 0;
	double t = dot(l, normal) / cosi;
	if (t < 0) return 0;
	h.normal = normal;
	h.t = t;
	Vec3 lhit = r.f(t) - point;
	Bias bias = Bias::buildFromW(normal);
	double u = dot(bias.u, lhit);
	double v = dot(bias.v, lhit);
	h.matAttr = mat->attribute(u, v);
	if (dot(normal, r.direction) > 0)
	{
		h.normal = -normal;
		return -1;
	}
	return 1;
}

BoundBox Surface::getBound() const
{
	return BoundBox{
		-INFINITY,INFINITY
	};
}

//http://www.cnblogs.com/graphics/archive/2010/08/09/1795348.html
int Triangle::intersect(Ray r, HitRecord &h) const
{
    Vec3 e1 = points[1] - points[0];
    Vec3 e2 = points[2] - points[0];
	if (dot(cross(e1, points[2] - points[1]), r.direction) > 0) return 0;
    Vec3 p = cross(r.direction,e2);
	double det = dot(e1, p);
	double u, v, t;
    Vec3 vt;
    if( det >0 )
        vt = r.origin - points[0];
    else
    {
        vt = points[0] - r.origin;
        det = -det;
    }
    if( det < 0.0001f )
        return 0;
	u = dot(vt, p);
    if( u < 0.0f || u > det )
        return 0;
	Vec3 q = cross(vt, e1);
	v = dot(r.direction, q);
    if( v < 0.0f || u + v > det )
        return 0;
	t = dot(e2, q);
    double fInvDet = 1.0f / det;
	t *= fInvDet;  u *= fInvDet; v *= fInvDet;
	h.normal = normalize(cross(e1, e2));
	h.t = t;
	double uu = us[0] * (1 - u - v) + us[1] * u + us[2] * v;
	double vv = vs[0] * (1 - u - v) + vs[1] * u + vs[2] * v;
	h.matAttr = mat->attribute(uu, vv);
	if (dot(h.normal, r.direction) > 0) 
	{
		h.normal = -h.normal;
		return -1;
	}
	return 1;
}
 

template <class T>
inline T min(T x, T y) { return x < y ? x : y; }

template <class T>
inline T max(T x, T y) { return x > y ? x : y; }


BoundBox Triangle::getBound() const
{
	double minx = min(points[0].x,points[1].x);
	minx = min(minx, points[2].x);
	double miny = min(points[0].y, points[1].y);
	miny = min(miny, points[2].y);
	double minz = min(points[0].z, points[1].z);
	minz = min(minz, points[2].z);
	double maxx = max(points[0].x, points[1].x);
	maxx = max(maxx, points[2].x);
	double maxy = max(points[0].y, points[1].y);
	maxy = max(maxy, points[2].y);
	double maxz = max(points[0].z, points[1].z);
	maxz = max(maxz, points[2].z);
	return BoundBox{
		Vec3{ minx,miny,minz },
		Vec3{ maxx,maxy,maxz }
	};
}

int Sphere::intersect(Ray r, HitRecord &h) const
{
	Vec3 center = trans.location;
	Vec3 l = center - r.origin;
	double tca = dot(r.direction, l);
	double d2 = dot(l, l) - tca*tca;
	double r2 = radius*radius;
	if (d2 > r2) return 0;
	double tha = sqrt(r2 - d2);
	double t = tca - tha;
	if (t < 0) t = tca + tha;
	if (t < 0) return 0;
	h.t = t;
	h.normal = normalize(r.f(t) - center);
	Vec3 ln = Matrix4x4::rotate(-trans.rotation).transform(h.normal);
	double pi = (double)M_PI, theta = acos(ln.z), phi = atan2(ln.y, ln.x);
	if (phi < 0.0f) phi += pi * 2;
	double u = phi / (pi * 2);
	double v = (pi - theta) / pi;
	h.matAttr = mat->attribute(u, v);
	if (dot(h.normal, r.direction) > 0)
	{
		h.normal = -h.normal;
		return -1;
	}
	else return 1;
}

BoundBox Sphere::getBound() const
{
	return BoundBox{
		Vec3{ trans.location - Vec3{ radius } },
		Vec3{ trans.location + Vec3{ radius } }
	};
}
