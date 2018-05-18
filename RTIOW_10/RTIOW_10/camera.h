#ifndef cameraH
#define cameraH

//M_PI 是一个宏定义，圆周率的定义，math文件中也有这家伙。
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
		//theta张角
		float theta = vfov*M_PI / 180;
		//一半的高度等于tan（theta/2）
		float half_height = tan(theta / 2);
		//aspect画面宽高比例
		float half_width = aspect * half_height;

		//lookform相机位置
		origin = lookfrom;
		//lookat相机看相的点
		//w得到一个相机正前方的向量
		//垂直于 w 与 view of up（简写为vup）的向量即为“平面上”横向的向量，
		//垂直于横向向量与 w 的向量即为竖向向量
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




