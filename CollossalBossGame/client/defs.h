/*
 * defs.h
 * Standard definitions and includes that would be useful for most objects
 */
#ifndef DEFS_H
#define DEFS_H
//Standard includes
#include <stdio.h>

typedef unsigned int uint;

typedef struct Vec3f {
	Vec3f() {
		x = y = z = 0.f;
	}
	Vec3f(float x, float y, float z) {
		this->x = x;
		this->y = y;
		this->z = z;
	}
	float x, y, z;
} Point_t, Rot_t;

#endif