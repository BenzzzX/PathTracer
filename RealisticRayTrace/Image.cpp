#include "Image.h"
#include <fstream>

Image2d::Image2d(unsigned w, unsigned h, Vec3 backgroundColor) :width(w), height(h), pixels(h*w)
{
	for (Vec3& i : pixels) 
		i = backgroundColor;
}

void Image2d::setPixel(unsigned x, unsigned y, Vec3 color)
{
	pixels[(height-y-1)*width + x] = color;
}

Vec3 Image2d::getPixel(unsigned x, unsigned y)
{
	return pixels[y*width + x];
}

void Image2d::gamma(double g)
{
	for (Vec3& i : pixels)
		i = pow(i, 1 / g);
}

unsigned char map(double v)
{
	v = v > 1.f ? 1.f : v;
	v = v < 0.f ? 0.f : v;
	return (unsigned char)(v * 255);
}

double unmap(unsigned char v)
{
	return (double)v / 255;
}

void Image2d::savePPM(const char* url)
{
	std::ofstream ofs(url, std::ios::out | std::ios::binary);
	ofs << "P6\n" << width << " " << height << "\n" << "255\n";
	for (Vec3& i : pixels)
		ofs << map(i.x) << map(i.y) << map(i.z);
	ofs.close();
}

void Image2d::loadPPM(const char* url)
{
	std::ifstream ifs(url, std::ios::in);
	char dump; int bit;
	ifs.get(dump); ifs.get(dump);
	ifs >> width >> height >> bit;
	ifs.get(dump);
	pixels.reserve(height*width);
	unsigned char r, g, b;
	for (Vec3& i : pixels)
	{
		ifs >> r >> g >> b;
		i = Vec3(unmap(r), unmap(g), unmap(b));
	}
	ifs.close();
}

