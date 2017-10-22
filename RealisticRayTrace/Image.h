#pragma once
#include "Structs.h"
#include <vector>

class string;
class Image2d
{
	std::vector<Vec3> pixels;
public:
	unsigned width, height;
	Image2d(unsigned w, unsigned h) :Image2d(w, h, Vec3(0, 0, 0)) {}
	Image2d(unsigned w, unsigned h, Vec3 backgroundColor);
	void setPixel(unsigned x, unsigned y, Vec3 color);
	Vec3 getPixel(unsigned x, unsigned y);
	void gamma(double g);
	void savePPM(const char* url);
	void loadPPM(const char* url);
};