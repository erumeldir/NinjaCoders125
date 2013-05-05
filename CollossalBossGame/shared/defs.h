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
#define M_PI 3.141592653589793238462643383279502884197169399375105820974944592307816406286
#define M_TAU (2 * M_PI)
#define AIR_FRICTION 1.001f
#define GROUND_FRICTION 1.1f

//Macros
#define GET_FLAG(flags, flag)        ((flags >> flag) & 0x1)
#define SET_FLAG(flags, flag, val)   ((val) ? (flags | (1 << flag)) : (flags & ~(1 << flag)))

// Forward Declare
struct Vec3f;

// Functions
// This goes up here because it's used inside Vec3f
float magnitude(const Vec3f &v);

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

	bool operator< (const Vec3f &rhs) const {
		// First, check magnitude
		float res =	magnitude(*this) - magnitude(rhs);
		if (res != 0) return res < 0;

		// If same, check each component (rather arbitrarily)
		if (this->z != rhs.z) return this->z < rhs.z;
		if (this->x != rhs.x) return this->x < rhs.x;
		return this->y < rhs.y;
	}

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

	void operator+= (const Vec3f &rhs) {
		x += rhs.x;
		y += rhs.y;
		z += rhs.z;
	}

	void operator-= (const Vec3f &rhs) {
		x -= rhs.x;
		y -= rhs.y;
		z -= rhs.z;
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
	void normalize() {
		float mag = sqrt(x * x + y * y + z * z);
		x /= mag;
		y /= mag;
		z /= mag;
	}
} Point_t;	//, Rot_t;

typedef struct Vec4f {
	//   (0, 1, 2, 3)
	float w, x, y, z;

	Vec4f() {
		x = y = z = 0.0f;
		w = 1.0f;
	}

	Vec4f(float x, float y, float z, float w) {
		this->x = x;
		this->y = y;
		this->z = z;
		this->w = w;
	}

	Vec4f(const Vec3f &axis, float angle) {
		float s = cos(angle / 2),
			  t = sin(angle / 2);
		this->w = s;
		this->x = axis.x * t;
		this->y = axis.y * t;
		this->z = axis.z * t;
		normalize();
	}

	//http://www.mathworks.com/help/aeroblks/quaternionmultiplication.html
	void operator*=(const Vec4f &rhs) {
		float w2 = w * rhs.w - x * rhs.x - y * rhs.y - z * rhs.z,
			  x2 = w * rhs.x + x * rhs.w + y * rhs.z - z * rhs.y,
			  y2 = w * rhs.y - x * rhs.z + y * rhs.w + z * rhs.x,
			  z2 = w * rhs.z + x * rhs.y - y * rhs.x + z * rhs.w;
		w = w2;
		x = x2;
		y = y2;
		z = z2;
	}

	Vec4f operator*(const Vec4f &rhs) const {
		Vec4f res = *this;
		res *= rhs;
		return res;
	}

	inline void normalize() {
		float mag = sqrt(x * x + y * y + z * z + w * w);
		x /= mag;
		y /= mag;
		z /= mag;
		w /= mag;
	}
} Quat_t;

Quat_t inverse(const Quat_t &q);
float magnitude(const Vec4f &v);
float angle(const Vec3f &v1, const Vec3f &v2);
Vec3f rotate(const Vec3f &v, const Quat_t &q);
void  getCorrectedAxes(const Quat_t &q, Vec3f *fwd, Vec3f *up, Vec3f *right);
Vec3f rotateUp(const Quat_t &q);
Vec3f rotateRight(const Quat_t &q);
Vec3f rotateFwd(const Quat_t &q);
void cross(Vec3f *res, const Vec3f &v1, const Vec3f &v2);
void slerp(Quat_t *res, const Quat_t &start, const Quat_t &end, float t);

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