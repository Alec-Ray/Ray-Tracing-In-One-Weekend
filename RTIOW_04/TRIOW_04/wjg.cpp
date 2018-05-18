#include <iostream>
#include <fstream>
#include "ray.h"
using namespace std;

bool hit_sphere(const vec3& center, float radius, const ray& r)
{
	vec3 oc = r.origin() - center;
	float a = dot(r.direction(), r.direction());
	float b = 2.0f*dot(oc, r.direction());
	float c = dot(oc, oc) - radius*radius;
	//判断这个方程有没有根，如果有2个根就是击中
	float discrimiant = b*b - 4.0f*a*c;
	return (discrimiant > 0.0f);
}

vec3 Color(const ray& r)
{
	if (hit_sphere(vec3(0.0f, 0.0f, -1.0f), 0.5f, r))
		return vec3(1.0f, 0.0f, 0.0f);

	vec3 unit_direction = unit_vector(r.direction());
	//t从0到1
	float t = 0.5f*(unit_direction.y() + 1.0f);

	//线性混合，t=1时蓝色，t=0时白色，t介于中间时是混合颜色
	//blended_value = (1-t)*start_value + t*end_value
	return (1.0f - t)*vec3(1.0f, 1.0f, 1.0f) + t*vec3(0.5f, 0.7f, 1.0f);
}

int main()
{
	ofstream outfile;
	outfile.open("IMG.ppm");

	int nx = 800;
	int ny = 400;
	outfile << "P3\n" << nx << " " << ny << "\n255\n";

	//视锥体左下角
	vec3 lower_left_corner(-2.0f, -1.0f, -1.0f);
	//距离左下角的水平距离
	vec3 horizontal(4.0f, 0.0f, 0.0f);
	//距离左下角的垂直距离
	vec3 vertical(0.0f, 2.0f, 0.0f);
	//起始点
	vec3 origin(0.0f, 0.0f, 0.0f);

	for (int j = ny - 1; j >= 0; j--)
	{
		for (int i = 0; i < nx; i++)
		{
			//u从0开始越来越接近1；v从无限接近1开始，越来越接近0
			float u = float(i) / float(nx);
			float v = float(j) / float(ny);

			ray r(origin, lower_left_corner + u*horizontal + v*vertical);
			vec3 col = Color(r);

			int ir = int(255.99*col[0]);
			int ig = int(255.99*col[1]);
			int ib = int(255.99*col[2]);
			outfile << ir << " " << ig << " " << ib << "\n";
		}
	}
	outfile.close();
	return 0;
}