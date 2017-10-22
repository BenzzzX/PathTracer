#include "Textures.h"

TestTex TestTexINST;
Texture *defaultTexture = &TestTexINST;

Vec3 testGetPixel(int x, int y)
{
	if (x % 2 != y % 2) return Vec3(1, 1, 1);
	else return Vec3(0.5, 0.5, 0.75);
}

Vec3 testColor(double u, double v)
{
	u *= 6; v *= 6;
	int iu = (int)u, iv = (int)v;
	return testGetPixel(iu, iv);
	double du = u - iu, dv = v - iv;
	Vec3 a = testGetPixel(iu, iv),
		b = testGetPixel(iu + 1, iv);
	return a*((1 - du)*(1 - dv) + du*dv) + b*((1 - du)*dv + du*(1 - dv));
}

Vec3 TestTex::sample(double u, double v)
{
	double nu = u - (int)u, nv = v - (int)v;
	return testColor(nu, nv);
}

Vec3 SimpleColor::sample(double u, double v)
{
	return vcolor;
}

Vec3 sampleTexture(Image2d* image, double u, double v)
{
	u *= (image->width - 2); v *= (image->height - 2);
	int iu = (int)u, iv = (int)v;
	double tu = u - iu, tv = v - iv;
	Vec3 a = image->getPixel(iu, iv),
		b = image->getPixel(iu+1, iv),
		c = image->getPixel(iu, iv+1),
		d = image->getPixel(iu+1, iv+1);
	return a*(1 - tu)*(1 - tv) + b*tu*(1 - tv) + c*(1 - tu)*tv + d*tu*tv;
}

Vec3 ImageSample::sample(double u, double v)
{
	double nu = u - (int)u, nv = v - (int)v;
	return sampleTexture(image, nu, nv);
}
