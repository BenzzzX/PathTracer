#pragma once

class Renderer
{
public:
	virtual class Image2d *render(const struct Scene *scene, const class Camera *camera) const = 0;
};

class PathTracer : public Renderer
{
	int sampleCount, traceDepth;
public:
	PathTracer(int sc = 4, int td = 5) :sampleCount(sc), traceDepth(td) {}
	class Image2d *render(const struct Scene* scene, const class Camera *camera) const override;
};