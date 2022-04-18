#ifndef RAY_H
#define RAY_H

#include "vec3.h"

class ray {
	public:
		ray(){}
		ray(const point3 & _origin, const vec3 & _direction):orig(_origin), dir(_direction){}
		point3 origin() const{return orig;}
		vec3 direction() const{return dir;}
		point3 at(double t) const{
			return orig + t * dir;
		}
	private:
		point3 orig;
		vec3 dir;
};

#endif // RAY_H
