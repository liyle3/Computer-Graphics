#ifndef AABB_H
#define AABB_H

#include "rtweekend.h"
#include "ray.h"
#include "vec3.h"
#include <cmath>
using namespace std;

class AABB {
	private:
		point3 _min, _max;

	public:
		AABB(){}
		AABB(const point3& a, const point3& b){
			_min = a;
			_max = b;
		}
		

		point3 get_min() const{
			return _min;
		}

		point3 get_max() const{
			return _max;
		}


		bool hit(const ray& r, double tmin, double tmax) const;
};


inline bool AABB::hit(const ray& r, double tmin, double tmax) const {
	for (int i = 0; i < 3; i++) {
		auto invD = 1.0f / r.direction()[i];
		double t0 = (this->get_min()[i] - r.origin()[i]) * invD;
		double t1 = (this->get_max()[i] - r.origin()[i]) * invD;

		if (invD < 0.0f) {
			std::swap(t0, t1);
		}

		tmin = t0 > tmin ? t0 : tmin;
		tmax = t1 < tmax ? t1 : tmax;
		if(tmax <= tmin)
			return false;
	}
	return true;
}

inline AABB surrounding_box(AABB box0, AABB box1) {
	point3 small(fmin(box0.get_min().x(), box1.get_min().x()),
		         fmin(box0.get_min().y(), box1.get_min().y()), 
		         fmin(box0.get_min().z(), box1.get_min().z()));

	point3 big(fmax(box0.get_max().x(), box1.get_max().x()),
		       fmax(box0.get_max().y(), box1.get_max().y()),
		       fmax(box0.get_max().z(), box1.get_max().z()));
	

	return AABB(small, big);
}



#endif