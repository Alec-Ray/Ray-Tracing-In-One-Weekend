#ifndef MATERIALH
#define MATERIALH 

struct hit_record;

#include "ray.h"
#include "hitable.h"


//通过入射光线，计算反射光线
vec3 reflect(const vec3& v, const vec3& n) {
     return v - 2*dot(v,n)*n;
}

//生成随机方向的标准向量
//产生一个“起点在原点，长度小于1，方向随机”的向量，该向量和交点处单位法向量相加就得到交点处反射光线随机的方向向量
vec3 random_in_unit_sphere() {
    vec3 p;
    do {
        //p = 2.0*vec3(drand48(),drand48(),drand48()) - vec3(1,1,1);
		p = 2.0f * vec3((rand() % 100 / float(100)), (rand() % 100 / float(100)), (rand() % 100 / float(100))) - vec3(1.0f, 1.0f, 1.0f);
    } while (p.squared_length() >= 1.0);
    return p;
}

//材质抽象类
class material  {
    public:
        virtual bool scatter(const ray& r_in, const hit_record& rec, vec3& attenuation, ray& scattered) const = 0;
};

//漫反射材质
class lambertian : public material {
    public:
        lambertian(const vec3& a) : albedo(a) {}
		//将求反射光线的部分放到了材质类的scatter()方法里，每个材质可以自己定义其反射光线
		//获取漫反射的反射光线；获取材料的衰减系数;
        virtual bool scatter(const ray& r_in, const hit_record& rec, vec3& attenuation, ray& scattered) const  {
             vec3 target = rec.p + rec.normal + random_in_unit_sphere();
             scattered = ray(rec.p, target-rec.p);
             attenuation = albedo;
             return true;
        }

        vec3 albedo;
};

//镜面反射材质，金属材质用这个反射光线
class metal : public material {
    public:
		//f为零，说明没有模糊
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




