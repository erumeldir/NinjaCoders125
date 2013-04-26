/*
 * defs.cpp
 * Definitions for methods defined in defs.h
 */
#include "defs.h"

Quat_t inverse(const Quat_t &q) {
	return Quat_t(-q.x, -q.y, -q.z, q.w);
}

float magnitude(const Vec3f &v) {
	return sqrt(v.x * v.x + v.y * v.y + v.z * v.z);
}

float magnitude(const Vec4f &v) {
	return sqrt(v.x * v.x + v.y * v.y + v.z * v.z + v.w * v.w);
}