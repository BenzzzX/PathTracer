#pragma once
#include "Structs.h"

class Camera
{
public:
	unsigned width, height;
	virtual Ray getRay(double x, double y) const = 0;
};

class OrthCamera : public Camera
{
	Vec3 origin, across, up, gaze;
public:
	OrthCamera(const Transform& t, unsigned w, unsigned h)
	{
		width = w; height = h;
		origin = t.location;
		Bias bias = Bias::buildFromRotator(t.rotation);
		across = bias.u;
		up = bias.v;
		gaze = bias.w;
	}
	Ray getRay(double x, double y) const override;
};

class PerspCamera : public Camera
{
	Vec3 origin, across, up, gaze;
	double angle, aspectratio;
public:
	PerspCamera(const Transform& t, double fov, unsigned w, unsigned h)
	{
		width = w; height = h;
		origin = t.location;
		Bias bias = Bias::buildFromRotator(t.rotation);
		across = bias.u;
		up = bias.v;
		gaze = bias.w;
		aspectratio = w / (double)h;
		angle = tan(M_PI * fov / 180.f);
	}
	Ray getRay(double x, double y) const override;
};