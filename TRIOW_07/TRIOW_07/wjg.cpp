#include <iostream>
#include <fstream>
#include <random>
#include "sphere.h"
#include "hitable_list.h"
#include "camera.h"

using namespace std;

//��÷�������
vec3 RandomInUnitSphere()
{
	vec3 p;
	do {
		p = 2.0f * vec3((rand()%100 / float(100)), (rand() % 100 / float(100)), (rand() % 100 / float(100))) - vec3(1.0f, 1.0f, 1.0f);
	} while (dot(p, p) >= 1.0f);

	return p;
}

vec3 Color(const ray& r, hitable *world)
{
	hit_record rec;
	if (world->hit(r, 0.0, FLT_MAX, rec))
	{
		vec3 target = rec.p + rec.normal + RandomInUnitSphere();
		//�ݹ飬ÿ������50%������
		return 0.5f*Color(ray(rec.p, target - rec.p), world);
	}
	else
	{
		vec3 unit_direction = unit_vector(r.direction());
		float t = 0.5f*(unit_direction.y() + 1.0f);
		//���Ի�ϣ�t=1ʱ��ɫ��t=0ʱ��ɫ��t�����м�ʱ�ǻ����ɫ
		//blended_value = (1-t)*start_value + t*end_value
		return (1.0f - t)*vec3(1.0f, 1.0f, 1.0f) + t*vec3(0.5f, 0.7f, 1.0f);
	}
}

int main()
{
	ofstream outfile;
	outfile.open("IMG01.ppm");

	int nx = 800;
	int ny = 400;
	//��������
	int ns = 100;
	outfile << "P3\n" << nx << " " << ny << "\n255\n";

	hitable *list[2];
	list[0] = new sphere(vec3(0.0f, 0.0f, -1.0f), 0.5f);
	list[1] = new sphere(vec3(0.0f, -100.5f, -1.0f), 100.0f);
	hitable *world = new hitable_list(list, 2);

	camera cam;

	//�������ÿ�����ص�����������������ĵ�Ϊ�����������Ϊ1�ķ�Χ
	default_random_engine reng;
	uniform_real_distribution<float> uni_dist(0.0f, 1.0f);

	for (int j = ny - 1; j >= 0; j--)
	{
		for (int i = 0; i < nx; i++)
		{
			vec3 col(0.0f, 0.0f, 0.0f);
			//ÿ���������ns��
			for (int s = 0; s < ns; s++)
			{
				float u = float(i + uni_dist(reng)) / float(nx);
				float v = float(j + uni_dist(reng)) / float(ny);
				ray r = cam.getray(u,v);
				//vec3 p = r.point_at_parameter(2.0);
				//��������(u,v)��(u+1,v+1)������ɫֵ�ۼ�
				col += Color(r, world);
			}
			//����������ɫ��ֵ
			col /= float(ns);
			//gamma����
			col = vec3(sqrt(col[0]), sqrt(col[1]), sqrt(col[2]));
			int ir = int(255.99*col[0]);
			int ig = int(255.99*col[1]);
			int ib = int(255.99*col[2]);
			outfile << ir << " " << ig << " " << ib << "\n";
		}
	}
	outfile.close();
	return 0;
}