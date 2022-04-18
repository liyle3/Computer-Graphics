# ifndef HITTABLE_H
# define HITTABLE_H
#include "ray.h"
#include "AABB.h"

class material;
struct hit_record {
	point3 intersection; //交点
	vec3 normal;   //法向量
	shared_ptr<material> mat_ptr;
	double t;      //射线的系数t
	bool front_face;    //是否为正面
	inline void set_face_normal(const ray& r, const vec3& outward_normal) {
		front_face = dot(r.direction(), outward_normal) < 0;
		normal = front_face ? outward_normal : -outward_normal;
	}
};
class hittable {
	public:
	virtual bool hit(const ray& r, double t_min, double t_max, hit_record& record) const = 0;

	virtual bool bounding_box(double time0, double time1, AABB& output_boxdouble) const = 0;
};


#endif