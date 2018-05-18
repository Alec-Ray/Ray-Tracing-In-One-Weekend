#ifndef cameraH
#define cameraH

//M_PI ��һ���궨�壬Բ���ʵĶ��壬math�ļ���Ҳ����һ
#define M_PI 3.14159265358979323846

#include "ray.h"

class camera 
{
	vec3 origin;
	vec3 lower_left_corner;
	vec3 horizontal;
	vec3 vertical;

public:
	camera(vec3 lookfrom, vec3 lookat, vec3 vup, float vfov, float aspect)
	{
		vec3 u, v, w;
		//theta�Ž�
		float theta = vfov*M_PI / 180;
		//һ��ĸ߶ȵ���tan��theta/2��
		float half_height = tan(theta / 2);
		//aspect�����߱���
		float half_width = aspect * half_height;

		//lookform���λ��
		origin = lookfrom;
		//lookat�������ĵ�
		//w�õ�һ�������ǰ��������
		//��ֱ�� w �� view of up����дΪvup����������Ϊ��ƽ���ϡ������������
		//��ֱ�ں��������� w ��������Ϊ��������
		w = unit_vector(lookfrom - lookat);
		u = unit_vector(cross(vup, w));
		v = cross(w, u);

		lower_left_corner = origin - half_width*u - half_height*v - w;
		horizontal = 2 * half_width*u;
		vertical = 2 * half_height*v;
	}

	ray getray(float u, float v) 
	{
		return ray(origin, lower_left_corner + u*horizontal + v*vertical - origin);
	}

};
#endif




