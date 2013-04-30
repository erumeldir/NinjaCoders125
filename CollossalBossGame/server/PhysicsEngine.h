#pragma once
#include "PhysicsModel.h"
#include "ServerObject.h"

class PhysicsEngine
{
public:
	static void init() { pe = new PhysicsEngine(); }
	static PhysicsEngine *get() { return pe; }
	static void clean() { delete pe; }

	bool applyPhysics(ServerObject *obj);
	void applyPhysics(ServerObject *obj1, ServerObject *obj2);
	void applyPhysics(ServerObject *obj1, ServerObject *obj2, Box b1, Box b2);

	void setLimits(float xNeg, float yNeg, float zNeg, float xPos, float yPos, float zPos) {
		this->xPos = xPos;
		this->xNeg = xNeg;
		this->yPos = yPos;
		this->yNeg = yNeg;
		this->zPos = zPos;
		this->zNeg = zNeg;
	}

	inline void setGravDir(uint dir) {
		gravDir = dir;

		if(dir & NORTH) {
			gravVec.z = 1;
		} else if(dir & SOUTH) {
			gravVec.z = -1;
		} else {
			gravVec.z = 0;
		}

		if(dir & EAST) {
			gravVec.x = 1;
		} else if(dir & WEST) {
			gravVec.x = -1;
		} else {
			gravVec.x = 0;
		}

		if(dir & UP) {
			gravVec.y = 1;
		} else if(dir & DOWN) {
			gravVec.y = -1;
		} else {
			gravVec.y = 0;
		}
	}

	inline uint getGravDir() { return gravDir; }
	inline const Vec3f &getGravVec() { return gravVec; }

private:
	PhysicsEngine(void);
	virtual ~PhysicsEngine(void);

	static PhysicsEngine *pe;

	void flatCollision(ServerObject * theObj, Frame * flat);

	bool aabbCollision(const Box &bx1, const Box &bx2);

	// Configuration options
	float gravMag;
	Vec3f gravVec;
	uint gravDir;

	//Position caps
	float xPos, yPos, zPos,
		  xNeg, yNeg, zNeg;
};
typedef PhysicsEngine PE;
