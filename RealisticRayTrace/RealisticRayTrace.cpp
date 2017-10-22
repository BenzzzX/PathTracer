// RealisticRayTrace.cpp : �������̨Ӧ�ó������ڵ㡣
//

#include "stdafx.h"
#include <ctime>
#include <cstdio>
#include "Tests.h"
#include "Renderers.h"
#include "Image.h"




int main()
{
	std::cout << "��Ⱦ��ʼ...\n";
	time_t start, finished;
	PathTracer pathtracer{ 200,10 };
	start = clock();
	std::auto_ptr<Image2d> image(testDrawMat(&pathtracer));
	finished = clock();
	double s = (double)(finished - start) / CLOCKS_PER_SEC;
	std::cout<< "��Ⱦ���. ����: " << s << "s \n";
	const char* url = "asd.ppm";
	std::cout << "���浽��"<<url<<"...\n";
	start = clock();
	image->savePPM(url);
	finished = clock();
	s = (double)(finished - start) / CLOCKS_PER_SEC;
	std::cout << "�������. ����: " << s << "s \n";
    return 0;
}

