#ifndef hitableH
#define hitableH 

#include "ray.h"

class material;

//撞击点信息
struct hit_record
{
	//射线参数t
    float t;  
	//撞击点位置向量p
    vec3 p;
	//撞击点位置向量N
    vec3 normal; 
    material *mat_ptr;
};

//所有能被射线撞击的物体的父类
class hitable  {
    public:
		//hit()虚函数
        virtual bool hit(const ray& r, float t_min, float t_max, hit_record& rec) const = 0;
};

#endif




