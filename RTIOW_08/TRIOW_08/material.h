#ifndef MATERIALH
#define MATERIALH 

struct hit_record;

#include "ray.h"
#include "hitable.h"


//ͨ��������ߣ����㷴�����
vec3 reflect(const vec3& v, const vec3& n) {
     return v - 2*dot(v,n)*n;
}

//�����������ı�׼����
//����һ���������ԭ�㣬����С��1��������������������������ͽ��㴦��λ��������Ӿ͵õ����㴦�����������ķ�������
vec3 random_in_unit_sphere() {
    vec3 p;
    do {
        //p = 2.0*vec3(drand48(),drand48(),drand48()) - vec3(1,1,1);
		p = 2.0f * vec3((rand() % 100 / float(100)), (rand() % 100 / float(100)), (rand() % 100 / float(100))) - vec3(1.0f, 1.0f, 1.0f);
    } while (p.squared_length() >= 1.0);
    return p;
}

//���ʳ�����
class material  {
    public:
        virtual bool scatter(const ray& r_in, const hit_record& rec, vec3& attenuation, ray& scattered) const = 0;
};

//���������
class lambertian : public material {
    public:
        lambertian(const vec3& a) : albedo(a) {}
		//��������ߵĲ��ַŵ��˲������scatter()�����ÿ�����ʿ����Լ������䷴�����
		//��ȡ������ķ�����ߣ���ȡ���ϵ�˥��ϵ��;
        virtual bool scatter(const ray& r_in, const hit_record& rec, vec3& attenuation, ray& scattered) const  {
             vec3 target = rec.p + rec.normal + random_in_unit_sphere();
             scattered = ray(rec.p, target-rec.p);
             attenuation = albedo;
             return true;
        }

        vec3 albedo;
};

//���淴����ʣ���������������������
class metal : public material {
    public:
		//fΪ�㣬˵��û��ģ��
        metal(const vec3& a, float f) : albedo(a) { if (f < 1) fuzz = f; else fuzz = 1; }
		//metal (const vec3& a):albedo(a){}
        virtual bool scatter(const ray& r_in, const hit_record& rec, vec3& attenuation, ray& scattered) const  {
            vec3 reflected = reflect(unit_vector(r_in.direction()), rec.normal);
            scattered = ray(rec.p, reflected + fuzz*random_in_unit_sphere());
            attenuation = albedo;
            return (dot(scattered.direction(), rec.normal) > 0);
        }
        vec3 albedo;
        float fuzz;
};

#endif




