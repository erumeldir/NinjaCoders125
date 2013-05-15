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

	void setGravDir(DIRECTION dir);

	inline DIRECTION getGravDir() { return gravDir; }
	inline const Vec3f &getGravVec() { return gravVec; }
	inline const Quat_t &getCurGravRot() { return curGravRot; }

private:
	PhysicsEngine(void);
	virtual ~PhysicsEngine(void);

	static PhysicsEngine *pe;

	void flatCollision(ServerObject * theObj, Frame * flat);

	bool aabbCollision(const Box &bx1, const Box &bx2);

	// Configuration options
	float gravMag;
	Vec3f gravVec;
	DIRECTION gravDir;
	Quat_t curGravRot;

	//Position caps
	float xPos, yPos, zPos,
		  xNeg, yNeg, zNeg;
};
typedef PhysicsEngine PE;
