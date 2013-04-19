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
	// Configuration options
	gravity = CM::get()->find_config_as_float("GRAVITY_FORCE");
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

/*flatCollision: detects and adjusts for collisions between 
 *  an object and a wall.
 *
 *
 * Author: Bryan
 */
void PhysicsEngine::flatCollision(ServerObject * theObj, Frame * flat)
{
	PhysicsModel * obj = theObj->getPhysicsModel();
	//1. Figure out the axis along which this object is flat
	bool xCollide, yCollide, zCollide;

	xCollide = flat->getRot().x != 0;
	yCollide = flat->getRot().y == 0;
	zCollide = flat->getRot().z == 0;

	//now if colliding with the wall is relevant, then it's nonzero

	//2. figure out what side the object originally came from
	Point_t prev = obj->ref->getPos() - obj->vel;

	//3. now we have the previous position, we can see if there's a 
	//   change in the object's relation to this

	Point_t pos = obj->ref->getPos();
	if (xCollide && (
			(prev.x > flat->getPos().x && obj->ref->getPos().x <= flat->getPos().x)
			|| (prev.x < flat->getPos().x && obj->ref->getPos().x >= flat->getPos().x) 
			)
		)
	{
		obj->ref->setPos(Point_t(0, pos.y, pos.z));
		obj->onGround = true;
		obj->frictCoeff = GROUND_FRICTION;
		obj->accel.x = 0;
		if(obj->vel.x < 0) {
			obj->vel.x = 0;
		}
		pos = obj->ref->getPos();
	}

	if (yCollide && (
			(prev.y > flat->getPos().y && obj->ref->getPos().y <= flat->getPos().y)
			|| (prev.y < flat->getPos().y && obj->ref->getPos().y >= flat->getPos().y) 
			) 
		)
	{
		obj->ref->setPos(Point_t(pos.x, 0, pos.z));
		obj->onGround = true;
		obj->frictCoeff = GROUND_FRICTION;
		obj->accel.y = 0;
		if(obj->vel.y < 0) {
			obj->vel.y = 0;
		}
		pos = obj->ref->getPos();
	}

	if (zCollide && (
			(prev.z > flat->getPos().z && obj->ref->getPos().z <= flat->getPos().z)
			|| (prev.z < flat->getPos().z && obj->ref->getPos().z >= flat->getPos().z) 
			) 
		)
	{
		obj->ref->setPos(Point_t(pos.x, pos.y, 0));
		obj->onGround = true;
		obj->frictCoeff = GROUND_FRICTION;
		obj->accel.z = 0;
		if(obj->vel.z < 0) {
			obj->vel.z = 0;
		}
		pos = obj->ref->getPos();
	}
	//if the object was above and is now below

	return;
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

	float r1, r2;
	bool isWall1 = false, isWall2 = false;
	switch (size1) {
	case CB_SMALL: r1 = SMALLRADIUS;
		break;
	case CB_LARGE:
		r1 = LARGERADIUS;
		break;
	default: //it's a wall, so we can deal with it here
		if (size2 != CB_FLAT) {
			flatCollision(obj2, p1->ref);
		}
		return;
	}
	
	switch (size2) {
	case CB_SMALL: r2 = SMALLRADIUS;
		break;
	case CB_LARGE:
		r2 = LARGERADIUS;
		break;
	default: //it's a wall, so we can deal with it here
		if (size1 != CB_FLAT) {
			flatCollision(obj1, p2->ref);
		}
		return;
	}

	if (isWall1) {
		
	} else if (isWall2)
	{

	}
	//3. calculate the distance
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

