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
	//�ж����������û�и��������2�������ǻ���
	float discrimiant = b*b - 4.0f*a*c;
	return (discrimiant > 0.0f);
}

vec3 Color(const ray& r)
{
	if (hit_sphere(vec3(0.0f, 0.0f, -1.0f), 0.5f, r))
		return vec3(1.0f, 0.0f, 0.0f);

	vec3 unit_direction = unit_vector(r.direction());
	//t��0��1
	float t = 0.5f*(unit_direction.y() + 1.0f);

	//���Ի�ϣ�t=1ʱ��ɫ��t=0ʱ��ɫ��t�����м�ʱ�ǻ����ɫ
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

	//��׶�����½�
	vec3 lower_left_corner(-2.0f, -1.0f, -1.0f);
	//�������½ǵ�ˮƽ����
	vec3 horizontal(4.0f, 0.0f, 0.0f);
	//�������½ǵĴ�ֱ����
	vec3 vertical(0.0f, 2.0f, 0.0f);
	//��ʼ��
	vec3 origin(0.0f, 0.0f, 0.0f);

	for (int j = ny - 1; j >= 0; j--)
	{
		for (int i = 0; i < nx; i++)
		{
			//u��0��ʼԽ��Խ�ӽ�1��v�����޽ӽ�1��ʼ��Խ��Խ�ӽ�0
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