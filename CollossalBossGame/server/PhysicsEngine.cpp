#include "PhysicsEngine.h"
#define TIMESTEP 5
#define SMALLRADIUS 2.5f
#define LARGERADIUS 10.0f

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
 *  Alex note: separate detection and collision response
 */

#include <iostream>
void PhysicsEngine::applyPhysics(ServerObject *obj1, ServerObject *obj2) {
	//Collision checks/inter-object physics
	
	//1. get their positions types (eventually size as well?)
	Point_t first = obj1->getPhysicsModel()->ref->getPos();
	Point_t second = obj2->getPhysicsModel()->ref->getPos();

	//2. get radii of each object
	CollisionBox size1 = obj1->getPhysicsModel()->getColBox();	
	CollisionBox size2 = obj2->getPhysicsModel()->getColBox();

	float r1 = (size1 == CB_SMALL) ? SMALLRADIUS : LARGERADIUS;
	float r2 = (size2 == CB_SMALL) ? SMALLRADIUS : LARGERADIUS;

	//3. calculate the distance
	float distX = first.x - second.x;
	float distY = first.y - second.y;
	float distZ = first.z - second.z;
	
	//4. find if we collided
	bool collidedX = distX * distX < (r1 + r2) * (r1 + r2);
	bool collidedY = distY * distY < (r1 + r2) * (r1 + r2);
	bool collidedZ = distZ * distZ < (r1 + r2) * (r1 + r2);

	/*General principle: conservation of momentum 
		*	If we were to have this be absolutely accurate, we would need
		*   a gauge of elasticity for both objects. However since we don't
		*   have elasticity, we will instead use a difference of mass as 
		*   a factor.
		*
		*/


	//Dealing with collisions!
	if (collidedX)
	{
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

		//Now we have two tasks: set the positions of the objects appropriately (A) and 
		// setting the velocities appropriately (B)

		//A. Set the positions, which we'll base on the difference.
		//1. We need the difference that we need to make up. 
		Vec3f positionDif = Vec3f (r1+r2 - distX, r1+r2 - distY, r1+r2 - distZ);

		//2. the amounts that we need to move
		Vec3f move1 = (obj1->getPhysicsModel()->vel / (obj1->getPhysicsModel()->vel + obj2->getPhysicsModel()->vel)) * positionDif;
		Vec3f move2 = (obj2->getPhysicsModel()->vel / (obj1->getPhysicsModel()->vel + obj2->getPhysicsModel()->vel)) * positionDif;

		//3. do the movement
		PhysicsModel * a = obj1->getPhysicsModel();
		PhysicsModel * b = obj2->getPhysicsModel();
		obj1->getPhysicsModel()->ref->setPos(obj1->getPhysicsModel()->vel - move1);
		obj2->getPhysicsModel()->ref->setPos(obj2->getPhysicsModel()->vel - move2);
	/*	
		//B. Set the velocities
		obj1->getPhysicsModel()->vel.x = obj1->getPhysicsModel()->vel.x * direction;
		obj1->getPhysicsModel()->vel.y = obj1->getPhysicsModel()->vel.y * direction;
		obj1->getPhysicsModel()->vel.z = obj1->getPhysicsModel()->vel.z * direction;

		Vec3f firstMom = Vec3f( obj1->getPhysicsModel()->vel.x * obj1->getPhysicsModel()->mass,
			obj1->getPhysicsModel()->vel.y * obj1->getPhysicsModel()->mass,
			obj1->getPhysicsModel()->vel.z * obj1->getPhysicsModel()->mass);

		//I got to override an operator! :D
		moment = moment - firstMom;

		//Overriding crazy! Also storing the final velocity in it
		obj2->getPhysicsModel()->applyForce(moment);
		*/
	}

	if (collidedY)
	{


	} 

	if (collidedZ)
	{



	}


	if (collidedX || collidedY || collidedZ) 
	{
		//0. set acceleration
		obj1->getPhysicsModel()->accel = Vec3f(0, 0, 0);
		obj2->getPhysicsModel()->accel = Vec3f(0, 0, 0);
	}

}

