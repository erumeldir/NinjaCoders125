#include "PhysicsEngine.h"
#define TIMESTEP 5

//Static members
PhysicsEngine *PhysicsEngine::pe;

PhysicsEngine::PhysicsEngine(void)
{
}


PhysicsEngine::~PhysicsEngine(void)
{
}


/*
 * applyPhysics(PhysicsModel *mdl)
 * Updates the physical position of the model and returns true if this object
 * should have collision physics applied.
 */
bool PhysicsEngine::applyPhysics(PhysicsModel *mdl) {
	float dt = TIMESTEP;

	//Update position
	float dx = 0.5 * mdl->accel.x * dt * dt + mdl->vel.x * dt,
		  dy = 0.5 * mdl->accel.y * dt * dt + mdl->vel.y * dt,
		  dz = 0.5 * mdl->accel.z * dt * dt + mdl->vel.z * dt;
	mdl->ref->translate(Point_t(dx, dy, dz));

	//Update velocity
	mdl->vel.x = mdl->accel.x * dt + mdl->vel.x;
	mdl->vel.y = mdl->accel.y * dt + mdl->vel.y;
	mdl->vel.z = mdl->accel.z * dt + mdl->vel.z;

	//Update acceleration
	mdl->accel = Vec3f();
	return true;	//We'll add a detection for has-moved later
}

void PhysicsEngine::applyPhysics(ServerObject *obj1, ServerObject *obj2) {
	//Collision checks/inter-object physics
}
