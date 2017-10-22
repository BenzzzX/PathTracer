#include "Structs.h"
#include "Shapes.h"
#include "Cameras.h"
#include "Image.h"
#include "RandomGenerator.h"
#include "Scene.h"
#include "Renderers.h"
#include <iostream>
#include <time.h>
RandomGenerator rng;

double clamp(double v, double min, double max)
{
	if (v < min) return min;
	if (v > max) return max;
	return v;
}

template<class T>
T lerp(T v1, T v2, double alpha)
{
	return v1*(1 - alpha) + v2*alpha;
}

Vec3 surfaceScatter(const Vec3& direct, double specular)
{
	Bias bias = Bias::buildFromW(direct);
	double phi = rng.rand() * 2 * M_PI;
	double sphi = sin(phi);
	double cphi = cos(phi);
	double sint = sin(rng.rand()) * (1 - specular);
	double cost = sqrt(1 - sint*sint);
	Vec3 x = bias.u * sint * cphi;
	Vec3 y = bias.v * sint * sphi;
	Vec3 z = bias.w * cost;
	return normalize(x + y + z);
}

Vec3 subsurfaceScatter()
{
	double x = rng.rand();
	double y = rng.rand();
	double z = rng.rand();
	return normalize(Vec3{ 1 - x * 2,1 - y * 2,1 - z * 2 });
}

Ray castRay(Vec3 p, Vec3 d)
{
	return { p + d*0.00001,d };
}


Vec3 radiance(const Scene* scene, Ray ray, int traceDepth, HitRecord rec, int state)
{
	int depth = 0; Vec3 refractionStart;
	Vec3 light{ 1,1,1 }, acc{ 0,0,0 };
	double mediaTranslucent[100]; int inMedia = 0;
	mediaTranslucent[inMedia] = scene->airTranslucent;
	while (1)
	{
		double tmax = log(rng.rand()) / log(mediaTranslucent[inMedia]);
		if (state != 0 && rec.t < tmax)
		{
			Material::Attribute attr = rec.matAttr;
			acc += light * attr.emmisionColor;
			Vec3 phit = ray.f(rec.t); 
			double specular = attr.specular;
			rec.normal = surfaceScatter(rec.normal, specular);
			double cosp = dot(-rec.normal, ray.direction);
			double a = 1 - attr.refractivity, b = 1 + attr.refractivity;
			double metallic = attr.matallic;
			double _R0 = metallic*0.8 + 0.2;
			double R0 = _R0 + (1 - _R0) * (a*a / (b*b));
			double fresnel = R0 + (1 - R0) * pow(1 - cosp, 5);
			double p = lerp(fresnel * metallic, fresnel, specular);
			Vec3 reflectDirection = ray.direction - 2 * rec.normal*dot(ray.direction, rec.normal);
			if (rng.rand() < p) //reflection
			{
			REFLECTION:
				Vec3 dir = reflectDirection;
				ray = castRay(phit, dir);
				Vec3 reflectionColor = attr.baseColor / 2 + Vec3(0.5);
				light *= reflectionColor;
				goto END;
			}
			else //refraction or scatter
			{
				double translucent = attr.translucent;
				double refrac = attr.refractivity;
				if (state == 1) refrac = 1 / refrac;
				double cosp22 = 1 - refrac*refrac*(1 - cosp*cosp);
				if (cosp22 > 0)
				{
					Vec3 refractDirection = ray.direction*refrac + (cosp*refrac - sqrt(cosp22)) * rec.normal;
					if (state == -1)
					{
						inMedia--;
					}
					else mediaTranslucent[++inMedia] = attr.translucent;
					refractionStart = phit;
					light *= attr.baseColor;
					ray = castRay(phit, refractDirection);
					goto END;
				}
				else
				{
					goto REFLECTION;
				}			
			}
		}
		else
		{
			ray = { ray.f(tmax),subsurfaceScatter() };
			light *= 0.97;
			goto END;
		}
		END:
		double md = 0.6;
		if ((int)depth > traceDepth || !md) if (rng.rand() < md) light /= md; else return acc;
		state = scene->trace(ray, rec);
		depth++;
	}
}

Image2d * PathTracer::render(const Scene* scene, const Camera *camera) const
{
	Image2d* image = new Image2d(camera->width, camera->height);
	Vec3 acc;
	static const int nthread = 8;
#pragma omp parallel for schedule(dynamic, 1) private(acc) num_threads(nthread)
	for (int y = 0; y < (int)camera->height; y++)
	{
		fprintf(stderr, "\r基于路径追踪 分辨率(%u x %u) 采样x%d 并行线程x%d: 已完成%5.2f%%", camera->width, camera->height, sampleCount * 4, nthread, 100.*y / (camera->height - 1));
		for (int x = 0; x < (int)camera->width; x++, acc = 0)
		{
			for (int s4 = 0; s4 < 4; s4++)
			{
				int sx = s4 % 2, sy = s4 / 2;
				double r1 = 2 * rng.rand(), dx = r1 < 1 ? sqrt(r1) - 1 : 1 - sqrt(2 - r1);
				double r2 = 2 * rng.rand(), dy = r2 < 1 ? sqrt(r2) - 1 : 1 - sqrt(2 - r2);
				double xx = ((sx + .5 + dx) / 2 + x) / (double)camera->width - .5;
				double yy = ((sy + .5 + dy) / 2 + y) / (double)camera->height - .5;
				Ray ray = camera->getRay(xx, yy); 
				HitRecord rec;
				int state = scene->trace(ray, rec);
				if (state == 0) continue;
				for (int s = 0; s < sampleCount; s++)
				{
					acc += radiance(scene, ray, traceDepth, rec, state);
				}
			}
			image->setPixel(x, y, acc / (double)(sampleCount * 4));
		}
	}
	std::cout << "\n";
	return image;
}

