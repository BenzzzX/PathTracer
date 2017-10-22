#include "Cameras.h"
#include <iostream>

Ray OrthCamera::getRay(double x, double y) const
{
	
	Vec3 ro = origin + x*width*across + y*height*up;
	return Ray( ro,gaze );
}

Ray PerspCamera::getRay(double x, double y) const
{
	double xx = x*angle*aspectratio;
	double yy = y*angle;
	Vec3 ro =xx*across + yy*up + gaze;
	return Ray(origin, normalize(ro));
}
