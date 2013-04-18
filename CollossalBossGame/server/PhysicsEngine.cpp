#include "PhysicsEngine.h"
#include "ConfigurationManager.h"
#define TIMESTEP 5

//Movement Defines
#define GROUND_FRICTION 1.1f	//A bit excessive, but it works for now
#define AIR_FRICTION 1.f	//A bit excessive, but it works for now

//Collision Defines
#define SMALLRADIUS 5.0f
#define LARGERADIUS 30.0f

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

/*applyPhysics: figures out if the two objects collide and how to react
 *
 *	General Note: Currently dealing only in spheres
 *  Author: Bryan
 *  Alex note: separate detection and collision response
 */

#include <iostream>
void PhysicsEngine::applyPhysics(ServerObject *obj1, ServerObject *obj2) {
	//Collision checks/inter-object physics
	
	//0. Get physics models. call the get physics model method a lot
	PhysicsModel * p1 = obj1->getPhysicsModel();
	PhysicsModel * p2 = obj2->getPhysicsModel();

	//1. get their positions types (eventually size as well?)
	Point_t first = p1->ref->getPos();
	Point_t second = p2->ref->getPos();

	//2. get radii of each object
	CollisionBox size1 = p1->getColBox();	
	CollisionBox size2 = p2->getColBox();

	float r1 = (size1 == CB_SMALL) ? SMALLRADIUS : LARGERADIUS;
	float r2 = (size2 == CB_SMALL) ? SMALLRADIUS : LARGERADIUS;

	//3. calculate the distance
		float x21 = second.x - first.x;
		float y21 = second.y - first.y;
		float z21 = second.z - first.z;
	Vec3f dist = Vec3f(first.x - second.x, first.y - second.y, first.z - second.z);
	
		float d=sqrt(dist.x * dist.x + dist.y * dist.y + dist.z * dist.z);

	//Dealing with collisions!
	if (d<r1 + r2)
	{

		//Now we have two tasks: set the positions of the objects appropriately (A) and 
		// setting the velocities appropriately (B)

		//A. Set the positions, which we'll base on the difference.
		//1. We need the difference that we need to make up. 
		Vec3f positionDif = Vec3f (r1+r2 - fabs(dist.x), r1+r2 - fabs(dist.y), r1+r2 - fabs(dist.z));

		//2. the amounts that we need to move
		Vec3f divisor = p1->vel + p2->vel;
		Vec3f move1a = (p1->vel / divisor);
		Vec3f move1 = move1a * positionDif;
		Vec3f move2a = p2->vel / divisor;
		Vec3f move2 = move2a * positionDif;
		
		//3. do the movement
		Vec3f set1 = p1->ref->getPos() - move1;
		Vec3f set2 = p2->ref->getPos() - move2;
		p1->ref->setPos(set1);
		p2->ref->setPos(set2);
		p1->vel = Vec3f(0, 0, 0);
		p2->vel = Vec3f(0, 0, 0);
		/*
		Vec3f force = p1->vel * direction;
		//B. Set the velocities
		p1->applyForce( force );

		Vec3f firstMom = Vec3f( p1->vel.x * p1->mass,
			p1->vel.y * p1->mass,
			p1->vel.z * p1->mass);

		//I got to override an operator! :D
		moment = moment - firstMom;

		//Overriding crazy! Also storing the final velocity in it
		p2->applyForce(moment * direction);
	*/
	}

}

