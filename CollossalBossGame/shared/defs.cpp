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

/*
 * Performs the same operation as rotate(Vec3f(0,1,0), q), but
 * simplifies the math so that fewer calculations need to be performed
 */
Vec3f rotateUp(const Quat_t &q) {
	//Convert the quaternion to matrix values
	float f01, f11, f21;
	f01 = 2 * q.x * q.y - 2 * q.w * q.z;		//2xy - 2wz
	f11 = 1 - 2 * q.x * q.x - 2 * q.z * q.z;	//1 - 2x^2 - 2z^2
	f21 = 2 * q.y * q.z + 2 * q.w * q.x;		//2yz + 2wx
	
	//Perform matrix multiplication
	Vec3f res = Vec3f(f01, f11, f21);	//only the y values are kept
	res.normalize();
	return res;
}

/*
 * Performs the same operation as rotate(Vec3f(1,0,0), q), but
 * simplifies the math so that fewer calculations need to be performed
 */
Vec3f rotateRight(const Quat_t &q) {
	//Convert the quaternion to matrix values
	float f00, f10, f20;
	f00 = 1 - 2 * q.y * q.y - 2 * q.z * q.z;	//1 - 2y^2 - 2z^2
	f10 = 2 * q.x * q.y + 2 * q.w * q.z;		//2xy + 2wz
	f20 = 2 * q.x * q.z - 2 * q.w * q.y;		//2xz - 2wy
	
	//Perform matrix multiplication
	Vec3f res = Vec3f(f00, f10, f20);	//only the x values are kept
	res.normalize();
	return res;
}

/*
 * Performs the same operation as rotate(Vec3f(0,0,1), q), but
 * simplifies the math so that fewer calculations need to be performed
 */
Vec3f rotateFwd(const Quat_t &q) {
	//Convert the quaternion to matrix values
	float f02, f12, f22;
	f02 = 2 * q.x * q.z + 2 * q.w * q.y;		//2xz + 2wy
	f12 = 2 * q.y * q.z - 2 * q.w * q.x;		//2yz - 2wx
	f22 = 1 - 2 * q.x * q.x - 2 * q.y * q.y;	//1 - 2x^2 - 2y^2

	//Perform matrix multiplication
	Vec3f res = Vec3f(f02, f12, f22);
	res.normalize();
	return res;
}

void cross(Vec3f *res, const Vec3f &v1, const Vec3f &v2) {
	res->x = v1.y * v2.z - v1.z * v2.y;
	res->y = v1.z * v2.x - v1.x * v2.z;
	res->z = v1.x * v2.y - v1.y * v2.x;
}
