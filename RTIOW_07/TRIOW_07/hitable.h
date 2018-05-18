#ifndef hitableH
#define hitableH 

#include "ray.h"

class material;

//ײ������Ϣ
struct hit_record
{
	//���߲���t
    float t;  
	//ײ����λ������p
    vec3 p;
	//ײ����λ������N
    vec3 normal; 
    material *mat_ptr;
};

//�����ܱ�����ײ��������ĸ���
class hitable  {
    public:
		//hit()�麯��
        virtual bool hit(const ray& r, float t_min, float t_max, hit_record& rec) const = 0;
};

#endif




