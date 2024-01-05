#pragma once

#include "hittable.h"
#include "vec3.h"

class sphere : public hittable {
public:
	// Stationary Sphere
	sphere(point3 _center, double _radius, shared_ptr<material> _material) 
		: center(_center), radius(_radius), mat(_material) 
	{
		auto rvec = vec3(radius, radius, radius);
		bbox = aabb(center - rvec, center + rvec);
	}


	bool hit(const ray& r, interval ray_t, hit_record& rec) const override {
		vec3 oc = r.origin() - center;
		auto a = dot(r.direction(), r.direction());
		auto half_b = dot(oc, r.direction());
		auto c = dot(oc, oc) - radius * radius;

		auto discriminant = half_b * half_b - a * c;

		//no hit
		if (discriminant < 0) return false;
		auto sqrtd = sqrt(discriminant);

		// Find the nearest root that lies in the acceptable range.
		auto root = (-half_b - sqrtd) / a;
		if (!ray_t.surrounds(root)) {
			root = (-half_b + sqrtd) / a;
			if (!ray_t.surrounds(root)) {
				return false;
			}
		}

		rec.t = root;
		rec.p = r.at(rec.t);
		vec3 outward_normal = (rec.p - center) / radius;
		rec.set_face_normal(r, outward_normal);
		rec.mat = mat;

		return true;
	}

	aabb bounding_box() const override { return bbox; }

private:

	point3 center;
	double radius;
	shared_ptr<material> mat;
	aabb bbox;

};