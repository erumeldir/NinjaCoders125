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

private:
	PhysicsEngine(void);
	virtual ~PhysicsEngine(void);

	static PhysicsEngine *pe;

	void flatCollision(ServerObject * theObj, Frame * flat);

	bool aabbCollision(const Box &bx1, const Box &bx2);

	// Configuration options
	float gravity;
};
typedef PhysicsEngine PE;
