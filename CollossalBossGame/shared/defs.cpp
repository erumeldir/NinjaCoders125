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

Vec3f rotate(const Vec3f &v, const Quat_t &q) {
	//Convert the quaternion to matrix values
	float f00, f01, f02,
		  f10, f11, f12,
		  f20, f21, f22;
	f00 = 1 - 2 * q.y * q.y - 2 * q.z * q.z;	//1 - 2y^2 - 2z^2
	f01 = 2 * q.x * q.y - 2 * q.w * q.z;		//2xy - 2wz
	f02 = 2 * q.x * q.z + 2 * q.w * q.y;		//2xz + 2wy
	f10 = 2 * q.x * q.y + 2 * q.w * q.z;		//2xy + 2wz
	f11 = 1 - 2 * q.x * q.x - 2 * q.z * q.z;	//1 - 2x^2 - 2z^2
	f12 = 2 * q.y * q.z - 2 * q.w * q.x;		//2yz - 2wx
	f20 = 2 * q.x * q.z - 2 * q.w * q.y;		//2xz - 2wy
	f21 = 2 * q.y * q.z + 2 * q.w * q.x;		//2yz + 2wx
	f22 = 1 - 2 * q.x * q.x - 2 * q.y * q.y;	//1 - 2x^2 - 2y^2

	//Perform matrix multiplication
	return Vec3f(
			v.x * f00 + v.y * f01 + v.z * f02,
			v.x * f10 + v.y * f11 + v.z * f12,
			v.x * f20 + v.y * f21 + v.z * f22
		);
}
