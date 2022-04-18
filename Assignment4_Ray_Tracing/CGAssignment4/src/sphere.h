#ifndef SPHERE_H
#define SPHERE_H

#include "vec3.h"
#include "hittable.h"

class sphere : public hittable {
	public:
		sphere(){}
		sphere(point3 _center, double _radius, shared_ptr<material> m):center(_center), radius(_radius), mat_ptr(m){}
		point3 get_center() const{return center;}
		double get_radius() const{return radius;}
		virtual bool hit(const ray& r, double t_min, double t_max, hit_record& record) const override;
		virtual bool bounding_box(double time0, double time1, AABB& output_boxdouble) const override;

	private:
		point3 center;
		double radius;
		shared_ptr<material> mat_ptr;

};

bool sphere::hit(const ray& r, double t_min, double t_max, hit_record& record) const {
	vec3 origin_center = r.origin() - center;
	double a = r.direction().length_squared();
	double half_b = dot(r.direction(), origin_center);
	double c = origin_center.length_squared() - radius * radius;
	double delta = half_b * half_b - a * c;
	if (delta > 0) {
		double t = (-half_b - sqrt(delta)) / a;
		if (t > t_min && t < t_max) {
			record.intersection = r.at(t);
			record.t = t;
			vec3 outward_normal = (r.at(t) - center) / radius;
			record.set_face_normal(r, outward_normal);
			record.mat_ptr = mat_ptr;
			return true;
		}
		t = (-half_b + sqrt(delta)) / a;
		if (t > t_min && t < t_max) {
			record.intersection = r.at(t);
			record.t = t;
			vec3 outward_normal = r.at(t) - center;
			record.set_face_normal(r, outward_normal);
			record.mat_ptr = mat_ptr;
			return true;
		}
	}

	return false;
}


bool sphere::bounding_box(double t0, double t1, AABB& output_box) const {
	output_box = AABB(
		center - vec3(radius, radius, radius),
		center + vec3(radius, radius, radius));
	return true;
}



#endif