#pragma once
#include <vector>
#include "Textures.h"

class Material
{
public:
	//attribute of one point on surface
	struct Attribute
	{
		Vec3 baseColor, emmisionColor, normal;
		double specular, matallic, translucent, refractivity;
	};
	//map attribute components to texture
	int baseColor = -1, emmisionColor = -1, normal = -1;//vector
	int specular[2] = { -1,0 }, metallic[2] = { -1,0 };//scaler
	double translucent = 0.0001, refractivity = 1;
	std::vector<Texture *> textures;
	Material() :textures() {}
	int addTexture(Texture *t) { textures.push_back(t); return (int)textures.size()-1; }
	Attribute attribute(double u, double v)
	{
		Vec3 tbaseColor, temmisionColor, tnormal;
		double tspecular, tmetalness;
		Texture *diffuse = baseColor < 0 ? defaultTexture : textures[baseColor];
		tbaseColor = diffuse->sample(u, v);
		temmisionColor = emmisionColor < 0 ? 0 : textures[emmisionColor]->sample(u, v);
		if (normal < 0) tnormal = Vec3{ 0,1,0 }; else tnormal = textures[normal]->sample(u, v);
		if (specular[0] < 0) tspecular = 0; else tspecular = textures[specular[0]]->sample(u, v)[specular[1]];
		if (metallic[0] < 0) tmetalness = 0; else tmetalness = textures[metallic[0]]->sample(u, v)[metallic[1]];
		/*double dielectricSpecular = 0.08f * tspecular;
		Vec3 tdiffuseColor = tbaseColor * (1 - tmetalness);
		Vec3 tspecularColor = dielectricSpecular * (1 - tmetalness) + tbaseColor * tmetalness;*/
		return Attribute{ tbaseColor, temmisionColor, tnormal, tspecular, tmetalness, translucent, refractivity };
	}
};