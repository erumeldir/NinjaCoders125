#pragma once
#include "PhysicsModel.h"

class PhysicsEngine
{
public:
	static void init() { pe = new PhysicsEngine(); }
	static PhysicsEngine *get() { return pe; }
	static void clean() { delete pe; }

	bool applyPhysics(PhysicsModel *mdl);
	void applyPhysics(PhysicsModel *mdl1, PhysicsModel *mdl2);

private:
	PhysicsEngine(void);
	virtual ~PhysicsEngine(void);

	static PhysicsEngine *pe;
};
typedef PhysicsEngine PE;
