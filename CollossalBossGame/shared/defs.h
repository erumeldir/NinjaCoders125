/*
 * defs.h
 * Standard definitions and includes that would be useful for most objects
 */
#ifndef DEFS_H
#define DEFS_H

//Standard includes
#include <stdio.h>
#include "DebugConsole.h"
#include <set>

//Constants (that we don't want to change, if we might, they should go in the config file)
#define M_PI 3.14159
#define M_TAU 6.2832
#define AIR_FRICTION 1.1f	//A bit excessive, but it works for now
#define GROUND_FRICTION 1.1f	//A bit excessive, but it works for now

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

	Vec3f operator-	(const Vec3f &rhs) const {
		return Vec3f(this->x - rhs.x, 
			this->y - rhs.y, 
			this->z - rhs.z);
	}
	
	Vec3f operator+	(const Vec3f &rhs) const {
		return Vec3f(this->x + rhs.x, 
			this->y + rhs.y, 
			this->z + rhs.z);
	}

	Vec3f operator*	(float rhs) const {
		return Vec3f(this->x * rhs, 
			this->y * rhs, 
			this->z * rhs);
	}

	Vec3f operator/	(float rhs) const  {
		if (rhs == 0)
			return Vec3f(0, 0, 0);
		else
			return Vec3f(this->x / rhs, 
				this->y / rhs, 
				this->z / rhs);
	}

	Vec3f operator*	(const Vec3f &rhs) const {
		return Vec3f(this->x * rhs.x, 
			this->y * rhs.y, 
			this->z * rhs.z);
	}

	// DOT PRODUCT!
	float operator^	(const Vec3f &rhs) const {
		return (this->x * rhs.x +
			this->y * rhs.y + 
			this->z * rhs.z);
	}

	Vec3f operator/	(const Vec3f &rhs) const {
		float x = (rhs.x == 0) ? 0 : this->x / rhs.x;
		float y = (rhs.y == 0) ? 0 : this->y / rhs.y;
		float z = (rhs.z == 0) ? 0 : this->z / rhs.z;
		return Vec3f(x, y, z);
	}

	//Scalar ops
	void operator*= (float s) {
		x *= s;
		y *= s;
		z *= s;
	}

	void operator/= (float s) {
		x /= s;
		y /= s;
		z /= s;
	}

} Point_t, Rot_t;

//Axis-aligned bounding box
typedef struct Box {
	float x, y, z;
	float w, h, l;

	Box() {
		x = y = z = w = l = h = 0;
	}
	
	Box(float x, float y, float z, float w, float h, float l) {
		this->x = x;
		this->y = y;
		this->z = z;
		this->w = w;
		this->h = h;
		this->l = l;
	}

	Box operator+ (const Vec3f &pt) const {
		return Box(x + pt.x, y + pt.y, z + pt.z,
				   w,        h,        l);
	}
} Vol_t;

//Enumerations
/*
 * Actions that are sent from the client to the server
 */

typedef enum OBJ_FLAG {
	//General flags
	IS_HEALTHY,
	IS_HARMFUL,
	IS_WALL,
	//Physics flags
	IS_STATIC,
	IS_PASSABLE,
	IS_FALLING
};

typedef enum DIRECTION {
	NONE  = 0x0,
	NORTH = 0x1,	//+z
	EAST  = 0x2,	//+x
	UP    = 0x4,	//+y
	SOUTH = 0x8,	//-z
	WEST  = 0x10,	//-x
	DOWN  = 0x20	//-y
};


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