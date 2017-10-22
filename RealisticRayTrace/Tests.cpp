#include "Scene.h"
#include "Cameras.h"
#include "Renderers.h"
SimpleColor white{ Vec3{ 0.99,0.99,0.99 } }, gray{ Vec3{ 0.75,0.75,0.75 } }, black{ Vec3{} }, blue{ Vec3{ 0.5,0.5,0.75 } }, red{ Vec3{ 0.75,0.5,0.5 } }, green{ Vec3{0.5,0.75,0.5} };

Image2d * testDrawRoom(Renderer *renderer)
{
	Scene scene;
	PerspCamera camera{ Transform{ Vec3(50,42.5,295.6),Vec3(0.01,M_PI,0) }, 27.4, 512, 512 };
	Sphere rwall(Transform(Vec3(1e5 + 1, 40.8, 81.6), Vec3()), 1e5);
	Sphere lwall(Transform(Vec3(-1e5 + 99, 40.8, 81.6), Vec3()), 1e5);
	Sphere bwall(Transform(Vec3(50, 40.8, -1e5), Vec3()), 1e5);
	Sphere land(Transform(Vec3(50, -1e5, 81.6), Vec3()), 1e5);
	Sphere ceil(Transform(Vec3(50, 1e5 + 81.6, 81.6), Vec3()), 1e5);
	Sphere metallicSphere(Transform(Vec3(73, 16.5, 47), Vec3()), 16.5);
	Sphere glassSphere(Transform(Vec3(27, 16.5, 78), Vec3()), 16.5);
	Sphere light(Transform(Vec3(50, 81.6 - 6, 81.6), Vec3()), 10);
	Material lightMat, metallicMat, glassMat,lwm, rwm, whiteDiffMat;
	SimpleColor lightc{ Vec3{ 1,1,1 } * (10 * 2) };
	whiteDiffMat.baseColor = whiteDiffMat.addTexture(&gray);
	lightMat.emmisionColor = lightMat.addTexture(&lightc);
	lightMat.baseColor = lightMat.addTexture(&black);
	light.mat = &lightMat;
	ceil.mat = bwall.mat = land.mat = &whiteDiffMat;
	lwm.baseColor = lwm.addTexture(&red);
	rwm.baseColor = rwm.addTexture(&blue);
	lwall.mat = &lwm; rwall.mat = &rwm;
	metallicMat.specular[0] = metallicMat.baseColor = metallicMat.addTexture(&white);
	metallicSphere.mat = &metallicMat;
	glassMat.translucent = 0.5;
	glassMat.specular[0] = glassMat.baseColor = glassMat.addTexture(&white);
	glassSphere.mat = &glassMat;
	scene.add(&land);scene.add(&lwall);
	scene.add(&ceil);scene.add(&bwall);
	scene.add(&rwall);scene.add(&metallicSphere);
	scene.add(&glassSphere);scene.add(&light);
	return renderer->render(&scene, &camera);
}


Sphere *getSphere(Vec3 color, double x, double y, double specular, double metallic, double translucent,double refractivity = 1)
{
	Sphere *sphere = new Sphere{
			Transform{{x,20,y},0},
			20
	};
	SimpleColor *specularT = new SimpleColor{ Vec3{specular} },
		*metallicT = new SimpleColor{ Vec3{metallic} },
		*colorT = new SimpleColor{ color };
	Material *mat = new Material;
	mat->baseColor = mat->addTexture(colorT);
	mat->specular[0] = mat->addTexture(specularT);
	mat->metallic[0] = mat->addTexture(metallicT);
	mat->translucent = translucent;
	mat->refractivity = refractivity;
	sphere->mat = mat;
	return sphere;
}

void addShapes(Scene& scene)
{
	/*
	scene.add(getSphere({ 0.99,0.99,0.99 }, 0, -140, 0, 1, 0.001));
	scene.add(getSphere({ 0.99,0.99,0.99 }, 70, 70, 1, 1, 0.001));
	scene.add(getSphere({ 0.99,0.99,0.99 }, -70, 70, 1, 0, 0.001));
	scene.add(getSphere({ 0.99,0.99,0.99 }, 70, -70, 1, 0, 0.999, 1.5));
	scene.add(getSphere({ 0.99,0.99,0.99 }, -70, -70, 0, 0, 0.999, 1.5));
	*/
	scene.add(getSphere({ 0.5,0.75,0.5 }, -40, 20, 1, 0, 0.01, 1.5));
	scene.add(getSphere({ 0.5,0.75,0.5 }, 0, 60, 1, 0, 0.97, 1.5));
	scene.add(getSphere({ 0.5,0.75,0.5 }, 40, 20, 0, 0, 0.98, 1.5));
	scene.add(getSphere({ 0.5,0.75,0.5 }, 0, -20, 1, 0, 0.99, 1.5));
}

Image2d *testDrawMat(Renderer *renderer)
{
	Scene scene;
	double distance = 350;
	double angle = M_PI / 6;
	PerspCamera camera{ Transform{ Vec3(0,distance*tan(angle),-distance),Vec3{angle,0,0} }, 30, 512, 512 };
	Surface top{
		Vec3{ 0,220,0 },
		Vec3{ 0,-1,0 }
	};
	Surface base{
		Vec3{0,0,0},
		Vec3{0,1,0}
	};
	Surface back{
		Vec3{0,0,160},
		Vec3{0,0,-1}
	};
	Surface light{
		Vec3{ -110,0,0 },
		Vec3{ 1,0,0 }
	};
	Surface right{
		Vec3{ 110,0,0 },
		Vec3{ -1,0,0 }
	};
	SimpleColor lightColor{ Vec3{1} };
	Material lightMaterial;
	lightMaterial.emmisionColor = lightMaterial.addTexture(&lightColor);
	lightMaterial.baseColor = lightMaterial.addTexture(&black);
	light.mat = &lightMaterial;
	addShapes(scene);
	Material baseMaterial;
	baseMaterial.baseColor = baseMaterial.addTexture(&gray);
	base.mat = &baseMaterial;
	Material backMaterial;
	backMaterial.baseColor = backMaterial.addTexture(&gray);
	back.mat= &backMaterial;
	Material leftMaterial;
	leftMaterial.baseColor = leftMaterial.addTexture(&blue);
	top.mat = &leftMaterial;
	Material rightMaterial;
	rightMaterial.baseColor = rightMaterial.addTexture(&red);
	right.mat = &rightMaterial;
	scene.add(&light);
	scene.add(&base);/* scene.add(&back);
	scene.add(&left); scene.add(&right);*/
	return renderer->render(&scene, &camera);
}
