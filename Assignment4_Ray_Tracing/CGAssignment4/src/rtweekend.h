#ifndef RTWEEKEND_H
#define RTWEEKEND_H
#include <cmath>
#include <cstdlib>
#include <limits>
#include <memory>

using std::shared_ptr;
using std::make_shared;
using std::sqrt;

//常量
const double infinity = std::numeric_limits<double>::infinity();  //无穷大
const double pi = 3.1415926535897932385;

inline double degree2radian(double degrees) {
	return degrees * pi / 180.0;
}

inline double random_double() {
	return rand() / (RAND_MAX + 1.0);
}

inline double random_double(double max, double min) {
	return min + (max - min) * random_double();
}

inline int random_int(int min, int max) {
	return static_cast<int>(random_double(min, max+1));
}

inline double clamp(double x, double min, double max) {
	if(x > max) return max;
	if(x < min) return min;
	return x;
}

#endif