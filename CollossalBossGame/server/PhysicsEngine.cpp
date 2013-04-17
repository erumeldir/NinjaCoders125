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

	//Apply additional forces, such as gravity and friction.
	// We are ignoring both for now and applying a half-assed version of
	// friction when we update the velocity.

	//Update position
	float dx = 0.5 * mdl->accel.x * dt * dt + mdl->vel.x * dt,
		  dy = 0.5 * mdl->accel.y * dt * dt + mdl->vel.y * dt,
		  dz = 0.5 * mdl->accel.z * dt * dt + mdl->vel.z * dt;
	mdl->ref->translate(Point_t(dx, dy, dz));

	//Update velocity
	mdl->vel.x = mdl->accel.x * dt + mdl->vel.x / mdl->frictCoeff;
	mdl->vel.y = mdl->accel.y * dt + mdl->vel.y / mdl->frictCoeff;
	mdl->vel.z = mdl->accel.z * dt + mdl->vel.z / mdl->frictCoeff;

	//Update acceleration
	mdl->accel = Vec3f();

	return true;	//We'll add a detection for has-moved later
}

/*applyPhysics: figures out if the two objects collide and how to react
 *
 *	General Note: Currently dealing only in spheres
 *  Author: Bryan
 */

void PhysicsEngine::applyPhysics(ServerObject *obj1, ServerObject *obj2) {
	//Collision checks/inter-object physics
	
	//1. get their positions types (eventually size as well?)
	Point_t first = obj1->getPhysicsModel()->ref->getPos();
	Point_t second = obj2->getPhysicsModel()->ref->getPos();

	//2. get radii of each object
	CollisionBox size1 = obj1->getPhysicsModel()->getColBox();	
	CollisionBox size2 = obj2->getPhysicsModel()->getColBox();

	float r1 = (size1 == CB_SMALL) ? 5.0f : 20.0f;
	float r2 = (size2 == CB_SMALL) ? 5.0f : 20.0f;

	//3. calculate the distance
	float distX = first.x - second.x;
	float distY = first.y - second.y;
	float distZ = first.z - second.z;

	float squareDist = distX * distX + distY * distY + distZ * distZ;
	
	//4. find if we collided
	bool collided = squareDist < (r1 + r2) * (r1 + r2);

	//Dealing with collisions!
	if (collided)
	{
		/*General principle: conservation of momentum 
		 *	If we were to have this be absolutely accurate, we would need
		 *   a gauge of elasticity for both objects. However since we don't
		 *   have elasticity, we will instead use a difference of mass as 
		 *   a factor.
		 *
		 */

		//1. calculate momentum. 
		Vec3f moment = Vec3f(obj1->getPhysicsModel()->mass * obj1->getPhysicsModel()->vel.x
								+ obj2->getPhysicsModel()->mass * obj2->getPhysicsModel()->vel.x,
								obj1->getPhysicsModel()->mass * obj1->getPhysicsModel()->vel.y
								+ obj2->getPhysicsModel()->mass * obj2->getPhysicsModel()->vel.y,
								obj1->getPhysicsModel()->mass * obj1->getPhysicsModel()->vel.z
								+ obj2->getPhysicsModel()->mass * obj2->getPhysicsModel()->vel.z);

		//2. calculate the percent difference of masses to indicate the final direction of obj1
		float direction = (obj1->getPhysicsModel()->mass - obj2->getPhysicsModel()->mass) 
								/(obj1->getPhysicsModel()->mass + obj2->getPhysicsModel()->mass);

		//Note: if direction is 0, then half of obj1's momentum is transferred to obj2.
		// this forms our anchor point. we got the percent difference of mass as an
		// easy way to adjust this number..
		direction = direction + .5f;

		obj1->getPhysicsModel()->vel.x = obj1->getPhysicsModel()->vel.x * direction;
		obj1->getPhysicsModel()->vel.y = obj1->getPhysicsModel()->vel.y * direction;
		obj1->getPhysicsModel()->vel.z = obj1->getPhysicsModel()->vel.z * direction;

		Vec3f firstMom = Vec3f( obj1->getPhysicsModel()->vel.x * obj1->getPhysicsModel()->mass,
			obj1->getPhysicsModel()->vel.y * obj1->getPhysicsModel()->mass,
			obj1->getPhysicsModel()->vel.z * obj1->getPhysicsModel()->mass);

		//I got to override an operator! :D
		moment = moment - firstMom;

		//Overriding crazy! Also storing the final velocity in it
		obj2->getPhysicsModel()->vel = moment/obj1->getPhysicsModel()->mass;


	}
}

