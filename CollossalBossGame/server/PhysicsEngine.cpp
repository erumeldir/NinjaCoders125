#include "PhysicsEngine.h"
#include "ConfigurationManager.h"
#define TIMESTEP 5
#define GROUND_FRICTION 1.1f	//A bit excessive, but it works for now
#define AIR_FRICTION 1.f	//A bit excessive, but it works for now

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

	if(mdl->isStatic) return true;

	//Apply additional forces, such as gravity and friction.
	// We are ignoring both for now and applying a half-assed version of
	// friction when we update the velocity.
	Vec3f downVector = Vec3f(0, -1, 0);
	float gravity = CM::get()->find_config_as_float("GRAVITY_FORCE");
	mdl->applyAccel(downVector*gravity);

	//Update position
	float dx = 0.5 * mdl->accel.x * dt * dt + mdl->vel.x * dt,
		  dy = 0.5 * mdl->accel.y * dt * dt + mdl->vel.y * dt,
		  dz = 0.5 * mdl->accel.z * dt * dt + mdl->vel.z * dt;
	mdl->ref->translate(Point_t(dx, dy, dz));

	// if pos < 0 reset y pos to 0 and if y vel < 0 clear out y vel set friction to 1.3, else friction = 0
	Point_t pos = mdl->ref->getPos();
	if (pos.y < 0) {
		mdl->ref->setPos(Point_t(pos.x, 0, pos.z));
		mdl->onGround = true;
		mdl->frictCoeff = GROUND_FRICTION;
		mdl->accel.y = 0;
		if(mdl->vel.y < 0) {
			mdl->vel.y = 0;
		}
	} else {
		mdl->onGround = false;
		mdl->frictCoeff = AIR_FRICTION;
	}

	//Update velocity
	mdl->vel.x = mdl->accel.x * dt + mdl->vel.x / mdl->frictCoeff;
	mdl->vel.y = mdl->accel.y * dt + mdl->vel.y / mdl->frictCoeff;
	mdl->vel.z = mdl->accel.z * dt + mdl->vel.z / mdl->frictCoeff;

	//Update acceleration
	mdl->accel = Vec3f();

	return true;	//We'll add a detection for has-moved later
}

void PhysicsEngine::applyPhysics(ServerObject *obj1, ServerObject *obj2) {
	//Collision checks/inter-object physics
}
