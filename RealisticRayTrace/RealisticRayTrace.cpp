// RealisticRayTrace.cpp : 定义控制台应用程序的入口点。
//

#include "stdafx.h"
#include <ctime>
#include <cstdio>
#include "Tests.h"
#include "Renderers.h"
#include "Image.h"




int main()
{
	std::cout << "渲染开始...\n";
	time_t start, finished;
	PathTracer pathtracer{ 200,10 };
	start = clock();
	std::auto_ptr<Image2d> image(testDrawMat(&pathtracer));
	finished = clock();
	double s = (double)(finished - start) / CLOCKS_PER_SEC;
	std::cout<< "渲染完成. 共计: " << s << "s \n";
	const char* url = "asd.ppm";
	std::cout << "保存到："<<url<<"...\n";
	start = clock();
	image->savePPM(url);
	finished = clock();
	s = (double)(finished - start) / CLOCKS_PER_SEC;
	std::cout << "保存完成. 共计: " << s << "s \n";
    return 0;
}

