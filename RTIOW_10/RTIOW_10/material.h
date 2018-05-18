#ifndef MATERIALH
#define MATERIALH 

struct hit_record;

#include "ray.h"
#include "hitable.h"

//菲涅尔反射，这里用的是Schlick近似
float schlick(float cosine, float ref_idx) {
	float r0 = (1-ref_idx) / (1+ref_idx);
	r0 = r0*r0;
	return r0 + (1-r0)*pow((1 - cosine),5);
}

//计算折射光线的方向向量。ni_over_nt 为入射介质的折射指数和折射介质的折射指数的比值。
bool refract(const vec3& v, const vec3& n, float ni_over_nt, vec3& refracted) {
	vec3 uv = unit_vector(v);
	float dt = dot(uv, n);
	float discriminant = 1.0 - ni_over_nt*ni_over_nt*(1-dt*dt);
	if (discriminant > 0) {
		refracted = ni_over_nt*(uv - n*dt) - n*sqrt(discriminant);
		return true;
	}
	else 
		//根号里面的内容小于零，说明折射光线的方向向量无实根，即没有折射光线，即出现全反射。
		//所以，折射光线函数return false
		return false;
}

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

//透明折射模型
class dielectric : public material { 
public:
	//相对于空气的折射率
	float ref_idx;
	dielectric(float ri) : ref_idx(ri) {}
	virtual bool scatter(const ray& r_in, const hit_record& rec, vec3& attenuation, ray& scattered) const  {
		vec3 outward_normal;
		vec3 reflected = reflect(r_in.direction(), rec.normal);

		//ni_over_nt为入射介质的折射指数和折射介质的折射指数的比值
		float ni_over_nt;

		//介质的衰减向量为（1，1，1）不是光线不衰减
		attenuation = vec3(1.0, 1.0, 1.0); 
		vec3 refracted;
		float reflect_prob;
		float cosine;

		if (dot(r_in.direction(), rec.normal) > 0) {
			outward_normal = -rec.normal;

			/*光线的方向向量和球的法向量的点乘大于零，说明光线是从球体内部射入空气。
			所以，入射时的法向量和球的法向量方向相反；注意，ref_idx是指光密介质的折射指数和光疏介质的折射指数的比值，
			此时入射介质是光密介质，折射介质是光疏介质，所以ni_over_nt=ref_idx*/
			ni_over_nt = ref_idx;

			//cosine = ref_idx * dot(r_in.direction(), rec.normal) / r_in.direction().length();
			cosine = dot(r_in.direction(), rec.normal) / r_in.direction().length();
			cosine = sqrt(1 - ref_idx*ref_idx*(1-cosine*cosine));
		}
		else {
			outward_normal = rec.normal;

			/*光线的方向向量和球的法向量的点乘bu大于零，说明光线是从空气射入球体气。
			所以，入射时的法向量和球的法向量方向同向；注意，ref_idx是指光密介质的折射指数和光疏介质的折射指数的比值，
			此时入射介质是光疏介质，折射介质是光密介质，所以ni_over_nt=1.0/ref_idx*/
			ni_over_nt = 1.0 / ref_idx;

			cosine = -dot(r_in.direction(), rec.normal) / r_in.direction().length();
		}
		if (refract(r_in.direction(), outward_normal, ni_over_nt, refracted)) 
			reflect_prob = schlick(cosine, ref_idx);
		else 
			reflect_prob = 1.0;
		//if (drand48() < reflect_prob) 
		if ((rand() % (100) / (float)(100)) < reflect_prob)
			scattered = ray(rec.p, reflected);
		else 
			scattered = ray(rec.p, refracted);
		return true;
	}

};
#endif




