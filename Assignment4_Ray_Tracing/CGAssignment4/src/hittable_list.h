#ifndef HITTABLE_LIST_H
#define HITTABLE_LIST_H

#include "hittable.h"
#include <memory>
#include <vector>
using std::shared_ptr;
using std::make_shared;
class hittable_list : public hittable {
public:
	hittable_list() {}
	hittable_list(shared_ptr<hittable> object) { add(object); }
	void clear() { objects.clear(); }
	void add(shared_ptr<hittable> object) { objects.push_back(object); }
	virtual bool hit(
		const ray& r, double t_min, double t_max, hit_record& record) const
		override;
	virtual bool bounding_box(double time0, double time1, AABB& output_boxdouble) const override;
public:
	std::vector<shared_ptr<hittable>> objects;
};


bool hittable_list::hit(const ray& r, double t_min, double t_max, hit_record&
	record) const {
	hit_record temp_record;
	bool hit_anything = false;
	auto closest = t_max;
	for (const auto& object : objects) {
		if (object->hit(r, t_min, closest, temp_record)) {
			hit_anything = true;
			closest = temp_record.t;
			record = temp_record;
		}
	} 
	return hit_anything;
}

bool hittable_list::bounding_box(double t0, double t1, AABB& output_box) const{
	if(objects.empty()) return false;

	AABB BOX;
	bool first_box = true;
	for (const auto& object : objects) {
		if(!object->bounding_box(t0, t1, BOX)) 
			return false;
		output_box = first_box ? BOX :surrounding_box(output_box, BOX);
		first_box = false;
	}
	return true;
}

#endif