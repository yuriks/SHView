#pragma once

#include "math/Vector.hpp"
#include "math/Matrix.hpp"
#include <cmath>

namespace hw {

inline bool spherePlaneIntersection(const vec4& sphere /* center + radius */, const vec4& plane /* normal + distance */) {
	// Assume the sphere is on the origin and translate the plane to match
	float plane_distance = plane.getW() - dot(vec3(sphere), vec3(plane));

	// Since sphere is on origin, and plane_distance is the ditsance between the plane and the origin
	return std::abs(plane_distance) <= sphere.getW();
}

inline vec4 normalizePlane(const vec4& p) {
	float len = invLength(vec3(p));
	return p * len;
}

enum FrustumPlanes {
	PLANE_LEFT, PLANE_RIGHT, PLANE_BOTTOM, PLANE_TOP, PLANE_NEAR, PLANE_FAR
};

inline void extractFrustumPlanes(const mat4& in_matrix, vec4 out_planes[6]) {
	// Extracted planes are in the following order:
	// left, right, bottom, top, near, far

	auto& m = in_matrix;
	auto& p = out_planes;

	/*  left */ p[0] = normalizePlane(m.rows[4] + m.rows[1]);
	/* right */ p[1] = normalizePlane(m.rows[4] - m.rows[1]);
	/* botom */ p[2] = normalizePlane(m.rows[4] + m.rows[2]);
	/*   top */ p[3] = normalizePlane(m.rows[4] - m.rows[2]);
	/*  near */ p[4] = normalizePlane(m.rows[4] + m.rows[3]);
	/*   far */ p[5] = normalizePlane(m.rows[4] - m.rows[3]);
}

} // namespace hw
