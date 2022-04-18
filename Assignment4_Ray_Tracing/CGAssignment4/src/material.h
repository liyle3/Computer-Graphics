#ifndef MATERIAL_H
#define MATERIAL_H

#include "rtweekend.h"
#include "vec3.h"
#include "ray.h"

struct hit_record;

class material {
	public:
		virtual bool scatter(const ray& r_in, const hit_record& record, color& attenuation, ray& scattered) const = 0;
};

class lambertian :public material {
	public:
		lambertian(const color& a):albedo(a){}

		virtual bool scatter(const ray& r_in, const hit_record& record, color& attenuation, ray& scattered) const override {
			vec3 scatter_direction = record.normal + random_unit_vector();

			if (scatter_direction.near_zero())
				scatter_direction = record.normal;

			scattered = ray(record.intersection, scatter_direction);
			attenuation = albedo;
			return true;
		}
	private:
		color albedo;
};


class metal :public material {
	public:
		metal(const color& a, double f):albedo(a), fuzz(f < 1 ? f:1){}

		virtual bool scatter(const ray& r_in, const hit_record& record, color& attenuation, ray& scattered) const override {
			vec3 reflected = reflect(unit_vector(r_in.direction()), record.normal);
			scattered = ray(record.intersection, reflected + fuzz * random_in_unit_sphere());
			attenuation = albedo;

			return (dot(scattered.direction(), record.normal) > 0);

		}

	private:
		color albedo;
		double fuzz;
};


class dielectric :public material {
	public:
		dielectric(double ir) : ref_idx(ir) {}

		virtual bool scatter(
			const ray& r_in, const hit_record& record, color& attenuation, ray& scattered
		) const override {
			attenuation = color(1.0, 1.0, 1.0);
			double refraction_ratio = record.front_face ? (1.0 / ref_idx) : ref_idx;

			vec3 unit_direction = unit_vector(r_in.direction());
			double cos_theta = fmin(dot(-unit_direction, record.normal), 1.0);
			double sin_theta = sqrt(1.0 - cos_theta * cos_theta);

			bool cannot_refract = refraction_ratio * sin_theta > 1.0;
			vec3 direction;

			if (reflectance(cos_theta, refraction_ratio) > random_double())
				direction = reflect(unit_direction, record.normal);
			else
				direction = refract(unit_direction, record.normal, refraction_ratio);

			scattered = ray(record.intersection, direction);
			return true;
		}

	private:
		double ref_idx;  //’€…‰¬ 

		static double reflectance(double cosine, double ref_index) {
			auto r0 = (1.0 - ref_index) / (1.0 + ref_index);
			r0 = r0 * r0;
			return r0 + (1-r0) * pow((1-cosine), 5);
		}
};


#endif