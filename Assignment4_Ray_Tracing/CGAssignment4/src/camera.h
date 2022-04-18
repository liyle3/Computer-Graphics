#ifndef CAMERA_H
#define CAMERA_H
#include "rtweekend.h"
#include "vec3.h"
#include "ray.h"

class camera {
	private:
		point3 origin;   //原点
		point3 lower_left_corner;  //左下角坐标
		vec3 horizontal;           //视口的水平向量
		vec3 vertical;             //视口的垂直向量
		vec3 u, v, w;
		double lens_radius;

	public:
		camera(point3 lookfrom, point3 lookat, vec3 vup, double vfov, double aspect_ratio, double aperture, double focus_distance) {
			auto theta = degree2radian(vfov);
			auto h = tan(theta / 2.0);

			double viewport_height = 2.0 * h;
			double viewport_width = aspect_ratio * viewport_height;

		    w = unit_vector(lookfrom - lookat);
			u = unit_vector(cross(vup, w));
			v = cross(w, u);

			origin = lookfrom;
			horizontal = focus_distance * viewport_width * u;
			vertical = focus_distance * viewport_height * v;
			lower_left_corner = origin - horizontal / 2 - vertical / 2 - focus_distance * w;
			lens_radius = aperture / 2;
		}
		ray get_ray(double s, double t) const{
			vec3 rd = lens_radius * random_in_unit_disk();
			vec3 offset = u * rd.x() + v * rd.y();
			return ray(origin + offset, lower_left_corner + s * horizontal + t * vertical - origin - offset);
		
		}

};


#endif