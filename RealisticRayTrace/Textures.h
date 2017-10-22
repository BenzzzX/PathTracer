#pragma once
#include "Structs.h"
#include "Image.h"

class Texture
{
public:
	virtual Vec3 sample(double u, double v) = 0;
};

class SimpleColor : public Texture
{
	Vec3 vcolor;
public:
	SimpleColor(Vec3 c) :vcolor(c) {}
	Vec3 sample(double u, double v) override;
};

class TestTex : public Texture
{
public:
	Vec3 sample(double u, double v) override;
};

class ImageSample : public Texture
{
	Image2d* image;
public:
	ImageSample(Image2d* i) : image(i) {}
	Vec3 sample(double u, double v) override;
};


extern Texture *defaultTexture;