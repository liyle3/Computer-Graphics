#ifndef BVH_H
#define BVH_H

#include <vector>
#include <algorithm>
#include "rtweekend.h"
#include "hittable.h"
#include "hittable_list.h"
using namespace std;

class BVH : public hittable {
	private:
		shared_ptr<hittable> left;
		shared_ptr<hittable> right;
		AABB BOX;

	public:
		BVH(){}
		BVH(const hittable_list& list, double t0, double t1) :BVH(list.objects, 0, list.objects.size(), t0, t1) {}

		BVH(const vector<shared_ptr<hittable>>& src, size_t start, size_t end, double t0, double t1);
		
		
		virtual bool hit(const ray& r, double tmin, double tmax, hit_record& record) const override;

		virtual bool bounding_box(double t0, double t1, AABB& output_box) const override;

};

inline bool box_compare(const shared_ptr<hittable> a, const shared_ptr<hittable> b, int axis) {
	AABB BOXA, BOXB;

	if (!a->bounding_box(0, 0, BOXA) || b -> bounding_box(0, 0, BOXB)) {
		cerr << "No bounding box in bvh_node constructor.\n";
	}
	return BOXA.get_min().e[axis] < BOXB.get_min().e[axis];
}

bool box_x_compare(const shared_ptr<hittable> a, const shared_ptr<hittable> b){
	return box_compare(a, b, 0);
}

bool box_y_compare(const shared_ptr<hittable> a, const shared_ptr<hittable> b){
	return box_compare(a, b, 1);
}

bool box_z_compare(const shared_ptr<hittable> a, const shared_ptr<hittable> b) {
	return box_compare(a, b, 2);
}

BVH::BVH(const vector<shared_ptr<hittable>>& src, size_t start, size_t end, double t0, double t1) {
	auto objects = src;

	int axis = random_int(0, 2);
	auto comparator = axis == 0 ? box_x_compare :
		              axis == 1 ? box_y_compare :
		                            box_z_compare;

	size_t object_span = end - start;

	if (object_span == 1) {
		left = right = objects[start];
	}
	else if(object_span == 2){
		if (comparator(objects[start], objects[start + 1])) {
			left = objects[start];
			right = objects[start+1];
		}
		else {
			left = objects[start + 1];
			right = objects[start];
		}
	}
	else {
		sort(objects.begin() + start, objects.begin() + end, comparator);

		auto mid = start + object_span / 2;
		left = make_shared<BVH>(objects, start, mid, t0, t1);
		right = make_shared<BVH>(objects, mid, end, t0, t1);

	}

	AABB left_box, right_box;


	if (!left->bounding_box(t0, t1, left_box) || !right->bounding_box(t0, t1, right_box)) {
		cerr << "No bounding box in bvh_node constructor.\n";
	}

	BOX = surrounding_box(left_box, right_box);
}


bool BVH::hit(const ray& r, double tmin, double tmax, hit_record& record) const{
	if(!BOX.hit(r, tmin, tmax)) return false;
	bool l_hit = left -> hit(r, tmin, tmax, record);
	bool r_hit = right -> hit(r, tmin, l_hit ? record.t : tmax, record);

	return l_hit || r_hit;
}


bool BVH::bounding_box(double t0, double t1, AABB& output_box) const {
	output_box = BOX;
	return true;
}




#endif