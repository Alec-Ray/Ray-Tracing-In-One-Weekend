#include <iostream>
#include <fstream>
#include <random>
#include "sphere.h"
#include "hitable_list.h"
#include "camera.h"
#include "material.h"

using namespace std;

//获得反射射线
vec3 RandomInUnitsphere()
{
	vec3 p;
	do {
		p = 2.0f * vec3((rand()%100 / float(100)), (rand() % 100 / float(100)), (rand() % 100 / float(100))) - vec3(1.0f, 1.0f, 1.0f);
	} while (dot(p, p) >= 1.0f);

	return p;
}

vec3 Color(const ray& r, hitable *world, int depth)
{
	//这个“rec”会在sphere::hit ()中带上来被撞击球的材料属性（指向一个材质对象的指针mat_ptr）。
	//根据这个指针可以获取材料对象的成员方法scatter()和成员变量albedo（反射衰减向量）
	hit_record rec;
	if (world->hit(r, 0.001f, FLT_MAX, rec))
	{
		ray scattered;
		vec3 attenuation;
		if (depth < 50 && rec.mat_ptr->scatter(r, rec, attenuation, scattered)) {
			//获取反射光线向量scattered和反射衰减向量attenuation
			return attenuation*Color(scattered, world, depth + 1);
			//反射光线的强度需要乘以反射衰减向量（对应坐标相乘作为新的向量）。
			//然后反射光线就扮演之前“原始光线”的角色。如果再次撞击到小球，就再次反射，直到不再撞击到任何球为止
		}
		else {
			return vec3(0.0f, 0.0f, 0.0f);
		}
	}
	else
	{
		//绘制背景
		vec3 unit_direction = unit_vector(r.direction());
		float t = 0.5f*(unit_direction.y() + 1.0f);

		//线性混合，t=1时蓝色，t=0时白色，t介于中间时是混合颜色
		//blended_value = (1-t)*start_value + t*end_value
		return (1.0f - t)*vec3(1.0f, 1.0f, 1.0f) + t*vec3(0.5f, 0.7f, 1.0f);

		//注意这里，原始光线和反射光线最后都会跑到这里来。
		//背景的颜色：原始光线的方向向量的映射
		//漫反射材料和镜面材料的颜色：最后一次反射光线的方向向量的映射 *  所有反射衰减系数的乘积。
		//漫反射和镜面反射的区别在于，漫反射的每次反射方向是随机的
	}
}

hitable *RandomScene() {
	int n = 500;
	hitable **list = new hitable *[n + 1];
	/*定义一个包含n+1个元素的数组，数组的每个元素是指向hitable对象的指针。然后将数组的指针赋值给list。所以，list是指针的指针。*/
	list[0] = new sphere(vec3(0, -1000, 0), 1000, new lambertian(vec3(0.5, 0.5, 0.5)));
	/*先创建一个中心在（0，-1000，0）半径为1000的超大漫射球，将其指针保存在list的第一个元素中。*/
	int i = 1;
	for (int a = -11; a < 11; a++) {
		for (int b = -11; b < 11; b++) {
			/*两个for循环中会产生（11+11）*(11+11)=484个随机小球*/
			float choose_mat = (rand() % (100) / (float)(100));
			/*产生一个（0，1）的随机数，作为设置小球材料的阀值*/
			vec3 center(a + 0.9*(rand() % (100) / (float)(100)), 0.2,b + 0.9*(rand() % (100) / (float)(100)));
			/*” a+0.9*(rand()%(100)/(float)(100))”配合[-11,11]产生（-11，11）之间的随机数，而不是[-11,11)之间的22个整数。使得球心的x,z坐标是（-11，11）之间的随机数*/
			if ((center - vec3(4, 0.2, 0)).length() > 0.9) {
				/*避免小球的位置和最前面的大球的位置太靠近*/
				if (choose_mat < 0.8) {     
					//diffuse  
					/*材料阀值小于0.8，则设置为漫反射球，漫反射球的衰减系数x,y,z都是（0，1）之间的随机数的平方*/
					list[i++] = new sphere(center, 0.2,
						new lambertian(vec3(
						(rand() % (100) / (float)(100))*(rand() % (100) / (float)(100)),
							(rand() % (100) / (float)(100))*(rand() % (100) / (float)(100)),
							(rand() % (100) / (float)(100))*(rand() % (100) / (float)(100)))));
				}
				else if (choose_mat < 0.95) {
					/*材料阀值大于等于0.8小于0.95，则设置为镜面反射球，镜面反射球的衰减系数x,y,z及模糊系数都是（0，1）之间的随机数加一再除以2*/
					list[i++] = new sphere(center, 0.2,
						new metal(vec3(0.5*(1 + (rand() % (100) / (float)(100))),
							0.5*(1 + (rand() % (100) / (float)(100))),
							0.5*(1 + (rand() % (100) / (float)(100)))),
							0.5*(1 + (rand() % (100) / (float)(100)))));
				}
				else {
					/*材料阀值大于等于0.95，则设置为介质球*/
					list[i++] = new sphere(center, 0.2, new dielectric(1.5));
				}
			}
		}
	}

	list[i++] = new sphere(vec3(0, 1, 0), 1.0, new dielectric(1.5));
	list[i++] = new sphere(vec3(-4, 1, 0), 1.0, new lambertian(vec3(0.4, 0.2, 0.1)));
	list[i++] = new sphere(vec3(4, 1, 0), 1.0, new metal(vec3(0.7, 0.6, 0.5), 0.0));
	/*定义三个大球*/
	return new hitable_list(list, i);
}


//And add some metal spheres
int main()
{
	ofstream outfile;
	outfile.open("IMG.ppm");

	int nx = 2000;
	int ny = 1000;
	//采样次数
	int ns = 100;
	outfile << "P3\n" << nx << " " << ny << "\n255\n";

	hitable *world = RandomScene();

	vec3 lookform(13.0f, 2.0f, 3.0f);
	vec3 lookat(0, 0, 0);
	float dist_to_focus = (lookform - lookat).length();
	float aperture = 0.0f;

	camera cam(lookform, lookat, vec3(0, 1, 0), 20, float(nx) / float(ny), aperture, 0.7*dist_to_focus);

	//随机数
	default_random_engine reng;
	uniform_real_distribution<float> uni_dist(0.0f, 1.0f);

	for (int j = ny - 1; j >= 0; j--)
	{
		for (int i = 0; i < nx; i++)
		{
			vec3 col(0.0f, 0.0f, 0.0f);
			//每个区域采样ns次
			for (int s = 0; s < ns; s++)
			{
				float u = float(i + uni_dist(reng)) / float(nx);
				float v = float(j + uni_dist(reng)) / float(ny);
				ray r = cam.getray(u,v);
				//vec3 p = r.point_at_parameter(2.0);
				//将本区域（(u,v)到(u+1,v+1)）的颜色值累加
				col += Color(r, world, 0);
			}
			//获得区域的颜色均值
			col /= float(ns);
			//gamma矫正
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