/*
 * defs.h
 * Standard definitions and includes that would be useful for most objects
 */
#ifndef DEFS_H
#define DEFS_H

//Standard includes
#include <stdio.h>
#include "DebugConsole.h"

//Constants (that we don't want to change, if we might, they should go in the config file)
#define M_PI 3.14159

//Macros
#define GET_FLAG(flags, flag)        ((flags >> flag) & 0x1)
#define SET_FLAG(flags, flag, val)   ((val) ? (flags | (1 << flag)) : (flags & ~(1 << flag)))

//Typedefs
typedef unsigned int uint;

//Types
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

	Vec3f operator-	(Vec3f rhs) {
		return Vec3f(this->x - rhs.x, 
			this->y - rhs.y, 
			this->z - rhs.z);
	}
	
	Vec3f operator+	(Vec3f rhs) {
		return Vec3f(this->x + rhs.x, 
			this->y + rhs.y, 
			this->z + rhs.z);
	}

	Vec3f operator*	(float rhs) {
		return Vec3f(this->x * rhs, 
			this->y * rhs, 
			this->z * rhs);
	}

	Vec3f operator/	(float rhs) {
		return Vec3f(this->x / rhs, 
			this->y / rhs, 
			this->z / rhs);
	}

	Vec3f operator*	(Vec3f rhs) {
		return Vec3f(this->x * rhs.x, 
			this->y * rhs.y, 
			this->z * rhs.z);
	}

	Vec3f operator/	(Vec3f rhs) {
		return Vec3f(this->x / rhs.x, 
			this->y / rhs.y, 
			this->z / rhs.z);
	}

} Point_t, Rot_t;

//Enumerations
/*
 * Actions that are sent from the client to the server
 */
typedef enum ACTION {
	ACT_MOVE_X,
	ACT_MOVE_Y,
	ACT_TURN_X,
	ACT_TURN_Y,
	ACT_FOCUS,
	ACT_JUMP,
	ACT_SPECIAL,
	ACT_ATTACK,
	ACT_NUM_ACTIONS
};

#endif